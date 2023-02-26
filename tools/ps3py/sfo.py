#!/usr/bin/env python3
from xml.dom.minidom import Document, parse, parseString
from Struct import Struct
import struct
import sys
import getopt
debug = None
pretty = None

SFO_MAGIC  = 0x46535000
SFO_STRING = 2
SFO_INT    = 4
def nullterm(str_plus):
	z = str_plus.find(b'\0')
	if z != -1:
		return str_plus[:z].decode('ascii')
	else:
		return str_plus.decode('ascii')

class Header(Struct):
	__endian__ = Struct.LE
	def __format__(self):
		self.magic = Struct.uint32
		self.unk1 = Struct.uint32
		self.KeyOffset = Struct.uint32
		self.ValueOffset = Struct.uint32
		self.PairCount = Struct.uint32
	def __str__(self):
		out  = ""
		out += "[X] Magic: %08x\n" % self.magic
		out += "[ ] Unk1: %08x\n" % self.unk1
		out += "[X] Key Offset: %08x\n" % self.KeyOffset
		out += "[X] Value Offset: %08x\n" % self.ValueOffset
		out += "[X] Pair Count: %08x" % self.PairCount
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
		out += "[X] Key Offset: %04x\n" % self.key_off
		out += "[ ] Unk1: %02x\n" % self.unk1
		out += "[/] Value Type: %02x\n" % self.value_type
		out += "[X] Value Length: %08x\n" % self.value_len
		out += "[X] Padded Length: %08x\n" % self.padded_len
		out += "[X] Value Offset: %08x" % self.value_off
		return out
	def PrettyPrint(self, data, key_off, value_off):
		out  = ""
		out += "[X] Key: '%s'[%04x]\n" % (nullterm(data[self.key_off + key_off:]), self.key_off)
		out += "[/] Unk: %02x\n" % (self.unk1)
		out += "[/] Value Type: %02x\n" % self.value_type
		out += "[X] Value Length: %08x\n" % self.value_len
		out += "[X] Padded Length: %08x\n" % self.padded_len
		out += "[X] Value Offset: %08x" % self.value_off
		if self.value_type == SFO_STRING:
			out += "[X] Value: '%s'[%08x]" % (nullterm(data[self.value_off + value_off:]), self.value_off+value_off)
		elif self.value_type == SFO_INT:
			out += "[X] Value: %d[%08x]" % (struct.unpack('<I', data[self.value_off + value_off:self.value_off + value_off + 4])[0], self.value_off+value_off)
		else:
			out += "[X] Value Type Unknown"
		return out
	
def usage():
	print("""usage:
    python sfo.py""")

def version():
	print("""sfo.py 0.2""")
	
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
			print(header)
			print()
		assert header.magic == SFO_MAGIC
		assert header.unk1 == 0x00000101
		offset += len(header)
		off1 = header.KeyOffset
		off2 = header.ValueOffset
		for x in range(header.PairCount):
			entry = Entry()
			entry.unpack(data[offset:offset+len(entry)])
			if debug and not pretty:
				print(entry)
				print()
			if debug and pretty:
				print(entry.PrettyPrint(data, off1, off2))
				print()
			key = nullterm(data[off1+entry.key_off:])
			if entry.value_type == SFO_STRING:
				value = nullterm(data[off2+entry.value_off:])
			else:
				value = struct.unpack('<I', data[entry.value_off + off2:entry.value_off + off2 + 4])[0]
			stuff[key] = value
			offset += len(entry)
		if not debug:
			print(stuff)
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
			print(header)
			print()
		assert header.magic == SFO_MAGIC
		assert header.unk1 == 0x00000101
		offset += len(header)
		off1 = header.KeyOffset
		off2 = header.ValueOffset
		for x in range(header.PairCount):
			entry = Entry()
			entry.unpack(data[offset:offset+len(entry)])
			if debug and not pretty:
				print(entry)
				print()
			if debug and pretty:
				print(entry.PrettyPrint(data, off1, off2))
				print()
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
		if not debug:
			print(stuff)
	
	doc.appendChild(sfo)
	file = open(xml, "w" )
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
	header.unk1 = 0x00000101
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
		file.write(k.encode('ascii') + b'\0')
	file.write(b'\0' * keypad)
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
			bv = v.encode('ascii')
			file.write(bv + b'\0')
			file.write(b'\0' * (align(len(bv) + 1, alignment) - (len(bv) +1)))
	file.close()
def main():
	global debug
	global pretty
	debug = False
	pretty = False
	list = False
	fileToList = None
	toxml = False
	fromxml = False
	forcetitle = None
	forceappid = None
	if len(sys.argv) < 1:
		return
	if "python" in sys.argv[0]:
		sys.argv = sys.argv[1:]
	try:
		opts, args = getopt.getopt(sys.argv[1:], "hdvpl:tf", ["help", "debug","version", "pretty", "list=", "toxml", "fromxml", "title=", "appid="])
	except getopt.GetoptError:
		usage()
		sys.exit(2)
	
	for opt, arg in opts:
		if opt in ("-h", "--help"):
			usage()
			sys.exit(2)
		elif opt in ("-v", "--version"):
			version()
			sys.exit(2)
		elif opt in ("-l", "--list"):
			fileToList = arg
			list = True
		elif opt in ("-d", "--debug"):
			debug = True
		elif opt in ("-p", "--pretty"):
			pretty = True
		elif opt in ("-t", "--toxml"):
			toxml = True
		elif opt in ("-f", "--fromxml"):
			fromxml = True
		elif opt in ("--title"):
			forcetitle = arg
		elif opt in ("--appid"):
			forceappid = arg
		else:
			usage()
			sys.exit(2)
	
	if list:
		listSFO(fileToList)
	elif toxml and not fromxml and len(args) == 2:
		convertToXml(args[0], args[1])
	elif fromxml and not toxml  and len(args) == 2:
		convertToSFO(args[0], args[1], forcetitle, forceappid)
	else:
		usage()

if __name__ == "__main__":
	main()
