#include <ntifs.h>

#define Print(a1, ...) DbgPrintEx(0, 0, a1)
#define IO_SEND_MESSAGE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x996, METHOD_BUFFERED, FILE_ANY_ACCESS)
UNICODE_STRING dev, dos;
PDEVICE_OBJECT DeviceObject;

NTSTATUS DriverUnload(PDRIVER_OBJECT DriverObject) {

	IoDeleteSymbolicLink(&dos);
	IoDeleteDevice(DriverObject->DeviceObject);

	Print("Driver Unloaded");

	return STATUS_SUCCESS;

}


NTSTATUS Create(PDEVICE_OBJECT DeviceObject, PIRP Irp) {


	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;

}

NTSTATUS Close(PDEVICE_OBJECT DeviceObject, PIRP Irp) {


	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;

}


NTSTATUS Ioctl(PDEVICE_OBJECT DeviceObject, PIRP Irp) {

	NTSTATUS Status;
	ULONG Bytes = 0;

	PIO_STACK_LOCATION io = IoGetCurrentIrpStackLocation(Irp);
	switch (io->Parameters.DeviceIoControl.IoControlCode) {

	case IO_SEND_MESSAGE: {

		PULONG Output = reinterpret_cast<PULONG>(Irp->AssociatedIrp.SystemBuffer);
		*Output = 123;

		Print("REquested output");

		Status = STATUS_SUCCESS;
		Bytes = sizeof(*Output);
		break;
	}
	default:
	{
		Print("Unknown Code");
		Status = STATUS_UNSUCCESSFUL;
	}
	}

	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = Bytes;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Status;

}



NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegPath) {
	
	RtlInitUnicodeString(&dev, L"\\Device\\RandomDriver");
	RtlInitUnicodeString(&dos, L"\\DosDevices\\RandomDriver");

	IoCreateDevice(DriverObject, 0, &dev, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);
	IoCreateSymbolicLink(&dos, &dev);

	IoSetDeviceInterfaceState(RegPath, TRUE);

	DriverObject->MajorFunction[IRP_MJ_CREATE] = Create;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = Ioctl;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = Close;

	DriverObject->DriverUnload = reinterpret_cast<PDRIVER_UNLOAD>(DriverUnload);


	DeviceObject->Flags |= DO_DIRECT_IO;
	DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
	Print("Loaded Kernel");

	return STATUS_SUCCESS;
}
