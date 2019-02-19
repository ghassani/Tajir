/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      Represents a bound key/combo
 */
class TajirKeyBinding
{
	protected string            m_name;

	protected string            m_description;

	protected Class             m_cbInstance;

	protected string            m_cbFunction;

	protected ref array<string> m_keybinds;

	protected UAInput           m_input;

	/**
	 * @brief      Constructor
	 *
	 * @param[in]  string  The name
	 * @param[in]  string  The description
	 * @param[in]  Class   The cb instance
	 * @param[in]  string  The cb function
	 * @param[in]  string  The type
	 */
	void TajirKeyBinding( string name, string description, Class cbInstance, string cbFunction, string type = "infantry" ) 
	{
		m_name 		  = name;
		m_description = description;
		m_keybinds    = new array<string>;
		m_cbInstance  = cbInstance;
		m_cbFunction  = cbFunction;
		m_input       = GetUApi().RegisterInput( ( TAJIR_KEYBIND_PREFIF + name ), description, type );
	}

	/**
	 * @brief      Destructor
	 */
	void ~TajirKeyBinding()
	{
		if ( m_input != NULL )
		{
			GetUApi().DeRegisterInput( TAJIR_KEYBIND_PREFIF + m_name );
		}
	}

	/**
	 * @brief      Gets the callback instance.
	 *
	 * @return     Class
	 */
	Class GetCallbackInstance()
	{
		return m_cbInstance;
	}

	/**
	 * @brief      Gets the callback function to call from m_cbInstance.
	 *
	 * @return     string
	 */
	string GetCallbackFunction()
	{
		return m_cbFunction;
	}

	/**
	 * @brief      Gets the bound keybinds set using BindCombo.
	 *
	 * @return     array<string>
	 */
	array<string> GetKeybinds()
	{
		return m_keybinds;
	}

	/**
	 * @brief      Binds a key combo for the keybind
	 *
	 * @param[in]  combo  The combo
	 */
	void BindCombo( string combo )
	{
		m_keybinds.Insert( combo );
		m_input.BindCombo( combo );
	}

	/**
	 * @brief      Gets the name.
	 *
	 * @return     string
	 */
	string GetName()
	{
		return m_name;
	}

	/**
	 * @brief      Gets the description.
	 *
	 * @return     string
	 */
	string GetDescription()
	{
		return m_description;
	}

	/**
	 * @brief      Gets the input.
	 *
	 * @return     UAInput
	 */
	UAInput GetInput()
	{
		return m_input;
	}
}