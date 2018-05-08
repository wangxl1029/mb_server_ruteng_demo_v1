#ifndef	CFREFCNTBASE_H
#define	CFREFCNTBASE_H

//	如果一个类本身需要实现引用计数，使用本文件中的方法

//	IRefCntBase
class IRefCntBase : public CBaseObj
{
protected:
	virtual ~IRefCntBase() {};

public:
	virtual long AddRef() = 0;
	virtual long Release() = 0;
};

//	Smart Pointer for IRefCntBase
template < class T > class CRCPtr
{
public:
	CRCPtr() : p( NULL ) {}
	~CRCPtr()
	{
		if( NULL != p ) {
			p->Release();
		}
	}
	operator T*()
	{
		return p;
	}
	T* operator->()
	{
		return p;
	}
	T** operator&()
	{
		return &p;
	}
	CRCPtr( const CRCPtr &r )
	{
		if( NULL != r.p ) {
			r.p->AddRef();
		}
		p	=	r.p;
	}
	CRCPtr &operator=( const CRCPtr &r )
	{
		if( this == &r || this->p == r.p ) {
			return *this;
		}
		if( NULL != p ) {
			p->Release();
			p	=	NULL;
		}
		if( NULL != r.p ) {
			r.p->AddRef();
		}
		p	=	r.p;
		return *this;
	}
	CRCPtr &operator=( T *p1 )
	{
		if( p == p1 ) {
			return *this;
		}
		if( NULL != p ) {
			p->Release();
			p	=	NULL;
		}
		if( NULL != p1 ) {
			p1->AddRef();
		}
		p	=	p1;
		return *this;
	}

	T			*p;
};

//	The following source code is for implementation of IRefCntBase
//	It can be called between dlls, because it is implemented in sub class but not in IRefCntBase.
//	ReDefine REF_CNT_DELETE if you have your own memory management
#ifndef		REF_CNT_DELETE
#define		REF_CNT_DELETE(_P_)											delete (_P_)
#endif		//	REF_CNT_DELETE
#define WXL_TEMPERORY (1)
class CRefCntImpl
{
public:
	CRefCntImpl() : m_lRefCnt( 1 ) {}
	long AddRef()
	{
#if defined( linux )
		CCFCSLocker	 clLock( m_clCS );
		return ++m_lRefCnt;
#elif WXL_TEMPERORY
		return ++m_lRefCnt;
#else
		return InterlockedIncrement( &m_lRefCnt );
#endif
	}
	long Release()
	{
#if defined( linux )
		CCFCSLocker	 clLock( m_clCS );
		return --m_lRefCnt;
#elif WXL_TEMPERORY
		return ++m_lRefCnt;
#else
		return InterlockedDecrement(&m_lRefCnt);
#endif
	}

public:
	long m_lRefCnt;
#if defined( linux )
	CCFCriticalSection	  m_clCS;
#endif
};

//	use this macro in sub class define( in .h files ) of IRefCntBase
#define		REF_CNT_IMPLEMENTTATION_H()									\
			CRefCntImpl	_clRefCnt;										\
			virtual long AddRef();										\
			virtual long Release();

//	use this macro in sub class implementation( in .cpp files ) of IRefCntBase
#define		REF_CNT_IMPLEMENTTATION_CPP( _ClassName_ )					\
			long _ClassName_::AddRef()									\
			{															\
				return _clRefCnt.AddRef();								\
			}															\
			long _ClassName_::Release()									\
			{															\
				long	l	=	_clRefCnt.Release();					\
				if( 0 == l ) {											\
					REF_CNT_DELETE(this);								\
				}														\
				return l;												\
			}

class CRefCntBase : public IRefCntBase
{
public:
	REF_CNT_IMPLEMENTTATION_H()
};

#endif	//	CFREFCNTBASE_H
