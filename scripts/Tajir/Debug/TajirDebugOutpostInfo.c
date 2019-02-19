/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirDebugOutpostPlayerInfo
{
	int network_id_low;
	int network_id_high;
	string id;
	string name;
	string fullname;
	int currency;
	vector position;
	bool merchant;
	bool inSafeZone;
	ref TajirEntityHealthSaveState state;
	
	void TajirDebugOutpostPlayerInfo( PlayerBase player )
	{
		id 			= "";
		name 		= "";
		fullname 	= "";
		position 	= Vector( 0, 0, 0 );
		currency 	= 0;
		merchant = false;
		inSafeZone = false;

		if ( player && GetGame().IsServer() )
		{
			currency = player.GetCurrency();
			position = player.GetPosition();

			if ( player.GetIdentity() )
			{
				id 		 = player.GetIdentity().GetId();
				name 	 = player.GetIdentity().GetName();
				fullname = player.GetIdentity().GetFullName();
			}
			else
			{
				id = player.GetType();
			}
			
			player.GetNetworkID( network_id_low, network_id_high );

			if ( player.IsMerchant() )
			{
				merchant = true;
			}

			inSafeZone = player.IsInSafeZone();
		}

		state = new TajirEntityHealthSaveState( EntityAI.Cast( player ) );
	}

	void Serialize( Serializer s )
	{
		s.Write( network_id_low );
		s.Write( network_id_high );
		s.Write( id );
		s.Write( name );
		s.Write( fullname );
		s.Write( currency );
		s.Write( position );
		s.Write( merchant );
		s.Write( inSafeZone );
		s.Write( state );
	}

	void Deserialize( Serializer s )
	{
		s.Read( network_id_low );
		s.Read( network_id_high );
		s.Read( id );
		s.Read( name );
		s.Read( fullname );
		s.Read( currency );
		s.Read( position );
		s.Read( merchant );
		s.Read( inSafeZone );
		s.Read( state );
	}
}

class TajirDebugOutpostTransportInfo
{
	int network_id_low;
	int network_id_high;
	string type;
	vector position;
	ref TajirEntityHealthSaveState state;
	bool inSafeZone;
	string ownerUUID;
	ref array<string> allowedUUIDs;

	void TajirDebugOutpostTransportInfo( Transport transport )
	{
		ownerUUID 	= "";
		type 		= "";
		position 	= Vector( 0, 0, 0 );
		allowedUUIDs = new array<string>;

		if ( transport && GetGame().IsServer() )
		{
			type = transport.GetType();

			position = transport.GetPosition();

			transport.GetNetworkID( network_id_low, network_id_high );
		}

		if ( transport.IsInherited( CarScript ) )
		{
			CarScript car = CarScript.Cast( transport );

			inSafeZone = car.IsInSafeZone();
			ownerUUID = car.GetOwner();
			allowedUUIDs = car.GetAllowed();
		}
		
		state = new TajirEntityHealthSaveState( EntityAI.Cast( transport ) );
	}

	void Serialize( Serializer s )
	{
		s.Write( network_id_low );
		s.Write( network_id_high );
		s.Write( type );
		s.Write( position );
		s.Write( state );
		s.Write( inSafeZone );
		s.Write( ownerUUID );
		s.Write( allowedUUIDs );
	}

	void Deserialize( Serializer s )
	{
		s.Read( network_id_low );
		s.Read( network_id_high );
		s.Read( type );
		s.Read( position );
		s.Read( state );
		s.Read( inSafeZone );
		s.Read( ownerUUID );
		s.Read( allowedUUIDs );
	}
}


class TajirDebugOutpostInfo
{
	ref array<ref TajirDebugOutpostPlayerInfo> 		players;
	ref array<ref TajirDebugOutpostTransportInfo> 	transports;
	ref TajirOutpostSafeZoneConfig 					safezone;
	int id;

	void TajirDebugOutpostInfo( TajirOutpost outpost = NULL )
	{
		ref array<Object> objects 		 = new array<Object>;
		ref array<CargoBase> proxies 	 = new array<CargoBase>;
		
		players 	 		= new array<ref TajirDebugOutpostPlayerInfo>;
		transports  		= new array<ref TajirDebugOutpostTransportInfo>;

		if ( outpost )
		{
			id 		 = outpost.GetId();
			safezone = outpost.GetSafeZoneConfig();

			GetGame().GetObjectsAtPosition3D( outpost.GetCenterPosition(), outpost.GetRadius(), objects, proxies );

			foreach ( Object object : objects )
			{
				if ( object.IsInherited( Transport ) )
				{
					transports.Insert( new TajirDebugOutpostTransportInfo( Transport.Cast( object ) ) );
				}
				else if ( object.IsInherited( PlayerBase ) )
				{
					players.Insert( new TajirDebugOutpostPlayerInfo( PlayerBase.Cast( object ) ) );
				}
			}
		}
	}

	void Serialize( Serializer s )
	{
		s.Write( players );
		s.Write( transports );
		s.Write( safezone );
		s.Write( id );
	}

	void Deserialize( Serializer s )
	{
		s.Read( players );
		s.Read( transports );
		s.Read( safezone );
		s.Read( id );
	}
}