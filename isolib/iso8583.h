/*!	\file		iso8583.h
 * 		\brief	The header file contain all structure of this library
 */
#ifndef _ISO8583_H
#define _ISO8583_H 1

#include <stdio.h>
#include <time.h>

#define ISO_BITMAP       0
#define ISO_NUMERIC      1
#define ISO_ALPHANUMERIC 2
#define ISO_BINARY       3

<<<<<<< .mine
#define IS_FIXED_LEN(def,idx) (def[(idx)].lenflds==0)

=======
#define BMP_BINARY		0
#define BMP_HEXA			1

>>>>>>> .r23
/*!	\struct		isodef
 * 		\brief		The structure holds all data delement definitions of an iso8583 version.  		
 */
typedef struct {
	/*!  \brief  The data element format:
	 * 			- 0 for BITMAP
	 * 			- 1 for Numeric
	 * 			- 2 for alphanumeric
	 * 			- 3 for binary
	 */ 
	int format;
	
	/*! \brief  This var represents the length of the len portion of this data element (LL or LLL) */	
	int lenflds; 				
	
	/*! \brief This var represents the len of the data element */
	int flds;					
	
	/*! \brief This var represents the description of this data element */
	const char *dsc;	
} isodef;

/*!	\struct		isomsg
 * 		\brief		The ISO message structure 		
 */		
typedef struct {
	/*! \brief The flag to identify whether the bitmap is in binary or hexa format */
	int bmp_flag;
	/*! \brief The 129 field pointer array		*/
	char *fld[129];	
} isomsg;

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

isodef iso93[] = {
    /*000*/ {ISO_NUMERIC, 0, 4, "Message Type Indicator"},
    /*001*/ {ISO_BITMAP, 0, 16, "Bitmap"},
    /*002*/ {ISO_NUMERIC, 2, 19, "Primary Account number"},
    /*003*/ {ISO_NUMERIC, 0, 6, "Processing Code"},
    /*004*/ {ISO_NUMERIC, 0, 12, "Amount, Transaction"},
    /*005*/ {ISO_NUMERIC, 0, 12, "Amount, Reconciliation"},
    /*006*/ {ISO_NUMERIC, 0, 12, "Amount, Cardholder billing"},
    /*007*/ {ISO_NUMERIC, 0, 10, "Date and time, transmission"},
    /*008*/ {ISO_NUMERIC, 0, 8, "Amount, Cardholder billing fee"},
    /*009*/ {ISO_NUMERIC, 0, 8, "Conversion rate, Reconciliation"},
    /*010*/ {ISO_NUMERIC, 0, 8, "Conversion rate, Cardholder billing"},
    /*011*/ {ISO_NUMERIC, 0, 6, "Systems trace audit number"},
    /*012*/ {ISO_NUMERIC, 0, 12, "Date and time, Local transaction"},
    /*013*/ {ISO_NUMERIC, 0, 4, "Date, Effective"},
    /*014*/ {ISO_NUMERIC, 0, 4, "Date, Expiration"},
    /*015*/ {ISO_NUMERIC, 0, 6, "Date, Settlement"},
    /*016*/ {ISO_NUMERIC, 0, 4, "Date, Conversion"},
    /*017*/ {ISO_NUMERIC, 0, 4, "Date, Capture"},
    /*018*/ {ISO_NUMERIC, 0, 4, "Merchant type"},
    /*019*/ {ISO_NUMERIC, 0, 3, "Country code, Acquiring institution"},
    /*020*/ {ISO_NUMERIC, 0, 3, "Country code, Primary account number"},
    /*021*/ {ISO_NUMERIC, 0, 3, "Country code, Forwarding institution"},
    /*022*/ {ISO_ALPHANUMERIC, 0, 12, "Point of service data code"},
    /*023*/ {ISO_NUMERIC, 0, 3, "Card sequence number"},
    /*024*/ {ISO_NUMERIC, 0, 3, "Function code"},
    /*025*/ {ISO_NUMERIC, 0, 4, "Message reason code"},
    /*026*/ {ISO_NUMERIC, 0, 4, "Card acceptor business code"},
    /*027*/ {ISO_NUMERIC, 0, 1, "Approval code length"},
    /*028*/ {ISO_NUMERIC, 0, 6, "Date, Reconciliation"},
    /*029*/ {ISO_NUMERIC, 0, 3, "Reconciliation indicator"},
    /*030*/ {ISO_NUMERIC, 0, 24, "Amounts, original"},
    /*031*/ {ISO_ALPHANUMERIC, 2, 99, "Acquirer reference data"},
    /*032*/ {ISO_NUMERIC, 2, 11, "Acquirer institution identification code"},
    /*033*/ {ISO_NUMERIC, 2, 11, "Forwarding institution identification code"},
    /*034*/ {ISO_ALPHANUMERIC, 2, 28, "Primary account number, extended"},
    /*035*/ {ISO_ALPHANUMERIC, 2, 37, "Track 2 data"},
    /*036*/ {ISO_ALPHANUMERIC, 3, 104, "Track 3 data"},
    /*037*/ {ISO_ALPHANUMERIC, 0, 12, "Retrieval reference number"},
    /*038*/ {ISO_ALPHANUMERIC, 0, 6, "Approval code"},
    /*039*/ {ISO_NUMERIC, 0, 3, "Action code"},
    /*040*/ {ISO_NUMERIC, 0, 3, "Service code"},
    /*041*/ {ISO_ALPHANUMERIC, 0, 8, "Card acceptor terminal identification"},
    /*042*/ {ISO_ALPHANUMERIC, 0, 15, "Card acceptor identification code"},
    /*043*/ {ISO_ALPHANUMERIC, 2, 99, "Card acceptor name/location"},
    /*044*/ {ISO_ALPHANUMERIC, 2, 99, "Additional response data"},
    /*045*/ {ISO_ALPHANUMERIC, 2, 76, "Track 1 data"},
    /*046*/ {ISO_ALPHANUMERIC, 3, 204, "Amounts, Fees"},
    /*047*/ {ISO_ALPHANUMERIC, 3, 999, "Additional data - national"},
    /*048*/ {ISO_ALPHANUMERIC, 3, 999, "Additional data - private"},
    /*049*/ {ISO_ALPHANUMERIC, 0, 3, "Currency code, Transaction"},
    /*050*/ {ISO_ALPHANUMERIC, 0, 3, "Currency code, Reconciliation"},
    /*051*/ {ISO_ALPHANUMERIC, 0, 3, "Currency code, Cardholder billing"},
    /*052*/ {ISO_BINARY, 0, 8, "Personal identification number, PIN) data"},
    /*053*/ {ISO_BINARY, 2, 48, "Security related control information"},
    /*054*/ {ISO_ALPHANUMERIC, 3, 120, "Amounts, additional"},
    /*055*/ {ISO_BINARY, 3, 255, "IC card system related data"},
    /*056*/ {ISO_BINARY, 3, 255, "Original data elements"},
    /*057*/ {ISO_NUMERIC, 0, 3, "Authorization life cycle code"},
    /*058*/ {ISO_NUMERIC, 2, 11, "Authorizing agent institution Id Code"},
    /*059*/ {ISO_ALPHANUMERIC, 3, 999, "Transport data"},
    /*060*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*061*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*062*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for private use"},
    /*063*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for private use"},
    /*064*/ {ISO_BINARY, 0, 8, "Message authentication code field"},
    /*065*/ {ISO_BINARY, 0, 8, "Reserved for ISO use"},
    /*066*/ {ISO_ALPHANUMERIC, 3, 204, "Amounts, original fees"},
    /*067*/ {ISO_NUMERIC, 0, 2, "Extended payment data"},
    /*068*/ {ISO_NUMERIC, 0, 3, "Country code, receiving institution"},
    /*069*/ {ISO_NUMERIC, 0, 3, "Country code, settlement institution"},
    /*070*/ {ISO_NUMERIC, 0, 3, "Country code, authorizing agent Inst."},
    /*071*/ {ISO_NUMERIC, 0, 8, "Message number"},
    /*072*/ {ISO_ALPHANUMERIC, 3, 999, "Data record"},
    /*073*/ {ISO_NUMERIC, 0, 6, "Date, action"},
    /*074*/ {ISO_NUMERIC, 0, 10, "Credits, number"},
    /*075*/ {ISO_NUMERIC, 0, 10, "Credits, reversal number"},
    /*076*/ {ISO_NUMERIC, 0, 10, "Debits, number"},
    /*077*/ {ISO_NUMERIC, 0, 10, "Debits, reversal number"},
    /*078*/ {ISO_NUMERIC, 0, 10, "Transfer, number"},
    /*079*/ {ISO_NUMERIC, 0, 10, "Transfer, reversal number"},
    /*080*/ {ISO_NUMERIC, 0, 10, "Inquiries, number"},
    /*081*/ {ISO_NUMERIC, 0, 10, "Authorizations, number"},
    /*082*/ {ISO_NUMERIC, 0, 10, "Inquiries, reversal number"},
    /*083*/ {ISO_NUMERIC, 0, 10, "Payments, number"},
    /*084*/ {ISO_NUMERIC, 0, 10, "Payments, reversal number"},
    /*085*/ {ISO_NUMERIC, 0, 10, "Fee collections, number"},
    /*086*/ {ISO_NUMERIC, 0, 16, "Credits, amount"},
    /*087*/ {ISO_NUMERIC, 0, 16, "Credits, reversal amount"},
    /*088*/ {ISO_NUMERIC, 0, 16, "Debits, amount"},
    /*089*/ {ISO_NUMERIC, 0, 16, "Debits, reversal amount"},
    /*090*/ {ISO_NUMERIC, 0, 10, "Authorizations, reversal number"},
    /*091*/ {ISO_NUMERIC, 0, 3, "Country code, transaction Dest. Inst."},
    /*092*/ {ISO_NUMERIC, 0, 3, "Country code, transaction Orig. Inst."},
    /*093*/ {ISO_NUMERIC, 2, 11, "Transaction Dest. Inst. Id code"},
    /*094*/ {ISO_NUMERIC, 2, 11, "Transaction Orig. Inst. Id code"},
    /*095*/ {ISO_ALPHANUMERIC, 2, 99, "Card issuer reference data"},
    /*096*/ {ISO_BINARY, 3, 999, "Key management data"},
    /*097*/ {ISO_NUMERIC, 0, 1+16, "Amount, Net reconciliation"}, /* was ISO_AMOUNT */
    /*098*/ {ISO_ALPHANUMERIC, 0, 25, "Payee"},
    /*099*/ {ISO_ALPHANUMERIC, 2, 11, "Settlement institution Id code"},
    /*100*/ {ISO_NUMERIC, 2, 11, "Receiving institution Id code"},
    /*101*/ {ISO_ALPHANUMERIC, 2, 17, "File name"},
    /*102*/ {ISO_ALPHANUMERIC, 2, 28, "Account identification 1"},
    /*103*/ {ISO_ALPHANUMERIC, 2, 28, "Account identification 2"},
    /*104*/ {ISO_ALPHANUMERIC, 3, 100, "Transaction description"},
    /*105*/ {ISO_NUMERIC, 0, 16, "Credits, Chargeback amount"},
    /*106*/ {ISO_NUMERIC, 0, 16, "Debits, Chargeback amount"},
    /*107*/ {ISO_NUMERIC, 0, 10, "Credits, Chargeback number"},
    /*108*/ {ISO_NUMERIC, 0, 10, "Debits, Chargeback number"},
    /*109*/ {ISO_ALPHANUMERIC, 2, 84, "Credits, Fee amounts"},
    /*110*/ {ISO_ALPHANUMERIC, 2, 84, "Debits, Fee amounts"},
    /*111*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for ISO use"},
    /*112*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for ISO use"},
    /*113*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for ISO use"},
    /*114*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for ISO use"},
    /*115*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for ISO use"},
    /*116*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*117*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*118*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*119*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*120*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*121*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*122*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*123*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for private use"},
    /*124*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for private use"},
    /*125*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for private use"},
    /*126*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for private use"},
    /*127*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for private use"},
    /*128*/ {ISO_BINARY, 0, 8, "Message authentication code field"}
};

isodef iso87[] = {
    /*000*/ {ISO_NUMERIC, 0, 4, "Message Type Indicator"},
    /*001*/ {ISO_BITMAP, 0, 16, "Bitmap"},
    /*002*/ {ISO_NUMERIC, 2, 19, "Primary Account number"},
    /*003*/ {ISO_NUMERIC, 0, 6, "Processing Code"},
    /*004*/ {ISO_NUMERIC, 0, 12, "Amount, Transaction"},
    /*005*/ {ISO_NUMERIC, 0, 12, "Amount, Settlement"},
    /*006*/ {ISO_NUMERIC, 0, 12, "Amount, Cardholder Billing"},
    /*007*/ {ISO_NUMERIC, 0, 10, "Transmission Date and Time"},
    /*008*/ {ISO_NUMERIC, 0, 8, "Amount, Cardholder Billing"},
    /*009*/ {ISO_NUMERIC, 0, 8, "Conversion Rate, Settlement"},
    /*010*/ {ISO_NUMERIC, 0, 8, "Conversion rate, Cardholder Billing"},
    /*011*/ {ISO_NUMERIC, 0, 6, "System Trace Audit Number"},
    /*012*/ {ISO_NUMERIC, 0, 6, "Time, Local Transaction"},
    /*013*/ {ISO_NUMERIC, 0, 4, "Date, Local Transaction"},
    /*014*/ {ISO_NUMERIC, 0, 4, "Date, Expiration"},
    /*015*/ {ISO_NUMERIC, 0, 4, "Date, Settlement"},
    /*016*/ {ISO_NUMERIC, 0, 4, "Date, Conversion"},
    /*017*/ {ISO_NUMERIC, 0, 4, "Date, Capture"},
    /*018*/ {ISO_NUMERIC, 0, 4, "Merchant Type"},
    /*019*/ {ISO_NUMERIC, 0, 3, "Acquiring Institution Country Code"},
    /*020*/ {ISO_NUMERIC, 0, 3, "Primary Account Number (PAN) Country Code"},
    /*021*/ {ISO_NUMERIC, 0, 3, "Forwarding Institution Country Code"},
    /*022*/ {ISO_ALPHANUMERIC, 0, 3, "Point of Service Entry Mode"},
    /*023*/ {ISO_NUMERIC, 0, 3, "Card Sequence Number"},
    /*024*/ {ISO_NUMERIC, 0, 3, "Network International Identifier"},
    /*025*/ {ISO_NUMERIC, 0, 2, "Point of Service Condition Code (ISO)"},
    /*026*/ {ISO_NUMERIC, 0, 2, "Point of Service (POS) PIN Capture Code"},
    /*027*/ {ISO_NUMERIC, 0, 1, "Authorization Identification Response Length"},
    /*028*/ {ISO_ALPHANUMERIC, 0, 9, "Amount, Transaction Fee"},   									/*	review	*/
    /*029*/ {ISO_ALPHANUMERIC, 0, 9, "Amount, Settlement Fee"},										/* review	*/
    /*030*/ {ISO_ALPHANUMERIC, 0, 9, "Amount, Transaction Processing Fee"},					/*	review	*/
    /*031*/ {ISO_ALPHANUMERIC, 0, 9, "Amount, Settlement Processing Fee"},
    /*032*/ {ISO_NUMERIC, 2, 11, "Acquiring Institution Identification Code"},
    /*033*/ {ISO_NUMERIC, 2, 11, "Forwarding Institution Identification Code"},
    /*034*/ {ISO_ALPHANUMERIC, 2, 28, "Primary Account Number, Extended"},
    /*035*/ {ISO_ALPHANUMERIC, 2, 37, "Track 2 data"},														/*	review	*/
    /*036*/ {ISO_ALPHANUMERIC, 3, 104, "Track 3 data"},													/* review	*/
    /*037*/ {ISO_ALPHANUMERIC, 0, 12, "Retrieval Reference Number"},
    /*038*/ {ISO_ALPHANUMERIC, 0, 6, "Authorization Identification Response"},
    /*039*/ {ISO_ALPHANUMERIC, 0, 2, "Response Code"},
    /*040*/ {ISO_ALPHANUMERIC, 0, 3, "Service Restriction Code"},
    /*041*/ {ISO_ALPHANUMERIC, 0, 8, "Card Acceptor Terminal Identification"},
    /*042*/ {ISO_ALPHANUMERIC, 0, 15, "Card Acceptor Identification Code"},
    /*043*/ {ISO_ALPHANUMERIC, 0, 40, "Card Acceptor Name and Location"},
    /*044*/ {ISO_ALPHANUMERIC, 2, 25, "Additional Response Data"},
    /*045*/ {ISO_ALPHANUMERIC, 2, 79, "Track 1 Data"},
    /*046*/ {ISO_ALPHANUMERIC, 3, 999, "Additional Data (ISO)"},									/*	review	*/
    /*047*/ {ISO_ALPHANUMERIC, 3, 999, "Additional Data (National)"},							/*	review	*/
    /*048*/ {ISO_ALPHANUMERIC, 3, 100, "Additional Data"},											/*	review	*/
    /*049*/ {ISO_NUMERIC, 0, 3, "Currency Code, Transaction"},
    /*050*/ {ISO_NUMERIC, 0, 3, "Currency Code, Settlement	"},							/* review */
    /*051*/ {ISO_NUMERIC, 0, 3, "Currency Code, Cardholder Billing"},
    /*052*/ {ISO_BINARY, 0, 64, "Personal Identification Number (PIN) Data"},
    /*053*/ {ISO_BINARY, 0, 16, "Security Related Control Information"},
    /*054*/ {ISO_ALPHANUMERIC, 3, 120, "Additional Amounts"},
    /*055*/ {ISO_BINARY, 3, 255, "Integrated Circuit Card (ICC) System-Related Data"},
    /*056*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for ISO Use"},
    /*057*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for National Use"},
    /*058*/ {ISO_NUMERIC, 2, 11, "Authorizing Agent Institution ID"},
    /*059*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for National Use"},
    /*060*/ {ISO_ALPHANUMERIC, 3, 60, "Advice Reason Code"},										/* review	*/
    /*061*/ {ISO_ALPHANUMERIC, 3, 26, "Point of Service (POS) Data"},
    /*062*/ {ISO_ALPHANUMERIC, 2, 50, "Intermediate Network Facility (INF) Data"},
    /*063*/ {ISO_ALPHANUMERIC, 3, 44, "Network Data"},
    /*064*/ {ISO_BINARY, 0, 64, "Message Authentication Code (MAC)"},
    /*065*/ {ISO_BINARY, 0, 8, "Reserved for ISO use"},
    /*066*/ {ISO_ALPHANUMERIC, 3, 204, "Amounts, original fees"},
    /*067*/ {ISO_NUMERIC, 0, 2, "Extended payment data"},
    /*068*/ {ISO_NUMERIC, 0, 3, "Country code, receiving institution"},
    /*069*/ {ISO_NUMERIC, 0, 3, "Country code, settlement institution"},
    /*070*/ {ISO_NUMERIC, 0, 3, "Country code, authorizing agent Inst."},
    /*071*/ {ISO_NUMERIC, 0, 8, "Message number"},
    /*072*/ {ISO_ALPHANUMERIC, 3, 999, "Data record"},
    /*073*/ {ISO_NUMERIC, 0, 6, "Date, action"},
    /*074*/ {ISO_NUMERIC, 0, 10, "Credits, number"},
    /*075*/ {ISO_NUMERIC, 0, 10, "Credits, reversal number"},
    /*076*/ {ISO_NUMERIC, 0, 10, "Debits, number"},
    /*077*/ {ISO_NUMERIC, 0, 10, "Debits, reversal number"},
    /*078*/ {ISO_NUMERIC, 0, 10, "Transfer, number"},
    /*079*/ {ISO_NUMERIC, 0, 10, "Transfer, reversal number"},
    /*080*/ {ISO_NUMERIC, 0, 10, "Inquiries, number"},
    /*081*/ {ISO_NUMERIC, 0, 10, "Authorizations, number"},
    /*082*/ {ISO_NUMERIC, 0, 10, "Inquiries, reversal number"},
    /*083*/ {ISO_NUMERIC, 0, 10, "Payments, number"},
    /*084*/ {ISO_NUMERIC, 0, 10, "Payments, reversal number"},
    /*085*/ {ISO_NUMERIC, 0, 10, "Fee collections, number"},
    /*086*/ {ISO_NUMERIC, 0, 16, "Credits, amount"},
    /*087*/ {ISO_NUMERIC, 0, 16, "Credits, reversal amount"},
    /*088*/ {ISO_NUMERIC, 0, 16, "Debits, amount"},
    /*089*/ {ISO_NUMERIC, 0, 16, "Debits, reversal amount"},
    /*090*/ {ISO_NUMERIC, 0, 10, "Authorizations, reversal number"},
    /*091*/ {ISO_NUMERIC, 0, 3, "Country code, transaction Dest. Inst."},
    /*092*/ {ISO_NUMERIC, 0, 3, "Country code, transaction Orig. Inst."},
    /*093*/ {ISO_NUMERIC, 2, 11, "Transaction Dest. Inst. Id code"},
    /*094*/ {ISO_NUMERIC, 2, 11, "Transaction Orig. Inst. Id code"},
    /*095*/ {ISO_ALPHANUMERIC, 2, 99, "Card issuer reference data"},
    /*096*/ {ISO_BINARY, 3, 999, "Key management data"},
    /*097*/ {ISO_NUMERIC, 0, 1+16, "Amount, Net reconciliation"}, /* was ISO_AMOUNT */
    /*098*/ {ISO_ALPHANUMERIC, 0, 25, "Payee"},
    /*099*/ {ISO_ALPHANUMERIC, 2, 11, "Settlement institution Id code"},
    /*100*/ {ISO_NUMERIC, 2, 11, "Receiving institution Id code"},
    /*101*/ {ISO_ALPHANUMERIC, 2, 17, "File name"},
    /*102*/ {ISO_ALPHANUMERIC, 2, 28, "Account identification 1"},
    /*103*/ {ISO_ALPHANUMERIC, 2, 28, "Account identification 2"},
    /*104*/ {ISO_ALPHANUMERIC, 3, 100, "Transaction description"},
    /*105*/ {ISO_NUMERIC, 0, 16, "Credits, Chargeback amount"},
    /*106*/ {ISO_NUMERIC, 0, 16, "Debits, Chargeback amount"},
    /*107*/ {ISO_NUMERIC, 0, 10, "Credits, Chargeback number"},
    /*108*/ {ISO_NUMERIC, 0, 10, "Debits, Chargeback number"},
    /*109*/ {ISO_ALPHANUMERIC, 2, 84, "Credits, Fee amounts"},
    /*110*/ {ISO_ALPHANUMERIC, 2, 84, "Debits, Fee amounts"},
    /*111*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for ISO use"},
    /*112*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for ISO use"},
    /*113*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for ISO use"},
    /*114*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for ISO use"},
    /*115*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for ISO use"},
    /*116*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*117*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*118*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*119*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*120*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*121*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*122*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for national use"},
    /*123*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for private use"},
    /*124*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for private use"},
    /*125*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for private use"},
    /*126*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for private use"},
    /*127*/ {ISO_ALPHANUMERIC, 3, 999, "Reserved for private use"},
    /*128*/ {ISO_BINARY, 0, 8, "Message authentication code field"}
};

void iso8583_init(isomsg *m);	
int iso8583_set_fmtbitmap(isomsg *m, int bmp_flag);
int iso8583_get_bitmap(isomsg *m, isodef* def, char *buf);
int iso8583_pack(const isomsg *m, const isodef *d, char *buf);
int iso8583_unpack(isomsg *m, const isodef *d, const char *buf);
void iso8583_dump(FILE *fp, isomsg *m);
void iso8583_free(isomsg *m);

<<<<<<< .mine
/*!	\func	set data to a field of iso msg	*/
int iso8583_set_field(isomsg* msg, const isodef *def, int idx, void* fld, int len);
/*!	\func	get data from a field of iso msg	*/
int iso8583_get_field(const char* buf, const isodef *def, int idx, void* fld, int *len);

=======
>>>>>>> .r23
#endif /* iso8583.h */
