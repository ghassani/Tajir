/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class ActionConstructor
{
	override void ConstructActions( out array<ref ActionBase> actions, out TIntArray suactions, out TIntArray cactions, out TIntArray iactions )
	{
		super.ConstructActions( actions, suactions, cactions, iactions );

		//actions.Insert( new ActionSearchPlayerCurrency );
		actions.Insert( new TajirActionSearchPlayerCurrency );

	}
}
