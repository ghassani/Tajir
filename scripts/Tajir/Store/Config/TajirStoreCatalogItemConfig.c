/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirStoreCatalogItemConfig - Data read from JSON configuration
 */
class TajirStoreCatalogItemConfig
{
	int 	item_id;
	bool 	can_sell;
	bool 	can_buy;
	int 	quantity;
	int 	restock_quantity;

	/**
	 * @brief      { function_description }
	 */
	void TajirStoreCatalogItemConfig()
	{
		item_id				= -1;
		can_sell			= false;
		can_buy 			= false;
		quantity			= -1;
		restock_quantity	= -1;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Serialize( Serializer s )
	{
		s.Write( item_id );
		s.Write( can_sell );
		s.Write( can_buy );
		s.Write( quantity );
		s.Write( restock_quantity );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Deserialize( Serializer s )
	{
		s.Read( item_id );
		s.Read( can_sell );
		s.Read( can_buy );
		s.Read( quantity );
		s.Read( restock_quantity );
	}
}