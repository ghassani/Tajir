/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirItemBundle
{
	protected TajirItem 						m_item;
	protected TajirItemBundle  					m_parent;
	protected TajirItemBundleConfig 			m_config;
	protected ref array<ref TajirItemBundle> 	m_children;

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirItem              The tajir item
	 * @param[in]  TajirItemBundleConfig  The tajir item bundle configuration
	 * @param[in]  TajirItemBundle        The tajir item bundle
	 */
	void TajirItemBundle( notnull ref TajirItem item, notnull ref TajirItemBundleConfig config, ref TajirItemBundle parent = NULL )
	{
		m_item 		= item;
		m_config 	= config;
		m_parent 	= parent;
		m_children  = new array<ref TajirItemBundle>;

		for ( int i = 0; i < m_config.items.Count(); i++ )
		{
			AddChild( new TajirItemBundle( m_item, m_config.items[ i ], this ) );
		}
	}

	/**
	 * { item_description }
	 */
	protected void Init()
	{

	}

	/**
	 * @brief      Gets the item.
	 *
	 * @return     The item.
	 */
	TajirItem GetItem()
	{
		return m_item;
	}

	/**
	 * @brief      Gets the object name.
	 *
	 * @return     The object name.
	 */
	string GetObjectName()
	{
		return m_config.object_name;
	}

	/**
	 * @brief      Gets the quantity.
	 *
	 * @return     The quantity.
	 */
	int GetQuantity()
	{
		return m_config.quantity;
	}

	/**
	 * @brief      Gets the children.
	 *
	 * @return     The children.
	 */
	array<ref TajirItemBundle> GetChildren()
	{
		return m_children;
	}

	/**
	 * @brief      Gets the parent.
	 *
	 * @return     The parent.
	 */
	TajirItemBundle GetParent()
	{
		return m_parent;
	}

	/**
	 * { item_description }
	 */
	protected void SetParent( TajirItemBundle parent )
	{
		m_parent = parent;
	}

	/**
	 * @brief      Adds a child.
	 *
	 * @param[in]  TajirItemBundle  The tajir item bundle
	 *
	 * @return     { description_of_the_return_value }
	 */
	int AddChild( notnull ref TajirItemBundle item )
	{
		if ( item.GetParent() != this )
		{
			item.SetParent( this );
		}

		m_children.Insert( item );

		return m_children.Count();
	}
}