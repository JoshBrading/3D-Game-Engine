#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_types.h"
#include "gf3d_model.h"
#include "static_entity.h"


typedef struct Entity_S
{
    Uint8               _inuse;     /* <keeps track of memory usage */
    Matrix4             modelMat;   /* <orientation matrix for the model */
    Model*              model;      /* <pointer to the entity model to draw  (optional) */

    void               (*think)(struct Entity_S* self); /* <pointer to the think function */
    void               (*thinkFixed)(struct Entity_S* self); /* <pointer to the think fixed function */

    void               (*update)(struct Entity_S* self); /* <pointer to the update function */
    void               (*updateFixed)(struct Entity_S* self); /* <pointer to the update fixed function */

    void               (*damage)(struct Entity_S* self, float damage, struct Entity_S* inflictor); /* <pointer to the damage function */
    void               (*onDeath)(struct Entity_S* self); /* <pointer to an funciton to call when the entity dies */

    Vector3D            position;
    Vector3D            velocity;

    Vector3D            scale;
    Vector3D            rotation;

    Uint32              health;     /* <entity dies when it reaches zero */
    Uint32              tier;       /* <current upgrade tier, edit with cfg */

    struct Entity_S*    parent;     /* <parent entity (optional) */
    struct Entity_S*    target;     /* <entity to target for weapons / ai*/
    struct Static_Entity_S* staticParent; /* < static parent for towers */

    char*               tag;		/* < tag to make it easier to know what entity is being interacted with */
    Uint8               team;       /* < team to make it easier to know what entity is */
    float               viewRange;  /* < max distance the entity can "see" */

    void*               data;       /* <IF an entity needs to keep track of extra data, we can do it here */
}Entity;

typedef struct
{
    Entity* entity_list;
    Uint32  entity_count;

}EntityManager;

/**
 * @brief initializes the entity subsystem
 * @param maxEntities the limit on number of entities that can exist at the same time
 */
void entity_system_init( Uint32 maxEntities );

/**
 * @brief provide a pointer to a new empty entity
 * @return NULL on error or a valid entity pointer otherwise
 */
Entity* entity_new( );

/**
 * @brief free a previously created entity from memory
 * @param self the entity in question
 */
void entity_free( Entity* self );


/**
 * @brief Draw an entity in the current frame
 * @param self the entity in question
 */
void entity_draw( Entity* self );

/**
 * @brief draw ALL active entities
 */
void entity_draw_all( );

/**
 * @brief Call an entity's think function if it exists
 * @param self the entity in question
 */
void entity_think( Entity* self );

/**
 * @brief run the think functions for ALL active entities
 */
void entity_think_all( );

/**
 * @brief run the think functions for ALL active entities at a fixed rate
 */
void entity_think_fixed_all( );

/**
 * @brief run the update functions for ALL active entities
 */
void entity_update_all( );

/**
 * @brief Fixed update runs the update functions for all entities at a fixed rate
 */
void entity_update_fixed_all( );

/*
* @brief returns the closest entity to the player
* @param self the entity in question
* @param range to search for entities
* @param teamMask exclude team from search
* @param tagMask exclude tag from search
* @return NULL on error or a valid entity pointer on success
*/
Entity* entity_get_closest( Entity* self, float range, Uint8 teamMask, char tagMask );

EntityManager* entity_get_manager( );
#endif
