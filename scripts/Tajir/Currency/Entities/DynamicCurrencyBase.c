/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class DynamicCurrencyBase extends CurrencyBase
{
	protected int m_currencyValue = 1;

	void DynamicCurrencyBase()
	{
		int randMin  = GetGame().ConfigGetInt( "CfgVehicles " + ClassName() + " minRandomCurrency" );
		int randMax = GetGame().ConfigGetInt( "CfgVehicles " + ClassName() + " maxRandomCurrency" );

		if ( randMin <= 0 )
		{
			randMin = 1;
		}

		if ( randMax <= 0 )
		{
			randMax = 500;
		}

		m_currencyValue = Math.RandomInt( randMin, randMax );

		RegisterNetSyncVariableInt( "m_currencyValue", TAJIR_PLAYER_CURRENCY_MIN, TAJIR_PLAYER_CURRENCY_MAX );
	}

	void SetCurrencyValue( int amount )
	{
		m_currencyValue = amount;
	}

	/**
	 * @brief      Gets the currency value.
	 *
	 * @return     The currency value.
	 */
	int GetCurrencyValue()
	{
		return m_currencyValue;
	}

	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );

		if ( !ctx.Write( m_currencyValue ) )
		{
			TajirLogE( "[OnStoreSave] Error Writing m_currencyValue", ClassName() );
		}
	}

	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !ctx.Read( m_currencyValue ) )
		{
			TajirLogE( "[OnStoreLoad] Error Reading m_currencyValue", ClassName() );
			return false;
		}

		return super.OnStoreLoad( ctx, version );
	}

	override string GetDisplayName()
	{
		string display = super.GetDisplayName();

		return string.Format( display, m_currencyValue );
	}
}