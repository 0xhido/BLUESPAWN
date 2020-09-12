#pragma once

#include <sal.h>
#include <ntdef.h>

#include "../../common/common.h"

class EventManager
{
public:
	EventManager();
	bool AddEvent(_In_ Event* item);

private:
	ULONG Count;
};

