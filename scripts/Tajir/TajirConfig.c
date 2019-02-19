/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirConfig - Data read from JSON configuration
 */
class TajirConfig
{
	ref array<ref TajirConfigKeyBinding>	keybindings;

	/**
	 * @brief      { function_description }
	 */
	void TajirConfig()
	{
		keybindings = new array<ref TajirConfigKeyBinding>;
	}

	/**
	 * @brief      Destroys the object.
	 */
	void ~TajirConfig()
	{
		foreach ( ref TajirConfigKeyBinding keybind : keybindings )
		{
			delete keybind;
		}
	}

	/**
	 * @brief      Gets the keybind configuration.
	 *
	 * @param[in]  name  The name
	 *
	 * @return     The keybind configuration.
	 */
	TajirConfigKeyBinding GetKeybindConfig( string name )
	{
		foreach ( TajirConfigKeyBinding keybind : keybindings )
		{
			if ( name == keybind.name )
			{
				return keybind;
			}
		}

		return NULL;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Serialize( Serializer s )
	{
		s.Write( keybindings );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  s     { parameter_description }
	 */
	void Deserialize( Serializer s )
	{
		s.Read( keybindings );
	}
}