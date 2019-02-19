/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirOutpostObject
{
	protected TajirOutpost m_outpost;

	protected TajirOutpostObjectConfig m_config;
	
	protected Object m_object;
	
	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirOutpost              The tajir outpost
	 * @param[in]  TajirOutpostObjectConfig  The tajir outpost object configuration
	 * @param[in]  object                    The object
	 */
	void TajirOutpostObject( notnull TajirOutpost outpost, notnull TajirOutpostObjectConfig config, Object object )
	{
		m_outpost = outpost;
		m_config  = config;
		m_object  = object;

		InitObject();
	}

	/**
	 * @brief      Destroys the object.
	 */
	void ~TajirOutpostObject()
	{
		if ( m_object )
		{
			if ( m_object.IsEntityAI() && GetGame().IsServer() )
			{
				EntityAI.Cast( m_object ).Delete();
			}			
		}
	}
	
	/**
	 * @brief      Gets the configuration.
	 *
	 * @return     The configuration.
	 */
	TajirOutpostObjectConfig GetConfig()
	{
		return m_config;
	}

	/**
	 * @brief      Gets the object.
	 *
	 * @return     The object.
	 */
	Object GetObject()
	{
		return m_object;
	}

	/**
	 * { item_description }
	 */
	protected void InitObject()
	{
		if ( GetGame().IsMultiplayer() && GetGame().IsClient() )
		{
			return;
		}
		
		m_object.SetPosition( m_config.position );
		m_object.SetOrientation( m_config.orientation );

		if ( m_object.IsItemBase() )
		{
			ItemBase item = ItemBase.Cast( m_object );
			
			SetupSpawnedItem( item, item.GetMaxHealth(), 1 );
			
			item.SetQuantity( 1 );
		}
	}
}