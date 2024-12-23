#ifndef __FLRING_H__
#define __FLRING_H__


#include <boost/static_assert.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/noncopyable.hpp>

//neodeath comment:
// const_iterator ��������.
// ���� : �� �����̳ʿ� ������ ��ü�� �� ��ü�� ���� �� �ִ�.
//		: static_cast�� ���� ( ���� ���[m_stHeadNode]�� FLNode Type�̿��� �߻� )
// ���� : O(1)�� ���⵵�� Pop ����.

template < class _Type >
class FLRing : boost::noncopyable			//�����̳� ���� ����
{
	BOOST_STATIC_ASSERT( ( boost::is_pointer< _Type >::value == false ) );

public:
	struct FLNode : boost::noncopyable		//���Ե� ����� ���� ����
	{
		//BOOST_STATIC_ASSERT( ( boost::is_base_and_derived< FLRing< _Type >::FLNode, _Type >::value == true ) );
		friend				FLRing< _Type >;
		friend				iterator;
	public:
		FLNode();
		virtual ~FLNode();

	public:
		bool				Unlink();
	
	//private:
	//	void *				operator new( size_t )			{}
	//	void				operator delete( void *, size_t ) {}

	private:
		FLNode *			m_pRPrev;
		FLNode *			m_pRNext;
	};

	//typedef					FLNode		_FLNode;

public:
	struct iterator			
	{
	public:
		iterator() : m_pNode( NULL )		{}
		iterator( FLNode * pNode ) : m_pNode( pNode )		{}

		_Type & operator *() const 		{		return * static_cast< _Type * >( m_pNode );		}
		_Type * operator->() const		{		return static_cast< _Type * >( m_pNode );		}

		iterator & operator++()			{
			m_pNode				= m_pNode->m_pRNext;
			return ( *this );
		}

		iterator operator++( int )
		{
			iterator tmp		= *this;
			++*this;
			return tmp;
		}

		iterator & operator--()
		{
			m_pNode				= m_pNode->m_pRPrev;
			return ( * this );
		}

		bool operator == ( const iterator & r ) const		{		return ( m_pNode == r.m_pNode );	}
		bool operator != ( const iterator & r ) const 		{		return ( m_pNode != r.m_pNode );	}

		// return node pointer
		FLNode * _MyNode() const							{		return m_pNode;						}
		// return node pointer	//@ ���� �ؾ��� : m_stHeadNode( == End() )�� ȣ������ ��� ���� 
		_Type * _MyType() const								{		return static_cast< _Type * >( m_pNode );		}

	private:
		 FLNode *				m_pNode;
	};

public:
	FLRing();			
	virtual ~FLRing();

	bool					InsertBack( iterator it, _Type * pDest );
	bool					InsertForward( iterator it, _Type * pDest );

	bool					UnlinkNode( _Type * pObj );

	iterator				PopHead();				
	iterator				PopTail();

	iterator				Begin();
	iterator				Tail();	
	iterator				End();

	iterator				Erase( iterator it );

	void					Clear();

private:
	bool					insertBack( FLNode * pSrc, FLNode * pDest );
	bool					insertForward( FLNode * pSrc, FLNode * pDest );

private:
	FLNode					m_stHeadNode;
};

#endif // __FLRING_H__