/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

enum TajirStoreComponentRPC
{	
	// Server
	ServerRPC_StoreConfiguration 			= 0x544A5230, 	// 1 param - TajirStoreConfig - Sent for each store
	ServerRPC_TransactionResponse 			= 0x544A5231, 	// 1 param - TajirStoreTransactionResponse
	ServerRPC_InventoryUpdate 				= 0x544A5232, 	// 1 param - TajirStoreInventoryUpdate
	
	// Client
	ClientRPC_TransactionRequest 			= 0x544A5233, 	// 1 param - TajirStoreTransactionRequest
}