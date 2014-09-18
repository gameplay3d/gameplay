
local this = nil
local animations = { }

function attached(node)
    this = node

    -- Create an AIAgent for the box
    node:setAgent(AIAgent.create())

    -- Get state machine
    local stateMachine = node:getAgent():getStateMachine()

    -- Register AI states
    stateMachine:addState("idle")
    stateMachine:addState("spin")
    stateMachine:addState("slide")
    stateMachine:addState("bounce")
    stateMachine:addState("scale")

    -- Set initial state
    stateMachine:setState("spin")

    -- Create animations, storing them in a table keyed on state name
    animations["slide"] = node:createAnimation("slide", Transform.ANIMATE_TRANSLATE(), 6, { 0, 250, 750, 1250, 1750, 2000 }, { 0,0,0, 2,0,0, 2,0,-4, -2,0,-4, -2,0,0, 0,0,0 }, Curve.LINEAR):getClip()
    animations["slide"]:setRepeatCount(AnimationClip.REPEAT_INDEFINITE())
    animations["bounce"] = node:createAnimation("bounce", Transform.ANIMATE_TRANSLATE_Y(), 3, { 0, 500, 1000 }, { 0, 0.75, 0 }, Curve.CUBIC_IN_OUT):getClip()
    animations["bounce"]:setRepeatCount(AnimationClip.REPEAT_INDEFINITE())
    animations["scale"] = node:createAnimation("scale", Transform.ANIMATE_SCALE(), 3, { 0, 750, 1500 }, { 1,1,1, 2,2,2, 1,1,1 }, Curve.QUADRATIC_IN_OUT):getClip()
    animations["scale"]:setRepeatCount(AnimationClip.REPEAT_INDEFINITE())
end

function stateEnter(node, state)
    local clip = animations[state:getId()]
    if clip then
        clip:play()
    end
end

function stateExit(node, state)
    local clip = animations[state:getId()]
    if clip then
        clip:pause()
    end
end

function stateUpdate(node, state, t)
    if state:getId() == "spin" then
        node:rotateY(t * math.rad(0.05))
    end
end

-- Put into the global table so it can be called by game.lua to toggle AI state
function _G.toggleState()
    local stateMachine = this:getAgent():getStateMachine()
    local state = stateMachine:getActiveState():getId()
    if state == "spin" then
        stateMachine:setState("slide")
    elseif state == "slide" then
        stateMachine:setState("bounce")
    elseif state == "bounce" then
        stateMachine:setState("scale")
    elseif state == "scale" then
        stateMachine:setState("idle")
    elseif state == "idle" then
        stateMachine:setState("spin")
    end
end
