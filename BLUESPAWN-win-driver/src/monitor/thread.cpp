#include "BluespawnDrv.h"
#include "monitor/thread.h"

VOID 
KbsThreadNotify(
    _In_ HANDLE ProcessId,
    _In_ HANDLE ThreadId,
    _In_ BOOLEAN Create) {
    ThreadEvent item;
    BOOLEAN IsRemoteThread = FALSE;

    if (Create) {
        ULONG sourcePID = HandleToULong(PsGetCurrentProcessId());
        ULONG targetPID = HandleToULong(ProcessId);

        if (sourcePID != targetPID &&
            sourcePID != 4 &&
            !g_ProcessList.IsNewProcess(targetPID)) {
            IsRemoteThread = TRUE;

            RemoteThreadEvent rtItem;
            // Event Header
            KeQuerySystemTimePrecise(&rtItem.CreationTime);
            rtItem.Size = sizeof(RemoteThreadEvent);
            rtItem.Type = EventType::RemoteThread;
            // Thread Data
            rtItem.sourcePID = sourcePID;
            rtItem.targetPID = targetPID;
            rtItem.targetTID = HandleToULong(ThreadId);

            dprint("[RemoteThread] Source Process [%ul] -> TargetProcess [%ul] Thread ID [%ul]\n", rtItem.sourcePID, rtItem.targetPID, rtItem.targetTID);

            g_EventCollector.AddEvent(&item);
        }
    }

    if (!IsRemoteThread) {
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
}