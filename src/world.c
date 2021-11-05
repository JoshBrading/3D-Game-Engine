#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_types.h"

#include "world.h"
#include "static_entity.h"

World* world_load( char* filename )
{
    SJson* json, * wjson;
    World* world= NULL;

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
    //modelName = sj_get_string_value( sj_object_get_value( wjson, "model" ) );
    //if ( modelName )
    //{
    //    getch( );
    //    world->worldModel = gf3d_model_load( (char*)modelName );
    //    gfc_matrix_identity( world->modelMat );
    //    gfc_matrix_scale(
    //        world->modelMat,
    //        vector3d( 10, 10, 10 )
    //    );
    //    gfc_matrix_translate(
    //        world->modelMat,
    //        vector3d( 0, 0, -20 )
    //    );
    //}
    //else
    //{
    //    slog( "world data (%s) has no model", filename );
    //}

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
                sEnt = static_entity_new( tile1, vector3d( row * 2, col * 2, 0 ), vector3d( 0, 0, 0 ) );
            }
            else
            {
                sEnt = static_entity_new( tile2, vector3d( row * 2, col * 2, 0 ), vector3d( 0, 0, 0 ) );
            }
            //s_ent->tag = tag;
            //slog( "Static Ent Count: %c", s_ent->tag );
            //slog( "Row: %i, Col %i", row, col );
            //slog( "Entity Read XYZ: %i, %i, %i", s_ent->position.x, s_ent->position.y, s_ent->position.z );

            tileFlip = -tileFlip;
            count++;
        }
    }

    slog( "World: Generating Complete." );

    //getch( );
    //StaticEntity* sEntTarget = static_entity_get_by_tag( (char*)"test" );
    //
    //if ( sEntTarget != NULL )
    //{
    //    slog( "Entity Read: %s", sEntTarget->tag );
    //}

    Entity* tower;
    Entity* enemy;
    Entity* enemy2;

    tower = tower_new( vector3d( 0, 0, 0 ), vector3d( 0, 0, 0 ) );
    enemy = entity_new( );
    vector3d_copy( enemy->position, vector3d( 0, 10, 0 ) );
    enemy->tag = "example";
    enemy->team = 1;
    
    enemy2 = entity_new( );
    vector3d_copy( enemy2->position, vector3d( 5, 9, 0 ) );
    enemy2->tag = "another";
    enemy2->team = 1;
  
    sj_free( json );
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
