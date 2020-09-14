#pragma once

enum class EventType : short {
    None,
    ProcessCreate,
    ProcessExit,
    ThreadCreate,
    ThreadExit,
    LoadImage
};

struct Event {
    EventType Type;
    ULONG Size;
    LARGE_INTEGER CreationTime;
};

struct ProcessCreationEvent : Event {
    ULONG PID;
    ULONG ParentProcess;
    ULONG FileNameLength;
    ULONG FileNameOffset;
    ULONG CommandLineLength;
    ULONG CommandLineOffset;
};

struct ProcessExitEvent : Event {
    ULONG PID;
};

struct ThreadEvent : Event {
    ULONG PID;
    ULONG TID;
};

struct LoadImageEvent : Event {
    ULONG PID;
    PVOID ImageBaseAddress;
    ULONG ImageNameLenght;
    ULONG ImageNameOffset;
};