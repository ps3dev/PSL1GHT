OUTPUT_FORMAT("elf64-powerpc", "elf64-powerpc", "elf64-powerpc")
OUTPUT_ARCH(powerpc:common64)

ENTRY(_start)

PHDRS
{
	hdr_text	PT_LOAD FILEHDR PHDRS	FLAGS(0x01 | 0x04);
	hdr_data	PT_LOAD					FLAGS(0x02 | 0x04);
	hdr_srodata	PT_LOAD					FLAGS(0x04 | 0x00400000);
	hdr_sdata	PT_LOAD					FLAGS(0x02 | 0x04 | 0x00200000 | 0x00400000);
	hdr_rsx		PT_LOAD					FLAGS(0x02 | 0x04 | 0x00200000 | 0x00400000 | 0x02000000 | 0x04000000);
	hdr_tls		PT_TLS					FLAGS(0x04);
	hdr_param	0x60000001				FLAGS(0x00);
	hdr_prx		0x60000002				FLAGS(0x00);
}

SECTIONS
{
	PROVIDE (__executable_start = 0x10000); . = 0x10000 + SIZEOF_HEADERS;
	.interp         : { *(.interp) } : hdr_text
	.hash           : { *(.hash) }
	.dynsym         : { *(.dynsym) }
	.dynstr         : { *(.dynstr) }
	.gnu.version    : { *(.gnu.version) }
	.gnu.version_d  : { *(.gnu.version_d) }
	.gnu.version_r  : { *(.gnu.version_r) }
	.rel.dyn		:
	  {
	    *(.rel.init)
	    *(.rel.text .rel.text.* .rel.gnu.linkonce.t.*)
	    *(.rel.fini)
	    *(.rel.rodata .rel.rodata.* .rel.gnu.linkonce.r.*)
	    *(.rel.data.rel.ro*)
	    *(.rel.data .rel.data.* .rel.gnu.linkonce.d.*)
	    *(.rel.tdata .rel.tdata.* .rel.gnu.linkonce.td.*)
	    *(.rel.tbss .rel.tbss.* .rel.gnu.linkonce.tb.*)
	    *(.rel.ctors)
	    *(.rel.dtors)
	    *(.rel.got)
	    *(.rel.sdata .rel.sdata.* .rel.gnu.linkonce.s.*)
	    *(.rel.sbss .rel.sbss.* .rel.gnu.linkonce.sb.*)
	    *(.rel.sdata2 .rel.sdata2.* .rel.gnu.linkonce.s2.*)
	    *(.rel.sbss2 .rel.sbss2.* .rel.gnu.linkonce.sb2.*)
	    *(.rel.bss .rel.bss.* .rel.gnu.linkonce.b.*)
	  }

	.rela.dyn		:
	  {
	    *(.rela.init)
	    *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*)
	    *(.rela.fini)
	    *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*)
	    *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*)
	    *(.rela.tdata .rela.tdata.* .rela.gnu.linkonce.td.*)
	    *(.rela.tbss .rela.tbss.* .rela.gnu.linkonce.tb.*)
	    *(.rela.ctors)
	    *(.rela.dtors)
	    *(.rela.got)
	    *(.rela.toc)
	    *(.rela.sdata .rela.sdata.* .rela.gnu.linkonce.s.*)
	    *(.rela.sbss .rela.sbss.* .rela.gnu.linkonce.sb.*)
	    *(.rela.sdata2 .rela.sdata2.* .rela.gnu.linkonce.s2.*)
	    *(.rela.sbss2 .rela.sbss2.* .rela.gnu.linkonce.sb2.*)
	    *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*)
	  }
	.rel.plt        : { *(.rel.plt) }
	.rela.plt       : { *(.rela.plt) }
	.rela.tocbss    : { *(.rela.tocbss) }
	.init           : {
		KEEP (*(.init))
	} = 0x60000000
	.text           : {
		*(.text .stub .text.* .gnu.linkonce.t.*)
		KEEP (*(.text.*personality*))
		/* .gnu.warning sections are handled specially by elf32.em.  */
		*(.gnu.warning)
		*(.sfpr .glink)
	} = 0x60000000
	.fini           : {
		KEEP (*(.fini))
	} = 0x60000000
	.sceStub.text   : {
		*(.sceStub.text)
		*(SORT(.sceStub.text.*))
	}
	PROVIDE (__etext = .);
	PROVIDE (_etext = .);
	PROVIDE (etext = .);

	.eh_frame_hdr : { *(.eh_frame_hdr) }
	.eh_frame       : ONLY_IF_RO { KEEP (*(.eh_frame)) }
	.gcc_except_table   : ONLY_IF_RO { *(.gcc_except_table .gcc_except_table.*) }

	.rodata.sceResident : { KEEP (*(.rodata.sceResident)) }
	.rodata.sceFNID     : { KEEP (*(.rodata.sceFNID .rodata.sceFNID.*)) }
	.rodata.sceVNID     : { KEEP (*(.rodata.sceVNID .rodata.sceVNID.*)) }
	.rodata.sceTNID     : { KEEP (*(.rodata.sceTNID .rodata.sceTNID.*)) }
	.rodata.sceVStub    : { KEEP (*(.rodata.sceVStub)) KEEP (*(SORT(.rodata.sceVStub.*))) }

	PROVIDE (__libentstart = .);
	.lib.ent.top : { KEEP (*(.lib.ent.top)) }
	.lib.ent	 : { KEEP (*(.lib.ent)) }
	.lib.ent.btm : { KEEP (*(.lib.ent.btm)) }
	PROVIDE (__libentend = .);

	PROVIDE (__libstubstart = .);
	.lib.stub.top : { KEEP (*(.lib.stub.top)) }
	.lib.stub	  : { KEEP (*(.lib.stub)) }
	.lib.stub.btm : { KEEP (*(.lib.stub.btm)) }
	PROVIDE (__libstubend = .);

	.ppu_rodata         : { *(.ppu_rodata .ppu_rodata.*) }
	.sys_proc_param     : { KEEP (*(.sys_proc_param)) } : hdr_text : hdr_param
	.sys_proc_prx_param : { KEEP (*(.sys_proc_prx_param)) } : hdr_text : hdr_prx

	. = ALIGN(64K);
	
	.ppu_data           : { *(.ppu_data .ppu_data.*) } : hdr_data
	.sdata2             : { *(.sdata2 .sdata2.* .gnu.linkonce.s2.*) }
	.sbss2              : { *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*) }

	/* Exception handling  */
	.eh_frame       : ONLY_IF_RW { KEEP (*(.eh_frame)) }
	.gcc_except_table   : ONLY_IF_RW { *(.gcc_except_table .gcc_except_table.*) }
	
	. = ALIGN(64 / 8);
	
	.preinit_array     : {
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
	}
	.init_array     : {
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
	}
	.fini_array     : {
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		PROVIDE_HIDDEN (__fini_array_end = .);
	}
	.ctors          : {
		/* gcc uses crtbegin.o to find the start of
		   the constructors, so we make sure it is
		   first.  Because this is a wildcard, it
		   doesn't matter if the user does not
		   actually link against crtbegin.o; the
		   linker won't look for a file to match a
		   wildcard.  The wildcard also means that it
		   doesn't matter which directory crtbegin.o
		   is in.  */
		KEEP (*crtbegin.o(.ctors))
		KEEP (*crtbegin?.o(.ctors))
		/* We don't want to include the .ctor section from
		   the crtend.o file until after the sorted ctors.
		   The .ctor section from the crtend file contains the
		   end of ctors marker and it must be last */
		KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*(.ctors))
	}
	.dtors          : {
		KEEP (*crtbegin.o(.dtors))
		KEEP (*crtbegin?.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*(.dtors))
	}
	.jcr            : { KEEP (*(.jcr)) }
	.data.rel.ro : { *(.data.rel.ro.local*) *(.data.rel.ro*) }
	.dynamic        : { *(.dynamic) }

	.data.sceFStub      : { KEEP (*(.data.sceFStub)) KEEP (*(SORT(.data.sceFStub.*))) }
	
	.toc1		  : ALIGN(8) { *(.toc1) }
	.opd		  : ALIGN(8) { KEEP (*(.opd)) }
	.branch_lt	  : ALIGN(8) { *(.branch_lt) }
	.got		  : ALIGN(8) { *(.got .toc) }
	.toc.sceStub  : { KEEP (*(.toc.sceStub)) KEEP (*(SORT(.toc.sceStub.*))) }
	
	/* Thread Local Storage sections  */
	.tdata	  : { *(.tdata .tdata.* .gnu.linkonce.td.*) } : hdr_data : hdr_tls
	.tbss		  : { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) } : hdr_data : hdr_tls
	
	. = ALIGN(256M);

	.rodata         : { *(.rodata .rodata.* .gnu.linkonce.r.*) } : hdr_srodata
	.rodata1        : { *(.rodata1) } : hdr_srodata

	.spu_image		: { *(.spu_image .spu_image.*) } : hdr_srodata
	
	. = ALIGN(64K);

	.data           :
	{
		*(.data .data.* .gnu.linkonce.d.*)
		KEEP (*(.gnu.linkonce.d.*personality*))
		SORT(CONSTRUCTORS)
	} : hdr_sdata
	.data1          : { *(.data1) }

  /* We want the small data sections together, so single-instruction offsets
     can access them all, and initialized data all before uninitialized, so
     we can shorten the on-disk segment size.  */
	.sdata          : {
		*(.sdata .sdata.* .gnu.linkonce.s.*)
	}
	_edata = .;
	PROVIDE (edata = .);

	__bss_start = .;
	.tocbss	  : ALIGN(8) { *(.tocbss)}
	.sbss           :
	{
		PROVIDE (__sbss_start = .);
		PROVIDE (___sbss_start = .);
		*(.dynsbss)
		*(.sbss .sbss.* .gnu.linkonce.sb.*)
		*(.scommon)
		PROVIDE (__sbss_end = .);
		PROVIDE (___sbss_end = .);
	}
	.plt            : { *(.plt) }
	.bss            :
	{
		*(.dynbss)
		*(.bss .bss.* .gnu.linkonce.b.*)
		*(COMMON)
		/* Align here to ensure that the .bss section occupies space up to
		   _end.  Align after .bss to ensure correct alignment even if the
		   .bss section disappears because there are no input sections.
		   FIXME: Why do we need it? When there is no .bss section, we don't
		   pad the .data section.  */
		. = ALIGN(64 / 8);
	}
	
	. = ALIGN(256M);

	.rsx_image	: { *(.rsx_*) } : hdr_rsx
		
	_end = .;
	__end = .;
	PROVIDE (end = .);
	
	. = ALIGN(8);
	
	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
	/* DWARF debug sections.
	   Symbols in the DWARF debugging sections are relative to the beginning
	   of the section so we begin them at 0.  */
	/* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
	/* DWARF 3 */
	.debug_pubtypes 0 : { *(.debug_pubtypes) }
	.debug_ranges   0 : { *(.debug_ranges) }
	.gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
	/DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) }
}
