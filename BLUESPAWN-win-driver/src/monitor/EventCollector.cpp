#include "BluespawnDrv.h"
#include "monitor/EventCollector.h"

void EventCollector::Init() {
	_events.Init();
	_count = 0;
}

bool EventCollector::AddEvent(_In_ Event* item) {
	EventItem* listItem = (EventItem*)ExAllocatePoolWithTag(PagedPool, sizeof(LIST_ENTRY) + item->Size, DRIVER_TAG);
	if (listItem == nullptr) {
		return false;
	}
	memset(listItem, 0, sizeof(LIST_ENTRY) + item->Size);

	// Copy data into list item
	memcpy(&listItem->Data, item, item->Size);

	_events.PushBack(listItem);
	_count++;

	return true;
}

LinkedList<EventItem, FastMutex>* EventCollector::GetEvents() {
	return &_events;
}

void EventCollector::ClearEvents() {
	while (!IsListEmpty(_events.GetHead())) {
		EventItem* item = _events.RemoveHead();
		ExFreePoolWithTag(item, DRIVER_TAG);
	}
	_count = 0;
}