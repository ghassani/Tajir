/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirKeyBindManager
{
	protected ref array<ref TajirKeyBinding> m_keybindings;

	/**
	 * @brief      { function_description }
	 */
	void TajirKeyBindManager()
	{
		m_keybindings = new array<ref TajirKeyBinding>;
	}

	/**
	 * @brief      Destroys the object.
	 */
	void ~TajirKeyBindManager()
	{

	}

	/**
	 * @brief      Adds a binding.
	 *
	 * @param[in]  TajirKeyBinding  The tajir key binding
	 */
	void AddBinding( ref TajirKeyBinding binding )
	{
		TajirLogD( string.Format( "Binding %1", binding.GetName() ), ClassName() );

		m_keybindings.Insert( binding );
	}

	/**
	 * @brief      Determines if it has binding.
	 *
	 * @param[in]  name  The name
	 *
	 * @return     True if has binding, False otherwise.
	 */
	bool HasBinding( string name )
	{
		foreach ( ref TajirKeyBinding binding : m_keybindings )
		{
			if ( binding.GetName() == name )
			{
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      { function_description }
	 */
	void ClearAllBindings()
	{
		foreach ( ref TajirKeyBinding binding : m_keybindings )
		{
			delete binding;
		}

		m_keybindings.Clear();
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  name  The name
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool ClearBinding( string name )
	{
		for ( int i = 0; i < m_keybindings.Count(); ++i )
		{
			if ( m_keybindings[ i ].GetName() == name )
			{
				delete m_keybindings[ i ];

				m_keybindings.Remove( i );

				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  input  The input
	 *
	 * @return     { description_of_the_return_value }
	 */
	bool ClearBinding( UAInput input )
	{
		for ( int i = 0; i < m_keybindings.Count(); ++i )
		{
			if ( m_keybindings[ i ].GetInput() == input )
			{
				delete m_keybindings[ i ];

				m_keybindings.Remove( i );

				return true;
			}
		}

		return false;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  timeslice  The timeslice
	 */
	void OnClientUpdate( float timeslice )
	{
		foreach ( ref TajirKeyBinding binding : m_keybindings )
		{
			if ( binding.GetInput() )
			{
				GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallByName( binding.GetCallbackInstance(), binding.GetCallbackFunction(), new Param1<UAInput>( binding.GetInput() ) );
			}
		}
	}
}