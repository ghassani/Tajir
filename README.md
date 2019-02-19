
# Tajir

Tajir is a dynamic merchant mod for DayZ Standalone. 

`Tajir` means Trader in Arabic `تاجر`

# Features
 - Configurable Stores, Merchants, Outposts with JSON configuration files.
 - Dynamic Inventory
   - Configurable item level restock quantity rules.
   - Configurable store level restock time frame.
   - Persisted inventory levels.
 - Optionally share stores and inventory levels between merchants.
 - Configurable `Transport` spawn locations
 - Configurable Merchant clothing
 - Configurable Outposts
   - Configurable Safe Zone
     - Enable/Disable Safezone
     - Enable/Disable a number of actions.
   - Configurable custom object placement 
 - World currency objects with "virtual" player currency wallet
 - Complex item creation
   - Create bundled items like vehicles with parts and/or cargo. weapons with attachments. 
 
# Installation

# Client Configuration

When you first load the game with the module enabled, a `client_config.json` file will be created in your profile directory. In this file you can adjust the client side settings.

### Keybinds
|Name|Default||
|--|--|--|
|toggle_merchant_shop|B|Toggles a merchant store menu when in distance.

Currently, the only configurable client side option is the keybinding to toggle a merchant shop. The 

# Server Configuration

Configuring your stores and inventory can be done by editing the json configuration files. In addition, you can use the store builder tool to configure it with a graphical user interface.


# JSON Configuration Overview

This section details all configuration option json format. 

## Enfusion specific JSON types

### vector3

A `Vector3` is represented by an array of exactly 3 float values.

    "position": [ 1.11, 2.22, 3.33 ]

## Important JSON formatting notes
At the time of this writing the enfusion engine has some odd behavior with parsing json files. Note the following:

- No blank lines.
- Use integer values for booleans.

### JSON Object `TajirConfig`

This object represents the client side configuration settings.

|Field|Type|Required||
|--|--|--|--|
|keybindings|TajirConfigKeyBinding[]|Yes|Array of keybind settings.

### JSON Object `TajirConfigKeyBinding`

This object represents a keybinding entry in `TajirConfig`.

|Field|Type|Required||
|--|--|--|--|
|name|string|Yes|The unique keybinding name.
|keybinds|string[]|Yes|Array of string keybinds.

### JSON Object: `TajirItem`
This object represents an item configuration. This is the base of every sellable/buyable item and where bundled items are configured.

|Field|Type|Required||
|--|--|--|--|
|id|int|Yes|A unique id identifying the item configuration.|
|object_name|string|Depends|The game object name. Required if not a bundle.|
|display_name|string|No|Optionally override the objects display name.|
|description|string|No|Optionally override the objects description.|
|display_object_name|string|No|Optionally specify what object to display in item preview. Default is object_name.|
|sell_price|int|Yes|Specify the sale price of the item.|
|buy_price|int|Yes|Specify the buy price of the item.|
|items|TajirStoreCatalogBundleItem[]|No|Nesting an array of items turns this item into a bundled item.|

### JSON Object `TajirItemBundle`
This object represents a bundled item in a `TajirItem` object. This object allows for recursive items as attachments/cargo.

|Field|Type|Required||
|--|--|--|--|
|object_name|string|Yes|The game object to create.|
|quantity|int|Yes|The quantity of the item to create.|
|items|TajirStoreCatalogBundleItem[]|No|Recursively add child items as attachment/cargo.|

### JSON Object `TajirStore`

This object represents a store configuration.

|Field|Type|Required||
|--|--|--|--|
|id|integer|Yes|Unique ID to identify this store configuration.|
|name|string|Yes|The name of the store.|
|type|string|Yes|The type of store. Currently only `default` is supported.|
|log_transactions|bool|No|Optionally log this stores transactions to server profiles directory.
|show_out_of_stock|bool|No|Enable to hide the catalog from being displayed.
|save_every_minutes|int|No|The time in minutes to save store inventory levels to disk.|
|restock_every_minutes|int|No|The time in minutes to restock this stores inventory levels.|
|allowed_conditions|int[]|Yes|An array of integer values representing item conditions: `0 = PRESTINE`, `1 = WORN`, `2 = DAMAGED`, `3 = VERY DAMAGED`. Destroyed is never accepted.
|catalogs|TajirStoreCatalog[]|Yes|The store catalog configurations.

### JSON Object `TajirStoreCatalog`

This object represents a `TajirStore` catalog entry.

|Field|Type|Required||
|--|--|--|--|
|name|string|Yes|The name of the catalog.
|hidden|bool|No|Enable to hide the catalog from players.
|items|TajirStoreCatalogItem[]|Yes|Array of items this catalog contains.


### JSON Object `TajirStoreCatalogItem`

This object represents a `TajirStoreCatalog` item.

|Field|Type|Required||
|--|--|--|--|
|item_id|int|Yes|The TajirItem id this item represents.|
|can_sell|bool|Yes|Whether this store can sell this item to players.|
|can_buy|bool|Yes|Whether this store can buy this from players.|
|quantity|int|No|The starting quantity available for sale. Server manages this value during runtime and will change from saved data.|
|restock_quantity|int|No|The amount to restock when this store triggers a restock.|

### JSON Object `TajirMerchant`

This object represents a merchant configuration.

|Field|Type|Required||
|--|--|--|--|
|id|int|Yes|A unique ID to identify the merchant.|
|name|string|Yes|A name for the merchant.|
|object_name|string|Yes|The game object name to represent this merchant.|
|store|int|Yes|The `TajirStore` ID this merchant uses.|
|unkillable|bool|No|Whether to keep the merchant alive or not if placed outside of an outpost safe zone.|
|start_position|vector3|Yes|The starting world position.|
|start_orientation|vector3|Yes|The starting world orientation.|
|clothing|TajirMerchantClothing|No|Optional Man merchant clothing options.|
|transport_bays|TajirMerchantTransportBay[]|No|An array of transport bays this merchant can spawn transports at.|

### JSON Object `TajirMerchantClothing`

This object represents a `TarjirMerchant` clothing configuration.

|Field|Type|Required||
|--|--|--|--|
|head|string|No|Game object name to place on head slot.|
|shoulder|string|No|Game object name to place on shoulder slot.|
|headgear|string|No|Game object name to place on headgear slot.|
|mask|string|No|Game object name to place on mask slot.|
|eyewear|string|No|Game object name to place on eyewear slot.|
|hands|string|No|Game object name to place on hands slot.|
|lefthand|string|No|Game object name to place on lefthand slot.|
|gloves|string|No|Game object name to place on gloves slot.|
|armband|string|No|Game object name to place on armband slot.|
|vest|string|No|Game object name to place on vest slot.|
|body|string|No|Game object name to place on body slot.|
|back|string|No|Game object name to place on back slot.|
|hips|string|No|Game object name to place on hips slot.|
|legs|string|No|Game object name to place on legs slot.|
|feet|string|No|Game object name to place on feet slot.|

### JSON Object `TajirMerchantTransportBay`

This object represents a `TajirMerchant` transportation bay location. 

|Field|Type|Required||
|--|--|--|--|
|position|vector3|Yes|The world position to spawn the transport.|
|orientation|vector3|Yes|The orientation to spawn the transport in.|
|radius|int|Yes|The radius at world position.|

### JSON Object `TajirOutpost`

This object represents an outpost configuration.

|Field|Type|Required||
|--|--|--|--|
|id|int|Yes|Unique ID to represent this outpost.|
|name|string|Yes|A name for the outpost.|
|position|vector3|Yes|Center position in world for outpost.|
|radius|int|Yes|Radius from center position.|
|safezone|TajirSafeZoneConfig|Yes|Safezone configuration.|
|objects|TajirOutpostObject[]|No|Array of game objects to place at outpost.|

### JSON Object `TajirSafeZoneConfig`

This object represents a `TajirOutpost` safe zone configuration.

|Field|Type|Required||
|--|--|--|--|
|enabled|bool|No|Enable/Disable the safezone.|
|remove_threats|bool|No|Whether threats should be auto-deleted.|
|threats|string[]|No|Array of game object names considered a threat. **Do not put any player object types in this array.**|
|unlock_melee|bool|No|Unlock melee action in outpost.|
|unlock_weapon_fire|bool|No|Unlock weapon fire action in outpost.|
|unlock_trap|bool|No|Unlock trap activation action in outpost.|
|unlock_restraining|bool|No|Unlock restraining target action in outpost.|
|unlock_bandaging|bool|No|Unlock bandaging target action in outpost.|
|unlock_sewing|bool|No|Unlock sweing target action in outpost.|
|unlock_collect_blood|bool|No|Unlock collect target blood action in outpost.|
|unlock_defibrilate|bool|No|Unlock defibrilate target action in outpost.|
|unlock_give_blood|bool|No|Unlock give target blood action in outpost.|
|unlock_give_saline|bool|No|Unlock give target saline action in outpost.|
|unlock_splint|bool|No|Unlock splint target action in outpost.|
|unlock_test_blood|bool|No|Unlock test target blood action in outpost.|
|unlock_cover_head|bool|No|Unlock cover target head action in outpost.|
|unlock_force_feed|bool|No|Unlock force feed target action in outpost.|
|unlock_gag|bool|No|Unlock gag target action in outpost.|
|unlock_door_locking|bool|No|Unlock door locking action in outpost.|

### JSON Object `TajirOutpostObject`

|Field|Type|Required||
|--|--|--|--|
|name|string|Yes|The game object name.|
|position|vector3|Yes|The position to place the object at.|
|orientation|vector3|Yes|The orientation to set the object to.|

# Obtaining Currency

Currency objects will need to be added to the loot table to in order for them to spawn on the map.

Currency can be obtained from the world, other players, and by selling items to merchants.

When you find a currency object, it redeems itself to your virtual wallet upon entering your inventory. The only exception is when you take the object to hands.

You can remove funds from your virtual wallet to stash or give away by using the Remove Funds feature added to the standard inventory menu.

You can search dead or restrained players pockets for their currency by moving within action distance, selecting the Search Pockets action and holding down the left mouse button.

# Assets
- Icons from [FontAwesome](https://fontawesome.com)
- Dollar Textures from [bigj93 on blendswap](https://www.blendswap.com/blends/view/83644)

# TODO

- Offline support
- Fix Dollar Assets

