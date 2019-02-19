/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

enum TajirOutpostComponentRPC
{
	// Server
	ServerRPC_OutpostConfiguration 			= 0x544A5250, // 1 param - TajirOutpostConfig - Sent for each outpost,
	ServerRPC_NotifyEnteredOutpost 			= 0x544A5251, // 1 param - int the outpost id
	ServerRPC_NotifyLeftOutpost 			= 0x544A5252, // 1 param - int the outpost id
}