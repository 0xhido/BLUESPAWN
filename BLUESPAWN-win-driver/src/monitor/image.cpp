#include "BluespawnDrv.h"
#include "monitor/image.h"

VOID
KbsLoadImageNotify(
	PUNICODE_STRING FullImageName,
	HANDLE ProcessId,
	PIMAGE_INFO ImageInfo) {
	if (ProcessId == nullptr)
		return; // System image - ignore

	ULONG allocSize = sizeof(LoadImageEvent);
	ULONG imageNameSize = 0;

	if (FullImageName) {
		imageNameSize = FullImageName->Length;
		allocSize += imageNameSize;
	}

	auto item = (LoadImageEvent*)ExAllocatePoolWithTag(PagedPool, allocSize, DRIVER_TAG);
	if (item == nullptr) {
		dprint("LoadImageEvent item memory allocation failed\n");
		return;
	}

	// Event Headers
	KeQuerySystemTimePrecise(&item->CreationTime);
	item->Size = sizeof(LoadImageEvent) + imageNameSize;
	item->Type = EventType::ImageLoad;

	// Image Data

}