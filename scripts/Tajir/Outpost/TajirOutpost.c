/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirOutpost
{
	protected TajirOutpostConfig 					m_config;
	protected ref array<ref TajirOutpostObject> 	m_objects;
	protected ref array<PlayerBase>					m_players;
	protected ref array<Transport>					m_transports;

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirOutpostConfig  The tajir outpost configuration
	 */
	void TajirOutpost( ref TajirOutpostConfig config )
	{
		m_config  		= config;
		m_objects 		= new array<ref TajirOutpostObject>;
		m_players 		= new array<PlayerBase>;
		m_transports 	= new array<Transport>;

		InitObjects();
	}

	/**
	 * @brief      Destroys the object.
	 */
	void ~TajirOutpost()
	{
		foreach ( TajirOutpostObject object : m_objects )
		{
			if ( object )
			{
				delete object;
			}
		}
	}
	
	/**
	 * { item_description }
	 */
	protected void InitObjects()
	{
		Object object;

		for ( int i = 0; i < m_config.objects.Count(); i++ )
		{
			if ( m_config.objects[ i ].name == "" )
			{
				TajirLogE( string.Format( "Skipped Entry %1 Object Name Missing", i ), ClassName() );
				continue;
			}
			
			if ( GetGame().IsMultiplayer() && GetGame().IsClient() )
			{
				object = NULL;
				
#ifdef TAJIR_DEBUG
				TajirLogD( string.Format( "Received Outpost Object %1 Position: %2 Orientation: %3", m_config.objects[ i ].name, m_config.objects[ i ].position.ToString(), m_config.objects[ i ].orientation.ToString() ), ClassName() );
#endif
			}
			else
			{
				object  = GetGame().CreateObject( m_config.objects[ i ].name, m_config.objects[ i ].position, false, GetGame().IsKindOf( m_config.objects[ i ].name, "DZ_LightAI" ), true );
				
				if ( object == NULL ) 
				{
					TajirLogE( string.Format( "Skipped Entry %1 Object `%2` Invalid", i, m_config.objects[ i ].name ), ClassName() );
					continue;
				}
#ifdef TAJIR_DEBUG
				TajirLogD( string.Format( "Adding Outpost Object %1 Position: %2 Orientation: %3", m_config.objects[ i ].name, m_config.objects[ i ].position.ToString(), m_config.objects[ i ].orientation.ToString() ), ClassName() );
#endif
			}

			AddObject( new TajirOutpostObject( this, m_config.objects[ i ], object ) );
		}
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
	TajirOutpostConfig GetConfig()
	{
		return m_config;
	}

	/**
	 * @brief      Gets the safe zone configuration.
	 *
	 * @return     The safe zone configuration.
	 */
	TajirOutpostSafeZoneConfig GetSafeZoneConfig()
	{
		return m_config.safezone;
	}

	/**
	 * @brief      Sets the safe zone configuration.
	 *
	 * @param[in]  conf  The conf
	 */
	void SetSafeZoneConfig( TajirOutpostSafeZoneConfig conf )
	{
		m_config.safezone = conf;
	}

	/**
	 * @brief      Adds an object.
	 *
	 * @param[in]  TajirOutpostObject  The tajir outpost object
	 */
	void AddObject( ref TajirOutpostObject obj )
	{
		m_objects.Insert( obj );
	}

	/**
	 * @brief      Gets the objects.
	 *
	 * @return     The objects.
	 */
	array<ref TajirOutpostObject> GetObjects()
	{
		return m_objects;
	}

	/**
	 * @brief      Adds a transport.
	 *
	 * @param[in]  Transport  The transport
	 */
	void AddTransport( notnull Transport transport )
	{
		CarScript car;
		Human human;
		PlayerBase passenger;
		int crewsize;

		foreach ( Transport t : m_transports )
		{
			if ( t == transport )
			{
				return;
			}
		}

		if ( transport.IsInherited( CarScript ) )
		{
			car = CarScript.Cast( transport );

			if ( car )
			{
				car.SetOutpost( this );

				if ( !car.HasOwner() )
				{
					crewsize = car.CrewSize();

					for ( int c = 0; c < crewsize; c++ )
					{
						human = car.CrewMember( c );
						
						if ( human )
						{
							passenger = PlayerBase.Cast( human );							

							if ( passenger )
							{
								if ( !car.HasOwner() )
								{
									car.SetOwner( passenger );
								}
								else
								{
									car.AddAllowedPlayer( passenger );
								}
							}
						}
					}
				}
			}		
		}

		m_transports.Insert( transport );
	}

	/**
	 * @brief      Determines if it has transport.
	 *
	 * @param[in]  Transport  The transport
	 *
	 * @return     True if has transport, False otherwise.
	 */
	bool HasTransport( notnull Transport transport )
	{
		foreach ( Transport t : m_transports )
		{
			if ( t == transport )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      Removes a transport.
	 *
	 * @param[in]  Transport  The transport
	 */
	void RemoveTransport( notnull Transport transport )
	{
		CarScript car;

		for ( int i = 0; i < m_transports.Count(); i++ )
		{
			if ( m_transports[ i ] == transport )
			{
				if ( transport.IsInherited( CarScript ) )
				{
					car = CarScript.Cast( transport );

					if ( car )
					{
						car.SetOutpost( NULL );
					}
				}

				m_transports.Remove( i );

				return;
			}
		}
	}

	/**
	 * @brief      Gets the transports.
	 *
	 * @return     The transports.
	 */
	array<Transport> GetTransports()
	{
		return m_transports;
	}

	/**
	 * @brief      Determines if safe zone.
	 *
	 * @return     True if safe zone, False otherwise.
	 */
	bool IsSafeZone()
	{
		if ( m_config.safezone )
		{
			return m_config.safezone.enabled;
		}
		
		return false;
	}

	/**
	 * @brief      Gets the center position.
	 *
	 * @return     The center position.
	 */
	vector GetCenterPosition()
	{
		return m_config.position;
	}

	/**
	 * @brief      Gets the radius.
	 *
	 * @return     The radius.
	 */
	float GetRadius()
	{
		return m_config.radius;
	}

	/**
	 * @brief      Gets the players.
	 *
	 * @return     The players.
	 */
	ref array<PlayerBase> GetPlayers()
	{
		return m_players;
	}

	/**
	 * @brief      Adds a player.
	 *
	 * @param[in]  player  The player
	 */
	void AddPlayer( PlayerBase player )
	{
		if ( !HasPlayer( player ) )
		{
			m_players.Insert( player );
		}
	}

	/**
	 * @brief      Determines if it has player.
	 *
	 * @param[in]  player  The player
	 *
	 * @return     True if has player, False otherwise.
	 */
	bool HasPlayer( PlayerBase player )
	{
		foreach ( PlayerBase p : m_players )
		{
			if ( player == p )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      Removes a player.
	 *
	 * @param[in]  player  The player
	 */
	void RemovePlayer( PlayerBase player )
	{
		for ( int i = 0; i < m_players.Count(); i++ )
		{
			if ( player == m_players[ i ] )
			{				
				m_players.Remove( i );

				return;
			}
		}
	}

	/**
	 * @brief      Removes all transports.
	 */
	void RemoveAllTransports()
	{
		m_transports.Clear();
	}

	/**
	 * @brief      Removes all players.
	 */
	void RemoveAllPlayers()
	{
		m_players.Clear();
	}
}