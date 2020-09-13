#pragma once

#include "stl/FastMutex.h"
#include "stl/LinkedList.h"

struct EventItem {
	LIST_ENTRY Entry;
	Event Data;
};

class EventCollector
{
public:
	void Init();

	bool AddEvent(_In_ Event* item);

	LinkedList<EventItem, FastMutex>* GetEvents();

	void ClearEvents();

private:
	ULONG _count;
	LinkedList<EventItem, FastMutex> _events;
};

