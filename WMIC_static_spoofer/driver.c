#include "imports.h"

void spoof_drives()
{
	INT count = 0;

	UINT64 address = GetKernelAddress("storport.sys");

	pRegDevInt = address + RegDevIntOFF;

	PDEVICE_OBJECT pObject = NULL;
	PFILE_OBJECT pFileObj = NULL;

	UNICODE_STRING DestinationString;
	RtlInitUnicodeString(&DestinationString, L"\\Device\\RaidPort0");

	NTSTATUS status = IoGetDeviceObjectPointer(&DestinationString, FILE_READ_DATA, &pFileObj, &pObject);

	PDRIVER_OBJECT pDriver = pObject->DriverObject;

	PDEVICE_OBJECT pDevice = pDriver->DeviceObject;

	while (pDevice->NextDevice != NULL)
	{
		if (pDevice->DeviceType == FILE_DEVICE_DISK)
		{
			PHDD_EXTENSION pDeviceHDD = pDevice->DeviceExtension;

			CHAR HDDSPOOFED_TMP[32] = { 0x0 };
			randstring(&HDDSPOOFED_TMP, SERIAL_MAX_LENGTH - 1);

			for (int i = 1; i <= SERIAL_MAX_LENGTH + 1; i = i + 2)
			{
				memcpy(&HDDORG_BUFFER[count][i - 1], &pDeviceHDD->pHDDSerial[i], sizeof(CHAR));
				memcpy(&HDDORG_BUFFER[count][i], &pDeviceHDD->pHDDSerial[i - 1], sizeof(CHAR));

				memcpy(&HDDSPOOF_BUFFER[count][i - 1], &HDDSPOOFED_TMP[i], sizeof(CHAR));
				memcpy(&HDDSPOOF_BUFFER[count][i], &HDDSPOOFED_TMP[i - 1], sizeof(CHAR));
			}

			RtlStringCchPrintfA(pDeviceHDD->pHDDSerial, SERIAL_MAX_LENGTH + 1, "%s", &HDDSPOOFED_TMP);

			pRegDevInt(pDeviceHDD);

			count++;
		}

		pDevice = pDevice->NextDevice;
	}
}

NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT  DriverObject, 
	_In_ PUNICODE_STRING RegistryPath)
{
	spoof_drives();

	return STATUS_SUCCESS;
}

