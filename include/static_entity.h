#ifndef __STATIC_ENTITY_H__
#define __STATIC_ENTITY_H__

#include "gfc_matrix.h"
#include "gf3d_model.h"
#include "entity.h"

typedef struct StaticEntity_S
{
	Uint8       _inuse;     /* < keeps track of memory usage */
	Matrix4		modelMat;	/* < orientation matrix for the model */
	Model*		model;		/* < Pointer to the static entity model */
	char*		tag;		/* < tag to make it easier to know what entity is being interacted with */
	void       (*update)(struct StaticEntity_S* self); /**<pointer to the update function*/

	struct Entity_S* child;     /* <child entity (optional) */

	Vector3D scale;
	Vector3D position;
	Vector3D rotation;

}StaticEntity;

typedef struct
{
	StaticEntity*	static_entity_list;
	Uint32          static_entity_count;

}StaticEntityManager;


/**
 * @brief Returns a pointer to the static_entity_manager
 */
StaticEntityManager* static_entity_get_manager( );

/**
 * @brief initializes the static entity subsystem
 * @param maxEntities the limit on number of entities that can exist at the same time
 */
void static_entity_system_init( Uint32 maxEntities );

/**
 * @brief provide a pointer to a new static entity
 * @param filename of the model to load
 * @param position to spawn static entity
 * @param rotation to spawn static entity
 * @return NULL on error or a valid static entity pointer otherwise
 */
StaticEntity* static_entity_new( char* filename, Vector3D position, Vector3D rotation );

/**
 * @brief free a previously created static entity from memory
 * @param self the static entity in question
 */
void static_entity_free( StaticEntity* self );

/**
 * @brief Draw a static entity in the current frame
 * @param self the static entity in question
 */
void static_entity_draw( StaticEntity* self );

/**
 * @brief draw ALL active static entities
 */
void static_entity_draw_all( );

/**
 * @brief run the update functions for ALL active entities
 */
void static_entity_update_all( );

//StaticEntity* static_entity_get_by_tag( char* tag );
#endif // !__STATIC_ENTITY_H__
