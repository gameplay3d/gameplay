#include "Base.h"
#include "PhysicsCollisionShape.h"

namespace gameplay
{

PhysicsCollisionShape::PhysicsCollisionShape(Type type, btCollisionShape* shape)
	: _type(type), _shape(shape)
{
	memset(&_shapeData, 0, sizeof(_shapeData));
}

PhysicsCollisionShape::PhysicsCollisionShape(const PhysicsCollisionShape& copy)
{
	// hidden
}

PhysicsCollisionShape::~PhysicsCollisionShape()
{
	if (_shape)
	{
		// Cleanup shape-specific cached data
		switch (_type)
		{
		case SHAPE_MESH:
			if (_shapeData.meshData)
			{
				SAFE_DELETE_ARRAY(_shapeData.meshData->vertexData);
				for (unsigned int i = 0; i < _shapeData.meshData->indexData.size(); i++)
				{
					SAFE_DELETE_ARRAY(_shapeData.meshData->indexData[i]);
				}
				SAFE_DELETE(_shapeData.meshData);
			}
			break;
		case SHAPE_HEIGHTFIELD:
			if (_shapeData.heightfieldData)
			{
				SAFE_DELETE_ARRAY(_shapeData.heightfieldData->heightData);
				SAFE_DELETE(_shapeData.heightfieldData);
			}
			break;
		}

		// Free the bullet shape
		SAFE_DELETE(_shape);
	}
}

PhysicsCollisionShape::Type PhysicsCollisionShape::getType() const
{
	return _type;
}

PhysicsCollisionShape::Definition::Definition()
	: isExplicit(false), centerAbsolute(false)
{
	memset(&data, 0, sizeof(data));
}

PhysicsCollisionShape::Definition::~Definition()
{
	switch (type)
	{
	case PhysicsCollisionShape::SHAPE_HEIGHTFIELD:
		SAFE_RELEASE(data.heightfield);
		break;

	case PhysicsCollisionShape::SHAPE_MESH:
		SAFE_RELEASE(data.mesh);
		break;
	}
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::box()
{
	Definition d;
	d.type = SHAPE_BOX;
	d.isExplicit = false;
	d.centerAbsolute = false;
	return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::box(const Vector3& extents, const Vector3& center, bool absolute)
{
	Definition d;
	d.type = SHAPE_BOX;
	d.data.boxExtents = extents;
	d.data.boxCenter = center;
	d.isExplicit = true;
	d.centerAbsolute = absolute;
	return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::sphere()
{
	Definition d;
	d.type = SHAPE_SPHERE;
	d.isExplicit = false;
	d.centerAbsolute = false;
	return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::sphere(float radius, const Vector3& center, bool absolute)
{
	Definition d;
	d.type = SHAPE_SPHERE;
	d.data.sphereRadius = radius;
	d.data.sphereCenter = center;
	d.isExplicit  = true;
	d.centerAbsolute = absolute;
	return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::capsule()
{
	Definition d;
	d.type = SHAPE_CAPSULE;
	d.isExplicit = false;
	d.centerAbsolute = false;
	return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::capsule(float radius, float height, const Vector3& center, bool absolute)
{
	Definition d;
	d.type = SHAPE_CAPSULE;
	d.data.capsuleRadius = radius;
	d.data.capsuleHeight = height;
	d.data.capsuleCenter = center;
	d.isExplicit = true;
	d.centerAbsolute = absolute;
	return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::heightfield(Image* image)
{
	image->addRef();

	Definition d;
	d.type = SHAPE_HEIGHTFIELD;
	d.data.heightfield = image;
	d.isExplicit = true;
	d.centerAbsolute = false;
	return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::mesh(Mesh* mesh)
{
	mesh->addRef();

	Definition d;
	d.type = SHAPE_MESH;
	d.data.mesh = mesh;
	d.isExplicit = true;
	d.centerAbsolute = false;
	return d;
}

}
