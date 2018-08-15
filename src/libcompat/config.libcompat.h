/***************************************************************************
 * libcompat: Compatibility library.                                       *
 * config.libcompat.h.in: libcompat configuration. (source file)           *
 *                                                                         *
 * Copyright (c) 2015-2016 by David Korth.                                 *
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

#ifndef __LIBCOMPAT_CONFIG_LIBCOMPAT_H__
#define __LIBCOMPAT_CONFIG_LIBCOMPAT_H__

/* Define to 1 if support for ANSI Windows should be enabled. */
/* #undef ENABLE_ANSI_WINDOWS */

/* Define to 1 if you have the `localtime_r` function. */
#define HAVE_LOCALTIME_R 1

/* Define to 1 if you have the `localtime_s` function. */
/* #undef HAVE_LOCALTIME_S */

/* Define to 1 if you have the `getpwuid_r` function. */
#define HAVE_GETPWUID_R 1

/* Define to 1 if you have the MSVC-specific `_aligned_malloc` function. */
/* #undef HAVE_MSVC_ALIGNED_MALLOC */

/* Define to 1 if you have the `aligned_alloc` function. */
#define HAVE_ALIGNED_ALLOC 1

/* Define to 1 if you have the `posix_memalign` function. */
#define HAVE_POSIX_MEMALIGN 1

/* Define to 1 if you have the `memalign` function. */
#define HAVE_MEMALIGN 1

#endif /* __LIBCOMPAT_CONFIG_LIBCOMPAT_H__ */
