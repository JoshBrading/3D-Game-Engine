#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_types.h"

#include "world.h"
#include "static_entity.h"
#include "gf3d_camera.h"
static World* world = NULL;
World* world_load( char* filename )
{
    SJson* json, * wjson;
    //World* world= NULL;

    const char* modelName = NULL;
    world= gfc_allocate_array( sizeof( World ), 1 );
    if ( world== NULL )
    {
        slog( "failed to allocate data for the world" );
        return NULL;
    }
    json = sj_load( filename );
    if ( !json )
    {
        slog( "failed to load json file (%s) for the world data", filename );
        free( world);
        return NULL;
    }
    wjson = sj_object_get_value( json, "world" );
    if ( !wjson )
    {
        slog( "failed to find world object in %s world config", filename );
        free( world);
        sj_free( json );
        return NULL;
    }

    int valid_maxRows, valid_maxCols;
    valid_maxRows = sj_get_integer_value( sj_object_get_value( wjson, "rows" ), &world->maxRows );
    valid_maxCols = sj_get_integer_value( sj_object_get_value( wjson, "cols" ), &world->maxCols );
    const char* tile1 = sj_get_string_value( sj_object_get_value( wjson, "tile1_model" ) );
    const char* tile2 = sj_get_string_value( sj_object_get_value( wjson, "tile2_model" ) );

    slog( "World: Generating ( %i, %i )", world->maxRows, world->maxCols );

    StaticEntity* sEnt;
    char tag;
    int tileFlip = 1;
    int count = 0;
    for ( int row = 0; row < world->maxRows; row++ )
    {
        for ( int col = 0; col < world->maxCols; col++ )
        {
            if ( tileFlip > 0)
            {
                sEnt = static_entity_new( tile1, vector3d( col * 2, row * 2, 0 ), vector3d( 0, 0, 0 ) );
            }
            else
            {
                sEnt = static_entity_new( tile2, vector3d( col * 2, row * 2, 0 ), vector3d( 0, 0, 0 ) );
            }

            tileFlip = -tileFlip; 
            count++;
        }
    }

    slog( "World: Generating Complete." );

    world_set_camera( world, vector3d( 0, -10, 10 ), vector3d( -150, 0, 0 ) );
  
    sj_free( json );
    return world;
}

void world_set_camera( World* self, Vector3D position, Vector3D rotation )
{
    //Vector3D position;
    
    int midPoint = self->maxCols - 1;
    
    position.x = midPoint;

    rotation.x = rotation.x * M_PI / 180;
    rotation.y = rotation.y * M_PI / 180;
    rotation.z = rotation.z * M_PI / 180;

    gf3d_camera_set_position( position );
    gf3d_camera_set_rotation( rotation );
}

World* world_get( )
{
    return world;
}

void world_draw( World* world )
{
    if ( !world )return;
    if ( !world->worldModel )return;// no model to draw, do nothing
    gf3d_model_draw( world->worldModel, world->modelMat );
}

void world_delete( World* world )
{
    if ( !world )return;
    gf3d_model_free( world->worldModel );
    free( world );
}


//void world_run_updates(World *world);

//void world_add_entity(World *world,Entity *entity);


/*eol@eof*/
