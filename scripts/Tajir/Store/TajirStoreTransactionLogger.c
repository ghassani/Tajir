/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

class TajirStoreTransactionLogger extends TajirFileLogger
{
	/**
	 * @brief      Logs a transaction.
	 *
	 * @param[in]  transaction  The transaction
	 */
	void LogTransaction( TajirStoreTransaction transaction )
	{
		string line = string.Format( "ID: %1 Merchant: %2 Store: %3 Player: %4 Result: %5", transaction.GetId(), transaction.GetRequest().GetMerchantId(), transaction.GetRequest().GetStoreId(), transaction.GetPlayer().GetIdentity().GetId(), transaction.GetError() );
		
		// TODO: add created objects
		
		WriteLine( CreateUTCTimestampISO8601(), line, typename.EnumToString( TajirStoreTransactionRequestType, transaction.GetRequest().GetType() ) );
	}

	/**
	 * { item_description }
	 */
	private string CreateUTCTimestampISO8601()
	{
		int hour;
		int minute;
		int second;
		int year;
		int month;
		int day;

		GetHourMinuteSecondUTC( hour, minute, second );
		GetYearMonthDayUTC( year, month, day );

		return string.Format( "%1-%2-%3T%4:%5:%6Z", year.ToStringLen( 4 ), month.ToStringLen( 2 ), day.ToStringLen( 2 ), hour.ToStringLen( 2 ),  minute.ToStringLen( 2 ), second.ToStringLen( 2 ) );
	}
}