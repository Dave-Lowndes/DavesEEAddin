typedef struct tagDEBUGHELPER
{
	DWORD dwVersion;
	HRESULT (WINAPI *ReadDebuggeeMemory)( struct tagDEBUGHELPER *pThis, DWORD dwAddr, DWORD nWant, VOID* pWhere, DWORD *nGot );
	// from here only when dwVersion >= 0x20000
	DWORDLONG (WINAPI *GetRealAddress)( struct tagDEBUGHELPER *pThis );
	HRESULT (WINAPI *ReadDebuggeeMemoryEx)( struct tagDEBUGHELPER *pThis, DWORDLONG qwAddr, DWORD nWant, VOID* pWhere, DWORD *nGot );
	int (WINAPI *GetProcessorType)( struct tagDEBUGHELPER *pThis );
} DEBUGHELPER;

typedef HRESULT (WINAPI *CUSTOMVIEWER)( DWORD dwAddress, DEBUGHELPER *pHelper, int nBase, BOOL bUniStrings, char *pResult, size_t max, DWORD reserved );

/* From MSDN: http://msdn.microsoft.com/en-us/library/8fwk67y3(v=vs.90).aspx
HRESULT WINAPI CustomViewer(
   DWORD dwAddress,       // low 32-bits of address
   DEBUGHELPER *pHelper,  // callback pointer to access helper functions
   int nBase,             // decimal or hex
   BOOL bIgnore,          // not used
   char *pResult,         // where the result needs to go
   size_t max,            // how large the above buffer is
   DWORD dwReserved       // always pass zero
)
*/
