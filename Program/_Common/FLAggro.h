#pragma once

#include <map>



#include "../_CommonDefine/FLSingleton.h"
#include "../_CommonDefine/FLStdContainer.h"

#if defined __WORLDSERVER && defined __AGGRO16


#define				INVALID_AGGRO_ID			NULL_ID

class FLAggroProp //: public FLSingleton< FLAggroProp >
{
public:
	struct FLData
	{
		FLData( const DWORD dwAggroID , const char * szAggroName );
		~FLData();

		const DWORD				m_dwAggroID;
		const std::string		m_strAggroName;

		int						nBuffAggroRate;
		int						nDebuffAggroRate;
		int						nFirstAttackAggroRate;
		int						nFirstTargetAggroRate;

		int						nHealRate;

		int						nSingleHealDice;
		int						nGroupHealDice;
		int						nOverHealAdditionDice;

		int						nLodSupAnger;
		int						nLodSupPulling;

		int						nNextTargetAggroRate;
	};

public:
	FLAggroProp();

	static FLAggroProp &	GetInstance( void )
	{
		static FLAggroProp inst;
		return inst;
	}

	BOOL					Load();
	const FLData *			GetProp( const DWORD dwAggroID );
	DWORD					StringToAggroID( const char * szAggroName );

	//const FLAggroProp &	m() const;				/// member�� ���� m -_-

private:
	typedef					std::map< std::string, DWORD >			StringToID;
	typedef					FLMap_DeleteSecond< DWORD, FLData * >	PropDataMap;

	StringToID				m_mapStringToID;
	PropDataMap				m_mapPropData;
};

inline static				FLAggroProp & AGGRO_PROP()		{		return FLAggroProp::GetInstance();		}


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

///AGGRO ��� ���İ�  ��� random access�� �����ϰ� �߰��ϱ�

class FLAggro
{
public: 
	enum					{ AGGRO_UNIT_VALUE = 100 };

	typedef					std::map< OBJID, int /* �Ⱦ��� ������0 */ >				
							MapAggroOwner;	

public:	
	FLAggro( const DWORD dwAggroID );
	virtual ~FLAggro();

	void					Initialize( const OBJID ownerOBJID );							//���� �ݵ�� ����

	////������ �� �ܺ� ���� �Լ�.
	OBJID					GetOwnerObjID() const;
	BOOL					GetMaxAggro( OBJID & o_objid );

	/*static */void			RemoveMover(/* CMover & Monster,*/ const OBJID objid );										//
	void					Clear();

	size_t					Size() const;

//----------------------------------------------------------------------------------------------------------------------------------//
	static void			SumBuffAggro( CMover & Behavior, CMover & Target );		
	static void			SumHealAggro( CMover & Healer, CMover & Target, const int nOverHeal, const DWORD dwExeTarget );	

	void				SumDebuffAggro( CMover & Behavior, CMover & Target );		
	BOOL				SumAttackAggro( CMover & Monster, const OBJID AttackerObjid, const int nDamage );		//
	BOOL				SumFirstAttackerAggro( CMover & Monster, const OBJID AttackerObjid );					//
	BOOL				SumFirstTargetAggro( CMover & Monster, const OBJID TargetObjid );						//

	BOOL				SumLodSupAnger( CMover & Monster, CMover & Player );						//
	BOOL				SumLodSupPulling( CMover & Monster, CMover & Player );						//

	BOOL				SumNextTagerAggro( CMover & Monster, CMover & ValidTarget );

	static BOOL			SumAggro( CMover & Monster, CMover & Player, const UINT64 u64Aggro );						//
	static BOOL			SumAggro( CMover & Monster, const OBJID objid, const UINT64 u64Aggro );						//

protected: 
	//Aggro ���� �Լ�
	static BOOL				sumAggro_autoLink( CMover & Monster, CMover & Player, const UINT64 u64Aggro );				//

	UINT64					getMaxAggro() const;
	BOOL					getAggro( const OBJID objid, UINT64 & o_u64Aggro );
	BOOL					initAggro( const OBJID objid );
	BOOL					sumAggro_autoInsert( const OBJID objid, const UINT64 u64Aggro );			// virtual�� �����Ͽ� 

	//map �� ���� ������ ���ʸ��� �Լ���
	BOOL					insertMover( const OBJID objid );
	BOOL					removeMover(  const OBJID objid );
	BOOL					findMover(  const OBJID objid );
		
	void					clear();

private: 
	typedef					std::map< OBJID, UINT64 >		MapOBJIDToAggroValue;

	const DWORD				m_dwAggroID;
	//const std::string		m_strAggroName;

	MapOBJIDToAggroValue	m_OBJIDToAggroValueMap;
	OBJID					m_OwnerOBJID;

	OBJID					m_nFirstTargetObjID;		/// ��밡 ù Ÿ���� �� ���
	OBJID					m_nFirstAttackerObjID;		/// ��밡 ù Ÿ���� �� ���.
};

//#include <boost/shared_ptr.hpp>
//typedef						boost::shared_ptr< FLAggro >	AggroPtr;

#endif //__AGGRO16


