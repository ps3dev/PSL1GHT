#---------------------------------------------------------------------------------
# TARGET is the name of the output, if this ends with _mb generates a multiboot image
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	cgcomp
BUILD		:=	build
SOURCES		:=	source
INCLUDE		:=	include
LIBDIRS		:=

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------

UNAME	:=	$(shell uname -s)

ifneq ($(strip $(DEBUGFLAGS)),)
	CFLAGS	:=	$(DEBUGFLAGS) -Wall
else
	CFLAGS	:=	-Wall -O2
endif

CFLAGS		+=	$(INCLUDES)
CXXFLAGS	:=	$(CFLAGS)
CGTOOLKIT	:= 	echo Linux and Mac users should install Cg Toolkit package from NVIDIA web site.

LDFLAGS	=	$(DEBUGFLAGS)

ifeq ($(strip $(PS3DEV)),)
  ifeq ($(strip $(DEVKITPS3)),)
    export PS3DEV := /usr/local/ps3dev
  else
    export PS3DEV := $(DEVKITPS3)
  endif
endif

# Allow for 'make VERBOSE=1' to see the recepie executions
ifndef VERBOSE
  VERB := @
endif

ifneq (,$(findstring MINGW,$(UNAME)))
	PLATFORM	:= win32
	EXEEXT		:= .exe
	CFLAGS		+= -mno-cygwin
	LDFLAGS		+= -mno-cygwin -s
	OS	:=	win32
	CGTOOLKIT	:= chmod 755 ./cgdll-installer.sh; ./cgdll-installer.sh
endif

ifneq (,$(findstring CYGWIN,$(UNAME)))
	LDFLAGS		+= -s
	EXEEXT		:= .exe
	CXXFLAGS	+= -DWIN32
	OS	:=	win32
	CGTOOLKIT	:= chmod 755 ./cgdll-installer.sh; ./cgdll-installer.sh
endif

ifneq (,$(findstring Darwin,$(UNAME)))
	SDK		:= $(shell xcrun --show-sdk-path)
	OSX_MIN		:= $(shell defaults read $(shell xcrun --show-sdk-platform-path 2> /dev/null)/Info MinimumSDKVersion 2> /dev/null || xcrun --show-sdk-version 2> /dev/null || echo 10.4)
	OSXCFLAGS	:= -mmacosx-version-min=$(OSX_MIN)
	OSXCXXFLAGS	:= $(OSXCFLAGS)
	CXXFLAGS	+= -fvisibility=hidden
	LDFLAGS += -mmacosx-version-min=$(OSX_MIN) -Wl,-syslibroot,$(SDK)
endif

ifneq (,$(findstring Linux,$(UNAME)))
	LDFLAGS += -ldl -lpthread
	OS := Linux
endif

CXXFLAGS        += -std=c++11

LIBS		:=

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

export CC	:=	$(PREFIX)gcc
export CXX	:=	$(PREFIX)g++
export AR	:=	$(PREFIX)ar
export OBJCOPY	:=	$(PREFIX)objcopy
#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
export LD	:=	$(CXX)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))

export OFILES	:= $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(sFILES:.s=.o) $(SFILES:.S=.o)
#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDES	:=	$(foreach dir,$(INCLUDE),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(CURDIR)/$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(CURDIR)/$(dir)/lib)

export OUTPUT	:=	$(CURDIR)/$(TARGET)$(EXEEXT)
.PHONY: $(BUILD) clean

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT)

install: $(BUILD)
	@echo Installing $(TARGET)$(EXEEXT)
	@[ -d $(DESTDIR)$(PS3DEV)/bin ] || mkdir -p $(DESTDIR)$(PS3DEV)/bin
	@install -m 755  $(OUTPUT) $(DESTDIR)$(PS3DEV)/bin
	@$(CGTOOLKIT)
#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT): $(DFILES) $(OFILES)
	$(VERB) echo linking ... $(notdir $@)
	$(VERB) $(LD) $^ $(LDFLAGS) $(LIBPATHS) $(LIBS) -o $@

#---------------------------------------------------------------------------------
# Compile Targets for C/C++
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
%.o : %.cpp
	$(VERB) echo $(notdir $<)
	$(VERB) $(CXX) -E -MMD $(CXXFLAGS) $< > /dev/null
	$(VERB) $(CXX) $(OSXCXXFLAGS) $(CXXFLAGS) -o $@ -c $<

#---------------------------------------------------------------------------------
%.o : %.c
	$(VERB) echo $(notdir $<)
	$(VERB) $(CC) -E -MMD $(CFLAGS) $< > /dev/null
	$(VERB) $(CC) $(OSXCFLAGS) $(CFLAGS) -o $@ -c $<



-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
