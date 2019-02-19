/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class ActionGetInTransport
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( GetGame().IsServer() )
		{
			Transport transport = Transport.Cast( target.GetObject() );

			if ( transport )
			{
				if ( transport.IsKindOf( "CarScript" ) )
				{
					CarScript car = CarScript.Cast( transport );

					if ( car )
					{
						if ( car.IsInSafeZone() )
						{
							if ( car.IsAccessAllowed( player ) )
							{
								return super.ActionCondition( player, target, item );
							}
							else
							{
								return false;
								
								TajirLogD( string.Format( "Not Owner. Expected %1 Got %2", car.GetOwner(), player.GetIdentity().GetId() ), ClassName() );
							}
							
						}
					}
				}
			}
		}

		return super.ActionCondition( player, target, item );
	}

	override void Start( ActionData action_data )
	{
		/*Transport transport = Transport.Cast( action_data.m_Target.GetObject() );

		if ( transport )
		{
			if ( transport.IsKindOf( "CarScript" ) )
			{
				CarScript car = CarScript.Cast( transport );

				if ( car )
				{
					if ( car.IsInSafeZone() && car.IsOwner( action_data.m_Player ) )
					{
						return;
					}
				}
			}
		}*/
		
		super.Start( action_data );
	}
}