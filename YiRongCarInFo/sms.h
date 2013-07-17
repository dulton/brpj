#ifndef _SMS_H_
#define _SMS_H_


typedef int (__stdcall *SP_Startup)(char * Server, char * Account, char * Password); 
typedef void (__stdcall *SP_Cleanup)(void); 
typedef int (__stdcall *SP_Login) (char * Operator, char * Password); 
typedef int (__stdcall *SP_Logout) ();
typedef int (__stdcall *SubmitShortMessage) (char * AtTime, char * SourceAddr, char * DestAddr, char * Content, unsigned long ContentLen, int NeedStateReport, char * ServiceID, char * FeeType, char * FeeCode);
typedef int (__stdcall *SubmitShortMessageMulti)( char *  AtTime, char * SourceAddr,  char * DestAddrFiles, char * Content, unsigned long ContentLen, int NeedStateReport, char * ServiceID, char * FeeType, char * FeeCode);


class SMS
{
public:
	HMODULE	hSMDLL;

	SP_Startup	pfnSP_Startup; 
	SP_Cleanup	pfnSP_Cleanup; 
	SP_Login	pfnSP_Login;
	SP_Logout	pfnSP_Logout;

	SubmitShortMessage			pfnSubmitShortMessage;
	SubmitShortMessageMulti		pfnSubmitShortMessageMulti;

public:
	void smsStart(void);
	int smsInit(void);
	int smsLogin(void);


	int smsSendOne(char *dstPhoneNumber,char *msg,unsigned long msglen);
	int smsSendMulti(char *dstPhonePath,char *msg,unsigned long msglen);
	int smsSendMultiMy(char *dstPhonePath,char *msg,unsigned long msglen);


	int smsLogout(void);
	void smsUninit(void);
	void smsEnd(void);

};
#endif