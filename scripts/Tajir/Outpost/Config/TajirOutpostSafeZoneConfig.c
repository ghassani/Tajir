/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirOutpostSafeZoneConfig - Data read from JSON configuration
 */
class TajirOutpostSafeZoneConfig
{
	bool enabled;
	bool remove_threats;
	ref array<string> threats;
	bool unlock_melee;
	bool unlock_weapon_fire;
	bool unlock_trap;
	bool unlock_restraining;
	bool unlock_bandaging;
	bool unlock_sewing;
	bool unlock_collect_blood;
	bool unlock_defibrilate;
	bool unlock_give_blood;
	bool unlock_give_saline;
	bool unlock_splint;
	bool unlock_test_blood;
	bool unlock_cover_head;
	bool unlock_force_feed;
	bool unlock_gag;
	bool unlock_door_locking;

	/**
	 * @brief      { function_description }
	 */
	void TajirOutpostSafeZoneConfig()
	{
		threats 	= new array<string>;

		enabled					= false;
		remove_threats 			= false;
		unlock_melee			= false;
		unlock_weapon_fire		= false;
		unlock_trap				= false;
		unlock_restraining		= false;
		unlock_bandaging		= false;
		unlock_sewing			= false;
		unlock_collect_blood	= false;
		unlock_defibrilate		= false;
		unlock_give_blood		= false;
		unlock_give_saline		= false;
		unlock_splint			= false;
		unlock_test_blood		= false;
		unlock_cover_head		= false;
		unlock_force_feed		= false;
		unlock_gag				= false;
		unlock_door_locking		= false;
	}

	/**
	 * @brief      Determines if enabled.
	 *
	 * @return     True if enabled, False otherwise.
	 */
	bool IsEnabled()
	{
		return enabled;
	}

	/**
	 * @brief      Removes threats.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool RemoveThreats()
	{
		return remove_threats;
	}

	/**
	 * @brief      Gets the threats.
	 *
	 * @return     The threats.
	 */
	array<string> GetThreats()
	{
		return threats;
	}

	/**
	 * @brief      Unlocks the melee.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockMelee()
	{
		return unlock_melee;
	}

	/**
	 * @brief      Unlocks the weapon fire.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockWeaponFire()
	{
		return unlock_weapon_fire;
	}

	/**
	 * @brief      Unlocks the trap.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockTrap()
	{
		return unlock_trap;
	}

	/**
	 * @brief      Unlocks the restraining.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockRestraining()
	{
		return unlock_restraining;
	}

	/**
	 * @brief      Unlocks the bandaging target.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockBandagingTarget()
	{
		return unlock_bandaging;
	}

	/**
	 * @brief      Unlocks the sewing target.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockSewingTarget()
	{
		return unlock_sewing;
	}

	/**
	 * @brief      Unlocks the collect target blood.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockCollectTargetBlood()
	{
		return unlock_collect_blood;
	}

	/**
	 * @brief      Unlocks the defibrilate target.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockDefibrilateTarget()
	{
		return unlock_defibrilate;
	}

	/**
	 * @brief      Unlocks the give target blood.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockGiveTargetBlood()
	{
		return unlock_give_blood;
	}

	/**
	 * @brief      Unlocks the give target saline.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockGiveTargetSaline()
	{
		return unlock_give_saline;
	}

	/**
	 * @brief      Unlocks the splint target.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockSplintTarget()
	{
		return unlock_splint;
	}

	/**
	 * @brief      Unlocks the test target blood.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockTestTargetBlood()
	{
		return unlock_test_blood;
	}

	/**
	 * @brief      Unlocks the cover target head.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockCoverTargetHead()
	{
		return unlock_cover_head;
	}

	/**
	 * @brief      Unlocks the force feed target.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockForceFeedTarget()
	{
		return unlock_force_feed;
	}

	/**
	 * @brief      Unlocks the gag target.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockGagTarget()
	{
		return unlock_gag;
	}

	/**
	 * @brief      Unlocks the door locking.
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool UnlockDoorLocking()
	{
		return unlock_door_locking;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Serialize( Serializer s )
	{
		s.Write( enabled );
		s.Write( remove_threats );
		s.Write( threats );
		s.Write( unlock_melee );
		s.Write( unlock_weapon_fire );
		s.Write( unlock_trap );
		s.Write( unlock_restraining );
		s.Write( unlock_bandaging );
		s.Write( unlock_sewing );
		s.Write( unlock_collect_blood );
		s.Write( unlock_defibrilate );
		s.Write( unlock_give_blood );
		s.Write( unlock_give_saline );
		s.Write( unlock_splint );
		s.Write( unlock_test_blood );
		s.Write( unlock_cover_head );
		s.Write( unlock_force_feed );
		s.Write( unlock_gag );
		s.Write( unlock_door_locking );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Deserialize( Serializer s )
	{
		s.Read( enabled );
		s.Read( remove_threats );
		s.Read( threats );
		s.Read( unlock_melee );
		s.Read( unlock_weapon_fire );
		s.Read( unlock_trap );
		s.Read( unlock_restraining );
		s.Read( unlock_bandaging );
		s.Read( unlock_sewing );
		s.Read( unlock_collect_blood );
		s.Read( unlock_defibrilate );
		s.Read( unlock_give_blood );
		s.Read( unlock_give_saline );
		s.Read( unlock_splint );
		s.Read( unlock_test_blood );
		s.Read( unlock_cover_head );
		s.Read( unlock_force_feed );
		s.Read( unlock_gag );
		s.Read( unlock_door_locking );
	}
}