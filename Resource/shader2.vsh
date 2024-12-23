//
// Shader Example.
// Copyleft (c) 2007 Heesung Oh(Galic Studio). All rights Not reserved.
////////////////////////////////////////////////////////////////////////////

//  c0: World * View * Projection matrix
//	c9: color
//	c27: Scale


#define	Scl	c27


vs_1_1

def c25, 0.0, 0.0, 0.0, 1.0

dcl_position  v0		// ���� ��ġ ���� �������� ���� v0
dcl_normal	  v1		// ���� ���� ���� �������� ���� v1

mov r0, v1				// ���� ����
mad r0, r0, Scl, v0		// ������ ��ġ' =  ���� ���� * ������ + ��ġ
mov	r0.w, c25.w			// ������ ��ġ' w = 1.0

m4x4 oPos, r0, c0		// ��ȯ

mov	oD0,	c9
