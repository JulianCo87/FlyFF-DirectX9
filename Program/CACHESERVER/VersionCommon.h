#ifndef __VERSION_COMMON_H__
#define __VERSION_COMMON_H__
#if !defined( _DEBUG )
	#define	__MAINSERVER
#endif
#define	__VER	23

#if !defined( __MAINSERVER )
	#define __INTERNALSERVER
#endif

#define		__SERVER					// Ŭ���̾�Ʈ �����ڵ带 �������� �ʱ� ���� define

// ��Ʈ��ũ ����
#define		__NETWORK_MASSIVE


#define		__MAP_SIZE					// CServerdesc��� ������



#if	  defined(__INTERNALSERVER)			// ���� �繫�� �׽�Ʈ����

	#define		__GUILDVOTE				// ��� ��ǥ(TODO �� ���̴� ����?)

#elif defined(__MAINSERVER)				// �ܺ� ����

#endif	// end - ���������� define 


#endif
