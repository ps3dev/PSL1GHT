[![License](https://img.shields.io/github/license/ps3dev/PSL1GHT.svg)](./LICENSE)
[![CI](https://github.com/ps3dev/PSL1GHT/actions/workflows/build.yml/badge.svg)](https://github.com/ps3dev/PSL1GHT/actions/workflows/build.yml)

# PSL1GHT

PSL1GHT is an open-source PlayStation 3 homebrew SDK. It uses the PS3
toolchain to compile applications for GameOS as SELF executables or
PKG packages.

> [!NOTE]
> This is the SDK repository.
> For the full development environment, please use
> [ps3dev](https://github.com/ps3dev/ps3dev).

## Environment

The master repository, [ps3dev](https://github.com/ps3dev/ps3dev), combines
the toolchain, libraries and PSL1GHT into a complete environment; this is the
recommended way for most users to get started with PSL1GHT.

PSL1GHT is built and tested with the toolchain provided by
[ps3toolchain](http://github.com/ps3dev/ps3toolchain). The toolchain supplies
compilers for the PPU and SPU, patched newlib environment, binutils, signing
tools and other programs required by PSL1GHT.

Nvidia's [Cg Toolkit](http://developer.nvidia.com/object/cg_toolkit.html)
is required for compiling vertex programs. The signing tools require libgmp.
Most PSL1GHT samples require various libraries from
[ps3libraries](http://github.com/ps3dev/ps3libraries) to be installed.


## Building

This is useful when developing PSL1GHT itself or testing changes. A working
PS3 toolchain must already  be installed.

```
git clone https://github.com/ps3dev/PSL1GHT.git
cd PSL1GHT
make install-ctrl
make
make install
```

make install-ctrl installs the shared build rules required during the SDK build.
make install installs the completed headers, libraries, rules, and host utilities
into $PSL1GHT and $PS3DEV.

To remove generated build files:
```
make clean
```

## Building Applications

PSL1GHT programs are built using their Makefile. The repository currently provides
samples organised by sub-system, such as `audio`, `graphics` and `input`.

To build an individual sample:
```
cd samples/<category>/<sample>
make
```
Depending on the sample, `.elf`, `.self` and/or `.pkg` files may be generated. Samples
that provide a package target can be packaged with `make pkg`. Package metadata is set
by the application's Makefile.

## Optional Dependencies

### ps3libraries

Though not required, many samples and applications use libraries from
[ps3libraries](https://github.com/ps3dev/ps3libraries/). It is a collection of libraries
ported to the PS3 such as SDL and zlib.

### NVidia CgToolkit

The current shader workflow relies upon the NVidia Cg Toolkit when compiling `.vcg`
and `.fcg` shader sources. Cg is proprietary and discontinued and cannot be included
in the PS3DEV environment - it must be installed separately. It is not required for
applications that don't need shaders.

## Documentation

There is a [DeepWiki](https://deepwiki.com/ps3dev/PSL1GHT) set up for PSL1GHT that
contains information about the SDK and allows you to ask it questions. 

The public headers contain Doxygen documentation for many APIs. Generate it with:
```
make doc
```

Doxygen must be installed. 

## Current Status

PSL1GHT is mostly historical code that is known to have problems building on modern
toolchains. CI needs to be expanded and made more robust to test the veracity of 
PSL1GHT's samples.

Contributions are welcome. Before submitting a pull request, please verify the code
is hardware-compatible and includes a description so that it can be replicated.

## Credits


    AerialX     - Founder, Author
    Parlane     - Author
    phiren      - Author
    Tempus      - PSL1GHT Logo
    lousyphreak - libaudio
    Hermes      - sysmodule, libpngdec, libjpgdec
    BigBoss     - EyeToy support added to libcamera sample, libgem sample.
    ooPo        - ps3libraries
    ElSemi      - Vertex Program Compiler
    zerkman     - SPU sample code
    shagkur     - Author
    miigotu     - Author
