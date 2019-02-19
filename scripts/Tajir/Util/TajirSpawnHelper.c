/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      Utility class used in assistance of spawning objects.
 */
class TajirSpawnHelper
{
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  posX  The position x
	 * @param[in]  posZ  The position z
	 *
	 * @return     { description_of_the_return_value }
	 */
	static vector SnapToGround( float posX, float posZ )
	{	    
	    return Vector( posX, GetGame().SurfaceY( posX, posZ ), posZ );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  typeName  The type name
	 *
	 * @return     { description_of_the_return_value }
	 */
	static bool EntityRequiresAI( string typeName )
	{
		return GetGame().IsKindOf( typeName, "DZ_LightAI" );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  EntityAI  The entity ai
	 * @param[in]  fill      The fill
	 */
	static void SetUpNewEntity( notnull EntityAI entity )
	{
		ItemBase item;
		Car car;

		ref array<string> damageZones = new array<string>;
				
		if ( entity.IsItemBase() )
		{
			item = ItemBase.Cast( entity );

			if ( item.HasQuantity() )
			{
				item.SetQuantity( item.GetQuantityInit() );
			}

			if ( item.HasEnergyManager() )
			{
				item.GetCompEM().SetEnergy( item.GetCompEM().GetEnergyMaxPristine() );
			}
		}
				
		entity.SetHealth( "GlobalHealth", "Health", entity.GetMaxHealth( "GlobalHealth", "Health" ) );

		entity.GetDamageZones( damageZones );

		foreach ( string damageZone : damageZones )
		{
			entity.SetHealth( damageZone, "", entity.GetMaxHealth( damageZone, "Health" ) );
		}

		if ( entity.IsTransport() && entity.IsInherited( Car ) )
		{
			car = Car.Cast( entity );

			if ( car )
			{
				car.Fill( CarFluid.FUEL, 	car.GetFluidCapacity( CarFluid.FUEL ) );
				car.Fill( CarFluid.OIL, 	car.GetFluidCapacity( CarFluid.OIL ) );
				car.Fill( CarFluid.BRAKE, 	car.GetFluidCapacity( CarFluid.BRAKE ) );
				car.Fill( CarFluid.COOLANT, car.GetFluidCapacity( CarFluid.COOLANT ) );
			} 
		}

		entity.SetSynchDirty();
	}
}