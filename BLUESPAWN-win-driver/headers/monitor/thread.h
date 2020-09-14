#pragma once

#include "../../headers/BluespawnDrv.h"
#include "../../common/common.h"

VOID KbsThreadNotify(
    _In_ HANDLE ProcessId,
    _In_ HANDLE ThreadId,
    _In_ BOOLEAN Create);
