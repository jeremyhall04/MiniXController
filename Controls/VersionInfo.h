// VersionInfo.h: interface for the CVersionInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERSIONINFO_H__9A48D272_9995_4B21_BBA3_FE348B66F011__INCLUDED_)
#define AFX_VERSIONINFO_H__9A48D272_9995_4B21_BBA3_FE348B66F011__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma comment (lib, "version.lib")

class CVersionInfo  
{
public:
	CVersionInfo();
	CVersionInfo(LPCTSTR imageName);
	virtual ~CVersionInfo();

    CString GetProductName() const;
    CString GetProductVersion() const;
    CString GetCompanyName() const;
    CString GetCopyright() const;
    CString GetComments() const;
    CString GetFileDescription() const;
    CString GetFileVersion() const;
    CString GetInternalName() const;
    CString GetLegalTrademarks() const;
    CString GetPrivateBuild() const;
    CString GetSpecialBuild() const;

private: 
    void InitVer(LPCTSTR imageName);
    CString GetValue(LPCTSTR key) const;

    LPVOID m_ResourceData;
    CString m_LangID;

};

#endif // !defined(AFX_VERSIONINFO_H__9A48D272_9995_4B21_BBA3_FE348B66F011__INCLUDED_)
