/*
 * This file is part of the Tajir package.
 *
 * (c) Gassan Idriss <ghassani@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

modded class MissionGameplay
{
	protected ref TajirManager m_tajirManager;

	void MissionGameplay()
	{
		m_tajirManager = new TajirManager();

		m_tajirManager.RegisterComponent( new TajirCurrencyComponent( m_tajirManager ) );
		m_tajirManager.RegisterComponent( new TajirItemComponent( m_tajirManager ) );
		m_tajirManager.RegisterComponent( new TajirStoreComponent( m_tajirManager ) );
		m_tajirManager.RegisterComponent( new TajirMerchantComponent( m_tajirManager ) );
		m_tajirManager.RegisterComponent( new TajirOutpostComponent( m_tajirManager ) );
#ifdef TAJIR_DEBUG_MENU
		m_tajirManager.RegisterComponent( new TajirDebugComponent( m_tajirManager ) );
#endif
	}

	void ~MissionGameplay()
	{
		if ( m_tajirManager )
		{
			delete m_tajirManager;
		}
	}

	override void OnInit()
	{
		super.OnInit();
		
		m_tajirManager.Init();
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
		m_tajirManager.OnMissionStart();

	}

	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		m_tajirManager.OnMissionFinish();
	}
}
