#ifndef __VERSION_COMMON_H__
#define __VERSION_COMMON_H__
#if !defined( _DEBUG )
	#define	__MAINSERVER
#endif
#define	__VER	23

#if !defined( __MAINSERVER )
	#define __INTERNALSERVER
#endif

#define		__SERVER				// Ŭ���̾�Ʈ �����ڵ带 �������� �ʱ� ���� define


#define		__MAP_SIZE						
#define		__LAYER_1015			// ���� ��ü �� TODO Ŭ���̾�Ʈ �̻��



#if	  defined(__INTERNALSERVER)		// ���� �繫�� �׽�Ʈ����

	#define		__GUILDVOTE			// ��� ��ǥ 


#elif defined(__MAINSERVER)			// �ܺ� ����

#endif	// end - ���������� define 


#endif


