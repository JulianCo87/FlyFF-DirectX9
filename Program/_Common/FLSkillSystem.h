
#ifndef	__FLSKILLSYSTEM_H__
#define	__FLSKILLSYSTEM_H__


class CMover;

const DWORD		MIN_SKILL_USE_LEVEL		= 1;

class	FLSkillSystem
{
public:

	static	FLSkillSystem*	GetInstance();

private:

	FLSkillSystem();
	~FLSkillSystem();

public:
	//////////////////////////////////////////////////////////////////////////
	bool	CanUseSkill( CMover* pUseMover, const DWORD dwSkillID, const DWORD dwSkillLevel, const OBJID idFocusObj, const SKILLUSETYPE sutType );
	bool	IsRemainSkillDelayTime( CMover* pUseMover, const DWORD dwSkillID ) const;
	//////////////////////////////////////////////////////////////////////////

	DWORD	DoUseSkill( CMover* pUseMover, const DWORD dwSkill, const int nLevel, const OBJID idFocusObj, const SKILLUSETYPE sutType = SUT_NORMAL, const BOOL bControl = FALSE, const int nCastingTime = 0 );		// dwSkill/nLevel�� ������ ����� �� �ִ� ����.
	void	DoActiveSkill( CCtrl* pUseCtrl, DWORD dwSkill, int nLevel, CCtrl *pTarget, bool bIgnoreProp = false, BOOL bControl = FALSE );		// ActiveSkill�ߵ�

	BOOL	OnMeleeSkill( CMover* pUseMover, int nType, int nCount = 1 );		// OBJMSG_ATK_MELEESKILL������ Ÿ���� �Ǿ����� �߻��ϴ� �̺�Ʈ.
	BOOL	OnMagicSkill( CMover* pUseMover, int nType, int nCount = 1 );		// OBJMSG_ATK_MAGICSKILL������ Ÿ���� �Ǿ����� �߻��ϴ� �̺�Ʈ.
	void	OnEndSkillState( CMover* pTarget, DWORD dwSkill, DWORD dwLevel );

	int		DoApplySkill( CCtrl* pUseCtrl, CCtrl *pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProp = false, int nParam = 0, BOOL bOnlyDmg = FALSE, BOOL bControl = FALSE );		// ��ų�� Ÿ���߽����� ����

	void	ApplySkillAround( CCtrl* pUseCtrl, CCtrl* pTarget, int nApplyType, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProp = false, BOOL bOnlyDmg = FALSE, FLOAT fRangeCustom = 0.0f, BOOL bControl = FALSE );		// ��ų�� this�� �߽����� �ֺ��� ����.
	void	ApplySkillRegion( CCtrl* pUseCtrl, const D3DXVECTOR3 &vPos, int nApplyType, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProp = false, BOOL bOnlyDmg = FALSE, CCtrl* pCenter = NULL, BOOL bControl = FALSE );	// vPos�� �߽����� ��ų�� ����

	void	ApplySkill( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProp = false, int nParam = 0, BOOL bOnlyDmg = FALSE, BOOL bTarget = TRUE );		// Ÿ���ϳ��� ���� ����Ǿ�� �ϴ� ȿ��

	BOOL	ApplyParam( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, BOOL bSend = TRUE, int nDamage = 0 );	// this���� �Ķ���͸� ����. pSrc�� ������.
	BOOL	ApplyActiveParam( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, BOOL bSend = TRUE, int nDamage = 0 );	// this���� �Ķ���͸� ����. pSrc�� ������.

private:

	int		DoApplySkillEx( CCtrl* pUseCtrl, CCtrl *pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProp, int nParam, BOOL bOnlyDmg, BOOL bControl );

	void	ShootSkill( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp* pSkillProp, AddSkillProp* pAddSkillProp );			// �߻������� ��ų�� �߻�ü ����

	void	ApplySkillAroundTroupe( CCtrl* pUseCtrl, u_long idParty, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, bool bIgnoreProp = false );		// idParty ��Ƽ ������� ��ųȿ���� ����.
	void	ApplySkillLine( CCtrl* pUseCtrl, int nApplyType, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, float fWidth, float fHeight, float fDepth, bool bIgnoreProp = false, BOOL bControl = FALSE );

	int		ApplyDamage( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, int nParam, BOOL bTarget );
	BOOL	ApplySkillHardCoding( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp );


	BOOL	DoActiveParam( CCtrl* pUseCtrl, CCtrl* pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp, int nIndex, BOOL bSend = TRUE, int nDamage = 0 );	// this���� �Ķ���͸� ����. pSrc�� ������.

	bool	CanApplyBuff( CMover* pSrcMover, CMover* pTargetMover, SkillProp* pSkillProp );

#ifdef __WORLDSERVER
	void	ApplyHeal( CMover* pHealer, CMover* pTarget, int nHealValue, DWORD dwChgParamVal, DWORD dwExeTarget );
#endif

#ifdef	__CLIENT
	void	CreateSkillSfx( CCtrl* pUseCtrl, CCtrl *pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp );		// ��ų������ Ŭ�󿡼� �����Ǿ�� �� sfx
	void	CreateYoyoSkill( CCtrl* pUseCtrl, CSfx* pSfx, CCtrl *pTarget, SkillProp *pSkillProp, AddSkillProp *pAddSkillProp );
#endif


};


#define	g_cSkillSystem	FLSkillSystem::GetInstance()







#endif