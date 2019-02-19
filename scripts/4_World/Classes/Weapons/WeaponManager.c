/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class WeaponManager
{
	override bool CanFire( Weapon_Base wpn )
	{
		if ( m_player.IsInSafeZone() && !m_player.GetOutpost().GetSafeZoneConfig().UnlockWeaponFire() )
		{
			return false;
		}

		return super.CanFire( wpn );
	}
}