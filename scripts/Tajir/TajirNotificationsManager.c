/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirNotificationsManager
{
	protected ref array<ref TajirNotification> m_notifications;
	
	protected Widget m_notificationsFrame;

	protected float m_pad = 0.01;

	/**
	 * @brief      Constructor
	 */
	void TajirNotificationsManager()
	{
		m_notifications = new array<ref TajirNotification>();

		m_notificationsFrame = GetGame().GetWorkspace().CreateWidgets( "Tajir/gui/layouts/notification_frame.layout" );
		m_notificationsFrame.Show( true );
	}

	/**
	 * @brief      Destructor
	 */
	void ~TajirNotificationsManager()
	{
		if ( m_notificationsFrame != NULL )
		{
			delete m_notificationsFrame;
		}
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  type          The type
	 * @param[in]  message       The message
	 * @param[in]  closeAfterMs  The close after milliseconds
	 */
	void ShowNotification( TajirNotificationType type, string message, int closeAfterMs )
	{
		ref TajirNotification notification = new TajirNotification( message, closeAfterMs, 0, type );

		m_notifications.Insert( notification );

		Show( notification );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  type          The type
	 * @param[in]  message       The message
	 * @param[in]  showAfterMs   The show after milliseconds
	 * @param[in]  closeAfterMs  The close after milliseconds
	 */
	void QueueNotification( TajirNotificationType type, string message, int showAfterMs, int closeAfterMs )
	{
		if ( showAfterMs == 0 )
		{
			ShowNotification( type, message, closeAfterMs );
			return;
		}

		ref TajirNotification notification = new TajirNotification( message, closeAfterMs, showAfterMs, type );

		m_notifications.Insert( notification );

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.Show, showAfterMs, false, notification );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirNotification  The tajir notification
	 */
	void Show( ref TajirNotification notification )
	{				
		m_notificationsFrame.AddChild( notification.GetWidget() );

		if ( m_notifications.Count() > 1 )
		{
			Vector2 pos 		= new Vector2( 0, 0 );
			Vector2 newPos 		= new Vector2( 0, 0 );
			Vector2 dimensions 	= new Vector2( 0, 0 );

			notification.GetWidget().GetPos( newPos.x, newPos.y );
			notification.GetWidget().GetSize( dimensions.x, dimensions.y );

			for ( int i = 0; i < m_notifications.Count(); i++ )
			{
				if ( m_notifications[ i ] == notification )
				{
					continue;
				}

				m_notifications[ i ].GetWidget().GetPos( pos.x, pos.y );
				m_notifications[ i ].GetWidget().GetSize( dimensions.x, dimensions.y );
				
				newPos.y += dimensions.y + m_pad;
			}

			notification.GetWidget().SetPos( newPos.x, newPos.y );
		}

		notification.Show();

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.Close, notification.CloseAfterMilliseconds(), false, notification );
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  TajirNotification  The tajir notification
	 */
	void Close( ref TajirNotification notification )
	{
		notification.Hide();

		m_notificationsFrame.RemoveChild( notification.GetWidget() );

		for ( int i = 0; i < m_notifications.Count(); i++ )
		{
			if ( m_notifications[ i ] == notification )
			{
				m_notifications.Remove( i );
				break;
			}
		}

		delete notification;

		if ( m_notifications.Count() > 0 )
		{
			Vector2 pos 		= new Vector2( 0, 0 );
			Vector2 newPos 		= new Vector2( 0, 0 );
			Vector2 dimensions 	= new Vector2( 0, 0 );

			for ( i = 0; i < m_notifications.Count(); i++ )
			{
				m_notifications[ i ].GetWidget().GetPos( pos.x, pos.y );
				m_notifications[ i ].GetWidget().GetSize( dimensions.x, dimensions.y );
				
				m_notifications[ i ].GetWidget().SetPos( newPos.x, newPos.y );				

				newPos.y += dimensions.y + m_pad;
			}
		}
	}

	/**
	 * @brief      Reads a notification from server.
	 *
	 * @param[in]  ctx   The context
	 */
	void ReadNotificationFromServer( ParamsReadContext ctx )
	{
		ref Param3<TajirNotificationType, string, int> data = new Param3<TajirNotificationType, string, int>( TajirNotificationType.Info, "", 0 );
		
		if ( !ctx.Read( data ) ) 
		{
			TajirLogE( "Error Reading Notification From Server", ClassName() );
			return;
		}

		ShowNotification( data.param1, data.param2, data.param3 );
	}

	/**
	 * @brief      Reads a queue notification from server.
	 *
	 * @param[in]  ctx   The context
	 */
	void ReadQueueNotificationFromServer( ParamsReadContext ctx )
	{
		ref Param4<TajirNotificationType, string, int, int> data = new Param4<TajirNotificationType, string, int, int>( TajirNotificationType.Info, "", 0, 0 );
		
		if ( !ctx.Read( data ) ) 
		{
			TajirLogE( "Error Reading Notification From Server", ClassName() );
			return;
		}

		QueueNotification(  data.param1, data.param2, data.param3, data.param4 );
	}
}