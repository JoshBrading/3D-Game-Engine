#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"

#include "static_entity.h"

typedef struct
{
    StaticEntity*   static_entity_list;
    Uint32          static_entity_count;

}StaticEntityManager;

StaticEntityManager static_entity_manager = { 0 };

StaticEntity* static_entity_new( char* filename, Vector3D position, Vector3D rotation )
{
    int i;
    Matrix4 modelMat;

    for ( i = 0; i < static_entity_manager.static_entity_count; i++ )
    {
        if ( !static_entity_manager.static_entity_list[i]._inuse )// not used yet, so we can!
        {
            static_entity_manager.static_entity_list[i]._inuse = 1;
            gfc_matrix_identity( static_entity_manager.static_entity_list[i].modelMat );
            static_entity_manager.static_entity_list[i].scale.x = 1;
            static_entity_manager.static_entity_list[i].scale.y = 1;
            static_entity_manager.static_entity_list[i].scale.z = 1;

            static_entity_manager.static_entity_list[i].position = position;
            static_entity_manager.static_entity_list[i].rotation = rotation;

            static_entity_manager.static_entity_list[i].tag = "test";

            static_entity_manager.static_entity_list[i].model = gf3d_model_load( (char*)filename );

            //slog( "Entity Spawning XYZ: %f, %f, %f", position.x, position.y, position.z );

            return &static_entity_manager.static_entity_list[i];
        }
    }
    slog( "static_entity_new: no free space in the static_entity list" );
    return NULL;
}

void static_entity_system_close( )
{
    int i;
    for ( i = 0; i < static_entity_manager.static_entity_count; i++ )
    {
        static_entity_free( &static_entity_manager.static_entity_list[i] );
    }
    free( static_entity_manager.static_entity_list );
    memset( &static_entity_manager, 0, sizeof( StaticEntityManager ) );
    slog( "static_entity_system: closed" );
}

void static_entity_system_init( Uint32 maxEntities )
{
    static_entity_manager.static_entity_list = gfc_allocate_array( sizeof( StaticEntity ), maxEntities );
    if ( static_entity_manager.static_entity_list == NULL )
    {
        slog( "static_entity_system: failed to allocate entity list, cannot allocate ZERO entities" );
        return;
    }
    static_entity_manager.static_entity_count = maxEntities;
    atexit( static_entity_system_close );
    slog( "static_entity_system: initialized" );
}

void static_entity_free( StaticEntity* self )
{
    if ( !self )return;
    //MUST DESTROY
    gf3d_model_free( self->model );
    memset( self, 0, sizeof( StaticEntity ) );
}

void static_entity_draw( StaticEntity* self )
{
    if ( !self )return;
    gf3d_model_draw( self->model, self->modelMat );
}

void static_entity_draw_all( )
{
    int i;
    for ( i = 0; i < static_entity_manager.static_entity_count; i++ )
    {
        if ( !static_entity_manager.static_entity_list[i]._inuse )// not used yet
        {
            continue;// skip this iteration of the loop
        }
        static_entity_draw( &static_entity_manager.static_entity_list[i] );
    }
}

void static_entity_update( StaticEntity* self )
{
    if ( !self )return;
    // HANDLE ALL COMMON UPDATE STUFF
    //vector3d_add(self->velocity,self->acceleration,self->velocity);

    gfc_matrix_identity( self->modelMat );
    gfc_matrix_scale( self->modelMat, self->scale );
    
    gfc_matrix_rotate( self->modelMat, self->modelMat, self->rotation.z, vector3d( 0, 0, 1 ) );
    gfc_matrix_rotate( self->modelMat, self->modelMat, self->rotation.y, vector3d( 0, 1, 0 ) );
    gfc_matrix_rotate( self->modelMat, self->modelMat, self->rotation.x, vector3d( 1, 0, 0 ) );
    
    gfc_matrix_translate( self->modelMat, self->position );
    
    if ( self->update )self->update( self );
}

void static_entity_update_all( )
{
    int i;
    for ( i = 0; i < static_entity_manager.static_entity_count; i++ )
    {
        if ( !static_entity_manager.static_entity_list[i]._inuse )// not used yet
        {
            continue;// skip this iteration of the loop
        }
        static_entity_update( &static_entity_manager.static_entity_list[i] );
    }
}

StaticEntity* static_entity_get_by_tag( char* tag )
{
    for ( int i = 0; i < static_entity_manager.static_entity_count; i++ )
    {
        if ( static_entity_manager.static_entity_list[i].tag == tag )
        {
            return &static_entity_manager.static_entity_list[i];
        }
    }
    slog( "static_entity_get_tag: No entity with tag %s", tag );
    return NULL;
}