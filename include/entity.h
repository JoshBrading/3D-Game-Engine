#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gfc_matrix.h"
#include "gf3d_model.h"

typedef struct Entity_S
{
	Uint8       _inuse;         /**<flag to keep track if this isntance is in use and should not be reassigned*/
	Model*		model;          /**<the 3d model for this entity*/
	Matrix4     modelMat;       /**<orientation matrix for the model*/
	void*		data;

}Entity;

Entity* entity_new( );

void entity_system_init( Uint32 maxEntities );

/**
 * @brief free an entity so it can be reused by the system
 * @param self the entity in question
 */
void entity_free( Entity* entity );

/**
* @brief queues up the entity to be drawn for the given draw command
* @param self the entity in question
* @param bufferFrame the current rending frame index
*/
void entity_draw( Entity* self, Uint32 bufferFrame, VkCommandBuffer commandBuffer );


/**
 * @brief draw ALL active entities
 * @param bufferFrame the current rending frame index
 */
void entity_draw_all( Uint32 bufferFrame, VkCommandBuffer commandBuffer );


/**
* @brief call think function for all active entities
* @param self the entity in question
*/
//void entity_think( Entity* self );

#endif