#pragma once

#include <ntddk.h>

namespace utils {
	NTSTATUS CompleteIrp(
		_In_ PIRP Irp,
		_Inout_ NTSTATUS status = STATUS_SUCCESS,
		_In_ ULONG_PTR inforamtion = 0
	);
};