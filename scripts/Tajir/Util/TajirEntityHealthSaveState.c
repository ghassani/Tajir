/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      Utility class to snapshot an Entities health state
 */
class TajirEntityHealthSaveState
{
	protected EntityAI 								m_entity;
	protected ref array<string> 					m_types;
	protected ref array<ref TajirEntityDamageZone> 	m_zones;
	protected int 									m_agents;
	protected ref map<EPlayerStats_v100, float> 	m_fstats;
	protected ref map<EPlayerStats_v100, float> 	m_fstatsmax;
	protected ref map<EPlayerStats_v100, int> 		m_istats;
	protected ref map<EPlayerStats_v100, int> 		m_istatsmax;
	protected bool 									m_isAlive;
	protected bool 									m_isDamageDestroyed;

	/**
	 * @brief      Constructor
	 *
	 * @param[in]  EntityAI  The entity
	 */
	void TajirEntityHealthSaveState( EntityAI entity )
	{
		m_entity 				= entity;
		m_types  				= new array<string>;
		m_zones  				= new array<ref TajirEntityDamageZone>;
		m_fstats 				= new map<EPlayerStats_v100, float>;
		m_fstatsmax 			= new map<EPlayerStats_v100, float>;
		m_istats 				= new map<EPlayerStats_v100, int>;
		m_istatsmax 			= new map<EPlayerStats_v100, int>;
		m_agents				= 0;
		m_isAlive 				= true;
		m_isDamageDestroyed 	= false;

		m_types.Insert( "Health" );

		AddZone( "GlobalHealth", "Health", 0 );

		if ( m_entity )
		{
			if ( m_entity.IsMan() )
			{
				m_types.Insert( "Blood" );
				m_types.Insert( "Shock" );
				
				AddZone( "GlobalHealth", "Blood", 0 );
				AddZone( "GlobalHealth", "Shock", 0 );
			}

			Save();
		}
	}

	/**
	 * @brief      Gets the zones.
	 *
	 * @return     The zones.
	 */
	array<ref TajirEntityDamageZone> GetZones()
	{
		return m_zones;
	}

	/**
	 * @brief      Gets the valid types.
	 *
	 * @return     The valid types.
	 */
	array<string> GetValidTypes()
	{
		return m_types;
	}

	/**
	 * @brief      Determines if it has agent.
	 *
	 * @param[in]  agent  The agent
	 *
	 * @return     True if has agent, False otherwise.
	 */
	bool HasAgent( eAgents agent )
	{
		if ( ( m_agents & agent ) == agent )
		{
			return true;
		}

		return false;
	}

	/**
	 * @brief      Adds a zone.
	 *
	 * @param[in]  zone   The zone
	 * @param[in]  type   The type
	 * @param[in]  value  The value
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool AddZone( string zone, string type, float value = 0 )
	{
		if ( !HasZone( zone, type ) )
		{
			m_zones.Insert( new TajirEntityDamageZone( zone, type, value ) );

			return true;
		}

		return false;
	}

	/**
	 * @brief      Determines if it has zone.
	 *
	 * @param[in]  zone  The zone
	 * @param[in]  type  The type
	 *
	 * @return     True if has zone, False otherwise.
	 */
	bool HasZone( string zone, string type )
	{
		foreach ( TajirEntityDamageZone entry : m_zones )
		{
			if ( entry.GetZone() == zone && entry.GetType() == type )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      Gets the zone.
	 *
	 * @param[in]  zone  The zone
	 * @param[in]  type  The type
	 *
	 * @return     The zone.
	 */
	TajirEntityDamageZone GetZone( string zone, string type )
	{
		foreach ( TajirEntityDamageZone entry : m_zones )
		{
			if ( entry.GetZone() == zone && entry.GetType() == type )
			{
				return entry;
			}
		}

		return NULL;
	}

	/**
	 * @brief      Get health of a type and zone
	 *
	 * @param[in]  string   The zone
	 * @param[in]  string   The type
	 *
	 * @return     float
	 */
	float GetHealth( string zone, string type )
	{
		TajirEntityDamageZone z = GetZone( zone, type );

		if ( z )
		{
			return z.GetValue();
		}
		
		return -1;
	}

	/**
	 * @brief      Set health of a type and zone to amount
	 *
	 * @param[in]  string   The zone
	 * @param[in]  string   The type
	 * @param[in]  float  	The amount
	 *
	 * @return     bool
	 */
	bool SetHealth( string zone, string type, float value )
	{
		TajirEntityDamageZone z = GetZone( zone, type );

		if ( z )
		{
			z.SetValue( value );

			return true;
		}
		
		return false;
	}

	/**
	 * @brief      Increment health of a type and zone by amount
	 *
	 * @param[in]  string    The zone
	 * @param[in]  string    The type
	 * @param[in]  float  The amount
	 *
	 * @return     bool
	 */
	bool AddHealth( string zone, string type, float amount )
	{
		TajirEntityDamageZone z = GetZone( zone, type );

		if ( z )
		{
			z.AddValue( amount );

			return true;
		}
		
		return false;
	}

	/**
	 * @brief      Saves the current health values from the current entity
	 * 
	 * Context: Server or Client - Client will not populate but wait for server updates
	 */
	void Save()
	{
		int i;
		int z;
		float health;

		ref array<string> zones = new array<string>;

		m_entity.GetDamageZones( zones );

		foreach ( string gtype : m_types )
		{
			health = 0;

			if ( GetGame().IsServer() )
			{
				health = m_entity.GetHealth( "GlobalHealth",  gtype );
			}

			if ( !SetHealth( "GlobalHealth", gtype, health ) )
			{
				TajirLogE( string.Format( "Error setting health for zone GlobalHealth type %1", gtype ), ClassName() );
				continue;
			}
		}
	
		foreach ( string zone : zones )
		{
			foreach ( string type : m_types )
			{
				if ( m_entity.GetMaxHealth( zone, type ) <= 0 )
				{
					continue;
				}

				if ( !HasZone( zone, type ) )
				{
					if ( !AddZone( zone, type ) )
					{
						TajirLogE( string.Format( "Error creating entry for zone %1 type %2", zone,  type ), ClassName() );
						continue;
					}
				}

				health = 0;

				if ( GetGame().IsServer() )
				{
					health = m_entity.GetHealth( zone,  type );
				}

				if ( !SetHealth( zone, type, health ) )
				{
					TajirLogE( string.Format( "Error setting health for zone %1 type %2 Value %3", zone,  type, health ), ClassName() );
					continue;
				}

			}
		}

		if ( GetGame().IsServer() )
		{
			m_isAlive = m_entity.IsAlive();
			m_isDamageDestroyed = m_entity.IsDamageDestroyed();

			if ( m_entity.IsPlayer() )
			{
				PlayerBase player = PlayerBase.Cast( m_entity );

				if ( player )
				{
					//m_agents = player.GetAgents();

					if ( player.GetPlayerStats() )
					{
						m_fstats.Set( EPlayerStats_v100.HEATCOMFORT, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.HEATCOMFORT ) ).Get() );
						m_fstatsmax.Set( EPlayerStats_v100.HEATCOMFORT, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.HEATCOMFORT ) ).GetMax() );

						m_fstats.Set( EPlayerStats_v100.TREMOR, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.TREMOR ) ).Get() );
						m_fstatsmax.Set( EPlayerStats_v100.TREMOR, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.TREMOR ) ).GetMax() );

						m_fstats.Set( EPlayerStats_v100.ENERGY, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.ENERGY ) ).Get() );
						m_fstatsmax.Set( EPlayerStats_v100.ENERGY, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.ENERGY ) ).GetMax() );

						m_fstats.Set( EPlayerStats_v100.WATER, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.WATER ) ).Get() );
						m_fstatsmax.Set( EPlayerStats_v100.WATER, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.WATER ) ).GetMax() );

						m_fstats.Set( EPlayerStats_v100.STOMACH_SOLID, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.STOMACH_SOLID ) ).Get() );
						m_fstatsmax.Set( EPlayerStats_v100.STOMACH_SOLID, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.STOMACH_SOLID ) ).GetMax() );

						m_fstats.Set( EPlayerStats_v100.STOMACH_ENERGY, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.STOMACH_ENERGY ) ).Get() );
						m_fstatsmax.Set( EPlayerStats_v100.STOMACH_ENERGY, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.STOMACH_ENERGY ) ).GetMax() );

						m_fstats.Set( EPlayerStats_v100.STOMACH_WATER, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.STOMACH_WATER ) ).Get() );
						m_fstatsmax.Set( EPlayerStats_v100.STOMACH_WATER, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.STOMACH_WATER ) ).GetMax() );

						m_fstats.Set( EPlayerStats_v100.DIET, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.DIET ) ).Get() );
						m_fstatsmax.Set( EPlayerStats_v100.DIET, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.DIET ) ).GetMax() );

						m_fstats.Set( EPlayerStats_v100.STAMINA, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.STAMINA ) ).Get() );
						m_fstatsmax.Set( EPlayerStats_v100.STAMINA, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.STAMINA ) ).GetMax() );

						m_fstats.Set( EPlayerStats_v100.SPECIALTY, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.SPECIALTY ) ).Get() );
						m_fstatsmax.Set( EPlayerStats_v100.SPECIALTY, PlayerStat<float>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.SPECIALTY ) ).GetMax() );

						m_istats.Set( EPlayerStats_v100.WET, PlayerStat<int>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.WET ) ).Get() );
						m_istatsmax.Set( EPlayerStats_v100.WET, PlayerStat<int>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.WET ) ).GetMax() );

						m_istats.Set( EPlayerStats_v100.BLOODTYPE, PlayerStat<int>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.BLOODTYPE ) ).Get() );
						m_istatsmax.Set( EPlayerStats_v100.BLOODTYPE, PlayerStat<int>.Cast( player.GetPlayerStats().GetStatObject( EPlayerStats_v100.BLOODTYPE ) ).GetMax() );
					}
				}
			}
		}
	}

	/**
	 * @brief      Gets the float statistics.
	 *
	 * @return     The float statistics.
	 */
	map<EPlayerStats_v100, float> GetFloatStats()
	{
		return m_fstats;
	}

	/**
	 * @brief      Gets the float maximum statistics.
	 *
	 * @return     The float maximum statistics.
	 */
	map<EPlayerStats_v100, float> GetFloatMaxStats()
	{
		return m_fstatsmax;
	}

	/**
	 * @brief      Gets the int statistics.
	 *
	 * @return     The int statistics.
	 */
	map<EPlayerStats_v100, int> GetIntStats()
	{
		return m_istats;
	}

	/**
	 * @brief      Gets the int maximum statistics.
	 *
	 * @return     The int maximum statistics.
	 */
	map<EPlayerStats_v100, int> GetIntMaxStats()
	{
		return m_istatsmax;
	}

	/**
	 * @brief      Gets the float stat.
	 *
	 * @param[in]  stat  The stat
	 *
	 * @return     The float stat.
	 */
	float GetFloatStat( EPlayerStats_v100 stat )
	{
		if ( m_fstats.Contains( stat ) )
		{
			return m_fstats.Get( stat );
		}

		return 0;
	}

	/**
	 * @brief      Gets the float stat maximum.
	 *
	 * @param[in]  stat  The stat
	 *
	 * @return     The float stat maximum.
	 */
	float GetFloatStatMax( EPlayerStats_v100 stat )
	{
		if ( m_fstatsmax.Contains( stat ) )
		{
			return m_fstatsmax.Get( stat );
		}

		return 0;
	}

	/**
	 * @brief      Determines if it has float stat.
	 *
	 * @param[in]  stat  The stat
	 *
	 * @return     True if has float stat, False otherwise.
	 */
	bool HasFloatStat( EPlayerStats_v100 stat )
	{
		return m_fstats.Contains( stat );
	}

	/**
	 * @brief      Determines if it has float stat maximum.
	 *
	 * @param[in]  stat  The stat
	 *
	 * @return     True if has float stat maximum, False otherwise.
	 */
	bool HasFloatStatMax( EPlayerStats_v100 stat )
	{
		return m_fstatsmax.Contains( stat );
	}

	/**
	 * @brief      Gets the int stat.
	 *
	 * @param[in]  stat  The stat
	 *
	 * @return     The int stat.
	 */
	float GetIntStat( EPlayerStats_v100 stat )
	{
		if ( m_istats.Contains( stat ) )
		{
			return m_istats.Get( stat );
		}

		return 0;
	}

	/**
	 * @brief      Gets the int stat maximum.
	 *
	 * @param[in]  stat  The stat
	 *
	 * @return     The int stat maximum.
	 */
	float GetIntStatMax( EPlayerStats_v100 stat )
	{
		if ( m_istatsmax.Contains( stat ) )
		{
			return m_istatsmax.Get( stat );
		}

		return 0;
	}

	/**
	 * @brief      Determines if it has int stat.
	 *
	 * @param[in]  stat  The stat
	 *
	 * @return     True if has int stat, False otherwise.
	 */
	bool HasIntStat( EPlayerStats_v100 stat )
	{
		return m_istats.Contains( stat );
	}

	/**
	 * @brief      Determines if it has int stat maximum.
	 *
	 * @param[in]  stat  The stat
	 *
	 * @return     True if has int stat maximum, False otherwise.
	 */
	bool HasIntStatMax( EPlayerStats_v100 stat )
	{
		return m_istatsmax.Contains( stat );
	}

	/**
	 * @brief      Determines if alive.
	 *
	 * @return     True if alive, False otherwise.
	 */
	bool IsAlive()
	{
		return m_isAlive;
	}

	/**
	 * @brief      Determines if damage destroyed.
	 *
	 * @return     True if damage destroyed, False otherwise.
	 */
	bool IsDamageDestroyed()
	{
		return m_isDamageDestroyed;
	}

	/**
	 *
	 * @brief      Restors the saved health values to the current Entity
	 * 
	 * Context: Server
	 */
	void Restore()
	{
		if ( !GetGame().IsServer() )
		{
			return;
		}

		foreach ( TajirEntityDamageZone entry : m_zones )
		{
			m_entity.SetHealth( entry.GetZone(), entry.GetType(), entry.GetValue() );
		}
	}

	/**
	 * @brief      { function_description }
	 */
	void DebugLogCurrentState()
	{
		TajirLogD( string.Format( "Type: %1 Alive: %2 Destroyed: %3", m_entity.GetType(), m_isAlive, m_isDamageDestroyed  ), ClassName() );

		foreach ( TajirEntityDamageZone entry : m_zones )
		{
			TajirLogD( string.Format( "Zone %1 Type %2 Value %3/%4 Level: %5", entry.GetZone(), entry.GetType(), entry.GetValue(), m_entity.GetMaxHealth( entry.GetZone(), entry.GetType() ), m_entity.GetHealthLevel( entry.GetZone() ) ), ClassName() );
		}
	}

	/**
	 * @brief      Gets the entity.
	 *
	 * @return     The entity.
	 */
	EntityAI GetEntity()
	{
		return m_entity;
	}
}

/**
 * @brief TajirEntityDamageZone used internally in TajirEntityHealthSaveState
 */
class TajirEntityDamageZone
{
	protected string m_zone;

	protected string m_type;

	protected float m_value;

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  zone   The zone
	 * @param[in]  type   The type
	 * @param[in]  value  The value
	 */
	void TajirEntityDamageZone( string zone, string type, float value = 0)
	{
		m_zone	= zone;
		m_type	= type;
		m_value	= value;
	}

	/**
	 * @brief      Gets the zone.
	 *
	 * @return     The zone.
	 */
	string GetZone()
	{
		return m_zone;
	}

	/**
	 * @brief      Sets the zone.
	 *
	 * @param[in]  zone  The zone
	 */
	void SetZone( string zone )
	{
		m_zone = zone;
	}

	/**
	 * @brief      Gets the type.
	 *
	 * @return     The type.
	 */
	string GetType()
	{
		return m_type;
	}

	/**
	 * @brief      Sets the type.
	 *
	 * @param[in]  type  The type
	 */
	void SetType( string type )
	{
		m_type = type;
	}

	/**
	 * @brief      Gets the value.
	 *
	 * @return     The value.
	 */
	float GetValue()
	{
		return m_value;
	}

	/**
	 * @brief      Sets the value.
	 *
	 * @param[in]  value  The value
	 */
	void SetValue( float value )
	{
		m_value = value;
	}

	/**
	 * @brief      Adds a value.
	 *
	 * @param[in]  amount  The amount
	 */
	void AddValue( float amount )
	{
		m_value += amount;
	}
}