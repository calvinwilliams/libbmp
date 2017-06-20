#include "libbmp.h"

int test_ReadBitmapFile( char *bmp_pathfilename)
{
	struct BitmapContext	*p_bitmap_context = NULL ;
	UINT32			width , height ;
	UINT32			x , y ;
	struct RBGA		*p_rbga = NULL ;
	int			nret = 0 ;
	
	printf( "BITMAP FILE HEADER SIZE[%d]\n" , GetBitmapFileHeaderSize() );
	printf( "BITMAP INFO HEADER SIZE[%d]\n" , GetBitmapInfoHeaderSize() );
	
	p_bitmap_context = ReadBitmapFile( bmp_pathfilename ) ;
	if( p_bitmap_context == NULL )
	{
		printf( "ReadBitmapFile failed[%d]\n" , GetBitmapLastError() );
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
	
	width = GetBitmapWidth(p_bitmap_context) ;
	height = GetBitmapHeight(p_bitmap_context) ;
	printf( "--- BITMAP DATA --------- WIDTH[%u] HEIGHT[%u]\n" , width , height );
	for( y = 0 ; y < height ; y++ )
	{
		for( x = 0 ; x < width ; x++ )
		{
			p_rbga = GetBitmapMatrixRbgaPtr( p_bitmap_context , x , y ) ;
			if( p_rbga == NULL )
			{
				printf( "GetBitmapMatrixRbgPtr[%u][%u] failed[%d]\n" , x , y , nret );
				return -1;
			}
			printf( " %02X%02X%02X%02X" , p_rbga->blue , p_rbga->green , p_rbga->red , p_rbga->alpha );
		}
		printf( "\n" );
	}
	printf( "\n" );
	
	nret = WriteBitmapFile( p_bitmap_context , "OUTPUT.bmp" ) ;
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
	if( argc == 1 + 1 )
	{
		return -test_ReadBitmapFile( argv[1] );
	}
	else
	{
		printf( "USAGE : test_ReadBitmapFile .bmp\n" );
		return 7;
	}
}
