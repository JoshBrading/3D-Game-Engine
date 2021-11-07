
#include "simple_logger.h"
#include "enemy.h"


void enemy_think( Entity* self );
void enemy_get_target( Entity* self );


Entity* enemy_new( char* enemy, Vector3D position )
{
    Entity* ent = NULL;

    ent = entity_new( );
    if ( !ent )
    {
        slog( "UGH OHHHH, no agumon for you!" );
        return NULL;
    }

    ent->model = gf3d_model_load( enemy );
    ent->think = enemy_think;
    ent->team = 0;
    vector3d_copy( ent->position, position );
    return ent;
}


void enemy_think( Entity* self )
{
    enemy_get_target( self );
}

void enemy_get_target( Entity* self )
{
    if ( !self )return;
    self->target = entity_get_closest( self, 50, "pfjpefj", 0 );
}

/*eol@eof*/
