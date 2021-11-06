
#include "simple_logger.h"
#include "tower.h"


void tower_think( Entity* self );
void tower_get_target( Entity* self );


Entity* tower_new( char tower, Vector3D position )
{
    Entity* ent = NULL;

    ent = entity_new( );
    if ( !ent )
    {
        slog( "UGH OHHHH, no agumon for you!" );
        return NULL;
    }

    ent->model = gf3d_model_load( "turret_base" );
    ent->think = tower_think;
    ent->team = 0;
    vector3d_copy( ent->position, position );
    return ent;
}


void tower_think( Entity* self )
{
    if ( !self )return;
    self->rotation.x += -0.002;

    tower_get_target( self );

}

void tower_get_target( Entity* self )
{
    if ( !self )return;
    self->target = entity_get_closest( self, 50, "pfjpefj", 0 );
}

/*eol@eof*/
