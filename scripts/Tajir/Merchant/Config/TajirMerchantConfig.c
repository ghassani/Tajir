/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirMerchantConfig - Data read from JSON configuration
 */
class TajirMerchantConfig
{
	int 	id;
	string 	name;
	string 	object_name;
	int 	store;
	bool 	unkillable;
	vector 	position;
	vector 	orientation;
	int 	network_id_low;
	int 	network_id_high;
	
	ref array<ref TajirMerchantTransportBayConfig> 	transport_bays;
	ref TajirMerchantClothingConfig 				clothing;

	void TajirMerchantConfig()
	{
		transport_bays = new array<ref TajirMerchantTransportBayConfig>;
		clothing = new TajirMerchantClothingConfig();
	}

	void Serialize( Serializer s )
	{
		s.Write( id );
		s.Write( name );
		s.Write( object_name );
		s.Write( store );
		s.Write( unkillable );
		s.Write( position );
		s.Write( orientation );
		s.Write( network_id_low );
		s.Write( network_id_high );
		s.Write( transport_bays );
		s.Write( clothing );
	}

	void Deserialize( Serializer s )
	{
		s.Read( id );
		s.Read( name );
		s.Read( object_name );
		s.Read( store );
		s.Read( unkillable );
		s.Read( position );
		s.Read( orientation );
		s.Read( network_id_low );
		s.Read( network_id_high );
		s.Read( transport_bays );
		s.Read( clothing );
	}
}