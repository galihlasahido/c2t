#ifndef ERRORS_H_
#define ERRORS_H_
#define	ERR_OVRLEN	100
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
} errmsg;*/
/*Define the const error of iso message*/
const errmsg errdef[] ={
		{1, ""}, /*The length of field is to long*/
		{2, ""}, /*The length of field is not correct*/
		{3, ""}  /*The format of this field is not correct*/
}; /*The format error*/
/*!	\fn			isoerrreport(int *fldErr, FILE *fp)
 *  	\brief		Show the message error to the logfile. 
 */
void err_iso(int *fldErr, FILE *fp)

void err_field(int err_code, int fld_idx)
/*!	\fn	void err_iso(int *fldErr, FILE *fp)		 
 * 		\brief	this procedure is call when having error during field setting
 * 		\param	err_code is the return value of the function iso8583_set_fmtbitmap
 * 		\param  fld_idx point to the field error 
 */
#endif /*ERRORS_H_*/
