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