#ifndef __WORLD_H__
#define __WORLD_H__


#include "gfc_types.h"
#include "gfc_list.h"
#include "gfc_matrix.h"
#include "gf3d_vgraphics.h"
#include "gf3d_model.h"
#include "entity.h"

typedef struct
{
    Matrix4 modelMat;
    Model *worldModel;

    Uint32 network_mode;    // Used for making decisions about how to communicate with host and clients
                            // 0 = offline, 1 = host, 2 = client
    Uint32 maxRows;
    Uint32 maxCols;

    List *spawnList;        //entities to spawn
    List *entityList;       //entities that exist in the world
}World;

World *world_load(char *filename);

World* world_get( );

void world_draw(World *world);

void world_delete(World *world);

void world_set_camera( World* self, Vector3D position, Vector3D rotation );


void world_run_updates(World *world);

void world_add_entity(World *world,Entity *entity);

#endif
