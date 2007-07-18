/*!	\file		errors.c
 * 		\brief	This file contains all functions that support main iso-related functions
 *		\All functin in this file use to process errors in iso message
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "errors.h"
/*!	\fn			isoerrreport(int *fldErr, char *filename)
 *  	\brief		Show the message error to the logfile. 
 * 		\param fldErr: the array contains all errors in message
 * 		\param filename: the name of log file
 * 		\Output: the log file write error in message
 */
void err_iso(int *fldErr, char *filename)
{
	time_t t;
	int i, j;
    file *fp;
    fp = fopen(filename, "a+");
    if (!fp)
    {
    	printf("Can not open file %s", filenamd);
    	exit(1);
    }
    t = time(0);
    fprintf(fp, "The debug error for day: %s", ctime(&t));
    for(i=0; i<129; i++)
    {
    	if (fldErr(i) != 0)
    	{
    		j = 0;
    		while (errdef(j) != NULL)
    		{
    			if (fleErr[i] == errdef(j).Err_ID)
    			{
    				fprintf(fp, "Error appear on the field %d is: %s", i, errdef[j].desc);
    				break;
    			}
    			j = j + 1;
    		}
    	    
    	}
    }
    fclose(fp);
}

/*!	\fn	void err_iso(int *fldErr, FILE *fp)		 
 * 		\brief	this procedure is call when having error during field setting
 * 		\param	err_code is the return value of the function iso8583_set_fmtbitmap
 * 		\param  fld_idx point to the field error 
 * 		\Output: description about the error
 */
/*
char *err_field(int err_code, int fld_idx)
{

}*/

/*!	\fn	void err_iso(int *fldErr, FILE *fp)		 
 * 		\brief	this procedure is call when having error during field setting
 * 		\param	err_code is the return value of the function iso8583_set_fmtbitmap
 * 		\Output: description about the error
 */

char *err_field(int err_code)
{
	int i;
	char *desc;
	i = 0;
	while (errdef[i]!= NULL)
	{
		if (errdef[i].Err_ID = err_code)
		{
			desc = (char *) malloc(strlen(errdef[i].desc) * sizeof(char));
			strcpy(desc, errdef[i].desc)
			break;
		}
		i++;
	}
	if (!desc)
	{
		return "Can not recognize this error code";
	}
	else
	{
		return desc;
	}
}

/*!	\fn	check_fld(char *value, int idx, cons isodef *def)		 
 * 		\brief	this procedure is call to check weather a field is correct or not (pack and unpack)
 * 		\param	value: the value of this field (is set or get from msg)
 * 		\param idx: the possition of this field in msg
 * 		\param def: the definition is used to parse msg
 * 		\output: the errors code if have or zero (not error)
*/
int check_fld(char *value, int idx, cons isodef *def)
{
	int i, err_code;
	int lengthfld; //The length of the value
	
	lengthfld = strlen(value)
	if (def[i].format == ISO_BITMAP)
	{
		err_code = 0;
	}
	else if (def[i].format == ISO_NUMERIC)
	{
		for (i = 0; i < lengthfld; i++)
		{
			if (isdigit(value[i]) == 0)
			{
				err_code = 5;
				break;
			}
		}
	}
	else if (def[i].format == ISO_ALPHANUMERIC)
	{
		for (i = 0; i < lengthfld; i++)
		{
			if (isalnum(value[i]) == 0)
			{
				err_code = 5;
				break;
			}
		}
	}
	else if (def[i].format == ISO_BINARY)
	{
		for (i = 0; i < lengthfld; i++)
		{
			if ((value[i] != '0') && (value[i] != '1'))
			{
				err_code = 5;
				break;
			}
		}
	}
	else
	{
		//format of this field is not defined
		err_code = 3;
	}
	return err_code;
}