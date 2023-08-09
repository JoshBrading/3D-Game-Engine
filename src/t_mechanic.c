
#include "simple_logger.h"
#include "t_mechanic.h"
#include "projectile.h"


void tower_mechanic_think( Entity* self );
void tower_mechanic_get_target( Entity* self );
void tower_mechanic_attack( Entity* self );
void tower_mechanic_upgrade( Entity* self );


void t_mechanic_damage( struct Entity_S* self, float damage, struct Entity_S* inflictor );
void t_mechanic_onDeath( struct Entity_S* self );

int time;

Entity* tower_mechanic_new( Vector3D position )
{
    Entity* ent = NULL;

    ent = entity_new( );
    if ( !ent )
    {
        slog( "UGH OHHHH, no agumon for you!" );
        return NULL;
    }

    ent->model = gf3d_model_load( "t_mechanic", 1 );

    ent->thinkFixed = tower_mechanic_think;
    ent->damage = t_mechanic_damage;
    ent->onDeath = t_mechanic_onDeath;
    ent->increaseTier = tower_mechanic_upgrade;
    ent->tag = "t_mechanic";

    ent->team = 0;
    ent->health = 100;
    ent->weaponTimeBetweenShots = 150;



    vector3d_copy( ent->position, position );
    return ent;

    time = 0;
}

void t_mechanic_damage( Entity* self, float damage, Entity* inflictor )
{
    // slog( inflictor->tag );
     //slog( "health: %f", self->health );
    self->health = self->health - damage;
}

void t_mechanic_onDeath( struct Entity_S* self )
{
    entity_free( self );
}

void tower_mechanic_think( Entity* self )
{    
     if (time > self->weaponTimeBetweenShots )
     {
         tower_mechanic_attack( self );
         time = 0;
     }
     time++;

     if ( self->health <= 0 )
     {
         self->onDeath( self );
     }
}

void tower_mechanic_get_target( Entity* self )
{
    if ( !self )return;
    //self->target = entity_get_in_row( self, 75, 0, "player" );
}

void tower_mechanic_upgrade( Entity* self )
{
    if ( !self )return;
    self->tier++;
    if ( self->tag == "t_mechanic" )
    {
        self->model = gf3d_model_load( "t_mechanic_t2", 1 );
    }
    else
    {
        self->model = gf3d_model_load( "t_support_t2", 1 );
    }
}

void tower_mechanic_attack( Entity* self )
{
    if ( !self )return;

   // float range = 3; // Damage a 3x3 radius
   // float distance;
   //
   // for ( int i = 0; i < entity_get_manager( )->entity_count; i++ )
   // {
   //     if ( self != &entity_get_manager( )->entity_list[i] && entity_get_manager( )->entity_list[i].team == 0 )
   //     {
   //         distance = vector3d_magnitude_between( self->position, entity_get_manager( )->entity_list[i].position );
   //
   //         if ( distance < range )
   //         {
   //             Entity* ent = &entity_get_manager( )->entity_list[i];
   //             if ( ent->health < 100 )
   //             {
   //             //    slog( "Health: %f", ent->health );
   //                 ent->health++;
   //             }
   //         }
   //     }
   // }
} 

/*eol@eof*/
