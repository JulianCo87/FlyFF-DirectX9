vs.1.1

dcl_position v0;
dcl_normal v1;
dcl_texcoord0 v2;


; ������Ʈ���Ϳ� ���ؽ��븻���Ϳ��� ������ ���ؼ� r0�� ����.
dp3 r0.x, v1, -c92	; c92=LightDir

;����Ʈ��ġ���� ���ؽ������� ���͸� ���� 
sub r1, v0, c91		; c91=LightPos

;If r0.x is less than zero, then this vertex is facing away
;from the light. If that's the case, push it farther.
slt r2.x, r0.x, c95.y	; r0.x�� c95.y(0)�� ���ؼ� �������� üũ ������� r2.x�� 1 �ƴϸ� 0
mul r2.x, r2.x, c95.w    ; c95.w=PushDistance
mad r3, r2.x, r1, v0

;Do the final transformation and emit it in oPos. This
;is completely independent of the lighting operations
dp4 oPos.x, r3, c84
dp4 oPos.y, r3, c85
dp4 oPos.z, r3, c86
dp4 oPos.w, r3, c87


