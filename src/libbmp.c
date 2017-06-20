/*
 * libbmp - BMP format function library written in c
 * author	: calvin
 * email	: calvinwilliams@163.com
 * LastVersion	: v0.1.0
 *
 * Licensed under the LGPL v2.1, see the file LICENSE in base directory.
 */

#include "libbmp.h"

/* �̱߳��ش洢ȫ�ֶ��� */ /* TLS */
#if ( defined _WIN32 )
__declspec( thread ) int	_libbmp_errno ;
#elif ( defined __linux__ ) || ( defined _AIX )
__thread int			_libbmp_errno ;
#endif

#pragma pack(1)

/* λͼ�ļ�ͷ */
struct BitmapFileHeader
{
	UINT16			type ;
	UINT32			size ;
	UINT16			reserved1 ;
	UINT16			reserved2 ;
	UINT32			bitmap_data_offset ;
} ;

/* λͼ��Ϣͷ */
struct BitmapInfoHeader
{
	UINT32			this_size ;
	INT32			width ;
	INT32			height ;
	UINT16			planes ;
	UINT16			bit_count ;
	UINT32			compression ;
	UINT32			size_image ;
	INT32			x_pels_per_meter ;
	INT32			y_pels_per_meter ;
	UINT32			clr_used ;
	UINT32			clr_important ;
} ;

/* ��ɫ�� */
struct RbgGuad
{
	UINT8			blue ;
	UINT8			green ;
	UINT8			red ;
	UINT8			reserved ;
} ;

/* λͼ���� */
struct BitmapData
{
	union
	{
		struct RBGA	*rbga_matrix ;
	} data ;
} ;

#pragma pack()

/* λͼ������ */
struct BitmapContext
{
	char			*bmp_pathfilename ;
	
	UINT32			fill_bytes_aligned ;
	
	struct BitmapFileHeader	bitmap_file_header ;
	struct BitmapInfoHeader	bitmap_info_header ;
	struct RbgGuad		rbg_guad ;
	struct BitmapData	bitmap_data ;
} ;

/* �õ��������� */
int GetBitmapLastError()
{
	return _libbmp_errno;
}

/* ��λͼ�ļ���λͼ������ */
struct BitmapContext *ReadBitmapFile( char *bmp_pathfilename_format , ... )
{
	va_list			valist ;
	char			bmp_pathfilename[ 256 + 1 ] ;
	struct BitmapContext	*p_bitmap_context = NULL ;
	FILE			*fp = NULL ;
	INT32			y , x ;
	int			nret = 0 ;
	
	/* ��λͼ�ļ� */
	memset( bmp_pathfilename , 0x00 , sizeof(bmp_pathfilename) );
	va_start( valist , bmp_pathfilename_format );
	vsnprintf( bmp_pathfilename , sizeof(bmp_pathfilename)-1 , bmp_pathfilename_format , valist );
	va_end( valist );
	fp = fopen( bmp_pathfilename , "rb" ) ;
	if( fp == NULL )
	{
		_libbmp_errno = LIBBMP_ERROR_FILE_NOT_FOUND ;
		return NULL;
	}
	
	/* �����ڴ��Դ��λͼ������ */
	p_bitmap_context = (struct BitmapContext *)malloc( sizeof(struct BitmapContext) ) ;
	if( p_bitmap_context == NULL )
	{
		_libbmp_errno = LIBBMP_ERROR_MALLOC ;
		fclose( fp );
		return NULL;
	}
	memset( p_bitmap_context , 0x00 , sizeof(struct BitmapContext) );
	
	/* ��λͼ�ļ�ͷ */
	nret = fread( & (p_bitmap_context->bitmap_file_header) , sizeof(struct BitmapFileHeader) , 1 , fp ) ;
	if( nret != 1 )
	{
		_libbmp_errno =LIBBMP_ERROR_READ_FILE ;
		FreeBitmapContext( p_bitmap_context );
		fclose( fp );
		return NULL;
	}
	
	/* ��λͼ��Ϣͷ */
	nret = fread( & (p_bitmap_context->bitmap_info_header) , sizeof(struct BitmapInfoHeader) , 1 , fp ) ;
	if( nret != 1 )
	{
		_libbmp_errno =LIBBMP_ERROR_READ_FILE ;
		FreeBitmapContext( p_bitmap_context );
		fclose( fp );
		return NULL;
	}
	
	/* ���λͼ��� */
	if( p_bitmap_context->bitmap_info_header.width <= 0 )
	{
		_libbmp_errno = LIBBMP_ERROR_FILE_INVALID ;
		FreeBitmapContext( p_bitmap_context );
		fclose( fp );
		return NULL;
	}
	
	/* ���ѹ������ */
	if( p_bitmap_context->bitmap_info_header.compression > 0 )
	{
		_libbmp_errno = LIBBMP_ERROR_NOT_SUPPORT ;
		FreeBitmapContext( p_bitmap_context );
		fclose( fp );
		return NULL;
	}
	
	/* ���ˮƽ�ֱ��� */
	if( p_bitmap_context->bitmap_info_header.x_pels_per_meter > 0 )
	{
		_libbmp_errno = LIBBMP_ERROR_NOT_SUPPORT ;
		FreeBitmapContext( p_bitmap_context );
		fclose( fp );
		return NULL;
	}
	
	/* ��鴹ֱ�ֱ��� */
	if( p_bitmap_context->bitmap_info_header.y_pels_per_meter > 0 )
	{
		_libbmp_errno = LIBBMP_ERROR_NOT_SUPPORT ;
		FreeBitmapContext( p_bitmap_context );
		fclose( fp );
		return NULL;
	}
	
	/* �����ɫ������ */
	if( p_bitmap_context->bitmap_info_header.clr_used > 0 )
	{
		_libbmp_errno = LIBBMP_ERROR_NOT_SUPPORT ;
		FreeBitmapContext( p_bitmap_context );
		fclose( fp );
		return NULL;
	}
	
	/* �����Ҫ����ɫ������ */
	if( p_bitmap_context->bitmap_info_header.clr_important > 0 )
	{
		_libbmp_errno = LIBBMP_ERROR_NOT_SUPPORT ;
		FreeBitmapContext( p_bitmap_context );
		fclose( fp );
		return NULL;
	}
	
	/* ��λͼ������ */
	if( p_bitmap_context->bitmap_info_header.bit_count == LIBBMP_BITCOUNT_24 )
	{
		/* �����Ȳ���ֵ */
		p_bitmap_context->fill_bytes_aligned = (p_bitmap_context->bitmap_info_header.width*3) % 4 ;
		if( p_bitmap_context->fill_bytes_aligned > 0 )
		{
			p_bitmap_context->fill_bytes_aligned = 4 - p_bitmap_context->fill_bytes_aligned ;
		}
		
		/* ���λͼ��С */
		if( p_bitmap_context->bitmap_info_header.size_image == 0 )
		{
			if( ((sizeof(struct RBGA)-sizeof(UINT8))*p_bitmap_context->bitmap_info_header.width+p_bitmap_context->fill_bytes_aligned) * p_bitmap_context->bitmap_info_header.height != p_bitmap_context->bitmap_info_header.size_image )
			{
				_libbmp_errno = LIBBMP_ERROR_FILE_INVALID ;
				FreeBitmapContext( p_bitmap_context );
				fclose( fp );
				return NULL;
			}
		}
		
		/* �����ڴ��Դ��λͼ���� */
		p_bitmap_context->bitmap_data.data.rbga_matrix = (struct RBGA *)malloc( sizeof(struct RBGA) * p_bitmap_context->bitmap_info_header.width * p_bitmap_context->bitmap_info_header.height ) ;
		if( p_bitmap_context->bitmap_data.data.rbga_matrix == NULL )
		{
			_libbmp_errno = LIBBMP_ERROR_MALLOC ;
			FreeBitmapContext( p_bitmap_context );
			fclose( fp );
			return NULL;
		}
		memset( p_bitmap_context->bitmap_data.data.rbga_matrix , 0x00 , sizeof(struct RBGA) * p_bitmap_context->bitmap_info_header.width * p_bitmap_context->bitmap_info_header.height );
		
		/* ��λͼ���� */
		for( y = p_bitmap_context->bitmap_info_header.height-1 ; y >= 0 ; y-- )
		{
			for( x = 0 ; x < p_bitmap_context->bitmap_info_header.width ; x++ )
			{
				nret = fread( p_bitmap_context->bitmap_data.data.rbga_matrix+y*p_bitmap_context->bitmap_info_header.width+x , sizeof(struct RBGA)-sizeof(UINT8) , 1 , fp ) ;
				if( nret != 1 )
				{
					_libbmp_errno =LIBBMP_ERROR_READ_FILE ;
					FreeBitmapContext( p_bitmap_context );
					fclose( fp );
					return NULL;
				}
			}
			
			if( p_bitmap_context->fill_bytes_aligned > 0 )
			{
				fseek( fp , p_bitmap_context->fill_bytes_aligned , SEEK_CUR );
			}
		}
	}
	else if( p_bitmap_context->bitmap_info_header.bit_count == LIBBMP_BITCOUNT_32 )
	{
		/* ���λͼ��С */
		if( p_bitmap_context->bitmap_info_header.size_image == 0 )
		{
			if( (sizeof(struct RBGA)*p_bitmap_context->bitmap_info_header.width) * p_bitmap_context->bitmap_info_header.height != p_bitmap_context->bitmap_info_header.size_image )
			{
				_libbmp_errno = LIBBMP_ERROR_FILE_INVALID ;
				FreeBitmapContext( p_bitmap_context );
				fclose( fp );
				return NULL;
			}
		}
		
		/* �����ڴ��Դ��λͼ���� */
		p_bitmap_context->bitmap_data.data.rbga_matrix = (struct RBGA *)malloc( sizeof(struct RBGA) * p_bitmap_context->bitmap_info_header.width * p_bitmap_context->bitmap_info_header.height ) ;
		if( p_bitmap_context->bitmap_data.data.rbga_matrix == NULL )
		{
			_libbmp_errno =LIBBMP_ERROR_MALLOC ;
			FreeBitmapContext( p_bitmap_context );
			fclose( fp );
			return NULL;
		}
		memset( p_bitmap_context->bitmap_data.data.rbga_matrix , 0x00 , sizeof(struct RBGA) * p_bitmap_context->bitmap_info_header.width * p_bitmap_context->bitmap_info_header.height );
		
		/* ��λͼ���� */
		for( y = p_bitmap_context->bitmap_info_header.height-1 ; y >= 0 ; y-- )
		{
			for( x = 0 ; x < p_bitmap_context->bitmap_info_header.width ; x++ )
			{
				nret = fread( p_bitmap_context->bitmap_data.data.rbga_matrix+y*p_bitmap_context->bitmap_info_header.width+x , sizeof(struct RBGA) , 1 , fp ) ;
				if( nret != 1 )
				{
					_libbmp_errno =LIBBMP_ERROR_READ_FILE ;
					FreeBitmapContext( p_bitmap_context );
					fclose( fp );
					return NULL;
				}
			}
		}
	}
	else
	{
		_libbmp_errno = LIBBMP_ERROR_NOT_SUPPORT ;
		FreeBitmapContext( p_bitmap_context );
		fclose( fp );
		return NULL;
	}
	
	/* ��������ļ�β��װ��ʧ�� */
	if( feof(fp) )
	{
		_libbmp_errno = LIBBMP_ERROR_FILE_INVALID ;
		FreeBitmapContext( p_bitmap_context );
		fclose( fp );
		return NULL;
	}
	
	/* �ر��ļ� */
	fclose( fp );
	
	return p_bitmap_context;
}

/* дλͼ�����ĵ�λͼ�ļ� */
int WriteBitmapFile( struct BitmapContext *p_bitmap_context , char *bmp_pathfilename_format , ... )
{
	va_list			valist ;
	char			bmp_pathfilename[ 256 + 1 ] ;
	FILE			*fp = NULL ;
	INT32			y , x ;
	int			nret = 0 ;
	
	/* ��λͼ�ļ� */
	memset( bmp_pathfilename , 0x00 , sizeof(bmp_pathfilename) );
	va_start( valist , bmp_pathfilename_format );
	vsnprintf( bmp_pathfilename , sizeof(bmp_pathfilename)-1 , bmp_pathfilename_format , valist );
	va_end( valist );
	fp = fopen( bmp_pathfilename , "wb" ) ;
	if( fp == NULL )
	{
		_libbmp_errno = LIBBMP_ERROR_FILE_READ_ONLY ;
		return -1;
	}
	
	/* ��λͼ�ļ�ͷ */
	nret = fwrite( & (p_bitmap_context->bitmap_file_header) , sizeof(struct BitmapFileHeader) , 1 , fp ) ;
	if( nret != 1 )
	{
		_libbmp_errno =LIBBMP_ERROR_WRITE_FILE ;
		fclose( fp );
		return -1;
	}
	
	/* ��λͼ��Ϣͷ */
	nret = fwrite( & (p_bitmap_context->bitmap_info_header) , sizeof(struct BitmapInfoHeader) , 1 , fp ) ;
	if( nret != 1 )
	{
		_libbmp_errno =LIBBMP_ERROR_WRITE_FILE ;
		fclose( fp );
		return -1;
	}
	
	/* дλͼ������ */
	if( p_bitmap_context->bitmap_info_header.bit_count == LIBBMP_BITCOUNT_24 )
	{
		/* дλͼ���� */
		for( y = p_bitmap_context->bitmap_info_header.height-1 ; y >= 0 ; y-- )
		{
			for( x = 0 ; x < p_bitmap_context->bitmap_info_header.width ; x++ )
			{
				nret = fwrite( p_bitmap_context->bitmap_data.data.rbga_matrix+y*p_bitmap_context->bitmap_info_header.width+x , sizeof(struct RBGA)-sizeof(UINT8) , 1 , fp ) ;
				if( nret != 1 )
				{
					_libbmp_errno =LIBBMP_ERROR_WRITE_FILE ;
					fclose( fp );
					return -1;
				}
			}
			
			if( p_bitmap_context->fill_bytes_aligned > 0 )
			{
				nret = fwrite( "\0\0\0\0" , p_bitmap_context->fill_bytes_aligned , 1 , fp );
				if( nret != 1 )
				{
					_libbmp_errno =LIBBMP_ERROR_WRITE_FILE ;
					fclose( fp );
					return -1;
				}
			}
		}
	}
	else if( p_bitmap_context->bitmap_info_header.bit_count == LIBBMP_BITCOUNT_32 )
	{
		/* дλͼ���� */
		for( y = p_bitmap_context->bitmap_info_header.height-1 ; y >= 0 ; y-- )
		{
			for( x = 0 ; x < p_bitmap_context->bitmap_info_header.width ; x++ )
			{
				nret = fwrite( p_bitmap_context->bitmap_data.data.rbga_matrix+y*p_bitmap_context->bitmap_info_header.width+x , sizeof(struct RBGA) , 1 , fp ) ;
				if( nret != 1 )
				{
					_libbmp_errno =LIBBMP_ERROR_WRITE_FILE ;
					fclose( fp );
					return -1;
				}
			}
		}
	}
	
	/* �ر��ļ� */
	fclose( fp );
	
	return 0;
}

/* ����λͼ������ */
struct BitmapContext *CreateBitmapContext( INT32 width , INT32 height , UINT16 planes , UINT16 bit_count , UINT32 compression , INT32 x_pels_per_meter , INT32 y_pels_per_meter , UINT8 clr_idx , struct RBGA *p_bg_rbga )
{
	struct BitmapContext	*p_bitmap_context = NULL ;
	INT32			y , x ;
	
	/* �����ڴ��Դ��λͼ������ */
	p_bitmap_context = (struct BitmapContext *)malloc( sizeof(struct BitmapContext) ) ;
	if( p_bitmap_context == NULL )
	{
		_libbmp_errno = LIBBMP_ERROR_MALLOC ;
		return NULL;
	}
	memset( p_bitmap_context , 0x00 , sizeof(struct BitmapContext) );
	
	/* ������ */
	if( width <= 0 || height <= 0 )
	{
		_libbmp_errno = LIBBMP_ERROR_PARAMETER ;
		FreeBitmapContext( p_bitmap_context );
		return NULL;
	}
	
	if( planes != LIBBMP_PLANES_1 || compression != LIBBMP_COMPRESSION_NONE || clr_idx != 0 )
	{
		_libbmp_errno = LIBBMP_ERROR_NOT_SUPPORT ;
		FreeBitmapContext( p_bitmap_context );
		return NULL;
	}
	
	/* �����Ȳ���ֵ */
	if( bit_count == LIBBMP_BITCOUNT_24 )
	{
		p_bitmap_context->fill_bytes_aligned = (width*3) % 4 ;
		if( p_bitmap_context->fill_bytes_aligned > 0 )
			p_bitmap_context->fill_bytes_aligned = 4 - p_bitmap_context->fill_bytes_aligned ;
	}
	else if( bit_count == LIBBMP_BITCOUNT_32 )
	{
		p_bitmap_context->fill_bytes_aligned = 0 ;
	}
	else
	{
		_libbmp_errno = LIBBMP_ERROR_NOT_SUPPORT ;
		FreeBitmapContext( p_bitmap_context );
		return NULL;
	}
	
	/* ���λͼ�ļ�ͷ */
	p_bitmap_context->bitmap_file_header.type = *(UINT16*)"BM" ;
	p_bitmap_context->bitmap_file_header.size = sizeof(struct BitmapFileHeader) + sizeof(struct BitmapInfoHeader) + (width*(sizeof(struct RBGA)-sizeof(UINT8))+p_bitmap_context->fill_bytes_aligned) * height ;
	p_bitmap_context->bitmap_file_header.bitmap_data_offset = sizeof(struct BitmapFileHeader) + sizeof(struct BitmapInfoHeader) ;
	
	/* ���λͼ��Ϣͷ */
	p_bitmap_context->bitmap_info_header.this_size = sizeof(struct BitmapInfoHeader) ;
	p_bitmap_context->bitmap_info_header.width = width ;
	p_bitmap_context->bitmap_info_header.height = height ;
	p_bitmap_context->bitmap_info_header.planes = planes ;
	p_bitmap_context->bitmap_info_header.bit_count = bit_count ;
	p_bitmap_context->bitmap_info_header.compression = compression ;
	p_bitmap_context->bitmap_info_header.size_image = (sizeof(struct RBGA)-sizeof(UINT8)) * width * height ;
	p_bitmap_context->bitmap_info_header.x_pels_per_meter = x_pels_per_meter ;
	p_bitmap_context->bitmap_info_header.y_pels_per_meter = y_pels_per_meter ;
	p_bitmap_context->bitmap_info_header.clr_used = 0 ;
	p_bitmap_context->bitmap_info_header.clr_important = 0 ;
	
	/* �����ڴ��Դ��λͼ���� */
	p_bitmap_context->bitmap_data.data.rbga_matrix = (struct RBGA *)malloc( sizeof(struct RBGA) * p_bitmap_context->bitmap_info_header.width * p_bitmap_context->bitmap_info_header.height ) ;
	if( p_bitmap_context->bitmap_data.data.rbga_matrix == NULL )
	{
		_libbmp_errno = LIBBMP_ERROR_MALLOC ;
		FreeBitmapContext( p_bitmap_context );
		return NULL;
	}
	
	for( y = 0 ; y < p_bitmap_context->bitmap_info_header.height ; y++ )
	{
		for( x = 0 ; x < p_bitmap_context->bitmap_info_header.width ; x++ )
		{
			memcpy( p_bitmap_context->bitmap_data.data.rbga_matrix+p_bitmap_context->bitmap_info_header.width*y+x , p_bg_rbga , sizeof(struct RBGA) );
		}
	}
	
	return p_bitmap_context;
}

/* �ͷ�λͼ������ */
void FreeBitmapContext( struct BitmapContext *p_bitmap_context )
{
	if( p_bitmap_context )
	{
		/* �ͷ�λͼ���� */
		if( p_bitmap_context->bitmap_data.data.rbga_matrix )
			free( p_bitmap_context->bitmap_data.data.rbga_matrix );
		
		/* �ͷ�λͼ������ */
		free( p_bitmap_context );
	}
	
	return;
}

/* �õ�λͼ�ļ�ͷ���� */
char *GetBitmapType( struct BitmapContext *p_bitmap_context )
{
	return (char*)&(p_bitmap_context->bitmap_file_header.type);
}

UINT32 GetBitmapSize( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_file_header.size;
}

UINT16 GetBitmapReserved1( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_file_header.reserved1;
}

UINT16 GetBitmapReserved2( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_file_header.reserved2;
}

UINT32 GetBitmapDataOffset( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_file_header.bitmap_data_offset;
}

/* �õ�λͼ��Ϣͷ���� */
INT32 GetBitmapWidth( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_info_header.width;
}

INT32 GetBitmapHeight( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_info_header.height;
}

UINT16 GetBitmapPlanes( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_info_header.planes;
}

UINT16 GetBitmapBitCount( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_info_header.bit_count;
}

UINT32 GetBitmapCompression( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_info_header.compression;
}

UINT32 GetBitmapSizeImage( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_info_header.size_image;
}

INT32 GetBitmapXPelsPerMeter( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_info_header.x_pels_per_meter;
}

INT32 GetBitmapYPelsPerMeter( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_info_header.y_pels_per_meter;
}

UINT32 GetBitmapClrUsed( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_info_header.clr_used;
}

UINT32 GetBitmapClrImportant( struct BitmapContext *p_bitmap_context )
{
	return p_bitmap_context->bitmap_info_header.clr_important;
}

/* ��λͼ���� */
int GetBitmapMatrixRbga( struct BitmapContext *p_bitmap_context , INT32 x , INT32 y , struct RBGA *p_rbga )
{
	struct RBGA	*p = NULL ;
	
	p = GetBitmapMatrixRbgaPtr( p_bitmap_context , x , y ) ;
	if( p == NULL )
		return -1;
	
	memcpy( p_rbga , p , sizeof(struct RBGA) );
	return 0;
}

struct RBGA *GetBitmapMatrixRbgaPtr( struct BitmapContext *p_bitmap_context , INT32 x , INT32 y )
{
	if( x >= p_bitmap_context->bitmap_info_header.width || y >= p_bitmap_context->bitmap_info_header.height )
	{
		_libbmp_errno = LIBBMP_ERROR_PARAMETER ;
		return NULL;
	}
	
	return p_bitmap_context->bitmap_data.data.rbga_matrix+y*p_bitmap_context->bitmap_info_header.width+x;
}

/* дλͼ���� */
int SetBitmapMatrixRbga( struct BitmapContext *p_bitmap_context , INT32 x , INT32 y , struct RBGA *p_rbga )
{
	if( x >= p_bitmap_context->bitmap_info_header.width || y >= p_bitmap_context->bitmap_info_header.height )
	{
		_libbmp_errno = LIBBMP_ERROR_PARAMETER ;
		return -1;
	}
	
	memcpy( p_bitmap_context->bitmap_data.data.rbga_matrix+y*p_bitmap_context->bitmap_info_header.width+x , p_rbga , sizeof(struct RBGA) );
	
	return 0;
}

/* �õ�ͷ��С */
UINT32 GetBitmapFileHeaderSize()
{
	return sizeof(struct BitmapFileHeader);
}

UINT32 GetBitmapInfoHeaderSize()
{
	return sizeof(struct BitmapInfoHeader);
}
