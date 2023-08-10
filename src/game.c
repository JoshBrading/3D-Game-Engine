#include <SDL.h>       


#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gfc_audio.h"
#include "gf3d_sprite.h"

#include "entity.h"
#include "static_entity.h"
#include "agumon.h"
#include "world.h"
#include "economy.h"
#include "network.h"
#include "network_server.h"
#include <SDL_ttf.h>

#include "gfc_vector.h"

void start_game();

int main( int argc, char* argv[] )
{
    int a;
    Uint8 validate = 0;
    for( a = 1; a < argc; a++ )
    {
        if( strcmp( argv[ a ], "-disable_validate" ) == 0 )
        {
            validate = 0;
        }
    }

    init_logger( "gf3d.log" );
    slog( "gf3d begin" );

    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d( 0.51f, 0.75f, 1.0f, 1.0f ),//background color
        0,                      //fullscreen
        validate                //validation
    );
    slog_sync();
    start_game();
    return 0;
}

void start_game()
{
    //int network_game = establish_connection( "127.0.0.1" );
    //if( !network_game )
    //{
    //    network_server_start();
    //}
    int done = 0;

    const Uint8* keys;
    Uint8 started = 0;
    //World* w;
    int mousex, mousey;
    float mouseFrame = 0;



    entity_system_init( 1024 );
    static_entity_system_init( 1024 );
    gf3d_sprite_manager_init( 32, gf3d_swapchain_get_chain_length(), gf3d_vgraphics_get_default_logical_device() );
    gfc_audio_init( 32, 1, 1, 4, true, false );



    //w = world_load( "config/world.json" );
    // main game loop
    slog( "World Loaded" );

    gf3d_camera_set_scale( vector3d( 1, 1, 1 ) );
    //gf3d_camera_set_position(vector3d(10, 0, 0));

    slog( "gf3d main loop begin" );
    slog_sync();

    Uint32 lastUpdate = 0;

    SDL_Color color_white = { 255, 255, 255 };
    SDL_Color color_red = { 255, 0, 0 };
    SDL_Color color_green = { 0, 255, 0 };
    SDL_Color color_blue = { 0, 0, 255 };
    SDL_Color color_cyan = { 255, 255, 0 };

    TTF_Init();

    //if( !network_game )
    //{
    //    textSurface = TTF_RenderText_Blended( font, "Server",  color_cyan);
    //}
    //else
    //{
    //    textSurface = TTF_RenderText_Blended( font, "Client", color_green );
    //}


    Entity* cube = entity_new();
    cube->model = gf3d_model_load("cube", 1);
    cube->hasPhysics = 1;
    vector3d_copy(cube->position, vector3d(10, 0, 0));

    Entity* cylinder = entity_new();
    cylinder->model = gf3d_model_load("cylinder", 1);
    vector3d_copy(cylinder->position, vector3d(0, 0, 0));

    Entity* cone = entity_new();
    cone->model = gf3d_model_load("cone", 1);
    vector3d_copy(cone->position, vector3d(-10, 0, 0));

    Entity* plane = entity_new();
    plane->model = gf3d_model_load("plane", 1);
    plane->scale = vector3d(20, 20, 20);
    vector3d_copy(plane->position, vector3d(0, 0, -10));

    gf3d_camera_set_position(vector3d(0, 40, 0));
    gf3d_camera_set_rotation(vector3d(0, 0, 0));

    float frames = 0;
    float ticks = 0;
    int start_time = SDL_GetTicks();
    while( !done )
    {

        slog_sync();

        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState( NULL ); // get the keyboard state for this frame
        SDL_GetMouseState( &mousex, &mousey );

        SDL_Event ev;

        SDL_PollEvent(&ev);

        if (ev.type == SDL_KEYDOWN)
        {
            if (ev.key.keysym.sym == SDLK_w) cube->position.y += 0.50f;
            if (ev.key.keysym.sym == SDLK_a) cube->position.x += -0.50f;
            if (ev.key.keysym.sym == SDLK_s) cube->position.y += -0.50f;
            if (ev.key.keysym.sym == SDLK_d) cube->position.x += 0.50f;
            if (ev.key.keysym.sym == SDLK_c) cube->position.z += 0.50f;
            if (ev.key.keysym.sym == SDLK_z) cube->position.z += -0.50f;

        }
        mouseFrame += 0.01;
        if( mouseFrame >= 16 )mouseFrame = 0;

        gf3d_camera_update_view();
        gf3d_camera_get_view_mat4( gf3d_vgraphics_get_view_matrix() );

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        gf3d_vgraphics_render_start();


            entity_think_all();
            entity_update_all();

            if( lastUpdate + 10 < SDL_GetTicks() ) // Fixed update, every 10ms
            {
                entity_update_fixed_all();
                entity_think_fixed_all();
                //static_entity_update_fixed_all( );
                //slog("Network: %i", network_game);

                //if( network_game )
                //{
                //    send_update();
                //}
                //else
                //{
                //    network_server_receive();
                //}


                lastUpdate = SDL_GetTicks();

            }
            static_entity_update_all();


            //3D draws
            //world_draw( w );
            entity_draw_all();
            static_entity_draw_all();


            //int now = SDL_GetTicks();
            //
            //int elapsed_time = now - start_time;
            //
            //float fps = 1000.0f / elapsed_time;
            //
            //start_time = now;
            //
            //char* text[16];
            //sprintf(text, "Frame: %.2f", fps);
            //
            //gf3d_sprite_free(textTest); // TODO: Modify the sprite texture in gf3d_sprite instead of freeing
            //textSurface = TTF_RenderText_Blended(font, text, color_white);
            //
            //textTest = gf3d_sprite_from_sdl_texture(textSurface, "test.png", textSurface->w, textSurface->h, 1);
            //
            ////2D draws
            //gf3d_sprite_draw(textTest, vector2d( 250, 100 ), vector2d( 1, 1 ), ( Uint32 )mouseFrame );
            //
            //sprintf(text, "Frame Time: %d ms", elapsed_time);
            //gf3d_sprite_free(frame_time_text); // TODO: Modify the sprite texture in gf3d_sprite instead of freeing
            //frame_time_surf = TTF_RenderText_Blended(font, text, color_white);
            //frame_time_text = gf3d_sprite_from_sdl_texture(textSurface, "test.png", textSurface->w, textSurface->h, 1);
            //
            //gf3d_sprite_draw(frame_time_text, vector2d(250, 120), vector2d(1, 1), (Uint32)mouseFrame);

        
        gf3d_vgraphics_render_end();
        if( keys[ SDL_SCANCODE_ESCAPE ] )done = 1; // exit condition

 // world_delete(w);


    }
    vkDeviceWaitIdle( gf3d_vgraphics_get_default_logical_device() );
    //cleanup
    gfc_sound_clear_all();
    slog( "gf3d program end" );
    slog_sync();
}

/*eol@eof*/
