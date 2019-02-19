/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class PlayerBase
{

	override void EEHealthLevelChanged( int oldLevel, int newLevel, string zone )
	{
		
		super.EEHealthLevelChanged( oldLevel, newLevel, zone );

		if ( !CommitedSuicide() )
		{
			if ( IsInSafeZone() || ( IsMerchant() && m_merchant.IsUnkillable() ) )
			{
				TajirLogD( string.Format( "EEHealthLevelChanged Old Level %1 New Level %2 Zone %3 - %4 / %5", oldLevel, newLevel, zone, GetType(), GetHealth( "", "" ) ), ClassName() );
				
				m_HealthLevel = 0;
				SetSynchDirty();
			}
		}
	}

	override void EEKilled( Object killer )
	{
		if ( !CommitedSuicide() )
		{
			if ( IsInSafeZone() || ( IsMerchant() && m_merchant.IsUnkillable() ) )
			{				
				TajirLogD( string.Format( "EEKilled In Outpost: %1 In Safe Zone: %2 Merchant: %3 Alive: %4 HealthLevel: %5", IsInOutpost(), IsInSafeZone(), IsMerchant(), IsAlive(), GetHealthLevel() ), ClassName() );
		
				return;
			}

			if ( killer.IsMan() )
			{
				PlayerBase playerKiller = PlayerBase.Cast( killer );

				if ( playerKiller && playerKiller.IsInSafeZone() )
				{
					TajirLogD( string.Format( "EEKilled Out Of Safe Zone by Safe Zone Player %1", playerKiller.GetIdentity().GetId() ), ClassName() );
		
					return;
				}
			}
		}

		super.EEKilled( killer );
	}

	override void EEHitBy( TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos )
	{
		if ( !CommitedSuicide() )
		{
			if ( IsInSafeZone() || ( IsMerchant() && m_merchant.IsUnkillable() ) )
			{	
				TajirHealthHelper.RestoreTotalDamageResult( this, damageResult );

				m_KilledByHeadshot = false;

				TajirLogD( string.Format( "EEHitBy In Outpost: %1 In Safe Zone: %2 Merchant: %3 At %4 Alive: %5 HealthLevel: %6 Zone Level: %7", IsInOutpost(), IsInSafeZone(), IsMerchant(), dmgZone, IsAlive(), GetHealthLevel(), GetHealthLevel(dmgZone) ), ClassName() );
				
				//ref TajirEntityHealthSaveState state = new TajirEntityHealthSaveState( this );
				//state.DebugLogCurrentState();

				return;
			}

			if ( source ) 
			{
				ManBase attacker = ManBase.Cast( source.GetHierarchyRootPlayer() );
				
				if( attacker && attacker.IsInSafeZone() )
				{
					TajirHealthHelper.RestoreTotalDamageResult( this, damageResult );
					
					TajirLogD( string.Format( "EEHitBy Attacker In Safe Zone In Outpost: %1 In Safe Zone: %2 Merchant: %3 At %4 Alive: %5 HealthLevel: %6 Zone Level: %7", IsInOutpost(), IsInSafeZone(), IsMerchant(), dmgZone, IsAlive(), GetHealthLevel(), GetHealthLevel(dmgZone) ), ClassName() );

					m_KilledByHeadshot = false;

					return;
				}
			}
		}

		super.EEHitBy( damageResult, damageType, source, component, dmgZone, ammo, modelPos );
	}

	override bool CanBeRestrained()
	{
		if ( IsInSafeZone() && !m_outpost.GetSafeZoneConfig().UnlockRestraining() || ( IsMerchant() && m_merchant.IsUnkillable() ) )
		{
			return false;
		}

		return super.CanBeRestrained();
	}
}