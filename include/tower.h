#pragma once
#ifndef __TOWER_H__
#define __TOWER_H__

#include "entity.h"

/**
 * @brief Create a new tower entity
 * @param position where to spawn the tower at
 * @param rotation to spawn the tower with
 * @return NULL on error, or a tower entity pointer on success
 */
Entity* tower_new( Vector3D position, Vector3D rotation );


#endif