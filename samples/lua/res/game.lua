-- This lua script file represents a lua implementation translation of sample00-mesh with a box instead of a duck.

function initialize()
    -- Display splash screen for at least 1 second.
    ScreenDisplayer.start("drawSplash", 1000)

    _touched = false
    _touchX = 0

    -- Load font
    _font = Font.create("res/arial40.gpb")

    -- Load mesh/scene from file
    _scene = Scene.load("res/box.gpb")

    -- Get the box node
    _modelNode = _scene:findNode("box")

    -- Bind the material to the model
    _modelNode:getModel():setMaterial("res/box.material")

    -- Find the light node
    local lightNode = _scene:findNode("directionalLight1")

    -- Bind the light node's direction into the box material.
    _modelNode:getModel():getMaterial():getParameter("u_lightDirection"):bindValue(lightNode, "&Node::getForwardVectorView")

    -- Update the aspect ratio for our scene's camera to match the current device resolution
    local game = Game.getInstance()
    _scene:getActiveCamera():setAspectRatio(game:getWidth() / game:getHeight())

    -- Create the grid and add it to the scene.
    local model = createGridModel()
    _scene:addNode("grid"):setModel(model)

    -- Load the AI script
    game:getScriptController():loadScript("res/ai.lua")

    ScreenDisplayer.finish()
end

function update(elapsedTime)
end

-- Avoid allocating new objects every frame.
textColor = Vector4.new(0, 0.5, 1, 1)

function render(elapsedTime)
    -- Clear the color and depth buffers.
    Game.getInstance():clear(Game.CLEAR_COLOR_DEPTH, Vector4.zero(), 1.0, 0)

    -- Visit all the nodes in the scene, drawing the models/mesh.
    _scene:visit("drawScene")

    -- Draw the fps.
    local buffer = string.format("%u\n%s", Game.getInstance():getFrameRate(), _stateMachine:getActiveState():getId())
    _font:start()
    _font:drawText(buffer, 5, 1, textColor, _font:getSize())
    _font:finish()
end

function finalize()
    _font = nil
    _scene = nil
end

function drawScene(node)
    local model = node:getModel()
    if model then
        model:draw()
    end
    return true
end

function drawSplash()
    local game = Game.getInstance()
    game:clear(Game.CLEAR_COLOR_DEPTH, 0, 0, 0, 1, 1.0, 0)
    local batch = SpriteBatch.create("res/logo_powered_white.png")
    batch:start()
    batch:draw(game:getWidth() * 0.5, game:getHeight() * 0.5, 0.0, 512.0, 512.0, 0.0, 1.0, 1.0, 0.0, Vector4.one(), true)
    batch:finish()
end

function keyEvent(evt, key)
    if evt == Keyboard.KEY_PRESS then
        if key == Keyboard.KEY_ESCAPE then
            Game.getInstance():exit()
        end
    end
end

function touchEvent(evt, x, y, contactIndex)
    if evt == Touch.TOUCH_PRESS then
        _touchTime = Game.getAbsoluteTime()
        _touched = true
        _touchX = x
    elseif evt == Touch.TOUCH_RELEASE then
        _touched = false
        _touchX = 0

        -- Basic emulation of tap to change state
        if (Game.getAbsoluteTime() - _touchTime) < 200 then
            toggleState()
        end
    elseif evt == Touch.TOUCH_MOVE then
        local deltaX = x - _touchX
        _touchX = x
        _modelNode:rotateY(math.rad(deltaX * 0.5))
    end    
end

function createGridModel()
    local lineCount = 41
    local pointCount = lineCount * 4
    local verticesSize = pointCount * (3 + 3)

    local vertices = {}
    local gridLength = math.floor(lineCount / 2)
    local value = -gridLength

    while #vertices + 1 < verticesSize do
        -- Default line color is dark grey
        local red, green, blue = 0.3, 0.3, 0.3

        -- Every 10th line is brighter grey
        if math.floor(value + 0.5) % 10 == 0 then
            red, green, blue = 0.45, 0.45, 0.45
        end
        -- The Z axis is blue
        if value == 0 then
            red, green, blue = 0.15, 0.15, 0.7
        end

        -- Build the lines
        vertices[#vertices+1] = value
        vertices[#vertices+1] = 0.0
        vertices[#vertices+1] = -gridLength
        vertices[#vertices+1] = red
        vertices[#vertices+1] = green
        vertices[#vertices+1] = blue

        vertices[#vertices+1] = value
        vertices[#vertices+1] = 0.0
        vertices[#vertices+1] = gridLength
        vertices[#vertices+1] = red
        vertices[#vertices+1] = green
        vertices[#vertices+1] = blue

        -- The X axis is red
        if value == 0.0 then
            red, green, blue = 0.7, 0.15, 0.15
        end
        vertices[#vertices+1] = -gridLength
        vertices[#vertices+1] = 0.0
        vertices[#vertices+1] = value
        vertices[#vertices+1] = red
        vertices[#vertices+1] = green
        vertices[#vertices+1] = blue

        vertices[#vertices+1] = gridLength
        vertices[#vertices+1] = 0.0
        vertices[#vertices+1] = value
        vertices[#vertices+1] = red
        vertices[#vertices+1] = green
        vertices[#vertices+1] = blue

        value = value + 1.0
    end

    local elements = { 
        VertexFormat.Element.new(VertexFormat.POSITION, 3),
        VertexFormat.Element.new(VertexFormat.COLOR, 3)
    }
    local mesh = Mesh.createMesh(VertexFormat.new(elements, 2), pointCount, false)
    if mesh == nil then
        return nil, "Error creating grid mesh."
    end
    mesh:setPrimitiveType(Mesh.LINES)
    mesh:setVertexData(vertices, 0, pointCount)

    local model = Model.create(mesh)
    model:setMaterial("res/grid.material")
    return model
end
