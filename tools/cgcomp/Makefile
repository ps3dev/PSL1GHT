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

CFLAGS	:=	$(DEBUGFLAGS) -Wall -O2
CFLAGS	+=	$(INCLUDES)
CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions

LDFLAGS	=	$(DEBUGFLAGS)

ifeq ($(strip $(PS3DEV)),)
  ifeq ($(strip $(DEVKITPS3)),)
    export PS3DEV := /usr/local/ps3dev
  else
    export PS3DEV := $(DEVKITPS3)
  endif
endif

ifneq (,$(findstring MINGW,$(UNAME)))
	PLATFORM	:= win32
	EXEEXT		:= .exe
	CFLAGS		+= -mno-cygwin
	LDFLAGS		+= -mno-cygwin -s
	OS	:=	win32
endif

ifneq (,$(findstring CYGWIN,$(UNAME)))
	LDFLAGS		+= -s
	EXEEXT		:= .exe
	OS	:=	win32
endif

ifneq (,$(findstring Darwin,$(UNAME)))
	SDK			:= /Developer/SDKs/MacOSX10.4u.sdk 
	OSXCFLAGS	:= -mmacosx-version-min=10.4 -arch i386
	OSXCXXFLAGS	:=	$(OSXCFLAGS)
	CXXFLAGS	+=	-fvisibility=hidden
	LDFLAGS += -mmacosx-version-min=10.4 -arch i386 -Wl,-syslibroot,$(SDK)
endif

ifneq (,$(findstring Linux,$(UNAME)))
	LDFLAGS += -ldl -lpthread
	OS := Linux
endif

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
	@[ -d $(PS3DEV)/bin ] || mkdir -p $(PS3DEV)/bin
	@install -m 755  $(OUTPUT) $(PS3DEV)/bin

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT): $(DFILES) $(OFILES)
	@echo linking ... $(notdir $@)
	@$(LD) $(LDFLAGS) $^ $(LIBPATHS) $(LIBS) -o $@

#---------------------------------------------------------------------------------
# Compile Targets for C/C++
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
%.o : %.cpp
	@echo $(notdir $<)
	@$(CXX) -E -MMD $(CXXFLAGS) $< > /dev/null
	@$(CXX) $(OSXCXXFLAGS) $(CXXFLAGS) -o $@ -c $<

#---------------------------------------------------------------------------------
%.o : %.c
	@echo $(notdir $<)
	@$(CC) -E -MMD $(CFLAGS) $< > /dev/null
	@$(CC) $(OSXCFLAGS) $(CFLAGS) -o $@ -c $<



-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
