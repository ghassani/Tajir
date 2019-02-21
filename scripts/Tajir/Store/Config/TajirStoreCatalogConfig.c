/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirStoreCatalogConfig - Data read from JSON configuration
 */
class TajirStoreCatalogConfig
{
	string 										 name;
	bool 										 hidden;
	ref array<ref TajirStoreCatalogItemConfig> 	 items;

	/**
	 * @brief      { function_description }
	 */
	void TajirStoreCatalogConfig()
	{
		items 	= new array<ref TajirStoreCatalogItemConfig>;
	}
}
