-- Create an AIAgent for the box
_modelNode:setAgent(AIAgent.create())

-- Store state machine for agent
_stateMachine = _modelNode:getAgent():getStateMachine()

-- Register AI states
stateIdle = _stateMachine:addState("idle")

stateSpinning = _stateMachine:addState("spinning")
stateSpinning:addScriptCallback("update", "spinningUpdate")

stateSpinning = _stateMachine:addState("sliding")
stateSpinning:addScriptCallback("enter", "slidingEnter")
stateSpinning:addScriptCallback("exit", "slidingExit")

stateBouncing = _stateMachine:addState("bouncing")
stateBouncing:addScriptCallback("enter", "bouncingEnter")
stateBouncing:addScriptCallback("exit", "bouncingExit")

stateScale = _stateMachine:addState("scale")
stateScale:addScriptCallback("enter", "scaleEnter")
stateScale:addScriptCallback("exit", "scaleExit")

-- Set initial state
_stateMachine:setState("spinning")

-- Create animations
_slidingClip = _modelNode:createAnimation("sliding", Transform.ANIMATE_TRANSLATE(), 6, { 0, 250, 750, 1250, 1750, 2000 }, { 0,0,0, 2,0,0, 2,0,-4, -2,0,-4, -2,0,0, 0,0,0 }, Curve.LINEAR):getClip()
_slidingClip:setRepeatCount(AnimationClip.REPEAT_INDEFINITE())
_bouncingClip = _modelNode:createAnimation("bouncing", Transform.ANIMATE_TRANSLATE_Y(), 3, { 0, 500, 1000 }, { 0, 0.75, 0 }, Curve.CUBIC_IN_OUT):getClip()
_bouncingClip:setRepeatCount(AnimationClip.REPEAT_INDEFINITE())
_scaleClip = _modelNode:createAnimation("scale", Transform.ANIMATE_SCALE(), 3, { 0, 750, 1500 }, { 1,1,1, 2,2,2, 1,1,1 }, Curve.QUADRATIC_IN_OUT):getClip()
_scaleClip:setRepeatCount(AnimationClip.REPEAT_INDEFINITE())

-- Called by game.lua to toggle AI state
function toggleState()
    local state = _stateMachine:getActiveState():getId()
    if state == "spinning" then
        _stateMachine:setState("sliding")
    elseif state == "sliding" then
        _stateMachine:setState("bouncing")
    elseif state == "bouncing" then
        _stateMachine:setState("scale")
    elseif state == "scale" then
        _stateMachine:setState("idle")
    elseif state == "idle" then
        _stateMachine:setState("spinning")
    end
end

-- SPINNING state handlers
function spinningUpdate(agent, state, elapsedTime)
    _modelNode:rotateY(elapsedTime * math.rad(0.05))
end

-- SLIDING state handlers
function slidingEnter(agent, state)
    _slidingClip:play()
end

function slidingExit(agent, state)
    _slidingClip:pause()
end

-- BOUNCING state handlers
function bouncingEnter(agent, state)
    _bouncingClip:play()
end

function bouncingExit(agent, state)
    _bouncingClip:pause()
end

-- SCALE state handlers
function scaleEnter(agent, state)
    _scaleClip:play()
end

function scaleExit(agent, state)
    _scaleClip:pause()
end


