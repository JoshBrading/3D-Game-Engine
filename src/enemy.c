
#include "simple_logger.h"
#include "enemy.h"
#include "economy.h"


void enemy_think( Entity* self );
void enemy_think_fixed( Entity* self );
void enemy_get_target( Entity* self );
void enemy_attack( Entity* self );
void damage( struct Entity_S* self, float damage, struct Entity_S* inflictor );
void onDeath( struct Entity_S* self );


Entity* enemy_new( char* enemy, Vector3D position )
{
    Entity* ent = NULL;

    ent = entity_new( );
    if ( !ent )
    {
        slog( "UGH OHHHH, no agumon for you!" );
        return NULL;
    }

    ent->model = gf3d_model_load( enemy , 20);
    ent->think = enemy_think;
    ent->thinkFixed = enemy_think_fixed;
    ent->damage = damage;
    ent->onDeath = onDeath;
    ent->health = 100;
    ent->weaponTimeBetweenShots = 100;
    ent->weaponDamage = 25;
    ent->team = 1;
    ent->tag = "enemy";
    vector3d_copy( ent->position, position );
    return ent;
}


void enemy_think( Entity* self )
{
    enemy_get_target( self );
    if ( self->health <= 0 )
    {
        onDeath( self );
    }
}

int time = 0;
void enemy_think_fixed( Entity* self )
{
    if ( !self->target )
    {
        self->position.x += -0.01;
    }
    else if ( time > self->weaponTimeBetweenShots )
    {
        enemy_attack( self );
        time = 0;
    }

    if( time % 5 == 0 )
    {
        if( self->model->currentFrame < self->model->frames - 1 )
            self->model->currentFrame += 1;
        else
            self->model->currentFrame = 0;
    }

    if( self->position.x < 0 )
    {
        slog( "You Lose!" );
    }

    time++;
}

void enemy_get_target( Entity* self )
{
    if ( !self )return;
    self->target = entity_get_in_row( self, 1, 1, "player" );
}

void damage( Entity* self, float damage, Entity* inflictor )
{
   // slog( inflictor->tag );
    //slog( "health: %f", self->health );
    self->health = self->health - damage;
}

void onDeath( struct Entity_S* self )
{
    eco_add_coin( 100 );
    entity_free( self );
}

void enemy_attack( Entity* self )
{
    if ( !self )return;
    if ( !self->target->damage )return;
    self->target->damage( self->target, self->weaponDamage, self );
}

/*eol@eof*/
