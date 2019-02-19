/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class OffroadHatchback
{
    override bool CanReleaseAttachment( EntityAI attachment )
	{
        if ( !super.CanReleaseAttachment( attachment ) )
        {
            return false;
        }
        
        if ( IsInSafeZone() )
        {
            return false;
        }
        
        return true;
    }

	override bool CanReleaseCargo (EntityAI cargo)
	{
        if ( !super.CanReleaseCargo( cargo ) )
        {
            return false;
        }
        
        if ( IsInSafeZone() )
        {
            return false;
        }

        return true;
	}

	override bool CanRemoveFromCargo (EntityAI parent)
	{
        if ( !super.CanRemoveFromCargo( parent ) )
        {
            return false;
        }
        
        if ( IsInSafeZone() )
        {
            return false;
        }

        return true;
	}
}