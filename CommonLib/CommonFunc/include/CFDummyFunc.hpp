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