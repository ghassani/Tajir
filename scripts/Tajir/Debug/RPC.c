/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

enum TajirDebugComponentRPC
{	
	// Server
	ServerRPC_UpdatePlayerResponse 		= 0x544A5300, // 1 param - TajirEntityHealthSaveState
	ServerRPC_StoreInfoResponse 		= 0x544A5301, // 1 param - TajirDebugStoreInfo
	ServerRPC_MerchantInfoResponse 		= 0x544A5302, // 1 param - TajirDebugMerchantInfo
	ServerRPC_OutpostInfoResponse 		= 0x544A5303, // 1 param - TajirDebugOutpostInfo
	
	// Client
	ClientRPC_UpdatePlayerRequest 			= 0x544A5304, // 1 param bool
	ClientRPC_SpawnManWithCurrency 			= 0x544A5305, // 1 param vector - position
	ClientRPC_SpawnManWithCurrencyAndItems 	= 0x544A5306, // 1 param vector - position
	ClientRPC_GiveCurrency 					= 0x544A5307, // 1 param vector - position
	ClientRPC_GiveAllCurrency 				= 0x544A5308,
	ClientRPC_GetStoreInfo 					= 0x544A5309, // 1 param
	ClientRPC_GetMerchantInfo 				= 0x544A5310, // 1 param
	ClientRPC_GetOutpostInfo 				= 0x544A5311, // 1 param
	ClientRPC_TeleportOutpost 				= 0x544A5312, // 1 param - int id of outpost
	ClientRPC_TeleportMerchant 				= 0x544A5313, // 1 param - int id of merchant
	ClientRPC_UpdateSafeZoneSettings 		= 0x544A5314, // 2 param - int id of outpost, TajirOutpostSafeZoneConfig
	ClientRPC_SpawnObjectAtPosition 		= 0x544A5315, // 2 param - string, vector
	ClientRPC_UpdatePlayerStat 				= 0x544A5315, // 2 param - int, int
}