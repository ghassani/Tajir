/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirOutpostConfig - Data read from JSON configuration
 */
class TajirOutpostConfig
{
	int id;
	string name;
	vector position;	
	float radius;
	ref TajirOutpostSafeZoneConfig safezone;

	ref array<ref TajirOutpostObjectConfig> objects;

	/**
	 * @brief      { function_description }
	 */
	void TajirOutpostConfig()
	{
		safezone 	= new TajirOutpostSafeZoneConfig();
		objects 	= new array<ref TajirOutpostObjectConfig>;
	}
}