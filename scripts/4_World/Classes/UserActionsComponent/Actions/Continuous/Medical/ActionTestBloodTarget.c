/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class ActionTestBloodTarget
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( GetGame().IsServer() )
		{
			if ( player.IsInSafeZone() && !player.GetOutpost().GetSafeZoneConfig().UnlockTestTargetBlood() )
			{
				return false;
			}
		}

		return super.ActionCondition( player, target, item );
	}
}