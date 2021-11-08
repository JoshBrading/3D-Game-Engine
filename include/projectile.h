#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "entity.h"

Entity* projectile_new( Entity* parent, Vector3D position, char* tag, float damage );

#endif // !__PROJECTILE_H__
