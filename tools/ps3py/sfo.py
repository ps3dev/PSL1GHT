#!/usr/bin/env python
from __future__ import with_statement
from xml.dom.minidom import Document, parse, parseString
from Struct import Struct
import struct
import os
import sys
import argparse
debug = None
pretty = None

SFO_MAGIC  = 0x46535000
SFO_STRING = 2
SFO_INT    = 4
def nullterm(str_plus):
	z = str_plus.find('\0')
	if z != -1:
		return str_plus[:z]
	else:
		return str_plus

class MyParser(argparse.ArgumentParser):
	def error(self, message):
		print message
		print "Try \'" + os.path.basename(sys.argv[0]) + " --help\' for more information."
		sys.exit(2)

class Header(Struct):
	__endian__ = Struct.LE
	def __format__(self):
		self.magic = Struct.uint32				# Magic number (Always PSF)
		self.version = Struct.uint32				# Version (Usually 1.1)
		self.KeyOffset = Struct.uint32				# key_table_start (Start offset of key_table)
		self.ValueOffset = Struct.uint32			# data_table_start (Start offset of data_table)
		self.PairCount = Struct.uint32				# tables_entries (Number of entries in all tables)
	def __str__(self):
		out  = ""
		out += "[X] Magic: %08xh\n" % self.magic
		out += "[X] SFO Version: %08xh\n" % self.version
		out += "[X] Key Offset: %08xh\n" % self.KeyOffset
		out += "[X] Value Offset: %08xh\n" % self.ValueOffset
		out += "[X] Pair Count: %08xh\n" % self.PairCount
		return out

class Entry(Struct):
	__endian__ = Struct.LE
	def __format__(self):
		self.key_off   = Struct.uint16
		self.unk1      = Struct.uint8
		self.value_type      = Struct.uint8
		self.value_len      = Struct.uint32
		self.padded_len   = Struct.uint32
		self.value_off = Struct.uint32
	def __str__(self):
		out  = ""
		out += "[X] Key Offset: %04xh\n" % self.key_off
		out += "[ ] Unk1: %02xh\n" % self.unk1
		out += "[/] Value Type: %02xh\n" % self.value_type
		out += "[X] Value Length: %08xh\n" % self.value_len
		out += "[X] Padded Length: %08xh\n" % self.padded_len
		out += "[X] Value Offset: %08xh" % self.value_off
		return out
	def PrettyPrint(self, data, key_off, value_off):
		out  = ""
		out += "[X] Key: '%s'[%04xh]\n" % (nullterm(data[self.key_off + key_off:]), self.key_off)
		out += "[/] Unk: %02xh\n" % (self.unk1)
		out += "[/] Value Type: %02xh\n" % self.value_type
		out += "[X] Value Length: %08xh\n" % self.value_len
		out += "[X] Padded Length: %08xh\n" % self.padded_len
		out += "[X] Value Offset: %08xh" % self.value_off
		if self.value_type == SFO_STRING:
			out += "\n[X] Value: '%s'[%08xh]" % (nullterm(data[self.value_off + value_off:]), self.value_off+value_off)
		elif self.value_type == SFO_INT:
			out += "\n[X] Value: %d[%08xh]" % (struct.unpack('<I', data[self.value_off + value_off:self.value_off + value_off + 4])[0], self.value_off+value_off)
		else:
			out += "\n[X] Value Type Unknown"
		return out
	
def usage():
	print "usage:\n    " + os.path.basename(sys.argv[0]) + """ [OPTION]...

A tool that does various things with a PS3 SFO file.

Mandatory arguments to long options are mandatory for short options too
  -l, --list <SFO>		list the contents of the PS3 SFO file <SFO file>.
  -p, --pretty			when used with --list and --debug, display the key
				name and value.
				when used with --list, display formated text from
				the SFO file on the screen.
  -d, --debug			display extra information such as header and table data.
  -t, --toxml <SFO> <XML>	convert a SFO file to an XML file.
  -f, --fromxml <XML> <SFO>	convert an XML file to a SFO file.
      --title <TITLE>		when converting from an XML file, use the title <title>
				instead of the title from the XML file.
      --appid <APPID>		when converting from an XML file, use the appid <appid>
				instead of the appid from the XML file.

  -h, --help			display this help and exit.
  -v, --version 		output version information and exit."""
	sys.exit(2)

def version():
	print os.path.basename(sys.argv[0]) + """ 0.3"""
	
def listSFO(file):
	global debug
	global pretty
	with open(file, 'rb') as fp:
		stuff = {}
		data = fp.read()
		offset = 0
		header = Header()
		header.unpack(data[offset:offset+len(header)])
		if debug:
			print header
			print
		if header.magic != SFO_MAGIC:
			print "Error:  SFO MAGIC does not equal PSF (%xh).  Are you sure this is a SFO file?" % SFO_MAGIC
			sys.exit(2)
		if header.version != 0x00000101:
			print "Error:  Found header version: (%08xh), but we where expecting header version (00000101h)." % header.version
			print "\tPlease submit an issue at https://github.com/Spork-Schivago/PSL1GHT"
			sys.exit(2)
		offset += len(header)
		off1 = header.KeyOffset
		off2 = header.ValueOffset
		for x in xrange(header.PairCount):
			entry = Entry()
			entry.unpack(data[offset:offset+len(entry)])
			if debug and not pretty:
				print entry
				print
			if debug and pretty:
				print entry.PrettyPrint(data, off1, off2)
				print
			key = nullterm(data[off1+entry.key_off:])
			if entry.value_type == SFO_STRING:
				value = nullterm(data[off2+entry.value_off:])
			else:
				value = struct.unpack('<I', data[entry.value_off + off2:entry.value_off + off2 + 4])[0]
			stuff[key] = value
			offset += len(entry)
		if not debug and pretty:
			for k,v in stuff.items():
				print '%s : %s' % (k, v)
				print '========================'
		if not debug and not pretty:
			print stuff

def convertToXml(sfofile, xml):
	doc = Document()
	sfo = doc.createElement("sfo")


	with open(sfofile, 'rb') as fp:
		stuff = {}
		data = fp.read()
		offset = 0
		header = Header()
		header.unpack(data[offset:offset+len(header)])
		if debug:
			print header
			print
		if header.magic != SFO_MAGIC:
			print "Error:  SFO MAGIC does not equal PSF (%xh).  Are you sure this is a SFO file?" % SFO_MAGIC
			sys.exit(2)
		if header.version != 0x00000101:
			print "Error:  Found header version: (%08xh), but we where expecting header version (00000101h)." % header.version
			print "\tPlease submit an issue at https://github.com/Spork-Schivago/PSL1GHT"
			sys.exit(2)
		offset += len(header)
		off1 = header.KeyOffset
		off2 = header.ValueOffset
		for x in xrange(header.PairCount):
			entry = Entry()
			entry.unpack(data[offset:offset+len(entry)])
			if debug and not pretty:
				print entry
				print
			if debug and pretty:
				print entry.PrettyPrint(data, off1, off2)
				print
			key = nullterm(data[off1+entry.key_off:])
			valuenode = doc.createElement("value")
			valuenode.setAttribute("name", key)
			if entry.value_type == SFO_STRING:
				value = nullterm(data[off2+entry.value_off:])
				valuenode.setAttribute("type", "string")
				valuenode.appendChild(doc.createTextNode(value))
			else:
				value = struct.unpack('<I', data[entry.value_off + off2:entry.value_off + off2 + 4])[0]
				valuenode.setAttribute("type", "integer")
				valuenode.appendChild(doc.createTextNode("%d" % value))
			sfo.appendChild(valuenode)
			stuff[key] = value
			offset += len(entry)
		if not debug and pretty:
			for k,v in stuff.items():
				print '%s : %s' % (k, v)
				print '========================'
		if not debug and not pretty:
			print stuff

	doc.appendChild(sfo)
	file = open(xml, "wb" )
	doc.writexml(file, '', '\t', '\n' )
	file.close()
	
def getText(nodelist):
    rc = []
    for node in nodelist:
        if node.nodeType == node.TEXT_NODE:
            rc.append(node.data)
    return (''.join(rc)).strip()
def align(num, alignment):
	return (num + alignment - 1) & ~(alignment-1)
def convertToSFO(xml, sfofile, forcetitle, forceappid):
	dom = parse(xml)
	nodes = dom.getElementsByTagName("value")
	kvs = []
	for node in nodes:
		if node.hasAttributes():
			type = None
			name = None
			for i in range(node.attributes.length):
				if(node.attributes.item(i).name == "type"):
					type = node.attributes.item(i).value
				if(node.attributes.item(i).name == "name"):
					name = node.attributes.item(i).value
			if name != None and type != None:
				if name == "TITLE" and forcetitle != None:
					kvs.append((name, forcetitle))
				elif name == "TITLE_ID" and forceappid != None:
					kvs.append((name, forceappid))
				elif type == "string":
					kvs.append((name, getText(node.childNodes)))
				elif type == "integer":
					kvs.append((name, int(getText(node.childNodes))))
	header = Header()
	header.magic = SFO_MAGIC
	header.version = 0x00000101
	header.PairCount = len(kvs)
	entries = []
	keyoff = 0
	valueoff = 0
	for (k,v) in kvs:
		entry = Entry()
		entry.key_off   = keyoff
		entry.unk1      = 4
		if isinstance(v, int): 
			entry.value_type = SFO_INT
			entry.value_len  = 4
			entry.padded_len = 4
		else: 
			entry.value_type = SFO_STRING
			entry.value_len  = len(v) + 1
			alignment = 4
			if k == "TITLE":
				alignment = 0x80
			elif k == "LICENSE":
				alignment = 0x200
			elif k == "TITLE_ID":
				alignment = 0x10

			entry.padded_len = align(entry.value_len, alignment) 
		entry.value_off = valueoff
		keyoff += len(k)+1
		valueoff += entry.padded_len
		entries.append(entry)
	header.KeyOffset = len(Header()) + 0x10 * header.PairCount
	header.ValueOffset = align(header.KeyOffset + keyoff, 4)
	keypad = header.ValueOffset - (header.KeyOffset + keyoff)
	valuepad = header.ValueOffset - (header.KeyOffset + keyoff)
	file = open(sfofile, "wb")
	file.write(header.pack())
	for entry in entries:
		file.write(entry.pack())
	for k,v in kvs:
		file.write(k + '\0')
	file.write('\0' * keypad)
	for k,v in kvs:
		if isinstance(v, int):
			file.write(struct.pack('<I', v))
		else:
			alignment = 4
			if k == "TITLE":
				alignment = 0x80
			elif k == "LICENSE":
				alignment = 0x200
			elif k == "TITLE_ID":
				alignment = 0x10
			file.write(v + '\0')
			file.write('\0' * (align(len(v) + 1, alignment) - (len(v) +1)))
	file.close()
def main():
	global debug
	global pretty
	debug = False
	pretty = False
	SFO_file = None
	XML_file = None
	forcetitle = None
	forceappid = None

	parser = MyParser(add_help=False)
	parser.add_argument('-d','--debug', action="store_true",required=False)
	parser.add_argument('-p','--pretty',action="store_true",required=False)
	group = parser.add_mutually_exclusive_group()
	group.add_argument('-l','--list',action="store",nargs=1,required=False)
	group.add_argument('-t','--toxml',nargs=2,required=False)
	group.add_argument('-f','--fromxml',nargs=2,required=False)
	parser.add_argument('--title',nargs=1,required=False)
	parser.add_argument('--appid',nargs=1,required=False)
	parser.add_argument('-v','--version',action="store_true",required=False)
	parser.add_argument('-h','--help',action="store_true",default=None)

	if len(sys.argv)==1:
		usage()
		sys.exit(2)
	args = parser.parse_args()

	if args.help:
		usage()
	if args.version:
		version()
		sys.exit(2)
	if args.debug:
		debug = getattr(args, 'debug')
	if args.pretty:
		pretty = getattr(args, 'pretty')
	if args.title:
		forcetitle = args.title[0]
		print "Forcing title %s" % forcetitle
	if args.appid:
		forceappid = args.appid[0]
		print "Forcing appid %s" % forceappid
	if args.list:
		SFO_file = args.list[0]
		listSFO(SFO_file)
	if args.toxml:
		SFO_file = args.toxml[0]
		XML_file = args.toxml[1]
		convertToXml(SFO_file,XML_file)
	if args.fromxml:
		XML_file = args.fromxml[0]
		SFO_file = args.fromxml[1]
		convertToSFO(XML_file, SFO_file, forcetitle, forceappid)
	if args.version:
		print version()

if __name__ == "__main__":
	main()
