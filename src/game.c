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
#include "player.h"
#include "world.h"
#include "economy.h"
#include "network.h"
#include <SDL_ttf.h>

int main(int argc,char *argv[])
{
    int network_game = 0;//establish_connection( "127.0.0.1" );

    int done = 0;
    int a;
    Uint8 validate = 0;
    const Uint8 * keys;
    World *w;
    Sprite* mouse = NULL;
    Sprite* mouse2 = NULL;
    int mousex, mousey;
    float mouseFrame = 0;

    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }
    
    init_logger("gf3d.log");    
    slog("gf3d begin");

    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51f,0.75f,1.0f,1.0f),//background color
        0,                      //fullscreen
        validate                //validation
    );

	slog_sync();
    
    entity_system_init(1024);
    static_entity_system_init( 1024 );
    gf3d_sprite_manager_init(16, gf3d_swapchain_get_chain_length(), gf3d_vgraphics_get_default_logical_device());
    gfc_audio_init( 32, 1, 1, 4, true, false);

    mouse = gf3d_sprite_load("images/pointer.png", 32, 32, 16);
    mouse2 = gf3d_sprite_load("images/pointer2.png", 32, 32, 16);

    w = world_load("config/world.json");

    //for (a = 0; a < 10;a++)
    //{
    //    agumon_new(vector3d(a * 10 -50,0,0));
    //}
    // main game loop
	slog_sync();
    gf3d_camera_set_scale(vector3d(1,1,1));
    
    slog("gf3d main loop begin");
    player_new(vector3d(0,0,0), vector3d( -175, 0, -45 ) );

  //  system( "cls" );

    Uint32 coinUpdate = 0; 
    Uint32 lastUpdate = 0;

    const SDL_Rect* dstrect;
    SDL_Color color = { 255, 255, 255 };

    TTF_Init();

    SDL_Renderer* renderer = SDL_CreateRenderer(gf3d_vgraphics_get_window(), -1, 0);

    if (!renderer) slog("Render failed ");

    TTF_Font* font = TTF_OpenFont("fonts/arial.ttf" /*path*/, 32 /*size*/);
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Coins: 0", color);

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    //int width, height;
    //SDL_QueryTexture(textSurface, NULL, NULL, &width, &height);
    slog("Creating Text Texture");
    Sprite* textTest = gf3d_sprite_from_sdl_texture(textSurface, "test", textSurface->w, textSurface->h, 1);
    //slog("Width: %i Height: %i", width, height);

   // SDL_RenderCopy(renderer, textTexture, NULL, dstrect);

    int currentCoin = 0;
    while(!done)
    {
        

        SDL_PumpEvents( );   // update SDL's internal event structures
        keys = SDL_GetKeyboardState( NULL ); // get the keyboard state for this frame
        SDL_GetMouseState(&mousex, &mousey);

        mouseFrame += 0.01;
        if (mouseFrame >= 16)mouseFrame = 0;

        entity_think_all( );
        entity_update_all( );
        
        if ( lastUpdate + 10 < SDL_GetTicks( ) ) // Fixed update, every 10ms
        {
            entity_update_fixed_all( );
            entity_think_fixed_all( );
            //static_entity_update_fixed_all( );
            //slog("Network: %i", network_game);

            if (currentCoin != eco_get_coin()) {
                currentCoin = eco_get_coin();

                char* text[16];
                sprintf(text, "Coins: %d", currentCoin);

                gf3d_sprite_free(textTest); // TODO: Modify the sprite texture in gf3d_sprite instead of freeing
                textSurface = TTF_RenderText_Blended(font, text, color);
                textTest = gf3d_sprite_from_sdl_texture(textSurface, "test.png", textSurface->w, textSurface->h, 1);
            }

            //slog("Current: %i GetCoin(): %i", currentCoin, eco_get_coin());
            if ( coinUpdate > 50 )
            {
                //eco_add_coin( 10 );
                coinUpdate = 0;
            }

            //coinUpdate++;
            lastUpdate = SDL_GetTicks( );

        }
        static_entity_update_all( );
        gf3d_camera_update_view( );
        gf3d_camera_get_view_mat4( gf3d_vgraphics_get_view_matrix( ) );

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        gf3d_vgraphics_render_start( );

        //3D drawsd
        world_draw( w );
        entity_draw_all( );
        static_entity_draw_all( );
        //2D draws
        gf3d_sprite_draw(mouse, vector2d(mousex, mousey), vector2d(1, 1), (Uint32)mouseFrame);
        gf3d_sprite_draw(textTest, vector2d(100, 100), vector2d(1, 1), (Uint32)mouseFrame);
       

        gf3d_vgraphics_render_end( );
      

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
   // world_delete(w);
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    gfc_sound_clear_all();
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
