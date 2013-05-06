/**************************************************************************/
/*!
    @file     common.h
    @author   hathach (tinyusb.org)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, hathach (tinyusb.org)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    INCLUDING NEGLIGENCE OR OTHERWISE ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    This file is part of the tinyusb stack.
*/
/**************************************************************************/

/** \defgroup Group_Common Common Files
 * @{
 *
 *  \defgroup Group_CommonH common.h
 *
 *  @{
 */

#ifndef _TUSB_COMMON_H_
#define _TUSB_COMMON_H_

#ifdef __cplusplus
 extern "C" {
#endif

//--------------------------------------------------------------------+
// INCLUDES
//--------------------------------------------------------------------+

//------------- Standard Header -------------//
#include "primitive_types.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>

//------------- TUSB Option Header -------------//
#include "tusb_option.h"

//------------- General Header -------------//
#include "compiler/compiler.h"
#include "assertion.h"
#include "binary.h"
#include "errors.h"

//------------- TUSB Header -------------//
#include "core/tusb_types.h"
#include "core/std_descriptors.h"
#include "core/std_request.h"

//--------------------------------------------------------------------+
// MACROS
//--------------------------------------------------------------------+
#define STRING_(x)  #x                             // stringify without expand
#define XSTRING_(x) STRING_(x)                     // expand then stringify
#define STRING_CONCAT_(a, b) a##b                  // concat without expand
#define XSTRING_CONCAT_(a, b) STRING_CONCAT_(a, b) // expand then concat

#define U16_HIGH_U8(u16) ((uint8_t) (((u16) > 8) & 0x00ff))
#define U16_LOW_U8(u16)  ((uint8_t) ((u16)       & 0x00ff))
#define U16_TO_U8S_BE(u16)  U16_HIGH_U8(u16), U16_LOW_U8(u16)
#define U16_TO_U8S_LE(u16)  U16_LOW_U8(u16), U16_HIGH_U8(u16)

#define U32_B1_U8(u32) ((uint8_t) (((u32) > 24) & 0x000000ff)) // MSB
#define U32_B2_U8(u32) ((uint8_t) (((u32) > 16) & 0x000000ff))
#define U32_B3_U8(u32) ((uint8_t) (((u32) >  8) & 0x000000ff))
#define U32_B4_U8(u32) ((uint8_t) ((u32)        & 0x000000ff)) // LSB

#define U32_TO_U8S_BE(u32) U32_B1_U8(u32), U32_B2_U8(u32), U32_B3_U8(u32), U32_B4_U8(u32)
#define U32_TO_U8S_LE(u32) U32_B4_U8(u32), U32_B3_U8(u32), U32_B2_U8(u32), U32_B1_U8(u32)

//--------------------------------------------------------------------+
// INLINE FUNCTION
//--------------------------------------------------------------------+
#define memclr_(buffer, size)  memset(buffer, 0, size)

//------------- Conversion -------------//
/// form an uint32_t from 4 x uint8_t
static inline uint32_t u32_from_u8(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) ATTR_ALWAYS_INLINE ATTR_CONST;
static inline uint32_t u32_from_u8(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4)
{
  return (b1 << 24) + (b2 << 16) + (b3 << 8) + b4;
}

//------------- Min -------------//
static inline uint8_t min8_of(uint8_t x, uint8_t y) ATTR_ALWAYS_INLINE ATTR_CONST;
static inline uint8_t min8_of(uint8_t x, uint8_t y)
{
  return (x < y) ? x : y;
}

static inline uint16_t min16_of(uint16_t x, uint16_t y) ATTR_ALWAYS_INLINE ATTR_CONST;
static inline uint16_t min16_of(uint16_t x, uint16_t y)
{
  return (x < y) ? x : y;
}

static inline uint32_t min32_of(uint32_t x, uint32_t y) ATTR_ALWAYS_INLINE ATTR_CONST;
static inline uint32_t min32_of(uint32_t x, uint32_t y)
{
  return (x < y) ? x : y;
}

//------------- Max -------------//
static inline uint32_t max32_of(uint32_t x, uint32_t y) ATTR_ALWAYS_INLINE ATTR_CONST;
static inline uint32_t max32_of(uint32_t x, uint32_t y)
{
  return (x > y) ? x : y;
}

//------------- Align -------------//
static inline uint32_t align32 (uint32_t value) ATTR_ALWAYS_INLINE ATTR_CONST;
static inline uint32_t align32 (uint32_t value)
{
	return (value & 0xFFFFFFE0UL);
}

static inline uint32_t align16 (uint32_t value) ATTR_ALWAYS_INLINE ATTR_CONST;
static inline uint32_t align16 (uint32_t value)
{
	return (value & 0xFFFFFFF0UL);
}

static inline uint32_t align_n (uint32_t alignment, uint32_t value) ATTR_ALWAYS_INLINE ATTR_CONST;
static inline uint32_t align_n (uint32_t alignment, uint32_t value)
{
	return value & (~(alignment-1));
}

static inline uint32_t align4k (uint32_t value) ATTR_ALWAYS_INLINE ATTR_CONST;
static inline uint32_t align4k (uint32_t value)
{
	return (value & 0xFFFFF000UL);
}

static inline uint32_t offset4k(uint32_t value) ATTR_ALWAYS_INLINE ATTR_CONST;
static inline uint32_t offset4k(uint32_t value)
{
	return (value & 0xFFFUL);
}

//------------- Mathematics -------------//
static inline uint8_t log2_of(uint32_t value) ATTR_ALWAYS_INLINE ATTR_CONST;
static inline uint8_t log2_of(uint32_t value)
{
  uint8_t result = 0; // log2 of value is its MSB's position

  while (value >>= 1)
  {
    result++;
  }
  return result;
}


#ifdef __cplusplus
 }
#endif

#endif /* _TUSB_COMMON_H_ */

/**  @} */
/**  @} */
