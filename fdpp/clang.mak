#
# CLANG.MAK - kernel copiler options for clang
#

CC = clang++
ifeq ($(CXX),)
CL = clang++
else
CL = $(CXX)
endif
CLC = clang
NASM = nasm
LINK = ld

TARGETOPT = -std=c++11 -c -fno-threadsafe-statics -fpic
# _XTRA should go at the end of cmd line
TARGETOPT_XTRA = -Wno-format-invalid-specifier

TARGET=fdppkrnl

DEBUG_MODE ?= 1
EXTRA_DEBUG ?= 0
DEBUG_MSGS ?= 1
USE_UBSAN ?= 0

IFLAGS = -iquote $(srcdir)/../hdr
CPPFLAGS = $(IFLAGS)
WFLAGS = -Wall
WCFLAGS = $(WFLAGS) -Wmissing-prototypes
ifeq ($(DEBUG_MODE),1)
DBGFLAGS += -ggdb3
endif
ifeq ($(EXTRA_DEBUG),1)
DBGFLAGS += -fdebug-macro -O0 -DFDPP_DEBUG -DEXTRA_DEBUG
NASMFLAGS += -DEXTRA_DEBUG
else
DBGFLAGS += -O2
endif
ifeq ($(DEBUG_MSGS),1)
DBGFLAGS += -DDEBUG
endif
ifeq ($(USE_UBSAN),1)
DBGFLAGS += -fsanitize=undefined -fno-sanitize=alignment
endif

CFLAGS = $(TARGETOPT) $(CPPFLAGS) $(WFLAGS) $(DBGFLAGS) $(TARGETOPT_XTRA)
CLCFLAGS = -c -fpic $(IFLAGS) $(WCFLAGS) $(DBGFLAGS)
LDFLAGS = -shared -Wl,-Bsymbolic -Wl,--build-id=sha1

ifeq ($(XFAT),32)
CPPFLAGS += -DWITHFAT32
NASMFLAGS += -DWITHFAT32
endif

NASMFLAGS := $(NASMFLAGS) -i$(srcdir)/../hdr/ -DXCPU=$(XCPU)
LOADSEG = 0x60
