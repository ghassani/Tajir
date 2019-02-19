/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class ItemBase
{
	override void EEHitBy( TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos )
	{
		EntityAI root = GetHierarchyRoot();

		if ( root && root.IsInherited( PlayerBase ) )
		{
			PlayerBase player = PlayerBase.Cast( root );

			if ( player.IsInSafeZone() )
			{
				TajirLogD( string.Format( "%1 Hit In %2 by %3 [ ROOT: %4 ]", GetType(), dmgZone, source.GetType(), GetHierarchyRoot().GetType() ), ClassName() );

				TajirHealthHelper.RestoreTotalDamageResult( this, damageResult );

				return;
			}
		}
		else if ( root && root.IsInherited( Transport ) )
		{
			if ( root.IsInherited( CarScript ) )
			{
				CarScript car = CarScript.Cast( root );

				if ( car.IsInSafeZone() )
				{
					TajirLogD( string.Format( "%1 Hit In %2 by %3 [ ROOT: %4 ]", GetType(), dmgZone, source.GetType(), GetHierarchyRoot().GetType() ), ClassName() );

					TajirHealthHelper.RestoreTotalDamageResult( this, damageResult );

					return;
				}
			}
		}

		super.EEHitBy( damageResult, damageType, source, component, dmgZone, ammo, modelPos );
	}
}