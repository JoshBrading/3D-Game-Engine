#include "simple_logger.h"
#include "projectile.h"
#include "particles.h"
#include "collision.h"

void projectile_think_fixed( Entity* self );
void projectile_think( Entity* self );

void projectile_bullet( Entity* self );
void projectile_grenade( Entity* self );


Uint32 lifetime;

Entity* projectile_new( Entity* parent, Vector3D position, char* tag, float damage )
{
    Entity* ent = NULL;

    ent = entity_new( );
    if ( !ent )
    {
        slog( "UGH OHHHH, no agumon for you!" );
        return NULL;
    }
    //slog( "test: %f", damage );
    ent->model = gf3d_model_load( tag, 1 );
    ent->baseFilename = tag;
    ent->thinkFixed = projectile_think_fixed;
    ent->think = projectile_think;
    ent->tag = tag;
    ent->parent = parent;
    ent->weaponDamage = damage;
    ent->team = 1; // Use this for stupid stuff later 
    vector3d_copy( ent->position, position );

    lifetime = SDL_GetTicks( ) + 2000;

    return ent;
}

void projectile_think_fixed( Entity* self )
{
    if ( !self )return;
    if ( self->tag == "p_bullet" )
    {
        projectile_bullet( self );
    }
    else if ( self->tag == "p_grenade" )
    {
        projectile_grenade( self );
    }
    
    //particle_new( self->position, 10 );

    if ( lifetime < SDL_GetTicks( ) )
    {
        entity_free( self );
    }  
}

void projectile_bullet( Entity* self )
{
    if ( !self )return;
    self->position.x += 0.1f; // assume projectile is moving positive X for now...
}

void projectile_grenade( Entity* self )
{
    if ( !self )return;
    self->position.x += 0.1f; // assume projectile is moving positive X for now...

    if ( self->position.z < 1.0f && self->team )
    {
        self->position.z += 0.05f;
    }
    else if ( self->position.z < 0.001 )
    {
        self->team = 1;
    }
    else
    {
        self->team = 0;
        self->position.z += -0.05f;
    }
}

void projectile_think( Entity* self )
{
    if ( !self )return;

    if ( self->tag == "p_bullet" )
    {
        Entity* ent = entity_get_closest( self, 10, 0, (char*)"player" );
        if ( !ent ) return;
        if ( collision_check_distance( self, ent ) )
        {
            ent->damage( ent, self->weaponDamage, self->parent );
            entity_free( self );
        }
    }
    else if ( self->tag == "p_grenade" )
    {
        float range = 3; // Damage a 3x3 radius
        float distance;

        Entity* ent = entity_get_closest( self, 10, 0, (char*)"player" );
        if ( !ent ) return;
        if ( collision_check_distance( self, ent ) )
        {
            for ( int i = 0; i < entity_get_manager( )->entity_count; i++ )
            {
                if ( self != &entity_get_manager( )->entity_list[i] && entity_get_manager( )->entity_list[i].team != 0 )
                {
                    distance = vector3d_magnitude_between( self->position, entity_get_manager( )->entity_list[i].position );

                    if ( distance < range )
                    {
                        entity_get_manager( )->entity_list[i].damage( &entity_get_manager( )->entity_list[i], 20, self->parent );
                    }
                }
            }
          //  slog( "damage done----------------" );
            entity_free( self );
        }


    }
}
