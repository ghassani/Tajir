/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      All base TajirManager RPC 
 */
enum TajirManagerRPC
{
	// Server
	ServerRPC_SendNotification 		= 0x544A5200, // 3 params, TajirNotificationType, string, int 
	ServerRPC_QueueNotification 	= 0x544A5201, // 4 params, TajirNotificationType, string, int , int
}