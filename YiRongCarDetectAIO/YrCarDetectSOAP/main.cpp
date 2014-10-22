// webserver.cpp : Defines the entry point for the console application.
//
#include "YrElecarDetect.nsmap"

int main(int argc, char **argv)
{ 
	SOAP_SOCKET m, s; /* master and slave sockets */
	
	struct soap soap;
	soap_init(&soap);
	if (argc < 2)
		soap_serve(&soap);	/* serve as CGI application */
	else
	{
		m = soap_bind(&soap, NULL, atoi(argv[1]), 100);
		if (!soap_valid_socket(m))
		{ 
			soap_print_fault(&soap, stderr);
			exit(-1);
		}
		fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
		for ( ; ; )
		{ 
			s = soap_accept(&soap);
			fprintf(stderr, "Socket connection successful: slave socket = %d\n", s);
			if (!soap_valid_socket(s))
			{ 
				soap_print_fault(&soap, stderr);
				exit(-1);
			} 
			soap_serve(&soap);
			soap_end(&soap);
		}
	}
	return 0;
} 

int ns__SendMission(struct soap *soap, char *MsgXml, char *ResponseXml)
{ 
	strcpy(ResponseXml,"aaa");
	return SOAP_OK;
} 

int ns__GetServerStatus(struct soap *soap, char *MsgXml, char *ResponseXml)
{ 
	strcpy(ResponseXml,"bbb");
	return SOAP_OK;
} 

int ns__AddCamera(struct soap *soap, char *MsgXml, char *ResponseXml)
{ 

	return SOAP_OK;
} 

int ns__IfCamera(struct soap *soap, char *MsgXml, char *ResponseXml)
{ 

	return SOAP_OK;
} 
int ns__DelCamera(struct soap *soap, char *MsgXml, char *ResponseXml)
{ 

	return SOAP_OK;
} 
