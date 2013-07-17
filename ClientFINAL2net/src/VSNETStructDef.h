//VSNETStructDef.h
#ifndef __VSNET_STRUCTDEF_H__
#define __VSNET_STRUCTDEF_H__

enum{
    VSNETALARMMSG_SERSTART = 1,	/* Server Start */	
    VSNETALARMMSG_MOTION,		/* Moving Detection Alarm */
    VSNETALARMMSG_VIDEOLOST,	/* Video Lost Alarm*/
    VSNETALARMMSG_SENSOR,		/* Sensor Alarm */
    VSNETALARMMSG_DISKFULL,		/* Disk Full Alarm */
    VSNETALARMMSG_HIDEALARM,	/* Video Hide Alarm  */	
    VSNETALARMMSG_SERSTOP,		/* Server Stop */
    VSNETALARMMSG_DISKERROR,    /* Disk Error Alarm(smart)*/
    VSNETALARMMSG_ACCESSVIOLATION, /*violative access*/
    VSNETALARMMSG_ANALYSESINGLELINEALARM = 11,      /* Intellective Analyses and single-line Alarm */
    VSNETALARMMSG_ANALYSEDOUBLELINEALARM,           /* Intellective Analyses and double-line Alarm */
    VSNETALARMMSG_ANALYSEREGIONENTRYALARM,          /* Alarm for entering Intellective Analyses Region */
    VSNETALARMMSG_ANALYSEREGIONEXITALARM,           /* Alarm for Leaving Intellective Analyses Region */
    VSNETALARMMSG_ANALYSEHOVERALARM,                /* Alarm for hovering Intellective Analyses Region */
    VSNETALARMMSG_SGLINEALARM = 100,                /* Alarm for Intellective Analyses caution line(>=100 and <=109，Now There are 10 rules for caution line*/
	VSNETALARMMSG_NODISK = 110,       /* No Disk*/
	VSNETALARMMSG_NET_BROKEN,   /*network line broken*/
	VSNETALARMMSG_USBSTATE = 200,                   /* DX USB status 0:plugin 1:plugout */
	VSNETALARMMSG_AUDIOCH = 201,					/* DX auidoch */
	VSNETALARMMSG_DXSWITCHCH = 202,					/* DX Swtch Channel*/
};

// record type
enum{
	HDISK_RECTYPE_HAND		= (1 << 0),
	HDISK_RECTYPE_TIMER		= (1 << 1),
	HDISK_RECTYPE_MOTION	= (1 << 2),
	HDISK_RECTYPE_ALARM		= (1 << 3),
	HDISK_RECTYPE_VILOST	= (1 << 4),
	HDISK_RECTYPE_VIHIDE	= (1 << 5),
	HDISK_RECTYPE_OTHER		= (1 << 6),
	HDISK_RECTYPE_BACKUP	= (1 << 7)
};

/*Transmit Customer's Rights*/
#define VSNET_USER_RIGHT_ADMIN		1	//Administrator Right
#define VSNET_USER_RIGHT_HIOPER		2	//High-grade Operator
#define VSNET_USER_RIGHT_OPERIAL	3	//Operator

#define VSNET_DVR_MAXCH             64  //Max number for channel
#define VSNET_DVR_MAXALARMOUT       16  //Max number for alarm output
#define VSNET_DVR_MAXTRACKNUM       16  //Max number for track
#define VSNET_DVR_MAXVONUM          8   //Max number for vo

/**********************************VSNET_DVR_GUI_PORT******************************************/
#define VSNET_DVR_GUI_PORT          10000   //DVR GUI内部通信端口10000~10999，DVR GUI专用
//10001:参数端口
//10002:串口数据
/************************************VSNET_DVR_GUI_PORT****************************************/

typedef struct
{
	char				m_username[20];		//Username
	char				m_password[20];		//Password
}ONEUSER;

typedef struct
{
    ONEUSER				m_admin;			//Administrator
    ONEUSER				hl_operator[10];	//High-grade Operator
    ONEUSER				m_operator[10];		//Operator
}WHOLE_USER;

typedef struct
{
    ONEUSER             m_adminsuper;
	ONEUSER				m_admin;			//Administrator
	ONEUSER				hl_operator[10];	//High-grade Operator
	ONEUSER				m_operator[10];		//Operator
}WHOLE_USERSUPER;

typedef struct
{
	int					bUseDDNS;			//DDNS Enable
	char				DDNSSerIp[40];		//DDNS Server IP
	unsigned short		DDNSSerPort;		//DDNS Server Port
	unsigned short		LocalMapPort;		//Local Port
}WHOLE_DDNS;

typedef struct
{
	char				pStrWebRomVer[50];		//Web Page Version
	char				pStrWebRomBuildTime[50];//Web Page Compile Time
}VERSIONWEBROM;

typedef struct
{
	char				pStrBSPVer[50];		//BSP Version
	char				pStrAPPVer[50];		//Application Version
	char				pStrBSPBuildTime[50];
											//BSP Compile Time
	char				pStrAPPBuildTime[50];
											//Application Compile Time
}VERSIONINFO;
typedef struct{
	char				m_servername[24];	//Server Name
	char				m_serverip[16];		//IP Address
	char				m_servermask[16];	//Subnet Mask 
	char				m_gatewayAddr[16];	//GateWay
	char				m_dnsaddr[16];		//DNS Address
	char				m_multiAddr[16];	//Multicasting Address	
	unsigned short		m_serport;			//Data Port
	unsigned short		m_mulport;			//Multicasting Port
	unsigned short		m_webport;			//Web Page Port
	unsigned char		m_isPAL;			//Video Mood(PAL,NTSC)
	unsigned char		m_launage;			//Language
	unsigned char		m_phyAddr[6];		//Physical Address
	unsigned short		m_reserved;         //reserved
	unsigned char		m_serial[8];		//Serial Number
	WHOLE_DDNS			m_ddns;				//DDNS Parameter
	VERSIONINFO			m_version;			//Version
}WHOLEPARAM;

typedef struct{
	char				m_servername[60];	//Server Name
	char				m_serverip[16];		//IP Address
	char				m_servermask[16];	//Subnet Mask
	char				m_gatewayAddr[16];	//GateWay
	char				m_dnsaddr[16];		//DNS Address
	char				m_multiAddr[16];	//Multicasting Address	
	unsigned short		m_serport;			//Data Port
	unsigned short		m_mulport;			//Multicasting Port
	unsigned short		m_webport;			//Web Page Port
	unsigned char		m_isPAL;			//Video Mood(PAL,NTSC)
	unsigned char		m_launage;			//Language
	unsigned char		m_phyAddr[6];		//Physical Address
	unsigned short		m_reserved;         //Reserved
	unsigned char		m_serial[8];		//Serial Number
	WHOLE_DDNS			m_ddns;				//DDNS Parameter
	VERSIONINFO			m_version;			//Version
}WHOLEPARAMEX;

typedef struct
{
	unsigned char		m_starthour;		//Start Hour
	unsigned char		m_startmin;			//Start Minute
	unsigned char		m_stophour;			//Stop Hour
	unsigned char		m_stopmin;			//Stop Hour
	unsigned char		m_maskweek;			//Week
	unsigned char		bReceive[3];		//Reserved
}TIMECHECK;

typedef struct
{
	TIMECHECK			pList[7];			//Time Table
}TIMELIST;

typedef struct
{
	unsigned char		m_beanb;             //Moving Detection Enable
	unsigned char		m_alarmsenstive;     //sensitivity of Moving Detection
	unsigned char		m_brecord;           //relate to recording
	unsigned char		bReceive;            //Reserved
	unsigned char		m_outputmap[8];      //relate to output
	unsigned char		m_detect[18][22];    //Detection Range
	TIMELIST			m_timelist;          //Detection Time Table
}CHANNMOTION;

typedef struct
{
	unsigned char		m_beanb;             //Moving Detection Enable
	unsigned char		m_alarmsenstive;     //sensitivity of Moving Detection
	unsigned char		m_brecord;            //relate to recording
	unsigned char		m_capjpeg;           //relate to jpeg capture image
	unsigned char		m_singlehost;        //Center for uploading
	unsigned char		m_reseved1;          //Reserved
	unsigned char		m_reseved2;          //Reserved
	unsigned char		m_reseved3;          //Reserved
	unsigned char		m_outputmap[8];      //relate to output
	unsigned char		m_detect[18][22];    //Detection Range
	TIMELIST			m_timelist;          //Detection Time Table
}CHANNMOTIONEX,CHANNVAEX;

typedef struct
{
	int					nFormat;			//Format of Display
	unsigned short		x,y;				//Coordinate of Display
}TIMEOSD;

typedef struct
{
	int					bShow;				//Show(1),Hide(0)
	unsigned short		x,y;				//Coordinate of Display
}BITSINFOOSD;

typedef struct
{
	char				lpString[32];		//characters to be showed	
	unsigned short		x,y;				//Coordinate
}TITALOSD;

typedef struct
{
	TIMEOSD				m_time;				//Time
	TITALOSD			m_tital[4];			//Title
	BITSINFOOSD			m_bits;				//Bit Rate
}CHANNOSD;

typedef struct
{
	int     	    m_enable;			        //label of channel OSD superposition for Intellective analyses alarm   0-Disable 1-Enable
	unsigned short	m_x,m_y;			        //Coordinate
    char	        m_tripwire_osdText[32];     //literal contents  of OSD superposition for alarm of caution line
    char            m_regionEnter_osdText[32];  //literal contents  of OSD superposition for alarm of entering region 
    char            m_regionLeave_osdText[32];  //literal contents  of OSD superposition for alarm of leaving region
    char            m_regionWander_osdText[32]; //literal contents  of OSD superposition for alarm of hovering region
}VSNETCHVAALARMOSD;

typedef struct
{
	int     	       m_enable;		  //label of Intellective analyses alarm  OSD superposition   0-Disable 1-Enable
	VSNETCHVAALARMOSD  m_chAlarmOsd[8];	  //setting of OSD superposition for channel Intellective analyses alarm
}VSNETVAALARMOSD;

typedef struct
{
	int					m_bmask;			//Image Mask Enable
	unsigned short		m_x;				//x
	unsigned short		m_y;				//y
	unsigned short		m_width;			//width
	unsigned short		m_height;			//height
}CHANNVIDEOMASK;

typedef struct
{
	unsigned char		m_bri;				//Image Brightness
	unsigned char		m_con;				//Image Contrast
	unsigned char		m_sat;				//Image Saturation
	unsigned char		m_hue;				//Image Hue
}CHANNVIPARAM;

typedef struct
{
	char				m_channelName[16];	//Channel Name
   	int					m_streamType;		//Stream Type:Video Stream、AV Stream
   	int					m_encodeType;		//Image Type:CIF,2CIF,4CIF
	int					m_Iinterval;		//key frame interval(10-200)
	int					m_videoFrameRate;	//Frame rate
	int					m_bitratetype;		//bit rate type：CBR & VBR
	int					m_maxqueue;			//VBR Quantified Coefficient(2-31)
	int					m_minqueue;			//CBR Quantified Coefficient(2-31)
	int					m_maxbitrates;		//CBR Max bit rate(64K-8000K)
	int					m_audiosample;		//AF sampling frequency
	int					m_audiobitrate;		//AF bit rate
	int					m_delay;			//Alarm Delay
	int					m_benrecord;		//Regular Recording Enable
	TIMELIST			m_record;			//Regular Recording Time Table
}CHANNELPARAM;

typedef struct
{
	char				m_channelName[60];	//Channel Name
   	int					m_streamType;		//Stream Type:Video Stream、AV Stream
   	int					m_encodeType;		//Image TypeCIF,2CIF,4CIF
	int					m_Iinterval;		//key frame interval(10-200)
	int					m_videoFrameRate;	//Frame rate
	int					m_bitratetype;		//bit rate type：CBR & VBR
	int					m_maxqueue;			//VBR Quantified Coefficient(2-31)
	int					m_minqueue;			//CBR Quantified Coefficient(2-31)
	int					m_maxbitrates;		//CBR Max bit rate(64K-8000K)
	int					m_audiosample;		//AF sampling frequency
	int					m_audiobitrate;		//AF bit rate
	int					m_delay;			//Alarm Delay
	int					m_benrecord;		//Regular Recording Enable
	TIMELIST			m_record;			//Regular Recording Time Table
}CHANNELPARAMEXNAME;

typedef struct
{
	char				m_channelName[16];	//Channel Name
   	int					m_streamType;		//Stream Type:Video Stream、AV Stream
   	int					m_encodeType;		//Image TypeCIF,2CIF,4CIF
	int					m_Iinterval;		//key frame interval(10-200)
	int					m_videoFrameRate;	//Frame rate
	int					m_bitratetype;		//bit rate type：CBR & VBR
	int					m_maxqueue;			//CBR Quantified Coefficient(2-31)
	int					m_minqueue;			//VBR Quantified Coefficient(2-31)
	int					m_maxbitrates;		//CBR Max bit rate(64K-8000K)
	int					m_audiosample;		//AF sampling frequency
	int					m_audiobitrate;		//AF bit rate
	int					m_delay;			//Alarm Delay
	int                 m_nPrerecordTime;   //Prerecording Time(0=off,1-10=ON)
	int                 m_videofilebackup;  //Video File Backup(0=OFF,1=ONLY Alarm File,2=All Files)
	int                 m_jpegfilebackup;	//Jpeg File Backup(0=OFF,1=ONLY Alarm File,2=All Files)
	int					m_benrecord;		//Regular Recording Enable
	TIMELIST			m_record;			//Regular Recording Time Table
}CHANNELPARAMEX;

typedef struct{
	int					m_baudrate;			//baud rate
	int					decoderModel;       //Protocol Type(Not Use)
	unsigned char		m_databit;			//Data bit
	unsigned char		m_stopbit;			//Stop bit
	unsigned char		m_checkbit;			//Check bit
	unsigned char		m_flowcontrol;		//Flow Control
	unsigned char		decoderAddress;		//Decoder Address
	unsigned char		bReceive[3];		//Reserved
}SERIAL485_PARAM;

typedef struct{
	int					baudrate;			//baud rate
	char				databit;			//data bit
	char				stopbit;            //stop bit
	char				checkbit;			//check bit
	char				flowcontrol;        //flow control
}VSSERIAL_INFO;

typedef struct
{
	int					m_year;				//Year
	unsigned char		m_month;			//Month
	unsigned char		m_dayofmonth;		//Day
	unsigned char		m_dayofweek;        //Week
	unsigned char		m_hour;             //Hour
	unsigned char		m_minute;			//Minute
	unsigned char		m_second;			//Second
	unsigned char		bReceive[2];		//Reserved
}TIME_PARAM;

typedef struct
{
	unsigned char		m_record[16];		//relate to recording
	unsigned char		m_out[8];			//relate to output
	unsigned char		m_enpreno[16];		//relate to preset point
	unsigned char		m_preno[16];		//number of preset point to be called
	TIMELIST			m_timelist;			//detection time table
}ALARMRECORD;

typedef struct
{
	int					m_alarmmode;		//Alarm Mode 0：OFF,1：ON
	int					m_singlehost;		//label of center for uploading alarm signal
	unsigned char		m_record[16];		//relate to recording
	unsigned char		m_out[8];			//relate to output
	unsigned char		m_enpreno[16];		//relate to preset point
	unsigned char		m_preno[16];		//number of preset point to be called
	unsigned char		m_capjpeg[16];		//relate to capturing
	TIMELIST			m_timelist;			//detection time table
}ALARMRECORDEX;

typedef struct
{
	int					m_benab;			//Alarm Enable
	ALARMRECORD			m_alarmrec[8];		//parameter of relating to alarm
}ALARMPARAM;

typedef struct
{
	int					m_benab;            //Alarm Enable
	ALARMRECORDEX		m_alarmrec[8];      //parameter of relating to alarm
}ALARMPARAMEX;

typedef struct{
	int					m_chanuser[16];		//Channel
	unsigned char		m_chanIP[16][20][4];//Linked IP Address
}CONNECTIP;

typedef struct{
	unsigned int		m_videotag;         //Video tag
	short				m_width,m_height;   //Video Size
	int					m_bhaveaudio;       //have audio or not   
	int					m_samplesize;       //audio sampling frequency
	unsigned short		m_audiotag;         //audio tag
	unsigned short		m_audiobitrate;		//audio bit rate
}VSTREAMINFO;

typedef struct{
	unsigned int		m_url;				//URL address
	char				m_sername[24];		//Server Address
	int					m_ch;				//channel
	int					trantype;			//transport type 
	int 				handle;				//handle
}VSUSERINFO;

typedef struct
{
	int					m_benable;			//hide detection enable
	int					m_decval;			//sensitivity of detection(1(lowest)---3(highest))
	unsigned char		m_out[8];			//output of relating alarm
	TIMELIST			pTimeList;			//detection time
}CHANNELHIDEALARM;

typedef struct
{
	int					m_benable;			//hide detection enable
	int					m_bsinglehost;		//label of center for alarm signal
	int					m_decval;			//sensitivity of detection(1(lowest)---3(highest))
	unsigned char		m_out[8];			//output of relating alarm  
	TIMELIST			pTimeList;			//detection time
}CHANNELHIDEALARMEX;

typedef struct
{
	int					m_benbale;			//Regular Restart Enable
	int					m_invalidtime;		//interval time(HOUR AS UNIT，10h-240h:10 hours to 10 days)
	int					m_rsthour;			//Restart Hour
	int					m_rstminute;		//Restart Minute
}VSTIMERRESET;

typedef struct
{
	char				m_hostserverurl[40];   
	                                        // IP of Recording Server 
	char				m_alarservermurl[40];    
	                                        //IP of Alarm Server
	unsigned short		m_hostserverport;   //Port of Recording Server
	unsigned short		m_alarserverport;   //Port of Alarm Server
}VSREMOTEHOST;

typedef struct
{
	int					m_streamType;		//Stream Type:Video Stream、AV Stream
   	int					m_encodeType;		//Image TypeCIF,2CIF,4CIF
	int					m_Iinterval;        //Key Frame interval(10-200)
	int					m_videoFrameRate;   //frame rate
	int					m_bitratetype;      //bit rate type：CBR & VBR
	int					m_maxqueue;         //VBR Quantified Coefficient(2-31)
	int					m_minqueue;         //CBR Quantified Coefficient(2-31)
	int					m_maxbitrates;      //CBR max bit rate(64K-8000K)
}VSSUBCHANPARAM;

typedef struct
{
	int					m_hds;			   // Total of Hard Disk
	int					m_hdtype[8];	   //Disk Type
	int					m_hdstate[8];      //Disk State 0xff-错误，0xfe-空硬盘
	int					m_totalsize[8];	   //Disk Size
	int					m_freesize[8];	   //free size 
}VSDISKSTATE;

typedef struct 
{
	unsigned int		m_channel;         //Channel NO.
	unsigned short		m_detectall;       //all alarm
	unsigned char		m_detect[18][22];  //alarm block
}VSNETVIDEOMOVE_BLOCK;

typedef struct 
{
	int					m_busepppoe;		//USE PPPOE
	char				m_pppoename[64];	//PPPOE username
	char				m_pppoepswd[64];	//PPPOE password
}VSNETPPPOEPARAM;

typedef struct
{
    int      m_packet_type;                 //packet type of recording(0:packing with file size; 1:packing with time)
    int 	 m_packet_size;                 //packet size of recording，MB as UNIT
    int      m_packet_time;                 //packet time of recording，Minute as UNIT
    int 	 m_hdisk_reserved_size;         //Reserved Space of DISK，MB as UNIT
    int      m_hdisk_full_alarm;            // whether alarm when free space less than reserved space of hard disk
    int      m_hdisk_full_action;           //the action when free space less than reserved space of hard disk(1:circular covering; 0:stop recording)
}VSNETRECORDPARAM;

typedef struct
{
	int					m_bsinglehost;		//label of center for uploading
	unsigned char		m_out[8];			//alarm output
	TIMELIST			m_timelist;			//detection time
}VSNETVIDEOLOSTPARAM;

typedef struct
{
	unsigned char		m_caprate;			//frame rate of capturing (1-5)
	unsigned char		m_encquant;			//quality of capturing(JPEG,1-100)
	unsigned char		m_encformat;		//format of capturing(CIF,2CIF,4CIF)
	unsigned char		m_reserved;			//reserved     
	int					m_capnums;			//frame number to be captured(-1:keep capturing)
}VSNETJPEGCAPPARAM;					    

typedef struct
{
	char				m_serverIP[40];		//Server IP
	unsigned int		m_port;				//Server Port
	unsigned int		m_intervaltime;		//interval time
	char				m_senddata[512];	//data to be sent
}VSNETHOSTNOTIFY;

typedef struct
{
	char				m_alarmname[32];	//name of alarm sensor
	int                 m_senseorinterval;  //interval time for sensor alarming
}VSNETSENSORALARM;

typedef struct
{
	int  m_videolossinterval;	//interval time for video losing
	int  m_videomoveinterval;	//interval time for video moving
	int  m_videohideinterval;	//interval time for video hiding
	int  m_reserved;		    //reserved
}VSNETALARMINTERVAL;            //interval time for alarming

typedef struct
{
	int		m_alarmtype;		//sensor type
}ALARMTYPE;

typedef struct
{
	int m_audioinline;		//1:linear input  0:Mic input
	int m_audioinDBline;    //proportion of linear input(-34 - +12DB) range of linear input(0-100)
	int m_audioinDBMic;     //proportion of Mic input(0,20DB)  range of Mic input(0-1)
	int m_audiooutDB;       //proportion of input(-73 - +6DB)  range of  input(0-100)
}VSNETAUDIOPARAM;	        //structure of audio parameter 

typedef struct
{
    int m_audiosilent;     //silent:0， not:others
}VSNETAUDIOSILENT;	        //structure of audio silent

typedef struct 
{
    int             m_usecdma;      //use cdma
}VSNETCDMAPARAM;

typedef struct 
{
    int             m_usewifi;			//use wifi
    char	    	m_wifiipaddr[16];   //wireless IP address
    char	    	m_wifinetmask[16];  //wireless MASK address
    char	    	m_wifigateway[16];  //wireless gateway
    char            m_wifidns[16];      //wireless DNS
    char            m_wifissid[40];     //wireless ssid
    int             m_wifiencmode;      //encription mode 0,no encription 1,wep-60  2,wep-128
    char            m_wifipwd[64];      //cryptographic key wep-60(5 ASCII or 10 HEX) wep-128(13 ASCII or 26 HEX) 
}VSNETWIFIPARAM;

typedef struct 
{
    int             m_usewifi;			//use wifi 0,OFF 1,Static 2,ppoe 3,DHCP
    int             m_usedefaultgw;     //wireless gateway as default enable (zero,disable nonzero,enable)
    char            m_pppoename[64];    //wireless PPPOE username
    char            m_pppoepswd[64];    //wireless PPPOE password 
    char	    	m_wifiipaddr[16];   //wireless IP address
    char	    	m_wifinetmask[16];  //wireless MASK address
    char	    	m_wifigateway[16];  //wireless gateway
    char            m_wifidns[16];      //wireless DNS
    char            m_wifissid[40];     //wireless ssid
    int             m_wifiencmode;      //encription mode 0,no encription 1,wep-60  2,wep-128 3,wep-152 8,wpa-psk/wpa2-psk
    char            m_wifipwd[64];      //cryptographic key wep-60(5 ASCII or 10 HEX) wep-128(13 ASCII or 26 HEX) 
}VSNETWIFIPARAMEX;

typedef struct 
{
    char            m_wifissid[40];     //wireless ssid
    int             m_wifiencmode;      //encription mode      0-no encription 1-WEP 2-WPA-PSK/WPA2-PSK
}VSNETWIFISSIDINFO;

#define MAX_SSIDNUM 40
typedef struct 
{
    int                 m_ssidnum;      // number of wireless router which have been searched 
    VSNETWIFISSIDINFO   m_ssidinfo[MAX_SSIDNUM]; //route information
}VSNETWIFISSIDLIST;

typedef struct
{
	char			m_deviceID[32];		//Device ID
	int				m_maxconnect;		//max connection number,computing with 5 per channel
	int				m_linktype;			//link type,1:LAN,2:ADSL,3:others
	char			m_ftpuser[20];		//ftp username
	char			m_ftppass[20];		//ftp password
	char			m_devicemodel[32];  //device model
	int				m_sipport;          //sip port
}VSNETLINKCONFIG;						//link configuration

typedef struct
{
	int				m_dnsprovider;     //ddns provider
	int             m_ddnsnotifytime;  //ddns update time
	char			m_dnsusername[20]; //ddns username
	char			m_dnspassword[20]; //ddns password
	char            m_dnsdomainname[50]; //dns domain name
}VSNETDNSEXPARAM;

typedef struct
{
	int				m_comport;				//serial port NO.
	int				m_protocol;				//protocol of serial port
	int				m_addr;					//device address
	int				m_alarmchecktime;		//time of alarm check
	int				m_alarmintervaltime;	//interval time of alarm
}VSNETEXALARMPARAM;

typedef struct
{
	int				m_benablecheck;			//temperature alarm enable
	TIMELIST    	m_timeList;				//detection time
	char            m_boutput[8];			//alarm output,each bit maps alarm output
	char			m_record[16];           //recording label
	char			m_capture[16];          //capturing label
	int             m_delay;				//alarm delay time
	int             m_singlehost;			//center  for uploading
	float           m_tempmax;              //max temperature
	float           m_tempmin;              //min temperature
	float           m_hummax;               //max humidity
	float           m_hummin;               //min humidity
}VSNETHUMITUREPRAM;

typedef struct
{
	int             m_xoffset;              //horizontal offset 0~32
}VSNETVIDEOOFFSET;

typedef struct
{
	unsigned short		m_x;				//x
	unsigned short		m_y;				//y
	unsigned short		m_width;			//width
	unsigned short		m_height;			//height
}VSNETVIDEOMASKAREA;

typedef struct
{
	int					m_bmask;			// image mask enable
    VSNETVIDEOMASKAREA  m_maskarea[4];      // image mask area
}VSNETVIDEOMASK;

typedef struct
{
	char				mans_url[128];       //address of manager server 
    char				mans_username[32];   //username of manager server 
    char				mans_psd[32];        //password of manager server 
    char				mans_conrequrl[128]; //inverse link PU address
    char				mans_conreqname[32]; //inverse linkPU username
    char				mans_conreqpsd[32];  //inverse link PU password
}VSNETSNMPCONFIG;

typedef struct
{	
	int					m_nyear;			/* received time  */
	unsigned char		m_nmonth;
	unsigned char		m_nday;
	unsigned char		m_nhour;
	unsigned char		m_nminute;
	unsigned char		m_nsecond;
	unsigned char		m_received[3];		/* reserved      */
	char				m_peplenum[100];	/* supercargo number*/
	char				m_boxnum[100];		/* box number*/
	unsigned short		m_recvnum;			/*  received box number */
	unsigned short		m_sendnum;			/*  sent box number  */
}NETRVS_BANKINFO;

typedef struct
{
    int             m_CapJpgEnable;			//regular capturing enable
    int             m_ftp_timecapjpg;       //regular capturing FTP uploading
    int             m_CapJpgInterval;       // interval time for regular capturing
    TIMELIST    	m_timeList;				//capture time
}VSNETTIMERCAP;

typedef struct
{
    int             m_busedhcp; 			//use DHCP
}VSNETUSEDHCP;

typedef struct 
{
	char			m_pppoename[64];    	//wifi PPPOE username
	char			m_pppoepswd[64];    	//wifi PPPOE password
}VSNETWIFIPPPOEPARAM;


typedef struct
{
	int m_busedefaultaddr;	//use default address or not
	int m_busealarmsms;		//message alarm enable
	int m_interval;			//min interval for sending
}VSNETALARMSMSPARAM;	    // parameter structure of message for setting alarm


/***************** long-distance retrieval,download,play ****************/
#define  MAX_FILE_DATA            10

typedef struct{
	int					m_year;				//year
	unsigned char		m_month;			//month
	unsigned char		m_day;				//day
	unsigned char		m_hour;				//hour
	unsigned char		m_minute;			//minute
	unsigned char		m_second;			//second
	unsigned char		Received[3];		//reserved
}VSNETPLAY_NETTIME;

typedef struct
{
	int					m_channel;			//channel NO.
	char				sFileName[100];		//file name
	VSNETPLAY_NETTIME	struStartTime;		//start time of file
	VSNETPLAY_NETTIME	struStopTime;		//end time of file
	unsigned int		dwFileSize;			//file size
}VSNETPLAY_FIND_DATA;

typedef struct
{
	int                 m_filenum;         //file number
	VSNETPLAY_FIND_DATA m_filedata[MAX_FILE_DATA];        
	                                       //file attribute
}VSNETPLAY_FILE_DATA;

typedef struct
{
	char				desc[200];
}VSNETALARMHOST_DESC;



/*************************FTP uploading parameter********************************/
typedef struct 
{
    int					m_enable_ftp;                //label of FTP upload
    char				m_remote_IP[32];             //IP address
    unsigned short      m_remote_port;               //port
    unsigned short		m_reserved;                  //reserved for alignment 
    char				m_ftp_user[40];              //Ftp username  
    char				m_ftp_pwd[40];               //Ftp password
}VSNETFTPINFO;

/*************************UPNP parameter********************************/
typedef struct 
{
    int					m_enable_upnp;				  //upnp enable
}VSNETUPNPCFG;

typedef struct 
{
    int 	            m_upnp_status;				   //upnp status
}VSNETUPNPSTATUS;

/*************************MAIL parameter********************************/
#define SEND_MAIL_MAX_ADDR_NUM              0x3
#define SEND_MAIL_MAX_ATTACH_NUM            0x3
#define SEND_MAIL_MAX_SUBJECT_LEN           0x40
#define SEND_MAIL_MAX_CONTENT_LEN           0x100
#define SEND_MAIL_STRING_LEN                0x40
#define SEND_MAIL_NAME_LEN                  0x20
#define SEND_MAIL_ADDR_LEN                  0x40


typedef struct
{
    char        m_mail_name[SEND_MAIL_NAME_LEN];    //sender name
    char        m_mail_addr[SEND_MAIL_ADDR_LEN];    //sender address
}VSNETMAILADDR;

typedef struct 
{
    int					m_enable_sendmail;                           //sending alarm mail enable
	char				m_server_addr[SEND_MAIL_ADDR_LEN];           //mail server address    
    int		            m_server_port;                               //mail server port，25 as default    
    char				m_auth_name[SEND_MAIL_STRING_LEN];           // mail verification name     
    char				m_auth_pwd[SEND_MAIL_STRING_LEN];            //mail verification password
    
    int		            m_priority;                                  //priority of mail
    char                m_subject[SEND_MAIL_MAX_SUBJECT_LEN];        //title of mail
    char                m_content[SEND_MAIL_MAX_CONTENT_LEN];        //content of mail
    VSNETMAILADDR       m_from_addr;                                 //sender address
    VSNETMAILADDR       m_send_addr[SEND_MAIL_MAX_ADDR_NUM];         //receiver address 

}VSNETMAILCFG;

/*************************CCD parameter ********************************/
typedef struct
{
    int              m_shuttermode;      //shutter mode     0:AES; 1:MES
    int              m_shtspeed;         //shutter speed 0~255
    int              m_aelevel;          //AE level       0~255
    
    int              m_awb;              //balance mode   0~255
    int              m_wbr;              //balance by hand R  0~255
    int              m_wbg;              //balance by hand G  0~255
    int              m_wbb;              //balance by hand B  0~255
    
    int              m_vflip;            //vertical overset    0:OFF; 1:ON
    int              m_hmirror;          //horizontal overset     0:OFF; 1:ON
    
    int              m_exposuremode;     //exposure mode     0:AE; 1:ME
    int              m_blc;              // back light compensation    0:OFF; 1:ON
    int              m_agc;              //automatic gain     0~255
    int              m_iris;             //iris        0~255
    int              m_toblackwhite;     //color to black-and-white       0:automatic; 1:infrared(night)
    int              m_zoom;             //zoom         0:OFF; 1:ON
    
    int              m_lightmode;        // switch between day and night     0~255
    int              m_flc;              //              0:OFF; 1:ON
    int              m_switch;           //switch between positive and negative    0:Posi 1:Nega
    int              m_sense;            //              0:OFF; 1:ON
    int              m_sensemax;         //              0:x2; 1:x4; ...; 7:x256
    int              m_prio;             //              0:AGC; 1:Sense up
}VSNETCCDPARAM;

/************************APP platform configuration parameter*******************************/
typedef struct
{
    unsigned char       m_register_type;                //register type
    unsigned char       m_enable_register;              //register enable
    short				m_reserved;                     //reserved for alignment
    unsigned int		m_register_interval;            //interval time for register，Second as UNIT，0 means only register once when device start 
    unsigned char       m_sessionid[128];               //platform session ID
    unsigned char       m_store_sessionid[128];         //store session ID
}VSNETPLATFORM_REGISTER_STRATEGY;

typedef struct
{
    unsigned char           m_heartbeat_type;            //heart beat type  0 udp  1 tcp
    unsigned char           m_enable_heartbeat;          //heart beat enable
    short					m_reserved;                  //reserved for alignment
    unsigned int			m_heartbeat_interval;        //interval time for heart beat ,Second as UNIT
}VSNETPLATFORM_HEARTBEAT_STRATEGY;

/*************************IP-PORT pair structure********************************/
typedef struct
{
    unsigned char           m_ipaddr[32];   //ip
    unsigned int	        m_port;         //port
}VSNETPLATFORM_IPPORT;

typedef struct
{
    VSNETPLATFORM_IPPORT       m_mannage_server;   //configuration of manager server
    VSNETPLATFORM_IPPORT       m_register_server;  //configuration of register server
    VSNETPLATFORM_IPPORT       m_heartbeat_server; //configuration of heart beat server
    VSNETPLATFORM_IPPORT       m_alarm_server;     //configuration of alarm server
    VSNETPLATFORM_IPPORT       m_store_server;     //configuration of store server
}VSNETPLATFORM_SERVERINFO;

#define VSNET_MAX_PLATFORM_IPPAIR   4
/*************************platform link configuration********************************/
typedef struct
{
    unsigned  int                m_platformtype;				//platform type
    char                         m_deviceID[32];				//device ID
    unsigned short               m_maxconnet;					//max connection number，computing with 5 per channel
    unsigned short               m_linktype;					//link type,1:LAN,2:ADSL,3:others
    unsigned short               m_msgport;						//message port
    unsigned short               m_videoport;					//video port
    unsigned short               m_audioport;					//audio port
    unsigned short               m_talkport;					//talk port
    unsigned short               m_updateport;					//update port
    short							   m_reserved;             //reserved for alignment
    VSNETPLATFORM_SERVERINFO           m_platform_serverinfo;  //information of platform server
    VSNETPLATFORM_REGISTER_STRATEGY    m_platform_reg;         //information of platform register
    VSNETPLATFORM_HEARTBEAT_STRATEGY   m_platform_heatbeat;    //information of platform heart beat
}VSNETPLATFORM_INFO;
/*********************************************************/

typedef struct
{
	unsigned char				 m_enable_vo;					//enable image circle or not
	unsigned char                m_enable_vo_vga;
    unsigned char                m_vo_vga_res;
	unsigned char				 m_reserved[1];					//reserved for alignment
}VSNETVOCFG;

typedef struct
{
	unsigned char				 m_upload_record;				//upload record
	unsigned char				 m_upload_capjpeg;				//upload captured image
	unsigned char				 m_reserved[2];					//reserved for alignment
}VSNETFTPUPLOAD;

typedef struct
{
    char	m_ccCode[32];										//manufacturer code
    char	m_devID[32];										//device ID
    char	m_devType[32];										//device type
    char	m_reserved[256];									//reserved for extension
}VSNETDEVINFO_HH;

typedef struct
{
    int		m_run_seconds;										//run time,second as UNIT
}VSNETRUNTIME_HH;

typedef struct
{
	VSNETFTPUPLOAD				 m_ftp_upload[16];               // channel FTP upload parameter
}VSNETALARMFTPUPLOAD;

typedef struct
{
    unsigned char     m_alarm_rec[16];                           //alarm relate to recording 
    unsigned char     m_alarm_capjpeg[16];                       //alarm relate to  JPEG capture
    unsigned char     m_ftp_alarmrec[16];                        //uploading recording of alarm relating to FTP
    unsigned char     m_ftp_alarmcapjpeg[16];                    //uploading capturing image of alarm relating to FTP
}VSNETALARMCONTACTEX;

typedef struct
{
    unsigned char     m_bEnable;                                 //preset point plan enable 
    unsigned char     m_reserved[1];                             //reserved for alignment
    unsigned char     m_start_month;                             //start month   
    unsigned char     m_start_day;                               //start day
    unsigned char     m_start_hour;                              //start hour
    unsigned char     m_start_min;                               //start minute
    unsigned char     m_stop_month;                              //end month   
    unsigned char     m_stop_day;                                //end day
    unsigned char     m_stop_hour;                               //end hour
    unsigned char     m_stop_min;                                //end minute
    unsigned char     m_point_intime;                            //preset point number to be called in time
    unsigned char     m_point_outtime;                           //preset point number to be called out of time
}VSNETAUTOPOINT_ITEM;

typedef struct
{
    unsigned char           m_bEnable;                           //preset point plan enable
    unsigned char           m_interval;                          //interval time reset automatically，minute as UNIT
    unsigned char           m_reserved[2];                       //reserved for alignment
    VSNETAUTOPOINT_ITEM     m_scheme[4];                         //4 scheme of call preset point
}VSNETAUTOGOTOPOINT;

typedef struct
{
    unsigned char           m_enable;                               //temperature and humidity sensor enable
    unsigned char           m_addrtem;                              //address of temperature and humidity server
	unsigned char           m_osd_temperature;						//OSD temperature enable
	unsigned char			m_osd_humidity;							//OSD humidity enable
    int				        m_alarmtime;                            //interval time for alarm
    TIMELIST		        m_timelist;                             //time for defense
	unsigned char           m_temperature_alarmout[16];             //output of temperature alarm relating,previous 4 outputs are alarm outputs ,then 11 GPIO
    unsigned char           m_humidity_alarmout[16];                //output of humidity alarm relating,previous 4 outputs are alarm outputs ,then 11 GPIO
	float				    m_max_temperature;                      //max temperature   
    float				    m_min_temperature;                      //min temperature
    float					m_max_humidity;                         //max humidity
    float					m_min_humidity;                         //min humidity
}VSNETTEMHUMYWCNCEN;

typedef struct
{
    unsigned char    m_enable;                    //voltage sensor enable
	unsigned char    m_addrtem;                   //voltage sensor address
	unsigned char    m_osd_voltage;               //OSD voltage enable
	unsigned char    m_reserved;                  //reserved for alignment
	int              m_alarmtime;                 //interval time for alarm
	TIMELIST         m_timelist;                  //time for defence
	unsigned char    m_voltage_alarmout[16];      //output of humidity alarm relating,previous 4 outputs are alarm outputs ,then 11 GPIO
	float            m_rating;                    //voltage rating ,V as UNIT
	float            m_alarm_voltage;             //alarm voltage，V as UNIT 
}VSNETVYWCNCEN;

typedef struct
{
    unsigned char           m_bEnable[3];                           //enable relate to dangerous temperature and humidity 
    unsigned char           m_reserved;                             //reserved
	unsigned char			m_bEnable_maxdan_tem[4];					//max dangerous temperature enable
    float					m_maxdan_temperature[3];                //max dangerous temperature 
	unsigned char			m_bEnable_mindan_tem[4];					//min dangerous temperature enable
    float					m_mindan_temperature[3];                //min dangerous temperature
	unsigned char			m_bEnable_maxdan_hum[4];				//max dangerous humidity enable
    float					m_maxdan_humidity[3];                   //max dangerous humidity
	unsigned char			m_bEnable_mindan_hum[4];					//min dangerous humidity enable
    float					m_mindan_humidity[3];                   //min dangerous humidity
	unsigned int			m_checktimersecond;						//interval time for checking
}VSNETDANTEMHUMYWCNCEN;

typedef struct
{
	int						m_alarmmode;		//alarm mode0：OFF,1：ON
	int						m_singlehost;		//label of center for uploading alarm signal
	unsigned char			m_record[16];		//relating to recording
	unsigned char			m_out[15];			//relating to output
	unsigned char			m_osd_enable;		//OSD Enable
	unsigned char			m_enpreno[16];		//relating to preset point
	unsigned char			m_preno[16];		//perset point number to be called
	unsigned char			m_capjpeg[16];		//relating to capture
	TIMELIST				m_timelist;			//check time table
}ALARMRECORDEXYWCNCEN;

typedef struct
{
	int						m_benab;            //alarm enable
	ALARMRECORDEXYWCNCEN	m_alarmrec[8];      //parameter of relating alarm
}ALARMPARAMEXYWCNCEN;

typedef struct 
{
	float					m_rate_voltage;		//rating voltage
	float					m_cur_voltage;		//current voltage
}VSNETVALARMYWCNCEN;

typedef struct 
{
	short					m_temhumdata;		//temperature and humidity
	short					m_alarmtype;		//alarm type 0:no alarm,1:higher than upper limit,2:lower than upper limit
}VSNETTEMHUMALARMYWCNCEN;

typedef struct
{
	int     m_sgline_flg;			            //single line alarm enable
    char    m_sgline_name[64];                  //alarm rule name for single line 
	int     m_sgline_drc;			            //single line direction	0:Left1:Right 2:bidirectional
	int     m_sgline_beg_x;		                //alarm coordinate of single line , top left
	int     m_sgline_beg_y;		                //alarm coordinate of single line , top left
	int     m_sgline_end_x;		                //alarm coordinate of single line , down right
	int     m_sgline_end_y;		                //alarm coordinate of single line , down right
}VSNETVASINGLELINERULE;// caution line

typedef struct
{
	int                    m_multiline_flg;	    //broken line alarm
    char                   m_multiline_name[64];//rule name of broken line alarm
    int                    m_lineCount;         //single line count of broken line
    VSNETVASINGLELINERULE  m_multiline[10];     //rule of broken line 
}VSNETVAMULTILINERULE;

typedef struct
{
	int     m_dbline_flg;			                //double line alarm enable
    char    m_dbline_name[64];                      //rule name of double line alarm
	int     m_dbline_drc;			                //double line direction	0:left 1:right
	int     m_dbline0_beg_x;		                //alarm coordinate of line one , top left
	int     m_dbline0_beg_y;		                //alarm coordinate of line one , top left
	int     m_dbline0_end_x;		                //alarm coordinate of line one , down right
	int     m_dbline0_end_y;		                //alarm coordinate of line one , down right
	int     m_dbline1_beg_x;		                //alarm coordinate of line two , top left
	int     m_dbline1_beg_y;		                //alarm coordinate of line two , top left
	int     m_dbline1_end_x;		                //alarm coordinate of line two , down right
	int     m_dbline1_end_y;		                //alarm coordinate of line two , down right
}VSNETVADOUBLELINERULE;

typedef struct
{
	int                     m_region_valid;			    //valid label of region alarm  
    int                     m_region_flg;               //region alarm enable
    char                    m_region_name[64];          //rule name of region alarm
	unsigned int            m_packed_msk[352*288/8/4];	//region mask---stored as bit (0-->>0) (1-->>255)
}VSNETVAREGIONERULE;//region alarm(enter,leave,hover) public segment

typedef struct
{
    int                     m_frm_bkgd;		            //frame number when background initialization finished
	int                     m_frm_pixels;		        //average value of background and covariance period for updating---per m_frm_pixels frame updating once
	int                     m_frm_object;		        //time for static object becomes background ---update moving point for a long time  to background
}VSNETVABACKGROUND;

typedef struct
{
	int                     m_obj_min_area;	            //min area of moving object
	int                     m_obj_max_area;	            //max area of moving object
}VSNETVAOBJECTAREA;

typedef struct
{
	int                     m_alm_window_enable;		        //object rectangle enable
	int                     m_alm_dotline_enable;		        //point of object track enable
	int                     m_stabilization_enable;	            //image stabilization enable
	int                     m_region_shield_enable;	            //region shield enable
	unsigned int            m_region_shield_msk[352*288/8/4];	//mask code of region shield after packing---store with bit (0-->>0) (1-->>255)
}VSNETVAALARMSETTING;

typedef struct
{
    int                     m_enable;           //0:disable; 1:enable local preview
    int                     m_scale;            //0:1 picture; 1:4 pictures; 2:9 pictures; 3:16 pictures
    unsigned char           m_vichn[16];        //select input channels for 16 output channels
}VSNETVIEWPARAM;

typedef struct
{
    VSNETVIEWPARAM          m_viewparam[VSNET_DVR_MAXVONUM];
}VSNETVIEWCFG;

typedef struct
{
    int                     m_time;             //round look time
    int                     m_scale;            //0:1 picture; 1:4 pictures; 2:9 pictures; 3:16 pictures
    unsigned char           m_vichn[16];        //select input channels for 16 output channels
}VSNETLOOPPARAM;

typedef struct
{
    int                     m_enable;           //0:disable; 1:enable round look
    int                     m_loopnum;          //group number of round look
    VSNETLOOPPARAM          m_loopparam[20];    //parameter of round look
}VSNETLOOPVIEW;

typedef struct
{
    VSNETLOOPVIEW           m_loopview[VSNET_DVR_MAXVONUM];
}VSNETLOOPCFG;

typedef struct
{
    int                     m_enabled;          //disk support smartctl or not,1:enable，0:disable
    int                     m_health;           //disk check,1:passed, 0:failed.
    int                     m_temperature;      //disk temperature
    int                     m_poweronhours;     //disk power-on time
    int                     m_uncorrectalbe;    //percentage of error sector scanned off line
}VSNETSMARTPARAM;

typedef struct
{
    int                     m_disknum;          //total of disk
    VSNETSMARTPARAM         m_smartinfo[8];     //smart information of 8 disks
}VSNETSMARTINFO;

typedef struct
{
    int                     m_enable;              //nfs 1：enable， 0：disable
    char                    m_deviceip[40];       //device IP address
    char                    m_devicedir[40];       //device path
}VSNETMNTNFS;

typedef struct
{
    int                 m_enable;                               //enable
	int                 m_singlehost;                           //label of center for uploading alarm signal
	int                 m_singlebuzzer;                         //buzzer trigger
	int                 m_magnify[VSNET_DVR_MAXVONUM];          //relating amplification
	int                 m_reseved1;                             //reserved
	unsigned char		m_record[VSNET_DVR_MAXCH];              //relate to recording
	unsigned char		m_capjpeg[VSNET_DVR_MAXCH];             //relate to capture
    unsigned char       m_ftprecord[VSNET_DVR_MAXCH];           //ftp upload recording
    unsigned char       m_ftpcapjpeg[VSNET_DVR_MAXCH];          //ftp upload alarm capturing 
	unsigned char		m_out[VSNET_DVR_MAXALARMOUT];           //relate to output
	unsigned char		m_enable_ptz[VSNET_DVR_MAXCH];          //enable relate to cloud platform
	unsigned char       m_enable_type[VSNET_DVR_MAXCH];         //ability type，1:call preset point，2:call track, 3:ONLY preset point,4:ONLY track
	unsigned char		m_preno[VSNET_DVR_MAXCH];               //call preset point
	unsigned char       m_pretrackno[VSNET_DVR_MAXCH];          //relating track number
	TIMELIST            m_timelist;                             //check time table
}VSNETDVRALARMPARAM;

typedef struct
{
    VSNETDVRALARMPARAM  m_alarmparam;
    unsigned char       m_alarmsenstive;    //sensitivity of moving detection
    unsigned char       m_reseved1;         //reserved
    unsigned char       m_reseved2;
    unsigned char       m_reseved3;
    unsigned char       m_detect[18][22];   //range to be detected
}VSNETDVRMOTIONALARM;                       //parameter of moving detection

typedef struct
{
    VSNETDVRALARMPARAM  m_alarmparam;
    int                 m_alarmmode;        //alarm mode0：OFF,1：ON
	int                 m_senseorinterval;  //interval time of sensor alarm
	char				m_alarmname[32];	//sensor name
}VSNETDVRSENSORALARM;                       //parameter of sensor alarm

typedef struct
{
    VSNETDVRALARMPARAM  m_alarmparam;
    int                 m_decval;           //detection sensitivity(1(lowest)---5(highest))
}VSNETDVRHIDEALARM;                         //hide alarm

typedef struct
{
    VSNETDVRALARMPARAM  m_alarmparam;
}VSNETDVRVILOSTALARM;                       //video lost 

typedef struct
{
    VSNETDVRALARMPARAM  m_diskfull;         //disk full
    VSNETDVRALARMPARAM  m_diskerror;        //disk error
    VSNETDVRALARMPARAM  m_accessviolation;  //violative access
    VSNETDVRALARMPARAM  m_nodisk;           //no disk
    VSNETDVRALARMPARAM  m_reseved1;         //reserved
    VSNETDVRALARMPARAM  m_reseved2;         //reserved
    VSNETDVRALARMPARAM  m_reseved3;         //reserved
    VSNETDVRALARMPARAM  m_reseved4;         //reserved
}VSNETDVROTHERALARM;                        //other parameter

typedef struct
{
    char                m_deviceID[16];     //device ID
    char                m_active;           //status, 1 : active
    char                m_reserved[3];
}VSNETREMOTECTRL;

typedef struct
{
    unsigned int      m_right;                //each bit represents a kind of right,from low to high,it is
                                              //local playback,long-distance playback,long-distance preview,talk,local cloud platform control,
                                              //long-distance cloud platform control、recording by hand、recording file backup、log management、
                                              //system parameter setting,upgrading system,close device,management of user account number 
    unsigned int      m_right1;               //reserved
    unsigned int      m_netview;              //0~31 channel of long-distance preview
    unsigned int      m_netview1;             //32~63 channel of long-distance preview
    unsigned int      m_netplay;              //0~31 channel of long-distance preview
    unsigned int      m_netplay1;             //32~63 channel of local playback
    unsigned int      m_localplay;            //0~31 channel of local playback
    unsigned int      m_localplay1;           //32~63 channel of local playback
}VSNETDVRUSERRIGHT;

typedef struct
{
    VSNETDVRUSERRIGHT	 m_right;
    char      	 	     m_groupname[20];
}VSNETDVRUSERGROUP;

typedef struct
{
    char            	 m_username[20];
    char            	 m_password[20];
    int			    	 m_groupnum;                         //right group of user
    VSNETDVRUSERRIGHT    m_right;
}VSNETDVRUSER;

typedef struct
{
    int             	m_usernum;                          //user number
    int             	m_groupnum;                         // number of valid groups
    VSNETDVRUSERGROUP   m_group[8];                         //8 groups，initialize 4 groups ,reserve 4 groups
    VSNETDVRUSER        m_user[32];                         //32 users，initialize 4 users
}VSNETDVRSYSUSER;

typedef struct
{
	char			m_status[32];               /* the status of user(0:disbale, 1:enable) */
	VSNETDVRSYSUSER m_dvrsysuser;
}VSNETDVRSYSUSEREX;                            /* extent user info(add the stauts of user) */

typedef struct
{
    int					bShow;				//show(1),hide(0)
	char				lpString[32];		//characters to be showed		
	unsigned short		x,y;				//coordinate
}TITALOSD_DVR;

typedef struct
{
	TIMEOSD				m_time;				//time
	TITALOSD_DVR		m_tital[4];			//title
	BITSINFOOSD			m_bits;				//bit rate
}CHANNOSD_DVR;

typedef struct
{
    int                 m_doublebits;       //1:double bit stream，0:single bit stream
}VSNETDVRDOUBLEBITS;

typedef struct
{
    char                m_name[9][32];      // name of switching button
}VSNETSWITCHSCREENNAME;

typedef struct
{
    char				m_servername[60];	//server name
	char				m_serverip[16];		//IP address
	char				m_servermask[16];	//network mask
	char				m_gatewayAddr[16];  //gateway
	unsigned short		m_serport;			//data port
    char				m_hostserverurl[40];//address of long-distance server
	unsigned short		m_hostserverport;   //port of long-distance server
    char				m_pppoename[64];	//PPPOE username
	char				m_pppoepswd[64];	//PPPOE password
}VSNETBASENETPARAM;

typedef struct
{
    int             m_ftp_timerec;       //regular FTP uploading capture
    TIMELIST    	m_timeList;		     //capture time
}VSNETFTPTIMEREC;

typedef struct
{
	int             m_xoffset;              // horizontal offset 0~32
	int             m_yoffset;              //vertical offset 0~32
}VSNETVIDEOOFFSETEX;

typedef struct 
{
    int             m_state;                    //3G state
    char            m_ip[32];                   //CenterIP
    char            m_subnetmask[32];           //subnet mask for WCDMA dialing
    char            m_gateway[32];              //gateway for WCDMA dialing
    char            m_wcdma_dns_pri[32];        //primary DNS for WCDMA dialing
    char            m_wcdma_dns_alt[32];        //Secondary DNS for WCDMA dialing
}VSNET3GSTATE;

typedef struct
{
    int              m_3gmode;           //connection parameter      -------->0-OFF 1-CDMA 2-evdo(3g cdma2000) 4-wcdma 8-tdcdma
    unsigned char    m_cdmaname[64];     //reserve cdma username(dv-do username)
    unsigned char    m_cdmapwd[64];      //reserve cdma password(dv-do password)
    char             m_cdmadial[16];     //reserve cdma dialing number(dv-do dialing number)
}VSNET3GPARAM;

typedef struct
{
    int              m_3gmode;                   //connection parameter      -------->0-OFF 1-CDMA 2-evdo(3g cdma2000) 4-wcdma 8-tdcdma
    unsigned char    m_cdmaname[64];             //reserve cdma username(dv-do username)
    unsigned char    m_cdmapwd[64];              //reserve cdma password(dv-do password)
    char             m_cdmadial[16];             //reserve cdma dialing number(dv-do dialing number)
    unsigned char    m_apnname[64];              //point name
    int              m_keepalive;                //keep connection alive
    int              m_lcpechointerval;
    int              m_lcpechofailure;
    int              m_maxreceiveunit;
    int              m_maxtransferunit;
    int              m_networkconnectionmode;    //online,dial for need,dial by hand
    int              m_networkregistertype;      //AUTO,2G,3G
    int              m_radiobankset;             //band setting,represented by bit
    int              m_bandsaving;               //reserved band
    int              m_getdnsfromoperator;
    char             m_dns_pri[32];              //Primary DNS
    char             m_dns_alt[32];              //Secondary DNS
    int              m_authtype;
    unsigned char    m_expertoptions[128];       //extension parameter
}VSNET3GPARAMEX;

typedef struct
{
    int              m_pinprotection;            //pin protection
    unsigned char    m_pincode[64];	             //pin code
}VSNETPININFO;

typedef struct
{
    int              m_simstatus;                //sim status
    unsigned char    m_simnumber[64];	         //sim number
    char             m_operator[20];             //provider
    int              m_cur_cdma_network;         //current network
    int              m_sig_percent;              //signal strength
    char             m_imei[20];                 //IMEI
}VSNETSIMINFO;

typedef struct
{
    int              m_smsdays;                  //message days
    int              m_sms_sendip;               //message sends IP address or not
    unsigned char    m_smscellphone[64];	     //number for receiving message
    unsigned char    m_smscenternumber[64];      //number of message center
    unsigned char    m_smscontent[128];	         //content of message
}VSNETSMSINFO;

typedef struct
{
    int              m_logsize;                  //log size
    unsigned char *  m_logstring;                //log content
    unsigned char    m_logfilename[64];          //log name
}VSNETWCDMALOG;

typedef struct
{
    int              m_check3gstatus;            //check 3g status
    unsigned char    m_pingipaddr[64];	         //ping IP Address
    int              m_pingipinterval;	         //interval  of ping ip 
    int              m_pingbytes;                //data size for ping
    int              m_pingfailcnt;              //redial number when ping failure
}VSNETCHECK3GSTATUS;

typedef struct
{
    int     m_linkenable;       //link enable
    int     m_listenport;     	//monitor port
}VSNETRTSPPARAM;

typedef struct
{
    int	    m_linkenable;       //link enable
    char    m_dstaddr[16];     	//destination address
    int     m_dstport;     		//destination port
    int     m_streamselect;     //PS or TS
}VSNETVLCPARAM;

typedef struct
{
    int     m_linkenable;       //link enable
    char    m_serveraddr[16];   //server address
    int     m_serverport;     	//server port
    int     m_timezone;         //timezone(-12 ~ 12)
}VSNETNTPPARAM;

typedef struct
{
    VSNETRTSPPARAM m_rtsp;		//RTSP parameter
    VSNETVLCPARAM  m_vlc;		//VLC  parameter
    VSNETNTPPARAM  m_ntp;		//NTP  parameter
}VSNETPROTOCOLPARAM;

typedef struct
{
    int             m_encodetype;           //encoding type, 0:H.264, 1:MJPEG
}VSNETENCODETYPE;

typedef struct
{
	short		m_audiotag;		//audio tag
	short		m_auchs;		//audio channel number
	short		m_aurates;		//audio bit rate
    short		m_reserved;		//reserved
	int			m_ausample;		//audio sampling frequency
}VSNETTALKBOTHWAY;

typedef struct
{
    int         m_locktime;
}VSNETPTZCTRLTIME;

#define MAX_MEM_NUM 10
#define MAX_PRO_NUM 10
typedef struct
{
    int                      m_prono;             //preset point
    int                      m_time;              //time
    char                       m_proname[32];       //name of preset point
}APP_CRUISE_PRO;
typedef struct
{
    int                     m_enable;            //enable
    TIMECHECK                   m_cruise_time;       //time
    char                      m_cruise_name[32];   //cruise name
    int                      m_line_enable;       //enable superposition of cruise route 
    int                      m_dot_enable;        //enable superposition of cruise point 
    int                      m_superpose_title;   //superposition to title, value as:0，1，2，3 represent superposition to 1,2,3,4 title
    APP_CRUISE_PRO              m_cruise_pro[MAX_PRO_NUM];    
}APP_CRUISE_MEMBER;
typedef struct 
{
    int            m_enable;
    APP_CRUISE_MEMBER m_cruise_member[MAX_MEM_NUM];
}VSNETPROLOOPPARAM;

typedef struct
{
    char  reccordName[60];      //record name
}VSNETSERVERRECPARAM;           //record command parameter of long-distance server

typedef struct
{
    int m_audio_bestir;
}NETFILE_REMOTEAUDIOBESTIR;

typedef struct
{
    int videoin;
}VSNETVIDEOENCMAPINFO;

typedef struct 
{
	char	m_value[8];			//show evidence		
}VSNETATTESTEXHIBIT;

typedef struct
{
	char m_actors[7];				//character array，from 0 to 5 represents 6  roles's channels，0 justice...，6 is end symbol
}VSNETACTORSINFO;

typedef struct 
{
	int					m_enable;				//enable
	unsigned int		m_brecord[2];			//relate to recording
	unsigned int		m_capjpeg[2];			//relate to jpeg capture
	unsigned int		m_ftp_alarmrec[2];		//uploading recording of alarm relating to FTP  
    unsigned int		m_ftp_alarmcapjpeg[2];	//uploading alarm capture to FTP
	unsigned int		m_outputmap[4];			//relate to output
	TIMELIST			m_timelist;				//detection time table
}VSNETSMALARMLINK;								//alarm relating of intellective analysis

typedef struct
{
	int					m_enable;   //enable
	unsigned short		m_x,m_y;	//coordinate
	char				m_text[32];	//content to be showed of OSD superposition 
}VSNETSMOSD;						//intellective analysis OSD

typedef struct
{
	int					m_enable;					//enable rule
	int					m_enableosd;				//OSD enable
	int					m_enabledefregion;			//enable default region
	char				m_rulename[64];				//rule name
	unsigned int        m_packed_msk[352*288/8/4];	//region mask---stored with bit (0-->>0) (1-->>255)
	VSNETSMALARMLINK	m_alarmlink;				//alarm relating
	VSNETSMOSD			m_osd[2];					//OSD superposition of intellective analysis alarm
}VSNETSMREGIONINOUT;								//alarm rule of entering and leaving intellective analysis region 

typedef struct
{
	int					m_enable;					//enable rule 
	int					m_enableosd;				//OSD enable
	int					m_enabledefregion;			//enable default region
	int					m_hovertime;				//standard time of hovering
	char				m_rulename[64];				//rule name
	unsigned int        m_packed_msk[352*288/8/4];	//region mask---stored with bit (0-->>0) (1-->>255)
	VSNETSMALARMLINK	m_alarmlink;				//alarm relating
	VSNETSMOSD			m_osd[2];					//OSD superposition of intellective analysis alarm
}VSNETSMREGIONHOVER;								//alarm rule of hovering intellective analysis region

typedef struct
{
	int					m_enable;            //rule enable
	int					m_sgline_drc;		 //single line direction	0:left 1:right 2:bidirectional
	int					m_sgline_beg_x;		 //coordinate of single line alarm,top left
	int					m_sgline_beg_y;		 //coordinate of single line alarm,top left
	int					m_sgline_end_x;		 //coordinate of single line alarm,down right
	int					m_sgline_end_y;		 //coordinate of single line alarm,down right
	char				m_rulename[64];		 //rule name
	unsigned char		m_enableosd;		 //OSD enable
	VSNETSMALARMLINK	m_alarmlink;         //alarm relating
	VSNETSMOSD		    m_osd[2];			 //OSD superposition of intellective analysis alarm
}VSNETSMALARMLINE;							 //alarm rule of caution line crossover

typedef struct
{
	int					m_count;			//line count
	VSNETSMALARMLINE	m_lines[10];		//mas line count is 10
}VSNETSMALARMLINES;							//alarm rule set of caution line crossover

typedef struct
{
    unsigned int        m_alarmtime;        //alarm time
    int                 m_alarmtype;        //alarm type
    char                m_alarmdesc[56];    //alarm description
}VSNETALARMJPEGCAP;

typedef struct
{
    char m_num[8];				//pit number
	int m_bmotionjpg;	        //uploading moving capture to FTP
	int m_waittime;	            //wait time(second) for protecting from building bridge
	int m_front_sd;	            //front value
}VSNETJNKYPARAM;

typedef struct
{
    char            m_postaddr[32]; //post address
    unsigned short  m_postport;     //post port
    unsigned short 	m_reserved;     //reserved for alignment
}VSNETJNKYSERINFO;

typedef struct 
{
    int					m_cruise_day;				//cruise date
    TIMELIST			m_cruise_time;				//cruise time
    int					m_line_enable;				//enable superposition of cruise route 
    char				m_osdstring[32];			//display character of OSD superposition 
    int					m_superpose_title;			//superposition to title, value as:0，1，2，3 represent superposition to 1,2,3,4 title
    int					m_staytime;					//stay time of cruise point，3-255 seconds
    int					m_cruise_pre[MAX_PRO_NUM];	//preset point number
}VSNETPROLOOPPARAMEX;								//server cruise parameter,no relationship with previous version VSNETPROLOOPPARAM

#define   HNDX_STROSD_NUM	8
#define   HNDX_STROSD_LEN	40
typedef struct
{
    int      m_format;  //not show，2003-09-12 15:23:33，2003-09-12 FRI 15:23:33，09-12-2003 15:23:33，09-12-2003 FRI 15:23:33，12-09-2003 15:23:33
    int      m_x;       //x position，8-704 (multiple of 4)
    int      m_y;       //y position，10-576
}VSNETHNDXTIMEOSD;		//time OSD

typedef struct
{
    int      m_isshow;      //show bit rate or not
    int      m_x;           //x position，8-704 (multiple of 4)
    int      m_y;           //y position，10-576
}VSNETHNDXBITRATEOSD;		//bit rate OSD

typedef struct
{
    int      m_x;						//x position，8-704 (multiple of 4)
    int      m_y;						//y position，10-576
    int      m_color;					//color 0:red 1:green 2:blue 3:white
    char     m_str[HNDX_STROSD_LEN];	//String  
}VSNETHNDXOSDSTRING;					//String OSD

typedef struct
{
    VSNETHNDXBITRATEOSD m_bitrateosd;
    VSNETHNDXTIMEOSD    m_timeosd;
    VSNETHNDXOSDSTRING  m_stringosd[HNDX_STROSD_NUM];  
}VSNETHNDXOSDATTR;				//OSD parameter

typedef struct
{
    int       m_enableaac;   //AAC enable
    int       m_enSmpRate;   //AAC sampling frequency
    int       m_BitWidth;    //AAC bit width
    int       m_BandWidth;   //AAC band width
}VSNETAUDIOAAC;				 //AAC parameter

typedef struct
{
    int       m_color_set;   //0: automatic, 1: by hand
}VSNETHQCOLOR;				 //set mode

typedef struct
{
    int     m_smart_sense;        //intellective analysis of sensitivity
}VSNETSMARTSENSE;

typedef struct
{
	int		m_smart_tracktype;    //intellective analysis of object type：0 all，1 human 2 vehicle
}VSNETSMARTTRACKTYPE;

typedef struct
{
    char    m_serurl[16];	      //server IP address
    int     m_serport;		      //server port
    char    m_stationID[10];	  //station number
    char    m_reserve[2];	      //reserved for alignment
    char    m_clienturl[16];      //additional client ip
    int     m_clientport;         //client port
    char    m_direction[4];       //station direction
}VSNETCDRHPARAM;

typedef struct
{
    int   m_enable;                 //ON or OFF
    int   m_interval;               //check interval
    float m_flowlow;                //flow lower limit
    float m_flowhigh;               //flow upper limit
    float m_phlow;                  //ph lower limit
    float m_phhigh;                 //ph upper limit
    float m_Densitylow[12];         // lower limit of 12 kinds of pollution
    float m_Densityhigh[12];        // upper limit of 12 kinds of pollution
    char  m_Alarmflow[4];		    //alarm ,flow relating to 
    char  m_Alarmph[4];		        //alarm ,PH relating to 
    char  m_AlarmDensity[12][4];	//alarm ,pollution relating to 
}VSNETYWCODPARAM;

typedef struct
{
    char   DeviceState[8];          //8 devices
    float  m_flow;                  //flow speed
    float  m_ph;                    //PH
    float  m_Density[12];           //lower limit of 12 kinds of pollution
}VSNETYWCODDATA;

typedef struct
{
    char m_cmd[52];                 //command of sampling machine for water quality
}VSNETYWSZCYQCODE;

typedef struct
{
    char m_code[12];               //command of electromagnetism flowmeter
}VSNETYWFLOWMETERCODE;

typedef struct
{
    int     m_region_flg;               //region flag of analysis for black degree,judge region or not
    int     m_region_left;              //region coordinate of analysis for black degree,top left
    int     m_region_top;               //region coordinate of analysis for black degree,top left
    int     m_region_right;             //region coordinate of analysis for black degree,down right
    int     m_region_bottom;            //region coordinate of analysis for black degree,down right
}VSNET_BLACK_REGION;

typedef struct
{
    VSNET_BLACK_REGION  m_region[10];   //region of analysis for black degree
}VSNET_BLACK_REGIONTARGET;

typedef struct
{
    VSNET_BLACK_REGION  m_region;       //reference region for  analysis for black degree
}VSNET_BLACK_REGIONREFER;

typedef struct 
{
    float m_min_val;                    //lowest value of black degree
    float m_max_val;                    //highest value of black degree
}VSNET_BLACK_ALARMRANGE;

typedef struct
{
    VSNET_BLACK_REGION  m_region;       //region of analysis for black degree
    float               m_value;        //value of black degree
}VSNET_BLACK_VALUE;

typedef struct
{
    VSNET_BLACK_VALUE   m_report[10];   //report value of analysis for black degree
}VSNET_BLACK_REPORT;

typedef struct 
{
	int		m_state;			// state: open,close
	int		m_limittime;		// system limit times(hour)
	int		m_runedtime;		// device runned times(hour，only get valid)
}VSNETYWDEVRUNTIME;

typedef struct
{
	unsigned char				 m_islock;					    //is lock the disk
	unsigned char				 m_reserved[3];					//reservation
}VSNETBUFFLOCK;

typedef struct
{
	unsigned short		m_serport;
	unsigned short		m_webport;
	unsigned short		m_rtspport;
	unsigned short		m_p2pport;
}VSNETCDHCPORT;

typedef struct
{
    char			m_szCaseName[28];                   //case name
	char			m_szCaseID[28];	                    //case id
	char			m_szCaseExecor[28];                 //case execor
	char			m_reverse1[28];						//court name
	char			m_reverse2[28];
	char			m_reverse3[28];
}VSNETDXJBCASEINFO;										//DXHJ7304 server record case info

typedef struct
{
	int				m_iVGANO;
	char			m_reverse[16];
}VSNETDXJBVGA;											//DXHJ7304 current max window chnannel no

typedef struct
{
	unsigned int    m_sigpercent;		//signal power
	unsigned int	m_rx_byte;			//accepted flow
	unsigned int	m_tx_byte;			//sent flow
}VSNETSIGAND3GFLOW;

typedef struct
{
	float			m_gpsvx;
	float			m_gpsvy;
	int             m_gpsvsout;
	float			m_gpsvspeed;
	float			m_gpsvheight;
	char			m_gpsvtime[32];
}VSNETHZFLGPSINFO;

typedef struct // GZJieWei Parameter = VSNETGZJWUIDHEADER + Data
{
	int   m_Mask1;		 //MSGHEAD_MASK1: 0X280028
	int   m_Vision;      //message version: current = 1
	int   m_MgsType;     //message type: m_MgsType = 8079
	int   m_ErrorCode;   //message status code
	int   m_DataSize;    //message data size: max size = 32 bytes
    short m_Ex1;         //extension 1
    short m_Ex2;         //extension 2
	int   m_Mask2;		 //MSGHEAD_MASK2: 0X280028
}VSNETGZJWUIDHEADER;

typedef struct  // GZJieWei Parameter
{
	VSNETGZJWUIDHEADER m_MsgHead;
	char			   m_MsgData[32];
}VSNETGZJWUIDPARAM;

typedef struct // beijing huatuo 
{
	int		m_bEnable;	// enable watcher, 1 enable, 0 disable
	int		m_iTime;	// check time > 0
	int		m_iPreset;	// preset number
}VSNETBJHTPTZWATCHER;

//for DVR(LC78XX) ---add by buwp,2010-6-11
/**********************************  DVR GUI ADD *****************************************/
typedef struct
{
    unsigned char  m_no;                     /* the number of preseting position */
    char           m_name[31];               /* the name   of preseting position */
}VSNET_PTZ_PRESET_NO_T;

typedef struct
{
    int                    m_num;            /* Now, how many preseting position we have */
    VSNET_PTZ_PRESET_NO_T  m_presetno[255];       
}VSNET_PTZ_PRESET_T;                       /* Save PTZ preseting position */

typedef struct
{
    int                 m_usernum;           /* how many user we have,now  */            
    int                 m_groupnum;          /* how many valid user group we have ,now */     
    VSNETDVRUSERGROUP   m_group[8];          /* 8 groups, initialize 4 groups  4 are reserved */   
    VSNETDVRUSER        m_user[32];          /* 32 users, initialize 4 users  */ 
}VSNET_SYSUSER_T;

typedef struct
{
    char m_status[32];                       /* the status of user(0:disbale, 1:enable) */
    VSNET_SYSUSER_T m_user;
}VSNETDVRSYSUSER_EX;                       /* extent user info(add the stauts of user) */
  
typedef enum
{
    HDISK_NOCHANGE = 0,
    HDISK_CHANGE,
    HDISK_ADD,
    HDISK_ABATE
}HDISK_CHANGE_STATUS;

typedef struct
{
	int			        m_diskno;
	int			        m_devname;
	int			        m_mntname;
	int			        m_size;
}VSNET_HDISK_DIFF;

typedef struct
{
	int 			    m_change_flag;		 /* the flag of any disk chanage,using with VSNET_HDISK_DIFF  */ 
    VSNET_HDISK_DIFF	m_disk[9];
}VSNET_HDISK_INIT_CHANGE;

typedef struct 
{
    int                 m_state;    // 0:未启用 1:成功 2:失败
    char                m_ip[16];
    char                m_submast[16];
    char                m_gateway[16];    
}VSNET_IP_INFO;

typedef struct
{
    long int            m_log_time;          /* log time reserved temporary for convenient (size=4) */ 
    VSNETPLAY_NETTIME   m_log_ctltime;       /* log will use this struct in the futurn  */
    int                 m_log_ch;            /* the NO. of channel  or alarm senser or 0 */    
    int                 m_log_type;          /* the event type of alarm log  */    
}VSNET_GUI_ALARM_LOG;

typedef struct
{
    int                 m_num;               /*Now,how many alarm log we have */
    VSNET_GUI_ALARM_LOG m_alarm_log[256];    
}VSNET_GUI_ALARM_LOG_INFO;

typedef struct
{
    char                m_log_ctl;           /* Operator Type  0: as default value, 1:delete log, 2:export log  */ 
    char                m_log_typesearch;    /* Search log base on log type, sepcially 0:all log  */ 
    char                Received[2];		 /* Reserved */ 
    VSNETPLAY_NETTIME   m_starttime;	     /* the start time of look for log */  
	VSNETPLAY_NETTIME   m_stoptime;	         /* the stop  time of look for log */    
}VSNET_GUI_LOG_SEARCH;

typedef struct
{
    long int            m_log_time;          /* log time reserved temporary for convenient (size=4) */ 
    VSNETPLAY_NETTIME   m_log_ctltime;       /* log will use this struct in the futurn  */
    char                m_log_admname[20];   /* the operator name :all will be set 0 if No operator when event accur */ 
    int                 m_log_ch;            /* the NO. of channel  or alarm senser or 0 */
    int                 m_log_majtype;       /* the major type of event */    
    int                 m_log_mintype;       /* the minor type of event */
}VSNET_LOGINFO_T;

typedef struct 
{
    int                 m_state;    		/* 0:unable 1:success 2:failed */
    char                m_ip[16];
    char                m_submast[16];
    char                m_gateway[16];    
}VSNET_IP_INFOEX;

typedef struct
{
    int                 m_log_num;           /* how many log we have,now */  
    int                 m_log_cur_locat;     /* how many log we will send */    
    VSNET_LOGINFO_T     m_log_info[10000];   /* log information */     
}VSNET_APP_LOGINFO_GUI; 

typedef struct
{
    int                 m_zoom;              /* amplification factor */
    int                 m_X;                 /* centre coordinate X */
    int                 m_Y;                 /* centre coordinate Y */
}VSNET_PTZCTRL_ZOOM;

typedef struct
{
    int                 playmode;            /* 1:play in DVS way, the struct for replay is ETI_FILE_HEAD */ 
                                             /* 2:play in DVR way, the struct for replay is ETI_DVR_FILE_HEAD */ 
                                             /* Note:if device cannot support play in DVR way,
                                              * the struct of ETI_FILE_HEAD as the return is permissible */ 
    int                 downtype;            /* 1:download by file name 2:download by time */
    int                 recvtype;            /* record type */                                         
    int                 channel;             /* channel */
    char                sFileName[100];      /* file name */
    VSNETPLAY_NETTIME   StartTime;           /* the start time of file */
    VSNETPLAY_NETTIME   StopTime;            /* the end   time of file */
}VSNET_NETFILE_PLAYINFO;

typedef struct 
{
    int filenum;
    char m_filename[10][256];
}VSNET_SLAVE_PLAYFILE_T;

typedef struct
{
    int                     m_cmd;
    int                     m_pts;
    VSNET_NETFILE_PLAYINFO  m_playinfo;
    VSNET_SLAVE_PLAYFILE_T  m_playfile;
}VSNET_SLAVE_REPLAY_T;

typedef struct
{
    VSDISKSTATE m_diskstate;
    char m_num[8];                           /* the name of disk */
    char m_reseved[32];                   
}VSNET_GUI_DISK_NUM;

typedef struct
{
    char                    m_filename[100]; /* file's name by searched */
    int                     m_filesize;     /* file's size by searched */
    VSNETPLAY_NETTIME       m_filetime;     /* file's last modify time */
}VSNET_UPDATE_FILEPARMAM;

typedef struct
{
    int                     m_filenum;      /* file's nummbers by searched */
    char                    m_upname[100];   /* update file's name by choosed */
    VSNET_UPDATE_FILEPARMAM m_filename[50]; /* file's parameters by searched */
}VSNET_UPDATE_FILENAME;

typedef struct
{
    int				m_hdisk_attrib;         /* disk attribute 0:default(read/write); 1:redundance ; 2:readonly */
    char			m_hdisk_serialno[32];   /* disk serial id */
    int				m_resev[40];            /* reserved */
}VSNETDISKPARAM;

typedef struct
{
    int		        m_disk_count;           /* disk count */
    VSNETDISKPARAM  m_diskparam[16];        /* disk param */
    int				m_disk_reduflag[64];    /* disk redundance single, by channels */
}VSNETRECORDDISKPARAM;

typedef struct
{
    char                    m_log_ctl;          /* default is 0; 1 : delete, 2 : export. */
    char                    m_log_typesearch;   /* she is used when we search record file. value is 0 or 1 or 2 or 3, default is 0. */
    char                    Received[2];		/* reserved */
    VSNETPLAY_NETTIME       m_starttime;	    /* start time to search */
	VSNETPLAY_NETTIME       m_stoptime;	        /* end time to search */
    //VSNET_APP_LOGINFO_GUI   m_log_buff;       /* save log */
}VSNET_LOG_CTL;

typedef struct
{
    int filenum;
    char m_filename[10][256];
}VSNET_PLAYFILE_T;

typedef struct
{
	int m_channel;					/* channel, -1 : all channels. */
	int m_filetype;					/* file type, 1 : MPEG4，2 : JPEG */
	int m_recvtype;					/* record type */
	int m_timelimite;				/* 1 : enable */
	VSNETPLAY_NETTIME m_starttime;	/* start time*/
	VSNETPLAY_NETTIME m_stoptime;	/* end time*/
}VSNET_SEARCHPARAM_T;

typedef struct
{
    int     m_channel;              /* channel */
    int     m_filesize;             /* size */
    char    m_filename[64];         /* name */
	VSNETPLAY_NETTIME m_starttime;	/* start time */
	VSNETPLAY_NETTIME m_stoptime;	/* end time */
}VSNET_SEARCHFILEINFO_T;

typedef struct
{
    int m_filenum;
    VSNET_SEARCHFILEINFO_T m_fileinfo[1000];
}VSNET_SEARCHRESULT_T;

typedef struct
{
    int m_cmd;
    int m_pts;
    int m_disktype;                     /* 1:record disk, 2:redundancy disk, 4:read only, 5(1<<0+1<<2):record and read only. */    
    VSNET_NETFILE_PLAYINFO m_playinfo;
    VSNET_PLAYFILE_T m_playfile;
}VSNET_REPLAY_T;

typedef struct
{
    int m_bktype;                   /* 0 : by time, 1 : by file */
	int m_channel;					/* channel, -1 : all channel */
	int m_bkdev;                    /* device type, 0 : SATA in DVR, 1 : USB, 2 : ESATA */
	int m_filenum;                  /* number to backup when we need backup files by file. */
    char m_filename[20][256];       /* name to backup */
	VSNETPLAY_NETTIME m_starttime;	/* start time when we need backup files by time. */
	VSNETPLAY_NETTIME m_stoptime;	/* end time when we need backup files by time. */
}VSNET_FILEBACKUP_T;

typedef struct
{
    int     m_type;                 /* contorl type */
    int     m_value;                /* speed or address; CVT: H */
    int     m_paramfirst;           /* 3D:speed for H; CVT: V */
    int     m_paramsecond;          /* 3D:speed for V; CVT: speed for H */
    int     m_paramthird;           /* 3D:zoom parameter; CVT: speed for V */
}VSNETDVRPTZ;

typedef struct
{
    int     m_channel;              /* channel, -1:all channels, bit0=1:channel 1 ... */
    int     m_disktype;             /* 1:record disk, 2):redundancy disk, 4:read only, 5:record and read only. */
    int     m_filetype;             /* 1：MPEG4，2：JPEG */
    int     m_recvtype;             /* record type */
    int     m_timelimite;           /* 1: enable */
    VSNETPLAY_NETTIME m_starttime;  /* start time */
    VSNETPLAY_NETTIME m_stoptime;   /* end time */
    char    m_reserved[40];         /* reserved */
}GUI_SEARCHPARAM;

typedef struct
{
    int     m_channel;              /* channel */
    int     m_filesize;             /* size */
    char    m_filename[256];        /* name */
    int     m_filelock;             /* state */
    int     m_recordtype;           /* record type */
    VSNETPLAY_NETTIME m_starttime;  /* start time */
    VSNETPLAY_NETTIME m_stoptime;   /* end time */
}GUI_SEARCHFILEINFO;

typedef struct
{
    int m_filenum;
    GUI_SEARCHFILEINFO m_fileinfo[1000];
}GUI_SEARCHRESULT;

typedef struct
{
    int         m_cmd;              /* command */
}GUI_HDISK_EPLAY_CMD;

typedef struct
{
    char  m_ishdmi;                 //vga or hdmi, vga:0, hdmi:1
    char  m_hdmi_mode;              //hdmi mode
    char  m_vga_mode;               //vga mode
    char  m_reseved[1];             //reserved
}VSNET_VGA_RESOLUTION;

typedef struct
{
    char                    m_filename[256];    /* file name */
    int                     m_lock;             /* 1:lock, 0:unlock */
}VSNET_LOCK_FILEINFO;


typedef struct
{
    int              m_audio_type;               //audio type
    int              m_audio_bitrate;            //bit rate: 16K,24K,32K,40K for G726
    int              m_audio_samplerate;         //sample rate
    int              m_audio_bitwidth;           //bitwidth
}VSNETAUDIO;


typedef struct  
{
	unsigned char timer;                     //0、HIDE 1、MM:SS:mm(00:00:00)2、HH:MM:SS(00:00:00)
	unsigned int  positionx;                 //XPOS
        unsigned int  positiony;                 //YPOS
}CHANNELTIMEROSD;


typedef struct
{
    int m_horizontal_denoise;       /*horizontal denoise level*/
    int m_vertical_denoise;         /*vertical denoise level*/
}VSNET_DENOISE;

//SHTY
typedef struct
{
    int             m_audio_timerec;       //enable flag
    int             m_packtime;            //package time
    TIMELIST        m_timeList;            //capture time
}VSNETVIDEOTIMEREC;


//SHTY
typedef struct
{
    unsigned int  m_recodch[8][16];         
}VSNETSHTYALARMAUDIO;

//SHTY
typedef struct
{
unsigned int  m_capTime[8];         
}VSNETSHTYALARMCAPTIME;

typedef struct
{
    int m_denoise;                  /*tvp5158 denoise level*/
    int m_reseved;                  //reserved
}VSNET_TVP5158_DENOISE;

/**********************************  DVR GUI END *****************************************/

/********************************** 88258 STRUCT *****************************************/

#define MAX_CLIP_NUM 10
#define MAX_FAN_NUM  3                    //fan number

//CARD TYPE
typedef enum{
        CLIP_TYPE_MASTER    = 0,          //master card
		CLIP_TYPE_DEC       = 1,          //decoder card
		CLIP_TYPE_ENC_D1    = 2,          //D1
		CLIP_TYPE_ENC_720P  = 3,          //720P
		CLIP_TYPE_ENC_1080P = 4,          //1080
		CLIP_TYPE_ALARM     = 5,          //ALARM
		CLIP_TYPE_DEVICE    = 6,          //DEVICE
}APP_CLIP_TYPE;

typedef enum{
        CLIP_SLOT_SUB0      = 0,	      //CLIP1
		CLIP_SLOT_SUB1      = 1,          //CLIP2
		CLIP_SLOT_SUB2      = 2,          //CLIP3
		CLIP_SLOT_SUB3      = 3,          //CLIP4
		CLIP_SLOT_SUB4      = 4,          //MASTER
		CLIP_SLOT_SUB5      = 5,          //CLIP5
		CLIP_SLOT_SUB6      = 6,          //CLIP6
		CLIP_SLOT_SUB7      = 7,          //CLIP7
		CLIP_SLOT_MASTER    = 8,          //CLIP8
		CLIP_SLOT_MACHINE   = 9,          //ALARM
		CLIP_SLOT_ALARM     =10,          //reserved
}APP_CLIP_NO;

typedef struct
{
    int        m_temperature;                    //Temperature
    int        m_humidity;                       //humidity
} VSNETHUMITUPARAM,*PVSNETHUMITUPARAM;           //Temperature and humidity


typedef struct
{
    unsigned int     m_speed;                  //fan speed： r/s
    unsigned char     m_state;                 //state
    unsigned char     m_reseved[3];            //reserved
} VSNETFANSTATE,*PVSNETFANSTATE;               //fan state


typedef struct
{
    unsigned char     m_isexist;                //Be Exist
    unsigned char     m_type;                   //card type
    unsigned char     m_state;                  //state
    unsigned char     m_reseved[5];             //reserved
} VSNETCLIPSTATE,*PVSNETCLIPSTATE;              //clip card state


typedef struct
{
    VSNETCLIPSTATE    m_stotstate;                //slot state:master、clip、slot、alarm
    VSNETHUMITUPARAM  m_humitu;                   //Temperature and humidity of card
}VSNETBANCARDINFO,*PVSNETBANCARDINFO;


typedef struct
{
    unsigned char     m_powerstate;                 //power state
    unsigned char     m_reseved[7];                 //reserved
    VSNETFANSTATE     m_fanstate[MAX_FAN_NUM];      //fan state
    VSNETBANCARDINFO  m_bancardinfo[MAX_CLIP_NUM];  //card information
} VSNETDECVICEPARAM,*PVSNETDECVICEPARAM;  

typedef struct
{
    int    m_clipstate;                 //clip card state
    int    m_clipnum;                   //clip card num
}VSNETUPDATESLOT;

// NAS state Information
#define NAS_STATE_UNENABLE      1
#define NAS_STATE_UNCONNECT     2
#define NAS_STATE_CONNECT       4
#define NAS_STATE_INUSE         8

typedef enum
{
    MNT_CIFS,
    MNT_NFS,
}MNTFILETYPE;

typedef struct
{
    char m_NASenable;               //Enable NAS
    char m_reseved[3];              //reseved
    char m_NASip[32];               //NAS IP
    char m_NASpath[80];             //NAS path
    char m_NASuser[40];             //NAS username
    char m_NASpassword[40];         //NAS password
    MNTFILETYPE  mnt_type;
}VSNETNASINFO;

typedef struct
{
    int  m_state;                   //Disk State 1、not use 2、Unlink  4.linked 8.on using
    int	 m_totalsize;	            //Disk Size
    int  m_freesize;                //free size 
}VSNETNASSTATE;

typedef struct{
	VSNETNASINFO  m_NASinfo[8]; 
	VSNETNASSTATE m_NASstate[8];
}VSNETNASPARAM;

typedef struct{
	char              m_url[32];                     //IP address
	int               m_port;                        //data port
}VSNETINFONET;

typedef struct{
    VSNETINFONET      m_netinfo[4];                  //net information
	char              m_sername[40];                 //server name
	char              m_username[20];                //user name
	char              m_password[20];                //password
	int               m_cardnum;                     //card number
	unsigned char     m_isexist;                     //card be exist
    unsigned char     m_type;                        //card type
    unsigned char     m_state;                       //card state
    unsigned char     m_reseved;                     //reseved
}VSNETCARDINFO;

typedef struct{
	VSNETCARDINFO m_cardinfo[8];
}VSNETCARDPARAM;

/************************************ 88258 END ******************************************/
#endif//__VSNET_STRUCTDEF_H__

