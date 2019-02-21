/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirConfigKeyBinding - Data read from JSON configuration
 */
class TajirConfigKeyBinding
{
	string 				name;
	ref array<string> 	keybinds;
	
	/**
	 * @brief      { function_description }
	 */
	void TajirConfigKeyBinding()
	{
		keybinds = new array<string>;
	}
}