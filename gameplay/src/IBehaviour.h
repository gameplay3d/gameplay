#ifndef IBEHAVIOUR_H
#define IBEHAVIOUR_H


class IBehaviour
{
public:
    // TODO remove if no needed
    IBehaviour(){}
    virtual ~IBehaviour(){}

    /**
     * Callback for update event.
     */
    virtual void Update(float elapsedTime){}

    // TODO add other callbacks (example: Awake, Start, PhysicsUpdate, CollisionStart, CollisionEnded ...)

};

#endif // IBEHAVIOUR_H
