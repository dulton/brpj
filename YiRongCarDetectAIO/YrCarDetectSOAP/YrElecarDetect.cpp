//放最前
#include "afx.h"

#include "soapH.h"
#include "xmlrw.h"

#include "init.h"
#include "io.h"
extern IO OracleIO;
extern struct GLOBAL_CONFIG_S GlobalConfig;

void ErrorStringXML(char* code,char* message,char *strText,int len)
{
	struct NAME_VALUE_S Rmapdata[2];
	Rmapdata[0].ins("Code",code);
	Rmapdata[1].ins("Message",message);

	if(false==CreateXmlLite_UTF8_NoSub("Return",Rmapdata,2,strText,len) )
	{
		strcpy(strText,"<Return><Code>-99</Code><Message>内部异常创建报文失败</Message></Return>");
	}
}


int ns__SendMission(struct soap *soap, char *MsgXml, char **ResponseXml)
{ 
	*ResponseXml = (char*)soap_malloc( soap,XMLRW_MAX_WSTR );
	char strText[XMLRW_MAX_WSTR]="";

	struct NAME_VALUE_S mapdata[3];
	mapdata[0].ins("DetectID","");
	mapdata[1].ins("CameraID","");
	mapdata[2].ins("Command","");

	if(ReadXML_UTF8_NoSub(MsgXml,mapdata,3))
	{

		if(mapdata[0].valuestr.length()==0 ||
			mapdata[1].valuestr.length()==0 ||
			mapdata[2].valuestr.length()==0 )
		{
			ErrorStringXML("-2","传入的报文字段参数缺失",strText,XMLRW_MAX_WSTR);
			goto ALL_EXIT;
		}

		long DetectID=-1;
		long CameraID=-1;
		long Command=-1;

		sscanf(mapdata[0].valuestr.c_str(),"%d",&DetectID);
		sscanf(mapdata[1].valuestr.c_str(),"%d",&CameraID);
		sscanf(mapdata[2].valuestr.c_str(),"%d",&Command);

		if(-1 == DetectID ||
			-1 ==CameraID ||
			-1 ==Command )
		{
			ErrorStringXML("-3","传入的报文字段参数有误",strText,XMLRW_MAX_WSTR);
			goto ALL_EXIT;
		}
		//检测摄像头是否存在
		if(false == OracleIO.DEVICE_IfCamera(CameraID))
		{
			ErrorStringXML("-4","摄像头不存在",strText,XMLRW_MAX_WSTR);
			goto ALL_EXIT;
		}
		//检测识别服务器是否存在
		if(false == OracleIO.DEVICE_IfDetectServer(DetectID))
		{
			ErrorStringXML("-5","识别服务器不存在",strText,XMLRW_MAX_WSTR);
			goto ALL_EXIT;
		}
	
		//测试识别服务器是否正常
		if(false == OracleIO.DEVICE_IfDetectServerEnable(DetectID))
		{
			ErrorStringXML("-6","识别服务器故障",strText,XMLRW_MAX_WSTR);
			goto ALL_EXIT;
		}
		
		if(Command)
		{
			long DetectDeviceID=0; 
			//查找是否有正在识别的摄像头
			if(false ==	OracleIO.DEVICE_SearchForStop(&DetectDeviceID,CameraID))
			{
				ErrorStringXML("-98","内部异常数据库出错",strText,XMLRW_MAX_WSTR);
				goto ALL_EXIT;
			}
			if(DetectDeviceID>0)
			{
				char strtemp[XMLRW_MAX_WSTR]="";
				sprintf(strtemp,"正在识别,服务器ID:%d",DetectDeviceID);
				ErrorStringXML("-7",strtemp,strText,XMLRW_MAX_WSTR);
				goto ALL_EXIT;
			}
			//测试识别服务器是否占用
			if(false == OracleIO.DEVICE_IfDetectServerPlay(DetectID))
			{
				ErrorStringXML("-8","识别服务器被占用",strText,XMLRW_MAX_WSTR);
				goto ALL_EXIT;
			}

		}
	
		if(false == OracleIO.Mission_ADD(DetectID,CameraID,GlobalConfig.UserID,Command))
		{
			ErrorStringXML("-98","内部异常数据库出错",strText,XMLRW_MAX_WSTR);
			goto ALL_EXIT;
		}

		ErrorStringXML("0","数据下发成功",strText,XMLRW_MAX_WSTR);
	}
	else
	{
		ErrorStringXML("-1","传入的报文无法解析",strText,XMLRW_MAX_WSTR);
		goto ALL_EXIT;
	}

ALL_EXIT:
	CHAR2UTF8(strText,*ResponseXml,XMLRW_MAX_WSTR);

	return SOAP_OK;
} 

//字符串最多支持100个识别服务器
int ns__GetServerStatus(struct soap *soap, char *MsgXml, char **ResponseXml)
{ 
	*ResponseXml = (char*)soap_malloc( soap,20480 );
	char strText[20480]="";

	list<DEVICE_LIST> DeviceList;
	DeviceList.clear();

	list<DEVICE_LIST>::iterator beglist;
	int i;

	struct NAME_VALUE_S Rmapdata[3];
	Rmapdata[0].ins("Code","0");
	Rmapdata[1].ins("Message","成功-无识别服务器");
	Rmapdata[2].ins("Total","0");

	if(false == OracleIO.DEVICE_ReadList(DeviceList))
	{
		ErrorStringXML("-98","内部异常数据库出错",strText,20480);
		goto ALL_EXIT;
	}

	if(DeviceList.size()<1)
	{
		if(false==CreateXmlLite_UTF8_NoSub("Return",Rmapdata,3,strText,20480) )
		{
			strcpy(strText,"<Return><Code>-99</Code><Message>内部异常创建报文失败</Message></Return>");
		}
		goto ALL_EXIT;
	}

	Rmapdata[0].ins("Code","0");
	Rmapdata[1].ins("Message","成功-有识别服务器");
	sprintf(strText,"%d",DeviceList.size());
	Rmapdata[2].ins("Total",strText);

	//二维类
	struct NAME_VALUE_S** Rsubdata;

	Rsubdata=(struct NAME_VALUE_S **)malloc(DeviceList.size()*sizeof(struct NAME_VALUE_S *));

	for(i=DeviceList.size()-1,beglist=DeviceList.begin();i>=0 && beglist!=DeviceList.end();i--,beglist++)
	{
		Rsubdata[i]= new struct NAME_VALUE_S[6];

		sprintf(strText,"%d",beglist->detect.id);
		Rsubdata[i][0].ins("DetectID",strText);
		Rsubdata[i][1].ins("ServerIP",beglist->detect.ip);
		sprintf(strText,"%d",beglist->detect.channel);
		Rsubdata[i][2].ins("Channel",strText);
		sprintf(strText,"%d",beglist->detect.isoccupy);
		Rsubdata[i][3].ins("IsOccupy",strText );
		sprintf(strText,"%d",beglist->detect.isenable);
		Rsubdata[i][4].ins("IsNormal",strText);
		sprintf(strText,"%d",beglist->camera.camid);
		Rsubdata[i][5].ins("CameraID",strText);
	}

	if(false==CreateXmlLite_UTF8_Sub("Return",Rmapdata,3,
		"DetectServerNo",DeviceList.size(),Rsubdata,6,
		strText,20480) )
	{
		strcpy(strText,"<Return><Code>-99</Code><Message>内部异常创建报文失败</Message></Return>");
	}
	
	//释放
	for(i=0;i<DeviceList.size();i++)
		delete[] Rsubdata[i];
	free(Rsubdata);


ALL_EXIT:
	CHAR2UTF8(strText,*ResponseXml,20480);

	return SOAP_OK;
} 

int ns__AddCamera(struct soap *soap, char *MsgXml, char **ResponseXml)
{ 
	*ResponseXml = (char*)soap_malloc( soap,XMLRW_MAX_WSTR );
	char strText[XMLRW_MAX_WSTR]="";

	struct NAME_VALUE_S mapdata[14];
	mapdata[0].ins("CamName","");
	mapdata[1].ins("CamIP","");
	mapdata[2].ins("Port","");
	mapdata[3].ins("User","");
	mapdata[4].ins("Password","");
	mapdata[5].ins("Channel","");
	mapdata[6].ins("VenderId","");
	mapdata[7].ins("RtspURL","");
	mapdata[8].ins("RTPMode","");
	mapdata[9].ins("DecodeTAG","");
	mapdata[10].ins("Longitude","");
	mapdata[11].ins("Latitude","");
	mapdata[12].ins("CrossIndex","");
	mapdata[13].ins("LaneId","");

	if(ReadXML_UTF8_NoSub(MsgXml,mapdata,14))
	{
		if(mapdata[0].valuestr.length()==0 ||
			mapdata[3].valuestr.length()==0 ||
			mapdata[4].valuestr.length()==0 ||
			mapdata[6].valuestr.length()==0 ||
			mapdata[12].valuestr.length()==0 ||
			mapdata[13].valuestr.length()==0 )
		{
			ErrorStringXML("-2","传入的报文字段参数缺失",strText,XMLRW_MAX_WSTR);
			goto ALL_EXIT;
		}
		long VenderId=-1;

		sscanf(mapdata[6].valuestr.c_str(),"%d",&VenderId);
		if(-1==VenderId)
		{
			ErrorStringXML("-3","传入的报文字段参数有误",strText,XMLRW_MAX_WSTR);
			goto ALL_EXIT;
		}
		else if(4 == VenderId)
		{
			//流媒体
			if(mapdata[7].valuestr.length()==0 ||
				mapdata[8].valuestr.length()==0 ||
				mapdata[9].valuestr.length()==0 )
			{
				ErrorStringXML("-2","传入的报文字段参数缺失",strText,XMLRW_MAX_WSTR);
				goto ALL_EXIT;
			}
		}
		else 
		{
			//私有
			if(mapdata[1].valuestr.length()==0 ||
				mapdata[2].valuestr.length()==0 ||
				mapdata[5].valuestr.length()==0 )
			{
				ErrorStringXML("-2","传入的报文字段参数缺失",strText,XMLRW_MAX_WSTR);
				goto ALL_EXIT;
			}
		}
		////////////////////
		int port=0;
		if( mapdata[2].valuestr.length() >0)
			sscanf(mapdata[2].valuestr.c_str(),"%d",&port);

		int channel=0;
		if( mapdata[5].valuestr.length() >0)	
			sscanf(mapdata[5].valuestr.c_str(),"%d",&channel);

		int venderID=0;
		sscanf(mapdata[6].valuestr.c_str(),"%d",&venderID);

		int RTP=0;
		if( mapdata[8].valuestr.length() >0)
			sscanf(mapdata[8].valuestr.c_str(),"%d",&RTP);

		int DecodeTAG=0;
		if( mapdata[9].valuestr.length() >0)
			sscanf(mapdata[9].valuestr.c_str(),"%d",&DecodeTAG);

		int LaneId=0;
		sscanf(mapdata[13].valuestr.c_str(),"%d",&LaneId);

		long long camid;
		if(false == OracleIO.DEVICE_AddNewCamera(GlobalConfig.AreaID,
			mapdata[0].valuestr.c_str(),mapdata[1].valuestr.c_str(),port,channel,
			mapdata[3].valuestr.c_str(),mapdata[4].valuestr.c_str(),venderID,
			mapdata[7].valuestr.c_str(),RTP,DecodeTAG,
			mapdata[10].valuestr.c_str(),mapdata[11].valuestr.c_str(),
			mapdata[12].valuestr.c_str(),LaneId,
			GlobalConfig.UserID,GlobalConfig.UserLV,
			&camid))
		{
			ErrorStringXML("-98","内部异常数据库出错",strText,XMLRW_MAX_WSTR);
			goto ALL_EXIT;
		}
		///////////////

		struct NAME_VALUE_S Rmapdata[3];
		Rmapdata[0].ins("Code","0");
		Rmapdata[1].ins("Message","成功");

		sprintf(strText,"%I64u",camid);
		Rmapdata[2].ins("CameraID",strText);

		if(false==CreateXmlLite_UTF8_NoSub("Return",Rmapdata,3,strText,XMLRW_MAX_WSTR) )
		{
			strcpy(strText,"<Return><Code>-99</Code><Message>内部异常创建报文失败</Message></Return>");
		}
	}
	else
	{
		ErrorStringXML("-1","传入的报文无法解析",strText,XMLRW_MAX_WSTR);
		goto ALL_EXIT;
	}



ALL_EXIT:
	CHAR2UTF8(strText,*ResponseXml,XMLRW_MAX_WSTR);

	return SOAP_OK;
} 

void IfCameraXML(char* code,char* message,char *flag,char *strText,int len)
{
	struct NAME_VALUE_S Rmapdata[3];
	Rmapdata[0].ins("Code",code);
	Rmapdata[1].ins("Message",message);
	Rmapdata[2].ins("Flag",flag);

	if(false==CreateXmlLite_UTF8_NoSub("Return",Rmapdata,3,strText,len) )
	{
		strcpy(strText,"<Return><Code>-99</Code><Message>内部异常创建报文失败</Message></Return>");
	}
}

int ns__IfCamera(struct soap *soap, char *MsgXml, char **ResponseXml)
{ 
	*ResponseXml = (char*)soap_malloc( soap,XMLRW_MAX_WSTR );
	char strText[XMLRW_MAX_WSTR]="";

	struct NAME_VALUE_S mapdata[1];
	mapdata[0].ins("CameraID","");

	if(ReadXML_UTF8_NoSub(MsgXml,mapdata,3))
	{
		if(mapdata[0].valuestr.length()==0  )
		{
			ErrorStringXML("-2","传入的报文字段参数缺失",strText,XMLRW_MAX_WSTR);
			goto ALL_EXIT;
		}

		long CameraID=-1;
		sscanf(mapdata[0].valuestr.c_str(),"%d",&CameraID);

		if(-1 ==CameraID  )
		{
			ErrorStringXML("-3","传入的报文字段参数有误",strText,XMLRW_MAX_WSTR);
			goto ALL_EXIT;
		}

		if(OracleIO.DEVICE_IfCamera(CameraID))
			IfCameraXML("0","成功-存在摄像头","1",strText,XMLRW_MAX_WSTR);
		else
			IfCameraXML("0","成功-不存在摄像头","0",strText,XMLRW_MAX_WSTR);
	}
	else
	{
		ErrorStringXML("-1","传入的报文无法解析",strText,XMLRW_MAX_WSTR);
		goto ALL_EXIT;
	}

ALL_EXIT:
	CHAR2UTF8(strText,*ResponseXml,XMLRW_MAX_WSTR);

	return SOAP_OK;
} 

int ns__DelCamera(struct soap *soap, char *MsgXml, char **ResponseXml)
{ 
	*ResponseXml = (char*)soap_malloc( soap,XMLRW_MAX_WSTR );
	char strText[XMLRW_MAX_WSTR]="";

	struct NAME_VALUE_S mapdata[1];
	mapdata[0].ins("CameraID","");


	if(ReadXML_UTF8_NoSub(MsgXml,mapdata,3))
	{
		if(mapdata[0].valuestr.length()==0  )
		{
			ErrorStringXML("-2","传入的报文字段参数缺失",strText,XMLRW_MAX_WSTR);
			goto ALL_EXIT;
		}

		long CameraID=-1;
		sscanf(mapdata[0].valuestr.c_str(),"%d",&CameraID);

		if(-1 ==CameraID  )
		{
			ErrorStringXML("-3","传入的报文字段参数有误",strText,XMLRW_MAX_WSTR);
			goto ALL_EXIT;
		}

		if(OracleIO.DEVICE_DeleteCamera(CameraID))
			ErrorStringXML("0","成功-删除成功",strText,XMLRW_MAX_WSTR);
		else
			ErrorStringXML("-98","内部异常数据库出错",strText,XMLRW_MAX_WSTR);
	}
	else
	{
		ErrorStringXML("-1","传入的报文无法解析",strText,XMLRW_MAX_WSTR);
		goto ALL_EXIT;
	}

ALL_EXIT:
	CHAR2UTF8(strText,*ResponseXml,XMLRW_MAX_WSTR);

	return SOAP_OK;
} 
