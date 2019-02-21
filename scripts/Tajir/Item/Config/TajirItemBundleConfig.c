/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirItemBundleConfig - Data read from JSON configuration
 */
class TajirItemBundleConfig
{
	string 	object_name;
	int 	quantity;
	ref array<ref TajirItemBundleConfig> items;

	/**
	 * @brief      { function_description }
	 */
	void TajirItemBundleConfig()
	{
		items = new array<ref TajirItemBundleConfig>;
	}
}