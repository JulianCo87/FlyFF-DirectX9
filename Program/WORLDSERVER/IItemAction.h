#pragma once

class FLWUser;
class FLItemElem;

//ret : ���/���� ����

class IItem_UseFunc
{
public:
	virtual bool			Use( FLWSUser & kUser, FLItemElem & io_kUseItem, CAr & ar )= 0;		
};

class IItem_ApplyFunc
{
public:
	virtual bool			Apply( FLWSUser & kUser, FLItemElem & io_kUseItem, FLItemElem & io_kDestItem, CAr & ar )	= 0;
};

//�Ⱓ ����
//���� // ����ϸ� �����Ǵ� Ÿ�� : �ƴ�Ÿ��	//	bPermanence
//�����ؾ� ���� �ִ� ������

