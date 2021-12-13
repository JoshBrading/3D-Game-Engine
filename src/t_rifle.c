
#include "simple_logger.h"
#include "t_rifle.h"
#include "projectile.h"
#include "gfc_audio.h"


void tower_rifle_think( Entity* self );
void tower_rifle_get_target( Entity* self );
void tower_rifle_attack( Entity* self );
void tower_rifle_upgrade( Entity* self );

void t_rifle_damage( struct Entity_S* self, float damage, struct Entity_S* inflictor );
void t_rifle_onDeath( struct Entity_S* self );

int time;
Uint32 fireRateOld;
Bool boosted;
Entity* tower_rifle_new( Vector3D position )
{
    Entity* ent = NULL;

    ent = entity_new( );
    if ( !ent )
    {
        slog( "UGH OHHHH, no agumon for you!" );
        return NULL;
    }

    ent->model = gf3d_model_load( "t_rifle" );
    ent->baseFilename = "t_rifle";
    ent->thinkFixed = tower_rifle_think;
    ent->increaseTier = tower_rifle_upgrade;
    ent->damage = t_rifle_damage;
    ent->onDeath = t_rifle_onDeath;

    ent->tag = "p_bullet";
    ent->health = 100;
    ent->team = 0;
    ent->weaponTimeBetweenShots = 200;
    ent->weaponDamage = 25;
    vector3d_copy( ent->position, position );

    fireRateOld = ent->weaponTimeBetweenShots;

    ent-> attackSFX = gfc_sound_load("sfx/SFX_Lasers.mp3", 50.0f, 1);

    time = 0;
    return ent;   
}

void tower_rifle_think( Entity* self )
{
    tower_rifle_get_target( self );

    if ( self->target && time > self->weaponTimeBetweenShots )
    {
        tower_rifle_attack( self );

        float range = 3; // Damage a 3x3 radius
        float distance;

        for ( int i = 0; i < entity_get_manager( )->entity_count; i++ )
        {

            distance = vector3d_magnitude_between( self->position, entity_get_manager( )->entity_list[i].position );

            if ( distance < range )
            {
                Entity* ent = &entity_get_manager( )->entity_list[i];
                if ( ent->tag == "t_mechanic" )
                {
                    //    slog( "Health: %f", ent->health );
                    //slog( "heal" );
                    self->health =+ 1.0f;
                }
                else if ( ent->tag == "t_support" && !boosted )
                {
                    self->weaponTimeBetweenShots = self->weaponTimeBetweenShots / 2;
                    boosted = true;
                }
            }
            
        }


        time = 0;
    }
    time++;

    if ( self->health <= 0 )
    {
        t_rifle_onDeath( self );
    }
}

void tower_rifle_get_target( Entity* self )
{
    if ( !self )return;
    self->target = entity_get_in_row( self, 75, 0, "player" );
}

void tower_rifle_upgrade( Entity* self )
{
    if ( !self )return;
    self->tier++;
    self->weaponTimeBetweenShots = 100;
    self->model = gf3d_model_load( "t_rifle_t2" );
}

void tower_rifle_attack( Entity* self )
{
    if ( !self )return;
    //slog( "attack" );
    projectile_new( self, self->position, "p_bullet", self->weaponDamage );
    gfc_sound_play(self->attackSFX, 0, 100.0f, 1, 1);

}

void t_rifle_damage( Entity* self, float damage, Entity* inflictor )
{
     //slog( inflictor->tag );
     //slog( "health: %f", self->health );
    self->health = self->health - damage;
}

void t_rifle_onDeath( struct Entity_S* self )
{
    entity_free( self );
}

/*eol@eof*/
