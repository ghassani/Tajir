/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

enum TajirNotificationType
{
	Info = 1
	Success,
	Error
}

class TajirNotification
{
	protected Widget m_widget;

	protected Widget m_iconPanel;

	protected TextWidget m_textWidget;

	protected TextWidget m_iconTextWidget;
	
	protected string m_text;

	protected int m_closeAfterMs 	= 1000;

	protected int m_showAfterMs 	= 0;

	protected TajirNotificationType m_type = TajirNotificationType.Info;

	/**
	 * @brief      Constructor
	 *
	 * @param[in]  message       The message
	 * @param[in]  closeAfterMs  The close after milliseconds
	 * @param[in]  showAfterMs   The show after milliseconds
	 * @param[in]  type          The type
	 */
	void TajirNotification( string message, int closeAfterMs, int showAfterMs = 0, TajirNotificationType type = TajirNotificationType.Info )
	{
		m_text 			= message;
		m_closeAfterMs 	= closeAfterMs;
		m_showAfterMs 	= showAfterMs;
		m_type 			= type;

		m_widget = GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/notification.layout" );

		if ( m_widget == NULL )
		{
			TajirLogE( "No Notification Widget Initialized", ClassName() );
			return;
		}

		m_iconPanel  		= m_widget.FindAnyWidget( "NotificationIcon" );
		m_textWidget  		= TextWidget.Cast( m_widget.FindAnyWidget( "NotificationText" ) );
		m_iconTextWidget  	= TextWidget.Cast( m_widget.FindAnyWidget( "NotificationIconText" ) );
		
		m_textWidget.SetText( m_text );

		switch ( m_type )
		{
			case TajirNotificationType.Info:
				m_widget.SetColor( 0xFF8195CF );
				m_iconPanel.SetColor( 0xFF002080 );
				m_iconTextWidget.SetText( "I" );
				break;
			case TajirNotificationType.Success:
				m_widget.SetColor( 0xFF289f46 );
				m_iconPanel.SetColor( 0xFF008020 );
				m_iconTextWidget.SetText( "S" );
				break;
			case TajirNotificationType.Error:
				m_widget.SetColor( 0xFFCF8181 );
				m_iconPanel.SetColor( 0xFF800000 );
				m_iconTextWidget.SetText( "E" );
				break;
		}

		m_widget.Show( false );
	}

	/**
	 * @brief      Destructor
	 */
	void ~TajirNotification()
	{
		if ( m_widget != NULL )
		{
			delete m_widget;
		}
	}

	/**
	 * @brief      Determines if visible.
	 *
	 * @return     True if visible, False otherwise.
	 */
	bool IsVisible()
	{
		if ( m_widget != NULL )
		{
			return m_widget.IsVisible();
		}

		return false;
	}

	/**
	 * @brief      Gets the widget.
	 *
	 * @return     The widget.
	 */
	Widget GetWidget()
	{
		return m_widget;
	}

	/**
	 * @brief      Gets the text.
	 *
	 * @return     The text.
	 */
	string GetText()
	{
		return m_text;
	}

	/**
	 * @brief      Gets the show after milliseconds.
	 *
	 * @return     The show after milliseconds.
	 */
	int ShowAfterMilliseconds()
	{
		return m_showAfterMs;
	}

	/**
	 * @brief      Gets the close after milliseconds.
	 *
	 * @return     The close after milliseconds.
	 */
	int CloseAfterMilliseconds()
	{
		return m_closeAfterMs;
	}

	/**
	 * @brief      { function_description }
	 */
	void Show()
	{
		m_widget.Show( true );
	}

	/**
	 * @brief      { function_description }
	 */
	void Hide()
	{
		m_widget.Show( false );
	}
}