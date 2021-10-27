#ifndef __STATIC_ENTITY_H__
#define __STATIC_ENTITY_H__

#include "gfc_matrix.h"
#include "gf3d_model.h"


typedef struct
{
	Matrix4 modelMat;
	Model* model;

	Vector3D position;
	Vector3D rotation;

}StaticEntity;

/**
 * @brief initializes the static entity subsystem
 * @param maxEntities the limit on number of entities that can exist at the same time
 */
 void static_entity_system_init( Uint32 maxEntities );

/**
 * @brief provide a pointer to a new empty entity
 * @return NULL on error or a valid static entity pointer otherwise
 */
//StaticEntity* static_entity_new( );

/**
 * @brief free a previously created static entity from memory
 * @param self the static entity in question
 */
 //void static_entity_free( StaticEntity* self );


/**
 * @brief Draw an static entity in the current frame
 * @param self the static entity in question
 */
 //void static_entity_draw( StaticEntity* self );

/**
 * @brief draw ALL active entities
 */
 //void static_entity_draw_all( );


#endif // !__STATIC_ENTITY_H__
