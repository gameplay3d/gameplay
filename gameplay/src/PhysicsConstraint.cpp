/*
 * PhysicsConstraint.cpp
 */

#include "PhysicsConstraint.h"

#include "Node.h"

namespace gameplay
{

PhysicsConstraint::PhysicsConstraint() : _constraint(NULL)
{
}

PhysicsConstraint::~PhysicsConstraint()
{
    if (_constraint)
    {
        SAFE_DELETE(_constraint);
    }
}

Vector3 PhysicsConstraint::midpoint(Node* a, Node* b)
{
    Vector3 tA, tB;
    a->getWorldMatrix().getTranslation(&tA);
    b->getWorldMatrix().getTranslation(&tB);
    
    Vector3 d(tA, tB);
    d.scale(0.5f);
    Vector3 c(tA);
    c.add(d);

    return c;
}

}
