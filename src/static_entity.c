#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"

#include "static_entity.h"

typedef struct
{
    StaticEntity* entity_list;
    Uint32  entity_count;

}StaticEntityManager;

static StaticEntityManager static_entity_manager = { 0 };

void static_entity_system_close( )
{
    int i;
    for ( i = 0; i < static_entity_manager.entity_count; i++ )
    {
        entity_free( &static_entity_manager.entity_list[i] );
    }
    free( static_entity_manager.entity_list );
    memset( &static_entity_manager, 0, sizeof( StaticEntityManager ) );
    slog( "static_entity_system: closed" );
}

void static_entity_system_init( Uint32 maxEntities )
{
    static_entity_manager.entity_list = gfc_allocate_array( sizeof( StaticEntity ), maxEntities );
    if ( static_entity_manager.entity_list == NULL )
    {
        slog( "static_entity_system: failed to allocate entity list, cannot allocate ZERO entities" );
        return;
    }
    static_entity_manager.entity_count = maxEntities;
    atexit( static_entity_system_close );
    slog( "static_entity_system: initialized" );
}