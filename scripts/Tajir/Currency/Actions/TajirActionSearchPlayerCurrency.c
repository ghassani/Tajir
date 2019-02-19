/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirActionSearchPlayerCurrencyCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime( UATimeSpent.DEFAULT_DEPLOY );
	}
};

class TajirActionSearchPlayerCurrency: ActionContinuousBase
{	
	/**
	 * @brief      { function_description }
	 */
	void TajirActionSearchPlayerCurrency()
	{
		m_CallbackClass = TajirActionSearchPlayerCurrencyCB;
		m_CommandUID 	= DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody 		= true;
		m_StanceMask 	= DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Sound 		= "craft_universal_0";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}

	/**
	 * @brief      Creates condition components.
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void CreateConditionComponents()  
	{
		m_ConditionItem   = new CCINone;
		m_ConditionTarget = new CCTMan( UAMaxDistances.DEFAULT, false );
	}	
	
	/**
	 * @brief      Gets the type.
	 *
	 * @return     The type.
	 */
	override int GetType()
	{
		return AT_ACTIVATE_TRAP;
		//return AT_TAKE_ITEM;
	}

	/**
	 * @brief      Gets the text.
	 *
	 * @return     The text.
	 */
	override string GetText()
	{
		return "Search Pockets";
	}
	
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  player  The player
	 * @param[in]  target  The target
	 * @param[in]  item    The item
	 *
	 * @return     { description_of_the_return_value }
	 */
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ManBase man = ManBase.Cast( target.GetObject() );

		if ( man )
		{
			if ( !man.IsAlive() || man.IsRestrained() || man.IsUnconscious() )
			{
				return true;
			}
		}
		
		return false;
	}
	
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  action_data  The action data
	 *
	 * @return     { description_of_the_return_value }
	 */
	override void OnFinishProgressServer( ActionData action_data )
	{		
		int found = 0;
		ManBase man = ManBase.Cast( action_data.m_Target.GetObject() );

		if ( man )
		{
			found = man.GetCurrency();
			
			if ( found > 0 )
			{
				action_data.m_Player.AddCurrency( found );
			}

			man.SetCurrency( 0 );
		}

		action_data.m_Player.RPCSingleParam( TajirCurrencyComponentRPC.ServerRPC_SearchPlayerCurrencyResult, new Param1<int>( found ), true, action_data.m_Player.GetIdentity() );

		super.OnFinishProgressServer( action_data );
	}
};