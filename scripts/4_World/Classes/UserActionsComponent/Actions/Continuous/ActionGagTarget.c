/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class ActionGagTarget
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( GetGame().IsServer() )
		{
			if ( player.IsInSafeZone() && !player.GetOutpost().GetSafeZoneConfig().UnlockGagTarget() )
			{
				return false;
			}

			if ( target.GetObject().IsInherited( PlayerBase ) ) 
			{
				PlayerBase targetPlayer = PlayerBase.Cast( target.GetObject() );

				if ( targetPlayer && targetPlayer.IsInSafeZone() && !targetPlayer.GetOutpost().GetSafeZoneConfig().UnlockGagTarget() )
				{
					return false;
				}
			}
		}
		
		return super.ActionCondition( player, target, item );
	}
}