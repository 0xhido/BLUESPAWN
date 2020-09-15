#pragma once

#include <ntddk.h>
#include "stl/LinkedList.h"

struct ProcessEntry {
	LIST_ENTRY Entry;
	ULONG ProcessId;
};

class ProcessList {
public:
	void Init();
	void AddProcess(ULONG ProcessId);
	void RemoveProcess(ULONG ProcessId);
	bool IsNewProcess(ULONG ProcessId);
	void ClearList();
private:
	LinkedList<ProcessEntry, FastMutex> _processes;
};