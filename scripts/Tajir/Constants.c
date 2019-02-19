/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

static const int TAJIR_CLIENT_TRANSACTION_TIMEOUT_SECONDS 	= 10;
static const int TAJIR_MERCHANT_STOP_TRIGGER_RADIUS 		= 2;
static const int TAJIR_PLAYER_CURRENCY_MIN 					= 0;
static const int TAJIR_PLAYER_CURRENCY_MAX 					= 2147483647;
static const int TAJIR_ID_MAX 								= 2147483647;

static const string TAJIR_CONFIG_DIR  				= "$profile:tajir";
static const string TAJIR_CONFIG_DIR_PREPEND  		= "tajir/"; // keep end slash

static const string TAJIR_LOG_FILE  				= "$profile:tajir.log";
static const string TAJIR_TRANSACTION_LOG_FILE  	= "$profile:" + TAJIR_CONFIG_DIR_PREPEND + "transactions.log";

static const string TAJIR_CLIENT_CONFIG_DEFAULT  	= "Tajir/data/config_client.json";
static const string TAJIR_CLIENT_CONFIG_USER 		= "$profile:/tajir_config.json";

static const string TAJIR_CURRENCY_CONFIG_DEFAULT   = "Tajir/data/currency.json";
static const string TAJIR_CURRENCY_CONFIG_USER 		= "$profile:" + TAJIR_CONFIG_DIR_PREPEND + "currency.json";

static const string TAJIR_ITEM_CONFIG_DEFAULT 		= "Tajir/data/items.json";
static const string TAJIR_ITEM_CONFIG_USER 			= "$profile:" + TAJIR_CONFIG_DIR_PREPEND + "items.json";

static const string TAJIR_STORE_CONFIG_DEFAULT 		= "Tajir/data/stores.json";
static const string TAJIR_STORE_CONFIG_USER 		= "$profile:" + TAJIR_CONFIG_DIR_PREPEND + "stores.json";

static const string TAJIR_STORE_INVENTORY_SAVE_FILE_FORMAT = "$profile:" + TAJIR_CONFIG_DIR_PREPEND + "store_%1_persist.json";

static const string TAJIR_MERCHANT_CONFIG_DEFAULT 	= "Tajir/data/merchants.json";
static const string TAJIR_MERCHANT_CONFIG_USER 		= "$profile:" + TAJIR_CONFIG_DIR_PREPEND + "merchants.json";

static const string TAJIR_OUTPOST_CONFIG_DEFAULT 	= "Tajir/data/outposts.json";
static const string TAJIR_OUTPOST_CONFIG_USER 		= "$profile:" + TAJIR_CONFIG_DIR_PREPEND + "outposts.json";

static const string TAJIR_KEYBIND_PREFIF 			= "tajir_";

const int TAJIR_MENU_STORE_ID	= 0x544A5200;
const int TAJIR_MENU_DEBUG_ID	= 0x544A5201;

const int TAJIR_AT_SEARCH_PLAYER_CURRENCY = 0x544A5200;

const int TARJIR_RPC_BASE 		= 0x544A5200;

const int TAJIR_SEARCH_PLAYER_CURRENCY_ACTION_TIME = 5;

#ifdef TAJIR_DEBUG
	static const int TAJIR_DEFAULT_PLAYER_START_CURRENCY = 100000;
#else
	static const int TAJIR_DEFAULT_PLAYER_START_CURRENCY = 0;
#endif

#ifdef TAJIR_SIMULATE_SERVER_TRANSACTION_TIMEOUT
	static const int TAJIR_SIMULATE_SERVER_TRANSACTION_TIMEOUT_SECONDS = 5;
#endif