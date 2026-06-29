[![License](https://img.shields.io/github/license/ps3dev/PSL1GHT.svg)](./LICENSE)
[![CI](https://github.com/ps3dev/PSL1GHT/actions/workflows/build.yml/badge.svg)](https://github.com/ps3dev/PSL1GHT/actions/workflows/build.yml)

# PSL1GHT

PSL1GHT is an open-source PlayStation 3 homebrew SDK. It provides libraries for
GameOS homebrew applications created using the PS3 toolchain.

> [!NOTE]
> This is the SDK repository.
> For the full development environment, please use
> [ps3dev](https://github.com/ps3dev/ps3dev).

## Environment

The master repository, [ps3dev](https://github.com/ps3dev/ps3dev), combines
the toolchain, libraries and PSL1GHT into a complete environment; this is the
recommended way for most users to get started with PSL1GHT.

PSL1GHT is built and tested with the toolchain provided by
[ps3toolchain](https://github.com/ps3dev/ps3toolchain). The toolchain supplies
compilers for the PPU and SPU, patched newlib environment, binutils, signing
tools and other programs required by PSL1GHT.

## Building

Building PSL1GHT directly is useful when developing PSL1GHT itself or testing changes.
A working PS3 toolchain must already be installed.

> [!NOTE]
> `make install` will overwrite the global PSL1GHT installation.
> If you do not want this, you may want to temporarily change $PSL1GHT to a separate location
> while working on a locally modified version.
>
> `make install` also installs parts into $PS3DEV; however, you cannot point $PS3DEV to an
> empty location, as the Makefiles will then fail to find the toolchain.

```sh
git clone https://github.com/ps3dev/PSL1GHT.git
cd PSL1GHT
make install-ctrl
make
make install
```

`make install-ctrl` installs the shared build rules required during the SDK build.
`make install` installs the completed headers, libraries, rules, and host utilities
into `$PSL1GHT` and `$PS3DEV`.

To remove generated build files:
```sh
make clean
```

## Building Applications

PSL1GHT programs are built using their Makefile. The repository currently provides
samples organised by subsystem, such as `audio`, `graphics` and `input`.

To build an individual sample:
```sh
cd samples/<category>/<sample>
make
```
A typical sample build produces `.elf` and `.self` outputs. Samples that define a package
target can be packaged with
```sh
make pkg
```

Package metadata is set by the application's Makefile. Quitting from the XMB requires the
application to register a callback to handle the event; an example can be found in the
camera sample.

## Optional Dependencies

### ps3libraries

Some samples and applications use libraries from
[ps3libraries](https://github.com/ps3dev/ps3libraries/). It is a collection of libraries
ported to the PS3 such as SDL and zlib.

### NVidia Cg Toolkit

The current shader workflow relies upon the discontinued proprietary NVidia Cg Toolkit
when compiling `.vcg` and `.fcg` shader sources. Cg Toolkit cannot be included in the
PS3DEV environment - it must be installed separately. It is not required for applications
that don't need shaders.

## Documentation

There is a [DeepWiki](https://deepwiki.com/ps3dev/PSL1GHT) set up for PSL1GHT that
contains information about the SDK and allows you to ask it questions. This can
provide a start for basic questions; the source code and public headers remain the
authority.

The public headers contain Doxygen documentation for many APIs. Generate it with:
```sh
make doc
```

Doxygen must be installed. 

## Current Status

The core PSL1GHT SDK is built in CI with the PS3 toolchain.
Coverage of samples is currently incomplete - some samples are excluded from the
top sample build script, and CI does not yet verify anything beyond a top-level 
`make`.

Contributions that improve compatibility, sample coverage and documentation are welcome.
For runtime changes, please describe the toolchain and hardware used for testing. 

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

Thanks to all [contributors](https://github.com/ps3dev/PSL1GHT/graphs/contributors) who have
helped to maintain and improve PSL1GHT over the years.
