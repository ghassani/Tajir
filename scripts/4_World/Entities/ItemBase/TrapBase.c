/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class TrapBase
{
	override void StartActivate( PlayerBase player )
	{
		if ( player.IsInSafeZone() && !player.GetOutpost().GetSafeZoneConfig().UnlockTrap() )
		{
			return;
		}

		super.StartActivate(player);
	}

	override void Explode( string ammoType = "" )
	{
		TajirLogD( "Explode", ClassName() );

		super.Explode(ammoType);
	}
}