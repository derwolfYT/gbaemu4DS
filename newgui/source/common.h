/* unofficial gameplaySP kai
 *
 * Copyright (C) 2006 Exophase <exophase@gmail.com>
 * Copyright (C) 2007 takka <takka@tfact.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/******************************************************************************
 * common.h
 * 通用头文件
 ******************************************************************************/

#ifndef COMMON_H
#define COMMON_H

#define OLD_COUNT

/******************************************************************************
 * 
 ******************************************************************************/

#define NDS_LAYER

#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bitmap.h"

//#include <fastmath.h>

//#include <zlib.h>


/******************************************************************************
 * 宏定义
 ******************************************************************************/

#define GET_DISK_FREE_SPACE FS_CMD_GET_DISKFREE
#define FILE_ID FILE*

//typedef SceUID FILE_TAG_TYPE;
typedef FILE_ID FILE_TAG_TYPE;
typedef u32 FIXED16_16;    // 整数部16bit 小数部16bit 的定点数
typedef u32 FIXED8_24;     // 整数部 8bit 小数部24bit 的定点数

#define MAX_PATH 512
#define MAX_FILE 512

#define SYS_CLOCK (16777216.0)

#define ROR(dest, value, shift)                                             \
  dest = ((value) >> (shift)) | ((value) << (32 - (shift)))                 \

//#define PSP_FILE_OPEN_APPEND (PSP_O_CREAT | PSP_O_APPEND | PSP_O_TRUNC)
#define PSP_FILE_OPEN_APPEND ("a+")

//#define PSP_FILE_OPEN_READ PSP_O_RDONLY
#define PSP_FILE_OPEN_READ ("rb")

//#define PSP_FILE_OPEN_WRITE (PSP_O_CREAT | PSP_O_WRONLY | PSP_O_TRUNC)
#define PSP_FILE_OPEN_WRITE ("wb")

#define FILE_OPEN(filename_tag, filename, mode)                             \
  filename_tag = fopen(filename, PSP_FILE_OPEN_##mode)						\

#define FILE_CHECK_VALID(filename_tag)                                      \
  (filename_tag != NULL)                                                    \

#define FILE_CLOSE(filename_tag)                                            \
  fclose(filename_tag)                                                      \

#define FILE_DELETE(filename)                                               \
  unlink(filename)                                                          \

#define FILE_READ(filename_tag, buffer, size)                               \
  fread(buffer, 1, size, filename_tag)                                      \

#define FILE_WRITE(filename_tag, buffer, size)                              \
  fwrite(buffer, 1, size, filename_tag)                                     \

#define FILE_READ_MEM(ptr, buffer, size)                                    \
{                                                                           \
  memcpy(buffer, ptr, size);                                                \
  ptr += size;                                                              \
}                                                                           \

#define FILE_WRITE_MEM(ptr, buffer, size)                                   \
{                                                                           \
  memcpy(ptr, buffer, size);                                                \
  ptr += size;                                                              \
}                                                                           \

#define FILE_SEEK(filename_tag, offset, type)                               \
  fseek(filename_tag, offset, type)                                         \

#define FILE_TELL(filename_tag)                                             \
  ftell(filename_tag)                                                       \

// These must be variables, not constants.

#define FILE_READ_VARIABLE(filename_tag, variable)                          \
  FILE_READ(filename_tag, &variable, sizeof(variable))                      \

#define FILE_WRITE_VARIABLE(filename_tag, variable)                         \
  FILE_WRITE(filename_tag, &variable, sizeof(variable))                     \

#define FILE_READ_MEM_VARIABLE(ptr, variable)                               \
  FILE_READ_MEM(ptr, &variable, sizeof(variable))                           \

#define FILE_WRITE_MEM_VARIABLE(ptr, variable)                              \
  FILE_WRITE_MEM(ptr, &variable, sizeof(variable))                          \

// These must be statically declared arrays (ie, global or on the stack,
// not dynamically allocated on the heap)

#define FILE_READ_ARRAY(filename_tag, array)                                \
  FILE_READ(filename_tag, array, sizeof(array))                             \

#define FILE_WRITE_ARRAY(filename_tag, array)                               \
  FILE_WRITE(filename_tag, array, sizeof(array))                            \

#define FILE_READ_MEM_ARRAY(ptr, array)                                     \
  FILE_READ_MEM(ptr, array, sizeof(array))                                  \

#define FILE_WRITE_MEM_ARRAY(ptr, array)                                    \
  FILE_WRITE_MEM(ptr, array, sizeof(array))                                 \

#define FLOAT_TO_FP16_16(value)                                             \
  (FIXED16_16)((value) * 65536.0)                                           \

#define FP16_16_TO_FLOAT(value)                                             \
  (float)((value) / 65536.0)                                                \

#define U32_TO_FP16_16(value)                                               \
  ((value) << 16)                                                           \

#define FP16_16_TO_U32(value)                                               \
  ((value) >> 16)                                                           \

#define FP16_16_FRACTIONAL_PART(value)                                      \
  ((value) & 0xFFFF)                                                        \

#define FIXED_DIV(numerator, denominator, bits)                             \
  ((((numerator) * (1 << (bits))) + ((denominator) / 2)) / (denominator))   \

#define ADDRESS8(base, offset)                                              \
  *((u8 *)((u8 *)(base) + (offset)))                                        \

#define ADDRESS16(base, offset)                                             \
  *((u16 *)((u8 *)(base) + (offset)))                                       \

#define ADDRESS32(base, offset)                                             \
  *((u32 *)((u8 *)(base) + (offset)))                                       \

//#define printf pspDebugScreenPrintf

#define USE_BIOS 0
#define EMU_BIOS 1

#define NO  0
#define YES 1

#ifdef USE_DEBUG
// デバッグ用の設定
#define DBG_FILE_NAME "dbg_msg.txt"
#define DBGOUT(...) if(gpsp_config.debug_flag != NO) fprintf(g_dbg_file, __VA_ARGS__)
#define DBGOUT_1(...) if(gpsp_config.debug_flag >= 1) fprintf(g_dbg_file, __VA_ARGS__)
#define DBGOUT_2(...) if(gpsp_config.debug_flag >= 2) fprintf(g_dbg_file, __VA_ARGS__)
#define DBGOUT_3(...) if(gpsp_config.debug_flag >= 3) fprintf(g_dbg_file, __VA_ARGS__)
#define DBGOUT_4(...) if(gpsp_config.debug_flag >= 4) fprintf(g_dbg_file, __VA_ARGS__)
#define DBGOUT_5(...) if(gpsp_config.debug_flag >= 5) fprintf(g_dbg_file, __VA_ARGS__)
FILE *g_dbg_file;

u64 dbg_time_1;

u64 dbg_time_2;
//#define GET_TIME_1() sceRtcGetCurrentTick(&dbg_time_1);
#define GET_TIME_1()
//#define GET_TIME_2() sceRtcGetCurrentTick(&dbg_time_2);
#define GET_TIME_2()
#define WRITE_TIME(msg) DBGOUT("%s: %d μs\n",msg,(int)(dbg_time_2 - dbg_time_1));
#else
#define DBGOUT(...)
#endif

#define dbg_printf printf

//ichfly def
struct key_buf
{
    unsigned short key;
    unsigned short x;
    unsigned short y;
};

typedef enum SCREEN_ID
{
    UP_SCREEN = 1,
    DOWN_SCREEN = 2,
    DUAL_SCREEN = 3,
    JOINTUSE_SCREEN = 3
} SCREEN_ID;

#define UP_MASK		0x1
#define DOWN_MASK	0x2
#define DUAL_MASK	0x3

//video buffer address of up screen
extern void* up_screen_addr;

//video buffer address of down screen
extern void* down_screen_addr;

struct rtc{
    volatile unsigned char year;		//add 2000 to get 4 digit year
    volatile unsigned char month;		//1 to 12
    volatile unsigned char day;			//1 to (days in month)

    volatile unsigned char weekday;		// day of week
    volatile unsigned char hours;		//0 to 11 for AM, 52 to 63 for PM
    volatile unsigned char minutes;		//0 to 59
    volatile unsigned char seconds;		//0 to 59
};
extern void ds2_getTime(struct rtc *time);

extern u32 sound_on;



#endif 
