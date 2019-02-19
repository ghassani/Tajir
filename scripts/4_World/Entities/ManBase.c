/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class ManBase
{
	protected TajirOutpost  m_outpost  = NULL;

	protected TajirMerchant  m_merchant  = NULL;

	// client synced
	protected int m_currency;
	
	void ManBase()
	{
		RegisterNetSyncVariableInt( "m_currency", TAJIR_PLAYER_CURRENCY_MIN, TAJIR_PLAYER_CURRENCY_MAX );
	}

	void SetOutpost( TajirOutpost outpost )
	{
		m_outpost = outpost;
	}

	bool IsMerchant()
	{
		return m_merchant != NULL;
	}

	void SetMerchant( TajirMerchant merchant )
	{
		m_merchant = merchant;
	}

	TajirOutpost GetOutpost()
	{
		return m_outpost;
	}

	TajirMerchant GetMerchant()
	{
		return m_merchant;
	}

	bool IsInOutpost()
	{
		return m_outpost != NULL;	
	}

	bool IsInSafeZone()
	{
		if ( !IsInOutpost() )
		{
			return false;
		}
		
		return m_outpost.IsSafeZone();
	}

	int GetCurrency()
	{
		return m_currency;
	}

	void AddCurrency( int amount )
	{
		m_currency += amount;
	}

	void DeductCurrency( int amount )
	{
		m_currency -= amount;
	}

	void SetCurrency( int amount )
	{
		m_currency = amount;
	}

	bool MoveFundsToHands( int amount )
	{
		if ( !GetGame().IsServer() )
		{
			return false;
		}

		return true;
	}

	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );

		if ( !ctx.Write( m_currency ) )
		{
			TajirLogE( "[OnStoreSave] Error Writing m_currency", ClassName() );
		}
	}

	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
		{
			return false;
		}

		m_currency = 0;
		
		if ( !ctx.Read( m_currency ) )
		{
			TajirLogE( "[OnStoreLoad] Error Reading m_currency", ClassName() );
			return false;
		}

		return true;
	}
}