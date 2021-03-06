/***************************************************************************
 * gens-qt4: Gens Qt4 UI.                                                  *
 * mcd_rom_db.c: Sega CD Boot ROM database.                                *
 *                                                                         *
 * Copyright (c) 2011-2015 by David Korth                                  *
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

#include "McdRomDB.hpp"

// ARRAY_SIZE()
#include "libgens/macros/common.h"

// C includes. (C++ namespace)
#include <cassert>

namespace GensQt4 {

/** McdRomDBPrivate **/

class McdRomDBPrivate
{
	protected:
		McdRomDBPrivate() { }
		~McdRomDBPrivate() { }

	private:
		Q_DISABLE_COPY(McdRomDBPrivate)

	public:
		/**
		 * Sega CD Boot ROM database entry.
		 */
		struct mcd_rom_db_t {
			uint32_t crc32;			// ROM CRC32. (with original HINT vector)
			uint8_t md5sum[16];		// ROM md5sum. (with original HINT vector)

			McdRomDB::MCD_RegionCode_t region_code;		// Region code.
			McdRomDB::MCD_RegionCode_t region_primary;	// Primary region. (only one bit may be set!)
			McdRomDB::MCD_RomStatus_t rom_status;		// ROM status.

			const char *description;	// ROM description.
			const char *notes;		// ROM notes. (Optional)
		};

		// ROM database.
		static const mcd_rom_db_t McdRomDatabase[];
};

/** ROM database. **/

const McdRomDBPrivate::mcd_rom_db_t McdRomDBPrivate::McdRomDatabase[] = {
	/** Sega CD: Model 1 **/

	// Mega CD (J) Boot ROM v1.00g
	// 1991/11/20 17:00
	{
		0x9BCE40B2,		// CRC32

		// md5sum
		{0xBC, 0x6A, 0xE4, 0xE1, 0xDB, 0x01, 0xA2, 0xF3,
		 0x49, 0xD9, 0xAF, 0x39, 0x2B, 0xF7, 0xE2, 0xBD},

		// Region code and ROM support status.
		(McdRomDB::MCD_RegionCode_t)(
			McdRomDB::MCD_REGION_JAPAN |
			McdRomDB::MCD_REGION_ASIA |
			McdRomDB::MCD_REGION_USA |
			McdRomDB::MCD_REGION_EUROPE),
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::RomStatus_Supported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Mega CD (J) v1.00g"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"This ROM will work on all regions, but will only run Japanese games.")
	},

	// Mega CD (J) Boot ROM v1.00l
	// 1991/12/02 15:20
	{
		0xF18DDE5B,		// CRC32

		// md5sum
		{0x29, 0xAD, 0x9C, 0xE8, 0x48, 0xB4, 0x9D, 0x0F,
		 0x9C, 0xEF, 0xC2, 0x94, 0x13, 0x7F, 0x65, 0x3C},

		// Region code and ROM support status.
		(McdRomDB::MCD_RegionCode_t)(
			McdRomDB::MCD_REGION_JAPAN |
			McdRomDB::MCD_REGION_ASIA |
			McdRomDB::MCD_REGION_USA |
			McdRomDB::MCD_REGION_EUROPE),
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::RomStatus_Supported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Mega CD (J) v1.00l"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"This ROM will work on all regions, but will only run Japanese games.")
	},

	// Mega CD (J) Boot ROM v1.00o
	// 1991/12/06 10:10
	{
		0x2EA250C0,		// CRC32

		// md5sum
		{0xCC, 0x04, 0x91, 0x59, 0xD7, 0xE7, 0x44, 0xC1,
		 0x5E, 0xEE, 0x08, 0x0C, 0x24, 0x12, 0x73, 0xB4},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::RomStatus_Supported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Mega CD (J) v1.00o"),
		""
	},

	// Mega CD (J) Boot ROM v1.00p
	// 1991/12/17 12:00
	{
		0x9D2DA8F2,		// CRC32

		// md5sum
		{0x27, 0x8A, 0x93, 0x97, 0xD1, 0x92, 0x14, 0x9E,
		 0x84, 0xE8, 0x20, 0xAC, 0x62, 0x1A, 0x8E, 0xDD},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::RomStatus_Recommended,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Mega CD (J) v1.00p"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Recommended boot ROM for Mega CD (J).")
	},

	// Mega CD (J) Boot ROM v1.00S
	// 1991/12/28 18:30
	{
		0x79F85384,		// CRC32 (with original HINT vector)

		// md5sum
		{0xA3, 0xDD, 0xCC, 0x84, 0x83, 0xB0, 0x36, 0x81,
		 0x41, 0xAD, 0xFD, 0x99, 0xD9, 0xA1, 0xE4, 0x66},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::RomStatus_Supported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Mega CD (J) v1.00S"),
		""
	},

	// Mega CD (Asia) Boot ROM v1.00S
	// 1991/12/28 20:30
	{
		0x550F30BB,		// CRC32 (with original HINT vector)

		// md5sum
		{0xBD, 0xEB, 0x4C, 0x47, 0xDA, 0x61, 0x39, 0x46,
		 0xD4, 0x22, 0xD9, 0x7D, 0x98, 0xB2, 0x1C, 0xDA},

		// Region code and ROM support status.
		(McdRomDB::MCD_RegionCode_t)(
			McdRomDB::MCD_REGION_ASIA |
			McdRomDB::MCD_REGION_JAPAN |
			McdRomDB::MCD_REGION_EUROPE),
		McdRomDB::MCD_REGION_ASIA,
		McdRomDB::RomStatus_Supported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Mega CD (Asia) v1.00S"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"This ROM will work on all regions except USA, but will only run Japanese games.")
	},

	// Sega CD (U) Boot ROM v1.00
	// 1992/08/04 03:00
	{
		0xE7E3AFE2,		// CRC32 (with original HINT vector)

		// md5sum
		{0x2E, 0xFD, 0x74, 0xE3, 0x23, 0x2F, 0xF2, 0x60,
		 0xE3, 0x71, 0xB9, 0x9F, 0x84, 0x02, 0x4F, 0x7F},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_USA,
		McdRomDB::MCD_REGION_USA,
		McdRomDB::RomStatus_Supported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Sega CD (U) v1.00"),
		""
	},

	// Sega CD (U) Boot ROM v1.10
	// 1992/10/11 18:30
	{
		0xC6D10268,		// CRC32 (with original HINT vector)

		// md5sum
		{0x2E, 0xFD, 0x74, 0xE3, 0x23, 0x2F, 0xF2, 0x60,
		 0xE3, 0x71, 0xB9, 0x9F, 0x84, 0x02, 0x4F, 0x7F},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_USA,
		McdRomDB::MCD_REGION_USA,
		McdRomDB::RomStatus_Recommended,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Sega CD (U) v1.10"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Recommended boot ROM for Sega CD (U).")
	},

	// Mega CD (E) Boot ROM v1.00
	// 1992/10/27 15:15
	{
		0x529AC15A,		// CRC32 (with original HINT vector)

		// md5sum
		{0xE6, 0x6F, 0xA1, 0xDC, 0x58, 0x20, 0xD2, 0x54,
		 0x61, 0x1F, 0xDC, 0xDB, 0xA0, 0x66, 0x23, 0x72},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_EUROPE,
		McdRomDB::MCD_REGION_EUROPE,
		McdRomDB::RomStatus_Recommended,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Mega CD (E) v1.00"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Recommended boot ROM for Mega CD (E).")
	},

	/** Sega CD: Model 2 **/

	// Mega CD 2 (J) Boot ROM v2.00c
	// 1992/12/22 14:00
	{
		0xDD6CC972,		// CRC32 (with original HINT vector)

		// md5sum
		{0x68, 0x3A, 0x8A, 0x9E, 0x27, 0x36, 0x62, 0x56,
		 0x11, 0x72, 0x46, 0x8D, 0xFA, 0x28, 0x58, 0xEB},

		// Region code and ROM support status.
		(McdRomDB::MCD_RegionCode_t)(
			McdRomDB::MCD_REGION_JAPAN |
			McdRomDB::MCD_REGION_ASIA |
			McdRomDB::MCD_REGION_USA |
			McdRomDB::MCD_REGION_EUROPE),
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Mega CD 2 (J) v2.00c"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Mega CD model 2.\n"
			"This ROM will work on all regions, but will only run Japanese games.")
	},

	// Sega CD 2 (U) Boot ROM v2.00 (Bad Dump)
	// 1993/03/14 01:00
	{
		0x340B4BE4,		// CRC32 (with original HINT vector)

		// md5sum
		{0x68, 0x45, 0x57, 0x9B, 0xD2, 0x11, 0xE2, 0x4E,
		 0xAF, 0xE3, 0x13, 0x93, 0x3E, 0x6F, 0x8D, 0x7B},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_USA,
		McdRomDB::MCD_REGION_USA,
		McdRomDB::RomStatus_Broken,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Sega CD 2 (U) v2.00 (Bad Dump)"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"This is a known bad dump, and will not work properly in any Sega CD emulator.")
	},

	// Sega CD 2 (U) Boot ROM v2.00
	// 1993/03/14 01:00
	{
		0x8AF65F58,		// CRC32 (with original HINT vector)

		// md5sum
		{0x31, 0x0A, 0x90, 0x81, 0xD2, 0xED, 0xF2, 0xD3,
		 0x16, 0xAB, 0x38, 0x81, 0x31, 0x36, 0x72, 0x5E},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_USA,
		McdRomDB::MCD_REGION_USA,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Sega CD 2 (U) v2.00"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Sega CD model 2.")
	},

	// Mega CD 2 (E) Boot ROM v2.00
	// 1993/03/30 12:00
	{
		0x0507B590,		// CRC32 (with original HINT vector)

		// md5sum
		{0x9B, 0x56, 0x2E, 0xBF, 0x2D, 0x09, 0x5B, 0xF1,
		 0xDA, 0xBA, 0xDB, 0xC1, 0x88, 0x1f, 0x51, 0x9A},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_EUROPE,
		McdRomDB::MCD_REGION_EUROPE,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Mega CD 2 (E) v2.00"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Mega CD model 2.")
	},

	/** Sega CD: Model 2 (ROM revision W) **/

	// Sega CD 2 (U) Boot ROM v2.00W
	// 1993/06/01 20:00
	{
		0x9F6F6276,		// CRC32 (with original HINT vector)

		// md5sum
		{0x85, 0x4B, 0x91, 0x50, 0x24, 0x0A, 0x19, 0x80,
		 0x70, 0x15, 0x0E, 0x45, 0x66, 0xAE, 0x12, 0x90},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_USA,
		McdRomDB::MCD_REGION_USA,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Sega CD 2 (U) v2.00W"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Sega CD model 2.")
	},

	// Mega CD 2 (E) Boot ROM v2.00W
	// 1993/06/01 20:00
	{
		0x4D5CB8DA,		// CRC32 (with original HINT vector)

		// md5sum
		{0xB1, 0x0C, 0x0A, 0x97, 0xAB, 0xC5, 0x7B, 0x75,
		 0x84, 0x97, 0xD3, 0xFA, 0xE6, 0xAB, 0x35, 0xA4},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_EUROPE,
		McdRomDB::MCD_REGION_EUROPE,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Mega CD 2 (E) v2.00W"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Mega CD model 2.")
	},

	/** Sega CD: Model 2 / Aiwa CSD G1M **/

	// Aiwa CSD-G1M (J) Boot ROM v2.11c
	// 1993/05/28 14:00
	{
		0x8052C7A0,		// CRC32 (with original HINT vector)

		// md5sum
		{0x7E, 0x85, 0xDD, 0x6A, 0xE3, 0xEC, 0xF5, 0x1E,
		 0xAC, 0x71, 0xCD, 0x6C, 0xED, 0x8F, 0xDC, 0x6D},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Aiwa CSD-G1M (J) v2.11c"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Aiwa CSD-G1M.")
	},

	// Sega CD 2 (U) Boot ROM v2.11X
	// 1993/06/21 16:00
	{
		0x2E49D72C,		// CRC32 (with original HINT vector)

		// md5sum
		{0xEC, 0xC8, 0x37, 0xC3, 0x1D, 0x77, 0xB7, 0x74,
		 0xC6, 0xE2, 0x7E, 0x38, 0xF8, 0x28, 0xAA, 0x9A},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_USA,
		McdRomDB::MCD_REGION_USA,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Sega CD 2 (U) v2.11X"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Sega CD model 2.")
	},

	/** Sega MultiMega / CDX **/

	// Sega CDX (U) Boot ROM v2.21X
	// 1993/09/07 19:00
	{
		0xD48C44B5,		// CRC32 (with original HINT vector)

		// md5sum
		{0xBA, 0xCA, 0x1D, 0xF2, 0x71, 0xD7, 0xC1, 0x1F,
		 0xE5, 0x00, 0x87, 0xC0, 0x35, 0x8F, 0x4E, 0xB5},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_USA,
		McdRomDB::MCD_REGION_USA,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Sega CDX (U) v2.21X"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Sega CDX.")
	},

	// Sega Multi-Mega (E) Boot ROM v2.21X
	// 1993/09/16 14:00
	{
		0xAACB851E,		// CRC32 (with original HINT vector)

		// md5sum
		{0xD9, 0x1B, 0x0F, 0xD4, 0x88, 0xD2, 0x96, 0x24,
		 0xC4, 0xEF, 0x2E, 0xE7, 0xB1, 0x9E, 0xFF, 0xF0},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_EUROPE,
		McdRomDB::MCD_REGION_EUROPE,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Sega Multi-Mega (E) v2.21X"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Sega Multi-Mega.")
	},

	/** Pioneer LaserActive **/

	// Pioneer LaserActive (J) Boot ROM v0.98
	// 1993/03/29 17:00
	{
		0x00EEDB3A,		// CRC32 (with original HINT vector)

		// md5sum
		{0xA5, 0xA2, 0xF9, 0xAA, 0xE5, 0x7D, 0x46, 0x4B,
		 0xC6, 0x6B, 0x80, 0xEE, 0x79, 0xC3, 0xDA, 0x6E},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Pioneer LaserActive (J) v0.98"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Pioneer LaserActive.")
	},

	// Pioneer LaserActive (U) Boot ROM v0.98
	// 1993/03/29 17:00
	{
		0x3B10CF41,		// CRC32 (with original HINT vector)

		// md5sum
		{0x69, 0x1C, 0x3F, 0xD3, 0x68, 0x21, 0x12, 0x80,
		 0xD2, 0x68, 0x64, 0x5C, 0x0E, 0xFD, 0x2E, 0xFF},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_USA,
		McdRomDB::MCD_REGION_USA,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Pioneer LaserActive (U) v0.98"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Pioneer LaserActive.")
	},

	// Pioneer LaserActive (U) Boot ROM v1.04
	// 1993/09/22 17:00
	{
		0x50CD3D23,		// CRC32 (with original HINT vector)

		// md5sum
		{0x0E, 0x73, 0x93, 0xCD, 0x09, 0x51, 0xD6, 0xDD,
		 0xE8, 0x18, 0xFC, 0xD4, 0xCD, 0x81, 0x94, 0x66},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_USA,
		McdRomDB::MCD_REGION_USA,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Pioneer LaserActive (U) v1.04"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Pioneer LaserActive.")
	},

	// Pioneer LaserActive (J) Boot ROM v1.05
	// 1993/10/07 11:00
	{
		0x474AAA44,		// CRC32 (with original HINT vector)

		// md5sum
		{0x92, 0x7D, 0xF7, 0x73, 0xF9, 0xA1, 0xC7, 0xDE,
		 0x94, 0xA7, 0x24, 0x20, 0xB2, 0xD5, 0x38, 0xDD},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Pioneer LaserActive (J) v1.05"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Pioneer LaserActive.")
	},

	/** JVC Wondermega / X'Eye **/

	// Sega Wondermega (J) Boot ROM v1.00W
	// 1992/02/06 01:30
	{
		0xD21FE71D,		// CRC32 (with original HINT vector)

		// md5sum
		{0x73, 0x2B, 0x60, 0x96, 0x02, 0x26, 0xCA, 0xF3,
		 0x3D, 0xF6, 0xB4, 0x87, 0xF3, 0x76, 0xDE, 0x69},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Sega Wondermega (J) v1.00W"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Sega Wondermega.")
	},

	// Sega Wondermega M2 (J) Boot ROM v2.00
	// 1993/05/27 13:20
	{
		0x2B19972F,		// CRC32 (with original HINT vector)

		// md5sum
		{0xD3, 0x2F, 0x50, 0xFB, 0x7F, 0x43, 0x55, 0xC4,
		 0x5D, 0x5D, 0xB3, 0x06, 0xA0, 0xD3, 0x8F, 0xA4},

		// Region code and ROM support status.
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::MCD_REGION_JAPAN,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Sega Wondermega M2 (J) v2.00"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the Sega Wondermega M2.")
	},

	// JVC X'Eye (U) Boot ROM v2.00
	// 1993/12/27 16:30
	{
		0x290F8E33,		// CRC32 (with original HINT vector)

		// md5sum
		{0x82, 0xCE, 0x23, 0x63, 0xF7, 0xDD, 0xC6, 0x20,
		 0xB9, 0xC2, 0x55, 0xD3, 0x58, 0x33, 0x11, 0x8F},

		// Region code and ROM support status.
		(McdRomDB::MCD_RegionCode_t)(
			McdRomDB::MCD_REGION_JAPAN |
			McdRomDB::MCD_REGION_ASIA |
			McdRomDB::MCD_REGION_USA |
			McdRomDB::MCD_REGION_EUROPE),
		McdRomDB::MCD_REGION_USA,
		McdRomDB::RomStatus_Unsupported,

		// Description and notes.
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"JVC X'Eye (U) v2.00"),
		QT_TRANSLATE_NOOP("GensQt4::McdRomDB",
			"Gens/GS II does not support the hardware changes in the JVC X'Eye.\n"
			"This ROM will work on all regions, but will only run USA games.")
	},

	// End of database.
	{0, {0},
	 (McdRomDB::MCD_RegionCode_t)0,
	 (McdRomDB::MCD_RegionCode_t)0,
	 (McdRomDB::MCD_RomStatus_t)0,
	 NULL, NULL}
};

/** McdBootRomInfo **/

McdRomDB::McdBootRomInfo::McdBootRomInfo()
	: m_romId(-1)
{ }

McdRomDB::McdBootRomInfo::McdBootRomInfo(int romId)
	: m_romId(romId)
{
	assert(romId >= -1 && romId < (ARRAY_SIZE(McdRomDBPrivate::McdRomDatabase)-1));
}

/**
 * Get this Boot ROM's description.
 * @return Boot ROM description.
 */
QString McdRomDB::McdBootRomInfo::description(void) const
{
	if (m_romId < 0)
		return QString();
	return McdRomDB::tr(McdRomDBPrivate::McdRomDatabase[m_romId].description);
}

/**
 * Get this Boot ROM's notes.
 * @return Boot ROM notes.
 */
QString McdRomDB::McdBootRomInfo::notes(void) const
{
	if (m_romId < 0)
		return QString();
	return McdRomDB::tr(McdRomDBPrivate::McdRomDatabase[m_romId].notes);
}

/**
 * Get this Boot ROM's region code.
 * This includes all hardware regions accepted by this Boot ROM.
 * @return Boot ROM region code.
 */
int McdRomDB::McdBootRomInfo::region(void) const
{
	if (m_romId < 0)
		return 0;
	return McdRomDBPrivate::McdRomDatabase[m_romId].region_code;
}

/**
 * Get this Boot ROM's primary region code.
 * @return Boot ROM primary region code
 */
int McdRomDB::McdBootRomInfo::primaryRegion(void) const
{
	if (m_romId < 0)
		return 0;
	return McdRomDBPrivate::McdRomDatabase[m_romId].region_primary;
}

/**
 * Get this Boot ROM's support status.
 * @return ROM support status.
 */
McdRomDB::MCD_RomStatus_t McdRomDB::McdBootRomInfo::supportStatus(void) const
{
	if (m_romId < 0)
		return McdRomDB::RomStatus_Broken;
	return McdRomDBPrivate::McdRomDatabase[m_romId].rom_status;
}

/** McdRomDB **/

/**
 * ROM fixup data.
 * mcd_rom_InitSP: Initial SP. (0xFFFFFD00)
 * mcd_rom_InitHINT: Initial HINT vector. (0xFFFFFD0C)
 */
const uint8_t McdRomDB::InitSP[4] = {0xFF, 0xFF, 0xFD, 0x00};
const uint8_t McdRomDB::InitHINT[4] = {0xFF, 0xFF, 0xFD, 0x0C};

McdRomDB::McdRomDB()
{ }

McdRomDB::~McdRomDB()
{ }

/**
 * Look up a Sega CD Boot ROM using its CRC32.
 * @param rom_crc32 ROM CRC32.
 * @return Boot ROM information.
 */
McdRomDB::McdBootRomInfo McdRomDB::findByCrc32(uint32_t rom_crc32)
{
	const McdRomDBPrivate::mcd_rom_db_t *db_entry = &McdRomDBPrivate::McdRomDatabase[0];

	for (int i = 0; db_entry->description != NULL; db_entry++, i++) {
		if (db_entry->crc32 == rom_crc32) {
			// CRC32 match!
			return McdBootRomInfo(i);
		}
	}

	// CRC32 not matched.
	return McdBootRomInfo(-1);
}

/**
 * Get a string describing a primary region code.
 * @param region_code Primary region code.
 * @return Region code string, or NULL if the region code is invalid.
 */
QString McdRomDB::regionCodeString(int region_code)
{
	switch (region_code) {
		case MCD_REGION_JAPAN:		return tr("Japan");
		case MCD_REGION_ASIA:		return tr("Asia");
		case MCD_REGION_USA:	 	return tr("USA");
		case MCD_REGION_EUROPE: 	return tr("Europe");

		// Combinations allowed due to some boot ROMs
		// that inadvertently accept them.
		// TODO: More combinations?
		case MCD_REGION_JAPAN | MCD_REGION_ASIA:
			return tr("Japan or Asia");

		case MCD_REGION_JAPAN | MCD_REGION_ASIA | MCD_REGION_EUROPE:
			return tr("Japan, Asia, or Europe");

		case MCD_REGION_JAPAN | MCD_REGION_ASIA | MCD_REGION_USA | MCD_REGION_EUROPE:
			return tr("Any Region");

		default:
			break;
	}

	return QString();
}

}
