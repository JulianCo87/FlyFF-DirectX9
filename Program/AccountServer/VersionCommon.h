#ifndef __VERSION_COMMON_H__
#define __VERSION_COMMON_H__
#if !defined( _DEBUG )
	#define	__MAINSERVER
#endif
#define	__VER	23

#if !defined( __MAINSERVER )
	#define __INTERNALSERVER
#endif

// ���� ����

#define		__SERVER			// Ŭ���̾�Ʈ �����ڵ带 �������� �ʱ� ���� define



#if	  defined(__INTERNALSERVER)	// ���� �繫�� �׽�Ʈ����


#elif defined(__MAINSERVER)		// �ܺ� ����


#endif	// end - ���������� define 

#endif	// __VERSION_COMMON_H__