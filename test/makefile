# 此文件由makeobj.sh自动生成
############################################################
# 项目名 : 
# 模块名 : 
# 备  注 : 
############################################################

###### 源文件配置区
#@ c_FILE
c_FILE		=	\
			test_ReadBitmapFile.c \
			test_CreateBitmapFile.c \

###### 目标文件、安装目录配置区
include makeinstall
BIN		=	test_ReadBitmapFile test_CreateBitmapFile

###### 编译选项
CFLAGS		=	$(_CFLAGS) \
			-I$(HOME)/include/libbmp \

###### 链接选项
LFLAGS		=	$(_LFLAGS) \
			-I$(HOME)/lib \
			-lbmp \

###### 额外宏定义区
CLEAN_ADDITION	=

###### 加载mktpl模板库
#@ make_all
#@ make_clean
#@ make_install
#@ make_install_BININST
#@ make_install_DFTHDERINST
#@ make_uninstall
#@ make_uninstall_BININST
#@ make_uninstall_DFTHDERINST
include $(MKTPLDIR)/makeobj_$(MKTPLOS).inc

###### 目标文件依赖关系
test_ReadBitmapFile	:	test_ReadBitmapFile.o
	$(CC) -o $@ test_ReadBitmapFile.o $(LFLAGS)

test_CreateBitmapFile	:	test_CreateBitmapFile.o
	$(CC) -o $@ test_CreateBitmapFile.o $(LFLAGS)

