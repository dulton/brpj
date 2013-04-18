// VSNETStructDef.h
#ifndef __VSNET_STRUCTDEF_H__
#define __VSNET_STRUCTDEF_H__

enum
{
    VSNETALARMMSG_SERSTART = 1,                // Server Start
    VSNETALARMMSG_MOTION,                      // Moving Detection Alarm
    VSNETALARMMSG_VIDEOLOST,                   // Video Lost Alarm
    VSNETALARMMSG_SENSOR,                      // Sensor Alarm
    VSNETALARMMSG_DISKFULL,                    // Disk Full Alarm
    VSNETALARMMSG_HIDEALARM,                   // Video Hide Alarm
    VSNETALARMMSG_SERSTOP,                     // Server Stop
    VSNETALARMMSG_DISKERROR,                   // Disk Error Alarm(smart)
    VSNETALARMMSG_ACCESSVIOLATION,             // violative access
    VSNETALARMMSG_ANALYSESINGLELINEALARM = 11, // Intellective Analyses and single-line Alarm
    VSNETALARMMSG_ANALYSEDOUBLELINEALARM,      // Intellective Analyses and double-line Alarm
    VSNETALARMMSG_ANALYSEREGIONENTRYALARM,     // Alarm for entering Intellective Analyses Region
    VSNETALARMMSG_ANALYSEREGIONEXITALARM,      // Alarm for Leaving Intellective Analyses Region
    VSNETALARMMSG_ANALYSEHOVERALARM,           // Alarm for hovering Intellective Analyses Region
    VSNETALARMMSG_NETANOMALYALARM,             // Network Anomaly Alarm
    VSNETALARMMSG_NVR_PUSENSOR,                // NVR pu Sensor Alarm
	VSNETALARMMSG_DISKLOST,                    // Disk Lost Alarm
	VSNETALARMMSG_ALARM_IN_SHUT = 34,          // Sensor Alarm stop
    VSNETALARMMSG_SGLINEALARM = 100,           // Alarm for Intellective Analyses caution line(>=100 and <=109，Now There are 10 rules for caution line
    VSNETALARMMSG_NODISK = 110,                // No Disk
    VSNETALARMMSG_NET_BROKEN,                  // network line broken
    VSNETALARMMSG_USBSTATE = 200,              // DX USB status 0:plugin 1:plugout
    VSNETALARMMSG_AUDIOCH = 201,               // DX auidoch
    VSNETALARMMSG_DXSWITCHCH = 202,            // DX Swtch Channel
    VSNETALARMMSG_DVDSTATE = 203,              // DX DVD status 0:plugin 1:plugout
};

enum ENUM_VSNET_VIDEO_RESOLUTION
{
    VSNET_VIDEO_RESOLUTION_QCIF = 0,
    VSNET_VIDEO_RESOLUTION_CIF,
    VSNET_VIDEO_RESOLUTION_2CIF,
    VSNET_VIDEO_RESOLUTION_4CIF,
    VSNET_VIDEO_RESOLUTION_DCIF,
    VSNET_VIDEO_RESOLUTION_QVGA,               // 320*240
    VSNET_VIDEO_RESOLUTION_VGA_60HZ,           // 640*480
    VSNET_VIDEO_RESOLUTION_SVGA_60HZ,          // 800*600
    VSNET_VIDEO_RESOLUTION_XGA_60HZ,           // 1024*768
    VSNET_VIDEO_RESOLUTION_SXGA_60HZ,          // 1280*1024
    VSNET_VIDEO_RESOLUTION_UXGA_60HZ,          // 1600*1200
    VSNET_VIDEO_RESOLUTION_720P,               // 1280*720, no use
    VSNET_VIDEO_RESOLUTION_HDTV,               // 1920*1080, no use
    VSNET_VIDEO_RESOLUTION_SVGA_75HZ,          // 800*600
    VSNET_VIDEO_RESOLUTION_XGA_75HZ,           // 1024*768
    VSNET_VIDEO_RESOLUTION_720P_50HZ,          // 1280*720, 50HZ
    VSNET_VIDEO_RESOLUTION_720P_60HZ,          // 1280*720, 60HZ
    VSNET_VIDEO_RESOLUTION_1080P_50HZ,         // 1920*1080, 50HZ
    VSNET_VIDEO_RESOLUTION_1080P_60HZ,         // 1920*1080, 60HZ
    VSNET_VIDEO_RESOLUTION_LTF,                // 240*192
    VSNET_VIDEO_RESOLUTION_WQVGA1,             // 480*352
    VSNET_VIDEO_RESOLUTION_WQVGA2,             // 480*272
    VSNET_VIDEO_RESOLUTION_UVGA_50HZ,          // 1280*960,50HZ
    VSNET_VIDEO_RESOLUTION_UVGA_60HZ,          // 1280*960,60Hz
    VSNET_VIDEO_RESOLUTION_1080P_30HZ,         // 1920*1080, 30HZ
    VSNET_VIDEO_RESOLUTION_1080I_50HZ,         // 1920*1080, 50HZ
    VSNET_VIDEO_RESOLUTION_1080I_60HZ,         // 1920*1080, 60HZ
    VSNET_VIDEO_RESOLUTION_SXGA_75HZ,          // 1280*1024, 75HZ
    VSNET_VIDEO_RESOLUTION_WXGA_60HZ,          // 1280*800, 60HZ
    VSNET_VIDEO_RESOLUTION_WXGA_75HZ,          // 1280*800, 75HZ
    VSNET_VIDEO_RESOLUTION_SXGAP_60HZ,         // 1400*1050, 60HZ SXGA+
    VSNET_VIDEO_RESOLUTION_SXGAP_75HZ,         // 1400*1050, 75HZ SXGA+
    VSNET_VIDEO_RESOLUTION_WXGAP_60HZ,         // 1440*900,  60HZ WXGA+
    VSNET_VIDEO_RESOLUTION_WSXGAP_60HZ,        // 1680*1050, 60HZ WSXGA+
    VSNET_VIDEO_RESOLUTION_WSUVGAP_60HZ,       // 1920*1080, 60HZ WSUVGA+
    VSNET_VIDEO_RESOLUTION_1366X768_60HZ,      // 1366*768, 60HZ
    VSNET_VIDEO_RESOLUTION_WXGA_59HZ,          // 1280*800, 60HZ,非标
    VSNET_VIDEO_RESOLUTION_1280X720_59HZ,      // 1280*720, 60HZ 非标
    VSNET_VIDEO_RESOLUTION_1360X768_60HZ,      // 1360*768, 60HZ
};

// record type
enum
{
    HDISK_RECTYPE_HAND       = (1 << 0),
    HDISK_RECTYPE_TIMER      = (1 << 1),
    HDISK_RECTYPE_MOTION     = (1 << 2),
    HDISK_RECTYPE_ALARM      = (1 << 3),
    HDISK_RECTYPE_VILOST     = (1 << 4),
    HDISK_RECTYPE_VIHIDE     = (1 << 5),
    HDISK_RECTYPE_OTHER      = (1 << 6),
    HDISK_RECTYPE_BACKUP     = (1 << 7),
    HDISK_RECTYPE_NETANOMALY = (1 << 8),
    HDISK_RECTYPE_PLATE      = (1 << 9), // 车牌识别触发的抓拍图片
};

// Transmit Customer's Rights
#define VSNET_USER_RIGHT_ADMIN      1   // Administrator Right
#define VSNET_USER_RIGHT_HIOPER     2   // High-grade Operator
#define VSNET_USER_RIGHT_OPERIAL    3   // Operator

#define VSNET_DVR_MAXCH             64  // Max number for channel
#define VSNET_DVR_MAXALARMOUT       16  // Max number for alarm output
#define VSNET_DVR_MAXTRACKNUM       16  // Max number for track
#define VSNET_DVR_MAXVONUM          8   // Max number for vo

/**********************************VSNET_DVR_GUI_PORT******************************************/
#define VSNET_DVR_GUI_PORT          10000   // DVR GUI内部通信端口10000~10999，DVR GUI专用
// 10001:参数端口
// 10002:串口数据
/************************************VSNET_DVR_GUI_PORT****************************************/

typedef struct
{
    char                m_username[20];     // Username
    char                m_password[20];     // Password
} ONEUSER;

typedef struct
{
    ONEUSER             m_admin;            // Administrator
    ONEUSER             hl_operator[10];    // High-grade Operator
    ONEUSER             m_operator[10];     // Operator
} WHOLE_USER;

typedef struct
{
    ONEUSER             m_adminsuper;
    ONEUSER             m_admin;            // Administrator
    ONEUSER             hl_operator[10];    // High-grade Operator
    ONEUSER             m_operator[10];     // Operator
} WHOLE_USERSUPER;

typedef struct
{
    int                 bUseDDNS;           // DDNS Enable
    char                DDNSSerIp[40];      // DDNS Server IP
    unsigned short      DDNSSerPort;        // DDNS Server Port
    unsigned short      LocalMapPort;       // Local Port
} WHOLE_DDNS;

typedef struct
{
    char                pStrWebRomVer[50];       // Web Page Version
    char                pStrWebRomBuildTime[50]; // Web Page Compile Time
} VERSIONWEBROM;

typedef struct
{
    char                pStrBSPVer[50];       // BSP Version
    char                pStrAPPVer[50];       // Application Version
    char                pStrBSPBuildTime[50]; // BSP Compile Time
    char                pStrAPPBuildTime[50]; // Application Compile Time
} VERSIONINFO;

// Flash版本信息
typedef struct
{
    char                pStrFLASHVer[64];       // FLASH Version
    char                pStrFLASHBuildTime[64]; // FLASH Compile Time
} VSNET_FLASHVERSION;

typedef struct
{
    char                m_servername[24];   // Server Name
    char                m_serverip[16];     // IP Address
    char                m_servermask[16];   // Subnet Mask
    char                m_gatewayAddr[16];  // GateWay
    char                m_dnsaddr[16];      // DNS Address
    char                m_multiAddr[16];    // Multicasting Address
    unsigned short      m_serport;          // Data Port
    unsigned short      m_mulport;          // Multicasting Port
    unsigned short      m_webport;          // Web Page Port
    unsigned char       m_isPAL;            // Video Mood(PAL,NTSC)
    unsigned char       m_launage;          // Language
    unsigned char       m_phyAddr[6];       // Physical Address
    unsigned short      m_reserved;         // reserved
    unsigned char       m_serial[8];        // Serial Number
    WHOLE_DDNS          m_ddns;             // DDNS Parameter
    VERSIONINFO         m_version;          // Version
} WHOLEPARAM;

typedef struct
{
    char                m_servername[60];   // Server Name
    char                m_serverip[16];     // IP Address
    char                m_servermask[16];   // Subnet Mask
    char                m_gatewayAddr[16];  // GateWay
    char                m_dnsaddr[16];      // DNS Address
    char                m_multiAddr[16];    // Multicasting Address
    unsigned short      m_serport;          // Data Port
    unsigned short      m_mulport;          // Multicasting Port
    unsigned short      m_webport;          // Web Page Port
    unsigned char       m_isPAL;            // Video Mood(PAL,NTSC)
    unsigned char       m_launage;          // Language
    unsigned char       m_phyAddr[6];       // Physical Address
    unsigned short      m_reserved;         // Reserved
    unsigned char       m_serial[8];        // Serial Number
    WHOLE_DDNS          m_ddns;             // DDNS Parameter
    VERSIONINFO         m_version;          // Version
} WHOLEPARAMEX;

typedef struct
{
    unsigned char       m_starthour;        // Start Hour
    unsigned char       m_startmin;         // Start Minute
    unsigned char       m_stophour;         // Stop Hour
    unsigned char       m_stopmin;          // Stop Hour
    unsigned char       m_maskweek;         // Week
    unsigned char       bReceive[3];        // Reserved
} TIMECHECK;

typedef struct
{
    TIMECHECK           pList[7];           // Time Table
} TIMELIST;

typedef struct
{
    unsigned char       m_beanb;             // Moving Detection Enable
    unsigned char       m_alarmsenstive;     // sensitivity of Moving Detection
    unsigned char       m_brecord;           // relate to recording
    unsigned char       bReceive;            // Reserved
    unsigned char       m_outputmap[8];      // relate to output
    unsigned char       m_detect[18][22];    // Detection Range
    TIMELIST            m_timelist;          // Detection Time Table
} CHANNMOTION;

typedef struct
{
    unsigned char       m_beanb;             // Moving Detection Enable
    unsigned char       m_alarmsenstive;     // sensitivity of Moving Detection
    unsigned char       m_brecord;           // relate to recording
    unsigned char       m_capjpeg;           // relate to jpeg capture image
    unsigned char       m_singlehost;        // Center for uploading
    unsigned char       m_reseved1;          // Reserved
    unsigned char       m_reseved2;          // Reserved
    unsigned char       m_reseved3;          // Reserved
    unsigned char       m_outputmap[8];      // relate to output
    unsigned char       m_detect[18][22];    // Detection Range
    TIMELIST            m_timelist;          // Detection Time Table
} CHANNMOTIONEX, CHANNVAEX;

typedef struct
{
    int                 nFormat;            // Format of Display
    unsigned short      x, y;               // Coordinate of Display
} TIMEOSD;

typedef struct
{
    int                 bShow;              // Show(1),Hide(0)
    unsigned short      x, y;               // Coordinate of Display
} BITSINFOOSD;

typedef struct
{
    char                lpString[32];       // characters to be showed
    unsigned short      x, y;               // Coordinate
} TITALOSD;

typedef struct
{
    TIMEOSD             m_time;             // Time
    TITALOSD            m_tital[4];         // Title
    BITSINFOOSD         m_bits;             // Bit Rate
} CHANNOSD;

typedef struct
{
    int             m_enable;                   // label of channel OSD superposition for Intellective analyses alarm   0-Disable 1-Enable
    unsigned short  m_x, m_y;                   // Coordinate
    char            m_tripwire_osdText[32];     // literal contents  of OSD superposition for alarm of caution line
    char            m_regionEnter_osdText[32];  // literal contents  of OSD superposition for alarm of entering region
    char            m_regionLeave_osdText[32];  // literal contents  of OSD superposition for alarm of leaving region
    char            m_regionWander_osdText[32]; // literal contents  of OSD superposition for alarm of hovering region
} VSNETCHVAALARMOSD;

#ifndef MAX_OSDSTRLEN
#define MAX_OSDSTRLEN 200
#endif 

typedef struct
{
    unsigned short    m_nStrLen;                     // 设备支持的最大字符个数 只能获取
    unsigned short    m_bShow;                       // show(1),hide(0)    
    char              m_szString[MAX_OSDSTRLEN];     // characters to be showed
    unsigned short    m_nX, m_nY;                    // coordinate
} VSNET_TITALOSD;

//osd叠加扩展 从设备获取支持的最大字符数
typedef struct
{
    TIMEOSD           m_time;            // time
    VSNET_TITALOSD    m_tital[4];        // title
    BITSINFOOSD       m_bits;            // bit rate
} VSNET_CHANNOSD;

typedef struct
{
    int                m_enable;            // label of Intellective analyses alarm  OSD superposition   0-Disable 1-Enable
    VSNETCHVAALARMOSD  m_chAlarmOsd[8];     // setting of OSD superposition for channel Intellective analyses alarm
} VSNETVAALARMOSD;

typedef struct
{
    int                 m_bmask;            // Image Mask Enable
    unsigned short      m_x;                // x
    unsigned short      m_y;                // y
    unsigned short      m_width;            // width
    unsigned short      m_height;           // height
} CHANNVIDEOMASK;

typedef struct
{
    unsigned char       m_bri;              // Image Brightness
    unsigned char       m_con;              // Image Contrast
    unsigned char       m_sat;              // Image Saturation
    unsigned char       m_hue;              // Image Hue
} CHANNVIPARAM;

typedef struct
{
    char                m_channelName[16];  // Channel Name
    int                 m_streamType;       // Stream Type:Video Stream、AV Stream
    int                 m_encodeType;       // Image Type:CIF,2CIF,4CIF
    int                 m_Iinterval;        // key frame interval(10-200)
    int                 m_videoFrameRate;   // Frame rate
    int                 m_bitratetype;      // bit rate type：CBR & VBR
    int                 m_maxqueue;         // VBR Quantified Coefficient(2-31)
    int                 m_minqueue;         // CBR Quantified Coefficient(2-31)
    int                 m_maxbitrates;      // CBR Max bit rate(64K-8000K)
    int                 m_audiosample;      // AF sampling frequency
    int                 m_audiobitrate;     // AF bit rate
    int                 m_delay;            // Alarm Delay
    int                 m_benrecord;        // Regular Recording Enable
    TIMELIST            m_record;           // Regular Recording Time Table
} CHANNELPARAM;

typedef struct
{
    char                m_channelName[60];  // Channel Name
    int                 m_streamType;       // Stream Type:Video Stream、AV Stream
    int                 m_encodeType;       // Image TypeCIF,2CIF,4CIF
    int                 m_Iinterval;        // key frame interval(10-200)
    int                 m_videoFrameRate;   // Frame rate
    int                 m_bitratetype;      // bit rate type：CBR & VBR
    int                 m_maxqueue;         // VBR Quantified Coefficient(2-31)
    int                 m_minqueue;         // CBR Quantified Coefficient(2-31)
    int                 m_maxbitrates;      // CBR Max bit rate(64K-8000K)
    int                 m_audiosample;      // AF sampling frequency
    int                 m_audiobitrate;     // AF bit rate
    int                 m_delay;            // Alarm Delay
    int                 m_benrecord;        // Regular Recording Enable
    TIMELIST            m_record;           // Regular Recording Time Table
} CHANNELPARAMEXNAME;

typedef struct
{
    char                m_channelName[16];  // Channel Name
    int                 m_streamType;       // Stream Type:Video Stream、AV Stream
    int                 m_encodeType;       // Image TypeCIF,2CIF,4CIF
    int                 m_Iinterval;        // key frame interval(10-200)
    int                 m_videoFrameRate;   // Frame rate
    int                 m_bitratetype;      // bit rate type：CBR & VBR
    int                 m_maxqueue;         // CBR Quantified Coefficient(2-31)
    int                 m_minqueue;         // VBR Quantified Coefficient(2-31)
    int                 m_maxbitrates;      // CBR Max bit rate(64K-8000K)
    int                 m_audiosample;      // AF sampling frequency
    int                 m_audiobitrate;     // AF bit rate
    int                 m_delay;            // Alarm Delay
    int                 m_nPrerecordTime;   // Prerecording Time(0=off,1-10=ON)
    int                 m_videofilebackup;  // Video File Backup(0=OFF,1=ONLY Alarm File,2=All Files)
    int                 m_jpegfilebackup;   // Jpeg File Backup(0=OFF,1=ONLY Alarm File,2=All Files)
    int                 m_benrecord;        // Regular Recording Enable
    TIMELIST            m_record;           // Regular Recording Time Table
} CHANNELPARAMEX;

typedef struct
{
    int                 m_baudrate;         // baud rate
    int                 decoderModel;       // Protocol Type(Not Use)
    unsigned char       m_databit;          // Data bit
    unsigned char       m_stopbit;          // Stop bit
    unsigned char       m_checkbit;         // Check bit
    unsigned char       m_flowcontrol;      // Flow Control
    unsigned char       decoderAddress;     // Decoder Address
    unsigned char       bReceive[3];        // Reserved
} SERIAL485_PARAM;

typedef struct
{
    int                 baudrate;           // baud rate
    char                databit;            // data bit
    char                stopbit;            // stop bit
    char                checkbit;           // check bit
    char                flowcontrol;        // flow control
} VSSERIAL_INFO;

typedef struct
{
    int                 m_year;             // Year
    unsigned char       m_month;            // Month
    unsigned char       m_dayofmonth;       // Day
    unsigned char       m_dayofweek;        // Week
    unsigned char       m_hour;             // Hour
    unsigned char       m_minute;           // Minute
    unsigned char       m_second;           // Second
    unsigned char       bReceive[2];        // Reserved
} TIME_PARAM;

typedef struct
{
    int                 m_bias;             // Timezone in minutes
} TIMEZONE_PARAM;

typedef struct
{
    unsigned char       m_record[16];       // relate to recording
    unsigned char       m_out[8];           // relate to output
    unsigned char       m_enpreno[16];      // relate to preset point
    unsigned char       m_preno[16];        // number of preset point to be called
    TIMELIST            m_timelist;         // detection time table
} ALARMRECORD;

typedef struct
{
    int                 m_alarmmode;        // Alarm Mode 0：OFF,1：ON
    int                 m_singlehost;       // label of center for uploading alarm signal
    unsigned char       m_record[16];       // relate to recording
    unsigned char       m_out[8];           // relate to output
    unsigned char       m_enpreno[16];      // relate to preset point
    unsigned char       m_preno[16];        // number of preset point to be called
    unsigned char       m_capjpeg[16];      // relate to capturing
    TIMELIST            m_timelist;         // detection time table
} ALARMRECORDEX;

typedef struct
{
    int                 m_benab;            // Alarm Enable
    ALARMRECORD         m_alarmrec[8];      // parameter of relating to alarm
} ALARMPARAM;

typedef struct
{
    int                 m_benab;            // Alarm Enable
    ALARMRECORDEX       m_alarmrec[8];      // parameter of relating to alarm
} ALARMPARAMEX;

typedef struct
{
    int                 m_benab;            // 使能
    ALARMRECORDEX       m_alarmrec[9];      // 1 + 8(外接报警盒上的8个报警输入)
} ALARMPARAMEX_SEA;

typedef struct
{
    int                 m_chanuser[16];     // Channel
    unsigned char       m_chanIP[16][20][4];// Linked IP Address
} CONNECTIP;

typedef struct
{
    unsigned int        m_videotag;         // Video tag
    short               m_width, m_height;  // Video Size
    int                 m_bhaveaudio;       // have audio or not
    int                 m_samplesize;       // audio sampling frequency
    unsigned short      m_audiotag;         // audio tag
    unsigned short      m_audiobitrate;     // audio bit rate
} VSTREAMINFO;

typedef struct
{
    unsigned int        m_url;              // URL address
    char                m_sername[24];      // Server Address
    int                 m_ch;               // channel
    int                 trantype;           // transport type
    int                 handle;             // handle
} VSUSERINFO;

typedef struct
{
    int                 m_benable;          // hide detection enable
    int                 m_decval;           // sensitivity of detection(1(lowest)---3(highest))
    unsigned char       m_out[8];           // output of relating alarm
    TIMELIST            pTimeList;          // detection time
} CHANNELHIDEALARM;

typedef struct
{
    int                 m_benable;          // hide detection enable
    int                 m_bsinglehost;      // label of center for alarm signal
    int                 m_decval;           // sensitivity of detection(1(lowest)---3(highest))
    unsigned char       m_out[8];           // output of relating alarm
    TIMELIST            pTimeList;          // detection time
} CHANNELHIDEALARMEX;

typedef struct
{
    int                 m_benbale;          // Regular Restart Enable
    int                 m_invalidtime;      // interval time(HOUR AS UNIT，10h-240h:10 hours to 10 days)
    int                 m_rsthour;          // Restart Hour
    int                 m_rstminute;        // Restart Minute
} VSTIMERRESET;

typedef struct
{
    char                m_hostserverurl[40]; // IP of Recording Server
    char                m_alarservermurl[40];// IP of Alarm Server
    unsigned short      m_hostserverport;    // Port of Recording Server
    unsigned short      m_alarserverport;    // Port of Alarm Server
} VSREMOTEHOST;

typedef struct
{
    int                 m_streamType;       // Stream Type:Video Stream、AV Stream
    int                 m_encodeType;       // Image TypeCIF,2CIF,4CIF
    int                 m_Iinterval;        // Key Frame interval(10-200)
    int                 m_videoFrameRate;   // frame rate
    int                 m_bitratetype;      // bit rate type：CBR & VBR
    int                 m_maxqueue;         // VBR Quantified Coefficient(2-31)
    int                 m_minqueue;         // CBR Quantified Coefficient(2-31)
    int                 m_maxbitrates;      // CBR max bit rate(64K-8000K)
} VSSUBCHANPARAM;

typedef struct
{
    int                 m_hds;             // Total of Hard Disk
    int                 m_hdtype[8];       // Disk Type
    int                 m_hdstate[8];      // Disk State 0xff-错误，0xfe-空硬盘
    int                 m_totalsize[8];    // Disk Size
    int                 m_freesize[8];     // free size
} VSDISKSTATE;

typedef struct
{
    unsigned int        m_channel;         // Channel NO.
    unsigned short      m_detectall;       // all alarm
    unsigned char       m_detect[18][22];  // alarm block
} VSNETVIDEOMOVE_BLOCK;

typedef struct
{
    int                 m_busepppoe;        // USE PPPOE
    char                m_pppoename[64];    // PPPOE username
    char                m_pppoepswd[64];    // PPPOE password
} VSNETPPPOEPARAM;

typedef struct
{
    int      m_packet_type;                 // packet type of recording(0:packing with file size; 1:packing with time)
    int      m_packet_size;                 // packet size of recording，MB as UNIT
    int      m_packet_time;                 // packet time of recording，Minute as UNIT
    int      m_hdisk_reserved_size;         // Reserved Space of DISK，MB as UNIT
    int      m_hdisk_full_alarm;            // whether alarm when free space less than reserved space of hard disk
    int      m_hdisk_full_action;           // the action when free space less than reserved space of hard disk(1:circular covering; 0:stop recording)
} VSNETRECORDPARAM;

typedef struct
{
    int                 m_bsinglehost;      // label of center for uploading
    unsigned char       m_out[8];           // alarm output
    TIMELIST            m_timelist;         // detection time
} VSNETVIDEOLOSTPARAM;

typedef struct
{
    unsigned char       m_caprate;          // frame rate of capturing (1-5)
    unsigned char       m_encquant;         // quality of capturing(JPEG,1-100)
    unsigned char       m_encformat;        // format of capturing(CIF,2CIF,4CIF)
    unsigned char       m_reserved;         // reserved
    int                 m_capnums;          // frame number to be captured(-1:keep capturing)
} VSNETJPEGCAPPARAM;

typedef struct
{
    char                m_serverIP[40];     // Server IP
    unsigned int        m_port;             // Server Port
    unsigned int        m_intervaltime;     // interval time
    char                m_senddata[512];    // data to be sent
} VSNETHOSTNOTIFY;

typedef struct
{
    char                m_alarmname[32];    // name of alarm sensor
    int                 m_senseorinterval;  // interval time for sensor alarming
} VSNETSENSORALARM;

typedef struct
{
    int  m_videolossinterval;   // interval time for video losing
    int  m_videomoveinterval;   // interval time for video moving
    int  m_videohideinterval;   // interval time for video hiding
    int  m_reserved;            // reserved
} VSNETALARMINTERVAL;           // interval time for alarming

typedef struct
{
    int     m_alarmtype;        // sensor type
} ALARMTYPE;

typedef struct
{
    int m_audioinline;          // 1:linear input  0:Mic input
    int m_audioinDBline;        // proportion of linear input(-34 - +12DB) range of linear input(0-100)
    int m_audioinDBMic;         // proportion of Mic input(0,20DB)  range of Mic input(0-1)
    int m_audiooutDB;           // proportion of input(-73 - +6DB)  range of  input(0-100)
} VSNETAUDIOPARAM;              // structure of audio parameter

// structure of audio silent
typedef struct
{
    int m_audiosilent;          // silent:0， not:others
} VSNETAUDIOSILENT;

typedef struct
{
    int             m_usecdma;  // use cdma
} VSNETCDMAPARAM;

typedef struct
{
    int             m_usewifi;          // use wifi
    char            m_wifiipaddr[16];   // wireless IP address
    char            m_wifinetmask[16];  // wireless MASK address
    char            m_wifigateway[16];  // wireless gateway
    char            m_wifidns[16];      // wireless DNS
    char            m_wifissid[40];     // wireless ssid
    int             m_wifiencmode;      // encription mode 0,no encription 1,wep-60  2,wep-128
    char            m_wifipwd[64];      // cryptographic key wep-60(5 ASCII or 10 HEX) wep-128(13 ASCII or 26 HEX)
} VSNETWIFIPARAM;

typedef struct
{
    int             m_usewifi;          // use wifi 0,OFF 1,Static 2,ppoe 3,DHCP
    int             m_usedefaultgw;     // wireless gateway as default enable (zero,disable nonzero,enable)
    char            m_pppoename[64];    // wireless PPPOE username
    char            m_pppoepswd[64];    // wireless PPPOE password
    char            m_wifiipaddr[16];   // wireless IP address
    char            m_wifinetmask[16];  // wireless MASK address
    char            m_wifigateway[16];  // wireless gateway
    char            m_wifidns[16];      // wireless DNS
    char            m_wifissid[40];     // wireless ssid
    int             m_wifiencmode;      // encription mode 0,no encription 1,wep-60  2,wep-128 3,wep-152 8,wpa-psk/wpa2-psk
    char            m_wifipwd[64];      // cryptographic key wep-60(5 ASCII or 10 HEX) wep-128(13 ASCII or 26 HEX)
} VSNETWIFIPARAMEX;

typedef struct
{
    char            m_wifissid[40];     // wireless ssid
    int             m_wifiencmode;      // encription mode      0-no encription 1-WEP 2-WPA-PSK/WPA2-PSK
} VSNETWIFISSIDINFO;

#define MAX_SSIDNUM 40
typedef struct
{
    int                 m_ssidnum;      // number of wireless router which have been searched
    VSNETWIFISSIDINFO   m_ssidinfo[MAX_SSIDNUM]; // route information
} VSNETWIFISSIDLIST;

typedef struct
{
    char            m_deviceID[32];      // Device ID
    int             m_maxconnect;        // max connection number,computing with 5 per channel
    int             m_linktype;          // link type,1:LAN,2:ADSL,3:others
    char            m_ftpuser[20];       // ftp username
    char            m_ftppass[20];       // ftp password
    char            m_devicemodel[32];   // device model
    int             m_sipport;           // sip port
} VSNETLINKCONFIG;                       // link configuration

typedef struct
{
    int             m_dnsprovider;       // ddns provider
    int             m_ddnsnotifytime;    // ddns update time
    char            m_dnsusername[20];   // ddns username
    char            m_dnspassword[20];   // ddns password
    char            m_dnsdomainname[50]; // dns domain name
} VSNETDNSEXPARAM;

typedef struct
{
    int             m_comport;           // serial port NO.
    int             m_protocol;          // protocol of serial port
    int             m_addr;              // device address
    int             m_alarmchecktime;    // time of alarm check
    int             m_alarmintervaltime; // interval time of alarm
} VSNETEXALARMPARAM;

typedef struct
{
    int             m_benablecheck;      // temperature alarm enable
    TIMELIST        m_timeList;          // detection time
    char            m_boutput[8];        // alarm output,each bit maps alarm output
    char            m_record[16];        // recording label
    char            m_capture[16];       // capturing label
    int             m_delay;             // alarm delay time
    int             m_singlehost;        // center  for uploading
    float           m_tempmax;           // max temperature
    float           m_tempmin;           // min temperature
    float           m_hummax;            // max humidity
    float           m_hummin;            // min humidity
} VSNETHUMITUREPRAM;

typedef struct
{
    int             m_xoffset;           // horizontal offset 0~32
} VSNETVIDEOOFFSET;

typedef struct
{
    unsigned short      m_x;             // x
    unsigned short      m_y;             // y
    unsigned short      m_width;         // width
    unsigned short      m_height;        // height
} VSNETVIDEOMASKAREA;

typedef struct
{
    int                 m_bmask;         // image mask enable
    VSNETVIDEOMASKAREA  m_maskarea[4];   // image mask area
} VSNETVIDEOMASK;

typedef struct
{
    unsigned short      m_x;             // x
    unsigned short      m_y;             // y
    unsigned short      m_width;         // width
    unsigned short      m_height;        // height
    int                 m_qp;            // quality,range(2-31),low is better
} VSNET_VIDEOINTEREST_AREA;

typedef struct
{
    int                      m_enable;   // video interest enable
    VSNET_VIDEOINTEREST_AREA m_area[4];  // video interest area
} VSNET_VIDEOINTEREST;

typedef struct
{
    char                mans_url[128];       // address of manager server
    char                mans_username[32];   // username of manager server
    char                mans_psd[32];        // password of manager server
    char                mans_conrequrl[128]; // inverse link PU address
    char                mans_conreqname[32]; // inverse linkPU username
    char                mans_conreqpsd[32];  // inverse link PU password
} VSNETSNMPCONFIG;

typedef struct
{
    int                 m_nyear;            // received time
    unsigned char       m_nmonth;
    unsigned char       m_nday;
    unsigned char       m_nhour;
    unsigned char       m_nminute;
    unsigned char       m_nsecond;
    unsigned char       m_received[3];      // reserved
    char                m_peplenum[100];    // supercargo number
    char                m_boxnum[100];      // box number
    unsigned short      m_recvnum;          // received box number
    unsigned short      m_sendnum;          // sent box number
} NETRVS_BANKINFO;

typedef struct
{
    int             m_CapJpgEnable;         // regular capturing enable
    int             m_ftp_timecapjpg;       // regular capturing FTP uploading
    int             m_CapJpgInterval;       // interval time for regular capturing
    TIMELIST        m_timeList;             // capture time
} VSNETTIMERCAP;

typedef struct
{
    int             m_busedhcp;             // use DHCP
} VSNETUSEDHCP;

typedef struct
{
    char            m_pppoename[64];        // wifi PPPOE username
    char            m_pppoepswd[64];        // wifi PPPOE password
} VSNETWIFIPPPOEPARAM;


typedef struct
{
    int m_busedefaultaddr;  // use default address or not
    int m_busealarmsms;     // message alarm enable
    int m_interval;         // min interval for sending
} VSNETALARMSMSPARAM;       // parameter structure of message for setting alarm

typedef struct
{
    int  m_zone;         // exactly timezone
    int  m_nReserve[3];  // reserved
} VSNET_EXACTTIMEZONE;

/***************** long-distance retrieval,download,play ****************/
#define  MAX_FILE_DATA            10

typedef struct
{
    int                 m_year;             // year
    unsigned char       m_month;            // month
    unsigned char       m_day;              // day
    unsigned char       m_hour;             // hour
    unsigned char       m_minute;           // minute
    unsigned char       m_second;           // second
    unsigned char       Received[3];        // reserved
} VSNETPLAY_NETTIME, VSNETDIARY_NETTIME;

typedef struct
{
    int                 m_channel;          // channel NO.
    char                sFileName[100];     // file name
    VSNETPLAY_NETTIME   struStartTime;      // start time of file
    VSNETPLAY_NETTIME   struStopTime;       // end time of file
    unsigned int        dwFileSize;         // file size
} VSNETPLAY_FIND_DATA;

typedef struct
{
    int                 m_filenum;                 // file number
    VSNETPLAY_FIND_DATA m_filedata[MAX_FILE_DATA]; // file attribute
} VSNETPLAY_FILE_DATA;

typedef struct
{
    char                desc[200];
} VSNETALARMHOST_DESC;


/*************************FTP uploading parameter********************************/
typedef struct
{
    int                 m_enable_ftp;      // label of FTP upload
    char                m_remote_IP[32];   // IP address
    unsigned short      m_remote_port;     // port
    unsigned short      m_reserved;        // reserved for alignment
    char                m_ftp_user[40];    // Ftp username
    char                m_ftp_pwd[40];     // Ftp password
} VSNETFTPINFO;

/*************************UPNP parameter********************************/
typedef struct
{
    int                 m_enable_upnp;     // upnp enable
} VSNETUPNPCFG;

typedef struct
{
    int                 m_upnp_status;     // upnp status
} VSNETUPNPSTATUS;

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
    char        m_mail_name[SEND_MAIL_NAME_LEN];    // sender name
    char        m_mail_addr[SEND_MAIL_ADDR_LEN];    // sender address
} VSNETMAILADDR;

typedef struct
{
    int                 m_enable_sendmail;                     // sending alarm mail enable
    char                m_server_addr[SEND_MAIL_ADDR_LEN];     // mail server address
    int                 m_server_port;                         // mail server port，25 as default
    char                m_auth_name[SEND_MAIL_STRING_LEN];     // mail verification name
    char                m_auth_pwd[SEND_MAIL_STRING_LEN];      // mail verification password

    int                 m_priority;                            // priority of mail
    char                m_subject[SEND_MAIL_MAX_SUBJECT_LEN];  // title of mail
    char                m_content[SEND_MAIL_MAX_CONTENT_LEN];  // content of mail
    VSNETMAILADDR       m_from_addr;                           // sender address
    VSNETMAILADDR       m_send_addr[SEND_MAIL_MAX_ADDR_NUM];   // receiver address
} VSNETMAILCFG;

/*************************CCD parameter ********************************/
typedef struct
{
    int              m_shuttermode;      // shutter mode 0:AES; 1:MES
    int              m_shtspeed;         // shutter speed 0~255
    int              m_aelevel;          // AE level 0~255

    int              m_awb;              // balance mode 0~255
    int              m_wbr;              // balance by hand R 0~255
    int              m_wbg;              // balance by hand G 0~255
    int              m_wbb;              // balance by hand B 0~255

    int              m_vflip;            // vertical overset 0:OFF; 1:ON
    int              m_hmirror;          // horizontal overset 0:OFF; 1:ON

    int              m_exposuremode;     // exposure mode 0:AE; 1:ME
    int              m_blc;              // back light compensation 0:OFF; 1:ON
    int              m_agc;              // automatic gain 0~255
    int              m_iris;             // iris 0~255
    int              m_toblackwhite;     // color to black-and-white 0:automatic; 1:infrared(night)
    int              m_zoom;             // zoom 0:OFF; 1:ON

    int              m_lightmode;        // switch between day and night 0~255
    int              m_flc;              // 0:OFF; 1:ON
    int              m_switch;           // switch between positive and negative 0:Posi 1:Nega
    int              m_sense;            // 0:OFF; 1:ON
    int              m_sensemax;         // 0:x2; 1:x4; ...; 7:x256
    int              m_prio;             // 0:AGC; 1:Sense up
} VSNETCCDPARAM;
/*************************SENSOR parameter ********************************/
// 色度参数
typedef struct
{
    int     m_nRed;      // 红色，取值范围0 ~ 1023
    int     m_nGreen;    // 绿色，取值范围0 ~ 1023
    int     m_nBlue;     // 蓝色，取值范围0 ~ 1023
} VSNET_VIRBGPARAM;

// 彩转黑参数
typedef struct
{
    int     m_nMode;     // 0:彩色；1:黑白；2:自动  
    int     m_nDay;      // 白天阈值，取值范围0~255，自动时有效
    int     m_nNight;    // 夜晚阈值，取值范围0~255，自动时有效
} VSNET_COLORMODE;

// SENSOR参数
typedef struct
{
    int                 m_nRGBEnable;            // RGB使能，0：关闭;1：开启
    VSNET_VIRBGPARAM    m_nRGB;                  // RGB参数值
    
    int                 m_nShutEnable;           // 快门使能，0：关闭;1：开启
    int                 m_nShutData;             // 快门速度，按位表示：1 < 0 1/25 1 < 1 1/30 1 < 2 1/50... 1 < 12 1/100K...
    int                 m_nShutMode;             // 快门模式，0：AES; 1：MES
    int                 m_nShutSpeed;            // 固定快门速度 0~255
    
    int                 m_nAwbEnable;            // 白平衡使能，0：关闭;1：开启
    int                 m_nAwb;                  // 白平衡模式，0：自动;1：手动
    int                 m_nWbr;                  // 手动白平衡R，取值范围0 ~ 255
    int                 m_nWbg;                  // 手动白平衡G，取值范围0~255
    int                 m_nWbb;                  // 手动白平衡B，取值范围0~255
    
    int                 m_nVflipEnable;          // 垂直翻转使能，0：关闭;1：开启
    int                 m_nVflip;                // 垂直翻转，0：OFF; 1：ON
    int                 m_nHmirrEnable;          // 水平翻转使能，0：关闭;1：开启
    int                 m_nHmirr;                // 水平翻转，0：OFF; 1：ON
    
    int                 m_nExposEnable;          // 曝光模式 0：关闭;1：开启
    int                 m_nExposMode;            // 曝光模式 0：AE; 1：ME
    
    int                 m_nBlcEnable;            // 背光补偿使能 0：关闭;1：开启
    int                 m_nBlc;                  // 背光补偿 0：OFF; 1：ON
    
    int                 m_nAgcEnable;            // 自动增益使能，0：关闭;1：开启
    int                 m_nAgcData;              // 按位：1 < 0 低 1 < 1 中 1 < 2 高 1 < 3 关  
    int                 m_nAgc;                  // 自动增益，0 ~ 3分别是高\中\低\关，默认为高，不能修改因为自动彩转黑必须开增益
    
    int                 m_nIrisEnable;           // 光圈使能，0：关闭;1：开启
    int                 m_nIris;                 // 光圈，取值范围0~255
    
    int                 m_nColorEnable;          // 彩转黑使能，0：关闭;1：开启
    int                 m_nColorData;            // 按位：1 < 0 彩色 1 < 1 黑白 1 < 2 自动
    VSNET_COLORMODE     m_nColorMode;            // 彩转黑参数
    
    int                 m_nZoomEnable;           // 变焦使能，0：关闭;1：开启
    int                 m_nZoom;                 // 变焦 0：OFF; 1：ON
    
    int                 m_nLightEnable;          // 日夜切换使能，0：关闭;1：开启
    int                 m_nLightMode;            // 日夜切换 0~255
    
    int                 m_nFlcEnable;            // 抗闪烁（应用场景）使能，0：关闭;1：开启
    int                 m_nFlc;                  // 抗闪烁（应用场景），0：室内50HZ光源; 1：室内60HZ光源; 2：室外
    
    int                 m_nSwitchEnable;         // 正负片切换使能，0：关闭;1：开启
    int                 m_nSwitch;               // 正负片切换，0：Posi 1：Nega
    
    int                 m_nSenseEnable;          // 锐度菜单使能，0：关闭;1：开启
    int                 m_nSensedate;            // 锐度按位取值，0：低; 1：中  2:较高 3:高
    int                 m_nSenMaxEnable;         // 锐度使能，0：关闭;1：开启
    int                 m_nSenMax;               // 锐度，取值范围0 ~ 30，0：x2; 1：x4; ...; 7：x256...
    
    int                 m_nGammaEnable;          // gamma 使能，0：关闭;1：开启
    int                 m_nGamma;                // gamma值，取值范围0~3
    
    int                 m_nAelEnable;            // AE级别使能，0：关闭;1：开启
    int                 m_nAel;                  // AE级别，取值范围0 ~ 255
    
    int                 m_nSensiEnable;          // 灵敏度使能，0：关闭;1：开启
    int                 m_nSensi;                // 灵敏度，取值范围0~255
    
    int                 m_nBadPtEnable;          // 坏点补偿使能，0：关闭;1：开启
    int                 m_nBadPt;                // 坏点补偿，0：OFF; 1：ON
    
    int                 m_nPrioEnable;           // 0：关闭;1：开启
    int                 m_nPrio;                 // 0：AGC; 1：Sense up
    
    int                 m_nReserve[20];          // 保留，用作扩展
}VSNET_SENSORPARAM;
/************************APP platform configuration parameter*******************************/
typedef struct
{
    unsigned char       m_register_type;       // register type
    unsigned char       m_enable_register;     // register enable
    short               m_reserved;            // reserved for alignment
    unsigned int        m_register_interval;   // interval time for register，Second as UNIT，0 means only register once when device start
    unsigned char       m_sessionid[128];      // platform session ID
    unsigned char       m_store_sessionid[128];// store session ID
} VSNETPLATFORM_REGISTER_STRATEGY;

typedef struct
{
    unsigned char           m_heartbeat_type;    // heart beat type  0 udp  1 tcp
    unsigned char           m_enable_heartbeat;  // heart beat enable
    short                   m_reserved;          // reserved for alignment
    unsigned int            m_heartbeat_interval;// interval time for heart beat ,Second as UNIT
} VSNETPLATFORM_HEARTBEAT_STRATEGY;

/*************************IP-PORT pair structure********************************/
typedef struct
{
    unsigned char           m_ipaddr[32];   // ip
    unsigned int            m_port;         // port
} VSNETPLATFORM_IPPORT;

typedef struct
{
    VSNETPLATFORM_IPPORT       m_mannage_server;   // configuration of manager server
    VSNETPLATFORM_IPPORT       m_register_server;  // configuration of register server
    VSNETPLATFORM_IPPORT       m_heartbeat_server; // configuration of heart beat server
    VSNETPLATFORM_IPPORT       m_alarm_server;     // configuration of alarm server
    VSNETPLATFORM_IPPORT       m_store_server;     // configuration of store server
} VSNETPLATFORM_SERVERINFO;

#define VSNET_MAX_PLATFORM_IPPAIR   4
/*************************platform link configuration********************************/
typedef struct
{
    unsigned  int                m_platformtype;               // platform type
    char                         m_deviceID[32];               // device ID
    unsigned short               m_maxconnet;                  // max connection number，computing with 5 per channel
    unsigned short               m_linktype;                   // link type,1:LAN,2:ADSL,3:others
    unsigned short               m_msgport;                    // message port
    unsigned short               m_videoport;                  // video port
    unsigned short               m_audioport;                  // audio port
    unsigned short               m_talkport;                   // talk port
    unsigned short               m_updateport;                 // update port
    short                              m_reserved;             // reserved for alignment
    VSNETPLATFORM_SERVERINFO           m_platform_serverinfo;  // information of platform server
    VSNETPLATFORM_REGISTER_STRATEGY    m_platform_reg;         // information of platform register
    VSNETPLATFORM_HEARTBEAT_STRATEGY   m_platform_heatbeat;    // information of platform heart beat
} VSNETPLATFORM_INFO;
/*********************************************************/

typedef struct
{
    unsigned char                m_enable_vo;      // enable image circle or not
    unsigned char                m_enable_vo_vga;
    unsigned char                m_vo_vga_res;
    unsigned char                m_reserved[1];    // reserved for alignment
} VSNETVOCFG;

typedef struct
{
    unsigned char                m_upload_record;  // upload record
    unsigned char                m_upload_capjpeg; // upload captured image
    unsigned char                m_reserved[2];    // reserved for alignment
} VSNETFTPUPLOAD;

typedef struct
{
    char    m_ccCode[32];                          // manufacturer code
    char    m_devID[32];                           // device ID
    char    m_devType[32];                         // device type
    char    m_reserved[256];                       // reserved for extension
} VSNETDEVINFO_HH;

typedef struct
{
    int     m_run_seconds;                         // run time,second as UNIT
} VSNETRUNTIME_HH;

typedef struct
{
    VSNETFTPUPLOAD               m_ftp_upload[16]; // channel FTP upload parameter
} VSNETALARMFTPUPLOAD;

typedef struct
{
    unsigned char     m_alarm_rec[16];             // alarm relate to recording
    unsigned char     m_alarm_capjpeg[16];         // alarm relate to  JPEG capture
    unsigned char     m_ftp_alarmrec[16];          // uploading recording of alarm relating to FTP
    unsigned char     m_ftp_alarmcapjpeg[16];      // uploading capturing image of alarm relating to FTP
} VSNETALARMCONTACTEX;

typedef struct
{
    unsigned char     m_bEnable;                   // preset point plan enable
    unsigned char     m_reserved[1];               // reserved for alignment
    unsigned char     m_start_month;               // start month
    unsigned char     m_start_day;                 // start day
    unsigned char     m_start_hour;                // start hour
    unsigned char     m_start_min;                 // start minute
    unsigned char     m_stop_month;                // end month
    unsigned char     m_stop_day;                  // end day
    unsigned char     m_stop_hour;                 // end hour
    unsigned char     m_stop_min;                  // end minute
    unsigned char     m_point_intime;              // preset point number to be called in time
    unsigned char     m_point_outtime;             // preset point number to be called out of time
} VSNETAUTOPOINT_ITEM;

typedef struct
{
    unsigned char           m_bEnable;             // preset point plan enable
    unsigned char           m_interval;            // interval time reset automatically，minute as UNIT
    unsigned char           m_reserved[2];         // reserved for alignment
    VSNETAUTOPOINT_ITEM     m_scheme[4];           // 4 scheme of call preset point
} VSNETAUTOGOTOPOINT;

typedef struct
{
    unsigned char           m_enable;                  // temperature and humidity sensor enable
    unsigned char           m_addrtem;                 // address of temperature and humidity server
    unsigned char           m_osd_temperature;         // OSD temperature enable
    unsigned char           m_osd_humidity;            // OSD humidity enable
    int                     m_alarmtime;               // interval time for alarm
    TIMELIST                m_timelist;                // time for defense
    unsigned char           m_temperature_alarmout[16];// output of temperature alarm relating,previous 4 outputs are alarm outputs ,then 11 GPIO
    unsigned char           m_humidity_alarmout[16];   // output of humidity alarm relating,previous 4 outputs are alarm outputs ,then 11 GPIO
    float                   m_max_temperature;         // max temperature
    float                   m_min_temperature;         // min temperature
    float                   m_max_humidity;            // max humidity
    float                   m_min_humidity;            // min humidity
} VSNETTEMHUMYWCNCEN;

typedef struct
{
    unsigned char    m_enable;               // voltage sensor enable
    unsigned char    m_addrtem;              // voltage sensor address
    unsigned char    m_osd_voltage;          // OSD voltage enable
    unsigned char    m_reserved;             // reserved for alignment
    int              m_alarmtime;            // interval time for alarm
    TIMELIST         m_timelist;             // time for defence
    unsigned char    m_voltage_alarmout[16]; // output of humidity alarm relating,previous 4 outputs are alarm outputs ,then 11 GPIO
    float            m_rating;               // voltage rating ,V as UNIT
    float            m_alarm_voltage;        // alarm voltage，V as UNIT
} VSNETVYWCNCEN;

typedef struct
{
    unsigned char    m_bEnable[3];              // enable relate to dangerous temperature and humidity
    unsigned char    m_reserved;                // reserved
    unsigned char    m_bEnable_maxdan_tem[4];   // max dangerous temperature enable
    float            m_maxdan_temperature[3];   // max dangerous temperature
    unsigned char    m_bEnable_mindan_tem[4];   // min dangerous temperature enable
    float            m_mindan_temperature[3];   // min dangerous temperature
    unsigned char    m_bEnable_maxdan_hum[4];   // max dangerous humidity enable
    float            m_maxdan_humidity[3];      // max dangerous humidity
    unsigned char    m_bEnable_mindan_hum[4];   // min dangerous humidity enable
    float            m_mindan_humidity[3];      // min dangerous humidity
    unsigned int     m_checktimersecond;        // interval time for checking
} VSNETDANTEMHUMYWCNCEN;

typedef struct
{
    int                     m_alarmmode;        // alarm mode0：OFF,1：ON
    int                     m_singlehost;       // label of center for uploading alarm signal
    unsigned char           m_record[16];       // relating to recording
    unsigned char           m_out[15];          // relating to output
    unsigned char           m_osd_enable;       // OSD Enable
    unsigned char           m_enpreno[16];      // relating to preset point
    unsigned char           m_preno[16];        // perset point number to be called
    unsigned char           m_capjpeg[16];      // relating to capture
    TIMELIST                m_timelist;         // check time table
} ALARMRECORDEXYWCNCEN;

typedef struct
{
    int                     m_benab;            // alarm enable
    ALARMRECORDEXYWCNCEN    m_alarmrec[8];      // parameter of relating alarm
} ALARMPARAMEXYWCNCEN;

typedef struct
{
    float                   m_rate_voltage;     // rating voltage
    float                   m_cur_voltage;      // current voltage
} VSNETVALARMYWCNCEN;

typedef struct
{
    short                   m_temhumdata;       // temperature and humidity
    short                   m_alarmtype;        // alarm type 0:no alarm,1:higher than upper limit,2:lower than upper limit
} VSNETTEMHUMALARMYWCNCEN;

// caution line
typedef struct
{
    int     m_sgline_flg;        // single line alarm enable
    char    m_sgline_name[64];   // alarm rule name for single line
    int     m_sgline_drc;        // single line direction 0:Left1:Right 2:bidirectional
    int     m_sgline_beg_x;      // alarm coordinate of single line , top left
    int     m_sgline_beg_y;      // alarm coordinate of single line , top left
    int     m_sgline_end_x;      // alarm coordinate of single line , down right
    int     m_sgline_end_y;      // alarm coordinate of single line , down right
} VSNETVASINGLELINERULE;

typedef struct
{
    int                    m_multiline_flg;      // broken line alarm
    char                   m_multiline_name[64]; // rule name of broken line alarm
    int                    m_lineCount;          // single line count of broken line
    VSNETVASINGLELINERULE  m_multiline[10];      // rule of broken line
} VSNETVAMULTILINERULE;

typedef struct
{
    int     m_dbline_flg;        // double line alarm enable
    char    m_dbline_name[64];   // rule name of double line alarm
    int     m_dbline_drc;        // double line direction 0:left 1:right
    int     m_dbline0_beg_x;     // alarm coordinate of line one , top left
    int     m_dbline0_beg_y;     // alarm coordinate of line one , top left
    int     m_dbline0_end_x;     // alarm coordinate of line one , down right
    int     m_dbline0_end_y;     // alarm coordinate of line one , down right
    int     m_dbline1_beg_x;     // alarm coordinate of line two , top left
    int     m_dbline1_beg_y;     // alarm coordinate of line two , top left
    int     m_dbline1_end_x;     // alarm coordinate of line two , down right
    int     m_dbline1_end_y;     // alarm coordinate of line two , down right
} VSNETVADOUBLELINERULE;

// region alarm(enter,leave,hover) public segment
typedef struct
{
    int                m_region_valid;                  // valid label of region alarm
    int                m_region_flg;                    // region alarm enable
    char               m_region_name[64];               // rule name of region alarm
    unsigned int       m_packed_msk[352 * 288 / 8 / 4]; // region mask---stored as bit (0-->>0) (1-->>255)
} VSNETVAREGIONERULE;

typedef struct
{
    int                m_frm_bkgd;        // frame number when background initialization finished
    int                m_frm_pixels;      // average value of background and covariance period for updating---per m_frm_pixels frame updating once
    int                m_frm_object;      // time for static object becomes background ---update moving point for a long time  to background
} VSNETVABACKGROUND;

typedef struct
{
    int                m_obj_min_area;    // min area of moving object
    int                m_obj_max_area;    // max area of moving object
} VSNETVAOBJECTAREA;

typedef struct
{
    int                m_alm_window_enable;     // object rectangle enable
    int                m_alm_dotline_enable;    // point of object track enable
    int                m_stabilization_enable;  // image stabilization enable
    int                m_region_shield_enable;  // region shield enable
    unsigned int       m_region_shield_msk[352 * 288 / 8 / 4]; // mask code of region shield after packing---store with bit (0-->>0) (1-->>255)
} VSNETVAALARMSETTING;

typedef struct
{
    int                m_enable;    // 0:disable; 1:enable local preview
    int                m_scale;     // 0:1 picture; 1:4 pictures; 2:9 pictures; 3:16 pictures
    unsigned char      m_vichn[16]; // select input channels for 16 output channels
    // nvr: index is vochn  value is decch
} VSNETVIEWPARAM;

typedef struct
{
    VSNETVIEWPARAM     m_viewparam[VSNET_DVR_MAXVONUM];
} VSNETVIEWCFG;

typedef struct
{
    int                m_time;             // round look time
    int                m_scale;            // 0:1 picture; 1:4 pictures; 2:9 pictures; 3:16 pictures
    unsigned char      m_vichn[16];        // select input channels for 16 output channels
} VSNETLOOPPARAM;

typedef struct
{
    int                m_enable;           // 0:disable; 1:enable round look
    int                m_loopnum;          // group number of round look
    VSNETLOOPPARAM     m_loopparam[20];    // parameter of round look
} VSNETLOOPVIEW;

typedef struct
{
    VSNETLOOPVIEW      m_loopview[VSNET_DVR_MAXVONUM];
} VSNETLOOPCFG;

typedef struct
{
    int                m_enabled;          // disk support smartctl or not,1:enable，0:disable
    int                m_health;           // disk check,1:passed, 0:failed.
    int                m_temperature;      // disk temperature
    int                m_poweronhours;     // disk power-on time
    int                m_uncorrectalbe;    // percentage of error sector scanned off line
} VSNETSMARTPARAM;

typedef struct
{
    int                m_disknum;          // total of disk
    VSNETSMARTPARAM    m_smartinfo[8];     // smart information of 8 disks
} VSNETSMARTINFO;

typedef struct
{
    int                m_enable;           // nfs 1：enable， 0：disable
    char               m_deviceip[40];     // device IP address
    char               m_devicedir[40];    // device path
} VSNETMNTNFS;

typedef struct
{
    int                m_enable;                        // enable
    int                m_singlehost;                    // label of center for uploading alarm signal
    int                m_singlebuzzer;                  // buzzer trigger
    int                m_magnify[VSNET_DVR_MAXVONUM];   // relating amplification
    int                m_reseved1;                      // reserved
    unsigned char      m_record[VSNET_DVR_MAXCH];       // relate to recording
    unsigned char      m_capjpeg[VSNET_DVR_MAXCH];      // relate to capture
    unsigned char      m_ftprecord[VSNET_DVR_MAXCH];    // ftp upload recording
    unsigned char      m_ftpcapjpeg[VSNET_DVR_MAXCH];   // ftp upload alarm capturing
    unsigned char      m_out[VSNET_DVR_MAXALARMOUT];    // relate to output
    unsigned char      m_enable_ptz[VSNET_DVR_MAXCH];   // enable relate to cloud platform
    unsigned char      m_enable_type[VSNET_DVR_MAXCH];  // ability type，1:call preset point，2:call track, 3:ONLY preset point,4:ONLY track
    unsigned char      m_preno[VSNET_DVR_MAXCH];        // call preset point
    unsigned char      m_pretrackno[VSNET_DVR_MAXCH];   // relating track number
    TIMELIST           m_timelist;                      // check time table
} VSNETDVRALARMPARAM;

typedef struct
{
    VSNETDVRALARMPARAM  m_alarmparam;
    unsigned char       m_alarmsenstive;    // sensitivity of moving detection
    unsigned char       m_reseved1;         // reserved
    unsigned char       m_reseved2;
    unsigned char       m_reseved3;
    unsigned char       m_detect[18][22];   // range to be detected
} VSNETDVRMOTIONALARM;                      // parameter of moving detection

typedef struct
{
    VSNETDVRALARMPARAM  m_alarmparam;
    int                 m_alarmmode;        // alarm mode0：OFF,1：ON
    int                 m_senseorinterval;  // interval time of sensor alarm
    char                m_alarmname[32];    // sensor name
} VSNETDVRSENSORALARM;                      // parameter of sensor alarm

typedef struct
{
    VSNETDVRALARMPARAM  m_alarmparam;
    int                 m_decval;           // detection sensitivity(1(lowest)---5(highest))
} VSNETDVRHIDEALARM;                        // hide alarm

typedef struct
{
    VSNETDVRALARMPARAM  m_alarmparam;
} VSNETDVRVILOSTALARM;                      // video lost

typedef struct
{
    VSNETDVRALARMPARAM  m_diskfull;         // disk full
    VSNETDVRALARMPARAM  m_diskerror;        // disk error
    VSNETDVRALARMPARAM  m_accessviolation;  // violative access
    VSNETDVRALARMPARAM  m_nodisk;           // no disk
    VSNETDVRALARMPARAM  m_reseved1;         // reserved
    VSNETDVRALARMPARAM  m_reseved2;         // reserved
    VSNETDVRALARMPARAM  m_reseved3;         // reserved
    VSNETDVRALARMPARAM  m_reseved4;         // reserved
} VSNETDVROTHERALARM;                       // other parameter

typedef struct
{
    char                m_deviceID[16];     // device ID
    char                m_active;           // status, 1 : active
    char                m_reserved[3];
} VSNETREMOTECTRL;

typedef struct
{
    unsigned int      m_right;                // each bit represents a kind of right,from low to high,it is
    // local playback,long-distance playback,long-distance preview,talk,local cloud platform control,
    // long-distance cloud platform control、recording by hand、recording file backup、log management、
    // system parameter setting,upgrading system,close device,management of user account number
    unsigned int      m_right1;               // reserved
    unsigned int      m_netview;              // 0~31 channel of long-distance preview
    unsigned int      m_netview1;             // 32~63 channel of long-distance preview
    unsigned int      m_netplay;              // 0~31 channel of long-distance preview
    unsigned int      m_netplay1;             // 32~63 channel of local playback
    unsigned int      m_localplay;            // 0~31 channel of local playback
    unsigned int      m_localplay1;           // 32~63 channel of local playback
} VSNETDVRUSERRIGHT;

typedef struct
{
    VSNETDVRUSERRIGHT m_right;
    char              m_groupname[20];
} VSNETDVRUSERGROUP;

typedef struct
{
    char              m_username[20];
    char              m_password[20];
    int               m_groupnum;            // right group of user
    VSNETDVRUSERRIGHT m_right;
} VSNETDVRUSER;

typedef struct
{
    int               m_usernum;             // user number
    int               m_groupnum;            // number of valid groups
    VSNETDVRUSERGROUP m_group[8];            // 8 groups，initialize 4 groups ,reserve 4 groups
    VSNETDVRUSER      m_user[32];            // 32 users，initialize 4 users
} VSNETDVRSYSUSER;

// extent user info(add the stauts of user)
typedef struct
{
    char              m_status[32];          // the status of user(0:disbale, 1:enable)
    VSNETDVRSYSUSER   m_dvrsysuser;
} VSNETDVRSYSUSEREX;

typedef struct
{
    int               bShow;             // show(1),hide(0)
    char              lpString[32];      // characters to be showed
    unsigned short    x, y;              // coordinate
} TITALOSD_DVR;

typedef struct
{
    TIMEOSD           m_time;            // time
    TITALOSD_DVR      m_tital[4];        // title
    BITSINFOOSD       m_bits;            // bit rate
} CHANNOSD_DVR;

typedef struct
{
    int               m_doublebits;      // 1:double bit stream，0:single bit stream
} VSNETDVRDOUBLEBITS;

typedef struct
{
    char              m_name[9][32];     // name of switching button
} VSNETSWITCHSCREENNAME;

typedef struct
{
    char              m_servername[60];   // server name
    char              m_serverip[16];     // IP address
    char              m_servermask[16];   // network mask
    char              m_gatewayAddr[16];  // gateway
    unsigned short    m_serport;          // data port
    char              m_hostserverurl[40];// address of long-distance server
    unsigned short    m_hostserverport;   // port of long-distance server
    char              m_pppoename[64];    // PPPOE username
    char              m_pppoepswd[64];    // PPPOE password
} VSNETBASENETPARAM;

typedef struct
{
    int             m_ftp_timerec;     // regular FTP uploading capture
    TIMELIST        m_timeList;        // capture time
} VSNETFTPTIMEREC;

typedef struct
{
    int             m_xoffset;         // horizontal offset 0~32
    int             m_yoffset;         // vertical offset 0~32
} VSNETVIDEOOFFSETEX;

typedef struct
{
    int             m_state;             // 3G state
    char            m_ip[32];            // CenterIP
    char            m_subnetmask[32];    // subnet mask for WCDMA dialing
    char            m_gateway[32];       // gateway for WCDMA dialing
    char            m_wcdma_dns_pri[32]; // primary DNS for WCDMA dialing
    char            m_wcdma_dns_alt[32]; // Secondary DNS for WCDMA dialing
} VSNET3GSTATE;

typedef struct
{
    int             m_3gmode;            // connection parameter      -------->0-OFF 1-CDMA 2-evdo(3g cdma2000) 4-wcdma 8-tdcdma
    unsigned char   m_cdmaname[64];      // reserve cdma username(dv-do username)
    unsigned char   m_cdmapwd[64];       // reserve cdma password(dv-do password)
    char            m_cdmadial[16];      // reserve cdma dialing number(dv-do dialing number)
} VSNET3GPARAM;

typedef struct
{
    int              m_3gmode;           // connection parameter      -------->0-OFF 1-CDMA 2-evdo(3g cdma2000) 4-wcdma 8-tdcdma
    unsigned char    m_cdmaname[64];     // reserve cdma username(dv-do username)
    unsigned char    m_cdmapwd[64];      // reserve cdma password(dv-do password)
    char             m_cdmadial[16];     // reserve cdma dialing number(dv-do dialing number)
    unsigned char    m_apnname[64];      // point name
    int              m_keepalive;        // keep connection alive
    int              m_lcpechointerval;
    int              m_lcpechofailure;
    int              m_maxreceiveunit;
    int              m_maxtransferunit;
    int              m_networkconnectionmode;    // online,dial for need,dial by hand
    int              m_networkregistertype;      // AUTO,2G,3G
    int              m_radiobankset;             // band setting,represented by bit
    int              m_bandsaving;               // reserved band
    int              m_getdnsfromoperator;
    char             m_dns_pri[32];              // Primary DNS
    char             m_dns_alt[32];              // Secondary DNS
    int              m_authtype;
    unsigned char    m_expertoptions[128];       // extension parameter
} VSNET3GPARAMEX;

typedef struct
{
    int              m_pinprotection;            // pin protection
    unsigned char    m_pincode[64];              // pin code
} VSNETPININFO;

typedef struct
{
    int              m_simstatus;                // sim status
    unsigned char    m_simnumber[64];            // sim number
    char             m_operator[20];             // provider
    int              m_cur_cdma_network;         // current network
    int              m_sig_percent;              // signal strength
    char             m_imei[20];                 // IMEI
} VSNETSIMINFO;

typedef struct
{
    int              m_smsdays;                  // message days
    int              m_sms_sendip;               // message sends IP address or not
    unsigned char    m_smscellphone[64];         // number for receiving message
    unsigned char    m_smscenternumber[64];      // number of message center
    unsigned char    m_smscontent[128];          // content of message
} VSNETSMSINFO;

typedef struct
{
    int              m_logsize;                  // log size
    unsigned char *  m_logstring;                // log content
    unsigned char    m_logfilename[64];          // log name
} VSNETWCDMALOG;

typedef struct
{
    int              m_check3gstatus;            // check 3g status
    unsigned char    m_pingipaddr[64];           // ping IP Address
    int              m_pingipinterval;           // interval  of ping ip
    int              m_pingbytes;                // data size for ping
    int              m_pingfailcnt;              // redial number when ping failure
} VSNETCHECK3GSTATUS;

typedef struct
{
    int              m_nPhoneDisconnauto;         // 短信电话激活无网络连接时自动断线：1：勾选；无传值：未勾选
    int              m_nPhoneDisconntimer;        // 短信、电话激活上线无连接时等待时间 
    int              m_nChkPhone;                 // 仅接受来自以下号码的短信或来电：1：勾选；无传值：未勾选
    int              m_nManualDisconnauto;        // 手动控制无网络连接时自动断线：1：勾选；无传值：未勾选         
    int              m_nManualDisconntimer;       // 手动控制无连接等待时间 
    int              m_nCuroption;                // 0：设置参数; 1：立即上线；2：立即断开
    char             m_strPhonenumber[10][20];    // 可接受电话号码 
    int              m_nAlarmDisconnenAble;       // 报警模式下设置一定时间断开连接使能
    int              m_nAlarmDisconnTmSet;        // 报警模式下设置断开连接时间
    TIMELIST         m_timeList;                  // 定时时间表
} VSNET_CHECK3GCONNPARAM;

typedef struct
{
    int                    m_nPhoneorSmsDisconnect; // 短信电话下线
    VSNET_CHECK3GCONNPARAM m_connParam;
} VSNET_CHECK3GCONNNET;

typedef struct
{
    int  m_nResult;          // 3G短信测试返回结果
    char m_szTestMsg[256];   // 3G短信测试返回结果字符串
} VSNET_3GSMSTESTRES;

typedef struct
{ 
    int             m_nLogSize;  // 拨号日志文件大小，以Byte为单位
} VSNET_3GDIALUPLOG;

typedef struct
{
    char             m_nSisNetState[120];  // 网络连接状态
    VSNETSIMINFO     m_sistate;
} VSNET_3GSIMINFONET;

typedef struct
{
    int     m_linkenable;       // link enable
    int     m_listenport;       // monitor port
} VSNETRTSPPARAM;

// RTSP扩展参数
typedef struct
{
    int     m_linkenable;       // RTSP使能，0-关闭，1-启动
    int     m_listenport;       // RTSP监听端口
    int     m_udpstartport;     // RTSP的UDP起始端口
} VSNETRTSPPARAMEX;

// VLC参数
typedef struct
{
    int     m_linkenable;       // VLC使能，0-关闭，1-启动
    char    m_dstaddr[16];      // VLC目标IP地址，只支持IP地址
    int     m_dstport;          // VLC目标端口
    int     m_streamselect;     // VLC流类型，0-PS流，1-TS流
} VSNETVLCPARAM;

// VLC扩展参数
typedef struct
{
    int     m_linkenable;       // VLC使能，0-关闭，1-启动
    char    m_dstaddr[64];      // VLC目标地址，支持IP地址和域名
    int     m_dstport;          // VLC目标端口
    int     m_streamselect;     // VLC流类型，0-PS流，1-TS流
} VSNETVLCPARAMEX;

// NTP参数
typedef struct
{
    int     m_linkenable;       // NTP使能，0-关闭，1-启动
    char    m_serveraddr[16];   // NTP服务器地址，只支持IP地址
    int     m_serverport;       // NTP服务端口
    int     m_timezone;         // 时区，取值范围-12 ~ 12
} VSNETNTPPARAM;

// NTP扩展参数
typedef struct
{
    int     m_linkenable;       // NTP使能，0-关闭，1-启动
    char    m_serveraddr[64];   // NTP服务器地址，支持IP地址和域名
    int     m_serverport;       // NTP服务端口
    int     m_timezone;         // 时区，取值范围-12 ~ 12
} VSNETNTPPARAMEX;

typedef struct
{
    VSNETRTSPPARAM m_rtsp;      // RTSP parameter
    VSNETVLCPARAM  m_vlc;       // VLC  parameter
    VSNETNTPPARAM  m_ntp;       // NTP  parameter
} VSNETPROTOCOLPARAM;

typedef struct
{
    int             m_encodetype;  // encoding type, 0:H.264, 1:MJPEG
} VSNETENCODETYPE;

typedef struct
{
    int             m_encodeprofile;  // encoding profile, 0:baseline, 1:MP, 2:HP
} VSNETENCODEPROFILE;

typedef struct
{
    short       m_audiotag;     // audio tag
    short       m_auchs;        // audio channel number
    short       m_aurates;      // audio bit rate
    short       m_reserved;     // reserved
    int         m_ausample;     // audio sampling frequency
} VSNETTALKBOTHWAY;

typedef struct
{
    int         m_locktime;
} VSNETPTZCTRLTIME;

#define MAX_MEM_NUM 10
#define MAX_PRO_NUM 10
typedef struct
{
    int                      m_prono;            // preset point
    int                      m_time;             // time
    char                     m_proname[32];      // name of preset point
} APP_CRUISE_PRO;
typedef struct
{
    int                     m_enable;            // enable
    TIMECHECK               m_cruise_time;       // time
    char                    m_cruise_name[32];   // cruise name
    int                     m_line_enable;       // enable superposition of cruise route
    int                     m_dot_enable;        // enable superposition of cruise point
    int                     m_superpose_title;   // superposition to title, value as:0，1，2，3 represent superposition to 1,2,3,4 title
    APP_CRUISE_PRO          m_cruise_pro[MAX_PRO_NUM];
} APP_CRUISE_MEMBER;

typedef struct
{
    int                 m_enable;
    APP_CRUISE_MEMBER   m_cruise_member[MAX_MEM_NUM];
} VSNETPROLOOPPARAM;

typedef struct
{
    char  reccordName[60];     // record name
} VSNETSERVERRECPARAM;         // record command parameter of long-distance server

typedef struct
{
    int m_audio_bestir;
} NETFILE_REMOTEAUDIOBESTIR;

typedef struct
{
    int videoin;
} VSNETVIDEOENCMAPINFO;

typedef struct
{
    char    m_value[8];        // show evidence
} VSNETATTESTEXHIBIT;

typedef struct
{
    char m_actors[7];          // character array，from 0 to 5 represents 6  roles's channels，0 justice...，6 is end symbol
} VSNETACTORSINFO;

typedef struct
{
    int                 m_enable;               // enable
    unsigned int        m_brecord[2];           // relate to recording
    unsigned int        m_capjpeg[2];           // relate to jpeg capture
    unsigned int        m_ftp_alarmrec[2];      // uploading recording of alarm relating to FTP
    unsigned int        m_ftp_alarmcapjpeg[2];  // uploading alarm capture to FTP
    unsigned int        m_outputmap[4];         // relate to output
    TIMELIST            m_timelist;             // detection time table
} VSNETSMALARMLINK;                             // alarm relating of intellective analysis

// 智能分析扩展报警联动参数(最多支持16个视频通道)
typedef struct 
{
	int					m_enable;               // enable
	unsigned int		m_brecord[16];          // relate to recording
	unsigned int		m_capjpeg[16];          // relate to jpeg capture
	unsigned int		m_ftp_alarmrec[16];     // uploading recording of alarm relating to FTP  
    unsigned int		m_ftp_alarmcapjpeg[16]; // uploading alarm capture to FTP
	unsigned int		m_outputmap[16];        // relate to output
	TIMELIST			m_timelist;             // detection time table
} VSNETSMALARMLINK_EX;

// 智能分析OSD叠加参数
typedef struct
{
    int                 m_enable;               // enable
    unsigned short      m_x, m_y;               // coordinate
    char                m_text[32];             // content to be showed of OSD superposition
} VSNETSMOSD;

// 智能分析区域入侵\离开规则
typedef struct
{
    int                 m_enable;                   // enable rule
    int                 m_enableosd;                // OSD enable
    int                 m_enabledefregion;          // enable default region
    char                m_rulename[64];             // rule name
    unsigned int        m_packed_msk[352 * 288 / 8 / 4]; // region mask---stored with bit (0-->>0) (1-->>255)
    VSNETSMALARMLINK    m_alarmlink;                // alarm relating
    VSNETSMOSD          m_osd[2];                   // OSD superposition of intellective analysis alarm
} VSNETSMREGIONINOUT;

// 智能分析扩展区域入侵\离开规则(最多支持16个视频通道)
typedef struct
{
	int					m_enable;					// enable rule
	int					m_enableosd;				// OSD enable
	int					m_enabledefregion;			// enable default region
	char				m_rulename[64];				// rule name
	unsigned int        m_packed_msk[352 * 288 / 8 /4];	// region mask---stored with bit (0-->>0) (1-->>255)
	VSNETSMALARMLINK_EX	m_alarmlink;				// alarm relating
	VSNETSMOSD			m_osd[2];					// OSD superposition of intellective analysis alarm
} VSNETSMREGIONINOUT_EX;

// 智能分析徘徊规则
typedef struct
{
    int                 m_enable;                   // enable rule
    int                 m_enableosd;                // OSD enable
    int                 m_enabledefregion;          // enable default region
    int                 m_hovertime;                // standard time of hovering
    char                m_rulename[64];             // rule name
    unsigned int        m_packed_msk[352 * 288 / 8 / 4]; // region mask---stored with bit (0-->>0) (1-->>255)
    VSNETSMALARMLINK    m_alarmlink;                // alarm relating
    VSNETSMOSD          m_osd[2];                   // OSD superposition of intellective analysis alarm
} VSNETSMREGIONHOVER;

// 智能扩展分析徘徊规则(最多支持16个视频通道)
typedef struct
{
	int					m_enable;                   // enable rule 
	int					m_enableosd;                // OSD enable
	int					m_enabledefregion;          // enable default region
	int					m_hovertime;                // standard time of hovering
	char				m_rulename[64];             // rule name
	unsigned int        m_packed_msk[352 * 288 /8 / 4];	// region mask---stored with bit (0-->>0) (1-->>255)
	VSNETSMALARMLINK_EX	m_alarmlink;                //alarm relating
	VSNETSMOSD			m_osd[2];                   //OSD superposition of intellective analysis alarm
} VSNETSMREGIONHOVER_EX;

// 智能分析警戒线规则			
typedef struct
{
    int                 m_enable;            // rule enable
    int                 m_sgline_drc;        // single line direction    0:left 1:right 2:bidirectional
    int                 m_sgline_beg_x;      // coordinate of single line alarm,top left
    int                 m_sgline_beg_y;      // coordinate of single line alarm,top left
    int                 m_sgline_end_x;      // coordinate of single line alarm,down right
    int                 m_sgline_end_y;      // coordinate of single line alarm,down right
    char                m_rulename[64];      // rule name
    unsigned char       m_enableosd;         // OSD enable
    VSNETSMALARMLINK    m_alarmlink;         // alarm relating
    VSNETSMOSD          m_osd[2];            // OSD superposition of intellective analysis alarm
} VSNETSMALARMLINE;

// 智能分析扩展警戒线规则(最多支持16个视频通道)
typedef struct
{
	int					m_enable;            // rule enable
	int					m_sgline_drc;        // single line direction	0:left 1:right 2:bidirectional
	int					m_sgline_beg_x;      // coordinate of single line alarm,top left
	int					m_sgline_beg_y;      // coordinate of single line alarm,top left
	int					m_sgline_end_x;      // coordinate of single line alarm,down right
	int					m_sgline_end_y;      // coordinate of single line alarm,down right
	char				m_rulename[64];      // rule name
	int					m_enableosd;         // OSD enable
	VSNETSMALARMLINK_EX	m_alarmlink;         // alarm relating
	VSNETSMOSD		    m_osd[2];            // OSD superposition of intellective analysis alarm
} VSNETSMALARMLINE_EX;

#if defined(TARGET_FJTS64LINE)
#define SMART_MAXLINES 64   // max line count is 64
#else
#define SMART_MAXLINES 10   // max line count is 10
#endif

// 智能分析警戒线规则集
typedef struct
{
    int                 m_count;            // line count
    VSNETSMALARMLINE    m_lines[SMART_MAXLINES];        
} VSNETSMALARMLINES;

// 智能分析扩展警戒线规则集(最多支持16个视频通道)               
typedef struct
{
	int					m_count;			// line count
	VSNETSMALARMLINE_EX	m_lines[SMART_MAXLINES];
} VSNETSMALARMLINES_EX;



/**************************** 智能分析规则是否显示在客服端 *********************************************/
#define MAXSMPTNUM 64
typedef struct
{
#if defined(TARGET_FJTS64LINE)
    unsigned int m_nShowLineL;               //是否显示拌线规则 按位表示 目前最多64条线可以表示
    unsigned int m_nShowLineH;
#else
    int m_nShowLine;                //是否显示拌线规则 按位表示 目前最多10条线可以表示
#endif
    unsigned char m_bShowInRule;   //是否显示区域侵入规则
    unsigned char m_bShowOutRule;  //是否显示区域离开规则
    unsigned char m_bShowHoverRule;//是否显示区域侵入规则    
    unsigned char m_cReserved1;    //保留
    unsigned char m_cReserved2[4]; //保留
} VSNET_SMCLIENTSHOW;

typedef struct
{
    float m_nX;
    float m_nY;
} VSNET_POINT;

typedef struct
{    
    VSNET_POINT m_pt[MAXSMPTNUM];//区域规则点保存
    int m_nPtNum;             
} VSNET_SMRECTRULE;
/**************************  智能分析规则是否显示在客服端 ---      完 *************************************/

typedef struct
{
    unsigned int        m_alarmtime;        // alarm time
    int                 m_alarmtype;        // alarm type
    char                m_alarmdesc[56];    // alarm description
} VSNETALARMJPEGCAP;

typedef struct
{
    char m_num[8];              // pit number
    int m_bmotionjpg;           // uploading moving capture to FTP
    int m_waittime;             // wait time(second) for protecting from building bridge
    int m_front_sd;             // front value
} VSNETJNKYPARAM;

typedef struct
{
    char            m_postaddr[32]; // post address
    unsigned short  m_postport;     // post port
    unsigned short  m_reserved;     // reserved for alignment
} VSNETJNKYSERINFO;

typedef struct
{
    int                 m_cruise_day;               // cruise date
    TIMELIST            m_cruise_time;              // cruise time
    int                 m_line_enable;              // enable superposition of cruise route
    char                m_osdstring[32];            // display character of OSD superposition
    int                 m_superpose_title;          // superposition to title, value as:0，1，2，3 represent superposition to 1,2,3,4 title
    int                 m_staytime;                 // stay time of cruise point，3-255 seconds
    int                 m_cruise_pre[MAX_PRO_NUM];  // preset point number
} VSNETPROLOOPPARAMEX;                              // server cruise parameter,no relationship with previous version VSNETPROLOOPPARAM

#define   HNDX_STROSD_NUM   8
#define   HNDX_STROSD_LEN   40

typedef struct
{
    int      m_format;  // not show，2003-09-12 15:23:33，2003-09-12 FRI 15:23:33，09-12-2003 15:23:33，09-12-2003 FRI 15:23:33，12-09-2003 15:23:33
    int      m_x;       // x position，8-704 (multiple of 4)
    int      m_y;       // y position，10-576
} VSNETHNDXTIMEOSD;     // time OSD

typedef struct
{
    int      m_isshow;  // show bit rate or not
    int      m_x;       // x position，8-704 (multiple of 4)
    int      m_y;       // y position，10-576
} VSNETHNDXBITRATEOSD;  // bit rate OSD

typedef struct
{
    int      m_x;                       // x position，8-704 (multiple of 4)
    int      m_y;                       // y position，10-576
    int      m_color;                   // color 0:red 1:green 2:blue 3:white
    char     m_str[HNDX_STROSD_LEN];    // String
} VSNETHNDXOSDSTRING;                   // String OSD

// OSD parameter
typedef struct
{
    VSNETHNDXBITRATEOSD m_bitrateosd;
    VSNETHNDXTIMEOSD    m_timeosd;
    VSNETHNDXOSDSTRING  m_stringosd[HNDX_STROSD_NUM];
} VSNETHNDXOSDATTR;

// AAC parameter
typedef struct
{
    int       m_enableaac;   // AAC enable
    int       m_enSmpRate;   // AAC sampling frequency
    int       m_BitWidth;    // AAC bit width
    int       m_BandWidth;   // AAC band width
} VSNETAUDIOAAC;

typedef struct
{
    int       m_color_set;   // 0: automatic, 1: by hand
} VSNETHQCOLOR;              // set mode

typedef struct
{
    int     m_smart_sense;   // intellective analysis of sensitivity
} VSNETSMARTSENSE;

typedef struct
{
    int     m_smart_tracktype;    // intellective analysis of object type：0 all，1 human 2 vehicle
} VSNETSMARTTRACKTYPE;

typedef struct
{
    char    m_serurl[16];         // server IP address
    int     m_serport;            // server port
    char    m_stationID[10];      // station number
    char    m_reserve[2];         // reserved for alignment
    char    m_clienturl[16];      // additional client ip
    int     m_clientport;         // client port
    char    m_direction[4];       // station direction
} VSNETCDRHPARAM;

typedef struct
{
    int   m_enable;               // ON or OFF
    int   m_interval;             // check interval
    float m_flowlow;              // flow lower limit
    float m_flowhigh;             // flow upper limit
    float m_phlow;                // ph lower limit
    float m_phhigh;               // ph upper limit
    float m_Densitylow[12];       // lower limit of 12 kinds of pollution
    float m_Densityhigh[12];      // upper limit of 12 kinds of pollution
    char  m_Alarmflow[4];         // alarm ,flow relating to
    char  m_Alarmph[4];           // alarm ,PH relating to
    char  m_AlarmDensity[12][4];  // alarm ,pollution relating to
} VSNETYWCODPARAM;

typedef struct
{
    char   DeviceState[8];        // 8 devices
    float  m_flow;                // flow speed
    float  m_ph;                  // PH
    float  m_Density[12];         // lower limit of 12 kinds of pollution
} VSNETYWCODDATA;

typedef struct
{
    char m_cmd[52];               // command of sampling machine for water quality
} VSNETYWSZCYQCODE;

typedef struct
{
    char m_code[12];              // command of electromagnetism flowmeter
} VSNETYWFLOWMETERCODE;

typedef struct
{
    int     m_region_flg;         // region flag of analysis for black degree,judge region or not
    int     m_region_left;        // region coordinate of analysis for black degree,top left
    int     m_region_top;         // region coordinate of analysis for black degree,top left
    int     m_region_right;       // region coordinate of analysis for black degree,down right
    int     m_region_bottom;      // region coordinate of analysis for black degree,down right
} VSNET_BLACK_REGION;

typedef struct
{
    VSNET_BLACK_REGION  m_region[10];   // region of analysis for black degree
} VSNET_BLACK_REGIONTARGET;

typedef struct
{
    VSNET_BLACK_REGION  m_region;       // reference region for  analysis for black degree
} VSNET_BLACK_REGIONREFER;

typedef struct
{
    float m_min_val;                    // lowest value of black degree
    float m_max_val;                    // highest value of black degree
} VSNET_BLACK_ALARMRANGE;

typedef struct
{
    VSNET_BLACK_REGION  m_region;       // region of analysis for black degree
    float               m_value;        // value of black degree
} VSNET_BLACK_VALUE;

typedef struct
{
    VSNET_BLACK_VALUE   m_report[10];   // report value of analysis for black degree
} VSNET_BLACK_REPORT;

typedef struct
{
    int     m_state;         // state: open,close
    int     m_limittime;     // system limit times(hour)
    int     m_runedtime;     // device runned times(hour，only get valid)
} VSNETYWDEVRUNTIME;

typedef struct
{
    unsigned char   m_islock;          // is lock the disk
    unsigned char   m_reserved[3];     // reservation
} VSNETBUFFLOCK;

typedef struct
{
    unsigned short  m_serport;
    unsigned short  m_webport;
    unsigned short  m_rtspport;
    unsigned short  m_p2pport;
} VSNETCDHCPORT;

typedef struct
{
    unsigned int    m_sigpercent;       // signal power
    unsigned int    m_rx_byte;          // accepted flow
    unsigned int    m_tx_byte;          // sent flow
} VSNETSIGAND3GFLOW;

typedef struct
{
    float           m_gpsvx;
    float           m_gpsvy;
    int             m_gpsvsout;
    float           m_gpsvspeed;
    float           m_gpsvheight;
    char            m_gpsvtime[32];
} VSNETHZFLGPSINFO;

// Sensor坏点检测
typedef struct
{
	unsigned short  badpixelcount;      // 设备返回实际检测的坏点数，设置时不关心
	unsigned short  badpixelcountmin;   // 用户设置检测的最小坏点数
	unsigned short  badpixelcountmax;   // 用户设置检测的最大坏点数
    unsigned short  reserved;           // 保留
} VSNET_BADPIXEL_RET;

// GZJieWei Parameter = VSNETGZJWUIDHEADER + Data
typedef struct
{
    int   m_Mask1;       // MSGHEAD_MASK1: 0X280028
    int   m_Vision;      // message version: current = 1
    int   m_MgsType;     // message type: m_MgsType = 8079
    int   m_ErrorCode;   // message status code
    int   m_DataSize;    // message data size: max size = 32 bytes
    short m_Ex1;         // extension 1
    short m_Ex2;         // extension 2
    int   m_Mask2;       // MSGHEAD_MASK2: 0X280028
} VSNETGZJWUIDHEADER;

// GZJieWei Parameter
typedef struct
{
    VSNETGZJWUIDHEADER m_MsgHead;
    char               m_MsgData[32];
} VSNETGZJWUIDPARAM;

// beijing huatuo
typedef struct
{
    int     m_bEnable;  // enable watcher, 1 enable, 0 disable
    int     m_iTime;    // check time > 0
    int     m_iPreset;  // preset number
} VSNETBJHTPTZWATCHER;

#define VSNET_MAXRESOLUTIONNUM    100    // 支持的最大视频分辨率个数

// DVR的VO接口定义
typedef enum
{
    ENUM_VSNET_VO_VGA = 0,     // VO的VGA接口
    ENUM_VSNET_VO_HDMI = 1     // VO的HDMI接口
} ENUM_VSNET_DVRVO;

// DVR的单个VO设置参数
typedef struct
{
    unsigned char  m_bSupport;                                  // 是否支持该VO接口（只能获取），0-不支持，1-支持
    unsigned char  m_supportResolution[VSNET_MAXRESOLUTIONNUM]; // 支持的分辨率（只能获取）,按照上面ENUM_VSNET_VIDEO_RESOLUTION定义的顺序,一个字节代表一个分辨率是否支持，1-支持，0-不支持
    unsigned char  m_bEnable;                                   // 是否启用该接口，只有支持该接口时才允许设置，0-不支持，1-未启用，2-启用
    unsigned char  m_indentation;                               // VO的缩放系数，只有支持该接口时才允许设置范围0~255，(暂未使用)
    unsigned char  m_resolution;                                // 当前的分辨率，只有支持该接口时才允许设置，取值按照上面ENUM_VSNET_VIDEO_RESOLUTION定义
} VSNET_DVRVOSETTS;

// DVR的VO设置参数
typedef struct
{
    VSNET_DVRVOSETTS m_vointerface[VSNET_DVR_MAXVONUM];         // DVR的VO参数设置，目前最多支持VSNET_DVR_MAXVONUM个VO接口，接口按照上面ENUM_VSNETVO的定义，0-VGA，1-HDMI
} VSNET_DVRVOPARAM;

/***********************新存储结构体*********************************/
//硬盘属性
typedef enum {
    VS_DISK_TYPE_RW,            /**< 读写盘 */
    VS_DISK_TYPE_RO,            /**< 只读盘 */
    VS_DISK_TYPE_RD,            /**< 冗余盘 */
    VS_DISK_TYPE_ALL,           /**< 所有类型 */
    VS_DISK_TYPE_BUTT,          /**< 无效值 */
}VS_DISK_TYPE_E;

typedef struct
{
    int          m_diskno[16];      /**< 硬盘号对应分组 普通盘组取值 [1-HD_MAXDISKNUM],冗余盘组取值-1*/
    int          m_chno[64];          /**< 通道号对应分组 普通盘组取值 [1-HD_MAXDISKNUM],冗余盘组取值-1*/
}VS_GROUP_CFG_T;

typedef struct
{
    int             m_type;         /**< 硬盘类型；注:请使用VS_DISK_TYPE_E里面的值*/
    int             m_lock;         /**< 硬盘是否锁定  LC_TRUE锁定 LC_FALSE未锁定  */
}VS_DISK_ATTR_T;

//硬盘配置
typedef struct
{
    VS_DISK_ATTR_T      m_disk[16];
}VS_DISK_CFG_T;

//存储盘组配置信息
typedef struct
{
    VS_GROUP_CFG_T      m_groupcfg;        /**< 分组配置  */
    VS_DISK_CFG_T       m_dattrcfg;        /**< 硬盘属性  */
}VS_STORAGE_CFG_T;

#define VS_SMART_ATTRIBUTES   30    
#define VS_SMART_ATTRNAMELEN  32    
#define VS_SMART_ATTRRAWVAL   32    

typedef struct
{
    unsigned char       m_id;
    unsigned char       m_curr;
    unsigned short      m_flags;
    unsigned char       m_worst;
    unsigned char       m_thresh;
    char                m_reserv[2];
    unsigned char       m_name[VS_SMART_ATTRNAMELEN];
    unsigned char       m_rawval[VS_SMART_ATTRRAWVAL];
}VS_SMART_ELEM_T;

//硬盘SMART信息
typedef struct
{
    int                     m_enabled;                      /**< 0不支持SMART，1支持SMART */
    unsigned int            m_version;
    VS_SMART_ELEM_T         m_smart[VS_SMART_ATTRIBUTES];
}VS_SMART_INFO_T;


// for DVR(LC78XX) ---add by buwp,2010-6-11
/**********************************  DVR GUI ADD *****************************************/
typedef struct
{
    unsigned char  m_no;                     // the number of preseting position
    char           m_name[31];               // the name   of preseting position
} VSNET_PTZ_PRESET_NO_T;

typedef struct
{
    int                    m_num;            // Now, how many preseting position we have
    VSNET_PTZ_PRESET_NO_T  m_presetno[255];
} VSNET_PTZ_PRESET_T;                        // Save PTZ preseting position

typedef struct
{
    int                 m_usernum;     // how many user we have,now
    int                 m_groupnum;    // how many valid user group we have ,now
    VSNETDVRUSERGROUP   m_group[8];    // 8 groups, initialize 4 groups  4 are reserved
    VSNETDVRUSER        m_user[32];    // 32 users, initialize 4 users
} VSNET_SYSUSER_T;

typedef struct
{
    char m_status[32];                 // the status of user(0:disbale, 1:enable)
    VSNET_SYSUSER_T m_user;
} VSNETDVRSYSUSER_EX;                  // extent user info(add the stauts of user)

typedef struct
{
    int             m_hdisk_attrib;         // disk attribute 0:default(read/write); 1:redundance ; 2:readonly
    char            m_hdisk_serialno[32];   // disk serial id
    int             m_resev[40];            // reserved
} VSNETDISKPARAM;

typedef struct
{
    int             m_disk_count;           // disk count
    VSNETDISKPARAM  m_diskparam[16];        // disk param
    int             m_disk_reduflag[64];    // disk redundance single, by channels
} VSNETRECORDDISKPARAM;

typedef struct
{
    int     m_type;                         // contorl type
    int     m_value;                        // speed or address; CVT: H
    int     m_paramfirst;                   // 3D:speed for H; CVT: V
    int     m_paramsecond;                  // 3D:speed for V; CVT: speed for H
    int     m_paramthird;                   // 3D:zoom parameter; CVT: speed for V
} VSNETDVRPTZ;

typedef struct
{
    int     m_zoom;                         // amplification factor
    int     m_X;                            // centre coordinate X
    int     m_Y;                            // centre coordinate Y
} VSNET_PTZCTRL_ZOOM;

typedef struct
{
    int              m_audio_type;             // audio type
    int              m_audio_bitrate;          // bit rate: 16K,24K,32K,40K for G726
    int              m_audio_samplerate;       // sample rate
    int              m_audio_bitwidth;         // bitwidth
} VSNETAUDIO;

typedef struct
{
    unsigned char timer;       // 0、HIDE 1、MM:SS:mm(00:00:00)2、HH:MM:SS(00:00:00)
    unsigned int  positionx;   // XPOS
    unsigned int  positiony;   // YPOS
} CHANNELTIMEROSD;

// SHTY
typedef struct
{
    int             m_audio_timerec;   // enable flag
    int             m_packtime;        // package time
    TIMELIST        m_timeList;        // capture time
} VSNETVIDEOTIMEREC;

// SHTY
typedef struct
{
    unsigned int  m_recodch[8][16];
} VSNETSHTYALARMAUDIO;

// SHTY
typedef struct
{
    unsigned int  m_capTime[8];
} VSNETSHTYALARMCAPTIME;

typedef struct
{
    int m_denoise;         // tvp5158 denoise level
    int m_reseved;         // reserved
} VSNET_TVP5158_DENOISE;

/**********************************  DVR GUI END *****************************************/


/**********************************  NVR 通道设备管理参数 *****************************************/	

//NVR 支持的分辨率类型
typedef enum
{
	VSNET_NVR_FORMAT_16F,
	VSNET_NVR_FORMAT_8F,
	VSNET_NVR_FORMAT_8H,
} VSNET_NVR_FORMAT_ENUM;

typedef struct
{
	VSNET_NVR_FORMAT_ENUM nNvrType;      //NVR 支持的分辨率类型 
} VSNET_NVR_FORMAT_TYPE;

typedef struct
{
    char            m_szSerName[24];     //server name
    char            m_szSerIp[20];       //server ip address
    unsigned short  m_nSerport;          //server port
    unsigned short  m_nChannel;          //the channel of server to be connected
    unsigned short  m_nTransType;        //the media transport type tcp/udp/multi
    unsigned short  m_nChlinkState;      //channel link state
    unsigned short  m_nImgHeight;        //image height
    unsigned short  m_nImgWidth;         //image width
    char            m_szUserName[20];    //server user name
    char            m_szPasswd[20];      //server password
} VSNET_NVR_DEVINFO;

typedef enum 
{
    NVR_FORMAT_NONE  = 0,
    NVR_FORMAT_D1    = 1,
	NVR_FORMAT_720P  = 3,
	NVR_FORMAT_1080P = 7
} VSNET_NVR_FORMAT_VALUE;

//NVR 绑定设备的信息
typedef struct
{
    int                     m_bEnable;        //channel been used ?
    VSNET_NVR_FORMAT_VALUE  m_nMaxSize;       //max video format
    VSNET_NVR_DEVINFO       m_vsDevInfo;      //channel info
} VSNET_NVR_CHINFO;


/**********************************  NVR 通道设备管理参数 完***************************************/

/********************************** 88258 STRUCT *****************************************/

#define MAX_CLIP_NUM 10
#define MAX_FAN_NUM  3                // fan number

// CARD TYPE
typedef enum
{
    CLIP_TYPE_MASTER    = 0,          // master card
    CLIP_TYPE_DEC       = 1,          // decoder card
    CLIP_TYPE_ENC_D1    = 2,          // D1
    CLIP_TYPE_ENC_720P  = 3,          // 720P
    CLIP_TYPE_ENC_1080P = 4,          // 1080
    CLIP_TYPE_ALARM     = 5,          // ALARM
    CLIP_TYPE_DEVICE    = 6,          // DEVICE
} APP_CLIP_TYPE;

typedef enum
{
    CLIP_SLOT_SUB0      = 0,          // CLIP1
    CLIP_SLOT_SUB1      = 1,          // CLIP2
    CLIP_SLOT_SUB2      = 2,          // CLIP3
    CLIP_SLOT_SUB3      = 3,          // CLIP4
    CLIP_SLOT_SUB4      = 4,          // MASTER
    CLIP_SLOT_SUB5      = 5,          // CLIP5
    CLIP_SLOT_SUB6      = 6,          // CLIP6
    CLIP_SLOT_SUB7      = 7,          // CLIP7
    CLIP_SLOT_MASTER    = 8,          // CLIP8
    CLIP_SLOT_MACHINE   = 9,          // ALARM
    CLIP_SLOT_ALARM     = 10,         // reserved
} APP_CLIP_NO;

typedef struct
{
    int        m_temperature;            // Temperature
    int        m_humidity;               // humidity
} VSNETHUMITUPARAM, *PVSNETHUMITUPARAM;  // Temperature and humidity


typedef struct
{
    unsigned int     m_speed;            // fan speed： r/s
    unsigned char     m_state;           // state
    unsigned char     m_reseved[3];      // reserved
} VSNETFANSTATE, *PVSNETFANSTATE;        // fan state


typedef struct
{
    unsigned char     m_isexist;         // Be Exist
    unsigned char     m_type;            // card type
    unsigned char     m_state;           // state
    unsigned char     m_reseved[5];      // reserved
} VSNETCLIPSTATE, *PVSNETCLIPSTATE;      // clip card state


typedef struct
{
    VSNETCLIPSTATE    m_stotstate;       // slot state:master、clip、slot、alarm
    VSNETHUMITUPARAM  m_humitu;          // Temperature and humidity of card
} VSNETBANCARDINFO, *PVSNETBANCARDINFO;


typedef struct
{
    unsigned char     m_powerstate;                 // power state
    unsigned char     m_reseved[7];                 // reserved
    VSNETFANSTATE     m_fanstate[MAX_FAN_NUM];      // fan state
    VSNETBANCARDINFO  m_bancardinfo[MAX_CLIP_NUM];  // card information
} VSNETDECVICEPARAM, *PVSNETDECVICEPARAM;
/************************************ 88258 END ******************************************/

typedef struct
{
    int    m_clipstate;      // clip card state
    int    m_clipnum;        // clip card num
} VSNETUPDATESLOT;

// NAS state Information
#define NAS_STATE_UNENABLE      1
#define NAS_STATE_UNCONNECT     2
#define NAS_STATE_CONNECT       4
#define NAS_STATE_INUSE         8

typedef enum
{
    MNT_CIFS,
    MNT_NFS,
} MNTFILETYPE;

typedef struct
{
    char m_NASenable;               // Enable NAS
    char m_reseved[3];              // reseved
    char m_NASip[32];               // NAS IP
    char m_NASpath[80];             // NAS path
    char m_NASuser[40];             // NAS username
    char m_NASpassword[40];         // NAS password
    MNTFILETYPE  mnt_type;
} VSNETNASINFO;

typedef struct
{
    int  m_state;                   // Disk State 1、not use 2、Unlink  4.linked 8.on using
    int  m_totalsize;               // Disk Size
    int  m_freesize;                // free size
} VSNETNASSTATE;

typedef struct
{
    VSNETNASINFO  m_NASinfo[8];
    VSNETNASSTATE m_NASstate[8];
} VSNETNASPARAM;

typedef struct
{
    char              m_url[32];    // IP address
    int               m_port;       // data port
} VSNETINFONET;

typedef struct
{
    VSNETINFONET      m_netinfo[4];    // net information
    char              m_sername[40];   // server name
    char              m_username[20];  // user name
    char              m_password[20];  // password
    int               m_cardnum;       // card number
    unsigned char     m_isexist;       // card be exist
    unsigned char     m_type;          // card type
    unsigned char     m_state;         // card state
    unsigned char     m_reseved;       // reseved
} VSNETCARDINFO;

typedef struct
{
    VSNETCARDINFO m_cardinfo[8];
} VSNETCARDPARAM;

// 探头当前状态
typedef struct
{
    int m_nStatus[16]; //0 开 1闭合 -1无效
} VSNET_ALARM_IN_STATUS;

/*******************************日记搜索定义******************************/
//集中编码器搜索日志的结构体中m_nMark的取值

/************************报********************警*****************/
/*主类型*/
#define LOG_ALARM_MAJOR 0x1
/*次类型*/
#define LOG_ALARM_MINOR_MOTIONDEC_START         0x101   /* 移动侦测开始*/
#define LOG_ALARM_MINOR_MOTIONDEC_STOP          0x111   /* 移动侦测结束*/
#define LOG_ALARM_MINOR_VIDEOLOST_START         0x102   /* 视频丢失开始*/
#define LOG_ALARM_MINOR_VIDEOLOST_STOP          0x112   /* 视频丢失结束*/
#define LOG_ALARM_MINOR_SENSOR_START            0x103   /* 探头报警开始*/
#define LOG_ALARM_MINOR_SENSOR_STOP             0x113   /* 探头报警结束*/
#define LOG_ALARM_MINOR_HIDEALARM_START         0x104   /* 遮挡报警开始*/
#define LOG_ALARM_MINOR_HIDEALARM_STOP          0x114   /* 遮挡报警结束*/

/************************异********************常*****************/
/*主类型*/
#define LOG_EXCEPTION_MAJOR 0x2
/*次类型*/
#define LOG_EXCEPTION_MINOR_ILLEGALACCESS               0x201   /* 非法访问*/
#define LOG_EXCEPTION_MINOR_HD_FULL                     0x202   /* 硬盘满*/
#define LOG_EXCEPTION_MINOR_HD_ERROR                    0x203   /* 硬盘错误*/
#define LOG_EXCEPTION_MINOR_NET_BROKEN                  0x204   /* 网线断*/
#define LOG_EXCEPTION_MINOR_NET_TIMEOUT                 0x205   /* 网络不通*/
#define LOG_EXCEPTION_MINOR_VIDEOMOODERROR              0x206   /* 视频制式错误*/
#define LOG_EXCEPTION_MINOR_IPADDRCONFLICT              0x207   /* IP地址冲突*/
#define LOG_EXCEPTION_MINOR_NODISK                      0x208   /* 无硬盘*/

#define LOG_EXCEPTION_MINOR_HD_TOOMANYBADSECTOR         0x209   /* 坏扇区太多 */
#define LOG_EXCEPTION_MINOR_HD_DISKUNHEALTH             0x210   /* 磁盘状态不健康 */
#define LOG_EXCEPTION_MINOR_HD_DISKREMOVE               0x211   /* 硬盘被不正常移去 */

#define LOG_EXCEPTION_MINOR_VIDEOEXCEPTION              0x212   /* 视频异常 */
#define LOG_EXCEPTION_MINOR_CPUEXCEPTION      	        0x213   /* CPU 异常 */
#define LOG_EXCEPTION_MINOR_NETEXCEPTION                0x214   /* 网络 异常 */
#define LOG_EXCEPTION_MINOR_NOUSEABLEDISK               0x215   /* 无可用硬盘*/

#define LOG_EXCEPTION_MINOR_MACHINE_FAN_STOP            0x216   /* 风扇停止转动*/
#define LOG_EXCEPTION_MINOR_MACHINE_FAN_SINGULARITY     0x217   /*风扇异常*/
#define LOG_EXCEPTION_MINOR_MACHINE_BANCARD_SINGULARITY 0x218  /*板卡异常*/

/************************操********************作*****************/
/*主类型*/
#define LOG_OPERAT_MAJOR 0x3
/*次类型*/
#define LOG_OPERAT_MINOR_START_DVR                      0x301   /* 开机*/
#define LOG_OPERAT_MINOR_STOP_DVR                       0x302   /* 关机*/

#define LOG_OPERAT_MINOR_LOCAL_LOGIN                    0x303   /* 本地登陆*/
#define LOG_OPERAT_MINOR_LOCAL_CFG_PARM                 0x304   /* 本地配置参数*/
#define LOG_OPERAT_MINOR_REMOTE_GET_PARM                0x305   /* 远程获得参数*/
#define LOG_OPERAT_MINOR_LOCAL_START_REC                0x306   /* 本地开始录像*/
#define LOG_OPERAT_MINOR_LOCAL_STOP_REC                 0x307   /* 本地停止录像*/
#define LOG_OPERAT_MINOR_LOCAL_PLAYBYFILE               0x308   /* 本地按文件回放*/
#define LOG_OPERAT_MINOR_LOCAL_PLAYBYTIME               0x309   /* 本地按时间回放*/
#define LOG_OPERAT_MINOR_LOCAL_PTZCTRL                  0x30A   /* 本地云台控制*/
#define LOG_OPERAT_MINOR_LOCAL_PREVIEW                  0x30B   /* 本地预览*/
#define LOG_OPERAT_MINOR_LOCAL_UPGRADE                  0x30C   /* 本地升级*/
#define LOG_OPERAT_MINOR_LOCAL_COPYFILE                 0x30D   /* 本地备份文件*/
#define LOG_OPERAT_MINOR_REMOTE_ARM                     0x30E   /* 布防*/
#define LOG_OPERAT_MINOR_REMOTE_REBOOT                  0x30F   /* 远程重启*/
#define LOG_OPERAT_MINOR_START_VT                       0x310   /* 开始语音对讲*/
#define LOG_OPERAT_MINOR_STOP_VT                        0x311   /* 停止语音对讲*/

#define LOG_OPERAT_MINOR_MACHINE_BANCARD_ADD            0x312   /*子板卡插入*/
#define LOG_OPERAT_MINOR_MACHINE_BANCARD_DEL            0x313   /*子板卡拔出*/

#define LOG_OPERAT_MINOR_FORMATDISK				        0x314   /* 格式化硬盘*/
#define LOG_OPERAT_MINOR_FORMATDISK_FAILED              0x315   /* 格式化硬盘失败*/
#define LOG_OPERAT_MINOR_DISK_LOCK				        0x316   /* 锁定硬盘*/
#define LOG_OPERAT_MINOR_DISK_UNLOCK                    0x317   /* 解锁硬盘*/


enum SEARCH_DIARY_MARK
{
    SEARCH_DIARY_MARK_TYPE     = 1 << 0, //日志主类型 有效
    SEARCH_DIARY_MARK_TYPE_SUB = 1 << 1, //日志次类型 有效
    SEARCH_DIARY_MARK_CHANNEL  = 1 << 2, //通道号有效
    SEARCH_DIARY_MARK_CARDNUM  = 1 << 3, //板卡号有效
    SEARCH_DIARY_MARK_TIME     = 1 << 4, //时间有效 
};

//集中编码器搜索日志的结构体
typedef struct
{
    int m_nMark;                      //规定那些值有效 见SEARCH_DIARY_MARK
    int m_nType;                      //日志主类型
    int m_nTypeSub;                   //日志次类型 
    int m_nChannel;                   //通道号 对于集中编码器0-255
    VSNETDIARY_NETTIME vsnStratTime;  //开始时间
    VSNETDIARY_NETTIME vsnStopTime;   //结束时间
    char m_szReseved[64];             //保留
} VSNET_SEARCH_DIARY;

//集中编码器搜索日志返回
#define MAXDIARYNUM 256
typedef struct
{
    int m_nMark;                         // 规定那些值有效 见SEARCH_DIARY_MARK
    int m_nChannel;                      // 通道号 对于集中编码器0-255 可算出板卡号
    int m_nType;                         // 日志主类型
    int m_nTypeSub;                      // 日志次类型     
    VSNETDIARY_NETTIME vsnTime;          // 发生时间
    char m_szReseved[12];                // 保留
} VSNET_DIARY_INFO_CONTENT;

typedef struct
{
    int m_nDiaryNum;                     //VSNET_DIARY_INFO_CONTENT有效个数
    VSNET_DIARY_INFO_CONTENT m_vdicInfo[MAXDIARYNUM];
} VSNET_DIARY_INFO;
/*******************************日记搜索定义 完**************************/


/************************************ 88258 END ******************************************/

/*
* 智能识别结构定义，用于智能DVS
* 修改时间：2011-03-23
* 修改人：zhangdw
*/

// 设备行为分析能力
typedef enum
{
    VCA_ABILITY_BKPLATEANALYSIS = (1 << 0),       // 车牌识别(识别做在管理软件上)
    VCA_ABILITY_PLATEANALYSIS = (1 << 1),         // 车牌识别(识别做在前端设备上)
	VCA_ABILITY_INTELLIGENTANALYSIS = (1 << 2),   // 允许智能行为分析(警戒线跨越、区域入侵、区域离开、徘徊)
} ENUM_VCA_ABILITYS;

typedef struct
{
    int ability_type;  // 行为分析能力类型
} VSNET_VCA_ABILITY;

/*
* 智能车牌识别结构定义，用于智能DVS
* 修改时间：2011-03-22
* 修改人：zhangdw
*/
/************************************ 智能车牌 开始 ***************************************/
#ifndef MAX_VCA_PLATERECT_NUM
#define MAX_VCA_PLATERECT_NUM  4     // 车牌分析最多车牌数
#endif

#ifndef MAX_VCA_PLATECAP_NUM
#define MAX_VCA_PLATECAP_NUM  3      // 车牌分析最多抓拍张数
#endif

#ifndef MAX_VCA_PLATERSTR_NUM
#define MAX_VCA_PLATERSTR_NUM 12     // 车牌分析车牌号码 字符长度 
#endif

#ifndef VCA_PLATE_JPGEND
#define VCA_PLATE_JPGEND 0xABABAAB
#endif

typedef struct
{
    short left;         // 车牌信息左上角横坐标
    short right;        // 车牌信息右下角横坐标
    short top;          // 车牌信息左上角纵坐标    
    short bottom;       // 车牌信息右下角纵坐标
} VSNET_VCA_PLATERECT;

typedef struct
{
    float left;         // 分析区域左上角横坐标
    float top;          // 分析区域左上角纵坐标
    float right;        // 分析区域右下角横坐标
    float bottom;       // 分析区域右下角纵坐标
} VSNET_VCA_PLATERECTF;

typedef struct
{
    unsigned int        version;                                // 车牌识别信息版本，0x01-V1.0
    unsigned int        time_stamp;                             // 分析时间戳，以毫秒为单位
    unsigned int        pos_num;                                // 当前帧定位到的车牌数目
    VSNET_VCA_PLATERECT pos_rect[MAX_VCA_PLATERECT_NUM];        // 当前帧定位到的车牌在原始图像中的位置
    unsigned char       recog_num;                              // 识别出的车牌号数目
    unsigned char       reserverd1[3];                          // 保留参数，置为0
    unsigned int        recog_gmttime[MAX_VCA_PLATERECT_NUM];   // 识别到的车牌最早出现的绝对时间，自1970年1月1日 00:00:00 开始的秒数,时区按GMT时区计算
    unsigned char		recog_number[MAX_VCA_PLATERECT_NUM][MAX_VCA_PLATERSTR_NUM];  // 识别出来的车牌字符
    unsigned char		recog_trust[MAX_VCA_PLATERECT_NUM];		// 车牌字符的置信度
    unsigned char       recog_brightness[MAX_VCA_PLATERECT_NUM];// 车牌亮度值1 ~ 255
    unsigned char		recog_color[MAX_VCA_PLATERECT_NUM];		// 车牌颜色 0:蓝底1:黄底2:白底－军牌3:白底－警牌4:黑牌5:未知
    unsigned char       recog_direction[MAX_VCA_PLATERECT_NUM];	// 车牌运动方向 0:未知方向 1:车牌从上到下运动 2:表示车牌从下到上运动
    unsigned char       recog_type[MAX_VCA_PLATERECT_NUM];		// 车牌类型 1:大型汽车号牌 2:小型汽车号牌 16:教练汽车号牌 23:警用汽车号牌 99:未知
    unsigned int        recog_speeds[MAX_VCA_PLATERECT_NUM];	// 车牌运动速度
    unsigned char       recog_carcolor[MAX_VCA_PLATERECT_NUM];	// 车身颜色 0:未知 1:蓝色 2:黑色 3:白色 4:黄色 5:红色 6:绿色 -1:失败
} VSNET_VCA_PLATEINFO;

typedef struct
{
    unsigned int    version;                                // 车牌识别抓拍信息版本，0x01-V1.0
    unsigned char   recog_num;                              // 识别出的车牌号数目
    unsigned char   cap_num;                                // 识别抓拍的张数
    unsigned char   reserverd1[2];                          // 保留参数，置为0
    unsigned int    recog_gmttime[MAX_VCA_PLATERECT_NUM];   // 识别到的车牌最早出现的绝对时间，自1970年1月1日 00:00:00 开始的秒数,时区按GMT时区计算
    unsigned char   recog_number[MAX_VCA_PLATERECT_NUM][MAX_VCA_PLATERSTR_NUM];  // 识别出来的车牌字符
    unsigned char   recog_trust[MAX_VCA_PLATERECT_NUM];		// 车牌字符的置信度
    unsigned char   recog_brightness[MAX_VCA_PLATERECT_NUM];// 车牌亮度值1 ~ 255
    unsigned char   recog_color[MAX_VCA_PLATERECT_NUM];		// 车牌颜色 0:蓝底1:黄底2:白底－军牌3:白底－警牌4:黑牌5:未知
    unsigned char   recog_direction[MAX_VCA_PLATERECT_NUM];	// 车牌运动方向 0:未知方向 1:车牌从上到下运动 2:表示车牌从下到上运动
    unsigned char   recog_type[MAX_VCA_PLATERECT_NUM];		// 车牌类型 1:大型汽车号牌 2:小型汽车号牌 16:教练汽车号牌 23:警用汽车号牌 99:未知
    unsigned int    recog_speeds[MAX_VCA_PLATERECT_NUM];	// 车牌运动速度
    unsigned char   recog_carcolor[MAX_VCA_PLATERECT_NUM];	// 车身颜色 0:未知 1:蓝色 2:黑色 3:白色 4:黄色 5:红色 6:绿色 -1:失败
} VSNET_VCA_PLATECAP;

typedef struct
{
    unsigned int        cap_gmttime;                        // 抓拍绝对时间，自1970年1月1日 00:00:00 开始的秒数,时区按GMT时区计算
    VSNET_VCA_PLATERECT cap_pos[MAX_VCA_PLATERECT_NUM];     // 识别到的车牌在抓拍图片中的位置
    unsigned int        cap_size;                           // 抓拍的数据大小，以字节为单位，不包含车牌定位信息大小
} VSNET_VCA_PLATEDATA;

// 车牌识别-车牌大小
typedef struct
{
    int plate_w_min;     // 车牌最小宽度，以像素为单位
    int plate_w_max;     // 车牌最大宽度，以像素为单位
} VSNET_VCA_PLATEWIDTH;

// 车牌识别默认省份
typedef struct
{
	int trust_thresh;    //信度的阈值低于该值 用默认省份代替

    // 默认省份:
    // 云36，京37，冀38，吉39，宁40，川41，新42，晋43，桂44，沪45，津46，浙47，
    // 渝48，湘49，琼50，甘51，皖52，粤53，苏54，蒙55，藏56，豫57，贵58，赣59，
    // 辽60，鄂61，闽62，陕63，青64，鲁65，黑66
    char plate_chn_default[MAX_VCA_PLATERSTR_NUM];//默认省份字符	
} VSNET_VCA_PLATECHNDEFAULT;

// 车牌识别汉字掩码
typedef struct
{
    unsigned char plate_chn_mask[64];
    // 按位表示中文汉字识别的掩码（提高字符识别的精度），
    // 1-屏蔽该汉字识别，0-开始该汉字识别，
    // 汉字对应掩码顺序如下（从高到底）：
    // 云 京 冀 吉 宁 川 新 晋 桂 沪 津 浙 渝 湘 琼 甘 皖 粤 苏 蒙 藏 豫 贵 赣 辽 鄂 闽 陕 青 鲁 黑
    // （目前只有31个省份汉字，其余位保留以后扩展更多汉字）
} VSNET_VCA_PLATECHNMASK;

// 车牌识别叠加模式
typedef enum
{
    VCA_PLATEDRAW_RECT = 0x01,         // 编码叠加车牌识别目标区域
    VCA_PLATEDRAW_TARGET = 0x02,       // 编码叠加车牌定位框
    VCA_PLATEDRAW_CAPRECT = 0x04,      // 抓拍叠加车牌识别目标区域
    VCA_PLATEDRAW_CAPTARGET = 0x08     // 抓拍叠加车牌定位框
} ENUM_VCA_PLATEDRAW;

// 车牌识别抓拍参数
typedef enum
{
    VCA_PLATECAPMODE_QUALITY = 0,     // 抓拍方式，最清晰的一张
    VCA_PLATECAPMODE_LAST = 1         // 抓拍方式，最后的一张
} ENUM_VCA_PLATECAPMODE;

// 车牌图片质量设置
typedef struct
{
    unsigned short cap_quality;       // 抓拍图像压缩率，取值1~100，数值越高表示质量越好
    unsigned short cap_num;           // 抓拍的图像张数，取值范围1~3
    unsigned short cap_mode;          // 抓拍方式，0-最清晰的一张，1-最后的一张
    unsigned short cap_store;         // 抓拍图片前端存储选项，0-不存储，1-存储
} VSNET_VCA_PLATECAPPARAM;

// 车牌图片车牌字符叠加设置 画框请在车牌识别叠加模式中设置
typedef struct
{
    unsigned char bEnable;            // 是否启用jpg图片车牌号码叠加
    unsigned char reserverd[3];       // 保留
    float         jpgosd_scale;       // 字符显示比例0.5-2.0
    float         jpgosd_x;           // 叠加车牌的x坐标
    float         jpgosd_y;           // 叠加车牌的y坐标
} VSNET_VCA_PLATEJPGOSD;

// 车牌模式
typedef struct
{
	int plate_draw;                   // 识别叠加框 详细见ENUM_VCA_PLATEDRAW
    int deskew_flag;                  // 车牌识别是否对车牌做倾斜校正
} VSNET_VCA_PLATEMODE;

// 车牌版本
typedef struct
{
    char   m_szDspVersion[16];          //DSP版本
    char   m_szDspDate[16];             //DSP日期
    char   m_szPlateVersion[16];        //算法版本
    char   m_szPlateDate[16];           //算法日期
} VSNET_VCA_PLATEVERSION;

/************************************ 智能车牌 结束 ***************************************/

/************************************ SXSD START ******************************************/
#define SXSD_PARAM_MEB_NUM           11
#define SXSD_PARAM_DATA_NUM          7

typedef enum
{
    SXSD_PARAM_METHANE_L = 0,        // 低浓度甲烷     0-4%         x.xx
    SXSD_PARAM_OXYGEN,               // 氧气           0-25%        xx.x0
    SXSD_PARAM_CARBONM,              // 一氧化碳       0-1000ppm    xxxx
    SXSD_PARAM_CARBOND,              // 二氧化碳       0-5%         x.xx
    SXSD_PARAM_TEMPERAT,             // 温度（单）     -10℃-+70℃  xx.x0
    SXSD_PARAM_WATERPRESS,           // 水压           0-9999Pa     xxxx(0.1-999.9KPa  xxx.x0  0.01-99.99MPa  xx.xx)
    SXSD_PARAM_WINDSPEED,            // 风速           0.4—15m/s   xx.x0
    SXSD_PARAM_HYDROGEN,             // 硫化氢         0-100ppm     xxxx
    SXSD_PARAM_WATERLINE,            // 水位           暂时不用
    SXSD_PARAM_METHANE_H,            // 高浓度甲烷     0%-100%      xxx.x0
    SXSD_PARAM_HUMIDITY              // 湿度           0-100%       xxx
} ENUM_SXSD_PARAM;

typedef struct
{
    unsigned short   m_nDataState;   // 数据状态(用于实时数据传输)(0.无数据 1.正常 2.报警 3.异常)
    unsigned short   m_nReserverd;   // 对于SXSD_PARAM_WATERLIN 用来表示(0、普通 1、KP 2、MP)；其他占时保留
    int              m_nEvrmtValue;  // 环境数据值 (带小数位 真值数据x100 注:只有一位小数用0补齐，显示时保留真值格式)
    int              m_nAlarmData;   // 报警浓度 下标由ENUM_SXSD_PARAM指定
	int              m_nTypeEvrmt;   // 环境数据值标示value(0 - 6) 共7个数据（现在每数据有7种类型）
} SXSD_DATA_PARAM;

// 用于SXSD实时传输数据
typedef struct
{
    char             m_cAddressDis[20];                       // 采集器地点(包含仓内、仓外)
	int              m_nValueType[SXSD_PARAM_MEB_NUM];        // 对应m_pEvrmtValue的类型(值由 ENUM_SXSD_PARAM指定)
    SXSD_DATA_PARAM  m_pEvrmtValue[SXSD_PARAM_MEB_NUM][SXSD_PARAM_DATA_NUM];  // 数据值 （共11组）
} SXSD_REAL_DATA_TEAP;

typedef struct
{
    SXSD_REAL_DATA_TEAP m_pRealData[2];                       //分别表示两组数据（仓内、仓外）
} VSNET_SXSD_REAL_DATA;

// 用于SXSD的报警阀值设置
typedef struct
{
    char             m_cAddressDis[20];                       // 采集器地点(包含仓内、仓外)
    unsigned         m_nXcoordinate;                          // OSD叠加垂直坐标值
    unsigned         m_nYcoordinate;                          // OSD叠加水平坐标值
	int              m_nValueType[SXSD_PARAM_MEB_NUM];        // OSD叠加类型(0-6) 
	                                                          // (如：m_nOsdOrder[1]=SXSD_PARAM_OXYGEN,m_nValueType[1]=1,
	                                                          // 就说明OSD位置2的模拟量为 氧气2)
    int              m_nOsdOrder[SXSD_PARAM_MEB_NUM];         // OSD叠加顺序 值为ENUM_SXSD_PARAM指定 数组中无记录为不显示
    int              m_nAlarmValue[SXSD_PARAM_MEB_NUM];       // 报警范围值  下标由ENUM_SXSD_PARAM指定
	int              m_nAlarmEnable[SXSD_PARAM_MEB_NUM];      // 报警有效标记 下标由ENUM_SXSD_PARAM指定（-1未收到数据，0传感器未链接，1传感器正常）
} SXSD_TEMP_EVRMTPARAM;

// 用于SXSD的参数设置
typedef struct
{
    SXSD_TEMP_EVRMTPARAM m_pEventParam[2];                    // 分别表示两组数据（仓内、仓外）
} VSNET_SXSD_EVRMTPARAM;
/************************************* SXSD ENDS ******************************************/

/************************************* HL-SAMMY STARTS ************************************/
// HL-SAMMY
typedef struct
{
    int m_nVideoMode;    // video mode 0,single 1,mul
    int m_nChannel;      // channels
} VSNET_ROUNDVIDEO_OUT;
/************************************* HL-SAMMY ENDS **************************************/

// 人车流量统计
typedef struct
{
    int m_nVehicle;      // 车计数
    int m_nPedestrian;   // 人计数
} VSNET_SMART_TARGETCOUNT;

// 电流环信息
typedef struct
{    
    char  m_flag[8];         // 是否超出阀值
    char  m_dlh_name[8][56]; // 电流环名字
    char  m_dlh_unit[8][16]; // 电流环单位
    float m_dlh_value[8];    // 电流环数值 
    int  m_dlh_precision[8]; // 精度
} VSNET_CURRENTLOOP;        

// 报警输入信息
typedef struct
{
    int  m_channel;
    int m_alarmin_states;    // 报警输入状态
    char m_alarmin_name[32]; // 探头名称
} VSNET_ALARMIN;

// WHYB_D球机参数
typedef struct
{
	int m_ptz_horizontal_angle;	// 水平角度的100倍
	int m_ptz_vertical_angle;	// 垂直角度的100倍
	int m_ptz_multiple;			// 变倍倍数
	int m_reverse;				// 保留
} VSNETPTZ_WHYB;

//SXKD 回传JPG 
typedef struct
{
	int           m_busnum;            //车辆编号 0~255
	int           m_chno;              //摄像机通道号
	int           m_jpegsize;          //jpeg长度
	int           m_year;
	int           m_mon;
	int           m_day;
	int           m_hour;
	int           m_min;
	int           m_sec;
	unsigned char m_jpgbuff[1024*200]; //JPG数据
}VSNET_SXKD_JPEG;
				
/************************************* NVR begin ******************************************/
#define                 MAX_DEV_SEARCH_NUM          100

typedef struct
{
    char            sername[24];
    unsigned char   mac[8];
    char            ip[20];         // ip
    char            mask[16];       // submask
    char            gw[16];         // gateway
    char            dns[16];        // DNS
    char            multiip[16];
    unsigned short  wPortWeb, wPortSer, wPortMulti;
    unsigned short  channel;
    unsigned short  devtype;
    unsigned short  reserve;
} VSNETDEVINFO;

// the device list of NVR dev search
typedef struct
{
    int             m_total;                      // the number of device
    VSNETDEVINFO    m_puinfo[MAX_DEV_SEARCH_NUM]; // info of each device
} VSNETNVRDEVSEARCHINFO;

typedef struct
{
    char            m_sername[24];      // server name
    char            m_serip[20];        // server ip address
    unsigned short  m_serport;          // server port
    unsigned short  m_channel;          // the channel of server to be connected
    unsigned short  m_transtype;        // the media transport type tcp/udp/multi
    unsigned short  m_chlinkstate;      // channel link state
    unsigned short  m_imgheight;        // image height
    unsigned short  m_imgwidth;         // image width
    char            m_username[20];     // server user name
    char            m_passwd[20];       // server password
} VSNETNVRDECCHINFO;

typedef struct
{
    int                 m_enable;       // channel been used ?
    int                 m_maxsize;      // max video format
    VSNETNVRDECCHINFO   m_chinfo;       // channel info
} VSNETNVRDECCHSTATE;

typedef struct
{
    int                 m_num;                          // number in data
    char                m_chname[VSNET_DVR_MAXCH][20];  // name fo each channel
    int                 m_chimgsize[VSNET_DVR_MAXCH];   // replay channel image size 0:NO 1:D1 3:720P 7:1080P
} VSNETNVRGUINOTIFYNODECABILITY;

typedef struct
{
    int                 m_enable;       // pu time synchronous enable? 0 disable, 1 everyday 00:00
} VSNETNVRPUTIMESYN;

/************************************* NVR end ********************************************/

/************************************** DXHJ Custom Begin *********************************/
// DXHJ7304 server record case info
typedef struct
{
    char            m_szCaseName[28];    // case name
    char            m_szCaseID[28];      // case id
    char            m_szCaseExecor[28];  // case execor
    char            m_reverse1[28];      // court name
    char            m_reverse2[28];
    char            m_reverse3[28];
} VSNETDXJBCASEINFO;

// DXHJ7304 current max window chnannel no
typedef struct
{
    int             m_iVGANO;
    char            m_reverse[16];
} VSNETDXJBVGA;

// DXHJ_D 服务器状态
typedef struct
{
    int     m_nCPUUsage;    // CPU使用率 0~100
    int     m_nMemUsage;    // 内存占用率 0~100
    int     m_bSerRec;      // 服务器录像状态
    char    m_reverse[60];  // 保留
} VSNETDXHJWORKSTATUS;

// DXHJ_D DVD控制
typedef struct
{
    char    m_value[8];
} VSNETDVDCTRL;

// DXHJ_D 摄像机控制
typedef struct
{
    char    m_value[8];
} VSNETCAMCTRL;

// DXHJ_D 视频切换方式
typedef struct
{
	int  m_enable;          // 控制视频切换方式:0:手动,1:自动
	char m_reverse[64];     // 保留
} VSNETAUTOORHANDCTRL; 

// ZGHY_D 服务器远程录像控制和返回参数
typedef struct
{
    VSNETDXJBCASEINFO   m_caseinfo;             // 设置: 案件信息
    int                 m_zgch[8];              // 设置: ZGHY通道录像参数, 1 开始, 0 不开始
    char                m_reccordName_get[64];  // 获取: 返回录像名
    char                m_diskserialnum[32];    // 获取: 磁盘序列号
    int                 m_slot;                 // 获取: 卡槽位
    char                m_sharename[32];        // 获取: 磁盘共享名
} VSNETSERVERRECPARAM_ZGHY;

// ZGHY_D OSD参数
typedef struct
{
    char m_osd[400];        // OSD叠加内容
    int  m_speed;           // 滚动速度
    int  m_width;           // 给定OSD显示宽度
    char m_reverse[64];     // 保留
} VSNETZGOSD;

// ZGHY_D OSD参数2标题
typedef struct
{
    int               bShow;           // show(1),hide(0)
    char              lpString[64];    // characters to be showed
    unsigned short    x, y;            // coordinate
} TITALOSD_ZGHY;

// ZGHY_D OSD参数2
typedef struct
{
    TIMEOSD           m_time;          // time
    TITALOSD_ZGHY     m_tital[4];      // title
    BITSINFOOSD       m_bits;          // bit rate
} CHANNOSD_ZGHY;

// ZGHY_D RTMP信息
typedef struct
{
    char m_hostname[48];    // RTMP主机地址 (IP或者域名) 默认 "0.0.0.0"
    int  m_rtmpport;        // RTMP端口 默认 1935
    char m_appname[40];     // 应用实例名称 默认 live
    char m_streamname[48];  // RTMP流名称 默认 livestream
} VSNETRTMPINFO;

// ZGHY_D 磁盘切换阀值参数
typedef struct
{
    int  m_diskchangelimen; // 磁盘切换阀值（单位：MB）
    char m_reverse[64];     // 保留
} VSNETDISKCHANGELIMEN;

// ZGHY_D 录像打包时间参数
typedef struct
{
    int  m_recpacktime;     // 录像打包时间（单位：分钟）
    char m_reverse[64];     // 保留
} VSNETRECPACKTIME;

// DXHJ 光盘信息
typedef struct
{
    int m_state;                // 当前状态
    unsigned int m_totalsize;   // 光盘总大小
    unsigned int m_freesize;    // 光盘剩余空间
    unsigned int m_usedsize;    // 已用空间
} VSNETCDROMINFO;

// DXHJ MAC地址
typedef struct
{
    char m_pcmac[20];
} VSNETPCMACADDR;

// DXHJ 聊天对象
typedef struct
{
    char m_chator[20];  //消息目标对象用户名
    char m_chaturl[16]; //消息目标对象IP地址
} VSNETCHATTO;

// DXHJ 聊天消息
typedef struct
{
    VSNETCHATTO m_chatto;
    char m_chatmsg[256];//消息内容
} VSNETCHATMSG;

/************************************** DXHJ Custom End ***********************************/

/************************************** SZWJ       Start***********************************/
//SZWJ RTMP控制参数
typedef struct
{
	int  m_startrtmp;   //0:停止RTMP，1:启动RTMP
	char m_reverse[32]; //保留
}VSNETSZWJRTMP;
/************************************** SZWJ Custom End ***********************************/

/************************************** CDTY       Start***********************************/
// CDTY_D WIFI和网络参数
typedef struct
{
	int  m_wifistate;   // wifi状态：0:未连接, 1:已连接
	int  m_wifisignal;  // wifi信号强度
    int  m_nettype;     // 网络类型：0-正常模式，1-NAT,2-UPNP
} VSNET_CDTY_PARAM;
/************************************** CDTY         End***********************************/

/************************************** FJTS       Start***********************************/
//FJTS_D 获取硬件序列号
typedef struct
{
	char  m_romno[80];   // 硬件序列号
} VSNET_FJTS_ROMNO;
/************************************** FJTS         End***********************************/

/************************************** SHRZ      Start ***********************************/
#define VSNET_SHRZ_ALARMTYPENUM 4

//SD卡状态
typedef struct
{
    int m_loadstate; //SD卡挂载状态:-1-无卡 0-挂载失败 1-挂载成功
    VSDISKSTATE m_sdstate;//SD卡详细信息
} VSNET_SHRZ_SDSTATE;

//3G信号强度
typedef struct
{
    int m_signal;
} VSNET_SHRZ_3GSIGNAL;

//报警状态
typedef struct
{
    int m_type;//报警类型
    int m_state;//状态:0-未报警，1-报警
} VSNET_SHRZ_ALARM;

typedef struct
{
    VSNET_SHRZ_ALARM m_alarm[VSNET_SHRZ_ALARMTYPENUM];
} VSNET_SHRZ_ALARMSTATE;
/************************************** SHRZ        End ***********************************/

#endif// __VSNET_STRUCTDEF_H__

