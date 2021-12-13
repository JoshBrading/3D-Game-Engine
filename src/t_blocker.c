
#include "simple_logger.h"
#include "t_blocker.h"
#include "projectile.h"


void tower_blocker_think( Entity* self );
void tower_blocker_get_target( Entity* self );
void tower_blocker_attack( Entity* self );
void tower_blocker_upgrade( Entity* self );

void t_blocker_damage( struct Entity_S* self, float damage, struct Entity_S* inflictor );
void t_blocker_onDeath( struct Entity_S* self );

int time;

Entity* tower_blocker_new( Vector3D position )
{
    Entity* ent = NULL;

    ent = entity_new( );
    if ( !ent )
    {
        slog( "UGH OHHHH, no agumon for you!" );
        return NULL;
    }

    ent->model = gf3d_model_load( "t_blocker", 1 );

    ent->thinkFixed = tower_blocker_think;
    ent->damage = t_blocker_damage;
    ent->onDeath = t_blocker_onDeath;
    ent->increaseTier = tower_blocker_upgrade;

    ent->team = 0;
    ent->health = 300;
    ent->weaponTimeBetweenShots = 150;



    vector3d_copy( ent->position, position );
    return ent;

    time = 0;
}

void t_blocker_damage( Entity* self, float damage, Entity* inflictor )
{
    // slog( inflictor->tag );
     //slog( "health: %f", self->health );
    self->health = self->health - damage;
}

void t_blocker_onDeath( struct Entity_S* self )
{
    entity_free( self );
}

void tower_blocker_think( Entity* self )
{
    if ( time > self->weaponTimeBetweenShots )
    {
        float range = 3; // Damage a 3x3 radius
        float distance;

        for ( int i = 0; i < entity_get_manager( )->entity_count; i++ )
        {

            distance = vector3d_magnitude_between( self->position, entity_get_manager( )->entity_list[i].position );

            if ( distance < range )
            {
                Entity* ent = &entity_get_manager( )->entity_list[i];
                if ( ent->tag == "t_mechanic" && self->health < 400)
                {
                    //    slog( "Health: %f", ent->health );
                    slog( "Health: %f", self->health );
                    self->health += 1.0f;
                }
            }

        }


        time = 0;
    }
    time++;


    if ( self->health < 0 )
    {
        t_blocker_onDeath( self );
    }
}

void tower_blocker_get_target( Entity* self )
{
    if ( !self )return;
    //self->target = entity_get_in_row( self, 75, 0, "player" );
}

void tower_blocker_upgrade( Entity* self )
{
    if ( !self )return;
    self->tier++;
    self->health += 300;
    self->model = gf3d_model_load( "t_blocker_t2", 1 );
}

void tower_blocker_attack( Entity* self )
{
    if ( !self )return;
    //slog( "attack" );
    //projectile_new( self, self->position, "p_grenade" );
}

/*eol@eof*/
