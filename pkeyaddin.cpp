#include <windows.h>
#include "custview.h"
#include "TimeAddIn.h"
#include <string>
using namespace std;
#include <atlbase.h>
#include <shobjidl.h>   // SHGetPropertyStoreFromParsingName, etc
#include <propkey.h>    // PKEY_Music_AlbumArtist
#include <propvarutil.h>// InitPropVariantFromString, needs shlwapi.lib
#pragma comment( lib, "propsys.lib" )

static wstring GetKeyName( const PROPERTYKEY & Key )
{
	wstring sRet;

	CComPtr<IPropertyDescription> pPropDesc;

	HRESULT hr = PSGetPropertyDescription( Key, IID_PPV_ARGS(&pPropDesc) );
	if ( SUCCEEDED( hr ) )
	{
		CComHeapPtr<wchar_t> pCName;
		hr = pPropDesc->GetCanonicalName( &pCName );
		if ( SUCCEEDED( hr ) )
		{
			sRet = pCName;

			CComHeapPtr<wchar_t> pDName;
			hr = pPropDesc->GetDisplayName( &pDName );
			if ( SUCCEEDED( hr ) )
			{
				sRet += L" \"";
				sRet += pDName;
				sRet += L"\"";
			}
			else
			{
				sRet += L" ***No Display Name***";
			}
		}
		else
		{
			sRet = L"Failed to get name info";
		}
	}
	else
	{
		sRet = L"Invalid";
	}

	return sRet;
}

ADDIN_API HRESULT WINAPI AddIn_PropertyKey( DWORD /*dwAddress*/, DEBUGHELPER *pHelper, int /*nBase*/, BOOL /*bUniStrings*/,
										  char *pResult, size_t max, DWORD /*reserved*/ )
{
	PROPERTYKEY Key;
    DWORD nGot;
	HRESULT hr;

    // read PROPERTYKEY from debuggee memory space
	hr = pHelper->ReadDebuggeeMemoryEx( pHelper, pHelper->GetRealAddress( pHelper ), sizeof( Key ), &Key, &nGot );
	if ( hr == S_OK )
	{
		if ( nGot == sizeof( Key ) )
		{
			wstring str = GetKeyName( Key );
			CW2A pA( str.c_str() );
			lstrcpynA( pResult, pA, max );
		}
		else
		{
			hr = E_FAIL;
		}
	}

	return hr;
}
