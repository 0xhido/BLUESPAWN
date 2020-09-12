#pragma once

#include <ntddk.h>

#include "../common/common.h"
#include "stl/FastMutex.h"

#define DRIVER_NAME L"BluespawnDrv"
#define DRIVER_DEVICE_NAME (L"\\Device\\" DRIVER_NAME)
#define DRIVER_SYMB_LINK (L"\\??\\" DRIVER_NAME)

#define DRIVER_PREFIX "[ BLUESPAWN ] "

#define DRIVER_TAG 'SB' // BS

#ifdef DBG
#define dprint(Format, ...) DbgPrint(DRIVER_PREFIX Format, __VA_ARGS__)
#else
#define dprint
#endif // DBG

//
//	Globals
//

extern ULONG g_EventListMaxCount;
extern LIST_ENTRY g_EventListHead;
extern ULONG g_EventListCount;
extern FastMutex g_EventListMutex;