#pragma once

#define		PI		(3.1415926535897)

class CCFUtil : public CBaseObj
{
public:
	static float Angle2Radian( int iAngle );
	static int Radian2Angle( float fRadian );
	static bool FloatEqual( float a, float b );
	static string A2SDec( int i );
	static string A2SHex( int i );
	static float Abs( float f );
};

template < typename T > class CCFFixedSizeLoopList
{
public:
	CCFFixedSizeLoopList( size_t uiSize = 10 ) : m_iHead( 0 ), m_iCount( 0 )
	{
		m_vclRecList.resize( uiSize );
	}

	void Clear()
	{
		m_iHead		=	0;
		m_iCount	=	0;
	}

	void Add( const T &c )
	{
		m_vclRecList[m_iHead]	=	c;
		++m_iHead;
		if( m_iHead >= (int)m_vclRecList.size() ) {
			m_iHead	=	0;
		}

		if( m_iCount < (int)m_vclRecList.size() ) {
			++m_iCount;
		}
	}

	int GetCount()
	{
		return m_iCount;
	}

	T& At( int i )
	{
		int	iRet	=	m_iHead - i - 1;
		while( iRet < 0 ) {
			iRet	+=	GetCount();
		}
		return m_vclRecList[iRet];
	}

public:
	vector< T >													m_vclRecList;
	int															m_iHead;
	int															m_iCount;
};


