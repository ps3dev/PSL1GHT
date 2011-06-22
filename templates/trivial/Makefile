#A trivial makefile for your PSL1GHT projects.
#Some variables have default values. These are commented
#out here. The not commented-out is the minimum set you need 
#to define yourself.
#
#
#Name of your project, binary and package. 
#Default - guess from current directory
#TARGET = 

#OFILES is what is linked into your binary
#the object files are made automatically from the corresponding c/cpp files
OFILES=source/main.o 

#The CFLAGS are not set by the framework makefiles. These are sound defaults.
CFLAGS=-I/opt/ps3dev/ppu/include/ -std=gnu99

LDFLAGS=

# Destination to place your binaries in
BUILDDIR=build
# If you want to use dependancy checking, define a directory for .d files
# DEPSDIR=



#Package specific options
#
# The title that appears in XMB. Do not quote.
#TITLE=Sample
#
# Icon that appears in XMB. Needs to be of certain properties.
#ICON=file.png
#
# ID of your application. Also the installation directory
# (/dev_hdd0/game/$APPID/)
#APPID = DEFAULT1

# Include psl1ght boilerplate makefiles
include $(PSL1GHT)/ppu_rules

# Targets to make.
#Installation package
pkg: $(TARGET).pkg

#Run with ps3load (requires supporting application on the ps3)
run:
	ps3load $(TARGET).self
clean:
	rm -rf $(TARGET).geohot.pkg $(TARGET).pkg $(BUILDDIR)/*
