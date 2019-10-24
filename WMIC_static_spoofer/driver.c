#include "imports.h"

void spoof_drives()
{
	INT count = 0;

	size_t storportSize = 0;
	UINT64 storportBase = get_kernel_address("storport.sys", storportSize);

	RTL_OSVERSIONINFOW osVersion = { 0 };
	osVersion.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
	RtlGetVersion(&osVersion);

	log("Windows Build Number: %d\n", osVersion.dwBuildNumber);
	log("Windows Major Version: %d\n", osVersion.dwMajorVersion);
	log("Windows Minor Version: %d\n", osVersion.dwMinorVersion);

	PDEVICE_OBJECT pObject = NULL;
	PFILE_OBJECT pFileObj = NULL;

	UNICODE_STRING DestinationString;
	RtlInitUnicodeString(&DestinationString, L"\\Device\\RaidPort0");

	NTSTATUS status = IoGetDeviceObjectPointer(&DestinationString, FILE_READ_DATA, &pFileObj, &pObject);

	PDRIVER_OBJECT pDriver = pObject->DriverObject;

	PDEVICE_OBJECT pDevice = pDriver->DeviceObject;

	if (osVersion.dwBuildNumber >= 18362) {
		RaidUnitRegisterInterfaces1903 pRegDevInt = find_pattern<RaidUnitRegisterInterfaces1903>((void*)storportBase, storportSize, "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x50", "xxxx?xxxxxxx");

		while (pDevice->NextDevice != NULL)
		{
			if (pDevice->DeviceType == FILE_DEVICE_DISK)
			{
				PHDD_EXTENSION1903 pDeviceHDD = (PHDD_EXTENSION1903)pDevice->DeviceExtension;

				CHAR HDDSPOOFED_TMP[32] = { 0x0 };
				randstring(HDDSPOOFED_TMP, SERIAL_MAX_LENGTH - 1);

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
	else if (osVersion.dwBuildNumber >= 17763) {
		RaidUnitRegisterInterfaces1809 pRegDevInt = find_pattern<RaidUnitRegisterInterfaces1809>((void*)storportBase, storportSize, "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x50", "xxxx?xxxxxxx");

		while (pDevice->NextDevice != NULL)
		{
			if (pDevice->DeviceType == FILE_DEVICE_DISK)
			{
				PHDD_EXTENSION1809 pDeviceHDD = (PHDD_EXTENSION1809)pDevice->DeviceExtension;

				CHAR HDDSPOOFED_TMP[32] = { 0x0 };
				randstring(HDDSPOOFED_TMP, SERIAL_MAX_LENGTH - 1);

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
	else if (osVersion.dwBuildNumber >= 17134) {
		RaidUnitRegisterInterfaces1803 pRegDevInt = find_pattern<RaidUnitRegisterInterfaces1803>((void*)storportBase, storportSize, "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x50", "xxxx?xxxxxxx");

		while (pDevice->NextDevice != NULL)
		{
			if (pDevice->DeviceType == FILE_DEVICE_DISK)
			{
				PHDD_EXTENSION1803 pDeviceHDD = (PHDD_EXTENSION1803)pDevice->DeviceExtension;

				CHAR HDDSPOOFED_TMP[32] = { 0x0 };
				randstring(HDDSPOOFED_TMP, SERIAL_MAX_LENGTH - 1);

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
}

NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT  DriverObject,
	_In_ PUNICODE_STRING RegistryPath)
{
	spoof_drives();

	return STATUS_SUCCESS;
}

