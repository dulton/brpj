--------------------------------------------
-- Export file for user USHOW2            --
-- Created by wyx on 2014-10-21, 14:25:40 --
--------------------------------------------

spool ushow2_gongsi_1021.log

prompt
prompt Creating table TB_ALARM_CAR
prompt ===========================
prompt
create table TB_ALARM_CAR
(
  nid         NUMBER not null,
  ncamera     NUMBER default 1,
  scarnumber  VARCHAR2(100),
  scolour     VARCHAR2(100),
  stype       VARCHAR2(100),
  sdirection  VARCHAR2(100),
  scolor      VARCHAR2(100),
  ndegree     NUMBER,
  dgettime    DATE,
  nflag       NUMBER default 0,
  dcreate     DATE default sysdate,
  bpicture    BLOB,
  sfile       VARCHAR2(260),
  nsize       NUMBER,
  scameraname VARCHAR2(100),
  sip         VARCHAR2(100),
  blackid     NUMBER
)
;
comment on table TB_ALARM_CAR
  is '车辆识别结果表';
comment on column TB_ALARM_CAR.nid
  is '主键ID';
comment on column TB_ALARM_CAR.ncamera
  is '摄像头编号';
comment on column TB_ALARM_CAR.scarnumber
  is '车牌号';
comment on column TB_ALARM_CAR.scolour
  is '车牌颜色';
comment on column TB_ALARM_CAR.stype
  is '车辆类型';
comment on column TB_ALARM_CAR.sdirection
  is '车辆方向';
comment on column TB_ALARM_CAR.scolor
  is '颜色，明码';
comment on column TB_ALARM_CAR.ndegree
  is '置信度';
comment on column TB_ALARM_CAR.dgettime
  is '捕获时间';
comment on column TB_ALARM_CAR.nflag
  is '处理标识，0未处理，1已处理';
comment on column TB_ALARM_CAR.dcreate
  is '入库时间';
comment on column TB_ALARM_CAR.bpicture
  is '照片';
comment on column TB_ALARM_CAR.sfile
  is '文件路径';
comment on column TB_ALARM_CAR.nsize
  is '照片大小';
comment on column TB_ALARM_CAR.scameraname
  is '摄像头名称';
comment on column TB_ALARM_CAR.sip
  is '摄像头IP';
comment on column TB_ALARM_CAR.blackid
  is '黑名单ID';
create index ALARM_CAR_INDEX on TB_ALARM_CAR (DCREATE);
alter table TB_ALARM_CAR
  add constraint ALARM_CAR_NID primary key (NID);

prompt
prompt Creating table TB_ALARM_VEHICLE
prompt ===============================
prompt
create table TB_ALARM_VEHICLE
(
  nid         NUMBER not null,
  ncamera     NUMBER default 1,
  scarnumber  VARCHAR2(100),
  scolour     VARCHAR2(100),
  stype       VARCHAR2(100),
  sdirection  VARCHAR2(100),
  scolor      VARCHAR2(100),
  ndegree     NUMBER,
  dgettime    DATE,
  nflag       NUMBER default 0,
  dcreate     DATE default sysdate,
  bpicture    BLOB,
  sfile       VARCHAR2(260),
  nsize       NUMBER,
  scameraname VARCHAR2(100),
  sip         VARCHAR2(100),
  blackid     NUMBER
)
;
comment on table TB_ALARM_VEHICLE
  is '电动车识别结果表';
comment on column TB_ALARM_VEHICLE.nid
  is '主键ID';
comment on column TB_ALARM_VEHICLE.ncamera
  is '摄像头编号';
comment on column TB_ALARM_VEHICLE.scarnumber
  is '电动车牌号';
comment on column TB_ALARM_VEHICLE.scolour
  is '电动车车牌颜色';
comment on column TB_ALARM_VEHICLE.stype
  is '电动车类型';
comment on column TB_ALARM_VEHICLE.sdirection
  is '电动车方向';
comment on column TB_ALARM_VEHICLE.scolor
  is '颜色，明码';
comment on column TB_ALARM_VEHICLE.ndegree
  is '置信度';
comment on column TB_ALARM_VEHICLE.dgettime
  is '捕获时间';
comment on column TB_ALARM_VEHICLE.nflag
  is '处理标识，0未处理，1已处理';
comment on column TB_ALARM_VEHICLE.dcreate
  is '入库时间';
comment on column TB_ALARM_VEHICLE.bpicture
  is '照片';
comment on column TB_ALARM_VEHICLE.sfile
  is '文件路径';
comment on column TB_ALARM_VEHICLE.nsize
  is '照片大小';
comment on column TB_ALARM_VEHICLE.scameraname
  is '摄像头名称';
comment on column TB_ALARM_VEHICLE.sip
  is '摄像头IP';
comment on column TB_ALARM_VEHICLE.blackid
  is '黑名单ID';
create index ALARM_VEHICLE_INDEX on TB_ALARM_VEHICLE (DCREATE);
alter table TB_ALARM_VEHICLE
  add constraint ALARM_VEHICLE_NID primary key (NID);

prompt
prompt Creating table TB_CAMERA_INFO
prompt =============================
prompt
create table TB_CAMERA_INFO
(
  objectid                INTEGER not null,
  sadministrativedivision VARCHAR2(100),
  sstreet                 VARCHAR2(100),
  spolicestation          VARCHAR2(100),
  ssecurityarea           VARCHAR2(100),
  slivingareasname        VARCHAR2(100),
  saddress                VARCHAR2(100),
  fheight                 NUMBER(19,10),
  stype                   VARCHAR2(100),
  fresolution             NUMBER(19,10),
  sequipmentmanufacture   VARCHAR2(100),
  susage                  VARCHAR2(100),
  sowership               VARCHAR2(100),
  stelphone               VARCHAR2(20),
  fx                      NUMBER(19,10),
  fy                      NUMBER(19,10),
  areatype                NUMBER(19,10),
  smark1                  VARCHAR2(100),
  smark2                  VARCHAR2(100),
  smark3                  VARCHAR2(100),
  sip                     VARCHAR2(100),
  personflag              NUMBER(1),
  carflag                 NUMBER(1),
  fabricflag              NUMBER(1),
  canceltime              DATE,
  status                  VARCHAR2(20),
  sport                   VARCHAR2(30),
  schannel                VARCHAR2(30),
  suser                   VARCHAR2(30),
  spwd                    VARCHAR2(30),
  nmachineid              NUMBER(19,10),
  sorgno                  VARCHAR2(20),
  scameraname             VARCHAR2(1000),
  sipserver               VARCHAR2(100),
  sipcent                 VARCHAR2(100),
  sportcent               VARCHAR2(30),
  sportserver             VARCHAR2(30)
)
;
comment on table TB_CAMERA_INFO
  is '摄像头信息';
comment on column TB_CAMERA_INFO.objectid
  is '主键';
comment on column TB_CAMERA_INFO.sadministrativedivision
  is '行政区划';
comment on column TB_CAMERA_INFO.sstreet
  is '街道';
comment on column TB_CAMERA_INFO.spolicestation
  is '派出所';
comment on column TB_CAMERA_INFO.ssecurityarea
  is '警务区';
comment on column TB_CAMERA_INFO.slivingareasname
  is '小区名称';
comment on column TB_CAMERA_INFO.saddress
  is '详细地址';
comment on column TB_CAMERA_INFO.fheight
  is '安装高度';
comment on column TB_CAMERA_INFO.stype
  is '摄像头类型';
comment on column TB_CAMERA_INFO.fresolution
  is '分辨率';
comment on column TB_CAMERA_INFO.sequipmentmanufacture
  is '设备厂商';
comment on column TB_CAMERA_INFO.susage
  is '用途';
comment on column TB_CAMERA_INFO.sowership
  is '产权单位';
comment on column TB_CAMERA_INFO.stelphone
  is '联系方式';
comment on column TB_CAMERA_INFO.fx
  is 'X坐标';
comment on column TB_CAMERA_INFO.fy
  is 'Y坐标';
comment on column TB_CAMERA_INFO.areatype
  is '应用场景，该字段目前不用，TS_CAMERA_SCENE_RELATION中展现';
comment on column TB_CAMERA_INFO.smark1
  is '备用字段';
comment on column TB_CAMERA_INFO.smark2
  is '备用字段';
comment on column TB_CAMERA_INFO.smark3
  is '备用字段';
comment on column TB_CAMERA_INFO.sip
  is 'IP地址';
comment on column TB_CAMERA_INFO.personflag
  is '抓拍人任务标志，1是';
comment on column TB_CAMERA_INFO.carflag
  is '抓拍车任务标志，1是';
comment on column TB_CAMERA_INFO.fabricflag
  is '抓拍物任务标志，1是';
comment on column TB_CAMERA_INFO.canceltime
  is '注销时间';
comment on column TB_CAMERA_INFO.status
  is '摄像头状态，1正常，其他不正常';
comment on column TB_CAMERA_INFO.sport
  is '端口号';
comment on column TB_CAMERA_INFO.schannel
  is '通道号';
comment on column TB_CAMERA_INFO.suser
  is '用户名';
comment on column TB_CAMERA_INFO.spwd
  is '密码';
comment on column TB_CAMERA_INFO.nmachineid
  is '对应的服务器ID';
comment on column TB_CAMERA_INFO.sorgno
  is '派出所代码简码';
comment on column TB_CAMERA_INFO.scameraname
  is '摄像头名称';
comment on column TB_CAMERA_INFO.sipserver
  is '服务端IP';
comment on column TB_CAMERA_INFO.sipcent
  is '中心IP';
comment on column TB_CAMERA_INFO.sportcent
  is '中心端口号';
comment on column TB_CAMERA_INFO.sportserver
  is '服务端端口号';
alter table TB_CAMERA_INFO
  add constraint CAMERA_INFO_OBJECTID primary key (OBJECTID);

prompt
prompt Creating table TB_CAR_BLACK
prompt ===========================
prompt
create table TB_CAR_BLACK
(
  nid        NUMBER not null,
  scarnumber VARCHAR2(100),
  sbrand     VARCHAR2(255),
  slostname  VARCHAR2(100),
  sphone     VARCHAR2(100),
  smark      VARCHAR2(1000)
)
;
comment on table TB_CAR_BLACK
  is '车辆黑名单（比对源）';
comment on column TB_CAR_BLACK.nid
  is '编号';
comment on column TB_CAR_BLACK.scarnumber
  is '车牌号';
comment on column TB_CAR_BLACK.sbrand
  is '品牌';
comment on column TB_CAR_BLACK.slostname
  is '失主姓名
';
comment on column TB_CAR_BLACK.sphone
  is '联系电话
';
comment on column TB_CAR_BLACK.smark
  is '其他信息
';
alter table TB_CAR_BLACK
  add constraint CAR_BLACK_NID primary key (NID);

prompt
prompt Creating table TB_DETECT_SERVER
prompt ===============================
prompt
create table TB_DETECT_SERVER
(
  nid        NUMBER not null,
  sip        VARCHAR2(32),
  isoccupy   NUMBER default 0,
  channelnum NUMBER,
  isenable   NUMBER default 0,
  lasttime   DATE default sysdate,
  missionid  NUMBER,
  ncamera    NUMBER default 0
)
;
comment on table TB_DETECT_SERVER
  is '分布式识别设备表';
comment on column TB_DETECT_SERVER.nid
  is '识别服务器ID';
comment on column TB_DETECT_SERVER.sip
  is '服务器IP';
comment on column TB_DETECT_SERVER.isoccupy
  is '占用状态 0未被占用 1占用';
comment on column TB_DETECT_SERVER.channelnum
  is '第N路 窗口序号';
comment on column TB_DETECT_SERVER.isenable
  is '启用状态 0故障 1正常';
comment on column TB_DETECT_SERVER.lasttime
  is '心跳 时间比对';
comment on column TB_DETECT_SERVER.missionid
  is '最后操作该服务器的任务ID 预先占用任务ID';
comment on column TB_DETECT_SERVER.ncamera
  is '预先占用的摄像头ID。 防止多次触发';

prompt
prompt Creating table TB_DEVICE
prompt ========================
prompt
create table TB_DEVICE
(
  scameraname VARCHAR2(200),
  sipserver   VARCHAR2(32),
  sport       NUMBER,
  suser       VARCHAR2(30),
  spwd        VARCHAR2(200),
  ncamera     NUMBER not null,
  nid         VARCHAR2(30),
  svenderid   NUMBER,
  channel     NUMBER,
  rtspurl     VARCHAR2(500),
  rtpmode     NUMBER,
  decodetag   NUMBER,
  detectip    VARCHAR2(32),
  isplay      NUMBER default 0,
  userid      NUMBER,
  userlv      NUMBER,
  longitude   VARCHAR2(32),
  latitude    VARCHAR2(32),
  crossindex  VARCHAR2(64),
  crossid     NUMBER,
  detectid    NUMBER
)
;
comment on table TB_DEVICE
  is '设备表';
comment on column TB_DEVICE.scameraname
  is '摄像头名称';
comment on column TB_DEVICE.sipserver
  is '摄像头IP';
comment on column TB_DEVICE.sport
  is '端口号';
comment on column TB_DEVICE.suser
  is '用户名';
comment on column TB_DEVICE.spwd
  is '密码';
comment on column TB_DEVICE.ncamera
  is '摄像头id';
comment on column TB_DEVICE.nid
  is '外键，关联T_ZD_ORG.nid';
comment on column TB_DEVICE.svenderid
  is '摄像机厂家ID,0海康，1大华 ';
comment on column TB_DEVICE.channel
  is '通道号';
comment on column TB_DEVICE.rtspurl
  is 'RTSP 流地址';
comment on column TB_DEVICE.rtpmode
  is '流媒体传输模式 0=RTP-RTSP 1=RTP-UDP';
comment on column TB_DEVICE.decodetag
  is '流媒体解码标志 0=haikang 1=ga_h264';
comment on column TB_DEVICE.detectip
  is '播放该摄像头的识别服务器IP';
comment on column TB_DEVICE.isplay
  is '识别服务器 是否识别  0 未识别 1识别';
comment on column TB_DEVICE.userid
  is '插入该设备的用户NID';
comment on column TB_DEVICE.userlv
  is '插入该设备的用户级别';
comment on column TB_DEVICE.longitude
  is '经度';
comment on column TB_DEVICE.latitude
  is '纬度';
comment on column TB_DEVICE.crossindex
  is '卡口索引号';
comment on column TB_DEVICE.crossid
  is '卡口ID';
comment on column TB_DEVICE.detectid
  is '播放该摄像头的识别服务器ID';
alter table TB_DEVICE
  add constraint DEVICE_NCAMERA primary key (NCAMERA);

prompt
prompt Creating table TB_LOG
prompt =====================
prompt
create table TB_LOG
(
  nid         NUMBER not null,
  dcreatetime DATE default SYSDATE,
  susername   VARCHAR2(32),
  smark       VARCHAR2(1024)
)
;
comment on table TB_LOG
  is '系统日志表';
comment on column TB_LOG.nid
  is '主键';
comment on column TB_LOG.dcreatetime
  is '入库时间';
comment on column TB_LOG.susername
  is '用户名';
comment on column TB_LOG.smark
  is '日志信息';
create index LOG_INDEX on TB_LOG (DCREATETIME);
alter table TB_LOG
  add constraint PRI_LOG_NID primary key (NID);

prompt
prompt Creating table TB_MATCH_RESULT_CAR
prompt ==================================
prompt
create table TB_MATCH_RESULT_CAR
(
  nid         NUMBER not null,
  ncamera     NUMBER default 1,
  scarnumber  VARCHAR2(100),
  scolour     VARCHAR2(100),
  stype       VARCHAR2(100),
  sdirection  VARCHAR2(100),
  ndegree     NUMBER,
  dgettime    DATE,
  nflag       NUMBER default 0,
  dcreate     DATE default sysdate,
  bpicture    BLOB,
  sfile       VARCHAR2(260),
  scolor      VARCHAR2(100),
  nsize       NUMBER,
  scameraname VARCHAR2(100),
  sip         VARCHAR2(100)
)
;
comment on table TB_MATCH_RESULT_CAR
  is '车辆报警表';
comment on column TB_MATCH_RESULT_CAR.nid
  is '主键ID';
comment on column TB_MATCH_RESULT_CAR.ncamera
  is '摄像头编号';
comment on column TB_MATCH_RESULT_CAR.scarnumber
  is '车牌号';
comment on column TB_MATCH_RESULT_CAR.scolour
  is '车牌颜色';
comment on column TB_MATCH_RESULT_CAR.stype
  is '车辆类型';
comment on column TB_MATCH_RESULT_CAR.sdirection
  is '车辆方向';
comment on column TB_MATCH_RESULT_CAR.ndegree
  is '置信度';
comment on column TB_MATCH_RESULT_CAR.dgettime
  is '捕获时间';
comment on column TB_MATCH_RESULT_CAR.nflag
  is '报警标志 1 报警 0 不报警';
comment on column TB_MATCH_RESULT_CAR.dcreate
  is '入库时间';
comment on column TB_MATCH_RESULT_CAR.bpicture
  is '照片';
comment on column TB_MATCH_RESULT_CAR.sfile
  is '文件路径';
comment on column TB_MATCH_RESULT_CAR.scolor
  is '颜色，明码';
comment on column TB_MATCH_RESULT_CAR.nsize
  is '照片大小';
comment on column TB_MATCH_RESULT_CAR.scameraname
  is '摄像头名称';
comment on column TB_MATCH_RESULT_CAR.sip
  is '摄像头IP';
create index MATCH_RESULT_CAR_INDEX on TB_MATCH_RESULT_CAR (DCREATE);
alter table TB_MATCH_RESULT_CAR
  add constraint MACTCH_RESULT_CAR_ID primary key (NID);

prompt
prompt Creating table TB_MATCH_RESULT_VEHICLE
prompt ======================================
prompt
create table TB_MATCH_RESULT_VEHICLE
(
  nid         NUMBER not null,
  ncamera     NUMBER default 1,
  scarnumber  VARCHAR2(100),
  scolour     VARCHAR2(100),
  stype       VARCHAR2(100),
  sdirection  VARCHAR2(100),
  ndegree     NUMBER,
  dgettime    DATE,
  nflag       NUMBER default 0,
  dcreate     DATE default sysdate,
  bpicture    BLOB,
  sfile       VARCHAR2(260),
  scolor      VARCHAR2(100),
  nsize       NUMBER,
  scameraname VARCHAR2(100),
  sip         VARCHAR2(100)
)
;
comment on table TB_MATCH_RESULT_VEHICLE
  is '电动车报警表';
comment on column TB_MATCH_RESULT_VEHICLE.nid
  is '主键ID';
comment on column TB_MATCH_RESULT_VEHICLE.ncamera
  is '摄像头编号';
comment on column TB_MATCH_RESULT_VEHICLE.scarnumber
  is '电动车牌号';
comment on column TB_MATCH_RESULT_VEHICLE.scolour
  is '电动车车牌颜色';
comment on column TB_MATCH_RESULT_VEHICLE.stype
  is '电动车类型';
comment on column TB_MATCH_RESULT_VEHICLE.sdirection
  is '电动车方向';
comment on column TB_MATCH_RESULT_VEHICLE.ndegree
  is '置信度';
comment on column TB_MATCH_RESULT_VEHICLE.dgettime
  is '捕获时间';
comment on column TB_MATCH_RESULT_VEHICLE.nflag
  is '报警标志 1 报警 0 不报警';
comment on column TB_MATCH_RESULT_VEHICLE.dcreate
  is '入库时间';
comment on column TB_MATCH_RESULT_VEHICLE.bpicture
  is '照片';
comment on column TB_MATCH_RESULT_VEHICLE.sfile
  is '文件路径';
comment on column TB_MATCH_RESULT_VEHICLE.scolor
  is '颜色，明码';
comment on column TB_MATCH_RESULT_VEHICLE.nsize
  is '照片大小';
comment on column TB_MATCH_RESULT_VEHICLE.scameraname
  is '摄像头名称';
comment on column TB_MATCH_RESULT_VEHICLE.sip
  is '摄像头IP';
create index MATCH_RESULT_VEHICLE_INDEX on TB_MATCH_RESULT_VEHICLE (DCREATE);
alter table TB_MATCH_RESULT_VEHICLE
  add constraint MATCH_RESULT_VEHICLE_NID primary key (NID);

prompt
prompt Creating table TB_MISSION
prompt =========================
prompt
create table TB_MISSION
(
  nid      NUMBER not null,
  userid   NUMBER,
  detectid NUMBER,
  isplay   NUMBER,
  ncamera  NUMBER,
  flag     NUMBER default 0,
  indate   DATE default sysdate,
  message  VARCHAR2(256)
)
;
comment on table TB_MISSION
  is '任务表';
comment on column TB_MISSION.nid
  is '任务ID';
comment on column TB_MISSION.userid
  is '操作用户ID';
comment on column TB_MISSION.detectid
  is '识别服务器ID';
comment on column TB_MISSION.isplay
  is '对摄像头进行操作 0停止 1播放';
comment on column TB_MISSION.ncamera
  is '摄像头ID';
comment on column TB_MISSION.flag
  is '本数据读取标志 0未读取  1中转器已读取 2识别器已读取';
comment on column TB_MISSION.indate
  is '入库时间';
comment on column TB_MISSION.message
  is '分配器提示的消息';
alter table TB_MISSION
  add constraint MISSION_NID primary key (NID);

prompt
prompt Creating table TB_RECORD_PLAN
prompt =============================
prompt
create table TB_RECORD_PLAN
(
  nid         NUMBER not null,
  ncamera     NUMBER,
  scameraarea VARCHAR2(260),
  scameraname VARCHAR2(260),
  sip         VARCHAR2(32),
  dailyflag   NUMBER,
  dateflag    NUMBER,
  weekflag    NUMBER,
  enableflag  NUMBER,
  startweek   NUMBER,
  endweek     NUMBER,
  starttime   DATE,
  endtime     DATE
)
;
comment on table TB_RECORD_PLAN
  is '定时录制计划表';
comment on column TB_RECORD_PLAN.nid
  is '主键';
comment on column TB_RECORD_PLAN.ncamera
  is '摄像头ID  ';
comment on column TB_RECORD_PLAN.scameraarea
  is '摄像头区域  ';
comment on column TB_RECORD_PLAN.scameraname
  is '摄像头名称   ';
comment on column TB_RECORD_PLAN.sip
  is '摄像头ip地址   ';
comment on column TB_RECORD_PLAN.dailyflag
  is '日计划录制标志 ';
comment on column TB_RECORD_PLAN.dateflag
  is '时间段录制标志 ';
comment on column TB_RECORD_PLAN.weekflag
  is '周计划录制标志  ';
comment on column TB_RECORD_PLAN.enableflag
  is '启用录制标志 ';
comment on column TB_RECORD_PLAN.startweek
  is '周计划的开始   ';
comment on column TB_RECORD_PLAN.endweek
  is '周计划的结束  ';
comment on column TB_RECORD_PLAN.starttime
  is '开始时间    ';
comment on column TB_RECORD_PLAN.endtime
  is '结束时间    ';
alter table TB_RECORD_PLAN
  add constraint RECORD_PLAN_NID primary key (NID);

prompt
prompt Creating table TB_USER
prompt ======================
prompt
create table TB_USER
(
  suser             VARCHAR2(100),
  spwd              VARCHAR2(200),
  ndeviceallot      NUMBER,
  nphoto            NUMBER,
  nvideo            NUMBER,
  nview             NUMBER,
  nalarm            NUMBER,
  nyuntai           NUMBER,
  nparam            NUMBER,
  nvideoset         NUMBER,
  ncarnumber        NUMBER,
  nblack            NUMBER,
  nset              NUMBER,
  nvideoquery       NUMBER,
  nalarmquery       NUMBER,
  ndistinguishquery NUMBER,
  nlogquery         NUMBER,
  nmanager          NUMBER,
  nid               NUMBER not null,
  nlevel            NUMBER,
  ndetectlimit      NUMBER
)
;
comment on table TB_USER
  is '用户表';
comment on column TB_USER.suser
  is '用户名';
comment on column TB_USER.spwd
  is '密码';
comment on column TB_USER.ndeviceallot
  is '设备管理';
comment on column TB_USER.nphoto
  is '抓拍图片';
comment on column TB_USER.nvideo
  is '本地录像';
comment on column TB_USER.nview
  is '预览管理';
comment on column TB_USER.nalarm
  is '识别报警管理';
comment on column TB_USER.nyuntai
  is '云台设置';
comment on column TB_USER.nparam
  is '系统参数设置';
comment on column TB_USER.nvideoset
  is '录像设置';
comment on column TB_USER.ncarnumber
  is '车牌识别';
comment on column TB_USER.nblack
  is '设置
黑名单';
comment on column TB_USER.nset
  is '设置';
comment on column TB_USER.nvideoquery
  is '系统视频查询';
comment on column TB_USER.nalarmquery
  is '系统报警查询';
comment on column TB_USER.ndistinguishquery
  is '系统识别查询';
comment on column TB_USER.nlogquery
  is '系统日志查询';
comment on column TB_USER.nmanager
  is '管理员权限
';
comment on column TB_USER.nid
  is '关键字';
comment on column TB_USER.nlevel
  is '级别 值越小 权限越大';
comment on column TB_USER.ndetectlimit
  is '识别数量限制';
alter table TB_USER
  add constraint USER_NID primary key (NID);

prompt
prompt Creating table TB_VEHICLE_BLACK
prompt ===============================
prompt
create table TB_VEHICLE_BLACK
(
  nid        NUMBER not null,
  scarnumber VARCHAR2(100),
  sbrand     VARCHAR2(255),
  slostname  VARCHAR2(100),
  sphone     VARCHAR2(100),
  smark      VARCHAR2(1000)
)
;
comment on table TB_VEHICLE_BLACK
  is '电动车黑名单（比对源）';
comment on column TB_VEHICLE_BLACK.nid
  is '编号';
comment on column TB_VEHICLE_BLACK.scarnumber
  is '电动车车牌号';
comment on column TB_VEHICLE_BLACK.sbrand
  is '品牌';
comment on column TB_VEHICLE_BLACK.slostname
  is '失主姓名
';
comment on column TB_VEHICLE_BLACK.sphone
  is '联系电话
';
comment on column TB_VEHICLE_BLACK.smark
  is '其他信息
';
create index VEHICLE_BLACK_INDEX on TB_VEHICLE_BLACK (NID);
alter table TB_VEHICLE_BLACK
  add constraint VEHICLE_BLACK_NID primary key (NID);

prompt
prompt Creating table TB_VIDEO
prompt =======================
prompt
create table TB_VIDEO
(
  nid         NUMBER not null,
  scameraname VARCHAR2(260),
  sip         VARCHAR2(260),
  svenderid   NUMBER,
  dgettime    DATE default sysdate,
  sformat     VARCHAR2(20),
  sfilesize   NUMBER,
  dstarttime  DATE,
  dendtime    DATE,
  spath       VARCHAR2(260)
)
;
comment on table TB_VIDEO
  is '视频信息';
comment on column TB_VIDEO.nid
  is '编号';
comment on column TB_VIDEO.scameraname
  is '摄像头名称';
comment on column TB_VIDEO.sip
  is '摄像头IP';
comment on column TB_VIDEO.svenderid
  is '摄像机厂家ID';
comment on column TB_VIDEO.dgettime
  is ' 入库时间';
comment on column TB_VIDEO.sformat
  is '格式';
comment on column TB_VIDEO.sfilesize
  is '文件大小';
comment on column TB_VIDEO.dstarttime
  is '视频开始时间';
comment on column TB_VIDEO.dendtime
  is '视频结束时间';
comment on column TB_VIDEO.spath
  is '文件路径';
alter table TB_VIDEO
  add constraint VIDEO_NID primary key (NID);

prompt
prompt Creating table T_ZD_ORG
prompt =======================
prompt
create table T_ZD_ORG
(
  sorgid       VARCHAR2(30),
  sorgname     VARCHAR2(100),
  sorgnamelong VARCHAR2(200),
  sorgno       VARCHAR2(30),
  nid          NUMBER not null
)
;
comment on table T_ZD_ORG
  is '机构表';
comment on column T_ZD_ORG.sorgid
  is '机构ID';
comment on column T_ZD_ORG.sorgname
  is '机构名称';
comment on column T_ZD_ORG.sorgnamelong
  is '机构长名称';
comment on column T_ZD_ORG.sorgno
  is '代码简码，机构代码最后一位非0数字后面的0全删除';
comment on column T_ZD_ORG.nid
  is '关键字';
alter table T_ZD_ORG
  add constraint ORG_NID primary key (NID);

prompt
prompt Creating sequence SEQ_ALARM_CAR
prompt ===============================
prompt
create sequence SEQ_ALARM_CAR
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence SEQ_ALARM_VEHICLE
prompt ===================================
prompt
create sequence SEQ_ALARM_VEHICLE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence SEQ_BLACK_CAR
prompt ===============================
prompt
create sequence SEQ_BLACK_CAR
minvalue 1
maxvalue 9999999999999999999999999999
start with 1261
increment by 1
cache 20;

prompt
prompt Creating sequence SEQ_BLACK_VEHICLE
prompt ===================================
prompt
create sequence SEQ_BLACK_VEHICLE
minvalue 1
maxvalue 9999999999999999999999999999
start with 681
increment by 1
cache 20;

prompt
prompt Creating sequence SEQ_DETECT_DEVICE
prompt ===================================
prompt
create sequence SEQ_DETECT_DEVICE
minvalue 1
maxvalue 9999999999999999999999999999
start with 140
increment by 1
cache 20;

prompt
prompt Creating sequence SEQ_LOG
prompt =========================
prompt
create sequence SEQ_LOG
minvalue 1
maxvalue 9999999999999999999999999999
start with 90320
increment by 1
cache 20;

prompt
prompt Creating sequence SEQ_MATCH_RESULT_CAR
prompt ======================================
prompt
create sequence SEQ_MATCH_RESULT_CAR
minvalue 1
maxvalue 9999999999999999999999999999
start with 14261
increment by 1
cache 20;

prompt
prompt Creating sequence SEQ_MATCH_RESULT_VEHICLE
prompt ==========================================
prompt
create sequence SEQ_MATCH_RESULT_VEHICLE
minvalue 1
maxvalue 9999999999999999999999999999
start with 91587
increment by 1
cache 20;

prompt
prompt Creating sequence SEQ_MISSION
prompt =============================
prompt
create sequence SEQ_MISSION
minvalue 1
maxvalue 9999999999999999999999999999
start with 140
increment by 1
cache 20;

prompt
prompt Creating sequence SEQ_NCAMERA
prompt =============================
prompt
create sequence SEQ_NCAMERA
minvalue 1
maxvalue 9999999999999999999999999999
start with 401
increment by 1
cache 20;

prompt
prompt Creating sequence SEQ_RECORD_PLAN
prompt =================================
prompt
create sequence SEQ_RECORD_PLAN
minvalue 1
maxvalue 9999999999999999999999999999
start with 41
increment by 1
cache 20;

prompt
prompt Creating sequence SEQ_USER
prompt ==========================
prompt
create sequence SEQ_USER
minvalue 1
maxvalue 9999999999999999999999999999
start with 141
increment by 1
cache 20;

prompt
prompt Creating sequence SEQ_VIDEO
prompt ===========================
prompt
create sequence SEQ_VIDEO
minvalue 1
maxvalue 9999999999999999999999999999
start with 325
increment by 1
cache 20;

prompt
prompt Creating sequence SEQ_ZD_ORG
prompt ============================
prompt
create sequence SEQ_ZD_ORG
minvalue 1
maxvalue 9999999999999999999999999999
start with 201
increment by 1
cache 20;

prompt
prompt Creating view DEVICELIST_VIEW
prompt =============================
prompt
create or replace view devicelist_view as
select a."NID",a."SIP",a."ISOCCUPY",a."CHANNELNUM",a."ISENABLE",a."MISSIONID",b.userid,b.isplay,to_char(b.indate) cindate,c.ncamera,c.scameraname,c.sipserver camip,c.rtspurl,c.isplay camisplay,d.suser
                 from TB_DETECT_SERVER a
                 left join tb_mission b
                 on a.missionid = b.nid
                 left join tb_device c
                 on a.nid =c.DETECTID
                  left join tb_user d
                 on d.nid = b.userid
                 order by nid desc;

prompt
prompt Creating procedure PRO_AUTO_MATCH_CAR
prompt =====================================
prompt
create or replace procedure pro_auto_match_car(i_nid        integer, --比对结果表ID值
                                               o_cnt        out integer, --比对成功标志，1成功0不成功
                                               o_result     out integer, --执行结果代码，1成功0失败
                                               o_errmessage out VARCHAR2 --错误信息，执行成功则为空
                                               ) is
  /*
    author: zzh
   created:20130327
   purpose:自动识别车辆，该功能由C++调用
   Logic  :和车辆黑名单比对，车牌号一样，则入报警库,并把标志位置1.
  修改记录：
   */

begin
  o_result := 0;

  insert into tb_alarm_car
    (nid,
     SCARNUMBER,
     BPICTURE,
     DCREATE,
     SFILE,
     DGETTIME,
     NDEGREE,
     SCOLOUR,
     SDIRECTION,
     STYPE,
     sip,
     scameraname,
     nsize,
     scolor,
     ncamera,
     blackid)

    select seq_alarm_car.nextval,
           a.SCARNUMBER,
           BPICTURE,
           DCREATE,
           SFILE,
           DGETTIME,
           NDEGREE,
           SCOLOUR,
           SDIRECTION,
           STYPE,
           sip,
           scameraname,
           nsize,
           scolor,
           ncamera,
           b.nid
      from tb_match_result_car a, tb_car_black b
     where a.scarnumber = b.scarnumber
       and a.nid = i_nid;
  if sql%rowcount > 0 then
    o_cnt := 1;
  else
    o_cnt := 0;
  end if;
  --把nflag置1
  update TB_MATCH_RESULT_car set nflag = o_cnt where NID = i_NID;
  commit;
  o_result := 1;
exception
  when others then
    o_result     := 0;
    o_errmessage := sqlerrm;
    rollback;
    --错误日志记载
end pro_auto_match_car;
/

prompt
prompt Creating procedure PRO_AUTO_MATCH_VEHICLE
prompt =========================================
prompt
create or replace procedure pro_auto_match_VEHICLE(i_nid        integer, --比对结果表ID值
                                                   o_cnt        out integer, --比对成功标志，1成功0不成功
                                                   o_result     out integer, --执行结果代码，1成功0失败
                                                   o_errmessage out VARCHAR2 --错误信息，执行成功则为空
                                                   ) is
  /*
    author: zzh
   created:20130409
   purpose:自动识别电动，该功能由C++调用
   Logic  :和车辆黑名单比对，车牌号一样，则入报警库,并把标志位置1.
  修改记录：
   */

begin
  o_result := 0;

  insert into tb_alarm_VEHICLE
    (nid,
     SCARNUMBER,
     BPICTURE,
     DCREATE,
     SFILE,
     DGETTIME,
     NDEGREE,
     SCOLOUR,
     SDIRECTION,
     STYPE,
     sip,
     scameraname,
     nsize,
     scolor,
     ncamera,
     blackid)

    select SEQ_ALARM_VEHICLE.Nextval,
           a.SCARNUMBER,
           BPICTURE,
           DCREATE,
           SFILE,
           DGETTIME,
           NDEGREE,
           SCOLOUR,
           SDIRECTION,
           STYPE,
           sip,
           scameraname,
           nsize,
           scolor,
           ncamera,
           b.nid
      from tb_match_result_VEHICLE a, tb_VEHICLE_black b
     where a.scarnumber = b.scarnumber
       and a.nid = i_nid;
  if sql%rowcount > 0 then
    o_cnt := 1;
  else
    o_cnt := 0;
  end if;
  --把nflag置1
  update tb_match_result_VEHICLE set nflag = o_cnt where NID = i_NID;
  commit;
  o_result := 1;
exception
  when others then
    o_result     := 0;
    o_errmessage := sqlerrm;
    rollback;
    --错误日志记载
end pro_auto_match_VEHICLE;
/


spool off
