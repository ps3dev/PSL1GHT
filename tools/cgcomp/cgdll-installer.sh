#!/bin/sh -e
# NVIDIA cg.dll installer script by CrystalCT (crystal@unict.it)

## Uname string
UNAME=$(uname -a)

 
	## Download a fresh cg.dll
	rm -f cg.zip;
	rm -f cg.dll;
	if [[ $UNAME =~ "x86_64" || $UNAME =~ "MINGW64" ]];
	then
		wget --continue --no-check-certificate https://wikidll.com/download/3726/cg.zip;
	else
		wget --continue --no-check-certificate https://wikidll.com/download/3730/cg.zip;
	fi;

	## Install cg.dll
	echo "Installing cg.dll in $PS3DEV/bin"
	unzip cg.zip
	rm -f cg.zip
	CGHASH=$(rhash -C cg.dll -p "%{crc32}")
	## CRC HASH values taken from dll inside original NVIDIA CG Tool Kit. Dll can't be downloaded directly from NIVIDA web site.
	if [[ $CGHASH =~ "28f6073b" || $CGHASH =~ "e5228ec2" ]];
	then
		chmod 766 cg.dll
		mv -f cg.dll $PS3DEV/bin;
	else
		echo "Error - cg.dll: wrong CRC32";
	fi
