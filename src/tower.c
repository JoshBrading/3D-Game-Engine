
#include "simple_logger.h"
#include "tower.h"
#include "projectile.h"


void tower_think( Entity* self );
void tower_get_target( Entity* self );
void tower_attack( Entity* self );

int time;

Entity* tower_new( char* tower, char* weapon, Vector3D position )
{
    Entity* ent = NULL;

    ent = entity_new( );
    if ( !ent )
    {
        slog( "UGH OHHHH, no agumon for you!" );
        return NULL;
    }

    ent->model = gf3d_model_load( tower, 1 );
    ent->thinkFixed = tower_think;
    ent->tag = weapon;
    ent->team = 0;
    vector3d_copy( ent->position, position );
    return ent;

    time = 0;
}

void tower_think( Entity* self )
{
    tower_get_target( self );

    if ( self->target && time > self->weaponTimeBetweenShots )
    {
        tower_attack( self );
        time = 0;
    }
    time++;
}

void tower_get_target( Entity* self )
{
    if ( !self )return;
    self->target = entity_get_in_row( self, 75, 0, "player" );
}

void tower_upgrade( Entity* self )
{
    self->tier++;
}

void tower_attack( Entity* self )
{
    //projectile_new( self, self->position, self->tag );
}

/*eol@eof*/
