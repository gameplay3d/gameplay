
-- Move speed (m/s)
MOVE_SPEED_NORMAL = 1
MOVE_SPEED_FAST = 5

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
_moveFast = false

function camera_setActive(flag)
    _useScriptCamera = flag

    if _useScriptCamera then
        Game.getInstance():setMultiTouch(true)
        
        _scene = Scene.getScene()

        _cameraNode = _scene:getActiveCamera():getNode()

        -- Set initial camera angles
        local eulers = camera_quatToEuler(_cameraNode:getRotation())
        _yaw = eulers:y()
        _pitch = eulers:x()
    else
		-- Release scene and camera
		_scene = nil
		_cameraNode = nil
	end
end

function camera_setSpeed(normal, fast)
    MOVE_SPEED_NORMAL = normal
    MOVE_SPEED_FAST = fast
end

function camera_update(elapsedTime)
	
    if not _useScriptCamera then
        return
    end

    if USE_PHYSICS_CHARACTER then

        local char = _scene:findNode("camera"):getCollisionObject():asCharacter()
        local speed = MOVE_SPEED_NORMAL
        if _moveFast then
            speed = MOVE_SPEED_FAST
        end

        -- Forward motion
        if _moveFlags[MOVE_FORWARD] then
            char:setForwardVelocity(speed)
        elseif _moveFlags[MOVE_BACKWARD] then
            char:setForwardVelocity(-speed)
        else
            char:setForwardVelocity(0)
        end

        -- Strafing
        if _moveFlags[MOVE_LEFT] then
            char:setRightVelocity(-speed)
        elseif _moveFlags[MOVE_RIGHT] then
            char:setRightVelocity(speed)
        else
            char:setRightVelocity(0)
        end

    else

        -- Manual camera movement
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
            _move:x(-1)
        elseif _moveFlags[MOVE_RIGHT] then
            _move:x(1)
        end

        if not _move:isZero() then
            local speed = MOVE_SPEED_NORMAL
            if _moveFast then
                speed = MOVE_SPEED_FAST
            end

            _move:normalize():scale(secs * speed)

            camera_moveForward(_move:y());
            camera_moveRight(_move:x());
        end

    end
end

function camera_keyEvent(evt, key)

    if not _useScriptCamera then
        return
    end

    if evt == Keyboard.KEY_PRESS then
        if key == Keyboard.KEY_W or key == Keyboard.KEY_CAPITAL_W then
            _moveFlags[MOVE_FORWARD] = true
        elseif key == Keyboard.KEY_S or key == Keyboard.KEY_CAPITAL_S then
            _moveFlags[MOVE_BACKWARD] = true
        elseif key == Keyboard.KEY_A or key == Keyboard.KEY_CAPITAL_A then
            _moveFlags[MOVE_LEFT] = true
        elseif key == Keyboard.KEY_D or key == Keyboard.KEY_CAPITAL_D then
            _moveFlags[MOVE_RIGHT] = true
        elseif key == Keyboard.KEY_SHIFT then
            _moveFast = true
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
            _moveFast = false
        end
    end

end

function camera_touchEvent(evt, x, y, contactIndex)

    if not _useScriptCamera then
        return
    end

    if evt == Touch.TOUCH_PRESS then
        if contactIndex == 0 then
            _touch:set(x, y)
        elseif contactIndex == 1 then
            _moveFlags[MOVE_FORWARD] = true
        elseif contactIndex == 2 then
            _moveFast = true
        end
    elseif evt == Touch.TOUCH_RELEASE then
        if contactIndex == 1 then
            _moveFlags[MOVE_FORWARD] = false
        elseif contactIndex == 2 then
            _moveFast = false
        end
    elseif evt == Touch.TOUCH_MOVE then
    	if contactIndex == 0 then
	        _delta:set(x - _touch:x(), y - _touch:y())
	        _touch:set(x, y)
	        _pitch = _pitch - math.rad(_delta:y() * 0.5)
	        _yaw = _yaw - math.rad(_delta:x() * 0.5)
	        _cameraNode:setRotation(Quaternion.identity())
	        _cameraNode:rotateY(_yaw)
	        _cameraNode:rotateX(_pitch)
	    end
    end

end

function camera_moveForward(by)
    local v = _cameraNode:getForwardVector()
    v:normalize():scale(by)
    _cameraNode:translate(v)
end

function camera_moveRight(by)
    local v = _cameraNode:getRightVector()
    v:normalize():scale(by)
    _cameraNode:translate(v)
end

function camera_quatToEuler(quat)
    local qx = quat:x()
    local qy = quat:y()
    local qz = quat:z()
    local qw = quat:w()
    local qx2 = qx * qx
    local qy2 = qy * qy
    local qz2 = qz * qz
    local qw2 = qw * qw

    local rotx = 0
    local roty = 0
    local rotz = 0

    if (qx*qy + qz*qw) == 0.5 then
        rotx = 0
        roty = 2 * math.atan2(qx, qw)
    elseif (qx*qy + qz*qw) == -0.5 then
        rotx = 0
        roty = -2 * math.atan2(qx, qw)
    else
        rotx = math.atan2(2*qx*qw-2*qy*qz , 1 - 2*qx2 - 2*qz2)
        roty = math.atan2(2*qy*qw-2*qx*qz , 1 - 2*qy2 - 2*qz2)
    end

    rotz = math.asin(2*qx*qy + 2*qz*qw)

    return Vector3.new(rotx, roty, rotz)
end
