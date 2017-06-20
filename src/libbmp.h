#ifndef _H_LIBBMP_
#define _H_LIBBMP_

/*
 * libbmp - BMP format function library written in c
 * author	: calvin
 * email	: calvinwilliams@163.com
 * LastVersion	: v0.1.0
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#if ( defined __unix ) || ( defined _AIX ) || ( defined __linux__ ) || ( defined __hpux )
#include <stdint.h>
#define INT8	int8_t
#define INT16	int16_t
#define INT32	int32_t
#define INT64	int64_t
#define UINT8	uint8_t
#define UINT16	uint16_t
#define UINT32	uint32_t
#define UINT64	uint64_t
#elif ( defined _WIN32 )
#define INT8	__int8
#define INT16	__int16
#define INT32	__int32
#define INT64	__int64
#define UINT8	unsigned __int8
#define UINT16	unsigned __int16
#define UINT32	unsigned __int32
#define UINT64	unsigned __int64
#endif

#ifndef _WINDLL_FUNC
#if ( defined __unix ) || ( defined __linux__ )
#define _WINDLL_FUNC
#elif ( defined _WIN32 )
#define _WINDLL_FUNC		_declspec(dllexport)
#endif
#endif

struct BitmapContext ;

#pragma pack(1)
struct RBGA
{
	UINT8			blue ;
	UINT8			green ;
	UINT8			red ;
	UINT8			alpha ;
} ;
#pragma pack()

#define LIBBMP_ERROR_PARAMETER			1
#define LIBBMP_ERROR_NOT_SUPPORT		2
#define LIBBMP_ERROR_MALLOC			11
#define LIBBMP_ERROR_FILE_INVALID		22
#define LIBBMP_ERROR_FILE_NOT_FOUND		23
#define LIBBMP_ERROR_FILE_READ_ONLY		24
#define LIBBMP_ERROR_READ_FILE			25
#define LIBBMP_ERROR_WRITE_FILE			26

#define LIBBMP_PLANES_1				1

#define LIBBMP_BITCOUNT_24			24
#define LIBBMP_BITCOUNT_32			32

#define LIBBMP_COMPRESSION_NONE			0

/* 得到最后错误码 */
_WINDLL_FUNC int GetBitmapLastError();

/* 读位图文件到位图上下文 */
_WINDLL_FUNC struct BitmapContext *ReadBitmapFile( char *bmp_pathfilename_format , ... );

/* 写位图上下文到位图文件 */
_WINDLL_FUNC int WriteBitmapFile( struct BitmapContext *p_bitmap_context , char *bmp_pathfilename_format , ... );

/* 创建位图上下文 */
_WINDLL_FUNC struct BitmapContext *CreateBitmapContext( INT32 width , INT32 height , UINT16 planes , UINT16 bit_count , UINT32 compression , INT32 x_pels_per_meter , INT32 y_pels_per_meter , UINT8 clr_idx , struct RBGA *p_bg_rbga );

/* 释放位图上下文 */
_WINDLL_FUNC void FreeBitmapContext( struct BitmapContext *p_bitmap_context );

/* 得到位图文件头属性 */
_WINDLL_FUNC char *GetBitmapType( struct BitmapContext *p_bitmap_context );
_WINDLL_FUNC UINT32 GetBitmapSize( struct BitmapContext *p_bitmap_context );
_WINDLL_FUNC UINT16 GetBitmapReserved1( struct BitmapContext *p_bitmap_context );
_WINDLL_FUNC UINT16 GetBitmapReserved2( struct BitmapContext *p_bitmap_context );
_WINDLL_FUNC UINT32 GetBitmapDataOffset( struct BitmapContext *p_bitmap_context );

/* 得到位图信息头属性 */
_WINDLL_FUNC INT32 GetBitmapWidth( struct BitmapContext *p_bitmap_context );
_WINDLL_FUNC INT32 GetBitmapHeight( struct BitmapContext *p_bitmap_context );
_WINDLL_FUNC UINT16 GetBitmapPlanes( struct BitmapContext *p_bitmap_context );
_WINDLL_FUNC UINT16 GetBitmapBitCount( struct BitmapContext *p_bitmap_context );
_WINDLL_FUNC UINT32 GetBitmapCompression( struct BitmapContext *p_bitmap_context );
_WINDLL_FUNC UINT32 GetBitmapSizeImage( struct BitmapContext *p_bitmap_context );
_WINDLL_FUNC INT32 GetBitmapXPelsPerMeter( struct BitmapContext *p_bitmap_context );
_WINDLL_FUNC INT32 GetBitmapYPelsPerMeter( struct BitmapContext *p_bitmap_context );
_WINDLL_FUNC UINT32 GetBitmapClrUsed( struct BitmapContext *p_bitmap_context );
_WINDLL_FUNC UINT32 GetBitmapClrImportant( struct BitmapContext *p_bitmap_context );

/* 读位图数据 */
_WINDLL_FUNC int GetBitmapMatrixRbga( struct BitmapContext *p_bitmap_context , INT32 x , INT32 y , struct RBGA *p_rbga );
_WINDLL_FUNC struct RBGA *GetBitmapMatrixRbgaPtr( struct BitmapContext *p_bitmap_context , INT32 x , INT32 y );

/* 写位图数据 */
_WINDLL_FUNC int SetBitmapMatrixRbga( struct BitmapContext *p_bitmap_context , INT32 x , INT32 y , struct RBGA *p_rbga );

/* 得到头大小 */
_WINDLL_FUNC UINT32 GetBitmapFileHeaderSize();
_WINDLL_FUNC UINT32 GetBitmapInfoHeaderSize();

#ifdef __cplusplus
}
#endif

#endif
