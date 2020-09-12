#include "../headers/BluespawnDrv.h"

#include "../headers/monitor/process.h"

#include "../headers/util/Utils.h"

//
// Declarations
//

extern "C" DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD DriverUnload;
_Dispatch_type_(IRP_MJ_CREATE | IRP_MJ_CLOSE) DRIVER_DISPATCH KbsDispatchCreateClose;

//
//	Globals
//

PDRIVER_OBJECT g_KbsDriverObject;
PDEVICE_OBJECT g_KbsDeviceObject;
BOOLEAN g_PsCreateProcessNotifyRoutineExCreated;

// TODO Create EventManage global instance

//
//	Driver load/unload
//

NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
	// TODO Create Registry configuration utility
	UNREFERENCED_PARAMETER(RegistryPath);

	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING deviceName;
	UNICODE_STRING symbLink;
	PDEVICE_OBJECT DeviceObject = nullptr;
	BOOLEAN symbLinkCreated = false;


	PAGED_CODE();

	g_KbsDriverObject = DriverObject;

	RtlInitUnicodeString(&deviceName, DRIVER_DEVICE_NAME);
	RtlInitUnicodeString(&symbLink, DRIVER_SYMB_LINK);

	// TODO Initialize EventManager

	do {
		status = IoCreateDevice(
			DriverObject,
			0,
			&deviceName,
			FILE_DEVICE_UNKNOWN,
			FILE_DEVICE_SECURE_OPEN,
			FALSE,
			&DeviceObject);
		if (!NT_SUCCESS(status)) {
			dprint("Device object creation failed (0x%08X)\n", status);
			break;
		}

		g_KbsDeviceObject = DeviceObject;

		status = IoCreateSymbolicLink(&symbLink, &deviceName);
		if (!NT_SUCCESS(status)) {
			dprint("Symbolic link creation failed (0x%08X)\n", status);
			break;
		}
		symbLinkCreated = true;

		status = PsSetCreateProcessNotifyRoutineEx(KbsPsCreateProcessNotifyRoutineEx, FALSE);
		if (!NT_SUCCESS(status)) {
			dprint("Process notify routine registration failed (0x%08X)\n", status);
			break;
		}
		g_PsCreateProcessNotifyRoutineExCreated = TRUE;
	} while (false);

	if (!NT_SUCCESS(status)) {
		if (symbLinkCreated)
			IoDeleteSymbolicLink(&symbLink);

		if (DeviceObject)
			IoDeleteDevice(DeviceObject);

		return status;
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE] = KbsDispatchCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = KbsDispatchCreateClose;
	DriverObject->DriverUnload = DriverUnload;

	DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

	dprint("Driver loaded\n");

	return status;
}

VOID
DriverUnload(_In_ PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);

	UNICODE_STRING symbLink;

	PAGED_CODE();

	RtlInitUnicodeString(&symbLink, DRIVER_SYMB_LINK);

	UnregisterNotifyRoutineCallbacks();
	
	// TODO EventManager.ClearEvents();

	IoDeleteSymbolicLink(&symbLink);
	IoDeleteDevice(g_KbsDeviceObject);

	dprint("Driver unloaded\n");
}

//	TODO Move to different file
//	Dispatch routines 
//

NTSTATUS
KbsDispatchCreateClose(_In_ PDEVICE_OBJECT DeviceObject, _Inout_ PIRP Irp)
{
	// TODO Create users' privileges here

	UNREFERENCED_PARAMETER(DeviceObject);

	return utils::CompleteIrp(Irp);
}

//
//	Helper functions
//
VOID
UnregisterNotifyRoutineCallbacks() {
	if (g_PsCreateProcessNotifyRoutineExCreated)
		PsSetCreateProcessNotifyRoutineEx(KbsPsCreateProcessNotifyRoutineEx, TRUE);
}