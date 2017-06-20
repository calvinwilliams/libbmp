#include "libbmp.h"

int test_CreateBitmapFile()
{
	struct BitmapContext	*p_bitmap_context = NULL ;
	UINT32			width , height ;
	UINT32			x , y ;
	struct RBGA		bg_rbga = { 0xFF , 0xFF , 0xFF , 0x00 } ;
	struct RBGA		fg_rbga = { 0x00 , 0x00 , 0x00 , 0x00 } ;
	int			nret = 0 ;
	
	printf( "BITMAP FILE HEADER SIZE[%d]\n" , GetBitmapFileHeaderSize() );
	printf( "BITMAP INFO HEADER SIZE[%d]\n" , GetBitmapInfoHeaderSize() );
	
	/* 生成20*10位图文件 */
	width = 20 ;
	height = 10 ;
	p_bitmap_context = CreateBitmapContext( width , height , LIBBMP_PLANES_1 , LIBBMP_BITCOUNT_24 , LIBBMP_COMPRESSION_NONE , 0 , 0 , 0 , & bg_rbga ) ;
	if( p_bitmap_context == NULL )
	{
		printf( "CreateBitmapContext failed[%d]\n" , GetBitmapLastError() );
		return -1;
	}
	
	printf( "--- BITMAP FILE HEADER ---------\n" );
	printf( "            type[%.2s]\n" , (char*)GetBitmapType(p_bitmap_context) );
	printf( "            size[%u]\n" , GetBitmapSize(p_bitmap_context) );
	printf( "       reserved1[%u]\n" , GetBitmapReserved1(p_bitmap_context) );
	printf( "       reserved2[%u]\n" , GetBitmapReserved2(p_bitmap_context) );
	printf( "     data_offset[%u]\n" , GetBitmapDataOffset(p_bitmap_context) );
	printf( "\n" );
	
	printf( "--- BITMAP INFO HEADER ---------\n" );
	printf( "           width[%d]\n" , GetBitmapWidth(p_bitmap_context) );
	printf( "          height[%d]\n" , GetBitmapHeight(p_bitmap_context) );
	printf( "          planes[%u]\n" , GetBitmapPlanes(p_bitmap_context) );
	printf( "       bit_count[%u]\n" , GetBitmapBitCount(p_bitmap_context) );
	printf( "     compression[%u]\n" , GetBitmapCompression(p_bitmap_context) );
	printf( "      size_image[%u]\n" , GetBitmapSizeImage(p_bitmap_context) );
	printf( "x_pels_per_meter[%d]\n" , GetBitmapXPelsPerMeter(p_bitmap_context) );
	printf( "y_pels_per_meter[%d]\n" , GetBitmapYPelsPerMeter(p_bitmap_context) );
	printf( "        clr_used[%u]\n" , GetBitmapClrUsed(p_bitmap_context) );
	printf( "   clr_important[%u]\n" , GetBitmapClrImportant(p_bitmap_context) );
	printf( "\n" );
	
	y = 1 ;
	for( x = 1 ; x < width-1 ; x++ )
	{
		nret = SetBitmapMatrixRbga( p_bitmap_context , x , y , & fg_rbga ) ;
		if( nret == -1 )
		{
			printf( "SetBitmapMatrixRbga[%u][%u] failed[%d]\n" , x , y , nret );
			return -1;
		}
	}
	
	y = height - 1 - 1 ;
	for( x = 1 ; x < width-1 ; x++ )
	{
		nret = SetBitmapMatrixRbga( p_bitmap_context , x , y , & fg_rbga ) ;
		if( nret == -1 )
		{
			printf( "SetBitmapMatrixRbga[%u][%u] failed[%d]\n" , x , y , nret );
			return -1;
		}
	}
	
	nret = WriteBitmapFile( p_bitmap_context , "24BITS_W%d_H%d.bmp" , width , height ) ;
	if( nret == -1 )
	{
		printf( "WriteBitmapFile failed[%d]\n" , GetBitmapLastError() );
		return -1;
	}
	
	FreeBitmapContext( p_bitmap_context );
	
	/* 生成10*5位图文件 */
	width = 10 ;
	height = 5 ;
	p_bitmap_context = CreateBitmapContext( width , height , LIBBMP_PLANES_1 , LIBBMP_BITCOUNT_24 , LIBBMP_COMPRESSION_NONE , 0 , 0 , 0 , & bg_rbga ) ;
	if( p_bitmap_context == NULL )
	{
		printf( "CreateBitmapContext failed[%d]\n" , GetBitmapLastError() );
		return -1;
	}
	
	printf( "--- BITMAP FILE HEADER ---------\n" );
	printf( "            type[%.2s]\n" , (char*)GetBitmapType(p_bitmap_context) );
	printf( "            size[%u]\n" , GetBitmapSize(p_bitmap_context) );
	printf( "       reserved1[%u]\n" , GetBitmapReserved1(p_bitmap_context) );
	printf( "       reserved2[%u]\n" , GetBitmapReserved2(p_bitmap_context) );
	printf( "     data_offset[%u]\n" , GetBitmapDataOffset(p_bitmap_context) );
	printf( "\n" );
	
	printf( "--- BITMAP INFO HEADER ---------\n" );
	printf( "           width[%d]\n" , GetBitmapWidth(p_bitmap_context) );
	printf( "          height[%d]\n" , GetBitmapHeight(p_bitmap_context) );
	printf( "          planes[%u]\n" , GetBitmapPlanes(p_bitmap_context) );
	printf( "       bit_count[%u]\n" , GetBitmapBitCount(p_bitmap_context) );
	printf( "     compression[%u]\n" , GetBitmapCompression(p_bitmap_context) );
	printf( "      size_image[%u]\n" , GetBitmapSizeImage(p_bitmap_context) );
	printf( "x_pels_per_meter[%d]\n" , GetBitmapXPelsPerMeter(p_bitmap_context) );
	printf( "y_pels_per_meter[%d]\n" , GetBitmapYPelsPerMeter(p_bitmap_context) );
	printf( "        clr_used[%u]\n" , GetBitmapClrUsed(p_bitmap_context) );
	printf( "   clr_important[%u]\n" , GetBitmapClrImportant(p_bitmap_context) );
	printf( "\n" );
	
	y = 1 ;
	for( x = 1 ; x < width-1 ; x++ )
	{
		nret = SetBitmapMatrixRbga( p_bitmap_context , x , y , & fg_rbga ) ;
		if( nret == -1 )
		{
			printf( "SetBitmapMatrixRbga[%u][%u] failed[%d]\n" , x , y , nret );
			return -1;
		}
	}
	
	y = height - 1 - 1 ;
	for( x = 1 ; x < width-1 ; x++ )
	{
		nret = SetBitmapMatrixRbga( p_bitmap_context , x , y , & fg_rbga ) ;
		if( nret == -1 )
		{
			printf( "SetBitmapMatrixRbga[%u][%u] failed[%d]\n" , x , y , nret );
			return -1;
		}
	}
	
	nret = WriteBitmapFile( p_bitmap_context , "24BITS_W%d_H%d.bmp" , width , height ) ;
	if( nret == -1 )
	{
		printf( "WriteBitmapFile failed[%d]\n" , GetBitmapLastError() );
		return -1;
	}
	
	FreeBitmapContext( p_bitmap_context );
	
	return 0;
}

int main( int argc , char *argv[] )
{
	return -test_CreateBitmapFile();
}
