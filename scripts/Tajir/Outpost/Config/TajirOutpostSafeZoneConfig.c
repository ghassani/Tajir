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
}