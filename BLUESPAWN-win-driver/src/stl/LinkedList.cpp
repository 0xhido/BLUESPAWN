#include "../../headers/stl/LinkedList.h"

template<typename T, typename TLock>
void LinkedList<T, TLock>::Init() {
	InitializeListHead(&_head);
	_lock.Init();
}

template<typename T, typename TLock>
void LinkedList<T, TLock>::PushBack(T* item) {
	AutoLock<TLock> locker(_lock);
	InsertTailList(&_head, &item->Entry); // expects a LIST_ENTRY named "Entry"
}

template<typename T, typename TLock>
void LinkedList<T, TLock>::PushFront(T* item) {
	AutoLock<TLock> locker(_lock);
	InsertHeadList(&_head, &item->Entry);
}

template<typename T, typename TLock>
PLIST_ENTRY LinkedList<T, TLock>::GetHead() {
	AutoLock<TLock> locker(_lock);
	return &_head;
}

template<typename T, typename TLock>
T* LinkedList<T, TLock>::RemoveHead() {
	AutoLock<TLock> locker(_lock);
	PLIST_ENTRY entry = RemoveHeadList(&_head);
	return CONTAINING_RECORD(entry, T, Entry);
}

template<typename T, typename TLock>
void LinkedList<T, TLock>::RemoveItem(T* item) {
	AutoLock<TLock> locker(_lock);
	PLIST_ENTRY entry = RemoveEntryList(&item->Entry);
	return CONTAINING_RECORD(entry, T, Entry);
}