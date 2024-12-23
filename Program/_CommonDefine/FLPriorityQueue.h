#ifndef __FLPRIORITYQUEUE_H__
#define __FLPRIORITYQUEUE_H__

#include "../_CommonDefine/FLRing.hpp"
#include <boost/static_assert.hpp>


//�����ս��� ���� _Type�� pointer Ÿ������ �ۿ� �� ����

template < class _Key, class _Ptr >		//DELETER �� �ʿ���~
class FLPriorityQueue
{
	BOOST_STATIC_ASSERT( ( boost::is_pointer< _Ptr >::value == true ) );

public:
	struct FLNode : public FLRing< FLNode >::FLNode
	{	
		friend FLPriorityQueue;
	public:
	//	FLNode() {}
	//	virtual ~FLNode()	{}

	private:
		_Key				key;
	};

public:
	FLPriorityQueue();
	virtual ~FLPriorityQueue(); 

	void					Push( const _Key key, const _Ptr & t );
	BOOL					PopFront( _Key & o_key, _Ptr & o_t );
	BOOL					Top( _Key & o_key, _Ptr & o_t );

private:
	//typedef					std::multimap< _Key, _Type >	List;
	typedef					FLRing< FLNode >		List;

	List					m_list;
};



#endif //__FLPRIORITYQUEUE_H__