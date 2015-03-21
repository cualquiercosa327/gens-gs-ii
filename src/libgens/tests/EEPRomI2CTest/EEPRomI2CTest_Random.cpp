/***************************************************************************
 * libgens/tests: Gens Emulation Library. (Test Suite)                     *
 * EEPRomI2CTest_Random.cpp: EEPRomI2C test: Random Access tests.          *
 *                                                                         *
 * Copyright (c) 2015 by David Korth.                                      *
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

// EEPRomI2C class.
#include "Save/EEPRomI2C.hpp"

// ARRAY_SIZE(x)
#include "macros/common.h"

// Test EEPROM data.
#include "EEPRomI2CTest_data.h"

#include "EEPRomI2CTest.hpp"
namespace LibGens { namespace Tests {

class EEPRomI2CTest_Random : public EEPRomI2CTest
{
	// TODO: Add stuff?
};

/**
 * I2C notes:
 * - START condition: SCL high, SDA high-to-low
 * - STOP condition: SCL high, SDA low-to-high
 *
 * - Other than these two, SDA should only be modified
 *   by the master when SCL is low.
 */

/**
 * X24C01: Test random reading of an empty EEPROM.
 * Reads 64 bytes at random addresses.
 */
TEST_F(EEPRomI2CTest_Random, X24C01_randomReadEmpty)
{
	// Set the EEPROM as X24C01.
	// TODO: Move to SetUp().
	m_eeprom->dbg_setEEPRomType(EEPRomI2C::EPR_X24C01);
	unsigned int eepromSize;
	m_eeprom->dbg_getEEPRomSize(&eepromSize);
	ASSERT_EQ(128U, eepromSize) << "X24C01 should be 128 bytes.";
	unsigned int eepromMask = eepromSize - 1;

	// Make sure we're in a STOP condition at first.
	doStop();

	// EEPROM values.
	uint8_t cmd, response;

	// Read up to 64 random addresses.
	for (int i = 64; i > 0; i--) {
		const unsigned int addr = (rand() & eepromMask);

		// START an I2C transfer.
		// We'll request a READ from address 0x00.
		// Mode 1 word address: [A6 A5 A4 A3 A2 A1 A0 RW]
		m_eeprom->dbg_setSDA(0);	// START
		cmd = (addr << 1) | 1;		// RW=1
		response = sendData(cmd);

		// Check for ACK.
		m_eeprom->dbg_getSDA(&response);
		EXPECT_EQ(0, response) << "NACK received; expected ACK.";

		// Read the data from the EEPROM.
		uint8_t data = recvData();
		EXPECT_EQ(0xFF, data) <<
			"EEPROM address 0x" <<
			std::hex << std::setw(2) << std::setfill('0') << std::uppercase << addr <<
			" should be 0xFF (empty ROM).";

		// STOP the transfer.
		doStop();
	}
}

} }