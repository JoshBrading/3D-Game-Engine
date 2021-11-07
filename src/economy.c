#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"
#include "economy.h"

int playerCoins = 0;
int playerPower = 0;

void eco_add_coins( int coins )
{
	playerCoins += coins;
}

int eco_get_coins( )
{
	return playerCoins;
}

void eco_add_power( int power )
{
	playerPower += power;
}

int eco_get_power( )
{
	return playerPower;
}