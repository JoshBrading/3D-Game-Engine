
#include "simple_logger.h"
#include "tower.h"
#include "projectile.h"


void tower_think( Entity* self );
void tower_get_target( Entity* self );
void tower_attack( Entity* self );


Entity* tower_new( char* tower, Vector3D position )
{
    Entity* ent = NULL;

    ent = entity_new( );
    if ( !ent )
    {
        slog( "UGH OHHHH, no agumon for you!" );
        return NULL;
    }

    ent->model = gf3d_model_load( tower );
    ent->think = tower_think;
    ent->team = 0;
    vector3d_copy( ent->position, position );
    return ent;
}

void tower_think( Entity* self )
{
    tower_get_target( self );

    if ( self->target )
    {
        tower_attack( self );
    }
}

void tower_get_target( Entity* self )
{
    if ( !self )return;
    self->target = entity_get_closest( self, 50, 0, "player" );
}

void tower_upgrade( Entity* self )
{
    self->tier++;
}

void tower_attack( Entity* self )
{
    projectile_new( self, self->position );
}

/*eol@eof*/
