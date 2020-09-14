#include "BluespawnDrv.h"
#include "monitor/thread.h"

VOID 
KbsThreadNotify(
    _In_ HANDLE ProcessId,
    _In_ HANDLE ThreadId,
    _In_ BOOLEAN Create) {
    ThreadEvent item;

    // Event Header
    KeQuerySystemTimePrecise(&item.CreationTime);
    item.Size = sizeof(ThreadEvent);
    item.Type = Create ? EventType::ThreadCreate : EventType::ThreadExit;
    // Thread Data
    item.PID = HandleToULong(ProcessId);
    item.TID = HandleToULong(ThreadId);

    dprint("[Thread%s] Process [%ul] Thread [%ul]\n", Create ? "Create" : "Exit", item.PID, item.TID);

    g_EventCollector.AddEvent(&item);
}