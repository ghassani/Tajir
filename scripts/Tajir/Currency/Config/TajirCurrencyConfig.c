/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirCurrencyConfig - Data read from JSON configuration
 */
class TajirCurrencyConfig
{
	string base_object;
	string dynamic_object;
	int player_start_currency;
	ref array<string> objects;

	/**
	 * @brief      { function_description }
	 */
	void TajirCurrencyConfig()
	{
		objects = new array<string>;
		player_start_currency = TAJIR_DEFAULT_PLAYER_START_CURRENCY;
	}
	
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Serialize( Serializer s )
	{
		s.Write( base_object );
		s.Write( dynamic_object );
		s.Write( player_start_currency );
		s.Write( objects );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Deserialize( Serializer s )
	{
		s.Read( base_object );
		s.Read( dynamic_object );
		s.Read( player_start_currency );
		s.Read( objects );
	}
}