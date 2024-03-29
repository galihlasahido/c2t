/*!	\file	iso8583.c
 * 		\brief	The main source file of this library
 */

#include <stdlib.h>
#include <string.h>
#include "iso8583.h"
#include "utilities.h"
#include "errors.h"
#include "include/expat.h"

/*!	\func	void init_message(isomsg *m, const isodef *def, const msgprop *prop);
 * 		\brief	Initialize an ISO message struct - i.e. set all entries to NULL
 * 		\param	m is an ::isomsg pointer that will be initialized
 * 		\param def is an ::isodef pointer that will be set as the iso definition of m
 * 		\param	prop is a ::msgprop pointer whose value will be set as the properties of m
 */
void init_message(isomsg *m, const isodef *def, const msgprop *prop){
	int i = 0;
	/* set isodef */
		m->def = def;		/* if def is NULL, ok it will be set later */
	/* set properties */
		m->prop.alphanumeric_pad = prop->alphanumeric_pad;
		m->prop.numeric_pad = prop->numeric_pad;
		if(prop->bmp_flag != BMP_BINARY && prop->bmp_flag != BMP_HEXA)
			m->prop.bmp_flag = BMP_BINARY;
		else
			 m->prop.bmp_flag = prop->bmp_flag;
	/* initialize fld */
		for(; i < 129; i++)
			empty_bytes(&m->fld[i]);
}

/*!	\func	void set_isodef(isomsg *m, const isodef *def)
 *  	\brief	assign an iso definition to m
 * 		\param	m		is an ::isomsg struct pointer whose ::isodef value will be set
 * 		\param	def	is an ::isodef pointer which will be set as  m->def
 */
void set_isodef(isomsg *m, isodef *def){
	m->def = def;
}

/*! 	\func	set_prop(isomsg *m, msgprop *prop)
 * 		\brief	assign an msgprop to m
 * 		\param	 m		is an ::isomsg struct pointer whose ::msgprop value will be set
 */
void set_prop(isomsg *m, msgprop *prop){

		m->prop.alphanumeric_pad = prop->alphanumeric_pad;
		m->prop.numeric_pad = prop->numeric_pad;

		if(prop->bmp_flag == BMP_BINARY || prop->bmp_flag == BMP_HEXA)
			 m->prop.bmp_flag = prop->bmp_flag;
}


/*!	\func 	int pack_message(const isomsg *m, char *buf, int *buf_len);
 *		\brief  Pack the content of the ISO message m into buf. \n
 * 				 NOTE: buf must be large enough to contain the packed message.
 *				   Returns the length of the data written to buf.
 *
 * 		\param		m is an ::isomsg structure pointer that contains all message elements to be packed
 * 		\param		buf is the iso message buffer that contains the packed iso message.
 * 		\param		buf_len is the pointer that hold the buf's length
 * 		\return		SUCCEEDED(0) if having no error. \n
 * 						error number if having an error
 */
 int pack_message(isomsg* m, char** buf, int* buf_len){
	bytes buf_bytes;
	char bitmap[16];
	int err=0, i = 0;
	char errmsg[100];
	bytes tmp_bytes, bin_bytes, hexa_bytes;
	/* initilize buf_bytes, binary bitmap */
	empty_bytes(&buf_bytes);
	memset(bitmap, 16, '\0');

	for( ; i <= 128 ; i++){
		if ( i == 1) /* this field is the bitmap, it will be built from bitmap after this loop */
			continue;
		err = verify_bytes(&m->fld[i]);

		/* this field does not contain data */
		if(err != HASDATA){
			if( i == 0 ){ /* this field is the MTI field, can't be empty, report error and return */
				memset(errmsg, 100, '\0');
				sprintf(errmsg, "%s:%d:The MTI field does not contain data", __FILE__, __LINE__);
				handle_err(ERR_IVLFLD, ISO, errmsg);
				free_bytes(&buf_bytes);
				return ERR_IVLFLD;
			}else{ /* this field is not the MTI field */
				continue;
			}
		}

		/* this field conatins data, verify its length */

		/* verify format */
		err = verify_datatype(&m->fld[i], m->def[i].format);
		if(err != CONFORM ){
				memset(errmsg, 100, '\0');
				sprintf(errmsg, "%s:%d:The field #%d does not conform its definition format", __FILE__, __LINE__);
				handle_err(ERR_IVLFMT, ISO, errmsg);
				free_bytes(&buf_bytes);
				return ERR_IVLFMT;
		}

		/* verify data length */
		if(m->def[i].lenflds != 0){ /* fixed length */
			if(m->fld[i].length > m->def[i].flds){		/* over its definition's length */
				memset(errmsg, 100, '\0');
				sprintf(errmsg, "%s:%d: The field #%d is over its defintion's length", __FILE__, __LINE__ - 3, i);
				handle_err(ERR_IVLLEN, ISO, errmsg);
				free_bytes(&buf_bytes);
				return ERR_IVLLEN;
			}
			if(m->fld[i].length < m->def[i].flds){	/* this field's length is smaller than its definition's length */
				/* pad this field */
				if(m->def[i].format == ISO_NUMERIC){
					err = left_pad(&m->fld[i], m->def[i].flds, m->prop.numeric_pad);
					if(err != SUCCEEDED){
						memset(errmsg, 100, '\0');
						sprintf(errmsg, "%s:%d: Can't left pad the field #%d", __FILE__, __LINE__ - 3, i);
						handle_err(err, SYS, errmsg);
						free_bytes(&buf_bytes);
						return err;
					}
				}else{
					right_pad(&m->fld[i], m->def[i].flds, m->prop.alphanumeric_pad);
					if(err != SUCCEEDED){
						memset(errmsg, 100, '\0');
						sprintf(errmsg, "%s:%d: Can't right pad the field #%d", __FILE__, __LINE__ - 3, i);
						handle_err(err, SYS, errmsg);
						free_bytes(&buf_bytes);
						return err;
					}
				}
			}
			/* copy data to tmp_bytes */
			empty_bytes(&tmp_bytes);
			err = import_data(&tmp_bytes, m->fld[i].bytes, m->fld[i].length);
			if(err != SUCCEEDED){
				memset(errmsg, 100, '\0');
				sprintf(errmsg, "%s:%d: Can not import data", __FILE__, __LINE__ - 3);
				handle_err(err, SYS, errmsg);
				free_bytes(&buf_bytes);
				return err;
			}
		}else{	/* variable length */
			char fmt[20], tmp[20];
			if(m->fld[i].length != m->def[i].flds){ /* this field's length is different from its definition */
				memset(errmsg, 100, '\0');
				sprintf(errmsg, "%s:%d: The length of the field #%d  is different from its definition", __FILE__, __LINE__, i);
				handle_err(ERR_IVLLEN, ISO, errmsg);
				free_bytes(&buf_bytes);
				return err;
			}
			/* this field has sufficient length, insert its length header, copy to tmp_bytes */
			empty_bytes(&tmp_bytes);
			sprintf(fmt, "%%0%dd", m->def[i].lenflds);
			sscanf(tmp, fmt, m->fld[i].length);
			err = import_data(&tmp_bytes, tmp, m->def[i].lenflds);
			if(err != SUCCEEDED){
				memset(errmsg, 100, '\0');
				sprintf(errmsg, "%s:%d: Can not import data", __FILE__, __LINE__ - 3);
				handle_err(err, SYS, errmsg);
				free_bytes(&buf_bytes);
				return err;
			}
			err = append_bytes(&tmp_bytes, &m->fld[i]);
			if(err != SUCCEEDED){
				memset(errmsg, 100, '\0');
				sprintf(errmsg, "%s:%d: Can not append bytes", __FILE__, __LINE__);
				handle_err(err, SYS, errmsg);
				free_bytes(&buf_bytes);
				return err;
			}
		}
		/* copy data from tmp_bytes to buf_bytes */
		err = append_bytes(&buf_bytes, &tmp_bytes);
		if(err != SUCCEEDED){
			memset(errmsg, 100, '\0');
			sprintf(errmsg, "%s:%d: Can not append bytes", __FILE__, __LINE__ - 3);
			handle_err(err, SYS, errmsg);
			free_bytes(&tmp_bytes);
			free_bytes(&buf_bytes);
			return err;
		}
		/* update bitmap */
		if( i > 64 ) bitmap[0] |= 0x80; /* there are more than 64 data element, set the first bit to 1 */
		bitmap[(i-1)/8] |= 0x80 >> ((i-1)%8); /* set bit ith of bitmap to 0 */
		/* free tmp_bytes */
		free_bytes(&tmp_bytes);
	}

	/* convert bitmap to the specified format (BIN, HEX) */
	empty_bytes(&bin_bytes);
	empty_bytes(&hexa_bytes);
	err= import_data(&bin_bytes, bitmap, ( i > 64 )?8:16);
	if(err != SUCCEEDED){
		memset(errmsg, 100, '\0');
		sprintf(errmsg, "%s:%d: Can not import data", __FILE__, __LINE__ - 3);
		handle_err(err, SYS, errmsg);
		free_bytes(&buf_bytes);
		return err;
	}
	err = bytes2hexachars(&bin_bytes, &hexa_bytes);
	if(err != SUCCEEDED){
		memset(errmsg, 100, '\0');
		sprintf(errmsg, "%s:%d: Can not convert data from binary format to hexadecimal format", __FILE__, __LINE__ - 3);
		handle_err(err, SYS, errmsg);
		free_bytes(&buf_bytes);
		return err;
	}
	/* Insert bitmap to buf_bytes */
	err = insert_bytes(&buf_bytes, &hexa_bytes, 4);
	if(err != SUCCEEDED){
		memset(errmsg, 100, '\0');
		sprintf(errmsg, "%s:%d: Can not insert bytes", __FILE__, __LINE__ - 3);
		handle_err(err, SYS, errmsg);
		free_bytes(&buf_bytes);
		return err;
	}

	/* copy the iso message to *buf, and its length to buf_len */
	err = export_data(&buf_bytes, *buf, buf_len);
	if(err != SUCCEEDED){
		memset(errmsg, 100, '\0');
		sprintf(errmsg, "%s:%d: Can not export bytes", __FILE__, __LINE__ - 3);
		handle_err(err, SYS, errmsg);
		free_bytes(&buf_bytes);
		return err;
	}

	return SUCCEEDED;
 }

//int pack_message(const isomsg *m, char *buf, int *buf_len)
//{
//	char *start = buf;
//	int flds;
//	int i, sys_err_code;
//	char *bitmap;
//	int len;
//	char tmp[20];
//	bytes byte_tmp, hexa_tmp;
//	int flderr[129];
//
//	if(verify_bytes(&m->fld[0])){
//		handle_err(ERR_IVLFMT, ISO, "Field 0 --> This field does not exist");
//		return ERR_IVLFMT;
//	}
//
//	if (m->fld[0].length != d[0].flds || d[0].lenflds != 0) {
//		/* FIXME: error */
//		/*This error is the length of field is not correct*/
//		handle_err(ERR_IVLLEN, ISO, "Field 0 --> Length is not the same as its iso8583 definition length");
//		return ERR_IVLLEN;
//	}
//	memcpy(buf, m->fld[0], d[0].flds);
//	buf += d[0].flds;
//
//	/*
//	 * The bitmap contains either 64 or 128 fields - flds is the
//	 * number of allowed fields, i.e. either 64 or 128.
//	 */
//	flds = 64;
//	for (i = 65; i <= 128; i++) {
//		if (m->fld[i] != NULL) {
//			flds = 128;
//			break;
//		}
//	}
//
//	bitmap = (char *) calloc(flds/8 + 1, sizeof(char));
//	if (!bitmap)
//	{
//		char err_msg[100];
//		sprintf("%s:%d --> Can't allocate memory for the bitmap buffer", __FILE__, __LINE__);
//		handle_err(ERR_OUTMEM, SYS, err_msg);
//		return ERR_OUTMEM;
//	}
//	/*
//	 * First bit in the bitmap (field 1) defines if the message is
//	 * extended or not.
//	 * 0: not exteded - i.e. only (some of) the fields 0 to 64 are
//	 *    used
//	 * 1: extended - i.e. (some of) the fields 0 to 128 are used
//	 * I.e. if one or more of the fields 65 to 128 is used, the
//	 * bit is 1.
//	 */
//	if (flds == 128) {
//		bitmap[0] |= 0x80;
//	}
//
//	/*
//	 * The bits 2 to 64/128 defines if the corresponding field is
//	 * present (1) or not (0).
//	 */
//	for (i = 2; i <= flds; i++) {
//		if (m->fld[i] != NULL) {
//			/* Set the i'th bit to 1 */
//			bitmap[(i-1)/8] |= 0x80 >> ((i-1)%8);
//		}
//	}
//
//	if(m->bmp_flag == BMP_BINARY){
//		memcpy(buf, bitmap, flds/8);
//		buf += flds/8;
//	}else{
//		byte_tmp.length = flds;
//		byte_tmp.bytes = (char*) calloc(byte_tmp.length, sizeof(char));
//		if(!byte_tmp.bytes){
//			char err_msg[100];
//			sprintf("%s:%d --> Can't allocate memory", __FILE__, __LINE__);
//			handle_err(ERR_OUTMEM, SYS, err_msg);
//			free(bitmap);
//			return ERR_OUTMEM;
//		}
//		memcpy(byte_tmp.bytes, bitmap, byte_tmp.length/8);
//		if( bytes2hexachars(&byte_tmp, &hexa_tmp) < 0){
//			char err_msg[100];
//			sprintf("%s:%d --> Can't convert bytes to a hexacharacter array", __FILE__, __LINE__);
//			handle_err(ERR_BYTHEX, SYS, err_msg);
//			free(bitmap);
//			free(byte_tmp.bytes);
//			return ERR_BYTHEX;
//		};
//		memcpy(buf, hexa_tmp.bytes, hexa_tmp.length);
//		buf += hexa_tmp.length;
//		free(byte_tmp.bytes);
//		free(hexa_tmp.bytes);
//	}
//
//	for (i = 2; i <= flds; i++) {
//		if ((bitmap[(i-1)/8] << ((i-1)%8)) & 0x80) { /* i'th bit != 0 */
//			if (d[i].lenflds) { /* Variable length */
//				switch (d[i].format) {
//				case ISO_NUMERIC: /* Fallthrough */
//				case ISO_ALPHANUMERIC:
//					len = strlen(m->fld[i]);
//					sprintf(tmp, "%%0%dd", d[i].lenflds);
//					sprintf(buf, tmp, len);
//					if (len > d[i].flds) {
//						/*The length of this field is too long*/
//						char err_msg[100];
//						sprintf(err_msg, "Field %d --> The length of this field is too long", i);
//						handle_err(ERR_OVRLEN, ISO, err_msg);
//						return ERR_OVRLEN;
//					}
//					buf += d[i].lenflds;
//					break;
//				case ISO_BINARY:
//					sprintf(tmp, "%%0%dd", d[i].lenflds);
//					sscanf(m->fld[i], tmp, &len);
//					if (len > d[i].flds) {
//						char err_msg[100];
//						sprintf(err_msg, "Field %d --> The length of this field is too long", i);
//						handle_err(ERR_OVRLEN, ISO, err_msg);
//						return ERR_OVRLEN;
//					}
//					/* Copy length bytes from m->fld[i] */
//					len += d[i].lenflds;
//					break;
//				default:{
//						/* FIXME: error */
//						/*The format error of this field*/
//						/*Format out of range*/
//						char err_msg[100];
//						sprintf(err_msg, "Field %d --> Format out of range", i);
//						handle_err(ERR_OUTRAG, ISO, err_msg);
//						return ERR_OUTRAG;
//					}
//					break;
//				}
//			} else { /* Fixed length */
//				len = d[i].flds;
//
//				/* FIXME: How can we check ISO_BINARY? */
//				if (d[i].format != ISO_BINARY &&
//				    strlen(m->fld[i]) != len) {
//					/* FIXME: error */
//					/*The lengthe is not correct*/
//					char err_msg[100];
//					sprintf(err_msg, "Field %d --> The length is not correct", i);
//					handle_err(ERR_IVLLEN, ISO, err_msg);
//					return ERR_IVLLEN;
//				}
//			}
//
//			memcpy(buf, m->fld[i], len);
//
//			buf += len;
//                }
//	}
//	*buf_len = buf-start;
//	return 0;
//}


/*!	\func	int unpack_message(isomsg *m, const char *buf, int buf_len);
 * 		\brief 	Unpack the content of buf into the ISO message struct m.
 * 		\param 	m is an ::isomsg structure pointer that contains all message elements which are unpacked
 * 		\param	buf is the iso message buffer that contains the iso message that needs unpacking.
 * 		\param	buf_len is the length of the iso message buffer
 * 		\returns	0 in case successful unpacking \n
 * 					error number in case an error occured
 */
//int unpack_message(isomsg *m, const char *buf, int buf_len)
//{
//	int flds;
//	int i;
//	int len;
//    char tmp[20];
//    Bytes byte_tmp, hexa_tmp;
//    const char* buf_start = buf;
//
//	/* Field 0 is mandatory and fixed length. */
//	if (d[0].lenflds != 0) {
//		/*The length define is not correct*/
//		handle_err(ERR_IVLLEN, ISO, "Definition 0 --> The length define is not correct");
//		return ERR_IVLLEN;
//	}
//
//	if(d[0].flds > buf_len){
//		char err_msg[100];
//		sprintf(err_msg, "The ISO message buffer's length(%d) is shorter than the length definition of field 0 (%d) ", buf_len, d[0].lenflds);
//		handle_err(ERR_SHTBUF, ISO, err_msg);
//		return ERR_SHTBUF;
//	}
//
//	m->fld[0] = (char *) malloc((d[0].flds + 1) * sizeof(char));
//	if(!m->fld[0])
//	{
//		char err_msg[100];
//		sprintf(err_msg, "%s:%d --> Can't allocate memory for field 0", __FILE__, __LINE__);
//		handle_err(ERR_OUTMEM, SYS, err_msg);
//		return ERR_OUTMEM;
//	}
//	memcpy(m->fld[0], buf, d[0].flds);
//	m->fld[0][d[0].flds] = 0;
//	buf += d[0].flds;
//
//	/*
//	 * First bit in the bitmap (field 1) defines if the message is
//	 * extended or not.
//	 * 0: not exteded - i.e. only (some of) the fields 0 to 64 are
//	 *    used
//	 * 1: extended - i.e. (some of) the fields 0 to 128 are used
//	 * I.e. if one or more of the fields 65 to 128 is used, the
//	 * bit is 1.
//	 */
//	if(m->bmp_flag == BMP_BINARY){
//		if(buf[0] & 0x80) {
//			flds = 128;
//		} else {
//			flds = 64;
//		}
//		m->fld[1] = (char *) calloc(flds/8 + 1, sizeof(char));
//		if (!m->fld[1]){
//			char err_msg[100];
//			sprintf(err_msg, "%s:%d --> Can't allocate memory for field 1", __FILE__, __LINE__);
//			handle_err(ERR_OUTMEM, SYS, err_msg);
//			return ERR_OUTMEM;
//		}
//		if( (buf - buf_start + flds/8) > buf_len){
//			memcpy(m->fld[1], buf, flds/8);
//			buf += flds/8;
//		}else{
//			char err_msg[100];
//			sprintf(err_msg, "The ISO message buffer's length(%d) is too short, stoped at field 1", buf_len);
//			handle_err(ERR_SHTBUF, ISO, err_msg);
//			return ERR_SHTBUF;
//		}
//	}else{
//		if(hexachar2int(buf[0]) & 0x08){
//			flds = 128;
//		} else {
//			flds = 64;
//		}
//		/* Handle buffer too short error	*/
//		if((buf - buf_start + flds/4) > buf_len){
//			char err_msg[100];
//			sprintf(err_msg, "The ISO message buffer's length(%d) is too short, stoped at field 1", buf_len);
//			handle_err(ERR_SHTBUF, ISO, err_msg);
//			return ERR_SHTBUF;
//		}
//		hexa_tmp.length = flds/4;
//		hexa_tmp.bytes = (char *) calloc(hexa_tmp.length, sizeof(char));
//		memcpy(hexa_tmp.bytes, buf, flds/4);
//		if( hexachars2bytes(&hexa_tmp, &byte_tmp) < 0){
//			handle_err(ERR_HEXBYT, SYS, "Can't convert the bitmap hexachar array to binary");
//			free(hexa_tmp.bytes);
//			free(m->fld[0]);
//			return ERR_OUTMEM;
//		}
//
//		m->fld[1] = (char*) calloc(byte_tmp.length/8 + 1, sizeof(char));
//		if (!m->fld[1])
//		{
//			char err_msg[100];
//			sprintf(err_msg, "%s:%d --> Can't allocate memory for field 1", __FILE__, __LINE__);
//			handle_err(ERR_OUTMEM, SYS, err_msg);
//			free(m->fld[0]);
//			free(hexa_tmp.bytes);
//			free(byte_tmp.bytes);
//			return ERR_OUTMEM;
//		}
//		memcpy(m->fld[1], byte_tmp.bytes, byte_tmp.length/8);
//		buf += flds/4;
//		free(hexa_tmp.bytes);
//		free(byte_tmp.bytes);
//	}
//
//	for (i = 2; i <= flds; i++) {
//		if ((m->fld[1][(i-1)/8] << ((i-1)%8)) & 0x80) { /* i'th bit != 0 */
//			if (d[i].lenflds) { /* Variable length */
//				sprintf(tmp, "%%0%dd", d[i].lenflds);
//				sscanf(buf, tmp, &len);
//				/*
//				 * The length of a field can't be
//				 * larger than defined by d[i].flds.
//				 */
//				if (len > d[i].flds) {
//					/* FIXME: warning/error */
//					/*The length of this field is too long*/
//					char err_msg[100];
//					sprintf(err_msg, "Field %d --> The length of this field is too long", i);
//					handle_err(ERR_OVRLEN, ISO, err_msg);
//					return ERR_OVRLEN;
//				}
//			} else { /* Fixed length */
//				len = d[i].flds;
//			}
//
//			/* Handle the buffer too short error */
//			if(buf - buf_start + len + d[i].lenflds > buf_len){
//				char err_msg[100];
//				sprintf(err_msg, "The ISO message buffer's length(%d) is too short, stoped at field %d", buf_len, i);
//				handle_err(ERR_SHTBUF, ISO, err_msg);
//				return ERR_SHTBUF;
//			}
//
//			switch (d[i].format) {
//			case ISO_NUMERIC: /* Fallthrough */
//			case ISO_ALPHANUMERIC:
//				/* Don't copy lenght bytes */
//				buf += d[i].lenflds;
//				break;
//			case ISO_BINARY:
//				/* Copy length bytes */
//				len += d[i].lenflds;
//				break;
//			default:{
//					/* FIXME: error */
//					/*The format of this field is not defined*/
//					char err_msg[100];
//					sprintf(err_msg, "Field %d --> The format of this field is not defined", i);
//					handle_err(ERR_IVLFMT, ISO, err_msg);
//					return ERR_IVLFMT;
//					break;
//				}
//			}
//			m->fld[i] = (char *) malloc((len + 1) * sizeof(char));
//			if (!m->fld[i])
//			{
//				char err_msg[100];
//				sprintf(err_msg, "%s:%d --> Can't allocate memory for field %d", __FILE__, __LINE__, i);
//				handle_err(ERR_OUTMEM, SYS, err_msg);
//				return ERR_OUTMEM;
//			}
//			memcpy(m->fld[i], buf, len);
//			m->fld[i][len] = 0;
//			buf += len;
//		}
//	}
//	return 0;
//}


/*!	\func	void dump_message(FILE *fp, isomsg *m, int fmt_flag);
 * 		\brief 	Dump the content of the ISO message m into a file
 * 		\param 	fp is a FILE pointer that points to the message-storing file
 * 		\param	m is an ::isomsg structure pointer that contains all message elements which needs dumping
 * 		\param	fmt_flag is a flag that indicates which format to dump message
 */

void dump_message(FILE *fp, isomsg *m, int fmt_flag)
{
	int i;
	char* plain_str, *xml_str, *tail; // xml string buffer
	char	tmp[FIELD_MAX_LENGTH];
	switch(fmt_flag){
		case FMT_PLAIN:{
			plain_str = (char*) calloc(PLAIN_MAX_LENGTH, sizeof(char));
			tail = plain_str;
			sprintf(tail, "Field list: ");
			tail = plain_str + strlen(plain_str);
			/* print bitmap */
			for (i =0; i <= 128; i++){
				if (verify_bytes(&m->fld[i]) == HASDATA) {
						memset(tmp, '\0', sizeof(tmp));
						sprintf(tmp, "%d \t ", i);
						if(strlen(tmp) + strlen(plain_str) < PLAIN_MAX_LENGTH){
							strcpy(tail, tmp);
							tail = plain_str + strlen(plain_str);
						}else{
							char err_msg[100];
							sprintf(err_msg, "The dumping string's length(%d) will exceed the defined maximum value (%d)", strlen(tmp) + strlen(plain_str), PLAIN_MAX_LENGTH);
							handle_err(ERR_OVRLEN, SYS, err_msg);
							free(plain_str);
							return;
						}
				}
			}
			memset(tmp, '\0', sizeof(tmp));
			sprintf(tmp, "\n ");
			if(strlen(tmp) + strlen(plain_str) < PLAIN_MAX_LENGTH){
				strcpy(tail, tmp);
				tail = plain_str + strlen(plain_str);
			}else{
				char err_msg[100];
				sprintf(err_msg, "The dumping string's length(%d) exceeds the defined maximum value (%d)", strlen(tmp) + strlen(plain_str), PLAIN_MAX_LENGTH);
				handle_err(ERR_OVRLEN, SYS, err_msg);
				free(plain_str);
				return;
			}
			for (i = 0; i <= 128; i++) {
				if(i==1) continue;
				if (verify_bytes(&m->fld[i]) == HASDATA) {
					memset(tmp, '\0', sizeof(tmp));
					/* up to the data of field i, convert it to text */
					switch(m->def[i].format){
						case ISO_ALPHABETIC:
						case ISO_ALPHANUMERIC_PAD:
						case ISO_ALPHANUMERIC:
						case ISO_ALPHASPECIAL:
						case ISO_ALPHANUMERIC_SPC:
						case ISO_NUMERIC:
						case ISO_NUMERICSPECIAL:
						case ISO_XNUMERIC:
						case ISO_Z:
							snprintf(tmp, m->fld[i].length, "field #%d = %s\n", i, m->fld[i].bytes);
							break;
						case ISO_BINARY:{
							/* convert m->fld[i].bytes to a hexa char array */
							bytes tmp_bytes;
							empty_bytes(&tmp_bytes);
							bytes2hexachars(&m->fld[i], &tmp_bytes);
							/* copy this hexa char array to tmp string */
							snprintf(tmp, tmp_bytes.length, "field #%d = %s\n(hexa)", i, tmp_bytes.bytes);
							free_bytes(&tmp_bytes);
						}
							break;
						default:
							break;
					}
					if(strlen(tmp) + strlen(plain_str) < PLAIN_MAX_LENGTH){
						strcpy(tail, tmp);
						tail = plain_str + strlen(plain_str);
					}else{
						char err_msg[100];
						sprintf(err_msg, "The dumping string's length(%d) exceeds the defined maximum value (%d)", strlen(tmp) + strlen(plain_str), PLAIN_MAX_LENGTH);
						handle_err(ERR_OVRLEN, SYS, err_msg);
						free(plain_str);
						return;
					}
				}
			}
			/* print buffer to file */
			fprintf(fp, plain_str);
			/* free buffer */
			free(plain_str);
		}
			break;
		case FMT_XML:{
			xml_str = (char*) calloc(XML_MAX_LENGTH, sizeof(char));
			tail = xml_str;
			sprintf(tail, "<?xml	version=\"1.0\"	 ?>\n<%s>\n", XML_ROOT_TAG);
			tail = xml_str + strlen(xml_str);
			for(i = 0; i <= 128; i++){
				if (i == 1) continue;
				if (verify_bytes(&m->fld[i]) == HASDATA){
					memset(tmp, '\0', sizeof(tmp));
					switch(m->def[i].format){
						case ISO_ALPHABETIC:
						case ISO_ALPHANUMERIC_PAD:
						case ISO_ALPHANUMERIC:
						case ISO_ALPHASPECIAL:
						case ISO_ALPHANUMERIC_SPC:
						case ISO_NUMERIC:
						case ISO_NUMERICSPECIAL:
						case ISO_XNUMERIC:
						case ISO_Z:
							snprintf(tmp, m->fld[i].length, "\t<%s\tid=\"%d\"\tvalue=\"%s\"/>\n", XML_CHILD_TAG, i, m->fld[i].bytes);
							break;
						case ISO_BINARY:{
							/* convert m->fld[i].bytes to a hexa char array */
							bytes tmp_bytes;
							empty_bytes(&tmp_bytes);
							bytes2hexachars(&m->fld[i], &tmp_bytes);
							/* copy this hexa char array to tmp string */
							snprintf(tmp, tmp_bytes.length, "\t<%s\tid=\"%d\"\tvalue=\"%s\"/>\n", XML_CHILD_TAG, i, tmp_bytes.bytes);
							free_bytes(&tmp_bytes);
						}
							break;
						default:
							break;
					}
					if((strlen(tmp) + strlen(xml_str)) < XML_MAX_LENGTH){
						sprintf(tail, "%s", tmp);
						tail = xml_str + strlen(xml_str);
					}else{
						char err_msg[100];
						sprintf(err_msg, "The dumping xml string's length(%d) exceeds the defined maximum value (%d)", strlen(tmp) + strlen(xml_str), XML_MAX_LENGTH);
						handle_err(ERR_OVRLEN, SYS, err_msg);
						free(xml_str);
						return;
					}
				}
			}
			memset(tmp, '\0', sizeof(tmp));
			sprintf(tmp, "</%s>", XML_ROOT_TAG);
			if(strlen(tmp) + strlen(xml_str) < XML_MAX_LENGTH){
				sprintf(tail, "%s", tmp);
				/* print buffer to file */
				fprintf(fp, xml_str);
				/* free buffer */
				free(xml_str);
			}else{
				char err_msg[100];
				sprintf(err_msg, "The dumping xml string's length(%d) exceeds the defined maximum value (%d)", strlen(tmp) + strlen(xml_str), XML_MAX_LENGTH);
				handle_err(ERR_OVRLEN, SYS, err_msg);
				free(xml_str);
				return;
			}

		}
			break;
		default:{
			char err_msg[100];
			sprintf(err_msg, "The format type is %d", fmt_flag);
			handle_err(ERR_NODFMT, ISO, err_msg);
		}
		break;
	}
}


/*!	\func			void free_message(isomsg *m)
 *  	\brief		Free memory used by the ISO message struct m.
 */
void free_message(isomsg *m)
{
	int i;
	for (i = 0; i <= 128; i++) {
		free_bytes(&m->fld[i]);
	}
}
/*!	\func	int set_field(isomsg* m, int idx, const char *fld, int fld_len);
 *  \brief	set data to a field of iso msg.
 * 	\param	m is an ::isomsg
 * 	\param	fld	is char string
 * 	\param  fld_len is the length of fld
 */

//int set_field(isomsg* m, int idx, const char *fld, int fld_len)
//{
//	char err_msg[100];
//
//	if ((idx > 128) || (idx < 0)) {
//		/*
//		 * The value of idx must be between 0 and 128
//		 */
//		sprintf(err_msg, "%s:%d --> Invalid field %d", __FILE__, __LINE__, idx);
//		handle_err(ERR_IVLFLD, ISO, err_msg);
//		return ERR_IVLFLD; /*Invalid field*/
//	}
//
//	isodef *def = m->iso_def;
//
//	if ((def[idx].format == ISO_BINARY && m->bmp_flag == BMP_HEXA && fld_len > 2*def[idx].flds)
//		|| (def[idx].format != ISO_BINARY && fld_len > def[idx].flds)) {
//		/*
//		 * The length of this field is too long
//		 */
//		sprintf(err_msg, "%s:%d --> The length of field %d is too long", __FILE__, __LINE__, idx);
//		handle_err(ERR_OVRLEN, ISO, err_msg);
//		return ERR_OVRLEN;
//	}
//
//	int err_code = check_fld(fld, fld_len, idx, def);
//	if (err_code) {
//		/*
//		 * The value of this field is invalid
//		 */
//		sprintf(err_msg, "%s:%d --> The value of field %d is invalid", __FILE__, __LINE__, idx);
//		handle_err(err_code, ISO, err_msg);
//		return err_code;
//	}
//
//	char *field = (char *)malloc((fld_len + 1)*sizeof(char));
//	if (field == NULL) {
//		sprintf(err_msg, "%s:%d --> Can't allocate memory for field %d", __FILE__, __LINE__,idx);
//		handle_err(ERR_OUTMEM, SYS, err_msg);
//		return ERR_OUTMEM;
//	}
//
//	switch (def[idx].format) {
//		case ISO_BITMAP:
//			break;
//		case ISO_NUMERIC:
//			if (IS_FIXED_LEN(def,idx)) {
//				lpad(field, def[idx].flds, '0');
//			}
//			break;
//		case ISO_ALPHANUMERIC:
//			if (IS_FIXED_LEN(def,idx)) {
//				rpad(field, def[idx].flds, ' ');
//			}
//			break;
//		case ISO_BINARY:
//			break;
//		default:
//			sprintf(err_msg, "Field %d --> Format out of range", idx);
//			handle_err(ERR_IVLFMT, ISO, err_msg);
//			return ERR_IVLFMT; /*invalid format*/
//	}
//
//	// set data to the idxth field
//	m->fld[idx].bytes = field;
//	m->fld[idx].length = fld_len;
//
//	return 0;
//}

/*!	\func	int get_field(char* buf, const isodef *def, int bmp_flag, int idx, char *fld, int *fld_len);
 * 	\brief	get data of a field from the msg buff.
 * 	\param	buf is iso message byte string
 * 	\param	def is an ::isodef
 * 	\param  bmp_flag
 * 	\param	idx is index of the field to be retrieved
 * 	\param	fld	is char string
 * 	\param  fld_len is the length of fld
 */

//int get_field(char* buf, const isodef *def, int bmp_flag, int idx, char *fld, int *fld_len)
//{
//	char *pos = &buf[0];
//	int bmp_len;
//
//	char err_msg[100];
//
//	if ((idx > 128) || (idx < 0)) {
//		/*
//		 * The value of idx must be between 0 and 128
//		 */
//		sprintf(err_msg, "%s:%d --> Invalid field %d", __FILE__, __LINE__, idx);
//		handle_err(ERR_IVLFLD, ISO, err_msg);
//		return ERR_IVLFLD; /*Invalid field*/
//	}
//
//	/* Field 0 is mandatory and fixed length. */
//	if (def[0].lenflds != 0) {
//		/*The length define is not correct*/
//		handle_err(ERR_IVLLEN, ISO, "Definition 0 --> The length define is not correct");
//		return 0;
//	}
//
//	if (idx == 0) {
//		/* the field is mti */
//		memcpy(fld,pos,def[0].flds);
//		*fld_len =  def[0].flds;
//		return 0;
//	}
//
//	pos += def[0].flds;
//	/* get the bitmap to determine a field exist in the msg */
//	/*
//	 * First bit in the bitmap (field 1) defines if the message is
//	 * extended or not.
//	 * 0: not exteded - i.e. only (some of) the fields 0 to 64 are
//	 *    used
//	 * 1: extended - i.e. (some of) the fields 0 to 128 are used
//	 * I.e. if one or more of the fields 65 to 128 is used, the
//	 * bit is 1.
//	 */
//	int flds = 64;
//	char bitmap[100];
//
//	if(bmp_flag == BMP_BINARY){
//		if(*pos & 0x80) {
//			flds = 128;
//		} else {
//			flds = 64;
//		}
//		bmp_len = flds/8;
//		memcpy(bitmap, pos, bmp_len);
//		bitmap[bmp_len] = 0;
//		pos += bmp_len;
//	}else{
//		if(hexachar2int(*pos) & 0x08) {
//			flds = 128;
//		} else {
//			flds = 64;
//		}
//		Bytes byte_tmp, hexa_tmp;
//		hexa_tmp.length = flds/4;
//		hexa_tmp.bytes = (char *) calloc(hexa_tmp.length, sizeof(char));
//		if (hexa_tmp.bytes == NULL) {
//			sprintf(err_msg, "%s:%d --> Can't allocate memory", __FILE__, __LINE__);
//			handle_err(ERR_OUTMEM, SYS, err_msg);
//			return ERR_OUTMEM;
//		}
//		memcpy(hexa_tmp.bytes, pos, flds/4);
//		if( hexachars2bytes(&hexa_tmp, &byte_tmp) < 0){
//			handle_err(ERR_HEXBYT, SYS, "Can't convert the bitmap hexachar array to binary");
//			free(hexa_tmp.bytes);
//			return ERR_HEXBYT;
//		}
//		bmp_len = byte_tmp.length/8;
//		memcpy(bitmap, byte_tmp.bytes, bmp_len);
//		free(hexa_tmp.bytes);
//		free(byte_tmp.bytes);
//		pos += flds/4;
//	}
//
//	if (idx == 1) {
//		/* the field is bitmap */
//		*fld_len = bmp_len;
//		memcpy(fld,bitmap,bmp_len);
//		return 0;
//	}
//
//	if (idx <= flds && ((bitmap[(idx-1)/8] << ((idx-1)%8)) & 0x80)) { /* idx'th bit != 0 */
//		int i;
//		int len;
//		char tmp[20];
//		for (i = 2; i <= idx; i++) {
//			if ((bitmap[(i-1)/8] << ((i-1)%8)) & 0x80) { /* i'th bit != 0 */
//				if (!IS_FIXED_LEN(def,i)) { /* Variable length */
//					sprintf(tmp, "%%0%dd", (bmp_flag == BMP_HEXA && def[idx].format == ISO_BINARY) ? 2*def[i].lenflds : def[i].lenflds);
//					sscanf(pos, tmp, &len);
//					/*
//					 * The length of a field can't be
//					 * larger than defined by def[i].flds.
//					 */
//					if (len > (bmp_flag == BMP_HEXA && def[idx].format == ISO_BINARY) ? 2*def[i].lenflds : def[i].lenflds) {
//						sprintf(err_msg, "%s:%d --> The length of field %d is too long", __FILE__, __LINE__, idx);
//						handle_err(ERR_OVRLEN, ISO, err_msg);
//						return ERR_OVRLEN;
//					}
//				}
//				else { /* Fixed length */
//					len = (bmp_flag == BMP_HEXA && def[idx].format == ISO_BINARY) ? 2*def[i].lenflds : def[i].lenflds;
//				}
//				switch (def[i].format) {
//					case ISO_NUMERIC: /* Fallthrough */
//					case ISO_ALPHANUMERIC:
//						/* Don't copy lenght bytes */
//						pos += def[i].lenflds;
//						break;
//					case ISO_BINARY:
//						/* Copy length bytes */
//						len += (bmp_flag == BMP_HEXA) ? 2*def[i].lenflds : def[i].lenflds;
//						break;
//					default:
//						sprintf(err_msg, "Field %d --> Format out of range", idx);
//						handle_err(ERR_IVLFMT, ISO, err_msg);
//						return ERR_IVLFMT; /*invalid format*/
//				}
//
//				if (i == idx) {
//					*fld_len = len;
//					memcpy(fld, pos, len);
//					fld[len] = '\0';
//				}
//				pos += len;
//			}
//		}
//	}
//	else {
//		/* the field not exist */
//		sprintf(err_msg, "The fied %d is not exist", idx);
//		handle_err(ERR_IVLFLD, ISO, err_msg);
//		return ERR_IVLFLD; /*invalid format*/
//	}
//	return 0;
//}
//
