/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class DayZPlayerMeleeFightLogic_LightHeavy
{
	override bool Process( int pCurrentCommandID, HumanInputController pInputs, EntityAI pEntityInHands, HumanMovementState pMovementState )
	{
		PlayerBase player = PlayerBase.Cast( m_DZPlayer );
		
		if ( player && player.IsInSafeZone() && !player.GetOutpost().GetSafeZoneConfig().UnlockMelee() )
		{
			return false;
		}

		return super.Process( pCurrentCommandID, pInputs, pEntityInHands, pMovementState );
	}
}