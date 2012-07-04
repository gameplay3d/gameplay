function init()
    --printError("init!\n\n")

    _touched = false

    --[ [
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
    --local lightNode = _scene:findNode("directionalLight1")

    -- Bind the light node's direction into duck's material.
    --_modelNode:getModel():getMaterial():getParameter("u_lightDirection"):bindValue(lightNode, &Node::getForwardVectorView);

    -- Update the aspect ratio for our scene's camera to match the current device resolution
    local game = Game.getInstance()
    _scene:getActiveCamera():setAspectRatio(game:getWidth() / game:getHeight())

    -- Create the grid and add it to the scene.
    --local model = createGridModel()
    --_scene:addNode("grid"):setModel(model)
    --model = nil
    --]]
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
