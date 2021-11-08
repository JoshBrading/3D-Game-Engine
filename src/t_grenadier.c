
#include "simple_logger.h"
#include "t_grenadier.h"
#include "projectile.h"


void tower_grenadier_think( Entity* self );
void tower_grenadier_get_target( Entity* self );
void tower_grenadier_attack( Entity* self );
void tower_grenadier_upgrade( Entity* self );

static int time;

Entity* tower_grenadier_new( Vector3D position )
{
    Entity* ent = NULL;

    ent = entity_new( );
    if ( !ent )
    {
        slog( "UGH OHHHH, no agumon for you!" );
        return NULL;
    }

    ent->model = gf3d_model_load( "t_grenadier" );
    ent->thinkFixed = tower_grenadier_think;
    ent->increaseTier = tower_grenadier_upgrade;
    ent->team = 0;
    ent->health = 100;
    ent->weaponTimeBetweenShots = 150;
    vector3d_copy( ent->position, position );
    time = 0;
    return ent;
}

void tower_grenadier_think( Entity* self )
{
    tower_grenadier_get_target( self );

    if ( self->target && time > self->weaponTimeBetweenShots )
    {
        tower_grenadier_attack( self );
        time = 0;
    }
    time++;
}

void tower_grenadier_get_target( Entity* self )
{
    if ( !self )return;
    self->target = entity_get_in_row( self, 75, 0, "player" );
}

void tower_grenadier_upgrade( Entity* self )
{
    if ( !self )return;
    self->tier++;
    self->weaponDamage = self->weaponDamage * 2;
    self->weaponTimeBetweenShots = (int)(self->weaponTimeBetweenShots * 0.75f);
    self->model = gf3d_model_load( "t_grenadier_t2" );
}

void tower_grenadier_attack( Entity* self )
{
    if ( !self )return;
    //slog( "attack" );
    projectile_new( self, self->position, "p_grenade", 20 );
}

/*eol@eof*/
