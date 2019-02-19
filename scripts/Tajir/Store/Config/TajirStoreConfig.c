/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirStoreConfig - Data read from JSON configuration
 */
class TajirStoreConfig
{
	int 									id;
	string 									name;
	string 									type;
	bool 									log_transactions;
	int 									save_every_minutes;
	int 									restock_every_minutes;
	bool 									show_out_of_stock;
	ref array<int> 							allowed_conditions;
	ref array<ref TajirStoreCatalogConfig>  catalogs;

	/**
	 * @brief      { function_description }
	 */
	void TajirStoreConfig()
	{
		type 					= "default";
		log_transactions 		= false;
		save_every_minutes 		= 0;
		restock_every_minutes 	= 0;
		show_out_of_stock 		= false;
		catalogs 			= new array<ref TajirStoreCatalogConfig>;
		allowed_conditions	= new array<int>;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Serialize( Serializer s )
	{
		s.Write( id );
		s.Write( name );
		s.Write( type );
		s.Write( log_transactions );
		s.Write( save_every_minutes );
		s.Write( restock_every_minutes );
		s.Write( show_out_of_stock );
		s.Write( allowed_conditions );
		s.Write( catalogs );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Deserialize( Serializer s )
	{
		s.Read( id );
		s.Read( name );
		s.Read( type );
		s.Read( log_transactions );
		s.Read( save_every_minutes );
		s.Read( restock_every_minutes );
		s.Read( show_out_of_stock );
		s.Read( allowed_conditions );
		s.Read( catalogs );
	}
}