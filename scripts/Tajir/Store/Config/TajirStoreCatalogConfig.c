/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirStoreCatalogConfig - Data read from JSON configuration
 */
class TajirStoreCatalogConfig
{
	string 										 name;
	bool 										 hidden;
	ref array<ref TajirStoreCatalogItemConfig> 	 items;

	/**
	 * @brief      { function_description }
	 */
	void TajirStoreCatalogConfig()
	{
		items 	= new array<ref TajirStoreCatalogItemConfig>;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Serialize( Serializer s )
	{
		s.Write( name );
		s.Write( hidden );
		s.Write( items );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Deserialize( Serializer s )
	{
		s.Read( name );
		s.Read( hidden );
		s.Read( items );
	}
}
