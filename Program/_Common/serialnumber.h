#ifndef __SERIAL_NUMBER_H__
#define	__SERIAL_NUMBER_H__


class	CSerialNumberRequest
{
public:

	virtual ~CSerialNumberRequest()	{}

	virtual void	Request()	= 0;
};

class	CSerialNumber
{
public:

	enum
	{
		SN_TYPE_ITEM			= 1,			// �ø��� �ѹ� ������ Ÿ�� ���̵�
	};

	enum
	{
		SN_END_NUMBER			= 0xFFFFFFFE,	// ������ �ø��� �ѹ� ��
		SN_IGNORE_SERIAL_NUMBER	= 0xFFFFFFFF,	// �ߺ� ���� ���� �ø���ѹ���(Shop �����ۿ� ���)
	};

	static	CSerialNumber*	GetInstance();

// constructions
	virtual ~CSerialNumber();

// operations
	virtual	bool			Init( CSerialNumberRequest * pRequest )			= 0;
	virtual bool			Add( SERIALNUMBER iStart, SERIALNUMBER iEnd )	= 0;
	virtual SERIALNUMBER	Get()		= 0;

	// TODO �ø���ѹ� �ʱ�ȭ �Ϸ� Ȯ���� ���� �ӽ��Լ�
	virtual bool			IsInit()	= 0;

	//////////////////////////////////////////////////////////////////////////
	virtual bool			IsManaged()	= 0;
	// �ϴ� ���⿡ ����
	virtual void			InsertSerialNumber( SERIALNUMBER iSerialNumber )	= 0;
	virtual	void			RemoveSerialNumber( SERIALNUMBER iSerialNumber )	= 0;
	//////////////////////////////////////////////////////////////////////////

};

#endif	// __SERIAL_NUMBER_H__