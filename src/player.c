#include "simple_logger.h"
#include "gfc_types.h"

#include "gf3d_camera.h"
#include "player.h"
#include "world.h"
#include "static_entity.h"
#include "entity.h"
#include "tower.h"


void player_think_fixed( Entity* self );
void player_update_fixed( Entity* self );
int tower_at_pos( Vector3D position );
void set_cursor_color( Entity* self );

Model* red;
Model* green;
Model* blue;

World* world;
                     // 0 = Cannot spawn
int cursorState = 1; // 1 = Can spawn
                     // 2 = Can upgrade
int lastUpdate = 0;
Vector3D targetPos;

Entity* player_new( Vector3D position, Vector3D rotation )
{
    Entity* ent = NULL;

    ent = entity_new( );
    if ( !ent )
    {
        slog( "UGH OHHHH, no player for you!" );
        return NULL;
    }

    red = gf3d_model_load( "red_box" );
    green = gf3d_model_load( "green_box" );
    blue = gf3d_model_load( "blue_box" );

    ent->model = green;
    ent->tag = "player";
    ent->thinkFixed = player_think_fixed;
    ent->updateFixed = player_update_fixed;
    vector3d_copy( ent->position, position );
    

    world = world_get( );
    lastUpdate = SDL_GetTicks( );
    return ent;
}

void player_think_fixed( Entity* self )
{
    SDL_Event ev;

    SDL_PollEvent( &ev );
    
    if ( ev.type == SDL_KEYDOWN && ev.key.repeat == 0 )
    {
        if ( ev.key.keysym.sym == SDLK_w ) targetPos.y += 2.0f;
        if ( ev.key.keysym.sym == SDLK_a ) targetPos.x += -2.0f;
        if ( ev.key.keysym.sym == SDLK_s ) targetPos.y += -2.0f;
        if ( ev.key.keysym.sym == SDLK_d ) targetPos.x += 2.0f;
        if ( ev.key.keysym.sym == SDLK_e )
        {
            if ( cursorState == 1 )
            {
                Entity* tower = tower_new( "turret_base", targetPos );
            } 
            if ( cursorState == 2 )
            {
                tower_upgrade( &entity_get_manager( )->entity_list[tower_at_pos( targetPos, "player" )] );
            }  
        }

        set_cursor_color( self );
    }
}

void player_update_fixed( Entity* self ) // TODO: Clean this up, This is very gross
{
    if ( !self )return;
    if ( targetPos.y < 0 ) targetPos.y = 0.0f;
    if ( targetPos.x < 0 ) targetPos.x = 0.0f;

    if ( targetPos.x > world->maxCols * 2 - 2 ) targetPos.x = (float)world->maxCols * 2 - 2;
    if ( targetPos.y > world->maxRows * 2 - 2 ) targetPos.y = (float)world->maxRows * 2 - 2;

    if ( self->position.x < targetPos.x )           self->position.x += 0.1f;
    if ( self->position.x > targetPos.x + 0.0001f)  self->position.x -= 0.1f;
    if ( self->position.y < targetPos.y )           self->position.y += 0.1f;
    if ( self->position.y > targetPos.y + 0.0001f ) self->position.y -= 0.1f;

    if ( lastUpdate + 400 > SDL_GetTicks( ) )
    {
        self->position.z += 0.005;
    }
    else if ( lastUpdate + 800 > SDL_GetTicks( ) )
    {
        self->position.z -= 0.005;

        if ( self->position.z < 0 ) self->position.z = 0; // I shouldnt need this if I start with a higher Z right??
    }
    else
    {
        lastUpdate = SDL_GetTicks( );
    }
}

void set_cursor_color( Entity* self )
{
    if ( tower_at_pos( targetPos, "player" ) == NULL )
    {
        self->model = green;
        cursorState = 1;
    }
    else if ( entity_get_manager( )->entity_list[tower_at_pos( targetPos, "player" )].tier < 1 )
    {
        //slog( entity_get_manager( )->entity_list[tower_at_pos( targetPos, "player" )].tag );
        self->model = blue;
        cursorState = 2;
    }
    else
    {
        self->model = red;
        cursorState = 0;
    }
}

// Return the index of tower at position
int tower_at_pos( Vector3D position, char tagMask ) // Move to tower.c
{
    int i;
    for ( i = 0; i < entity_get_manager( )->entity_count; i++ )
    {
        if ( entity_get_manager( )->entity_list[i].tag != tagMask )
        {
            if ( entity_get_manager( )->entity_list[i].position.x == position.x && entity_get_manager( )->entity_list[i].position.y == position.y )
            {
                return i;
            }
        }
    }
    return NULL;
}
