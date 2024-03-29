#ifndef ERRORS_H_
#define ERRORS_H_
#include "iso8583.h"

#define	WARN				0
#define	ERR_OVRLEN	100
#define	ERR_OUTMEM	101
#define  ERR_OVIDX		102 		//The index is out of range
#define  ERR_IVLLEN 	103
#define  ERR_IVLFMT 	104  	//invalid format(format is not defined)
#define  ERR_IVLVAL 	105 		//The value is not compatible with the format of this field
#define  ERR_IVLFLD		106 		// Invalid field
#define 	ERR_IVLVER		107		// Invalid version
#define	ERR_OUTRAG 	108 		//Out of range
#define 	ERR_IVLFLG		109		// Invalid flag
#define	ERR_CPYNUL	110		//Copy NULL memory
#define	ERR_APDNUL	111		//Use an empty memory to append
#define	ERR_INSNUL		112		//Use an empty memory to insert
#define	ERR_IVLPOS		113		// Invalid position


/*! \brief	errors for utility functions from 2000 to 3000 */
#define  ERR_HEXBYT	2000
#define	ERR_BYTHEX	2001
#define	ERR_WROFMT	2002

#define	ERR_NODFMT	4000	// not support this dump format

/*!	\brief	errors for convert functions from 5000 to 6000 */
#define ERR_PASMEM	5000
#define ERR_XMLPAS		5001
#define ERR_IVLIDX		5002
#define ERR_XMLSYT		5003

/*!	\brief	buffer-related errors	from 6001 to 7000	*/
#define ERR_SHTBUF		6001


#define ISO 1
#define SYS 2
/*!	\struct		errmsg
 * 		\brief		The ERROR ISO message structure
 */
typedef struct {
	/*!  \brief  The error format:
	 * 			- 0 for
	 * 			- 1 for
	 * 			- 2 for
	 * 			- 3 for
	 */
	int Err_ID;

	/*! \brief This var represents the description of this error */

	const char *dsc;
} errmsg;

/*Define the const error of iso message*/
static const errmsg errdef[] ={
		{WARN, "Warning"},
		{ERR_OVRLEN, "The length of field is too long"}, /*The length of field is too long*/
		{ERR_IVLLEN, "The length of field is not correct"}, /*The length of field is not correct*/
		{ERR_IVLFMT, "The format of this field is not define"},  /*The format of this field is not define*/
		{ERR_OVIDX, "The index of field is out of range"},	/*The index of field is out of range*/
		{ERR_IVLVAL,"The value of this field is not correct format"}, /*The value of this field is not correct format*/
		{ERR_OUTMEM, "Out of memory"}, /*Out of memory*/
		{ERR_IVLFLD, "Invalid field"},
		{ERR_HEXBYT, "Failed to convert hexa characters to a byte array"},
		{ERR_BYTHEX, "Failed to convert a byte array to hexa characters"},
		{ERR_NODFMT, "Not support this dump format"},
		{ERR_IVLVER, "Invalid version"},
		{ERR_IVLFLG, "Invalid flag"},
		{ERR_OUTRAG, "Out of range"},
		{ERR_WROFMT, "Wrong format"},
		{ERR_PASMEM, "Couldn't allocate memory for parser"},
		{ERR_XMLPAS, "The XML document is not well-formed"},
		{ERR_IVLIDX,"Invalid index value"},
		{ERR_SHTBUF,"The buffer is too short"},
		{ERR_XMLSYT,"Xml syntax error"},
		{ERR_CPYNUL,"Copy NULL memory"},
		{ERR_APDNUL,"Use an empty memory to append"},
		{ERR_INSNUL, "Use an empty memory to insert"},
		{ERR_IVLPOS, "Invalid position"}
}; /*The format error*/

/*!	\func	void iso_err(int *fldErr, FILE *fp)
 * 		\brief this function is call to write the errors in msg to the log file
 * 		\param	fldErr is an array that contains all errors in msg 8583
 * 		\filename is name of log file
*/
void iso_err(int *fldErr, char *filename);
//char *scan_err(int err_code, int fld_idx)
/*!	\func	char *scan_err(int *fldErr, FILE *fp)
 * 		\brief	This function is used to show the description of errors
 * 		\param	err_code is the return value of the function iso8583_set_fmtbitmap
 * 		\Output: one message to description the error
 */
char *scan_err(int err_code);

/*!	\func	void *sys_err(int err_code, FILE *fp)
 * 		\brief	This function is used to process the system error (such as out of memory ...)
 * 		\param	err_code is the code of this error
 * 		\param filename is the name of the log file for system error
 * 		\Output: the desc of this error is written to log file (file name)
 */
void sys_err(int err_code, char *filename);

/*! \func void handle_err(int err_code, int err_type, char *desc)
 * 		\brief This function is used to write the error of one field to the log file
 * 		\param err_code the error code that recieved when error appear
 * 		\param err_type is type of error, if err_type = 1 is the system error else is the iso error
 * 		\param moredesc is the description of this error of developer
 * 		\output: the desc of this error is written to log file (file name)
 */
int handle_err(int err_code, int err_type, char *moredesc);
#endif /*ERRORS_H_*/
