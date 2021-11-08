#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"
#include "economy.h"

int playerCoins = 0;
int playerPower = 0;

void updateCount( );

void eco_add_coin( int coins )
{
	playerCoins += coins;
	updateCount( );
}

int eco_get_coin( )
{
	return playerCoins;
	updateCount( );
}

void eco_add_power( int power )
{
	playerPower += power;
	updateCount( );
}

int eco_get_power( )
{
	return playerPower;
	updateCount( );
}

void updateCount( )
{
	system( "cls" );
	slog( "Coins: %i", eco_get_coin( ) );
}