/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirMerchantTransportBayConfig - Data read from JSON configuration
 */
class TajirMerchantTransportBayConfig
{
	vector  position;
	vector  orientation;
	int 	radius;

	void Serialize( Serializer s )
	{
		s.Write( position );
		s.Write( orientation );
		s.Write( radius );	
	}

	void Deserialize( Serializer s )
	{
		s.Read( position );
		s.Read( orientation );
		s.Read( radius );	
	}
}