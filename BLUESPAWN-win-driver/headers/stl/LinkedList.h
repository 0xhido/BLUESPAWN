#pragma once

#include "AutoLock.h"
#include "FastMutex.h"

template<typename T, typename TLock = FastMutex>
class LinkedList
{
public:
	void Init();

	// expects a LIST_ENTRY named "Entry"
	void PushBack(T* item);
	void PushFront(T* item);
	PLIST_ENTRY GetHead();
	T* RemoveHead();
	void RemoveItem(T* item);

private:
	LIST_ENTRY _head;
	TLock _lock;
};

