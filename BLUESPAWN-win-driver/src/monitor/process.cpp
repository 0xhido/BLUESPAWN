#include "../../headers/monitor/process.h"

VOID
KbsPsCreateProcessNotifyRoutineEx(
    _Inout_ PEPROCESS Process,
    _In_ HANDLE ProcessId,
    _Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo) {
    UNREFERENCED_PARAMETER(Process);

    ULONG allocSize = sizeof(ProcessCreationEvent);
    ULONG fileNameSize = 0;
    ULONG commandLineSize = 0;

    if (CreateInfo) {
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

        // Item Header
        KeQuerySystemTimePrecise(&item->CreationTime);
        item->Size = sizeof(ProcessCreationEvent) + fileNameSize + commandLineSize;
        item->Type = EventType::ProcessCreate;

        // Process Creation Data
        item->dwPID = HandleToULong(ProcessId);
        item->dwParentProcess = HandleToULong(CreateInfo->ParentProcessId);

        item->FileNameLength = 0;
        item->FileNameOffset = sizeof(item); // Starting right after the struct
        if (fileNameSize > 0) {
            memcpy((UCHAR*)&item + item->FileNameOffset,
                CreateInfo->ImageFileName->Buffer,
                fileNameSize);
            item->FileNameLength = fileNameSize / sizeof(WCHAR);
        }

        item->CommandLineLength = 0;
        item->CommandLineOffset = sizeof(item) + fileNameSize;
        if (commandLineSize > 0) {
            memcpy((UCHAR*)&item + item->CommandLineOffset,
                CreateInfo->CommandLine->Buffer,
                commandLineSize);
            item->CommandLineLength = commandLineSize / sizeof(WCHAR);
        }

        // TODO EventManager.AddEvent(item);
    }

}