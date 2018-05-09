#pragma once

#if defined( linux )
#define INFINITE	(-1)
#endif

class CCFCriticalSection
{
public:
	CCFCriticalSection();
	~CCFCriticalSection();

	bool Lock(uint uiTimeout = INFINITE);
	void Unlock();
};

class CCFMutex
{
public:
	CCFMutex() {}
	~CCFMutex() {}

	bool Lock(uint uiTimeout = INFINITE) { return true; }
	void Unlock() {}
};

class CCFMutexLocker
{
public:
	CCFMutexLocker(CCFMutex&){}
};
class CCFCSLocker
{
public:
	CCFCSLocker(CCFMutex&){}
};

//	class CCFObjectLevelLockable
template <class Host> class CCFObjectLevelLockable
{
	CCFCriticalSection				  m_cs;

public:
	CCFObjectLevelLockable()
	{
	}

	~CCFObjectLevelLockable()
	{
	}

	class Lock;
	friend class Lock;

	class Lock
	{
		CCFObjectLevelLockable & host_;

		Lock(const Lock&);
		Lock& operator=(const Lock&);
	public:

		explicit Lock(CCFObjectLevelLockable& host) : host_(host)
		{
			host_.m_cs.Lock();
		}

		~Lock()
		{
			host_.m_cs.Unlock();
		}
	};

	typedef volatile Host VolatileType;

	long AtomicIncrement(volatile long& lval)
	{
		CCFCSLocker	 clLock(m_cs);
		return ++lval;
	}

	long AtomicDecrement(volatile long& lval)
	{
		CCFCSLocker	 clLock(m_cs);
		return --lval;
	}

	void AtomicAssign(volatile long& lval, long val)
	{
		CCFCSLocker	 clLock(m_cs);
		lval = val;
	}

	void AtomicExchange(long& lval, volatile long& val)
	{
		CCFCSLocker	 clLock(m_cs);
		long	t = lval;
		lval = val;
		val = t;
	}
};

template< class T > class CCFLocker
{
public:
	CCFLocker(T &c, uint uiTimeout = INFINITE) : m_c(c), m_bLocked(false)
	{
		Lock(uiTimeout);
	}

	~CCFLocker()
	{
		Unlock();
	}

	void Lock(uint uiTimeout = INFINITE)
	{
		if (!m_bLocked) {
			m_bLocked = m_c.Lock(uiTimeout);
		}
	}

	void Unlock()
	{
		if (m_bLocked) {
			m_c.Unlock();
			m_bLocked = false;
		}
	}

	bool IsLocked()
	{
		return m_bLocked;
	}

	T						&m_c;
	bool					m_bLocked;
};
