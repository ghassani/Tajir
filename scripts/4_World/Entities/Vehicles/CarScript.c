/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class CarScript
{
	protected TajirOutpost		m_outpost;

	protected string 			m_ownerUUID;

	protected ref array<string> m_allowedUUIDs;

	protected bool m_IsInSafezoneClient = false;

	void CarScript()
	{
		m_allowedUUIDs = new array<string>;

		RegisterNetSyncVariableBool( "m_IsInSafezoneClient" );
	}

	array<string> GetAllowed()
	{
		return m_allowedUUIDs;
	}

	TajirOutpost GetOutpost()
	{
		return m_outpost;
	}

	void SetOutpost( TajirOutpost outpost )
	{
		m_outpost = outpost;

		if ( !m_outpost )
		{
			ClearOwner();
			ClearAllowed();
			m_IsInSafezoneClient = false;
		}
		else
		{
			m_IsInSafezoneClient = IsInSafeZone();
		}

		SetSynchDirty();
	}

	bool IsInSafeZone()
	{
		if ( GetGame().IsServer() )
		{
			if ( m_outpost )
			{
				return m_outpost.IsSafeZone();
			}

			return false;
		}

		return m_IsInSafezoneClient;
	}

	void SetOwner( notnull PlayerBase player )
	{
#ifdef TAJIR_TRANSPORT_OWNER_FAKE
		m_ownerUUID = "FAKE-UUID-FOR-TESTING";
#else
		m_ownerUUID = player.GetIdentity().GetId();
#endif
		TajirLogD( string.Format( "Added Player %1 As Owner", m_ownerUUID ), ClassName() );
		//SetSynchDirty();
	}

	string GetOwner()
	{
		return m_ownerUUID;
	}

	void ClearOwner()
	{
		m_ownerUUID = "";
	}

	void AddAllowedPlayer( notnull PlayerBase player )
	{
		m_allowedUUIDs.Insert( player.GetIdentity().GetId() );
		TajirLogD( string.Format( "Added Player %1 As Allowed", player.GetIdentity().GetId() ), ClassName() );
	}

	void RemoveAllowedPlayer( notnull PlayerBase player )
	{
		for ( int i = 0; i < m_allowedUUIDs.Count(); i++ )
		{
			if ( m_allowedUUIDs[ i ] == player.GetIdentity().GetId() )
			{
				m_allowedUUIDs.Remove( i );
				return;
			}
		}
	}

	bool IsAccessAllowed( notnull PlayerBase player )
	{
		if ( IsOwner( player ) )
		{
			return true;
		}

		for ( int i = 0; i < m_allowedUUIDs.Count(); i++ )
		{
			if ( m_allowedUUIDs[ i ] == player.GetIdentity().GetId() )
			{
				return true;
			}
		}

		return false;
	}

	bool HasOwner()
	{
		return m_ownerUUID.Length() > 0;
	}

	bool IsOwner( notnull PlayerBase player )
	{
		if ( m_ownerUUID.Length() > 0 && player.GetIdentity().GetId() == m_ownerUUID )
		{
			return true;
		}

		return false;
	}

	void ClearAllowed()
	{
		m_allowedUUIDs.Clear();
	}
	
	override void OnContact( string zoneName, vector localPos, IEntity other, Contact data )
	{
		if ( IsInSafeZone() )
		{
			TajirLogD( "On Contact" );

			return;
		}

		super.OnContact( zoneName, localPos, other, data );
	}

	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );

		if ( !ctx.Write( m_ownerUUID ) )
		{
			TajirLogE( "[OnStoreSave] Error Writing m_ownerUUID", ClassName() );
		}

		if ( !ctx.Write( m_allowedUUIDs ) )
		{
			TajirLogE( "[OnStoreSave] Error Writing m_allowedUUIDs", ClassName() );
		}
	}

	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		m_ownerUUID 	= "";
		m_allowedUUIDs 	= new array<string>;

		if ( !ctx.Read( m_ownerUUID ) )
		{
			TajirLogE( "[OnStoreLoad] Error Reading m_ownerUUID", ClassName() );
			return false;
		}
		
		if ( !ctx.Read( m_allowedUUIDs ) )
		{
			TajirLogE( "[OnStoreLoad] Error Reading m_allowedUUIDs", ClassName() );
			return false;
		}

		return super.OnStoreLoad( ctx, version );
	}
	
    override bool CanReleaseAttachment( EntityAI attachment )
	{
        if ( IsInSafeZone() )
        {
        	return false;
        }
        
        return super.CanReleaseAttachment( attachment );
    }

	override bool CanReleaseCargo (EntityAI cargo)
	{
        if ( IsInSafeZone() )
        {
        	return false;
        }
        
        return super.CanReleaseCargo( cargo );
	}

	override bool CanRemoveFromCargo (EntityAI parent)
	{
        if ( IsInSafeZone() )
        {
        	return false;
        }
        
        return super.CanRemoveFromCargo( parent );
	}

	override void EEHitBy( TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos )
	{
		if ( IsInSafeZone() )
		{	
			TajirHealthHelper.RestoreTotalDamageResult( this, damageResult );

			TajirLogD( string.Format( "EEHitBy In Safe Zone: %1 ", IsInSafeZone() ), ClassName() );
			return;
		}

		super.EEHitBy( damageResult, damageType, source, component, dmgZone, ammo, modelPos );
	}
}