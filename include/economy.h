#ifndef __ECONOMY_H__
#define __ECONOMY_H__

/**
 * @brief Add int coins to the players balance
 * @param int coins to add
 */
void eco_add_coin( int coins );

/**
 * @brief Returns the players current balance
 */
int eco_get_coin( );

/**
 * @brief Add int power to the players power
 * @param int power to add
 */
void eco_add_power( int power );

/**
 * @brief Returns the players current balance
 */
int eco_get_power( );

#endif // !__ECONOMY_H__
