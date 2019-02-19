/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirItemConfig - Data read from JSON configuration
 */
class TajirItemConfig
{
	int 	id;
	string 	object_name;
	string 	display_name;
	string 	description;
	string 	display_object_name;
	int 	sell_price;
	int 	buy_price;
	ref array<ref TajirItemBundleConfig> 	items;

	/**
	 * @brief      { function_description }
	 */
	void TajirStoreCatalogItemConfig()
	{
		items = new array<ref TajirItemBundleConfig>;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Serialize( Serializer s )
	{
		s.Write( id );
		s.Write( object_name );
		s.Write( display_name );
		s.Write( description );
		s.Write( display_object_name );
		s.Write( sell_price );
		s.Write( buy_price );
		s.Write( items );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Deserialize( Serializer s )
	{
		s.Read( id );
		s.Read( object_name );
		s.Read( display_name );
		s.Read( description );
		s.Read( display_object_name );
		s.Read( sell_price );
		s.Read( buy_price );
		s.Read( items );
	}
}