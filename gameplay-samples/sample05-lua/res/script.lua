-- This lua script file represents an exact lua implementation translation of sample00-mesh with a box instead of a duck.

DEG_TO_RAD = 0.0174532925

function initialize()
    -- Display splash screen for at least 1 second.
    ScreenDisplayer.start("drawSplash", 1000);

    _touched = false
    _touchX = 0

    -- Load font
    _font = Font.create("res/arial40.gpb")

    -- Load mesh/scene from file
    local bundle = Bundle.create("res/box.gpb")
    _scene = bundle:loadScene()

    -- Get the box node
    _modelNode = _scene:findNode("box")

    -- Bind the material to the model
    _modelNode:getModel():setMaterial("res/box.material")

    -- Find the light node
    local lightNode = _scene:findNode("directionalLight1")

    -- Bind the light node's direction into the box material.
    _modelNode:getModel():getMaterial():getParameter("u_lightDirection"):bindValue(lightNode, "&Node::getForwardVectorView");

    -- Update the aspect ratio for our scene's camera to match the current device resolution
    local game = Game.getInstance()
    _scene:getActiveCamera():setAspectRatio(game:getWidth() / game:getHeight())

    -- Create the grid and add it to the scene.
    local model = createGridModel()
    _scene:addNode("grid"):setModel(model)

    ScreenDisplayer.finish()
end

function update(elapsedTime)
    if not _touched then
        _modelNode:rotateY(0.5 * 0.0174532925)
    end
end

-- Avoid allocating new objects every frame.
textColor = Vector4.new(0, 0.5, 1, 1)

function render(elapsedTime)
    -- Clear the color and depth buffers.
    Game.getInstance():clear(Game.CLEAR_COLOR_DEPTH, Vector4.zero(), 1.0, 0)

    -- Visit all the nodes in the scene, drawing the models/mesh.
    _scene:visit("drawScene");
    
    -- Draw the fps.
    buffer = string.format("%u", Game.getInstance():getFrameRate());
    _font:start()
    _font:drawText(buffer, 5, 1, textColor, _font:getSize())
    _font:finish()
end

function finalize()
    _font = nil
    _scene = nil
end

function drawScene(node)
    model = node:getModel()
    if model then
        model:draw()
    end
    return true
end

function drawSplash()
    local game = Game.getInstance()
    game:clear(Game.CLEAR_COLOR_DEPTH, Vector4.new(0, 0, 0, 1), 1.0, 0)
    local batch = SpriteBatch.create("res/logo_powered_white.png")
    batch:start()
    batch:draw(game:getWidth() * 0.5, game:getHeight() * 0.5, 0.0, 512.0, 512.0, 0.0, 1.0, 1.0, 0.0, Vector4.one(), true)
    batch:finish()
    batch = nil
end

function keyEvent(evt, key)
    --local str = string.format("key: %s, %s!\n\n", evt, key)
    --printError(str)

    if evt == Keyboard.KEY_PRESS then
        if key == Keyboard.KEY_ESCAPE then
            Game.getInstance():exit()
        end
    end
end

function touchEvent(evt, x, y, contactIndex)
    --local str = string.format("touch: %s, %d, %d, %d!\n\n", evt, x, y, contactIndex)
    --printError(str)

    if evt == Touch.TOUCH_PRESS then
        _touched = true
        _touchX = x
    elseif evt == Touch.TOUCH_RELEASE then
        _touched = false
        _touchX = 0
    elseif evt == Touch.TOUCH_MOVE then
        local deltaX = x - _touchX
        _touchX = x
        _modelNode:rotateY(deltaX * 0.5 * DEG_TO_RAD)
    end    
end

function createGridModel()
    local lineCount = 41
    local pointCount = lineCount * 4
    local verticesSize = pointCount * (3 + 3) -- (3 (position(xyz) + 3 color(rgb))

    local vertices = {};
    local gridLength = math.floor(lineCount / 2)

    local value = -gridLength
    local i = 1 -- NOTE: Lua arrays start at 1!
    while i < verticesSize do
        -- Default line color is dark grey
        local color = Vector4.new(0.3, 0.3, 0.3, 1.0)

        -- Every 10th line is brighter grey
        if math.floor(value + 0.5) % 10 == 0 then
            color:set(0.45, 0.45, 0.45, 1.0)
        end

        -- The Z axis is blue
        if value == 0.0 then
            color:set(0.15, 0.15, 0.7, 1.0)
        end

        -- Build the lines
        vertices[i] = value
        i = i + 1
        vertices[i] = 0.0
        i = i + 1
        vertices[i] = -gridLength
        i = i + 1
        vertices[i] = color:x()
        i = i + 1
        vertices[i] = color:y()
        i = i + 1
        vertices[i] = color:z()
        i = i + 1

        vertices[i] = value
        i = i + 1
        vertices[i] = 0.0
        i = i + 1
        vertices[i] = gridLength
        i = i + 1
        vertices[i] = color:x()
        i = i + 1
        vertices[i] = color:y()
        i = i + 1
        vertices[i] = color:z()
        i = i + 1

        -- The X axis is red
        if value == 0.0 then
            color:set(0.7, 0.15, 0.15, 1.0)
        end
        vertices[i] = -gridLength
        i = i + 1
        vertices[i] = 0.0
        i = i + 1
        vertices[i] = value
        i = i + 1
        vertices[i] = color:x()
        i = i + 1
        vertices[i] = color:y()
        i = i + 1
        vertices[i] = color:z()
        i = i + 1

        vertices[i] = gridLength
        i = i + 1
        vertices[i] = 0.0
        i = i + 1
        vertices[i] = value
        i = i + 1
        vertices[i] = color:x()
        i = i + 1
        vertices[i] = color:y()
        i = i + 1
        vertices[i] = color:z()
        i = i + 1

        value = value + 1.0
    end

    elements = { 
        VertexFormat.Element.new(VertexFormat.POSITION, 3),
        VertexFormat.Element.new(VertexFormat.COLOR, 3)
    }
    mesh = Mesh.createMesh(VertexFormat.new(elements, 2), pointCount, false)
    if mesh == nil then
        return nil
    end
    mesh:setPrimitiveType(Mesh.LINES)
    mesh:setVertexData(vertices, 0, pointCount)

    model = Model.create(mesh)
    model:setMaterial("res/grid.material")
    mesh:release()
    return model
end