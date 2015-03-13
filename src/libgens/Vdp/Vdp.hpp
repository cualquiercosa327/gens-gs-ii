/***************************************************************************
 * libgens: Gens Emulation Library.                                        *
 * Vdp.hpp: VDP emulation class.                                           *
 *                                                                         *
 * Copyright (c) 1999-2002 by Stéphane Dallongeville.                      *
 * Copyright (c) 2003-2004 by Stéphane Akhoun.                             *
 * Copyright (c) 2008-2011 by David Korth.                                 *
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

#ifndef __LIBGENS_MD_VDP_HPP__
#define __LIBGENS_MD_VDP_HPP__

#include <stdint.h>

// VdpRend includes.
#include "../Util/MdFb.hpp"

// Needed for FORCE_INLINE.
#include "../macros/common.h"

// VDP types.
#include "VdpTypes.hpp"
#include "VdpReg.hpp"
#include "VdpPalette.hpp"
#include "VdpStatus.hpp"
#include "VdpStructs.hpp"

namespace LibZomg {
	class Zomg;
}

namespace LibGens
{

class VdpRend_Err_Private;

class Vdp
{
	public:
		Vdp(MdFb *fb = nullptr);
		~Vdp();

	private:
		// Q_DISABLE_COPY() equivalent.
		// TODO: Add LibGens-specific version of Q_DISABLE_COPY().
		Vdp(const Vdp &);
		Vdp &operator=(const Vdp &);

	public:
		/**
		 * Reset the VDP.
		 */
		void reset(void);

		// PAL/NTSC.
	public:
		bool isPal(void);
		bool isNtsc(void);
		void setPal(void);
		void setNtsc(void);
		void setVideoMode(bool videoMode);

		// Update flags.
	public:
		void MarkVRamDirty(void);
	private:
		// Update flags.
		union {
			uint8_t flags;
			struct {
				bool VRam	:1;	// VRam was modified. (Implies VRam_Spr.)
				bool VRam_Spr	:1;	// Sprite Attribute Table was modified.
			};
		} m_updateFlags;

	public:
		/**
		 * VDP emulation options.
		 * TODO: Make private, and add accessor/mutator?
		 */
		VdpTypes::VdpEmuOptions_t options;

	public:
		/**
		 * Save the VDP state. (MD mode)
		 * @param zomg ZOMG savestate object to save to.
		 */
		void zomgSaveMD(LibZomg::Zomg *zomg) const;

		/**
		 * Restore the VDP state. (MD mode)
		 * @param zomg ZOMG savestate object to restore from.
		 */
		void zomgRestoreMD(LibZomg::Zomg *zomg);

	/*!**************************************************************
	 * VdpIo: I/O registers and variables.                          *
	 ****************************************************************/

	public:
		// VDP registers.
		VdpTypes::VdpReg_t VDP_Reg;

	private:
		// DMA helper functions.

		/**
		 * Get the DMA length from the VDP Mode 5 registers.
		 * @return DMA length.
		 */
		inline uint16_t DMA_Length(void);

		/**
		 * Set the DMA length in the VDP Mode 5 registers.
		 * @param length DMA length.
		 */
		inline void set_DMA_Length(uint16_t length);

		/**
		 * Get the DMA source address from the VDP Mode 5 registers.
		 * @return DMA source address, divided by 2.
		 */
		inline uint32_t DMA_Src_Adr(void);

		/**
		 * Increment the DMA source address from the VDP Mode 5 registers.
		 * Implements 128 KB wrapping, so only use this from DMA functions!
		 * @param length Number of words to increment the source address by.
		 */
		inline void inc_DMA_Src_Adr(uint16_t length);

		// DMAT variables.
		// TODO: Mark DMAT_Length private.
	public:	
		int DMAT_Length;
	private:
		/**
		 * DMA transfer type:
		 * - 0: External ROM/RAM to VRAM
		 * - 1: External ROM/RAM to CRAM/VSRAM
		 * - 2: DMA FILL
		 * - 3: DMA COPY
		 */
		enum DMAT_Type_t {
			DMAT_MEM_TO_VRAM = 0,
			DMAT_MEM_TO_CRAM_VSRAM,
			DMAT_FILL,
			DMAT_COPY,
		};
		DMAT_Type_t DMAT_Type;

	private:
		/** VDP address functions: Get Pointers. **/
		uint16_t *ScrA_Addr_Ptr16(uint16_t offset);
		uint16_t *ScrB_Addr_Ptr16(uint16_t offset);
		uint16_t *Win_Addr_Ptr16(uint16_t offset);
		VdpStructs::SprEntry_m5 *Spr_Addr_PtrM5(uint16_t offset);
		uint16_t *H_Scroll_Addr_Ptr16(uint16_t offset);

		/** VDP address functions: Get Values. **/
		uint16_t ScrA_Addr_u16(uint16_t offset);
		uint16_t ScrB_Addr_u16(uint16_t offset);
		uint16_t Win_Addr_u16(uint16_t offset);
		uint16_t Spr_Addr_u16(uint16_t offset);
		uint16_t H_Scroll_Addr_u16(uint16_t offset);

		/**
		 * VDP address pointers.
		 * These are relative to VRam[] and are based on register values.
		 */
		uint16_t ScrA_Addr;
		uint16_t ScrB_Addr;
		uint16_t Win_Addr;
		uint16_t Spr_Addr;
		uint16_t H_Scroll_Addr;

		/**
		 * Window row shift.
		 * H40: 6. (64x32 window)
		 * H32: 5. (32x32 window)
		 */
		unsigned int H_Win_Shift;

		// VDP convenience values: Scroll.
		unsigned int V_Scroll_MMask;
		unsigned int H_Scroll_Mask;

		unsigned int H_Scroll_CMul;
		unsigned int H_Scroll_CMask;
		unsigned int V_Scroll_CMask;

		// TODO: Eliminate these.
		int Win_X_Pos;
		int Win_Y_Pos;

		// Interlaced mode.
		VdpTypes::Interlaced_t Interlaced;

		// Sprite dot overflow.
		// If set, the previous line had a sprite dot overflow.
		// This is needed to properly implement Sprite Masking in S1.
		int SpriteDotOverflow;

	private:
		/**
		 * VDP_Mode: Current VDP mode.
		 * TODO: Mark as private after integrating VdpRend_Err within the Vdp class.
		 */
		#define VDP_MODE_M1	(1 << 0)
		#define VDP_MODE_M2	(1 << 1)
		#define VDP_MODE_M3	(1 << 2)
		#define VDP_MODE_M4	(1 << 3)
		#define VDP_MODE_M5	(1 << 4)
		unsigned int VDP_Mode;

	public:
		// VRam, VSRam.
		// NOTE: VdpSpriteMaskingTest requires access to these variables.
		VdpTypes::VRam_t VRam;
		VdpTypes::VSRam_t VSRam;

	public:
		int HInt_Counter;	// Horizontal Interrupt Counter.
		int VDP_Int;		// VDP interrupt state.
		VdpStatus Reg_Status;	// VDP status register.

	public:
		// VDP line counters.
		// NOTE: Gens/GS currently uses 312 lines for PAL. It should use 313!
		VdpTypes::VdpLines_t VDP_Lines;

		// System status.
		// TODO: Move this to a more relevant file.
		union {
			struct {
				unsigned int Genesis	:1;
				unsigned int SegaCD	:1;
				unsigned int _32X	:1;
			};
			unsigned int data;
		} SysStatus;

	private:
		/** VDP tables. **/
		uint8_t H_Counter_Table[512][2];
		static const uint8_t DMA_Timing_Table[4][4];
	
	public:
		/** Interrupt functions. **/
		uint8_t Int_Ack(void);
		void Update_IRQ_Line(void);

		// Lines.
		void updateVdpLines(bool resetCurrent);
		void Check_NTSC_V30_VBlank(void);

		/**
		 * Set the value of a register. (Mode 5 only!)
		 * @param reg_num Register number.
		 * @param val New value for the register.
		 */
		void setReg(int reg_num, uint8_t val);

		uint8_t Read_H_Counter(void);
		uint8_t Read_V_Counter(void);
		uint16_t Read_Status(void);
		uint16_t Read_Data(void);

		/**
		 * Update the DMA state.
		 * @return Number of cycles taken from the 68000 for DMA.
		 */
		unsigned int Update_DMA(void);
		
		void Write_Data_Byte(uint8_t data);
		void Write_Data_Word(uint16_t data);
		void Write_Ctrl(uint16_t data);

	private:
		/**
		 * Internal VDP data write function.
		 * Used by Write_Data_Word() and DMA.
		 * @param data Data word.
		 */
		void vdpDataWrite_int(uint16_t data);

	public:
		/**
		 * GetHPix(): Get the current horizontal resolution.
		 * NOTE: Do NOT use this if a ROM isn't loaded!
		 * @return Horizontal resolution, in pixels.
		 */
		int GetHPix(void) const;
		
		/**
		 * GetHPixBegin(): Get the first horizontal pixel number.
		 * NOTE: Do NOT use this if a ROM isn't loaded!
		 * @return First horizontal pixel number.
		 */
		int GetHPixBegin(void) const;
		
		/**
		 * GetHCells(): Get the current horizontal resolution, in cells.
		 * @return Horizontal resolution, in cells.
		 */
		int GetHCells(void) const;
		
		/**
		 * GetVPix(): Get the current vertical resolution.
		 * NOTE: Do NOT use this if a ROM isn't loaded!
		 * @return Vertical resolution, in pixels.
		 */
		int GetVPix(void) const;
	
	private:
		/**
		 * Vdp::Update_Mode(): Update VDP_Mode.
		 */
		void Update_Mode(void);
		
		// VDP convenience values: Horizontal.
		// NOTE: These must be signed for VDP arithmetic to work properly!
		int H_Cell;
		int H_Pix;
		int H_Pix_Begin;

		/**
		 * Determine if the display mode is H40. (320px wide)
		 * @return True if H40; false otherwise.
		 */
		bool isH40(void) const;
		
		/** DMA **/

		void DMA_Fill(uint16_t data);

		enum DMA_Dest_t {
			DMA_DEST_VRAM	= 1,
			DMA_DEST_CRAM	= 2,
			DMA_DEST_VSRAM	= 3,
		};

		enum DMA_Src_t {
			DMA_SRC_ROM			= 0,
			DMA_SRC_M68K_RAM		= 1,
			DMA_SRC_PRG_RAM			= 2,
			DMA_SRC_WORD_RAM_2M		= 3,
			DMA_SRC_WORD_RAM_1M_0		= 5,
			DMA_SRC_WORD_RAM_1M_1		= 6,
			DMA_SRC_WORD_RAM_CELL_1M_0	= 7,
			DMA_SRC_WORD_RAM_CELL_1M_1	= 8,
		};

		// NOTE: This needs to be a macro, since it's used in case statements.
		#define DMA_TYPE(src, dest) (((int)src << 2) | ((int)dest))

		template<DMA_Src_t src_component, DMA_Dest_t dest_component>
		inline void T_DMA_Loop(void);

		/**
		 * VDP control struct.
		 */
		struct {
			/**
			 * ctrl_latch: Control word latch.
			 * 0: Next control word is FIRST word.
			 * 1: Next control word is SECOND word, which triggers an action.
			 */
			uint8_t ctrl_latch;	// Control word latch.

			// VDP internal registers.
			uint8_t code;		// Access code. (CD5-CD0)
			uint16_t address;	// Address counter.

			// DMA values.
			uint8_t DMA_Mode;	// (DMA ADDRESS HIGH & 0xC0) [reg 23]

			void reset(void)
			{
				ctrl_latch = 0;
				code = 0;
				address = 0;
				DMA_Mode = 0;
			}
		} VDP_Ctrl;

		/**
		 * Scroll_Size_t: Convenience enum for dealing with scroll plane sizes.
		 */
		enum Scroll_Size_t {
			V32_H32 = 0, V32_H64,  V32_HXX,  V32_H128,
			V64_H32,     V64_H64,  V64_HXX,  V64_H128,
			VXX_H32,     VXX_H64,  VXX_HXX,  VXX_H128,
			V128_H32,    V128_H64, V128_HXX, V128_H128
		};

	/*!**************************************************************
	 * VdpRend: Rendering functions and variables.                  *
	 ****************************************************************/

	private:
		/**
		 * NOTE: rend_init(), rend_end(), and rend_reset() should ONLY be called from
		 * Vdp::Vdp(), Vdp::~Vdp(), and Vdp::Reset()!
		 */
		void rend_init(void);
		void rend_end(void);
		void rend_reset(void);

	public:
		// Palette manager.
		VdpPalette m_palette;

		// MD framebuffer.
		MdFb *MD_Screen;

	public:
		// VDP layer control.
		unsigned int VDP_Layers;

		/** Line rendering functions. **/
		void Render_Line(void);

	private:
		// Line buffer for current line.
		union LineBuf_t
		{
			struct LineBuf_px_t
			{
#if GENS_BYTEORDER == GENS_LIL_ENDIAN
				uint8_t pixel;
				uint8_t layer;
#else /* GENS_BYTEORDER == GENS_BIG_ENDIAN */
				uint8_t layer;
				uint8_t pixel;
#endif
			};
			LineBuf_px_t px[336];
			uint8_t  u8[336<<1];
			uint16_t u16[336];
			uint32_t u32[336>>1];
		};
		LineBuf_t LineBuf;

		template<bool hs, typename pixel>
		inline void T_Update_Palette(pixel *MD_palette, const pixel *palette);

	/*!**************************************************************
	 * VdpRend_m5: Mode 5 rendering functions and variables.        *
	 ****************************************************************/

	public:
		/** Line rendering functions. **/
		void Render_Line_m5(void);

	private:
		// Sprite structs.
		struct {
			int Pos_X;
			int Pos_Y;
			unsigned int Size_X;
			unsigned int Size_Y;
			int Pos_X_Max;
			int Pos_Y_Max;
			unsigned int Num_Tile;	// Includes palette, priority, and flip bits.
			int Pos_X_Max_Vis;	// Number of visible horizontal pixels. (Used for Sprite Limit.)
		} Sprite_Struct[80];
		uint8_t Sprite_Visible[80];	// List of visible sprites. (element == sprite idx in Sprite_Struct[])
		uint8_t TotalSprites;		// Total number of visible sprites.

		template<bool interlaced>
		FORCE_INLINE int T_GetLineNumber(void) const;

		template<bool plane, bool h_s, int pat_pixnum, uint32_t mask, int shift>
		FORCE_INLINE void T_PutPixel_P0(int disp_pixnum, uint32_t pattern, unsigned int palette);

		template<bool plane, bool h_s, int pat_pixnum, uint32_t mask, int shift>
		FORCE_INLINE void T_PutPixel_P1(int disp_pixnum, uint32_t pattern, unsigned int palette);

		template<bool priority, bool h_s, int pat_pixnum, uint32_t mask, int shift>
		FORCE_INLINE uint8_t T_PutPixel_Sprite(int disp_pixnum, uint32_t pattern, unsigned int palette);

		template<bool plane, bool h_s, bool flip>
		FORCE_INLINE void T_PutLine_P0(int disp_pixnum, uint32_t pattern, int palette);

		template<bool plane, bool h_s, bool flip>
		FORCE_INLINE void T_PutLine_P1(int disp_pixnum, uint32_t pattern, int palette);

		template<bool priority, bool h_s, bool flip>
		FORCE_INLINE void T_PutLine_Sprite(int disp_pixnum, uint32_t pattern, int palette);

		template<bool plane>
		FORCE_INLINE uint16_t T_Get_X_Offset(void);

		template<bool plane, bool interlaced>
		FORCE_INLINE unsigned int T_Get_Y_Offset(int cell_cur);

		template<bool interlaced, bool vscroll_mask>
		FORCE_INLINE unsigned int T_Get_Y_Cell_Offset(unsigned int y_offset);

		template<bool interlaced>
		FORCE_INLINE unsigned int T_Get_Y_Fine_Offset(unsigned int y_offset);

		template<bool plane>
		FORCE_INLINE uint16_t T_Get_Pattern_Info(unsigned int x, unsigned int y);

		template<bool interlaced>
		FORCE_INLINE uint32_t T_Get_Pattern_Data(uint16_t pattern, unsigned int y_fine_offset);

		template<bool plane, bool interlaced, bool vscroll, bool h_s>
		FORCE_INLINE void T_Render_Line_Scroll(int cell_start, int cell_length);

		template<bool interlaced, bool vscroll, bool h_s>
		FORCE_INLINE void T_Render_Line_ScrollA_Window(void);

		template<bool interlaced, bool partial>
		FORCE_INLINE void T_Make_Sprite_Struct(void);

		template<bool sprite_limit, bool interlaced>
		FORCE_INLINE unsigned int T_Update_Mask_Sprite(void);

		template<bool interlaced, bool h_s>
		FORCE_INLINE void T_Render_Line_Sprite(void);

		template<bool interlaced, bool h_s>
		FORCE_INLINE void T_Render_Line_m5(void);

		template<typename pixel>
		FORCE_INLINE void T_Render_LineBuf(pixel *dest, pixel *md_palette);

		template<typename pixel>
		FORCE_INLINE void T_Apply_SMS_LCB(pixel *dest, pixel border_color);

	/*!***************************************************************
	 * VdpRend_Err: Error message rendering functions and variables. *
	 *****************************************************************/

	private:
		friend class VdpRend_Err_Private;
		VdpRend_Err_Private *const d_err;

		void Render_Line_Err(void);
		void Update_Err(void);
};

// PAL/NTSC.
inline bool Vdp::isPal(void)
	{ return Reg_Status.isPal(); }
inline bool Vdp::isNtsc(void)
	{ return Reg_Status.isNtsc(); }
inline void Vdp::setPal(void)
	{ Reg_Status.setBit(VdpStatus::VDP_STATUS_PAL, true); }
inline void Vdp::setNtsc(void)
	{ Reg_Status.setBit(VdpStatus::VDP_STATUS_PAL, false); }
inline void Vdp::setVideoMode(bool videoMode)
	{ Reg_Status.setBit(VdpStatus::VDP_STATUS_PAL, videoMode); }

/**
 * Mark VRam as dirty.
 */
inline void Vdp::MarkVRamDirty(void)
	{ m_updateFlags.VRam = true; }

inline int Vdp::GetHPix(void) const
	{ return H_Pix; }

inline int Vdp::GetHPixBegin(void) const
	{ return H_Pix_Begin; }

inline int Vdp::GetHCells(void) const
	{ return H_Cell; }

inline int Vdp::GetVPix(void) const
	{ return VDP_Lines.totalVisibleLines; }

/**
 * Determine if the display mode is H40. (320px wide)
 * @return True if H40; false otherwise.
 */
inline bool Vdp::isH40(void) const
{
	// H40 mode is activated by setting VDP_Reg.m5.Set4, bit 0 (0x01, RS1).
	// Bit 7 (0x80, RS0) is also needed, but RS1 is what tells the VDP
	// to increase the pixel counters to 320px per line.
	// Source: http://wiki.megadrive.org/index.php?title=VDPRegs_Addendum (Jorge)
	return (VDP_Reg.m5.Set4 & 0x01);
}

/**
 * Get the DMA length from the VDP Mode 5 registers.
 * @return DMA length.
 */
inline uint16_t Vdp::DMA_Length(void)
{
	return (VDP_Reg.m5.DMA_Length_H << 8) |
	       (VDP_Reg.m5.DMA_Length_L);
}

/**
 * Set the DMA length in the VDP Mode 5 registers.
 * @param length DMA length.
 */
inline void Vdp::set_DMA_Length(uint16_t length)
{
	VDP_Reg.m5.DMA_Length_H = (length >> 8);
	VDP_Reg.m5.DMA_Length_L = (length & 0xFF);
}

/**
 * Get the DMA source address from the VDP Mode 5 registers.
 * @return DMA source address, divided by 2.
 */
inline uint32_t Vdp::DMA_Src_Adr(void)
{
	return ((VDP_Reg.m5.DMA_Src_Adr_H & 0x7F) << 16) |
		(VDP_Reg.m5.DMA_Src_Adr_M << 8) |
		(VDP_Reg.m5.DMA_Src_Adr_L);
}

/**
 * Increment the DMA source address from the VDP Mode 5 registers.
 * Implements 128 KB wrapping, so only use this from DMA functions!
 * @param length Number of words to increment the source address by.
 */
inline void Vdp::inc_DMA_Src_Adr(uint16_t length)
{
	uint16_t src_adr = (VDP_Reg.m5.DMA_Src_Adr_M << 8) |
			   (VDP_Reg.m5.DMA_Src_Adr_L);

	// Increment the address with 16-bit overflow.
	src_adr += length;

	// Save the new address.
	VDP_Reg.m5.DMA_Src_Adr_M = (src_adr >> 8);
	VDP_Reg.m5.DMA_Src_Adr_L = (src_adr & 0xFF);
}

/** VDP address functions: Get Pointers. **/
inline uint16_t *Vdp::ScrA_Addr_Ptr16(uint16_t offset)
	{ return &VRam.u16[((ScrA_Addr + offset) & 0xFFFF) >> 1]; }
inline uint16_t *Vdp::ScrB_Addr_Ptr16(uint16_t offset)
	{ return &VRam.u16[((ScrB_Addr + offset) & 0xFFFF) >> 1]; }
inline uint16_t *Vdp::Win_Addr_Ptr16(uint16_t offset)
	{ return &VRam.u16[((Win_Addr + offset) & 0xFFFF) >> 1]; }
inline VdpStructs::SprEntry_m5 *Vdp::Spr_Addr_PtrM5(uint16_t offset)
	{ return (VdpStructs::SprEntry_m5*)&VRam.u16[((Spr_Addr + offset) & 0xFFFF) >> 1]; }
inline uint16_t *Vdp::H_Scroll_Addr_Ptr16(uint16_t offset)
	{ return &VRam.u16[((H_Scroll_Addr + offset) & 0xFFFF) >> 1]; }

/** VDP address functions: Get Values. **/
inline uint16_t Vdp::ScrA_Addr_u16(uint16_t offset)
	{ return VRam.u16[((ScrA_Addr + offset) & 0xFFFF) >> 1]; }
inline uint16_t Vdp::ScrB_Addr_u16(uint16_t offset)
	{ return VRam.u16[((ScrB_Addr + offset) & 0xFFFF) >> 1]; }
inline uint16_t Vdp::Win_Addr_u16(uint16_t offset)
	{ return VRam.u16[((Win_Addr + offset) & 0xFFFF) >> 1]; }
inline uint16_t Vdp::Spr_Addr_u16(uint16_t offset)
	{ return VRam.u16[((Spr_Addr + offset) & 0xFFFF) >> 1]; }
inline uint16_t Vdp::H_Scroll_Addr_u16(uint16_t offset)
	{ return VRam.u16[((H_Scroll_Addr + offset) & 0xFFFF) >> 1]; }

}

#endif /* __LIBGENS_MD_VDPIO_HPP__ */
