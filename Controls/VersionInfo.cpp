// VersionInfo.cpp: implementation of the CVersionInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VersionInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVersionInfo::CVersionInfo()
{
    // m_pszExeName is the real image name in MFC
    CString strName = AfxGetApp()->m_pszExeName;
    strName += ".EXE";
    InitVer(strName);
}

CVersionInfo::CVersionInfo(LPCTSTR imageName)
{
	CString strName;
	strName = imageName;
	if (strName.IsEmpty())
	{
		strName = AfxGetApp()->m_pszExeName;
		strName += ".EXE";
	}
	InitVer(strName);
}


CVersionInfo::~CVersionInfo()
{
    ::free(m_ResourceData);

}

// Attributes

CString CVersionInfo::GetProductName() const
{
    return GetValue(_T("ProductName"));
}

CString CVersionInfo::GetProductVersion() const
{
    return GetValue(_T("ProductVersion"));
}

CString CVersionInfo::GetCompanyName() const
{
    return GetValue(_T("CompanyName"));
}

CString CVersionInfo::GetCopyright() const
{
    return GetValue(_T("LegalCopyright"));
}

CString CVersionInfo::GetComments() const
{
    return GetValue(_T("Comments"));
}

CString CVersionInfo::GetFileDescription() const
{
    return GetValue(_T("FileDescription"));
}

CString CVersionInfo::GetFileVersion() const
{
    return GetValue(_T("FileVersion"));
}

CString CVersionInfo::GetInternalName() const
{
    return GetValue(_T("InternalName"));
}

CString CVersionInfo::GetLegalTrademarks() const
{
    return GetValue(_T("LegalTrademarks"));
}

CString CVersionInfo::GetPrivateBuild() const
{
    return GetValue(_T("PrivateBuild"));
}

CString CVersionInfo::GetSpecialBuild() const
{
    return GetValue(_T("SpecialBuild"));
}

// Construction helper

void CVersionInfo::InitVer(LPCTSTR imageName)
{
    DWORD resourceSize, zeroHandle = 0;
    LPTSTR image = const_cast<LPTSTR>(imageName); // WinAPI wants non-const arguments

    m_ResourceData = NULL;
    resourceSize = ::GetFileVersionInfoSize(image, &zeroHandle);
    if (zeroHandle == 0) 
	{
        m_ResourceData = ::malloc(resourceSize);
        if (::GetFileVersionInfo(image, NULL, resourceSize, m_ResourceData)) 
		{
            // Pick up the language ID used by this version resource.
            // Does not work with multi-language resources.
            UINT dataSize;
            LPVOID valuePointer;
            if (::VerQueryValue(m_ResourceData, _T("\\VarFileInfo\\Translation"), &valuePointer, &dataSize)) 
			{
                // Must know the size of the scalar type used.
                // I bet there are more portable ways of doing this...
                __int16* lang = reinterpret_cast<__int16*>(valuePointer);
                m_LangID.Format("%04x%04x", lang[0], lang[1]);
            }
        }
    }
}

// Attribute helper

CString CVersionInfo::GetValue(LPCTSTR key) const
{
    CString str;
    LPVOID valuePointer;
    UINT dataSize;

    ASSERT(m_ResourceData != NULL);

    str.Format("\\StringFileInfo\\%s\\%s", m_LangID, key);
    // We need a non-const pointer to the string data.
    ::VerQueryValue(m_ResourceData, str.GetBuffer(0), &valuePointer, &dataSize);
    str.ReleaseBuffer();
    str.Format("%s", reinterpret_cast<LPTSTR>(valuePointer));
    return str;
}
