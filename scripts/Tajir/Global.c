/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      { function_description }
 *
 * @param[in]  message  The message
 * @param[in]  tag      The tag
 */
void TajirLogI( string message, string tag = "" )
{
	if ( TajirManager.GetInstance() && TajirManager.GetInstance().GetLogger() )
	{
		TajirManager.GetInstance().GetLogger().Info( message, tag );
	}
}

/**
 * @brief      { function_description }
 *
 * @param[in]  message  The message
 * @param[in]  tag      The tag
 */
void TajirLogE( string message, string tag = "" )
{
	if ( TajirManager.GetInstance() && TajirManager.GetInstance().GetLogger() )
	{
		TajirManager.GetInstance().GetLogger().Error( message, tag );
	}
}

/**
 * @brief      { function_description }
 *
 * @param[in]  message  The message
 * @param[in]  tag      The tag
 */
void TajirLogW( string message, string tag = "" )
{

	if ( TajirManager.GetInstance() && TajirManager.GetInstance().GetLogger() )
	{
		TajirManager.GetInstance().GetLogger().Warning( message, tag );
	}
}

/**
 * @brief      { function_description }
 *
 * @param[in]  message  The message
 * @param[in]  tag      The tag
 */
void TajirLogD( string message, string tag = "" )
{
#ifdef TAJIR_DEBUG
	if ( TajirManager.GetInstance() && TajirManager.GetInstance().GetLogger() )
	{
		TajirManager.GetInstance().GetLogger().Debug( message, tag );
	}
#endif
}