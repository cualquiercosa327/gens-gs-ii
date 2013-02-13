/***************************************************************************
 * libgenscd: Gens/GS II CD-ROM Handler Library.                           *
 * ScsiSpti.cpp: SPTI (Windows NT) SCSI device handler class.              *
 *                                                                         *
 * Copyright (c) 2013 by David Korth.                                      *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation; either version 2 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.           *
 ***************************************************************************/

#include "ScsiSpti.hpp"

// C includes. (C++ namespace)
#include <cstring>
#include <cstdio>
#include <cctype>

// C++ includes.
#include <string>
using std::string;

// SCSI and storage IOCTLs.
#include <winioctl.h>
#include <ntddscsi.h>
#include <ntddstor.h>
#include <winerror.h>

// SPTI/SCSI headers.
#include <devioctl.h>

namespace LibGensCD
{

ScsiSpti::ScsiSpti(const string& filename)
	: ScsiBase(filename)
	, m_hDevice(INVALID_HANDLE_VALUE)
{
	// Attempt to open the CD-ROM device.
	// Assume that filename is in the format "C:" for now.
	if (filename.size() < 1 || !isalpha(filename.at(0))) {
		// Invalid filename.
		return;
	}

	// Convert the drive letter to uppercase.
	char drive_letter = toupper(filename.at(0));

	// Create the pathname.
	const char spti_pathname[7] =
		{'\\', '\\', '.', '\\', drive_letter, ':', 0};

	// Open the specified drive using SPTI.
	// TODO: Error handling.
	m_hDevice = CreateFileA(spti_pathname,				// lpFileName
				GENERIC_READ | GENERIC_WRITE,		// dwDesiredAccess
				FILE_SHARE_READ | FILE_SHARE_WRITE,	// dwShareMode
				nullptr,				// lpSecurityAttributes
				OPEN_EXISTING,				// dwCreationDisposition,
				FILE_ATTRIBUTE_NORMAL,			// dwFlagsAndAttributes
				nullptr);				// hTemplateFile

}

ScsiSpti::~ScsiSpti()
{
	// Make sure the CD-ROM device file is closed.
	close();
}

/**
 * Check if the CD-ROM device file is open.
 * @return True if open; false if not.
 */
bool ScsiSpti::isOpen(void) const
{
	return !!m_hDevice;
}

/**
 * Close the CD-ROM device file if it's open.
 */
void ScsiSpti::close(void)
{
	if (m_hDevice != INVALID_HANDLE_VALUE) {
		CloseHandle(m_hDevice);
		m_hDevice = INVALID_HANDLE_VALUE;
	}
}

/**
 * Check if a disc is present.
 * @return True if a disc is present; false if not.
 */
bool ScsiSpti::isDiscPresent(void)
{
	/**
	 * READ CAPACITY seems to return invalid values
	 * on Wine if no disc is present.
	 *
	 * Instead, use the Win32 IOCTL_STORAGE_CHECK_VERIFY.
	 *
	 * References:
	 * - http://stackoverflow.com/questions/3158054/how-to-detect-if-media-is-inserted-into-a-removable-drive-card-reader
	 * - http://msdn.microsoft.com/en-us/library/windows/desktop/aa363404%28v=vs.85%29.aspx
	 * - http://msdn.microsoft.com/en-us/library/windows/hardware/ff560535%28v=vs.85%29.aspx
	 */

	if (!isOpen())
		return false;

	DWORD returned;
	int ret = DeviceIoControl(
			m_hDevice, IOCTL_STORAGE_CHECK_VERIFY,
			nullptr, 0, nullptr, 0,
			&returned, nullptr);

	// IOCTL_STORAGE_CHECK_VERIFY succeeds if the medium is accessible,
	// and fails if the medium is not accessible.
	return !!ret;
}

/**
 * Check if the disc has changed since the last access.
 * @return True if the disc has changed; false if not.
 */
bool ScsiSpti::hasDiscChanged(void)
{
	// TODO: Figure out a better way to implement this.
	// MMC's GET_EVENT_STATUS_NOTIFICATION command doesn't seem to work properly,
	// and Win32's WM_DEVICECHANGE requires a window in order to receive events.

	// Alternatively, just have the main program force a refresh whenever
	// WM_DEVICECHANGE is received.
	return false;
}

/**
 * Send a SCSI command descriptor block to the drive.
 * @param cdb		[in] SCSI command descriptor block.
 * @param cdb_len	[in] Length of cdb.
 * @param out		[out] Output buffer, or nullptr if no data is requested.
 * @param out_len	[out] Length of out.
 * @param mode		[in] Data direction mode. (IN == receive from device; OUT == send to device)
 * @return 0 on success, positive for SCSI sense key, negative for OS error.
 */
int ScsiSpti::scsi_send_cdb(const void *cdb, uint8_t cdb_len,
				void *out, size_t out_len,
				scsi_data_mode mode)
{
	
	if (!isOpen())
		return -1;	// TODO: Proper SCSI error code.

	// Based on http://www.codeproject.com/KB/system/mydvdregion.aspx
	DWORD returned;

	// SCSI_PASS_THROUGH_DIRECT struct with extra space for sense data.
	struct {
		SCSI_PASS_THROUGH_DIRECT p;
		struct {
			SCSI_RESP_REQUEST_SENSE s;
			uint8_t b[78];	// Additional sense data. (TODO: Best size?)
		} sense;
	} srb;
	memset(&srb, 0x00, sizeof(srb));

	// Copy the CDB to the SCSI_PASS_THROUGH structure.
	memcpy(srb.p.Cdb, cdb, cdb_len);

	// Convert scsi_data_mode to Win32 DataIn.
	uint8_t DataIn;
	switch (mode) {
		case SCSI_DATA_NONE:
		default:
			DataIn = SCSI_IOCTL_DATA_UNSPECIFIED;
			break;
		case SCSI_DATA_IN:
			DataIn = SCSI_IOCTL_DATA_IN;
			break;
		case SCSI_DATA_OUT:
			DataIn = SCSI_IOCTL_DATA_OUT;
			break;
	}

	// Initialize the other SCSI command variables.
	srb.p.DataBuffer = out;
	srb.p.DataTransferLength = out_len;
	srb.p.DataIn = DataIn;
	srb.p.CdbLength = cdb_len;
	srb.p.Length = sizeof(srb.p);
	srb.p.SenseInfoLength = sizeof(srb.sense);
	srb.p.SenseInfoOffset = sizeof(srb.p);
	srb.p.TimeOutValue = 5; // 5-second timeout.

	int ret = DeviceIoControl(
			m_hDevice, IOCTL_SCSI_PASS_THROUGH_DIRECT,
			(LPVOID)&srb.p, sizeof(srb.p),
			(LPVOID)&srb, sizeof(srb),
			&returned, nullptr);

	if (ret == 0) {
		// DeviceIoControl failed.
		ret = -GetLastError();
	} else {
		// Check if the SCSI command failed.
		switch (srb.sense.s.ErrorCode) {
			case SCSI_ERR_REQUEST_SENSE_CURRENT:
			case SCSI_ERR_REQUEST_SENSE_DEFERRED:
				// Error. Return the sense key.
				ret = (srb.sense.s.SenseKey << 16) |
				      (srb.sense.s.AddSenseCode << 8) |
				      (srb.sense.s.AddSenseQual);
				break;

			case SCSI_ERR_REQUEST_SENSE_CURRENT_DESC:
			case SCSI_ERR_REQUEST_SENSE_DEFERRED_DESC:
				// Error, but using descriptor format.
				// Return a generic error.
				ret = -ERROR_INVALID_PARAMETER;	// The parameter is incorrect.
				break;

			default:
				// No error.
				ret = 0;
				break;
		}
	}

	// Return the error code.
	return ret;
}

}
