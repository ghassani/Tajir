/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @brief      TajirMerchantConfig - Data read from JSON configuration
 */
class TajirMerchantConfig
{
	int 	id;
	string 	name;
	string 	object_name;
	int 	store;
	bool 	unkillable;
	vector 	position;
	vector 	orientation;
	int 	network_id_low;
	int 	network_id_high;
	
	ref array<ref TajirMerchantTransportBayConfig> 	transport_bays;
	ref TajirMerchantClothingConfig 				clothing;

	void TajirMerchantConfig()
	{
		transport_bays = new array<ref TajirMerchantTransportBayConfig>;
		clothing = new TajirMerchantClothingConfig();
	}
}