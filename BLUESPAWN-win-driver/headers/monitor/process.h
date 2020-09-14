#pragma once

#include "BluespawnDrv.h"
#include "../common/common.h"

VOID KbsProcessNotifyEx(
    _Inout_ PEPROCESS Process,
    _In_ HANDLE ProcessId,
    _Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo);