#pragma once

#include "BluespawnDrv.h"
#include "../common/common.h"

VOID KbsLoadImageNotify(
	PUNICODE_STRING FullImageName,
	HANDLE ProcessId,
	PIMAGE_INFO ImageInfo
);