#include "simple_logger.h"
#include "collision.h"
#include "simple_logger.h"


Uint8 collision_aabb_plane_test(ColliderAABB aabb, ColliderPlane plane)
{
	// Example data
	// Plane MIN (0, 0, -10) 
	// Plane MAX (20, 20, 0)
	// AABB MAX  (12, 2, -8)
	// AABB MIN  (10, 0, -10)

	if (aabb.max.x < plane.position.x) return 0;
	if (aabb.min.x > plane.position.x + plane.scale.x) return 0;
	if (aabb.max.y < plane.position.y) return 0;
	if (aabb.min.y > plane.position.y + plane.scale.y) return 0;
	if (aabb.max.z < plane.position.z) return 0;
	if (aabb.min.z > plane.position.z) return 0;

	return 1;

}

Uint8 collision_aabb_aabb_test(ColliderAABB a, ColliderAABB b)
{
	if (a.max.x < b.min.x) return 0;
	if (a.min.x > b.max.x) return 0;
	if (a.max.y < b.min.y) return 0;
	if (a.min.y > b.max.y) return 0;
	if (a.max.z < b.min.z) return 0;
	if (a.min.z > b.max.z) return 0;

	slog("Collision detected");

	return 1;
}

Bool collision_check_distance( Entity* self, Entity* other ) // This is not very precise but enough for my game currently
{
	if ( self->position.x > other->position.x - 0.1 && self->position.x < other->position.x + 0.1 )
	{
		if ( self->position.y > other->position.y - 0.1 && self->position.y < other->position.y + 0.1 )
		{
			return true;
		}
	}
	return false;
}