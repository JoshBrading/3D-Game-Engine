#include "simple_logger.h"
#include "projectile.h"
#include "particles.h"
#include "collision.h"

void projectile_think_fixed( Entity* self );
void projectile_think( Entity* self );

Uint32 lifetime;

Entity* projectile_new( Entity* parent, Vector3D position )
{
    Entity* ent = NULL;

    ent = entity_new( );
    if ( !ent )
    {
        slog( "UGH OHHHH, no agumon for you!" );
        return NULL;
    }

    ent->model = gf3d_model_load( "p_bullet" );
    ent->thinkFixed = projectile_think_fixed;
    ent->think = projectile_think;
    ent->tag = "proj";
    ent->parent = parent;
    vector3d_copy( ent->position, position );

    lifetime = SDL_GetTicks( ) + 1000;

    return ent;
}

void projectile_think_fixed( Entity* self )
{
    self->position.x += 0.1f; // assume projectile is moving positive X for now...
    //particle_new( self->position, 10 );

    if ( lifetime < SDL_GetTicks( ) )
    {
        entity_free( self );
    }  
    slog( "entity_killed" );
}

void projectile_think( Entity* self )
{
    Entity* ent = entity_get_closest( self, 10, 0, (char*)"player" );
    if ( !ent ) return;
    slog( "target found at pos: %f, %f, %f", ent->position.x, ent->position.y, ent->position.z );
    //if ( collision_check_distance( self, ent ) )
    //{
    //    ent->damage( ent, 15.0f, self->parent );
    //}
}
