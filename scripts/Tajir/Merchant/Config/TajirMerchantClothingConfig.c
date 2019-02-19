/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirMerchantClothingConfig - Data read from JSON configuration
 */
class TajirMerchantClothingConfig
{
	string head;
	string shoulder;
	string headgear;
	string mask;
	string eyewear;
	string hands;
	string lefthand;
	string gloves;
	string armband;
	string vest;
	string body;
	string back;
	string hips;
	string legs;
	string feet;

	void Serialize( Serializer s )
	{
		s.Write( head );
		s.Write( shoulder );
		s.Write( headgear );
		s.Write( mask );
		s.Write( eyewear );
		s.Write( hands );
		s.Write( lefthand );
		s.Write( gloves );
		s.Write( armband );
		s.Write( vest );
		s.Write( body );
		s.Write( back );
		s.Write( hips );
		s.Write( legs );
		s.Write( feet );		
	}

	void Deserialize( Serializer s )
	{
		s.Read( head );
		s.Read( shoulder );
		s.Read( headgear );
		s.Read( mask );
		s.Read( eyewear );
		s.Read( hands );
		s.Read( lefthand );
		s.Read( gloves );
		s.Read( armband );
		s.Read( vest );
		s.Read( body );
		s.Read( back );
		s.Read( hips );
		s.Read( legs );
		s.Read( feet );	
	}
}