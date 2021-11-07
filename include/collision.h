#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "entity.h"

/*
* @brief check for collision between 2 entities
* @param self entity
* @param entity to check for collision against
*/
Bool collision_check_distance( Entity* self, Entity* other );

#endif // !__COLLISION_H__
