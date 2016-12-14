#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include "custview.h"
#include "TimeAddIn.h"
#include <string>
using namespace std;

static string FormatSystemTime( const SYSTEMTIME & st )
{
	char szDate[50];

	size_t NumChars = GetDateFormatA( GetThreadLocale(), DATE_SHORTDATE, &st, NULL, szDate, _countof( szDate ) );

	if ( NumChars == 0 )
	{
		// Failed
		szDate[0] = '\0';
	}

	char szTime[50];

	NumChars = GetTimeFormatA( GetThreadLocale(), TIME_FORCE24HOURFORMAT, &st, NULL, szTime, _countof( szTime ) );

	if ( NumChars == 0 )
	{
		// Failed
		szTime[0] = '\0';
	}

	string str;
	str = szDate;
	str += ' ';
	str += szTime;

	// If the string isn't more than the separator something's wrong, return an empty string
	if ( str.length() <= 1 )
	{
		str.clear();
	}

	return str;
}

ADDIN_API HRESULT WINAPI AddIn_SystemTime( DWORD /*dwAddress*/, DEBUGHELPER *pHelper, int /*nBase*/, BOOL /*bUniStrings*/,
	char *pResult, size_t max, DWORD /*reserved*/ )
{
	SYSTEMTIME SysTime;
	DWORD nGot;

	// read system time from debuggee memory space
	HRESULT hr;
	hr = pHelper->ReadDebuggeeMemoryEx( pHelper, pHelper->GetRealAddress( pHelper ), sizeof( SysTime ), &SysTime, &nGot );
	if ( hr == S_OK )
	{
		if ( nGot == sizeof( SysTime ) )
		{
			string str = FormatSystemTime( SysTime );

			lstrcpynA( pResult,
						!str.empty() ?
							str.c_str() :
							"Invalid",
						max );
		}
		else
		{
			hr = E_FAIL;
		}
	}

	return hr;
}

ADDIN_API HRESULT WINAPI AddIn_FileTime( DWORD /*dwAddress*/, DEBUGHELPER *pHelper, int nBase, BOOL /*bUniStrings*/,
	char *pResult, size_t BufferMax, DWORD /*reserved*/ )
{
	FILETIME ftUtc;
	DWORD nGot;
	HRESULT hr;

	// read file time from debuggee memory space
	hr = pHelper->ReadDebuggeeMemoryEx( pHelper, pHelper->GetRealAddress( pHelper ), sizeof( ftUtc ), &ftUtc, &nGot );
	if ( hr == S_OK )
	{
		if ( nGot == sizeof( ftUtc ) )
		{
			/* Note: VS2008 (for me) passes a buffer of size 398 */

			// convert to SystemTime
			SYSTEMTIME SysTime;

			if ( FileTimeToSystemTime( &ftUtc, &SysTime ) )
			{
				string strUtc = FormatSystemTime( SysTime );

				SYSTEMTIME lst;

				if ( SystemTimeToTzSpecificLocalTime( NULL, &SysTime, &lst ) )
				{
					string strLoc;
					strLoc = FormatSystemTime( lst );

					// Determine if there's a difference between local & utc so that I can display the most appropriate zone name
					LONGLONG diffInMins;
					{
						FILETIME lft;
						SystemTimeToFileTime( &lst, &lft );
						LONGLONG diffInTicks = reinterpret_cast<LARGE_INTEGER*>(&lft)->QuadPart - reinterpret_cast<LARGE_INTEGER*>(&ftUtc)->QuadPart;
						diffInMins = diffInTicks / (10000000 * 60);
					}

					TIME_ZONE_INFORMATION tzi;
					GetTimeZoneInformation( &tzi );

					// Remove the current bias from UTC and if it's now non-zero, its the DST zone
					diffInMins += tzi.Bias;
					LPCWSTR ZoneName = diffInMins != 0 ? tzi.DaylightName : tzi.StandardName;

					sprintf_s( pResult, BufferMax, "[utc] %s [%ls] %s", strUtc.c_str(), ZoneName, strLoc.c_str() );
				}
				else
				{
					sprintf_s( pResult, BufferMax, "utc: %s ???", strUtc.c_str() );
				}
			}
			else
			{
				// Prior to VS2017 RC2, VS only ever passed 10 for nBase, newer versions should support base values of 2, 8, 10, or 16
				char szHigh[33];	// 32 chars max for base 2 from a DWORD value
				char szLow[33];
				_ui64toa_s( ftUtc.dwHighDateTime, szHigh, _countof(szHigh), nBase );
				_ui64toa_s( ftUtc.dwLowDateTime, szLow, _countof(szLow), nBase );

				char * Prefix;
				switch ( nBase )
				{
				case 2:
					Prefix = "0b";	// binary
					break;
				case 8:
					Prefix = "0";	// octal
					break;
				case 10:
					Prefix = "";	// decimal
					break;
				case 16:
					Prefix = "0x";	// hex
					break;
				default:
					Prefix = "?";	// unknown
					break;
				}

				sprintf_s( pResult, BufferMax, "Invalid [%s%s:%s%s]", Prefix, szHigh, Prefix, szLow );
			}
		}
		else
		{
			hr = E_FAIL;
		}
	}

	return hr;
}
