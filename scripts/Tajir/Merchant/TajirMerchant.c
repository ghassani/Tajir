/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirMerchant
{
	protected TajirMerchantConfig m_config;

	protected Object 		m_object = NULL;

	protected int 			m_netIDL;

	protected int 			m_netIDH;

	protected TajirStore 	m_store = NULL;

	protected ref array<ref TajirMerchantTransportBay> m_transportBays;

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirMerchantConfig  The tajir merchant configuration
	 * @param[in]  Object               The object
	 */
	void TajirMerchant( notnull TajirMerchantConfig config, notnull Object object )
	{
		m_config  		= config;
		m_object  		= object;
		m_transportBays = new array<ref TajirMerchantTransportBay>;

		object.GetNetworkID( m_netIDL, m_netIDH );

		InitObject();
	}

	/**
	 * @brief      Destroys the object.
	 */
	void ~TajirMerchant()
	{
		if ( m_object && m_object.IsEntityAI() && GetGame().IsServer() )
		{
			EntityAI.Cast( m_object ).Delete();
		}

		if ( m_config )
		{
			delete m_config;
		}
	}

	/**
	 * { item_description }
	 */
	protected void InitObject()
	{
		if ( !m_object )
		{
			TajirLogE( string.Format( "No object instance for Merchant ID %1", m_config.id ), ClassName() );
		}
		else
		{
			if ( m_object.IsMan() ) 
			{
				ManBase man = ManBase.Cast( m_object );
				
				man.SetMerchant( this );	

				if ( GetGame().IsServer() )
				{
					AttachItemToMerchant( man, m_config.clothing.head );
					AttachItemToMerchant( man, m_config.clothing.shoulder );
					AttachItemToMerchant( man, m_config.clothing.headgear );
					AttachItemToMerchant( man, m_config.clothing.mask );
					AttachItemToMerchant( man, m_config.clothing.eyewear );
					AttachItemToMerchant( man, m_config.clothing.hands );
					AttachItemToMerchant( man, m_config.clothing.lefthand );
					AttachItemToMerchant( man, m_config.clothing.gloves );
					AttachItemToMerchant( man, m_config.clothing.armband );
					AttachItemToMerchant( man, m_config.clothing.vest );
					AttachItemToMerchant( man, m_config.clothing.body );
					AttachItemToMerchant( man, m_config.clothing.back );
					AttachItemToMerchant( man, m_config.clothing.hips );
					AttachItemToMerchant( man, m_config.clothing.legs );
					AttachItemToMerchant( man, m_config.clothing.feet );
				}
			}

			if ( GetGame().IsServer() )
			{
				m_object.SetPosition( m_config.position );
				m_object.SetOrientation( m_config.orientation );

				if ( m_config.unkillable )
				{
					//m_object.SetAllowDamage( false );
					m_object.SetCanBeDestroyed( false );
				}
			}
		}

		for ( int i = 0; i < m_config.transport_bays.Count(); i++ )
		{
			if ( m_config.transport_bays[ i ].position[ 0 ] == 0 || m_config.transport_bays[ i ].position[ 2 ] == 0 )
			{
				TajirLogE( string.Format( "Merchant ID %1 - Transport Bay %2 Invalid Position %3", GetId(), i, m_config.transport_bays[ i ].position.ToString() ), ClassName() );
				continue;
			}
			else if ( m_config.transport_bays[ i ].radius <= 0 )
			{
				TajirLogE( string.Format( "Merchant ID %1 - Transport Bay %2 Invalid Radius %3", GetId(), i, m_config.transport_bays[ i ].radius ), ClassName() );
				continue;
			}
			
			TajirLogE( string.Format( "Merchant ID %1 - Adding Transport Bay %2 Position %3 Radius %4", GetId(), i, m_config.transport_bays[ i ].position.ToString(), m_config.transport_bays[ i ].radius ), ClassName() );

			m_transportBays.Insert( new TajirMerchantTransportBay( this, m_config.transport_bays[ i ] ) );
		}

		m_store = TajirStoreComponent.GetInstance().GetStore( m_config.store );

		if ( !m_store )
		{
			TajirLogE( string.Format( "Merchant ID %1 - Unable to Find Store ID %1. Merchant Not Assigned Any Store", GetId(), m_config.store ), ClassName() );
			return;
		}

#ifdef TAJIR_DEBUG
		TajirLogD( string.Format( "Merchant ID %1 - Assigned To Store ID %2",  GetId(), m_store.GetId() ), ClassName() );
#endif
	}

	/**
	 * @brief      Sets the store.
	 *
	 * @param[in]  store  The store
	 */
	void SetStore( TajirStore store )
	{
		m_store = store;
	}

	/**
	 * @brief      Gets the store.
	 *
	 * @return     The store.
	 */
	TajirStore GetStore()
	{
		return m_store;
	}

	/**
	 * @brief      Gets the identifier.
	 *
	 * @return     The identifier.
	 */
	int GetId()
	{
		return m_config.id;
	}

	/**
	 * @brief      Gets the name.
	 *
	 * @return     The name.
	 */
	string GetName()
	{
		return m_config.name;
	}

	/**
	 * @brief      Gets the configuration.
	 *
	 * @return     The configuration.
	 */
	ref TajirMerchantConfig GetConfig()
	{
		return m_config;
	}

	/**
	 * @brief      Gets the object.
	 *
	 * @return     The object.
	 */
	Object GetObject()
	{
		if ( m_object )
		{
			return m_object;
		}

		if ( m_netIDL || m_netIDH )
		{
			m_object = GetGame().GetObjectByNetworkId( m_netIDL, m_netIDH );
		}

		return m_object;
	}

	/**
	 * @brief      Determines if unkillable.
	 *
	 * @return     True if unkillable, False otherwise.
	 */
	bool IsUnkillable()
	{
		return m_config.unkillable;
	}

	/**
	 * @brief      Determines if it has transport bay.
	 *
	 * @return     True if has transport bay, False otherwise.
	 */
	bool HasTransportBay()
	{
		if ( m_config.transport_bays.Count() )
		{
			return true;
		}

		return false;
	}

	/**
	 * @brief      Gets the transportation bays.
	 *
	 * @return     The transportation bays.
	 */
	array<ref TajirMerchantTransportBay> GetTransportationBays()
	{
		return m_transportBays;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  Transport  The transport
	 *
	 * @return     { description_of_the_return_value }
	 */
	TajirMerchantTransportBay FindFreeTransportationBayFor( notnull Transport transport )
	{
		foreach ( TajirMerchantTransportBay bay : m_transportBays )
		{
			if ( bay.CanBayFit( transport ) )
			{
				return bay;
			}
		}

		return NULL;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  transportType  The transport type
	 *
	 * @return     { description_of_the_return_value }
	 */
	TajirMerchantTransportBay FindFreeTransportationBayFor( string transportType )
	{
		foreach ( TajirMerchantTransportBay bay : m_transportBays )
		{
			if ( bay.CanBayFit( transportType ) )
			{
				return bay;
			}
		}

		return NULL;
	}

	/**
	 * { item_description }
	 */
	protected void AttachItemToMerchant( ManBase merchant, string objName )
	{
		if ( objName == "" )	return;
		
		EntityAI entity = merchant.GetInventory().CreateInInventory( objName );

		if ( !entity )
		{
			TajirLogE( string.Format( "Unable To Create %1 on Merchant %2", objName, GetId() ), ClassName() );
		}
	}

	/**
	 * @brief      Check if a player is in a valid distance of the merchant.
	 *
	 * @param[in]  PlayerBase  The player
	 *
	 * @return    bool
	 */
	bool PlayerNearMerchant( PlayerBase player )
	{
		ref array<Object> objects 	 = new array<Object>;
		ref array<CargoBase> proxies = new array<CargoBase>;

		GetGame().GetObjectsAtPosition3D( GetObject().GetPosition(), TAJIR_MERCHANT_STOP_TRIGGER_RADIUS, objects, proxies );

		foreach ( Object object : objects )
		{
			if ( object == player )
			{
				return true;
			}
		}

		return false;
	}
}