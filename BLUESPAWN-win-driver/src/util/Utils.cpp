#include "../../headers/util/Utils.h"

namespace utils {
	NTSTATUS CompleteIrp(
		_In_ PIRP Irp,
		_Inout_ NTSTATUS status,
		_In_ ULONG_PTR inforamtion) {
		Irp->IoStatus.Status = status;
		Irp->IoStatus.Information = inforamtion;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);

		return status;
	}
}
