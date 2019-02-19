/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirDebugMerchantInfo
{
	int id;
	
	ref ref TajirEntityHealthSaveState state;

	void TajirDebugMerchantInfo( notnull TajirMerchant merchant )
	{
		id 		= merchant.GetId();
		state 	= new TajirEntityHealthSaveState( EntityAI.Cast( merchant.GetObject() ) );
	}

	void Serialize( Serializer s )
	{
		s.Write( id );
		s.Write( state );
	}

	void Deserialize( Serializer s )
	{
		s.Read( id );
		s.Read( state );
	}
}