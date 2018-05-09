#ifndef	DPDATASHARED_H
#define	DPDATASHARED_H

class CDPLevelMetadataRec : public CDPData
{
public:
	CDPLevelMetadataRec() : m_uiCoordShift( 0 ), m_uiMinScaleDenominator( 0 ), m_uiMaxScaleDenominator( 0 )
		, m_iUpperLevel( -1 ), m_iDownLevel( -1 ) {}
	uint			m_uiCoordShift;
	uint			m_uiMinScaleDenominator;
	uint			m_uiMaxScaleDenominator;
	int				m_iUpperLevel;
	int				m_iDownLevel;
};

class CDPDataShared : public CDPData
{
public:
	CDPDataShared() {}
	virtual ~CDPDataShared() {}
};

#endif	//	DPDATASHARED_H
