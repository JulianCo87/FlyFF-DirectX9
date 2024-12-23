#ifndef __WND_2ND_PASSWORD__
#define __WND_2ND_PASSWORD__


class CWnd2ndPassword : public CWndNeuz
{
public:
	CWnd2ndPassword( void );
	~CWnd2ndPassword( void );

public:
	virtual BOOL Initialize( CWndBase* pWndParent = NULL, DWORD dwType = MB_OK );
	virtual	void OnInitialUpdate( void );
	virtual BOOL OnChildNotify( UINT message, UINT nID, LRESULT* pLResult );

public:
	void SetInformation( u_long idNumberPad, int nSelectCharacter = -1 );
	void InsertPassword( int nPasswordNumber );
	void DeletePassword( void );
	void ResetNumberpad( u_long idNumberPad );

private:
	enum { TABLE_NUMBER_X = 10, TABLE_NUMBER_Y = 1000 };

private:
	int m_nSelectCharacter;



#ifdef _DEBUG
	BYTE m_byPassTable[TABLE_NUMBER_X];
	BOOL m_bInputKeybord;
#else
#ifdef PASSWORD_RESET_2ND
	BYTE m_byPassTable[TABLE_NUMBER_X];
#endif // PASSWORD_RESET_2ND
#endif 

};


#endif // __WND_2ND_PASSWORD__



















