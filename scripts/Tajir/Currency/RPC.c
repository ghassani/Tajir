/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

const int TajirCurrencyComponentRPCB

enum TajirCurrencyComponentRPC
{	
	// Server
	ServerRPC_CurrencyConfiguration 		= 0x544A5210, 	// 1 param - TajirCurrencyConfig
	ServerRPC_SearchPlayerCurrencyResult 	= 0x544A5211, 	// 1 param - int amount found
	ServerRPC_MoveFundsToHandsResult 		= 0x544A5212, 	// 1 param - int result
	// Client
	ClientRPC_MoveFundsToHands 				= 0x544A5213 	// 1 param - int amount
}