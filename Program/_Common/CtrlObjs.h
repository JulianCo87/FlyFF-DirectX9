#ifndef __CTRLOBJS_H__
#define __CTRLOBJS_H__

#ifdef __WORLDSERVER
#include "Ctrl.h"

/// ���������� ������ �������� �ִ� ���������Ʈ
class CContDamageCtrl : public CCtrl
{
private:
	DWORD	m_tmStart, m_tmUpdate;
	
	void Init( void );
	void Destroy( void );
	
public:
	SkillProp *m_pSkillProp;
	AddSkillProp *m_pAddSkillProp;
	OBJID m_idSrc;
	OBJID m_idTarget;
	BOOL m_bControl;
	// chipi - ���� ctrl ���� ��ų�� ���� ��� ���� �� ī���Ǵ� ���� ����
	BOOL m_bDuelTarget;

	void	ApplyDamage( void );
	void	Process();
	
	CContDamageCtrl() 
	{ 
	#ifdef __WORLDSERVER
		SetVirtual( TRUE ); 
	#endif
		Init(); 
	}
	virtual ~CContDamageCtrl() { Destroy(); }
	
};

#endif // Worldserver

#endif // CtrlObjs_h