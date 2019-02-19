/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirItemList
{
	protected ref array<ref TajirItem> 	m_items;

	/**
	 * @brief      { function_description }
	 */
	void TajirItemList()
	{
		m_items = new array<ref TajirItem>;
	}

	/**
	 * @brief      Adds an item.
	 *
	 * @param[in]  TajirItem  The tajir item
	 */
	void AddItem( ref TajirItem item )
	{
		m_items.Insert( item );
	}

	/**
	 * @brief      Gets the items.
	 *
	 * @return     The items.
	 */
	array<ref TajirItem> GetItems()
	{
		return m_items;
	}
}