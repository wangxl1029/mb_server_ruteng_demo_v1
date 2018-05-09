#pragma once
#include <memory>
#include <map>
#include <algorithm>

//	最近最少使用算法的Cache的模板实现

template < typename KEY, typename DATA, typename _Pr = less<KEY>, typename ThreadModel = CCFObjectLevelLockable< int > >
class CCFSimpleCache
{
public:
	CCFSimpleCache() : m_pclHead(NULL), m_uiCapacity(200), m_uiLinearSearchCount(0), m_uiLinearSearchCondition(0) {}
	virtual ~CCFSimpleCache() {}

	class CDataRec
	{
	public:
		CDataRec() : pclPrev(NULL), pclNext(NULL) {}
		CDataRec(KEY k1, SmartPointer<DATA> spData1) : pclPrev(NULL), pclNext(NULL), k(k1), spData(spData1) {}
		CDataRec			*pclPrev;
		CDataRec			*pclNext;
		KEY					k;
		SmartPointer<DATA>	spData;
	};

	SmartPointer<DATA> GetData(const KEY &k)
	{
		typename ThreadModel::Lock	clLock(m_clThreadModel);

		if (NULL != m_pclHead) {
			if (!(_Pr()(m_pclHead->k, k) || _Pr()(k, m_pclHead->k))) {
				return m_pclHead->spData;
			}

			if (m_uiLinearSearchCount > 0 && m_mapData.size() < m_uiLinearSearchCondition) {
				size_t			uiSearchCount = min(m_uiLinearSearchCount, m_mapData.size());
				CDataRec		*pclData = m_pclHead;
				for (size_t i = 0; i < uiSearchCount; ++i) {
					if (!(_Pr()(pclData->k, k) || _Pr()(k, pclData->k))) {
						CDataRec		&clDataRec = *pclData;

						//	从链表中间取出
						clDataRec.pclPrev->pclNext = clDataRec.pclNext;
						clDataRec.pclNext->pclPrev = clDataRec.pclPrev;

						//	移到链表头
						CDataRec		*pclTail = m_pclHead->pclPrev;
						pclTail->pclNext = &clDataRec;
						clDataRec.pclNext = m_pclHead;
						m_pclHead->pclPrev = &clDataRec;
						clDataRec.pclPrev = pclTail;

						m_pclHead = &clDataRec;

						return pclData->spData;
					}
					pclData = pclData->pclNext;
				}
			}
		}

		typename map< KEY, CDataRec, _Pr >::iterator	it = m_mapData.lower_bound(k);
		if (it == m_mapData.end() || _Pr()(k, it->first)) {
			return SmartPointer<DATA>();
		}
		if (&(it->second) != m_pclHead) {
			CDataRec		&clDataRec = it->second;

			//	从链表中间取出
			clDataRec.pclPrev->pclNext = clDataRec.pclNext;
			clDataRec.pclNext->pclPrev = clDataRec.pclPrev;

			//	移到链表头
			CDataRec		*pclTail = m_pclHead->pclPrev;
			pclTail->pclNext = &clDataRec;
			clDataRec.pclNext = m_pclHead;
			m_pclHead->pclPrev = &clDataRec;
			clDataRec.pclPrev = pclTail;

			m_pclHead = &clDataRec;
		}

		return it->second.spData;
	}

	int PutData(const KEY &k, SmartPointer<DATA> &spData)
	{
		typename ThreadModel::Lock	clLock(m_clThreadModel);

		typename map< KEY, CDataRec, _Pr >::iterator	it = m_mapData.lower_bound(k);
		if (it == m_mapData.end() || k < it->first) {
			//	新Data
			it = m_mapData.insert(it, pair<KEY, CDataRec>(k, CDataRec(k, spData)));
			if (it == m_mapData.end()) {
				return FAILURE;
			}
			CDataRec		&clDataRec = it->second;
			clDataRec.k = k;
			if (NULL == m_pclHead) {
				//	链表中无数据
				m_pclHead = &clDataRec;
				clDataRec.pclPrev = &clDataRec;
				clDataRec.pclNext = &clDataRec;
			}
			else {
				//	插入链表头
				CDataRec	*pclTail = m_pclHead->pclPrev;
				pclTail->pclNext = &clDataRec;
				clDataRec.pclNext = m_pclHead;
				m_pclHead->pclPrev = &clDataRec;
				clDataRec.pclPrev = pclTail;

				m_pclHead = &clDataRec;
			}

			//	cache使用过大时删除最后一个
			//	在插入新Data后再删除最后一个，因为在erase时，可能会将it删除
			if (m_mapData.size() > m_uiCapacity) {
				CDataRec		*pclTail = m_pclHead->pclPrev;
				CDataRec		*pclNewTail = pclTail->pclPrev;
				pclNewTail->pclNext = m_pclHead;
				m_pclHead->pclPrev = pclNewTail;

				m_mapData.erase(pclTail->k);
			}
		}
		else {
			CDataRec		&clDataRec = it->second;
			clDataRec.spData = spData;

			if (&(it->second) != m_pclHead) {
				//	从链表中间取出
				clDataRec.pclPrev->pclNext = clDataRec.pclNext;
				clDataRec.pclNext->pclPrev = clDataRec.pclPrev;

				//	移到链表头
				CDataRec		*pclTail = m_pclHead->pclPrev;
				pclTail->pclNext = &clDataRec;
				clDataRec.pclNext = m_pclHead;
				m_pclHead->pclPrev = &clDataRec;
				clDataRec.pclPrev = pclTail;

				m_pclHead = &clDataRec;
			}
		}

		return SUCCESS;
	}

	void RemoveData(const KEY &k)
	{
		typename ThreadModel::Lock	clLock(m_clThreadModel);

		typename map< KEY, CDataRec, _Pr >::iterator	it = m_mapData.lower_bound(k);
		if (it == m_mapData.end() || _Pr()(k, it->first)) {
			return;
		}

		CDataRec		&clDataRec = it->second;

		if (&clDataRec == m_pclHead) {
			m_pclHead = &clDataRec.pclNext;
		}

		//	从链表中间取出
		clDataRec.pclPrev->pclNext = clDataRec.pclNext;
		clDataRec.pclNext->pclPrev = clDataRec.pclPrev;

		m_mapData.erase(it);
	}

	void SetCapacity(size_t uiCapacity)
	{
		typename ThreadModel::Lock	clLock(m_clThreadModel);

		m_uiCapacity = uiCapacity;
	}

	void SetLinearSearchCount(size_t uiLinearSearchCount, size_t uiLinearSearchCondition = 0xFFFFFFFF)
	{
		m_uiLinearSearchCount = uiLinearSearchCount;
		m_uiLinearSearchCondition = uiLinearSearchCondition;
	}

	void ClearData()
	{
		typename ThreadModel::Lock	clLock(m_clThreadModel);

		m_mapData.clear();
		m_pclHead = NULL;
	}

	typedef	typename map< KEY, CDataRec, _Pr >::iterator		iterator;

	iterator begin()
	{
		return m_mapData.begin();
	}

	iterator end()
	{
		return m_mapData.end();
	}

protected:
	map< KEY, CDataRec, _Pr >		m_mapData;
	CDataRec						*m_pclHead;
	size_t							m_uiCapacity;
	size_t							m_uiLinearSearchCount;
	size_t							m_uiLinearSearchCondition;
	ThreadModel						m_clThreadModel;
};

class CCFSimpleFileCacheContext
{
};

template < class KEY, class DATA, class CONTEXT = CCFSimpleFileCacheContext, class _Pr = less<KEY>, class ThreadModel = CCFObjectLevelLockable< int > >
class CCFSimpleFileCache
{
public:
	CCFSimpleFileCache() {}
	virtual ~CCFSimpleFileCache() {}

	SmartPointer<DATA> GetData(const KEY &k)
	{
		typename ThreadModel::Lock	clLock(m_clThreadModel);

		SmartPointer<DATA>	spclData = m_clSimpleCache.GetData(k);
		if (spclData == NULL) {
			if (!spclData.Create()) {
				return SmartPointer<DATA>();
			}
			if (SUCCESS != ReadData(k, *spclData)) {
				return SmartPointer<DATA>();
			}
			if (SUCCESS != m_clSimpleCache.PutData(k, spclData)) {
				return SmartPointer<DATA>();
			}
		}
		return spclData;
	}

	void SetContext(const CONTEXT &clContext)
	{
		m_clContext = clContext;
	}

	CONTEXT &GetContext()
	{
		return m_clContext;
	}

	void SetCapacity(size_t uiCapacity)
	{
		m_clSimpleCache.SetCapacity(uiCapacity);
	}

	void ClearData()
	{
		typename ThreadModel::Lock	clLock(m_clThreadModel);

		m_clSimpleCache.ClearData();
	}

protected:
	virtual int ReadData(const KEY &k, DATA &d) = 0;

protected:
	CONTEXT											m_clContext;
	CCFSimpleCache< KEY, DATA, _Pr, ThreadModel >	m_clSimpleCache;
	ThreadModel										m_clThreadModel;
};

template < class ELEMENT, class CONTAINER >
class CCFRefCountHolder
{
public:
	CCFRefCountHolder()
		: m_pclElem(NULL)
	{
	}

	CCFRefCountHolder(ELEMENT *p, SmartPointer<CONTAINER> spContainer)
		: m_pclElem(p), m_spclContainer(spContainer)
	{
	}

	ELEMENT* operator->() const
	{
		return m_pclElem;
	}

	operator ELEMENT*() const
	{
		return m_pclElem;
	}

	ELEMENT* Get() const
	{
		return m_pclElem;
	}

	ELEMENT							*m_pclElem;
	SmartPointer<CONTAINER>			m_spclContainer;
};
