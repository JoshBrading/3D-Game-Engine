#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "entity.h"

/**
 * @brief Create a new enemy entity
 * @param filename of enemy
 * @param position where to spawn the enemy at
 * @return NULL on error, or an enemy entity pointer on success
 */
Entity* enemy_new( char* enemy, Vector3D position );


#endif