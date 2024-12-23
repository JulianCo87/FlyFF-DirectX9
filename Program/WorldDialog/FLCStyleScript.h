#pragma once


#include "FLCStyleParser.h"

// < ������ >
// �Լ� ���ϰ��� üũ ���ϴ� ������.. 
// �Լ� ������ �پ缺�� �������� ���ϴ� ������ 
//		( boost::any�� �ذ�Ǳ� ������ �Լ� ȣ��� ���ε� �Լ��� ȣ�� ������� )

//class CNpcScript;

enum E_OPERATION_TYPE
{
	NONE_OPERATION		= 0,
	LOGIC_CALC_OPERATION,				//�� ����		
	ARITHMETIC_COMPARE_OPERATION,		//�� ����		
	CALC_ADD_SUB_OPERATION, 
	CALC_MUL_DIV_OPERATION,				//��Ģ ����		
};

class FLExecuter_Sequence;
class FLExecuter_Sentence;
class IExecuter;
class IExecuter_BinaryOperation;
class FLExecuter_Function;

class FLExecuter_LogicCmp;
class FLExecuter_ArithmeticCmp;
class FLExecuter_CalcAddSub;
class FLExecuter_CalcMulDiv;

class FLExecuter_CalcSequence;


class FLCStyleScript
{
public:
	//static FLCStyleScript & 

	static bool				CompileScript( FLCStyleReader & reader, map< string, FLExecuter_Sequence * > & o_map );

	static E_OPERATION_TYPE	GetOperationType( const string & op );
	static bool				IsSentenceEnd( const string & op )
	{
		return ( ::strcmp( op.c_str(), ")" ) == 0 || ::strcmp( op.c_str(), ";" ) == 0 || ::strcmp( op.c_str(), "," ) == 0 );
	}

	bool					FindFunction( const string & func );
	static bool				CallFunction( const string & func );

private:
	static FLExecuter_Sequence *		compileProcess(	vector< FLCStyleReader::FLToken >::const_iterator & it, vector< FLCStyleReader::FLToken >::const_iterator & it_end );

	//����
	static FLExecuter_Sentence *		new_IfSentence( vector< FLCStyleReader::FLToken >::const_iterator & it, vector< FLCStyleReader::FLToken >::const_iterator & it_end );
	//�� ���� ����( ��ȣ������ �ѹ������� �ν� )
	static FLExecuter_Sentence *		new_Sentence( vector< FLCStyleReader::FLToken >::const_iterator & it, vector< FLCStyleReader::FLToken >::const_iterator & it_end );

	//
	static IExecuter *					new_Arg( vector< FLCStyleReader::FLToken >::const_iterator & it, vector< FLCStyleReader::FLToken >::const_iterator & it_end );
	//static IExecuter_BinaryOperation *	new_Operation( const size_t line, IExecuter & l, const string & op, IExecuter & r );
	static FLExecuter_Function *		new_Funtion( const string & name, vector< FLCStyleReader::FLToken >::const_iterator & it, vector< FLCStyleReader::FLToken >::const_iterator & it_end );

	//���� 
	static FLExecuter_CalcSequence *	new_TryLogicCmpSentence( IExecuter * pLeft, vector< FLCStyleReader::FLToken >::const_iterator & it, vector< FLCStyleReader::FLToken >::const_iterator & it_end );
	static FLExecuter_CalcSequence *	new_TryArithmeticCmpSentence( IExecuter * pLeft, vector< FLCStyleReader::FLToken >::const_iterator & it, vector< FLCStyleReader::FLToken >::const_iterator & it_end );
	static FLExecuter_CalcSequence *	new_TryCalcAddSub( IExecuter * pLeft, vector< FLCStyleReader::FLToken >::const_iterator & it, vector< FLCStyleReader::FLToken >::const_iterator & it_end );
	static FLExecuter_CalcSequence *	new_TryCalcMulDiv( IExecuter * pLeft, vector< FLCStyleReader::FLToken >::const_iterator & it, vector< FLCStyleReader::FLToken >::const_iterator & it_end );
};




class IExecuter
{
public:
	virtual int				Execute() = 0;
	virtual size_t			GetLine() = 0;
};

class IExecuter_BinaryOperation : public IExecuter
{
public:
	IExecuter_BinaryOperation( const size_t line, IExecuter & l, const std::string & op, IExecuter & r ) 
		: m_line( line ), m_l( &l ), m_op( op ), m_r( &r )	{}

	virtual int				Execute_ModifyLeftValue( const int l ) = 0;
	virtual int				Execute_ModifyRightValue( const int r ) = 0;
	virtual int				Execute_ModifyValue( const int l, const int r ) = 0;

	virtual size_t			GetLine()	{			return m_line;		}

private:
	virtual int				execute( IExecuter & l, const std::string & op, IExecuter & r ) = 0;

public:
	size_t					m_line;

	IExecuter *				m_l;
	const std::string		m_op;
	IExecuter *				m_r;
};

//// �ƹ��͵� ���� �ʴ�
//class FLExecuter_Noop : public IExecuter
//{
//public:
//	virtual int				Execute();
//	virtual size_t			GetLine()	{			return 0;		}
//};

// ����
class FLExecuter_Number : public IExecuter
{
public:
	FLExecuter_Number( const size_t line, const int num );
	virtual int				Execute();
	virtual size_t			GetLine()	{			return m_line;		}

private:
	const size_t			m_line;
	const int				m_num;
};

// �Լ�
class FLExecuter_Function : public IExecuter			//arg�� �ɼ� �ִ�.
{
public:
	FLExecuter_Function( const size_t line, const string & str );
	virtual ~FLExecuter_Function();

	void					AddArgs( IExecuter & op );
	virtual int				Execute();
	virtual size_t			GetLine()	{			return m_line;		}

private:
	const size_t			m_line;
	const std::string		m_strFuntion;
	vector< IExecuter * >	m_args;
};



//class FLExecuter_Sentence;
//class FLExecuter_Sequence;
class FLExecuter_If : public IExecuter
{
public:
	FLExecuter_If( const size_t line ) : m_line( line ), m_pLastElseProcess( NULL ) {}
	~FLExecuter_If();

	void					AddCondition( FLExecuter_Sentence & p );
	void					AddSequence( FLExecuter_Sequence & p );
	int						Get_GotoIndex();

	virtual int				Execute();
	virtual size_t			GetLine()	{			return m_line;		}

private:
	const size_t						m_line;

	vector< FLExecuter_Sentence * >		m_vecConditions;		//���� ����Ʈ
	vector< FLExecuter_Sequence * >		m_vecProcess;			//���ǿ� �ش��ϴ� ó�� ����Ʈ.
	FLExecuter_Sequence	*				m_pLastElseProcess;		//���ǿ� �ش��ϴ� ó�� ����Ʈ.
};

//���� ���� Ȥ�� ���ʴ�� ����Ǵ� ���ӵ� ���� �Ѵ��� //ex 1) 1 + 2 * 3		ex 2) GetVal(); GetCount()

class FLExecuter_Sentence : public IExecuter			
{
public:
	FLExecuter_Sentence( const size_t line ) : m_line( line ) {}
	~FLExecuter_Sentence();

	void					Add( IExecuter & p );			

	virtual int				Execute();
	virtual size_t			GetLine()	{			return m_line;		}

private:									
	const size_t			m_line;
	vector< IExecuter * >	m_vec;
};

// ���ʴ�� ����Ǵ� ����
class FLExecuter_Sequence : public IExecuter
{
public:
	FLExecuter_Sequence( const size_t line ) : m_line( line ) {}
	~FLExecuter_Sequence();

	void					AddExecuter( IExecuter & p );
	virtual int				Execute();
	virtual size_t			GetLine()	{			return m_line;		}

private:
	const size_t			m_line;
	vector< IExecuter * >	m_vecOp;
};


//---------------------------------------------- ���� ������ ���� �� -----------------------------------------------//


class FLExecuter_LogicCmp	: public IExecuter_BinaryOperation
{
public:
	FLExecuter_LogicCmp( const size_t line, IExecuter & l, const string & op, IExecuter & r );
	virtual ~FLExecuter_LogicCmp();

	virtual int				Execute();

public:
	virtual int				Execute_ModifyLeftValue( const int l );
	virtual int				Execute_ModifyRightValue( const int r );
	virtual int				Execute_ModifyValue( const int l, const int r );

private:
	virtual int				execute( IExecuter & l, const std::string & op, IExecuter & r );
};

class FLExecuter_ArithmeticCmp	: public IExecuter_BinaryOperation
{
public:
	FLExecuter_ArithmeticCmp( const size_t line, IExecuter & l, const string & op, IExecuter & r );
	virtual ~FLExecuter_ArithmeticCmp();

	virtual int				Execute();

public:
	virtual int				Execute_ModifyLeftValue( const int l );
	virtual int				Execute_ModifyRightValue( const int r );
	virtual int				Execute_ModifyValue( const int l, const int r );

private:
	virtual int				execute( IExecuter & l, const std::string & op, IExecuter & r );
};

class FLExecuter_CalcAddSub	: public IExecuter_BinaryOperation
{
public:
	FLExecuter_CalcAddSub( const size_t line, IExecuter & l, const string & op, IExecuter & r );
	virtual ~FLExecuter_CalcAddSub();

	virtual int				Execute();

public:
	virtual int				Execute_ModifyLeftValue( const int l );
	virtual int				Execute_ModifyRightValue( const int r );
	virtual int				Execute_ModifyValue( const int l, const int r );

private:
	virtual int				execute( IExecuter & l, const std::string & op, IExecuter & r );
};

class FLExecuter_CalcMulDiv	: public IExecuter_BinaryOperation
{
public:
	FLExecuter_CalcMulDiv( const size_t line, IExecuter & l, const string & op, IExecuter & r );
	virtual ~FLExecuter_CalcMulDiv();

	virtual int				Execute();

public:
	virtual int				Execute_ModifyLeftValue( const int l );
	virtual int				Execute_ModifyRightValue( const int r );
	virtual int				Execute_ModifyValue( const int l, const int r );

private:
	virtual int				execute( IExecuter & l, const std::string & op, IExecuter & r );
};


//--------------------------

class FLExecuter_CalcSequence : public IExecuter
{
public:
	FLExecuter_CalcSequence( const size_t line );
	virtual ~FLExecuter_CalcSequence();

public:
	virtual int				Execute();

	virtual size_t			GetLine();
	void					PushBack( IExecuter * p );	

private:
	vector< IExecuter * >		m_vecSequence;
	const size_t			m_line;
};


//IExecuter_BinaryOperation * NewOperation( const E_OPERATION_TYPE eOperationType, IExecuter & l, const string & op, IExecuter & r )
//{
//	switch( eOperationType )
//	{
//	case LOGIC_CALC_OPERATION:
//		return new FLExecuter_LogicCmp( l.GetLine(), l, op, r );
//	case ARITHMETIC_COMPARE_OPERATION:
//		return new FLExecuter_ArithmeticCmp( l.GetLine(), l, op, r );
//	case CALC_ADD_SUB_OPERATION:
//		return new FLExecuter_CalcAddSub( l.GetLine(), l, op, r );
//	case CALC_MUL_DIV_OPERATION:
//		return new FLExecuter_CalcMulDiv( l.GetLine(), l, op, r );
//	default:
//		{
//			DEBUG_BREAK;
//		}
//		return NULL;
//	}
//}