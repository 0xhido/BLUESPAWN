#include "BluespawnDrv.h"
#include "util/ProcessList.h"

void ProcessList::Init() {
	_processes.Init();
}

void ProcessList::AddProcess(ULONG ProcessId) {
	ProcessEntry* pe = (ProcessEntry*)ExAllocatePoolWithTag(PagedPool, sizeof(ProcessEntry), DRIVER_TAG);
	if (pe == nullptr) {
		dprint("Process entry memory allocation failed\n");
		return;
	}
	pe->ProcessId = ProcessId;
	
	_processes.PushBack(pe);
}

void ProcessList::RemoveProcess(ULONG ProcessId) {
	AutoLock<FastMutex> locker(_processes.GetLock());

	auto head = _processes.GetHead();
	auto current = head;

	while (current != head) {
		auto entry = CONTAINING_RECORD(current, ProcessEntry, Entry);

		if (ProcessId == entry->ProcessId) {
			RemoveEntryList(&entry->Entry);
			return;
		}

		current = current->Flink;
	}
}

bool ProcessList::IsNewProcess(ULONG ProcessId) {
	AutoLock<FastMutex> locker(_processes.GetLock());

	auto head = _processes.GetHead();
	auto current = head;

	while (current != head) {
		auto entry = CONTAINING_RECORD(current, ProcessEntry, Entry);

		if (ProcessId == entry->ProcessId) {
			return false;
		}

		current = current->Flink;
	}
	
	return false;
}

void ProcessList::ClearList() {
	while (!IsListEmpty(_processes.GetHead())) {
		ProcessEntry* item = _processes.RemoveHead();
		ExFreePoolWithTag(item, DRIVER_TAG);
	}
}