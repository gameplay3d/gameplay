
-- Move speed (m/s)
MOVE_SPEED_NORMAL = 50
MOVE_SPEED_FAST = 200

-- Move flags
MOVE_FORWARD = 1
MOVE_BACKWARD = 2
MOVE_RIGHT = 3
MOVE_LEFT = 4

_useScriptCamera = false
_forwardSpeed = 0
_sideSpeed = 0
_touch = Vector2.new()
_delta = Vector2.new()
_move = Vector2.new()
_moveFlags = { false, false, false, false }
_yaw = 0
_pitch = 0
_shiftDown = false

function setScene(scene)
    _scene = scene
    _cameraNode = scene:getActiveCamera():getNode()
end

function toggleCamera()
    _useScriptCamera = not _useScriptCamera
end

function initialize()
end

function finalize()
end

function update(elapsedTime)

    if not _useScriptCamera then
        return
    end

    local secs = elapsedTime / 1000.0

    _move:set(0,0)

    -- Forward motion
    if _moveFlags[MOVE_FORWARD] then
        _move:y(1)
    elseif _moveFlags[MOVE_BACKWARD] then
        _move:y(-1)
    end

    -- Strafing
    if _moveFlags[MOVE_LEFT] then
        _move:x(1)
    elseif _moveFlags[MOVE_RIGHT] then
        _move:x(-1)
    end

    if not _move:isZero() then
        local speed = MOVE_SPEED_NORMAL
        if _shiftDown then
            speed = MOVE_SPEED_FAST
        end

        _move:normalize():scale(secs * speed)

        moveForward(_move:y());
        moveRight(-_move:x());
    end
end

function render(elapsedTime)
end

function keyEvent(evt, key)
    if evt == Keyboard.KEY_PRESS then
        if key == Keyboard.KEY_W or key == Keyboard.KEY_CAPITAL_W then
            _moveFlags[MOVE_FORWARD] = true
        elseif key == Keyboard.KEY_S or key == Keyboard.KEY_CAPITAL_S then
            _moveFlags[MOVE_BACKWARD] = true
        elseif key == Keyboard.KEY_A or key == Keyboard.KEY_CAPITAL_A then
            _moveFlags[MOVE_LEFT] = true
        elseif key == Keyboard.KEY_D or key == Keyboard.KEY_CAPITAL_D then
            _moveFlags[MOVE_RIGHT] = true
        elseif key == Keyboard.KEY_C then
            _useScriptCamera = not _useScriptCamera
        elseif key == Keyboard.KEY_SHIFT then
            _shiftDown = true
        end
    elseif evt == Keyboard.KEY_RELEASE then
        if key == Keyboard.KEY_W or key == Keyboard.KEY_CAPITAL_W then
            _moveFlags[MOVE_FORWARD] = false
        elseif key == Keyboard.KEY_S or key == Keyboard.KEY_CAPITAL_S then
            _moveFlags[MOVE_BACKWARD] = false
        elseif key == Keyboard.KEY_A or key == Keyboard.KEY_CAPITAL_A then
            _moveFlags[MOVE_LEFT] = false
        elseif key == Keyboard.KEY_D or key == Keyboard.KEY_CAPITAL_D then
            _moveFlags[MOVE_RIGHT] = false
        elseif key == Keyboard.KEY_SHIFT then
            _shiftDown = false
        end
    end

    return false
end

function touchEvent(evt, x, y, contactIndex)

    if not _useScriptCamera then
        return
    end

    if evt == Touch.TOUCH_PRESS then
        _touch:set(x, y)
    elseif evt == Touch.TOUCH_RELEASE then
    elseif evt == Touch.TOUCH_MOVE then
        _delta:set(x - _touch:x(), y - _touch:y())
        _touch:set(x, y)
        _pitch = _pitch + -math.rad(_delta:y() * 0.5)
        _yaw = _yaw + math.rad(_delta:x() * 0.5)
        _cameraNode:setRotation(Quaternion.identity())
        _cameraNode:rotateY(-_yaw)
        _cameraNode:rotateX(_pitch)
    end

    return false
end

function moveForward(by)
    local v = _cameraNode:getForwardVector()
    v:normalize():scale(by)
    _cameraNode:translate(v)
end

function moveRight(by)
    local v = _cameraNode:getRightVector()
    v:normalize():scale(by)
    _cameraNode:translate(v)
end
