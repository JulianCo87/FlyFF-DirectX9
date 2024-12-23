
#ifndef __FLITEMELEM_H__
#define __FLITEMELEM_H__


#include "FLItemBase.h"
#include "FLPiercingOption.h"
#include "FLRandomOption.h"
#include "pet.h"


class FLItemElem : public FLItemBase
{
	public:
		//	Constructions
		FLItemElem();
		FLItemElem( const FLItemElem & r );
		virtual	~FLItemElem();

		virtual	FLItemElem&	operator = ( const FLItemElem & ie );
		virtual void	Serialize( CAr & ar ) const;
		virtual void	Deserialize( CAr & ar );

	private:
		void		constructor();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	public:
		int			GetItemIndex(){return m_dwItemId;}
		BOOL		IsPierceAble( DWORD dwTargetItemKind3 = NULL_ID, BOOL bSize = FALSE );
		BOOL		IsPerin( void )		{	return m_dwItemId == ITEM_INDEX( 26456, II_SYS_SYS_SCR_PERIN );	}	//sun: 12, ����
		BOOL		IsCollector( BOOL bRefinable = FALSE )	{	return( m_dwItemId == ITEM_INDEX( 26452, II_GEN_TOO_COL_NORMAL ) || ( !bRefinable && m_dwItemId == ITEM_INDEX( 26565, II_GEN_TOO_COL_EVENT ) ) );		}

		bool		SelectRandomOption( BYTE bySelectFlag );

		int			GetMinLimitLevel( void );
		

		
		BOOL		IsPet( void )	{	return GetProp()->dwItemKind3 == IK3_EGG;	}

		BOOL		IsEatPet( void )	{	return GetProp()->dwItemKind3 == IK3_PET;	}
		BOOL		IsFeed( void )		{	return GetProp()->dwItemKind3 == IK3_FEED;	}
		BOOL		IsEgg( void );
		CString		GetName( void );

		virtual	void			Empty();
		int			GetAttrOption();		// �������� +�ɼǰ��� �Ӽ�/�Ӽ��������� ���ļ� ����.
		int			GetAbilityOption() { return m_nAbilityOption; }	//���ü�ġ
		BYTE		GetItemResist( void ) const;
		int			GetResistAbilityOption( void ) const;
		int*		GetAbilityOptionPtr() { return &m_nAbilityOption; }
		void		SetAbilityOption( int nAbilityOption ) { nAbilityOption > 0 ? m_nAbilityOption = nAbilityOption : m_nAbilityOption = 0; }
		void		SetResistAbilityOption( int nResistAbilityOption ) { nResistAbilityOption > 0 ? m_nResistAbilityOption = nResistAbilityOption : m_nResistAbilityOption = 0; }
		void		SetItemResist( BYTE byItemResist ) { m_byItemResist < SAI79::END_PROP ?  m_byItemResist = byItemResist : m_byItemResist = SAI79::NO_PROP; }
		int			GetGold();

#if defined( __WORLDSERVER ) || defined (__CLIENT )
		BOOL		IsAccessory( void );
		bool		CanUseLevel( CMover* pMover );	// ����� ������ �����ΰ�?
		void		GetPiercingAvail( PPIERCINGAVAIL pPiercingAvail/*input, output*/ );

		void		GetCombinedPiercingAvail( PPIERCINGAVAIL pPiercingAvail/*input, output*/ );

#ifdef COSTUME_UPGRADE_ENHANCEMENT_GEM
		int			GetGemAvail( int nAblity );
#endif

		DWORD		GetMinAddDamage();
#endif // defined( __WORLDSERVER ) || defined (__CLIENT )

		BOOL		IsExpiring( void );
		void		ResetFlag( BYTE byFlag )	{	m_byFlag	&= ~byFlag;		}
		void		SetFlag( BYTE byFlag )		{	m_byFlag	|= byFlag;		}
		BOOL		IsFlag( BYTE byFlag )	{	return ( m_byFlag & byFlag ) ? TRUE: FALSE;		}
		BOOL		IsOwnState();		// ���� ������ ���� �����̴�
		bool		IsBindState();		// �ͼӵ� �����ΰ�
		bool		IsUseBindState();	// ��� �� �ͼӵǴ� �����ΰ�
		BOOL		IsUndestructable( void );
		BOOL		IsCharged();
		BOOL		IsLogable();
		// test
		//////////////////////////////////////////////////////////////////////////
		// mirchang_100820 �Ⱓ�� ������ ����..
		BOOL		IsPeriod();
		//////////////////////////////////////////////////////////////////////////

		static BOOL	IsDiceRefineryAble( PT_ITEM_SPEC pProp );
		static BOOL	IsEleRefineryAble( PT_ITEM_SPEC pProp );
		static BOOL	IsElementalCard( const DWORD dwItemID );

		bool		IsStackable( FLItemElem* pItemElem );

		//	Operations
		void		UseItem();
		
		BOOL		IsVisPet();
		DWORD		GetVisPetSfxId();
		BOOL		IsTransformVisPet();

		void		InitializeLooksChangeItemID();
		DWORD		GetLooksChangeItemID() const;
		void		SetLooksChangeItemID( const DWORD dwItemID );
		bool		IsMathingLooksChangeMaterialItem( const DWORD dwItemID ) const;

		bool		IsSetCombinedOption() const;
		void		SetCombinedOption( bool bCombined );

		void		InitializeCombinedAddDamage();
		DWORD		GetCombinedAddDamage() const;
		void		SetCombinedAddDamage( const DWORD dwCombinedAddDamage );

		void		InitializeCombinedPiercingItem();

		bool		IsSetGeneralPiercingOption();
		bool		IsSetUltimatePiercingOption();
		bool		IsExistGeneralPiercedItemID( const DWORD dwItemID );
		bool		IsSetCombinedGeneralPiercingOption();

		size_t		GetGeneralPiercingSize() const;
		size_t		GetUltimatePiercingSize() const;
		size_t		GetVisPetPiercingSize() const;
		size_t		GetCombinedGeneralPiercingSize() const;

		DWORD		GetGeneralPiercingItemID( const size_t Nth ) const;
		DWORD		GetUltimatePiercingItemID( const size_t Nth ) const;
		time_t		GetVisPetPiercingItemKeepTime( const size_t Nth ) const;
		DWORD		GetCombinedGeneralPiercingItemID( const size_t Nth ) const;

		void		SetGeneralPiercingSize( const size_t nSize );
		void		SetUltimatePiercingSize( const size_t nSize );
		void		SetVisPetPiercingSize( const size_t nSize );
		void		SetCombinedGeneralPiercingSize( const size_t nSize );

		void		SetGeneralPiercingItemID( const size_t Nth, const DWORD dwItemID );
		void		SetUltimatePiercingItemID( const size_t Nth, const DWORD dwItemID );
		void		SetVisPetPiercingItemKeepTime( const size_t Nth, const time_t tKeepTime );
		void		SetCombinedGeneralPiercingItemID( const size_t Nth, const DWORD dwItemID );

		void		SwapVisItem( const size_t nSrc, const size_t nDest );

		void		SerializePiercingOption( CAr & ar );
		void		SerializeCombinedPiercingOption( CAr & ar );
		void		DeserializePiercingOption( CAr & ar );
		void		DeserializeCombinedPiercingOption( CAr & ar );

		void		CopyPiercingOption( const FLPiercingOption & kPiercingOption );
		void		GetPiercingOption( FLPiercingOption & kPiercingOption );


		void		InitializeRandomOptionOriginID();
		void		InitializeCombinedRandomOptionOriginID();

		WORD		GetRandomOptionOriginID() const;
		WORD		GetCombinedRandomOptionOriginID() const;

		void		SetRandomOptionOriginID( const WORD wRandomOptionOriginID );
		void		SetCombinedRandomOptionOriginID( const WORD wCombinedRandomOptionOriginID );

		void		InitializeRandomOptionExtension();
		void		InitializeNewRandomOptionExtension();
		void		InitializeCombinedRandomOptionExtension();

		WORD		GetRandomOptionExtensionFlag() const;
		void		SetRandomOptionExtensionFlag( const WORD wRandomOptionExtensionFlag );

		bool		IsSetSafeGuardRandomOptionExtensionFlag() const;
		void		SetSafeGuardRandomOptionExtensionFlag();
		void		ResetSafeGuardRandomOptionExtensionFlag();

		bool		IsSetRandomOptionExtension() const;
		bool		IsSetNewRandomOptionExtension() const;
		bool		IsSetCombinedRandomOptionExtension() const;

		size_t		GetRandomOptionExtensionSize() const;
		size_t		GetNewRandomOptionExtensionSize() const;
		size_t		GetCombinedRandomOptionExtensionSize() const;

		WORD		GetRandomOptionExtensionDstID( const size_t Nth ) const;
		WORD		GetNewRandomOptionExtensionDstID( const size_t Nth ) const;
		WORD		GetCombinedRandomOptionExtensionDstID( const size_t Nth ) const;

		short		GetRandomOptionExtensionAdjValue( const size_t Nth ) const;
		short		GetNewRandomOptionExtensionAdjValue( const size_t Nth ) const;
		short		GetCombinedRandomOptionExtensionAdjValue( const size_t Nth ) const;

		bool		SetRandomOptionExtension( const T_RANDOMOPTION_EXT & kRandomOptionExtension );
		bool		SetNewRandomOptionExtension( const T_RANDOMOPTION_EXT & kRandomOptionExtension );
		bool		SetCombinedRandomOptionExtension( const T_RANDOMOPTION_EXT & kRandomOptionExtension );

		bool		OverwriteNewRandomOptionExtension();

		void		SerializeRandomOptionExtension( CAr & ar );
		void		SerializeNewRandomOptionExtension( CAr & ar );
		void		SerializeCombinedRandomOptionExtension( CAr & ar );

		void		DeserializeRandomOptionExtension( CAr & ar );
		void		DeserializeNewRandomOptionExtension( CAr & ar );
		void		DeserializeCombinedRandomOptionExtension( CAr & ar );

		void		CopyRandomOption( const FLRandomOption & kRandomOption );
		void		GetRandomOption( FLRandomOption & kRandomOption ) const;

		void		RandomOptionDataConverting( __int64 n64RandomOptionId );

		
		int			GetLevelDown() const;
		void		SetLevelDown( const int nLevelDown );
		void		InitializeLevelDown();

		DWORD		GetCoupleId() const;
		void		SetCoupleId( const DWORD dwPlayerId );
		void		InitializeCoupleId();

		void		LogItemElem( TCHAR* pszLog, size_t nSize ) const;

		bool		CanSellNPC() const;

		void		SetRandomOptionExtension( const DB_RANDOMOPTION_DST& DST, const DB_RANDOMOPTION_ADJ& ADJ );
		void		SetCombinedRandomOptionExtension( const DB_RANDOMOPTION_DST& DST, const DB_RANDOMOPTION_ADJ& ADJ );

		int			GetRandomOptionExtensionDstID( const size_t lower_nth, const size_t upper_nth ) const;
		int			GetCombinedRandomOptionExtensionDstID( const size_t lower_nth, const size_t upper_nth ) const;
		int			GetRandomOptionExtensionAdjValue( const size_t lower_nth, const size_t upper_nth ) const;
		int			GetCombinedRandomOptionExtensionAdjValue( const size_t lower_nth, const size_t upper_nth ) const;

		DWORD		GetKind3() {	return GetProp() ? GetProp()->dwItemKind3 : NULL; }

	public:
		enum	{	normal = 0,	expired	= 0x01,	binds = 0x02,	is_using = 0x04,	period = 0x08,	reset_bind = 0x10	};

		CPet*		m_pPet;		// �� ��ü, ù ��ȯ �� ����

		int			m_nAbilityOption;	// �߰� �ɷ�ġ ���� �ɼ�
		int			m_nRepair;			// ���� Ƚ��
		BYTE		m_nRepairNumber;
		int			m_nHitPoint;
		int			m_nItemNum; 
		BYTE		m_byFlag;
		DWORD		m_idGuild;			// �����ۿ� ��� ��ȣ�� �ٴ� ���(����)

#ifdef __CLIENT
		BOOL		m_bRepair;
#endif	// __CLIENT

		BYTE   	    m_byItemResist;				// ��� �Ӽ� �ΰ�? / TEXT���·� �˽�Ʈ�� �۵���Ű�� �����ۿ��� TRUE�̸� ���̻� ����Ʈ�� �۵��� �� ���� �ȴ�.
		int			m_nResistAbilityOption;		// �Ӽ� �߰� �ɷ�ġ ���� �ɼ�
		int			m_nResistSMItemId;	

#ifdef __WORLDSERVER
		BOOL	m_bQuery;
#endif	// __WORLDSERVER


		BOOL	m_bCharged;			// ���ȭ ���������� Ȯ��
		DWORD	m_dwKeepTime;		// ���ӽð�
		BOOL	m_bTranformVisPet;
	

	private:
		int					m_nLevelDown;
		DWORD				m_dwCouplePlayerId;
		bool				m_bCombined;
		DWORD				m_dwCombinedAddDamage;
		WORD				m_wRandomOptionOriginID;
		WORD				m_wCombinedRandomOptionOriginID;
		DWORD				m_dwLooksChangeItemID;

		FLPiercingOption	m_kPiercingOption;
		FLRandomOption		m_kRandomOption;

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
};

inline int	FLItemElem::GetAttrOption()		// �������� +�ɼǰ��� �Ӽ�/�Ӽ��������� ���ļ� ����.
{
	int nAttr = static_cast<int>( m_byItemResist );
	int nOption = m_nResistAbilityOption;
	int nRet = m_nAbilityOption;

	nRet |= (nAttr << 16);		// D16~D23 : �Ӽ�
	nRet |= (nOption << 24);	// D24~D31 : �Ӽ�����
	return nRet;
}

inline BYTE	FLItemElem::GetItemResist( void ) const
{
	return m_byItemResist;
}

inline int	FLItemElem::GetResistAbilityOption( void ) const
{
	return m_nResistAbilityOption;
}

#endif // __FLITEMELEM_H__
