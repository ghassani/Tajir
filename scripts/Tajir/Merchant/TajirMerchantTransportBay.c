/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirMerchantTransportBay
{
	protected TajirMerchant m_merchant = NULL;

	protected TajirMerchantTransportBayConfig m_config;

	void TajirMerchantTransportBay( notnull ref TajirMerchant merchant, notnull TajirMerchantTransportBayConfig config )
	{
		m_merchant  = merchant;
		m_config  	= config;

		if ( m_config.position[ 1 ] == 0 )
		{
			// snap to ground if no exact Y coord is provided
			m_config.position = TajirSpawnHelper.SnapToGround( m_config.position[ 0 ], m_config.position[ 2 ] );
		}
	}

	void ~TajirMerchantTransportBay()
	{
		
	}

	/**
	 * @brief      Gets the position of the transportation bay.
	 *
	 * @return     vector The position.
	 */
	vector GetPosition()
	{
		return m_config.position;
	}

	/**
	 * @brief      Gets the radius of the transportation bay.
	 *
	 * @return     int
	 */
	int GetRadius()
	{
		return m_config.radius;
	}

	/**
	 * @brief      Check if a given object can fit in this transport bay.
	 *
	 * @param[in]  Transport  The transport object
	 *
	 * @return     bool
	 */
	bool CanBayFit( notnull Transport transport )
	{
		return CanBayFit( transport.GetType() );
	}

	/**
	 * @brief      Check if a given object type can fit in this transport bay.
	 *
	 * @param[in]  string  The transport object type
	 *
	 * @return     bool
	 */
	bool CanBayFit( string transportType )
	{
		ref array<Object> objects 	 = new array<Object>;
		ref array<CargoBase> proxies = new array<CargoBase>;

		GetGame().GetObjectsAtPosition3D( GetPosition(), GetRadius(), objects, proxies );

		foreach( Object object : objects )
		{
#ifdef TAJIR_DEBUG
			TajirLogD( string.Format( "Merchant ID %1 - Found Object %2 Transport: %3", m_merchant.GetId(), object.GetType(), object.IsTransport() ), ClassName() );
#endif
			if ( object.IsTransport() )
			{
				return false;
			}
			else if ( object.IsMan() )
			{
				return false;
			}
		}

		return true;
	}
}