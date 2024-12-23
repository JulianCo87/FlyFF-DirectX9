#ifndef __HWOPTION_H__
#define	__HWOPTION_H__

class COption
{
private:
	bool	m_bStartFullScreen;

public:
	enum
	{
		MAX_CAHTFILTER = 6,
	};
	TCHAR	m_szFileName[MAX_PATH];
//	int		m_nSoundEffect;
	int		m_nResWidth;
	int		m_nResHeight;
//	float	m_fMusicVolume;
	int		m_nTextureQuality;		// 텍스쳐 품질 높음0,중간1,낮음2
	int		m_nViewArea;			// 시야범위 멀리0,중간1,가깝게2
	int		m_nObjectDetail;		// 오브젝트 표현수준 높음0,중간1,낮음2
	int     m_nObjectDistant;       // 오브젝트 거리  높음0,중간1,낮음2
	int     m_nShadow; // 그림자 
	int		m_nToggleScreen;		// 알트엔터 가능/못가능.  이건 유저들은 모른다.
	BOOL    m_bOperator;
	DWORD   m_dwChatFilter[MAX_CAHTFILTER];
	DWORD   m_dwChatFilterEtc;
	int		m_nBloom;				// 뽀샤시 효과.	
	int		m_nMonName;		// 몬스터 이름 On / Off
	int		m_nPlayerName;		    // 자신 이름 On / Off
	int		m_nOtherPlayerName;		// 다른 플레이어 이름 On / Off
	int		m_nToolTipTexture;		// 툴팁 인터페이스 바꾸기
	int		m_nToolTipText;
#ifdef __SFX_OPT
	int		m_nSfxLevel;	
#endif
	BOOL	m_nWeatherEffect;
//#ifdef __YADDSOUND
	BOOL    m_bVoice;
	float	m_fEffectVolume;
	float	m_fBGMVolume;
//#endif //__YADDSOUND

	// 윈도 옵션
	int     m_nWindowAlpha; // 0 ~ 255
	int     m_nWindowEffect; // 0(일반), 1(반투명), 2(사각)

	// 속어 단어 필터
	int     m_nSlangWord; // 0(교체), 1(숨김)

	// 채팅 명령어
	int     m_nChatCommand; // 0(한글 풀문장),1(한글축약어),2(영문풀문장),3(영문축약어)
	
	// 초보자 도움말(채팅창에 나오는 것)
	int     m_nInstantHelp; // 
	
	// 인터페이스 방식
	int		m_nInterface;	// 0 : 구버전 1 : 신버전(우클릭으로 카메라 회전하는..)
	BOOL	m_bRollEffect;	// 비행시 롤링 효과.
	
	BOOL	m_bSay;			// 귀속말 승인 / 거부
	BOOL	m_bTrade;		// 거래 승인 / 거부
	BOOL	m_bParty;		// 파티 승인 / 거부
	BOOL	m_bShout;		// 외치기 표시 ON / OFF
	BOOL	m_bMessengerJoin; // Messenger 알림 표시 On / Off
	BOOL	m_bMessenger;	// Messenger 승인 / 거부
	BOOL	m_bBattleBGM;	// 전투 음악 ON / OFF
	BOOL	m_bGameGradeRendering;

	int		m_nSer;		// 이전에 접속했던 서버
	int		m_nMSer;	// 이전에 접속했던 멀티 서버
	
	BOOL    m_bSaveAccount;
	TCHAR   m_szAccount[ 64 ];
	BOOL    m_bNotice;
	BOOL    m_bTip;
	time_t  m_tNoticeTime;
	
	int		m_nGuide1;	
	int		m_nGuide2;	
	int		m_nGuide3;	

	int		m_nTutorialLv;	//sun: 12, 튜토리얼 개선
	BOOL	m_bViewMask;	//sun: 12, 옵션창 개선
#ifdef PAT_LOOTOPTION
	int		m_nLootType;
#endif
	BOOL    m_bIsShowReSkillMessage;

	TCHAR	m_IPAddress[32];

#ifdef __YENV
	BOOL	m_bExitChange;
	BOOL	m_bSpecBump;
#endif //__YENV

	BOOL	m_bDamageRender;

	BOOL	m_bVisibleBuffTimeRender;
	
	int		m_MouseSpeed;
	BOOL	m_bSFXRenderOff;	
	BOOL	m_bCameraLock;
	BOOL	m_bAutoAttack;
	int*	m_pGuide;
	
	//sun: 밝기, 감마, 명암 조절 Neuz
	int     m_nOverBright;
	float	m_fGamma;
	float   m_fContrast;
	
	int m_BuffStatusMode;	// sun 8차 버프창 관련 적용

	BOOL	m_bZoomLimit;	//sun: 11, 줌옵션 추가	// 줌 제한 여부

	COption();
	~COption();
	
	void Init();
	int Load( LPCTSTR szFileName );
	int Save( LPCTSTR szFileName );
#ifdef _DEBUG
	int LoadToolTip( LPCTSTR szFileName );
#endif // _DEBUG
	
//sun: Wide및 고해상도 추가
	void CheckResolution();

	//gmpbigsun( 20120621 ) : 기동시 nprotect기동이후에 클라Show
	BOOL m_bFullScreenOldData;

	void	SetStartFullScreenMode( const bool bFull )	{	m_bStartFullScreen = bFull;	}
	bool	IsStartFullScreenMode() const				{	return m_bStartFullScreen;	}
};

extern COption		g_Option;

#endif
