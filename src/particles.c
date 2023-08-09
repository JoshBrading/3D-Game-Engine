#include "simple_logger.h"
#include "particles.h"

void particle_update_fixed( StaticEntity* self );

int lifetime;

StaticEntity* particle_new( Vector3D position, int pLifetime )
{
	StaticEntity* sEnt = static_entity_new( "p_bullet", position, vector3d( 0, 0, 0 ) );

	sEnt->updateFixed = particle_update_fixed;

	lifetime = pLifetime + SDL_GetTicks( );

	return sEnt;

}

void particle_update_fixed( StaticEntity* self )
{
	self->position.z += -0.01;

	//if ( lifetime < SDL_GetTicks( ) )
	//{
	//	static_entity_free( self );
//	}


}