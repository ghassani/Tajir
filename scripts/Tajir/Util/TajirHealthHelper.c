/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      Utility class to help with health and damage
 */
class TajirHealthHelper
{
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  EntityAI  The entity ai
	 */
	static void RestoreFullHealth( notnull EntityAI entity )
	{
		ref array<string> zones = new array<string>;

		entity.GetDamageZones( zones );

		if ( entity.IsMan() )
		{
			entity.SetHealth( "GlobalHealth", "Health", entity.GetMaxHealth( "GlobalHealth", "Health" ) );
			entity.SetHealth( "GlobalHealth", "Blood", entity.GetMaxHealth( "GlobalHealth", "Blood" ) );
			entity.SetHealth( "GlobalHealth", "Shock", entity.GetMaxHealth( "GlobalHealth", "Shock" ) );
		}
		else
		{
			entity.SetHealth( "GlobalHealth", "Health", entity.GetMaxHealth( "GlobalHealth", "Health" ) );
		}

		foreach ( string zone : zones )
		{
			entity.SetHealth( zone, "Health", entity.GetMaxHealth( zone, "Health" ) );
			
			if ( entity.IsMan() )
			{
				if ( entity.GetMaxHealth( zone, "Blood" ) > 0 )
				{
					entity.SetHealth( zone, "Blood", entity.GetMaxHealth( zone, "Blood" ) );
				}

				if ( entity.GetMaxHealth( zone, "Shock" ) > 0 )
				{
					entity.SetHealth( zone, "Shock", entity.GetMaxHealth( zone, "Shock" ) );
				}
			}
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  EntityAI  The entity ai
	 */
	static void KillEntity( notnull EntityAI entity )
	{
		entity.SetHealth( "GlobalHealth", "Health", 0 );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  EntityAI           The entity ai
	 * @param[in]  TotalDamageResult  The total damage result
	 */
	static void RestoreTotalDamageResult( notnull EntityAI entity, notnull TotalDamageResult damageResult )
	{
		ref array<string> zones = new array<string>;

		entity.GetDamageZones( zones );
		
		TajirLogD( string.Format( "Zone: GlobalHealth/Health Current: %1 Damage: %2 Max: %3", entity.GetHealth( "GlobalHealth", "Health" ), damageResult.GetDamage( "GlobalHealth", "Health" ), entity.GetMaxHealth( "GlobalHealth", "Health" ) ), ClassName() );

		if ( !entity.GetHealth( "GlobalHealth", "Health" ) )
		{
			entity.SetHealth( "GlobalHealth", "Health", entity.GetMaxHealth( "GlobalHealth", "Health" ) );
		}
		else
		{
			entity.AddHealth( "GlobalHealth", "Health", damageResult.GetDamage( "GlobalHealth", "Health" ) );
		}

		if ( entity.IsMan() )
		{
			if ( entity.GetMaxHealth( "GlobalHealth", "Blood" ) > 0 )
			{
				TajirLogD( string.Format( "Zone: GlobalHealth/Blood Current: %1 Damage: %2 Max: %3", entity.GetHealth( "GlobalHealth", "Blood" ), damageResult.GetDamage( "GlobalHealth", "Blood" ), entity.GetMaxHealth( "GlobalHealth", "Blood" ) ), ClassName() );

				entity.AddHealth( "GlobalHealth", "Blood", damageResult.GetDamage( "GlobalHealth", "Blood" ) );
			}

			if ( entity.GetMaxHealth( "GlobalHealth", "Shock" ) > 0 )
			{
				TajirLogD( string.Format( "Zone: GlobalHealth/Shock Current: %1 Damage: %2 Max: %3", entity.GetHealth( "GlobalHealth", "Shock" ), damageResult.GetDamage( "GlobalHealth", "Shock" ), entity.GetMaxHealth( "GlobalHealth", "Shock" ) ), ClassName() );
				entity.AddHealth( "GlobalHealth", "Shock", damageResult.GetDamage( "GlobalHealth", "Shock" ) );
			}
		}
		
		foreach ( string zone : zones )
		{
			TajirLogD( string.Format( "Zone: %1/Health Current: %2 Damage: %3 Max: %4", zone, entity.GetHealth( zone, "Health" ), damageResult.GetDamage( zone, "Health" ), entity.GetMaxHealth( zone, "Health" ) ), ClassName() );

			entity.AddHealth( zone, "Health", damageResult.GetDamage( zone, "Health" ) );
			
			if ( entity.IsMan() )
			{
				if ( entity.GetMaxHealth( zone, "Blood" ) > 0 )
				{
					TajirLogD( string.Format( "Zone: %1/Blood Current: %2 Damage: %3 Max: %4", zone, entity.GetHealth( zone, "Blood" ), damageResult.GetDamage( zone, "Blood" ), entity.GetMaxHealth( zone, "Blood" ) ), ClassName() );
					entity.AddHealth( zone, "Blood", damageResult.GetDamage( zone, "Blood" ) );
				}

				if ( entity.GetMaxHealth( zone, "Shock" ) > 0 )
				{
					TajirLogD( string.Format( "Zone: %1/Shock Current: %2 Damage: %3 Max: %4", zone, entity.GetHealth( zone, "Shock" ), damageResult.GetDamage( zone, "Shock" ), entity.GetMaxHealth( zone, "Shock" ) ), ClassName() );

					entity.AddHealth( zone, "Shock", damageResult.GetDamage( zone, "Shock" ) );
				}
			}
		}
	}
}