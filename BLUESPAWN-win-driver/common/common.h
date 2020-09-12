#pragma once

enum class EventType : short {
    None,
    ProcessCreate,
    ProcessExit,
    ThreadCreate,
    ThreadExit,
    ImageLoad
};

struct Event {
    EventType Type;
    ULONG Size;
    LARGE_INTEGER CreationTime;
};

typedef struct _PROCESS_CREATION_DATA : Event {
    ULONG dwPID;
    ULONG dwParentProcess;
    ULONG FileNameLength;
    ULONG FileNameOffset;
    ULONG CommandLineLength;
    ULONG CommandLineOffset;
} PROCESS_CREATION_DATA, * PPROCESS_CREATION_DATA, ProcessCreationEvent;