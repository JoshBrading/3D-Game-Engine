#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "entity.h"

typedef struct {
	Vector3D	max;
	Vector3D	min;
	Vector3D	scale;
}ColliderAABB;

typedef struct {
	Vector3D	position;
	float		radius;
}ColliderSphere;

typedef struct {
	Vector3D	position;
	Vector2D	scale;
}ColliderPlane;

typedef enum
{
	COL_TOP,
	COL_BOTTOM,
	COL_LEFT,
	COL_RIGHT,
	COL_NULL
}CollisionSide;

typedef struct {
	CollisionSide side;
}CollisionInfo;

/*
* @brief check for collision between a cube and plane
* @param ColliderCube
* @param ColliderPlane
* @return CollisionInfo
*/
Uint8 collision_aabb_plane_test(ColliderAABB aabb, ColliderPlane plane);

Uint8 collision_aabb_aabb_test(ColliderAABB a, ColliderAABB b);

/*
* @brief check for collision between 2 entities
* @param self entity
* @param entity to check for collision against
*/
Bool collision_check_distance( Entity* self, Entity* other );

#endif // !__COLLISION_H__
