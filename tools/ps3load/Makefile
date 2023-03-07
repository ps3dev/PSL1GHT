#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	ps3load
BUILD		:=	build
SOURCES		:=	source
INCLUDES	:=	include
#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
DEBUGFLAGS	:=
CFLAGS	:=	$(DEBUGFLAGS) -Wall -O3\

CFLAGS	+=	$(INCLUDE)

LDFLAGS	=	$(DEBUGFLAGS)

UNAME	:=	$(shell uname -s)

ifeq ($(strip $(PS3DEV)),)
  ifeq ($(strip $(DEVKITPS3)),)
    export PS3DEV := /usr/local/ps3dev
  else
    export PS3DEV := $(DEVKITPS3)
  endif
endif

ifneq (,$(findstring MINGW,$(UNAME)))
	EXEEXT		:= .exe
	PLATFORM_LIBS	:=	-static -lws2_32
endif

ifneq (,$(findstring Darwin,$(shell uname -s)))
	SDK		:= $(shell xcrun --show-sdk-path)
	OSX_MIN 	:= $(shell defaults read $(shell xcrun --show-sdk-platform-path 2> /dev/null)/Info MinimumSDKVersion 2> /dev/null || xcrun --show-sdk-versin 2> /dev/null || echo 10.4)
	OSXCFLAGS	:= -mmacosx-version-min=$(OSX_MIN) -isysroot $(SDK)
	OSXCXXFLAGS	:=	$(OSXCFLAGS)
	CXXFLAGS	+=	-fvisibility=hidden
	LDFLAGS		+= -mmacosx-version-min=$(OSX_MIN) -Wl,-syslibroot,$(SDK)
endif

ifneq (,$(findstring SunOS,$(UNAME)))
	PLATFORM_LIBS	:= -lresolv -lsocket -lnsl
endif

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:=	$(PLATFORM_LIBS) -lz
#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=
#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------


export OUTPUT	:=	$(CURDIR)/$(TARGET)$(EXEEXT)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

export CC	:=	gcc
export CXX	:=	g++
export AR	:=	ar
export OBJCOPY	:=	objcopy

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES			:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

export OFILES	:= $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT)

#---------------------------------------------------------------------------------
install: $(BUILD)
	@echo Installing $(TARGET)$(EXEEXT)
	@[ -d $(DESTDIR)$(PS3DEV)/bin ] || mkdir -p $(DESTDIR)$(PS3DEV)/bin
	@install -m 755 $(OUTPUT) $(DESTDIR)$(PS3DEV)/bin

#---------------------------------------------------------------------------------
run:
	$(OUTPUT)

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)


#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT)	:	$(OFILES)
	@echo linking ... $(notdir $@)
	@$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

#---------------------------------------------------------------------------------
# Compile Targets for C/C++
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
%.o : %.cpp
	@echo $(notdir $<)
	@$(CXX) -E -MMD $(CFLAGS) $< > /dev/null
	@$(CXX) $(OSXCXXFLAGS) $(CFLAGS) -o $@ -c $<

#---------------------------------------------------------------------------------
%.o : %.c
	@echo $(notdir $<)
	@$(CC) -E -MMD $(CFLAGS) $< > /dev/null
	@$(CC) $(OSXCFLAGS) $(CFLAGS) -o $@ -c $<

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
