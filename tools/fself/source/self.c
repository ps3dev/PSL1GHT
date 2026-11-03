#include "self.h"

void self_read_headers(FILE *in,SELF *self,APP_INFO *app_info,ELF *elf,ELF_PHDR **phdr, ELF_SHDR **shdr, SECTION_INFO **section_info,SCEVERSION_INFO *sceversion_info,CONTROL_INFO **control_info)
{
	// SELF
	if (fread (self, sizeof(SELF), 1, in) != 1) {
		fail("Couldn't read SELF header");
	}

	self->magic = swap32 (self->magic);
	self->version = swap32 (self->version);
	self->flags = swap16 (self->flags);
	self->type = swap16 (self->type);
	self->metadata_offset = swap32 (self->metadata_offset);
	self->header_len = swap64 (self->header_len);
	self->elf_filesize = swap64 (self->elf_filesize);
	self->appinfo_offset = swap64 (self->appinfo_offset);
	self->elf_offset = swap64 (self->elf_offset);
	self->phdr_offset = swap64 (self->phdr_offset);
	self->shdr_offset = swap64 (self->shdr_offset);
	self->section_info_offset = swap64 (self->section_info_offset);
	self->sceversion_offset = swap64 (self->sceversion_offset);
	self->controlinfo_offset = swap64 (self->controlinfo_offset);
	self->controlinfo_size = swap64 (self->controlinfo_size);

	if (self->magic != SCE_MAGIC) {
		fail("not a SELF\n");
	}

	// APP INFO
	if (app_info) {
		fseek (in, self->appinfo_offset, SEEK_SET);
		if (fread (app_info, sizeof(APP_INFO), 1, in) != 1) {
			fail("Couldn't read APP INFO header");
		}
		app_info->authid = swap64 (app_info->authid);
		app_info->vendor_id = swap32 (app_info->vendor_id);
		app_info->self_type = swap32 (app_info->self_type);
		app_info->version = swap32 (app_info->version);
	}

	// ELF
	if (elf) {
		fseek (in, self->elf_offset, SEEK_SET);
		if (fread (elf, sizeof(ELF), 1, in) != 1) {
			fail("Couldn't read ELF header");
		}
		elf->type = swap16 (elf->type);
		elf->machine = swap16 (elf->machine);
		elf->version = swap32 (elf->version);
		elf->entry_point = swap64 (elf->entry_point);
		elf->phdr_offset = swap64 (elf->phdr_offset);
		elf->shdr_offset = swap64 (elf->shdr_offset);
		elf->flags = swap16 (elf->flags);
		elf->header_size = swap32 (elf->header_size);
		elf->phent_size = swap16 (elf->phent_size);
		elf->phnum = swap16 (elf->phnum);
		elf->shent_size = swap16 (elf->shent_size);
		elf->shnum = swap16 (elf->shnum);
		elf->shstrndx = swap16 (elf->shstrndx);
	}

	// PHDR and SECTION INFO
	if (phdr || section_info) {
		uint16_t phnum = 0;
		uint16_t i;

		if (elf) {
			phnum = elf->phnum;
		} else {
			fseek (in, self->elf_offset + 52, SEEK_SET);
			fread (&phnum, sizeof(uint16_t), 1, in);
		}

		if (phdr) {
			ELF_PHDR *elf_phdr = NULL;

			elf_phdr = malloc (sizeof(ELF_PHDR) * phnum);

			fseek (in, self->phdr_offset, SEEK_SET);
			if (fread (elf_phdr, sizeof(ELF_PHDR), phnum, in) != phnum) {
				fail("Couldn't read ELF PHDR header");
			}

			for (i = 0; i < phnum; i++) {
				elf_phdr[i].type = swap32 (elf_phdr[i].type);
				elf_phdr[i].flags = swap32 (elf_phdr[i].flags);
				elf_phdr[i].offset_in_file = swap64 (elf_phdr[i].offset_in_file);
				elf_phdr[i].vitual_addr = swap64 (elf_phdr[i].vitual_addr);
				elf_phdr[i].phys_addr = swap64 (elf_phdr[i].phys_addr);
				elf_phdr[i].segment_size = swap64 (elf_phdr[i].segment_size);
				elf_phdr[i].segment_mem_size = swap64 (elf_phdr[i].segment_mem_size);
				elf_phdr[i].alignment = swap64 (elf_phdr[i].alignment);
			}

			*phdr = elf_phdr;
		}

		// SECTION INFO
		if (section_info) {
			SECTION_INFO *sections = NULL;

			sections = malloc (sizeof(SECTION_INFO) * phnum);

			fseek (in, self->section_info_offset, SEEK_SET);
			if (fread (sections, sizeof(SECTION_INFO), phnum, in) != phnum) {
				fail("Couldn't read SECTION INFO header");
			}

			for (i = 0; i < phnum; i++) {
				sections[i].offset = swap64 (sections[i].offset);
				sections[i].size = swap64 (sections[i].size);
				sections[i].compressed = swap32 (sections[i].compressed);
				sections[i].encrypted = swap32 (sections[i].encrypted);
			}

			*section_info = sections;
		}
	}

	if (sceversion_info) {
		fseek (in, self->sceversion_offset, SEEK_SET);
		if (fread (sceversion_info, sizeof(SCEVERSION_INFO), 1, in) != 1) {
			fail("Couldn't read SCE VERSION INFO header");
		}
		sceversion_info->unknown1 = swap32(sceversion_info->unknown1);
		sceversion_info->unknown2 = swap32(sceversion_info->unknown2);
		sceversion_info->size = swap32(sceversion_info->size);
		sceversion_info->unknown3 = swap32(sceversion_info->unknown3);
		sceversion_info->section_idx = swap16(sceversion_info->section_idx);
		sceversion_info->unknown4 = swap16(sceversion_info->unknown4);
		sceversion_info->unknown5 = swap32(sceversion_info->unknown5);
		sceversion_info->unknown6 = swap32(sceversion_info->unknown6);
		sceversion_info->unknown7 = swap32(sceversion_info->unknown7);
		sceversion_info->offset = swap64(sceversion_info->offset);
		sceversion_info->str_size = swap64(sceversion_info->str_size);
	}

	// CONTROL INFO
	if (control_info) {
		uint32_t i,offset = 0;
		uint32_t index = 0;
		CONTROL_INFO *info = NULL;

		while (offset < self->controlinfo_size) {

			info = realloc (info, sizeof(CONTROL_INFO) * (index + 1));

			fseek (in, self->controlinfo_offset + offset, SEEK_SET);

			if (fread (info + index, sizeof(CONTROL_INFO), 1, in) != 1) {
				fail("Couldn't read CONTROL INFO header");
			}

			info[index].type = swap32 (info[index].type);
			info[index].size = swap32 (info[index].size);
			info[index].cont_flag = swap64(info[index].cont_flag);
			if (info[index].type == 1) {
				for(i=0;i<8;i++)
					info[index].control_flags.control_flags[i] = swap32(info[index].control_flags.control_flags[i]);
			}

			offset += info[index].size;
			index++;
		}
		*control_info = info;
	}

	// SHDR
	if (shdr) {
		uint16_t shnum = 0;
		uint16_t i;
		ELF_SHDR *elf_shdr = NULL;

		if (elf) {
			shnum = elf->shnum;
		} else {
			fseek (in, self->elf_offset + 56, SEEK_SET);
			fread (&shnum, sizeof(uint16_t), 1, in);
		}

		if (shnum > 0 && self->shdr_offset != 0) {
			elf_shdr = malloc (sizeof(ELF_SHDR) * shnum);

			fseek (in, self->shdr_offset, SEEK_SET);
			if (fread (elf_shdr, sizeof(ELF_SHDR), shnum, in) != shnum) {
				fail("Couldn't read ELF SHDR header");
			}

			for (i = 0; i < shnum; i++) {
				elf_shdr[i].name_idx = swap32 (elf_shdr[i].name_idx);
				elf_shdr[i].type = swap32 (elf_shdr[i].type);
				elf_shdr[i].flags = swap64 (elf_shdr[i].flags);
				elf_shdr[i].virtual_addr = swap64 (elf_shdr[i].virtual_addr);
				elf_shdr[i].offset_in_file = swap64 (elf_shdr[i].offset_in_file);
				elf_shdr[i].segment_size = swap64 (elf_shdr[i].segment_size);
				elf_shdr[i].link = swap32 (elf_shdr[i].link);
				elf_shdr[i].info = swap32 (elf_shdr[i].info);
				elf_shdr[i].addr_align = swap64 (elf_shdr[i].addr_align);
				elf_shdr[i].entry_size = swap64 (elf_shdr[i].entry_size);
			}

			*shdr = elf_shdr;
		}
	}
}

void self_build_controlinfo_header(CONTROL_INFO **control_info,int *info_len,const void *elf_data,int elf_len,int npdrm)
{
	CONTROL_INFO *info = calloc((npdrm ? 3 : 2),sizeof(CONTROL_INFO));
	const char control_info_digest1[20] = { 0x62,0x7c,0xb1,0x80,0x8a,0xb9,0x38,0xe3,0x2c,0x8c,0x09,0x17,0x08,0x72,0x6a,0x57,0x9e,0x25,0x86,0xe4 };

	info[0].type = 1;
	info[0].size = 48;
	info[0].cont_flag = 1;

	info[1].type = 2;
	info[1].size = 64;

	memcpy(info[1].file_digest.digest1,control_info_digest1,20);
	sha1(elf_data,elf_len,info[1].file_digest.digest2);

	if(npdrm) {
		info[1].cont_flag = 1;

		info[2].type = 3;
		info[2].size = 144;
	}

	*control_info = info;
	*info_len = 112 + (npdrm ? 144 : 0);
}

void self_build_self_header(SELF *self,ELF *elf_hdr,uint64_t *headerEnd,uint64_t *elfOffset,int elf_filesize,int ctrl_info_size,int npdrm)
{
	memset(self,0,sizeof(SELF));

	self->magic = SCE_MAGIC;
	self->version = 2;
	self->flags = 0x8000;
	self->unknown = 3;
	self->type = 1;
	self->elf_filesize = elf_filesize;
	self->appinfo_offset = align(sizeof(SELF),0x10);
	self->elf_offset = align(self->appinfo_offset + sizeof(APP_INFO),0x10);
	self->phdr_offset = self->elf_offset + sizeof(ELF);
	self->section_info_offset = align(self->phdr_offset + (elf_hdr->phnum*sizeof(ELF_PHDR)),0x10);
	self->sceversion_offset = align(self->section_info_offset + (elf_hdr->phnum*sizeof(SECTION_INFO)),0x10);
	self->controlinfo_offset = align(self->sceversion_offset + sizeof(SCEVERSION_INFO),0x10);
	self->controlinfo_size = ctrl_info_size;

	*headerEnd = self->controlinfo_offset + ctrl_info_size;
	*elfOffset = align(*headerEnd + (npdrm ? 0x5c0 : 0x550),0x80);

	self->header_len = *elfOffset;
	self->metadata_offset = (uint32_t)*headerEnd - 0x20;
	self->shdr_offset = *elfOffset + elf_hdr->shdr_offset;
}

void self_build_app_info_header(APP_INFO *app_info,int npdrm)
{
	memset(app_info,0,sizeof(APP_INFO));

	app_info->authid = 0x1010000001000003ULL;
	app_info->vendor_id = 0x01000002;
	app_info->self_type = npdrm ? 0x08 : 0x04;
	app_info->version = 0x00010000;
}

void self_build_elf_section_info_headers(ELF *elf_hdr,ELF_PHDR **phdrs,ELF_SHDR **shdrs,SECTION_INFO **sec_info,const void *elf_data,uint64_t elfOffset)
{
	int i;
	ELF_PHDR *phdr;
	ELF_SHDR *shdr;
	SECTION_INFO *secinfo;

	elf_read_phdr_header(elf_data,elf_hdr,&phdr);
	elf_read_shdr_header(elf_data,elf_hdr,&shdr);

	secinfo = calloc(elf_hdr->phnum,sizeof(SECTION_INFO));

	for(i=0;i<elf_hdr->phnum;i++) {
		secinfo[i].offset = phdr[i].offset_in_file + elfOffset;
		secinfo[i].size = phdr[i].segment_size;
		secinfo[i].compressed = 1;
		if(phdr[i].type==1) secinfo[i].encrypted = 2;
	}

	*phdrs = phdr;
	*shdrs = shdr;
	*sec_info = secinfo;
}

void self_build_sceversion_header(SCEVERSION_INFO *info,const void *elf_data,uint64_t elfOffset)
{
	int sce_idx = elf_get_shstr_idx(elf_data,".sceversion");
	uint64_t sce_off = elf_get_shstr_off(elf_data,".sceversion");

	memset(info,0,sizeof(SCEVERSION_INFO));

	info->unknown1 = 1;
	info->unknown2 = 1;
	info->size = sizeof(SCEVERSION_INFO);
	info->unknown4 = 1;
	info->unknown6 = 1;
	/* no .sceversion section: sce_off is -1, and strlen would read before elf_data */
	if (sce_idx < 0)
		return;
	info->section_idx = sce_idx;
	info->offset = sce_off + elfOffset;
	info->str_size = strlen((char*)elf_data + sce_off);
}

void self_write_self_header(char *self_data,uint64_t offset,SELF *in)
{
	SELF *out = (SELF*)((char*)self_data + offset);

	out->magic = swap32(in->magic);
	out->version = swap32(in->version);
	out->flags = swap16(in->flags);
	out->type = swap16(in->type);
	out->metadata_offset = swap32(in->metadata_offset);
	out->header_len = swap64(in->header_len);
	out->elf_filesize = swap64(in->elf_filesize);
	out->unknown = swap64(in->unknown);
	out->appinfo_offset = swap64(in->appinfo_offset);
	out->elf_offset = swap64(in->elf_offset);
	out->phdr_offset = swap64(in->phdr_offset);
	out->shdr_offset = swap64(in->shdr_offset);
	out->section_info_offset = swap64(in->section_info_offset);
	out->sceversion_offset = swap64(in->sceversion_offset);
	out->controlinfo_offset = swap64(in->controlinfo_offset);
	out->controlinfo_size = swap64(in->controlinfo_size);
	out->padding = swap64(in->padding);
}

void self_write_appinfo_header(char *self_data,uint64_t offset,APP_INFO *in)
{
	APP_INFO *out = (APP_INFO*)((char*)self_data + offset);

	out->authid = swap64(in->authid);
	out->vendor_id = swap32(in->vendor_id);
	out->self_type = swap32(in->self_type);
	out->version = swap32(in->version);
	memcpy(out->padding,in->padding,sizeof(in->padding));
}

void self_write_elf_header(char *self_data,uint64_t offset,ELF *in)
{
	char *elf_data = (char*)self_data + offset;
	elf_write_elf_header(elf_data,in);
}

void self_write_phdr_header(char *self_data,uint64_t offset,ELF *elf_hdr,ELF_PHDR *in)
{
	char *elf_data = (char*)self_data + offset;
	elf_write_phdr_header(elf_data,elf_hdr,in);
}

void self_write_sectioninfo_header(char *self_data,uint64_t offset,ELF *elf_hdr,SECTION_INFO *in)
{
	uint16_t i;
	SECTION_INFO *out = (SECTION_INFO*)((char*)self_data + offset);

	for(i=0;i<elf_hdr->phnum;i++) {
		out[i].offset = swap64(in[i].offset);
		out[i].size = swap64(in[i].size);
		out[i].compressed = swap32(in[i].compressed); // 2=compressed
		out[i].unknown1 = swap32(in[i].unknown1);
		out[i].unknown2 = swap32(in[i].unknown2);
		out[i].encrypted = swap32(in[i].encrypted); // 1=encrypted
	}
}

void self_write_sceversion_header(char *self_data,uint64_t offset,SCEVERSION_INFO *in)
{
	SCEVERSION_INFO *out = (SCEVERSION_INFO*)((char*)self_data + offset);

	out->unknown1 = swap32(in->unknown1);
	out->unknown2 = swap32(in->unknown2);
	out->size = swap32(in->size);
	out->unknown3 = swap32(in->unknown3);
	out->section_idx = swap16(in->section_idx);
	out->unknown4 = swap16(in->unknown4);
	out->unknown5 = swap32(in->unknown5);
	out->unknown6 = swap32(in->unknown6);
	out->unknown7 = swap32(in->unknown7);
	out->offset = swap64(in->offset);
	out->str_size = swap64(in->str_size);
}

void self_write_control_info_header(char *self_data,uint64_t offset,CONTROL_INFO *in,int npdrm)
{
	uint16_t i,j;
	uint16_t cnt = npdrm ? 3 : 2;

	for(i=0;i<cnt;i++) {
		CONTROL_INFO *out = (CONTROL_INFO*)((char*)self_data + offset);
		
		out->type = swap32(in[i].type);
		out->size = swap32(in[i].size);
		out->cont_flag = swap64(in[i].cont_flag);
		
		switch(in[i].type) {
			case 1:
				for(j=0;j<8;j++) out->control_flags.control_flags[j] = swap32(in[i].control_flags.control_flags[j]);
				break;
			case 2:
				memcpy(out->file_digest.digest1,in[i].file_digest.digest1,0x14);
				memcpy(out->file_digest.digest2,in[i].file_digest.digest2,0x14);
				memcpy(out->file_digest.padding,in[i].file_digest.padding,0x08);
				break;
			case 3:
				for(j=0;j<8;j++) out->npdrm.data[j] = swap32(in[i].npdrm.data[j]);
				break;
			default:
				break;
		}

		offset += in[i].size;
	}
}
