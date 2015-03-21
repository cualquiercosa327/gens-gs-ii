/***************************************************************************
 * libgens/tests: Gens Emulation Library. (Test Suite)                     *
 * EEPRomI2CTest.cpp: EEPRomI2C test.                                      *
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

// Base class for all EEPRomI2C tests.
// This needed to be split out because INSTANTIATE_TEST_CASE_P()
// instantiates *all* tests in the test fixture, which results
// in repeated tests and failing tests due to incorrect parameters.

#ifndef __LIBGENS_TESTS_EEPROMI2CTEST_HPP__
#define __LIBGENS_TESTS_EEPROMI2CTEST_HPP__

// Google Test
#include "gtest/gtest.h"

namespace LibGens {

class EEPRomI2C;

namespace Tests {

class EEPRomI2CTest : public ::testing::TestWithParam<unsigned int>
{
	protected:
		EEPRomI2CTest()
			: m_eeprom(nullptr) { }
		virtual ~EEPRomI2CTest() { }

		virtual void SetUp(void) override;
		virtual void TearDown(void) override;

	protected:
		EEPRomI2C *m_eeprom;

		/**
		 * Ensure we're in a STOP condition.
		 */
		void doStop(void);

		/**
		 * Send an 8-bit data word to the EEPROM.
		 * @param data 8-bit data word.
		 * @return 0 if ACK; 1 if NACK.
		 */
		uint8_t sendData(uint8_t data);

		/**
		 * Receive an 8-bit data word from the EEPROM.
		 * @return 8-bit data word.
		 */
		uint8_t recvData(void);
};

} }

#endif /* __LIBGENS_TESTS_EEPROMI2CTEST_HPP__ */