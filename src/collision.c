#include "simple_logger.h"
#include "collision.h"

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