DEG_TO_RAD = 0.0174532925

function init()
    -- Display splash screen for at least 1 second.
    ScreenDisplayer.start("drawSplash", 1000);

    _touched = false
    _touchX = 0

    -- Load font
    _font = Font.create("res/arial40.gpb")

    -- Load mesh/scene from file
    local bundle = Bundle.create("res/duck.gpb")
    _scene = bundle:loadScene()
    bundle = nil

    -- Get the duck node
    _modelNode = _scene:findNode("duck")

    -- Bind the material to the model
    _modelNode:getModel():setMaterial("res/duck.material")

    -- Find the light node
    local lightNode = _scene:findNode("directionalLight1")

    -- Bind the light node's direction into duck's material.
    _modelNode:getModel():getMaterial():getParameter("u_lightDirection"):bindValue(lightNode, "&Node::getForwardVectorView");

    -- Update the aspect ratio for our scene's camera to match the current device resolution
    local game = Game.getInstance()
    _scene:getActiveCamera():setAspectRatio(game:getWidth() / game:getHeight())

    -- Create the grid and add it to the scene.
    --local model = createGridModel()
    --_scene:addNode("grid"):setModel(model)
    --model = nil

    ScreenDisplayer.finish()
end

function update(elapsedTime)
    if not _touched then
        _modelNode:rotateY(0.5 * 0.0174532925)
    end
end

function render(elapsedTime)
    -- Clear the color and depth buffers.
    Game.getInstance():clear(Game.CLEAR_COLOR_DEPTH, Vector4.zero(), 1.0, 0)

    -- Visit all the nodes in the scene, drawing the models/mesh.
    _scene:visit("drawScene");
    
    -- Draw the fps.
    buffer = string.format("%u", Game.getInstance():getFrameRate());
    _font:begin()
    _font:drawText(buffer, 5, 1, Vector4.new(0, 0.5, 1, 1), _font:getSize())
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
    batch:begin()
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