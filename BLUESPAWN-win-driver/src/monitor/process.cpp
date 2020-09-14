#include "BluespawnDrv.h"
#include "monitor/process.h"

VOID
KbsProcessNotifyEx(
    _Inout_ PEPROCESS Process,
    _In_ HANDLE ProcessId,
    _Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo) {
    UNREFERENCED_PARAMETER(Process);

    ULONG allocSize = sizeof(ProcessCreationEvent);
    ULONG fileNameSize = 0;
    ULONG commandLineSize = 0;

    if (CreateInfo) {   // Process Creation
        if (CreateInfo->ImageFileName) {
            fileNameSize = CreateInfo->ImageFileName->Length;
            allocSize += fileNameSize;
        }

        if (CreateInfo->CommandLine) {
            commandLineSize = CreateInfo->CommandLine->Length;
            allocSize += commandLineSize;
        }

        auto item = (ProcessCreationEvent*)ExAllocatePoolWithTag(PagedPool, allocSize, DRIVER_TAG);
        if (item == nullptr) {
            dprint("Item memory allocation failed\n");
            return;
        }

        // Event Header
        KeQuerySystemTimePrecise(&item->CreationTime);
        item->Size = sizeof(ProcessCreationEvent) + fileNameSize + commandLineSize;
        item->Type = EventType::ProcessCreate;

        // Process Creation Data
        item->PID = HandleToULong(ProcessId);
        item->ParentProcess = HandleToULong(CreateInfo->ParentProcessId);

        item->FileNameLength = 0;
        item->FileNameOffset = sizeof(ProcessCreationEvent); // Starting right after the struct
        if (fileNameSize > 0) {
            memcpy((UCHAR*)item + item->FileNameOffset,
                CreateInfo->ImageFileName->Buffer,
                fileNameSize);
            item->FileNameLength = fileNameSize / sizeof(WCHAR);
        }

        item->CommandLineLength = 0;
        item->CommandLineOffset = sizeof(ProcessCreationEvent) + fileNameSize;
        if (commandLineSize > 0) {
            memcpy((UCHAR*)item + item->CommandLineOffset,
                CreateInfo->CommandLine->Buffer,
                commandLineSize);
            item->CommandLineLength = commandLineSize / sizeof(WCHAR);
        }

        g_EventCollector.AddEvent(item);
        ExFreePoolWithTag(item, DRIVER_TAG);
    }
    else {  // Process Termination
        ProcessExitEvent item;

        // Event Header
        KeQuerySystemTimePrecise(&item.CreationTime);
        item.Size = sizeof(ProcessExitEvent);
        item.Type = EventType::ProcessExit;

        // Process Exit Data
        item.PID = HandleToULong(ProcessId);

        g_EventCollector.AddEvent(&item);
    }
}