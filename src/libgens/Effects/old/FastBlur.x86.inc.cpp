/***************************************************************************
 * libgens: Gens Emulation Library.                                        *
 * FastBlur.x86.inc.cpp: Fast Blur effect. (i386/amd64 optimized.)         *
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

#ifndef __IN_LIBGENS_FASTBLUR_CPP__
#error FastBlur.x86.inc.cpp should only be included by FastBlur.cpp.
#endif

#if !defined(__GNUC__) || !(defined(__i386__) || defined(__amd64__) || defined(__x86_64__))
#error FastBlur.x86.inc.cpp should only be compiled on i386/amd64 with gcc.
#endif

#if (!defined(DO_1FB) && !defined(DO_2FB)) || \
    ( defined(DO_1FB) &&  defined(DO_2FB))
#error Must define DO_1FB or DO_2FB, but not both.
#endif

#include "libcompat/aligned_malloc.h"

namespace LibGens {

#if 0
// FIXME: SSE2 Fast Blur is generally slow due to
// unaligned access requirements.
// TODO: Try using 'pavgb'?

/**
 * 32-bit color Fast Blur, SSE2-optimized.
 * @param outScreen [out] Destination screen.
 * @param mdScreen  [in]  Source screen. [2-FB only]
 * @param pxCount   [in]  Pixel count.
 */
void FastBlurPrivate::DoFastBlur_32_SSE2(
	uint32_t* RESTRICT outScreen,
#ifdef DO_2FB
	const uint32_t* RESTRICT mdScreen,
#endif
	unsigned int pxCount)
{
	// Make sure the framebuffer(s) are 16-byte aligned.
	assert((uintptr_t)outScreen % 16 == 0);
#ifdef DO_2FB
	assert((uintptr_t)mdScreen % 16 == 0);
#endif

	static const uint32_t ALIGN(16) MASK_DIV2_32_SSE2[4] =
		{0x007F7F7F, 0x007F7F7F, 0x007F7F7F, 0x007F7F7F};

	// Load the 32-bit color mask.
	__asm__ (
		"movdqa %[MASK_DIV2_32_SSE2], %%xmm7"
		:
		: [MASK_DIV2_32_SSE2] "m" (MASK_DIV2_32_SSE2[0])
		);

	// Blur 8px at a time.
	assert(pxCount % 8 == 0);
	for (pxCount /= 8; pxCount > 0; pxCount--) {
		__asm__ (
			// Get source pixels.
			// TODO: movdqu, or movdqa + shift?
#ifdef DO_2FB
			"movdqa	  (%[mdScreen]), %%xmm0\n"
			"movdqu	 4(%[mdScreen]), %%xmm1\n"
			"movdqa	16(%[mdScreen]), %%xmm2\n"
			"movdqu	20(%[mdScreen]), %%xmm3\n"
#else /* DO_1FB */
			"movdqa	  (%[outScreen]), %%xmm0\n"
			"movdqu	 4(%[outScreen]), %%xmm1\n"
			"movdqa	16(%[outScreen]), %%xmm2\n"
			"movdqu	20(%[outScreen]), %%xmm3\n"
#endif

			// Blur source pixels.
			// NOTE: This may lose some precision in the Red LSB on LE architectures.
			"psrld	    $1, %%xmm0\n"
			"psrld	    $1, %%xmm1\n"
			"psrld	    $1, %%xmm2\n"
			"psrld	    $1, %%xmm3\n"
			"pand	%%xmm7, %%xmm0\n"
			"pand	%%xmm7, %%xmm1\n"
			"pand	%%xmm7, %%xmm2\n"
			"pand	%%xmm7, %%xmm3\n"
			"paddd	%%xmm1, %%xmm0\n"
			"paddd	%%xmm3, %%xmm2\n"

			// Put destination pixels.
			"movdqa	%%xmm0,   (%[outScreen])\n"
			"movdqa	%%xmm2, 16(%[outScreen])\n"
			:
			: [outScreen] "r" (outScreen)
#ifdef DO_2FB
			, [mdScreen] "r" (mdScreen)
#endif
			// FIXME: gcc complains that xmm? registers are unknown.
			// May need to compile with -msse...
			//: "xmm0", "xmm1", "xmm2", "xmm3"
			);

		// Next group of pixels.
		outScreen += 8;
#ifdef DO_2FB
		mdScreen += 8;
#endif
	}

	// Reset the FPU state.
	__asm__ __volatile__ ("emms");
}
#endif

/**
 * 15/16-bit color Fast Blur, MMX-optimized.
 * @param outScreen	[out] Destination screen.
 * @param mdScreen	[in]  Source screen. [2-FB only]
 * @param pxCount	[in]  Pixel count.
 * @param mask		[in]  Division mask to use. (MASK_DIV2_15_MMX[] or MASK_DIV2_16_MMX[])
 */
void FastBlurPrivate::DoFastBlur_16_MMX(
	uint16_t* RESTRICT outScreen,
#ifdef DO_2FB
	const uint16_t* RESTRICT mdScreen,
#endif
	unsigned int pxCount,
	const uint32_t *mask)
{
	// Load the 15/16-bit color mask.
	__asm__ (
		"movq %[mask], %%mm7"
		:
		: [mask] "m" (*mask)
		);

	// Blur 4px at a time.
	// TODO: Figure out how to blur 8px at a time.
	assert(pxCount % 4 == 0);
	for (pxCount /= 4; pxCount > 0; pxCount--) {
		__asm__ (
			// Get source pixels.
#ifdef DO_2FB
			"movq	 (%[mdScreen]), %%mm0\n"
			"movq	2(%[mdScreen]), %%mm1\n"
#else /* DO_1FB */
			"movq	 (%[outScreen]), %%mm0\n"
			"movq	2(%[outScreen]), %%mm1\n"
#endif

			// Blur source pixels.
			// NOTE: This may lose some precision in the Red LSB on LE architectures.
			"psrlw	   $1, %%mm0\n"
			"psrlw	   $1, %%mm1\n"
			"pand	%%mm7, %%mm0\n"
			"pand	%%mm7, %%mm1\n"
			"paddw	%%mm1, %%mm0\n"

			// Put destination pixels.
			"movq	%%mm0, (%[outScreen])\n"
			:
			: [outScreen] "r" (outScreen)
#ifdef DO_2FB
			, [mdScreen] "r" (mdScreen)
#endif
			// FIXME: gcc complains that mm? registers are unknown.
			// May need to compile with -mmmx...
			//: "mm0", "mm1"
			);

		// Next group of pixels.
		outScreen += 4;
#ifdef DO_2FB
		mdScreen += 4;
#endif
	}

	// Reset the FPU state.
	__asm__ __volatile__ ("emms");
}

/**
 * 32-bit color Fast Blur, MMX-optimized.
 * @param outScreen [out] Destination screen.
 * @param mdScreen  [in]  Source screen. [2-FB only]
 * @param pxCount   [in]  Pixel count.
 */
void FastBlurPrivate::DoFastBlur_32_MMX(
	uint32_t* RESTRICT outScreen,
#ifdef DO_2FB
	const uint32_t* RESTRICT mdScreen,
#endif
	unsigned int pxCount)
{
	// FIXME: 2FB version is slower on Core 2 T7200
	// than the C implementation.
	static const uint32_t MASK_DIV2_32_MMX[2] =
		{0x007F7F7F, 0x007F7F7F};

	// Load the 32-bit color mask.
	__asm__ (
		"movq %[MASK_DIV2_32_MMX], %%mm7"
		:
		: [MASK_DIV2_32_MMX] "m" (MASK_DIV2_32_MMX[0])
		);

	// Blur 4px at a time.
	assert(pxCount % 4 == 0);
	for (pxCount /= 4; pxCount > 0; pxCount--) {
		__asm__ (
			// Get source pixels.
#ifdef DO_2FB
			"movq	  (%[mdScreen]), %%mm0\n"
			"movq	 4(%[mdScreen]), %%mm1\n"
			"movq	 8(%[mdScreen]), %%mm2\n"
			"movq	12(%[mdScreen]), %%mm3\n"
#else /* DO_1FB */
			"movq	  (%[outScreen]), %%mm0\n"
			"movq	 4(%[outScreen]), %%mm1\n"
			"movq	 8(%[outScreen]), %%mm2\n"
			"movq	12(%[outScreen]), %%mm3\n"
#endif

			// Blur source pixels.
			// NOTE: This may lose some precision in the Red LSB on LE architectures.
			"psrld	   $1, %%mm0\n"
			"psrld	   $1, %%mm1\n"
			"psrld	   $1, %%mm2\n"
			"psrld	   $1, %%mm3\n"
			"pand	%%mm7, %%mm0\n"
			"pand	%%mm7, %%mm1\n"
			"pand	%%mm7, %%mm2\n"
			"pand	%%mm7, %%mm3\n"
			"paddd	%%mm1, %%mm0\n"
			"paddd	%%mm3, %%mm2\n"

			// Put destination pixels.
			"movq	%%mm0,  (%[outScreen])\n"
			"movq	%%mm2, 8(%[outScreen])\n"
			:
			: [outScreen] "r" (outScreen)
#ifdef DO_2FB
			, [mdScreen] "r" (mdScreen)
#endif
			// FIXME: gcc complains that mm? registers are unknown.
			// May need to compile with -mmmx...
			//: "mm0", "mm1", "mm2", "mm3"
			);

		// Next group of pixels.
		outScreen += 4;
#ifdef DO_2FB
		mdScreen += 4;
#endif
	}

	// Reset the FPU state.
	__asm__ __volatile__ ("emms");
}

}
