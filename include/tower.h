#pragma once
#ifndef __TOWER_H__
#define __TOWER_H__

#include "entity.h"

/**
 * @brief Create a new tower entity
 * @param filename of tower
 * @param position where to spawn the tower at
 * @return NULL on error, or a tower entity pointer on success
 */
Entity* tower_new( char* tower, Vector3D position );

/*
* @bief Upgrade a tower
* @param tower to upgrade
*/
void tower_upgrade( Entity* self );


#endif