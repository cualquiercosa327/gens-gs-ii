/***************************************************************************
 * libgens: Gens Emulation Library.                                        *
 * M68K.hpp: Main 68000 CPU wrapper class.                                 *
 *                                                                         *
 * Copyright (c) 1999-2002 by Stéphane Dallongeville.                      *
 * Copyright (c) 2003-2004 by Stéphane Akhoun.                             *
 * Copyright (c) 2008-2015 by David Korth.                                 *
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

#include <libgens/config.libgens.h>

#include "M68K.hpp"
#include "M68K_Mem.hpp"

#include "macros/common.h"
#include "Cartridge/RomCartridgeMD.hpp"

// C includes. (C++ namespace)
#include <cstring>

namespace LibGens {

c68k_struc M68K::ms_Context;

static u32 Gens_M68K_RB(u32 address)
{
	/** WORKAROUND for Starscream not properly saving ecx/edx. **/
	return LibGens::M68K_Mem::M68K_RB(address);
}
static u32 Gens_M68K_RW(u32 address)
{
	/** WORKAROUND for Starscream not properly saving ecx/edx. **/
	return LibGens::M68K_Mem::M68K_RW(address);
}
static void Gens_M68K_WB(u32 address, u32 data)
{
	/** WORKAROUND for Starscream not properly saving ecx/edx. **/
	LibGens::M68K_Mem::M68K_WB(address, data);
}
static void Gens_M68K_WW(uint32_t address, u32 data)
{
	/** WORKAROUND for Starscream not properly saving ecx/edx. **/
	LibGens::M68K_Mem::M68K_WW(address, data);
}

// Last system ID.
M68K::SysID M68K::ms_LastSysID = SYSID_NONE;

/**
 * Reset handler.
 * TODO: What does this function do?
 */
void M68K::M68K_Reset_Handler(void)
{
	//Init_Memory_M68K(GENESIS);
}

/**
 * Initialize the M68K CPU emulator.
 */
void M68K::Init(void)
{
	// Clear the 68000 context.
	memset(&ms_Context, 0x00, sizeof(ms_Context));

	// Initialize the memory handlers.
	C68k_Init(&ms_Context, NULL);
	
	C68k_Set_ReadB(&ms_Context, Gens_M68K_RB);
	C68k_Set_ReadW(&ms_Context, Gens_M68K_RW);
	C68k_Set_WriteB(&ms_Context, Gens_M68K_WB);
	C68k_Set_WriteW(&ms_Context, Gens_M68K_WW);

	ms_Context.Reset_CallBack = M68K_Reset_Handler;
}

/**
 * Shut down the M68K CPU emulator.
 */
void M68K::End(void)
{
	// TODO
}

static uint8_t dummy_ram[32];

/**
 * Initialize a specific system for the M68K CPU emulator.
 * @param system System ID.
 */
void M68K::InitSys(SysID system)
{
	// TODO: This is not 64-bit clean!
	ms_LastSysID = system;

	// Clear M68K RAM.
	memset(Ram_68k.u8, 0x00, sizeof(Ram_68k.u8));

	// Initialize the M68K memory handlers.
	M68K_Mem::InitSys(system);

	// Initialize M68K RAM handlers.
	for (int i = 0; i < 32; i++) {
		uint32_t ram_addr = (0xE00000 | (i << 16));
		SetFetch(ram_addr, (ram_addr | 0xFFFF), Ram_68k.u8);
	}

	// Update the system-specific banking setup.
	UpdateSysBanking();

	// Reset the M68K CPU.
	C68k_Reset(&ms_Context);
}

/**
 * Shut down M68K emulation.
 */
void M68K::EndSys(void)
{
	for (int i = 0; i < ((1 << 24) / 32); i++) {
		SetFetch(i * 32, ((i * 32) | 0x1F), dummy_ram);
	}
}

/**
 * Update system-specific memory banking.
 * Uses the last system initialized via InitSys().
 */
void M68K::UpdateSysBanking(void)
{
	// Start at M68K_Fetch[0x20].
	switch (ms_LastSysID) {
		case SYSID_MD:
		case SYSID_PICO:
			// Sega Genesis / Mega Drive.
			// Also Pico. (This only adds cartridge ROM.)
			M68K_Mem::UpdateSysBanking(10);
			break;

		case SYSID_MCD:
			// Sega CD.
			// TODO
#if 0
			Bank_M68K = 0;
			
			// Set up Word RAM. (Entry 33)
			MS68K_Set_Word_Ram();
			
			// Set up Program RAM. (Entry 34)
			M68K_Fetch[34].lowaddr = 0x020000;
			M68K_Fetch[34].highaddr = 0x03FFFF;
			M68K_Set_Prg_Ram();
			
			// Terminate the list.
			M68K_Fetch[35].lowaddr = -1;
			M68K_Fetch[35].highaddr = -1;
			M68K_Fetch[35].offset = (unsigned int)NULL;
#endif
			break;
		
		case SYSID_32X:
			// Sega 32X.
			// TODO
#if 0
			Bank_SH2 = 0;
			
			// Nothing else is required. Terminate the list.
			M68K_Fetch[33].lowaddr  = -1;
			M68K_Fetch[33].highaddr = -1;
			M68K_Fetch[33].offset   = (unsigned int)NULL;
#endif
			break;
		
		default:
			break;
	}

	// Set the terminator.
	/*M68K_Fetch[cur_fetch].lowaddr = -1;
	M68K_Fetch[cur_fetch].highaddr = -1;
	M68K_Fetch[cur_fetch].offset = 0;*/
	
	// FIXME: Make sure Starscream's internal program counter
	// is updated to reflect the updated M68K_Fetch[].
}

/** ZOMG savestate functions. **/

/**
 * ZomgSaveReg(): Save the M68K registers.
 * @param state Zomg_M68KRegSave_t struct to save to.
 */
void M68K::ZomgSaveReg(Zomg_M68KRegSave_t *state)
{
	// NOTE: Byteswapping is done in libzomg.
	
#if 0	// TODO
	struct S68000CONTEXT m68k_context;
	main68k_GetContext(&m68k_context);
	
	// Save the main registers.
	for (int i = 0; i < 8; i++)
		state->dreg[i] = m68k_context.dreg[i];
	for (int i = 0; i < 7; i++)
		state->areg[i] = m68k_context.areg[i];
	
	// Save the stack pointers.
	if (m68k_context.sr & 0x2000) {
		// Supervisor mode.
		// m68k_context.areg[7] == ssp
		// m68k_context.asp     == usp
		state->ssp = m68k_context.areg[7];
		state->usp = m68k_context.asp;
	} else {
		// User mode.
		// m68k_context.areg[7] == usp
		// m68k_context.asp     == ssp
		state->ssp = m68k_context.asp;
		state->usp = m68k_context.areg[7];
	}

	// Other registers.
	state->pc = m68k_context.pc;
	state->sr = m68k_context.sr;

	// Reserved fields.
	state->reserved1 = 0;
	state->reserved2 = 0;
#else
	memset(state, 0x00, sizeof(*state));
#endif /* GENS_ENABLE_EMULATION */
}


/**
 * Restore the M68K registers.
 * @param state Zomg_M68KRegSave_t struct to restore from.
 */
void M68K::ZomgRestoreReg(const Zomg_M68KRegSave_t *state)
{
#if 0	// TODO
	main68k_GetContext(&ms_Context);

	// Load the main registers.
	for (int i = 0; i < 8; i++)
		ms_Context.dreg[i] = state->dreg[i];
	for (int i = 0; i < 7; i++)
		ms_Context.areg[i] = state->areg[i];

	// Load the stack pointers.
	if (ms_Context.sr & 0x2000) {
		// Supervisor mode.
		// ms_Context.areg[7] == ssp
		// ms_Context.asp     == usp
		ms_Context.areg[7] = state->ssp;
		ms_Context.asp     = state->usp;
	} else {
		// User mode.
		// ms_Context.areg[7] == usp
		// ms_Context.asp     == ssp
		ms_Context.asp     = state->ssp;
		ms_Context.areg[7] = state->usp;
	}

	// Other registers.
	ms_Context.pc = state->pc;
	ms_Context.sr = state->sr;

	main68k_SetContext(&ms_Context);
#endif /* GENS_ENABLE_EMULATION */
}

}
