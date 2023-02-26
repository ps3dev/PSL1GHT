#!/usr/bin/env python3
from Struct import Struct
import struct
import getopt
import sys

"""
	This is a quick and dirty implementation of make_fself based on the
	documentation collected here:
		http://ps3wiki.lan.st/index.php/Self_file_format
	It's not ment to look pretty, or be well documented but just provide
	an alternative to using the illegal Sony SDK until a better solution
	is released. (Such as a propper ELF loader built into lv2)
					-- phiren
"""

class SelfHeader(Struct):
	__endian__ = Struct.BE
	def __format__(self):
		self.magic	= Struct.uint32
		self.headerVer  = Struct.uint32
		self.flags	= Struct.uint16
		self.type	= Struct.uint16
		self.meta	= Struct.uint32
		self.headerSize = Struct.uint64
		self.encryptedSize = Struct.uint64
		self.unknown	= Struct.uint64
		self.AppInfo	= Struct.uint64
		self.elf	= Struct.uint64
		self.phdr	= Struct.uint64
		self.shdr	= Struct.uint64
		self.phdrOffsets = Struct.uint64
		self.sceversion = Struct.uint64
		self.digest	= Struct.uint64
		self.digestSize = Struct.uint64

class AppInfo(Struct):
	__endian__ = Struct.BE
	def __format__(self):
		self.authid	= Struct.uint64
		self.unknown	= Struct.uint32
		self.appType	= Struct.uint32
		self.appVersion = Struct.uint64

class phdrOffset(Struct):
	__endian__ = Struct.BE
	def __format__(self):
		self.offset	= Struct.uint64
		self.size	= Struct.uint64
		self.unk1	= Struct.uint32
		self.unk2	= Struct.uint32
		self.unk3	= Struct.uint32
		self.unk4	= Struct.uint32

class DigestSubHeader(Struct):
	__endian__ = Struct.BE
	def __format__(self):
		self.type		= Struct.uint32
		self.size		= Struct.uint32
		self.cont		= Struct.uint64

class DigestType2(Struct):
	__endian__ = Struct.BE
	def __format__(self):
		self.magicBits		= Struct.uint8[0x14]
		self.digest		= Struct.uint8[0x14]
		self.padding		= Struct.uint8[0x08]

class DigestTypeNPDRM(Struct):
	__endian__ = Struct.BE
	def __format__(self):
		self.magic 		= Struct.uint32
		self.unk1 		= Struct.uint32
		self.drmType 		= Struct.uint32
		self.unk2		= Struct.uint32
		self.contentID 		= Struct.uint8[0x30]
		self.fileSHA1 		= Struct.uint8[0x10]
		self.notSHA1 		= Struct.uint8[0x10]
		self.notXORKLSHA1 	= Struct.uint8[0x10]

class Elf64_ehdr(Struct):
	__endian__ = Struct.BE
	def __format__(self):
		self.ident 		= Struct.uint8[16]
		self.type		= Struct.uint16
		self.machine		= Struct.uint16
		self.version		= Struct.uint32
		self.entry		= Struct.uint64
		self.phoff		= Struct.uint64
		self.shoff		= Struct.uint64
		self.flags		= Struct.uint32
		self.ehsize		= Struct.uint16
		self.phentsize		= Struct.uint16
		self.phnum		= Struct.uint16
		self.shentsize		= Struct.uint16
		self.shnum		= Struct.uint16
		self.shstrndx		= Struct.uint16

class Elf64_phdr(Struct):
	__endian__ = Struct.BE
	def __format__(self):
		self.type	= Struct.uint32
		self.flags	= Struct.uint32
		self.offset	= Struct.uint64
		self.vaddr	= Struct.uint64
		self.paddr	= Struct.uint64
		self.filesz	= Struct.uint64
		self.memsz	= Struct.uint64
		self.align	= Struct.uint64

def align(address, alignment):
	padding = alignment - (address % alignment)
	return address + padding

def padding(address, alignment):
	padding = alignment - (address % alignment)
	return b"\0" * padding

def readElf(infile):
	with open(infile, 'rb') as fp:
		data = fp.read()
		ehdr = Elf64_ehdr()
		ehdr.unpack(data[0:len(ehdr)])
		phdrs = []
		offset = ehdr.phoff
		for i in range(ehdr.phnum):
			phdr = Elf64_phdr()
			phdr.unpack(data[offset:offset+len(phdr)])
			offset += len(phdr)
			phdrs.append(phdr)
		
		return data, ehdr, phdrs

def genDigest(out, npdrm):
	digestSubHeader = DigestSubHeader()
	digestType2 = DigestType2()
	digestTypeNPDRM = DigestTypeNPDRM()

	digestSubHeader.type = 2
	digestSubHeader.size = 0x40
	if npdrm:
		digestSubHeader.cont = 1
	out.write(digestSubHeader.pack())

	digestType2.magicBits = (0x62, 0x7c, 0xb1, 0x80, 0x8a, 0xb9, 0x38, 0xe3, 0x2c, 0x8c, 0x09, 0x17, 0x08, 0x72, 0x6a, 0x57, 0x9e, 0x25, 0x86, 0xe4)
	out.write(digestType2.pack())

	if not npdrm:
		return

	digestSubHeader.type = 3
	digestSubHeader.size = 0x90
	digestSubHeader.cont = 0
	out.write(digestSubHeader.pack())

	digestTypeNPDRM.magic = 0x4e504400
	digestTypeNPDRM.unk1 = 1
	digestTypeNPDRM.drmType = 2
	digestTypeNPDRM.unk2 = 1
	digestTypeNPDRM.contentID = [0x30] * 0x2f + [0]
	digestTypeNPDRM.fileSHA1 = (0x42, 0x69, 0x74, 0x65, 0x20, 0x4d, 0x65, 0x2c, 0x20, 0x53, 0x6f, 0x6e, 0x79, 0x00, 0xde, 0x07)
	digestTypeNPDRM.notSHA1 = [0xab] * 0x10
	digestTypeNPDRM.notXORKLSHA1 = [0x01] * 0x0f + [0x02]
	out.write(digestTypeNPDRM.pack())


def createFself(npdrm, infile, outfile="EBOOT.BIN"):
	elf, ehdr, phdrs = readElf(infile)
	
	header = SelfHeader()
	appinfo = AppInfo()
	digestSubHeader = DigestSubHeader()
	digestType2 = DigestType2()
	digestTypeNPDRM = DigestTypeNPDRM()
	phdr = Elf64_phdr()
	phdrOffsets = phdrOffset()

	header.magic = 0x53434500
	header.headerVer = 2
	header.flags = 0x8000
	header.type = 1
	header.encryptedSize = len(elf)
	header.unknown = 3
	header.AppInfo = align(len(header), 0x10)
	header.elf = align(header.AppInfo + len(appinfo), 0x10)
	header.phdr = header.elf + len(ehdr)
	phdrOffsetsOffset = header.phdr + len(phdr) * len(phdrs)
	header.phdrOffsets = align(phdrOffsetsOffset, 0x10);

	header.sceVersion = 0
	
	digestOffset = header.phdrOffsets + len(phdrs) * len(phdrOffsets)
	header.digest = align(digestOffset, 0x10)
	header.digestSize = len(digestSubHeader) + len(digestType2
)
	if npdrm:
		header.digestSize += len(digestSubHeader) + len(digestTypeNPDRM)

	endofHeader = header.digest + header.digestSize
	elfOffset = align(endofHeader, 0x80)

	header.shdr = elfOffset + ehdr.shoff
	header.headerSize = elfOffset
	header.meta = endofHeader - 0x10

	appinfo.authid = 0x1010000001000003
	appinfo.unknown = 0x1000002
	if npdrm:
		appinfo.appType = 0x8
	else:
		appinfo.appType = 0x4
	appinfo.appVersion = 0x0001000000000000

	offsets = []
	for phdr in phdrs:
		offset = phdrOffset()
		offset.offset = phdr.offset + elfOffset
		offset.size = phdr.filesz
		offset.unk1 = 1
		offset.unk2 = 0
		offset.unk3 = 0
		if phdr.type == 1:
			offset.unk4 = 2
		else:
			offset.unk4 = 0
		offsets.append(offset)
	out = open(outfile, 'wb')
	out.write(header.pack())
	out.write(padding(len(header), 0x10))
	out.write(appinfo.pack())
	out.write(padding(header.AppInfo + len(appinfo), 0x10))
	out.write(ehdr.pack())
	for phdr in phdrs:
		out.write(phdr.pack())
	out.write(padding(phdrOffsetsOffset, 0x10))
	for offset in offsets:
		out.write(offset.pack())
	out.write(padding(digestOffset, 0x10))
	genDigest(out, npdrm)
	out.write(padding(endofHeader, 0x80))
	out.write(elf)


def usage():
	print("""fself.py usage:
	fself.py [options] input.elf output.self
	If output file is not specified, fself.py will default to EBOOT.BIN
	Options:
		--npdrm: will output a file for use with pkg.py.""")
def main():
	try:
		opts, args = getopt.getopt(sys.argv[1:], "hn", ["help", "npdrm"])
	except getopt.GetoptError:
		usage()
		sys.exit(2)
	npdrm = False
	for opt, arg in opts:
		if opt in ("-h", "--help"):
			usage()
			sys.exit(2)
		elif opt in ("-n", "--npdrm"):
			npdrm = True
		else:
			usage()
			
	if len(args) == 1:
		createFself(npdrm, args[0])
	elif len(args) == 2:
		createFself(npdrm, args[0], args[1])
	else:
		usage()
if __name__ == "__main__":
	main()
	
