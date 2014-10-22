

//gsoap ns service name:	YrElecarDetect
//gsoap ns service protocol:	SOAP
//gsoap ns service style:	rpc
//gsoap ns service encoding:	encoded
//gsoap ns service namespace:	http://localhost/YrElecarDetect.wsdl
//gsoap ns service location:	http://localhost/YrElecarDetect.cgi

//gsoap ns schema namespace:	urn:YrElecarDetect

int ns__SendMission(char *MsgXml, char *ResponseXml);

int ns__GetServerStatus(char *MsgXml, char *ResponseXml);

int ns__AddCamera(char *MsgXml, char *ResponseXml);

int ns__IfCamera(char *MsgXml, char *ResponseXml);

int ns__DelCamera(char *MsgXml, char *ResponseXml);
