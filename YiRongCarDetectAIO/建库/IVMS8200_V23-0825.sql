----------------------------------------------------
-- Export file for user IVMS8200_V23              --
-- Created by Administrator on 2014/8/25, 9:39:35 --
----------------------------------------------------

spool IVMS8200_V23-0825.log

prompt
prompt Creating table CONTROL_UNIT
prompt ===========================
prompt
create table CONTROL_UNIT
(
  control_unit_id   NUMBER not null,
  index_code        VARCHAR2(64) not null,
  name              VARCHAR2(128) not null,
  parent_id         NUMBER not null,
  cms_cascade_id    NUMBER,
  cms_cascade_type  NUMBER,
  unit_level        NUMBER not null,
  use_stream_sv     NUMBER(1) not null,
  stream_sv_id      NUMBER,
  description       VARCHAR2(256),
  sequence_idx      NUMBER not null,
  operator_id       NUMBER not null,
  create_time       TIMESTAMP(6) not null,
  update_time       TIMESTAMP(6) not null,
  int_rev_1         NUMBER,
  int_rev_2         NUMBER,
  str_rev_1         VARCHAR2(64),
  str_rev_2         VARCHAR2(128),
  parent_tree       VARCHAR2(256),
  cascade_share_ids VARCHAR2(256)
)
;
comment on column CONTROL_UNIT.name
  is '区域名称';
comment on column CONTROL_UNIT.parent_id
  is '上级域的ID,0表示根域';
comment on column CONTROL_UNIT.cms_cascade_id
  is '级联ID';
comment on column CONTROL_UNIT.cms_cascade_type
  is '级联服务器所属平台0：公安专网1：监控专网';
comment on column CONTROL_UNIT.unit_level
  is '控制单元所属的级别，根单元为0，其他表示所属的层次';
comment on column CONTROL_UNIT.use_stream_sv
  is '是否使用流媒体';
comment on column CONTROL_UNIT.description
  is '备注信息';
comment on column CONTROL_UNIT.operator_id
  is '操作员ID';
comment on column CONTROL_UNIT.create_time
  is '创建时间';
comment on column CONTROL_UNIT.int_rev_1
  is '整型保留字段';
comment on column CONTROL_UNIT.int_rev_2
  is '整型保留字段';
comment on column CONTROL_UNIT.str_rev_1
  is '字符串保留字段';
comment on column CONTROL_UNIT.str_rev_2
  is '字符串保留字段';
comment on column CONTROL_UNIT.parent_tree
  is '组织结构字符串';
comment on column CONTROL_UNIT.cascade_share_ids
  is '共享平台编号';
create index IDX_CU_PARENT_ID on CONTROL_UNIT (PARENT_ID);
alter table CONTROL_UNIT
  add constraint PK_CONTROL_UNIT primary key (CONTROL_UNIT_ID);
alter table CONTROL_UNIT
  add constraint UK_CONTROL_UNIT unique (INDEX_CODE);

prompt
prompt Creating table ALARM_DEVICE
prompt ===========================
prompt
create table ALARM_DEVICE
(
  alarm_dev_id    NUMBER not null,
  control_unit_id NUMBER not null,
  type_code       NUMBER not null,
  index_code      VARCHAR2(64) not null,
  name            VARCHAR2(64) not null,
  input_num       NUMBER not null,
  output_num      NUMBER not null,
  connect_type    NUMBER not null,
  ip_addr         VARCHAR2(16) not null,
  port            NUMBER not null,
  dev_username    VARCHAR2(16),
  dev_psw         VARCHAR2(16),
  sequence_idx    NUMBER not null,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column ALARM_DEVICE.control_unit_id
  is '控制单元ID';
comment on column ALARM_DEVICE.type_code
  is '设备类型编号';
comment on column ALARM_DEVICE.index_code
  is '索引编号';
comment on column ALARM_DEVICE.name
  is '服务器名称';
comment on column ALARM_DEVICE.port
  is '控制端口';
comment on column ALARM_DEVICE.sequence_idx
  is '排序索引';
comment on column ALARM_DEVICE.operator_id
  is '操作员ID';
comment on column ALARM_DEVICE.create_time
  is '创建时间';
comment on column ALARM_DEVICE.int_rev_1
  is '整型保留字段';
comment on column ALARM_DEVICE.int_rev_2
  is '整型保留字段';
comment on column ALARM_DEVICE.str_rev_1
  is '字符串保留字段';
comment on column ALARM_DEVICE.str_rev_2
  is '字符串保留字段';
create index INDX_AD_CONTROL_UNIT_ID on ALARM_DEVICE (CONTROL_UNIT_ID);
alter table ALARM_DEVICE
  add constraint PK_ALARM_DEVICE primary key (ALARM_DEV_ID);
alter table ALARM_DEVICE
  add constraint UK_ALARM_DEVICE unique (INDEX_CODE);
alter table ALARM_DEVICE
  add constraint FK_ALARM_DEVICE foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table ALARM_CHANNEL_INFO
prompt =================================
prompt
create table ALARM_CHANNEL_INFO
(
  alarm_chan_id NUMBER not null,
  alarm_dev_id  NUMBER not null,
  index_code    VARCHAR2(64),
  name          VARCHAR2(64) not null,
  region_id     NUMBER not null,
  channel       NUMBER not null,
  type          NUMBER not null,
  operator_id   NUMBER not null,
  create_time   TIMESTAMP(6) not null,
  update_time   TIMESTAMP(6) not null,
  int_rev_1     NUMBER,
  int_rev_2     NUMBER,
  str_rev_1     VARCHAR2(64),
  str_rev_2     VARCHAR2(128)
)
;
comment on column ALARM_CHANNEL_INFO.alarm_dev_id
  is '报警设备ID';
comment on column ALARM_CHANNEL_INFO.operator_id
  is '操作员ID';
comment on column ALARM_CHANNEL_INFO.create_time
  is '创建时间';
comment on column ALARM_CHANNEL_INFO.int_rev_1
  is '整型保留字段';
comment on column ALARM_CHANNEL_INFO.int_rev_2
  is '整型保留字段';
comment on column ALARM_CHANNEL_INFO.str_rev_1
  is '字符串保留字段';
comment on column ALARM_CHANNEL_INFO.str_rev_2
  is '字符串保留字段';
create index INDX_ACI_ALARM_DEV_ID on ALARM_CHANNEL_INFO (ALARM_DEV_ID);
alter table ALARM_CHANNEL_INFO
  add constraint PK_ALARM_CHANNEL_INFO primary key (ALARM_CHAN_ID);
alter table ALARM_CHANNEL_INFO
  add constraint UK_ALARM_CHANNEL_INFO unique (INDEX_CODE);
alter table ALARM_CHANNEL_INFO
  add constraint FK_ALARM_CHANNEL_INFO foreign key (ALARM_DEV_ID)
  references ALARM_DEVICE (ALARM_DEV_ID);

prompt
prompt Creating table ALARM_EVENT
prompt ==========================
prompt
create table ALARM_EVENT
(
  event_id        NUMBER not null,
  event_name      VARCHAR2(128) not null,
  event_level     NUMBER not null,
  sched_templ_id  NUMBER not null,
  region_id       NUMBER not null,
  control_unit_id NUMBER not null,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column ALARM_EVENT.event_name
  is '报警事件名称';
comment on column ALARM_EVENT.event_level
  is '报警事件级别';
comment on column ALARM_EVENT.control_unit_id
  is '报警事件所属的控制单元';
comment on column ALARM_EVENT.operator_id
  is '操作员ID';
comment on column ALARM_EVENT.create_time
  is '创建时间';
comment on column ALARM_EVENT.int_rev_1
  is '整型保留字段';
comment on column ALARM_EVENT.int_rev_2
  is '整型保留字段';
comment on column ALARM_EVENT.str_rev_1
  is '字符串保留字段';
comment on column ALARM_EVENT.str_rev_2
  is '字符串保留字段';
alter table ALARM_EVENT
  add constraint PK_ALARM_EVENT primary key (EVENT_ID);

prompt
prompt Creating table ALARM_INPUT_TYPE
prompt ===============================
prompt
create table ALARM_INPUT_TYPE
(
  input_type  NUMBER not null,
  input_name  VARCHAR2(64) not null,
  create_time TIMESTAMP(6) not null,
  int_rev_1   NUMBER,
  int_rev_2   NUMBER,
  str_rev_1   VARCHAR2(64),
  str_rev_2   VARCHAR2(128)
)
;
comment on column ALARM_INPUT_TYPE.input_name
  is '报警输入信号类别名称';
comment on column ALARM_INPUT_TYPE.create_time
  is '创建时间';
comment on column ALARM_INPUT_TYPE.int_rev_1
  is '整型保留字段';
comment on column ALARM_INPUT_TYPE.int_rev_2
  is '整型保留字段';
comment on column ALARM_INPUT_TYPE.str_rev_1
  is '字符串保留字段';
comment on column ALARM_INPUT_TYPE.str_rev_2
  is '字符串保留字段';
alter table ALARM_INPUT_TYPE
  add constraint PK_ALARM_INPUT_TYPE primary key (INPUT_TYPE);

prompt
prompt Creating table ALARM_EVENT_INPUT
prompt ================================
prompt
create table ALARM_EVENT_INPUT
(
  alarm_event_input_id NUMBER not null,
  alarm_event_id       NUMBER not null,
  alarm_input_type     NUMBER not null,
  alarm_input_source   NUMBER not null,
  operator_id          NUMBER not null,
  create_time          TIMESTAMP(6) not null,
  update_time          TIMESTAMP(6) not null,
  int_rev_1            NUMBER,
  int_rev_2            NUMBER,
  str_rev_1            VARCHAR2(64),
  str_rev_2            VARCHAR2(128)
)
;
comment on column ALARM_EVENT_INPUT.alarm_event_id
  is '报警事件编号';
comment on column ALARM_EVENT_INPUT.alarm_input_type
  is '报警输入类型，对应于 alarm_input_code 表中定义的类型';
comment on column ALARM_EVENT_INPUT.alarm_input_source
  is '报警输入来源（如：camera_info/io_info/alarm_a8_info 的编号）';
comment on column ALARM_EVENT_INPUT.operator_id
  is '操作员ID';
comment on column ALARM_EVENT_INPUT.create_time
  is '创建时间';
comment on column ALARM_EVENT_INPUT.int_rev_1
  is '整型保留字段';
comment on column ALARM_EVENT_INPUT.int_rev_2
  is '整型保留字段';
comment on column ALARM_EVENT_INPUT.str_rev_1
  is '字符串保留字段';
comment on column ALARM_EVENT_INPUT.str_rev_2
  is '字符串保留字段';
create index IDX_AEI_ALARM_INPUT_SOURCE on ALARM_EVENT_INPUT (ALARM_INPUT_SOURCE);
create index INDX_AEI_ALARM_EVENT_ID on ALARM_EVENT_INPUT (ALARM_EVENT_ID);
create index INDX_AEI_ALARM_INPUT_TYPE on ALARM_EVENT_INPUT (ALARM_INPUT_TYPE);
alter table ALARM_EVENT_INPUT
  add constraint PK_ALARM_EVENT_INPUT primary key (ALARM_EVENT_INPUT_ID);
alter table ALARM_EVENT_INPUT
  add constraint FK_ALARM_EVENT_INPUT1 foreign key (ALARM_EVENT_ID)
  references ALARM_EVENT (EVENT_ID);
alter table ALARM_EVENT_INPUT
  add constraint FK_ALARM_EVENT_INPUT2 foreign key (ALARM_INPUT_TYPE)
  references ALARM_INPUT_TYPE (INPUT_TYPE);

prompt
prompt Creating table ALARM_HOST_INFO
prompt ==============================
prompt
create table ALARM_HOST_INFO
(
  device_id              NUMBER not null,
  control_unit_id        NUMBER,
  index_code             VARCHAR2(64),
  type_code              NUMBER,
  rs485_count            NUMBER,
  name                   VARCHAR2(64),
  register_type          NUMBER,
  network_addr           VARCHAR2(128),
  network_port           NUMBER,
  user_name              VARCHAR2(64),
  user_pwd               VARCHAR2(64),
  dns_addr               VARCHAR2(128),
  dns_port               NUMBER,
  serial_no              VARCHAR2(64),
  alarm_in_total_count   NUMBER,
  alarm_in_local_count   NUMBER,
  alarm_in_expend_count  NUMBER,
  alarm_out_total_count  NUMBER,
  alarm_out_local_count  NUMBER,
  alarm_out_expend_count NUMBER,
  sequence_idx           NUMBER,
  status                 NUMBER,
  operator_id            NUMBER,
  matrix_id              NUMBER,
  create_time            TIMESTAMP(6),
  update_time            TIMESTAMP(6),
  int_rev_1              NUMBER,
  int_rev_2              NUMBER,
  str_rev_1              VARCHAR2(64),
  str_rev_2              VARCHAR2(128),
  update_state           NUMBER,
  pag_server_id          NUMBER
)
;
comment on column ALARM_HOST_INFO.control_unit_id
  is '所属的控制单元';
comment on column ALARM_HOST_INFO.index_code
  is '索引编号';
comment on column ALARM_HOST_INFO.type_code
  is '类型编号';
comment on column ALARM_HOST_INFO.rs485_count
  is '485口数';
comment on column ALARM_HOST_INFO.name
  is '设备名称';
comment on column ALARM_HOST_INFO.register_type
  is '注册类型,0-普通/1-私有域名/2-普通域名';
comment on column ALARM_HOST_INFO.network_addr
  is '设备的IP地址';
comment on column ALARM_HOST_INFO.user_name
  is '设备登录的用户名';
comment on column ALARM_HOST_INFO.user_pwd
  is '用户密码';
comment on column ALARM_HOST_INFO.dns_addr
  is 'dns的IP地址';
comment on column ALARM_HOST_INFO.serial_no
  is '设备序列号,用来在使用IP-Server时查询';
comment on column ALARM_HOST_INFO.alarm_in_total_count
  is '报警输入总通道数';
comment on column ALARM_HOST_INFO.alarm_in_local_count
  is '本地报警输入通道数';
comment on column ALARM_HOST_INFO.alarm_in_expend_count
  is '扩展报警输入通道数';
comment on column ALARM_HOST_INFO.alarm_out_total_count
  is '报警输出总通道数';
comment on column ALARM_HOST_INFO.alarm_out_local_count
  is '本地报警输出通道数';
comment on column ALARM_HOST_INFO.alarm_out_expend_count
  is '扩展报警输出通道数';
comment on column ALARM_HOST_INFO.status
  is '设备状态 1：在线 2：不在线';
comment on column ALARM_HOST_INFO.operator_id
  is '操作员ID';
comment on column ALARM_HOST_INFO.matrix_id
  is '矩阵ID';
comment on column ALARM_HOST_INFO.create_time
  is '创建时间';
comment on column ALARM_HOST_INFO.int_rev_1
  is '整型保留字段';
comment on column ALARM_HOST_INFO.int_rev_2
  is '整型保留字段';
comment on column ALARM_HOST_INFO.str_rev_1
  is '字符串保留字段';
comment on column ALARM_HOST_INFO.str_rev_2
  is '字符串保留字段';
comment on column ALARM_HOST_INFO.update_state
  is '升级状态';
comment on column ALARM_HOST_INFO.pag_server_id
  is '关联设备代理服务器id';
alter table ALARM_HOST_INFO
  add constraint PK_ALARM_HOST_INFO primary key (DEVICE_ID);
alter table ALARM_HOST_INFO
  add constraint UK_ALARM_HOST_INFO unique (INDEX_CODE);
alter table ALARM_HOST_INFO
  add constraint FK_ALARM_HOST_INFO foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table ALARM_HOST_CHANNEL_INFO
prompt ======================================
prompt
create table ALARM_HOST_CHANNEL_INFO
(
  alarm_chan_id NUMBER not null,
  alarm_host_id NUMBER not null,
  index_code    VARCHAR2(64),
  name          VARCHAR2(64) not null,
  region_id     NUMBER,
  channel       NUMBER not null,
  type          NUMBER not null,
  sub_type      NUMBER not null,
  operator_id   NUMBER not null,
  create_time   TIMESTAMP(6) not null,
  update_time   TIMESTAMP(6) not null,
  int_rev_1     NUMBER,
  int_rev_2     NUMBER,
  str_rev_1     VARCHAR2(64),
  str_rev_2     VARCHAR2(128)
)
;
comment on column ALARM_HOST_CHANNEL_INFO.alarm_host_id
  is '报警设备ID';
comment on column ALARM_HOST_CHANNEL_INFO.operator_id
  is '操作员ID';
comment on column ALARM_HOST_CHANNEL_INFO.create_time
  is '创建时间';
comment on column ALARM_HOST_CHANNEL_INFO.int_rev_1
  is '整型保留字段';
comment on column ALARM_HOST_CHANNEL_INFO.int_rev_2
  is '整型保留字段';
comment on column ALARM_HOST_CHANNEL_INFO.str_rev_1
  is '字符串保留字段';
comment on column ALARM_HOST_CHANNEL_INFO.str_rev_2
  is '字符串保留字段';
alter table ALARM_HOST_CHANNEL_INFO
  add constraint PK_ALARM_HOST_CHANNEL_INFO primary key (ALARM_CHAN_ID);
alter table ALARM_HOST_CHANNEL_INFO
  add constraint UK_ALARM_HOST_CHANNEL_INFO unique (INDEX_CODE);
alter table ALARM_HOST_CHANNEL_INFO
  add constraint FK_ALARM_HOST_CHANNEL_INFO foreign key (ALARM_HOST_ID)
  references ALARM_HOST_INFO (DEVICE_ID);

prompt
prompt Creating table ALARM_LOG
prompt ========================
prompt
create table ALARM_LOG
(
  alarm_log_id       VARCHAR2(64) not null,
  control_unit_id    NUMBER not null,
  alarm_event_id     NUMBER not null,
  alarm_input_source NUMBER not null,
  alarm_input_type   NUMBER not null,
  alarm_event_name   VARCHAR2(128) not null,
  alarm_start_time   TIMESTAMP(6) not null,
  alarm_stop_time    TIMESTAMP(6),
  trigger_record     NUMBER(1) not null,
  log_txt            VARCHAR2(1024),
  log_remark         VARCHAR2(1024),
  int_rev_1          NUMBER,
  int_rev_2          NUMBER,
  str_rev_1          VARCHAR2(64),
  str_rev_2          VARCHAR2(128)
)
;
comment on column ALARM_LOG.control_unit_id
  is '报警事件所在的控制单元的编号';
comment on column ALARM_LOG.alarm_event_id
  is '报警事件编号';
comment on column ALARM_LOG.alarm_input_source
  is '报警输入资源';
comment on column ALARM_LOG.alarm_input_type
  is '报警输入类型';
comment on column ALARM_LOG.alarm_event_name
  is '报警事件名称';
comment on column ALARM_LOG.alarm_start_time
  is '报警开始时间';
comment on column ALARM_LOG.alarm_stop_time
  is '报警结束时间';
comment on column ALARM_LOG.trigger_record
  is '报警事件是否联动录像';
comment on column ALARM_LOG.log_txt
  is '报警日志内容';
comment on column ALARM_LOG.log_remark
  is '报警日志附加说明';
comment on column ALARM_LOG.int_rev_1
  is '整型保留字段';
comment on column ALARM_LOG.int_rev_2
  is '整型保留字段';
comment on column ALARM_LOG.str_rev_1
  is '字符串保留字段';
comment on column ALARM_LOG.str_rev_2
  is '字符串保留字段';
create index IDX_ALARM_CONTROL_ID on ALARM_LOG (CONTROL_UNIT_ID);
create index IDX_ALARM_INPUT_SOURCE on ALARM_LOG (ALARM_INPUT_SOURCE);
create index IDX_ALARM_INPUT_TYPE on ALARM_LOG (ALARM_INPUT_TYPE);
create index IDX_ALARM_START_TIME on ALARM_LOG (ALARM_START_TIME);
alter table ALARM_LOG
  add constraint PK_ALARM_LOG primary key (ALARM_LOG_ID);

prompt
prompt Creating table ALARM_MSG
prompt ========================
prompt
create table ALARM_MSG
(
  id              NUMBER not null,
  alarm_info      VARCHAR2(255),
  alarm_level     NUMBER,
  alarm_time      TIMESTAMP(6),
  alarm_type      VARCHAR2(255),
  control_unit_id NUMBER,
  ip              VARCHAR2(255),
  name            VARCHAR2(255),
  object_id       NUMBER,
  object_type     NUMBER,
  state_type      NUMBER,
  update_time     TIMESTAMP(6)
)
;
alter table ALARM_MSG
  add constraint PK_ALARM_MSG primary key (ID);

prompt
prompt Creating table SERVER_INFO
prompt ==========================
prompt
create table SERVER_INFO
(
  server_id       NUMBER not null,
  bas_type        NUMBER,
  cms_cascade_id  NUMBER,
  control_port    NUMBER,
  control_unit_id NUMBER,
  data_port       NUMBER,
  index_code      VARCHAR2(60),
  net_port        NUMBER,
  net_zone_id     NUMBER,
  ip_addr         VARCHAR2(64),
  name            VARCHAR2(60),
  hpp_port        NUMBER,
  serverres1      NUMBER,
  serverres2      VARCHAR2(60),
  server_type     NUMBER,
  share_flag      NUMBER
)
;
alter table SERVER_INFO
  add constraint PK_SERVER_INFO primary key (SERVER_ID);
alter table SERVER_INFO
  add constraint UK_SERVER_INFO unique (INDEX_CODE);
alter table SERVER_INFO
  add constraint FK_SERVER_INFO foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table ALARM_SERVER
prompt ===========================
prompt
create table ALARM_SERVER
(
  alarm_sv_id     NUMBER not null,
  control_unit_id NUMBER not null,
  server_id       NUMBER,
  index_code      VARCHAR2(64) not null,
  name            VARCHAR2(64) not null,
  ip_addr         VARCHAR2(16) not null,
  second_ip       VARCHAR2(16),
  net_zone_id     NUMBER not null,
  ctrl_port       NUMBER not null,
  work_mode       NUMBER(5) not null,
  net_agent_port  NUMBER,
  notify_way      NUMBER,
  sms_com         VARCHAR2(16),
  use_soft_dog    NUMBER(1) not null,
  sequence_idx    NUMBER not null,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column ALARM_SERVER.control_unit_id
  is '控制单元ID';
comment on column ALARM_SERVER.server_id
  is 'serverId';
comment on column ALARM_SERVER.index_code
  is '索引编号';
comment on column ALARM_SERVER.name
  is '服务器名称';
comment on column ALARM_SERVER.net_zone_id
  is 'net zone id';
comment on column ALARM_SERVER.ctrl_port
  is '控制端口';
comment on column ALARM_SERVER.work_mode
  is '工作模式（0-监听，1-布防）';
comment on column ALARM_SERVER.net_agent_port
  is '网管端口';
comment on column ALARM_SERVER.notify_way
  is '0-直连转发 1-MQ转发';
comment on column ALARM_SERVER.sms_com
  is '短信串口';
comment on column ALARM_SERVER.use_soft_dog
  is '是否启用软件狗';
comment on column ALARM_SERVER.operator_id
  is '操作员ID';
comment on column ALARM_SERVER.create_time
  is '创建时间';
comment on column ALARM_SERVER.int_rev_1
  is '整型保留字段';
comment on column ALARM_SERVER.int_rev_2
  is '整型保留字段';
comment on column ALARM_SERVER.str_rev_1
  is '字符串保留字段';
comment on column ALARM_SERVER.str_rev_2
  is 'E家设备对接IP群以","隔开';
create index INDX_AS_CONTROL_UNIT_ID on ALARM_SERVER (CONTROL_UNIT_ID);
alter table ALARM_SERVER
  add constraint PK_ALARM_SERVER primary key (ALARM_SV_ID);
alter table ALARM_SERVER
  add constraint UK_ALARM_SERVER unique (INDEX_CODE);
alter table ALARM_SERVER
  add constraint FK_ALARM_SERVER1 foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);
alter table ALARM_SERVER
  add constraint FK_ALARM_SERVER2 foreign key (SERVER_ID)
  references SERVER_INFO (SERVER_ID);

prompt
prompt Creating table ALARM_SNAPSHOT_INFO
prompt ==================================
prompt
create table ALARM_SNAPSHOT_INFO
(
  id           NUMBER not null,
  alarm_log_id VARCHAR2(64) not null,
  pic_url      VARCHAR2(255)
)
;
comment on column ALARM_SNAPSHOT_INFO.alarm_log_id
  is '报警日志ID';
comment on column ALARM_SNAPSHOT_INFO.pic_url
  is '图片路径';
create index IDX_SNAPSHOT_ALARM_LOG_ID on ALARM_SNAPSHOT_INFO (ALARM_LOG_ID, ID);
alter table ALARM_SNAPSHOT_INFO
  add constraint PK_ALARM_SNAPSHOT_INFO primary key (ID);

prompt
prompt Creating table ALARM_TRIGGER_CLIENT
prompt ===================================
prompt
create table ALARM_TRIGGER_CLIENT
(
  trigger_client_id  NUMBER not null,
  alarm_event_id     NUMBER not null,
  valid_flag         NUMBER(1) not null,
  talk_flag          NUMBER(1) not null,
  talk_device_id     NUMBER,
  talk_chan          NUMBER,
  sound_flag         NUMBER(1) not null,
  sound_text         VARCHAR2(128),
  words_overlay_flag NUMBER(1) not null,
  words_overlay_txt  VARCHAR2(128),
  promt              NUMBER(1) not null,
  operator_id        NUMBER not null,
  create_time        TIMESTAMP(6) not null,
  update_time        TIMESTAMP(6) not null,
  int_rev_1          NUMBER,
  int_rev_2          NUMBER,
  str_rev_1          VARCHAR2(64),
  str_rev_2          VARCHAR2(128)
)
;
comment on column ALARM_TRIGGER_CLIENT.alarm_event_id
  is '报警事件编号';
comment on column ALARM_TRIGGER_CLIENT.valid_flag
  is '该联动是否有效的标志';
comment on column ALARM_TRIGGER_CLIENT.talk_flag
  is '是否对讲';
comment on column ALARM_TRIGGER_CLIENT.talk_device_id
  is '对讲设备序号';
comment on column ALARM_TRIGGER_CLIENT.talk_chan
  is '对讲通道，从1开始';
comment on column ALARM_TRIGGER_CLIENT.sound_flag
  is '是否声音报警的标示';
comment on column ALARM_TRIGGER_CLIENT.sound_text
  is '发声字符';
comment on column ALARM_TRIGGER_CLIENT.words_overlay_flag
  is '字符叠加的标示';
comment on column ALARM_TRIGGER_CLIENT.words_overlay_txt
  is '字符叠加文字';
comment on column ALARM_TRIGGER_CLIENT.promt
  is '是否允许用户手动干预（即在联动之前给予用户报警联动提示）';
comment on column ALARM_TRIGGER_CLIENT.operator_id
  is '操作员ID';
comment on column ALARM_TRIGGER_CLIENT.create_time
  is '创建时间';
comment on column ALARM_TRIGGER_CLIENT.int_rev_1
  is '整型保留字段';
comment on column ALARM_TRIGGER_CLIENT.int_rev_2
  is '整型保留字段';
comment on column ALARM_TRIGGER_CLIENT.str_rev_1
  is '字符串保留字段';
comment on column ALARM_TRIGGER_CLIENT.str_rev_2
  is '字符串保留字段';
create index INDX_CLI_ALARM_EVENT_ID on ALARM_TRIGGER_CLIENT (ALARM_EVENT_ID);
alter table ALARM_TRIGGER_CLIENT
  add constraint PK_ALARM_TRIGGER_CLIENT primary key (TRIGGER_CLIENT_ID);
alter table ALARM_TRIGGER_CLIENT
  add constraint FK_ALARM_TRIGGER_CLIENT foreign key (ALARM_EVENT_ID)
  references ALARM_EVENT (EVENT_ID);

prompt
prompt Creating table ALARM_TRIGGER_EMAIL
prompt ==================================
prompt
create table ALARM_TRIGGER_EMAIL
(
  trigger_email_id NUMBER not null,
  alarm_event_id   NUMBER not null,
  valid_flag       NUMBER(1) not null,
  user_id          NUMBER not null,
  email_title      VARCHAR2(64),
  email_content    VARCHAR2(1024),
  operator_id      NUMBER not null,
  create_time      TIMESTAMP(6) not null,
  update_time      TIMESTAMP(6) not null,
  int_rev_1        NUMBER,
  int_rev_2        NUMBER,
  str_rev_1        VARCHAR2(64),
  str_rev_2        VARCHAR2(128)
)
;
comment on column ALARM_TRIGGER_EMAIL.alarm_event_id
  is '报警事件编号';
comment on column ALARM_TRIGGER_EMAIL.user_id
  is '接收联动邮件的用户的编号';
comment on column ALARM_TRIGGER_EMAIL.operator_id
  is '操作员ID';
comment on column ALARM_TRIGGER_EMAIL.create_time
  is '创建时间';
comment on column ALARM_TRIGGER_EMAIL.int_rev_1
  is '整型保留字段';
comment on column ALARM_TRIGGER_EMAIL.int_rev_2
  is '整型保留字段';
comment on column ALARM_TRIGGER_EMAIL.str_rev_1
  is '字符串保留字段';
comment on column ALARM_TRIGGER_EMAIL.str_rev_2
  is '字符串保留字段';
create index INDX_TE_ALARM_EVENT_ID on ALARM_TRIGGER_EMAIL (ALARM_EVENT_ID);
alter table ALARM_TRIGGER_EMAIL
  add constraint PK_ALARM_TRIGGER_EMAIL primary key (TRIGGER_EMAIL_ID);
alter table ALARM_TRIGGER_EMAIL
  add constraint FK_ALARM_TRIGGER_EMAIL foreign key (ALARM_EVENT_ID)
  references ALARM_EVENT (EVENT_ID);

prompt
prompt Creating table ALARM_TRIGGER_IO
prompt ===============================
prompt
create table ALARM_TRIGGER_IO
(
  trigger_io_id  NUMBER not null,
  alarm_event_id NUMBER not null,
  valid_flag     NUMBER(1) not null,
  io_type        NUMBER,
  io_id          NUMBER not null,
  io_state       NUMBER(1) not null,
  operator_id    NUMBER not null,
  create_time    TIMESTAMP(6) not null,
  update_time    TIMESTAMP(6) not null,
  int_rev_1      NUMBER,
  int_rev_2      NUMBER,
  str_rev_1      VARCHAR2(64),
  str_rev_2      VARCHAR2(128)
)
;
comment on column ALARM_TRIGGER_IO.alarm_event_id
  is '报警事件编号';
comment on column ALARM_TRIGGER_IO.io_type
  is 'IO Type';
comment on column ALARM_TRIGGER_IO.io_id
  is '联动目标IO';
comment on column ALARM_TRIGGER_IO.io_state
  is '联动目标 IO 的状态';
comment on column ALARM_TRIGGER_IO.operator_id
  is '操作员ID';
comment on column ALARM_TRIGGER_IO.create_time
  is '创建时间';
comment on column ALARM_TRIGGER_IO.int_rev_1
  is '整型保留字段';
comment on column ALARM_TRIGGER_IO.int_rev_2
  is '整型保留字段';
comment on column ALARM_TRIGGER_IO.str_rev_1
  is '字符串保留字段';
comment on column ALARM_TRIGGER_IO.str_rev_2
  is '字符串保留字段';
create index INDX_ATI_ALARM_EVENT_ID on ALARM_TRIGGER_IO (ALARM_EVENT_ID);
alter table ALARM_TRIGGER_IO
  add constraint PK_ALARM_TRIGGER_IO primary key (TRIGGER_IO_ID);
alter table ALARM_TRIGGER_IO
  add constraint FK_ALARM_TRIGGER_IO foreign key (ALARM_EVENT_ID)
  references ALARM_EVENT (EVENT_ID);

prompt
prompt Creating table ALARM_TRIGGER_MESSAGE
prompt ====================================
prompt
create table ALARM_TRIGGER_MESSAGE
(
  trigger_message_id NUMBER not null,
  alarm_event_id     NUMBER not null,
  valid_flag         NUMBER(1) not null,
  user_id            NUMBER not null,
  msg_content        VARCHAR2(512),
  operator_id        NUMBER not null,
  create_time        TIMESTAMP(6) not null,
  update_time        TIMESTAMP(6) not null,
  int_rev_1          NUMBER,
  int_rev_2          NUMBER,
  str_rev_1          VARCHAR2(64),
  str_rev_2          VARCHAR2(128)
)
;
comment on column ALARM_TRIGGER_MESSAGE.alarm_event_id
  is '报警事件编号';
comment on column ALARM_TRIGGER_MESSAGE.user_id
  is '接收报警短消息的用户的编号';
comment on column ALARM_TRIGGER_MESSAGE.operator_id
  is '操作员ID';
comment on column ALARM_TRIGGER_MESSAGE.create_time
  is '创建时间';
comment on column ALARM_TRIGGER_MESSAGE.int_rev_1
  is '整型保留字段';
comment on column ALARM_TRIGGER_MESSAGE.int_rev_2
  is '整型保留字段';
comment on column ALARM_TRIGGER_MESSAGE.str_rev_1
  is '字符串保留字段';
comment on column ALARM_TRIGGER_MESSAGE.str_rev_2
  is '字符串保留字段';
create index INDX_ATM_ALARM_EVENT_ID on ALARM_TRIGGER_MESSAGE (ALARM_EVENT_ID);
alter table ALARM_TRIGGER_MESSAGE
  add constraint PK_ALARM_TRIGGER_MESSAGE primary key (TRIGGER_MESSAGE_ID);
alter table ALARM_TRIGGER_MESSAGE
  add constraint FK_ALARM_TRIGGER_MESSAGE foreign key (ALARM_EVENT_ID)
  references ALARM_EVENT (EVENT_ID);

prompt
prompt Creating table ALARM_TRIGGER_POPUP_VIDEO
prompt ========================================
prompt
create table ALARM_TRIGGER_POPUP_VIDEO
(
  popup_video_id NUMBER not null,
  alarm_event_id NUMBER not null,
  valid_flag     NUMBER(1) not null,
  camera_id      NUMBER not null,
  promt          NUMBER(1) not null,
  operator_id    NUMBER not null,
  create_time    TIMESTAMP(6) not null,
  update_time    TIMESTAMP(6) not null,
  int_rev_1      NUMBER,
  int_rev_2      NUMBER,
  str_rev_1      VARCHAR2(64),
  str_rev_2      VARCHAR2(128)
)
;
comment on column ALARM_TRIGGER_POPUP_VIDEO.alarm_event_id
  is '报警事件编号';
comment on column ALARM_TRIGGER_POPUP_VIDEO.camera_id
  is '将弹出该编号的 camera 的图像';
comment on column ALARM_TRIGGER_POPUP_VIDEO.promt
  is '是否允许用户手动干预（即在联动之前给予用户报警联动提示）';
comment on column ALARM_TRIGGER_POPUP_VIDEO.operator_id
  is '操作员ID';
comment on column ALARM_TRIGGER_POPUP_VIDEO.create_time
  is '创建时间';
comment on column ALARM_TRIGGER_POPUP_VIDEO.int_rev_1
  is '整型保留字段';
comment on column ALARM_TRIGGER_POPUP_VIDEO.int_rev_2
  is '整型保留字段';
comment on column ALARM_TRIGGER_POPUP_VIDEO.str_rev_1
  is '字符串保留字段';
comment on column ALARM_TRIGGER_POPUP_VIDEO.str_rev_2
  is '字符串保留字段';
create index INDX_ATPV_ALARM_EVENT_ID on ALARM_TRIGGER_POPUP_VIDEO (ALARM_EVENT_ID);
alter table ALARM_TRIGGER_POPUP_VIDEO
  add constraint PK_ALARM_TRIGGER_POPUP_VIDEO primary key (POPUP_VIDEO_ID);
alter table ALARM_TRIGGER_POPUP_VIDEO
  add constraint FK_ALARM_TRIGGER_POPUP_VIDEO foreign key (ALARM_EVENT_ID)
  references ALARM_EVENT (EVENT_ID);

prompt
prompt Creating table ALARM_TRIGGER_PRESET
prompt ===================================
prompt
create table ALARM_TRIGGER_PRESET
(
  trigger_preset_id    NUMBER not null,
  alarm_event_id       NUMBER not null,
  valid_flag           NUMBER(1) not null,
  camera_id            NUMBER not null,
  type                 NUMBER not null,
  preset_index_trigger NUMBER not null,
  preset_index_normal  NUMBER not null,
  operator_id          NUMBER not null,
  create_time          TIMESTAMP(6) not null,
  update_time          TIMESTAMP(6) not null,
  int_rev_1            NUMBER,
  int_rev_2            NUMBER,
  str_rev_1            VARCHAR2(64),
  str_rev_2            VARCHAR2(128)
)
;
comment on column ALARM_TRIGGER_PRESET.alarm_event_id
  is '报警事件编号';
comment on column ALARM_TRIGGER_PRESET.camera_id
  is '将对该编号的 camera 进行联动预置点操作';
comment on column ALARM_TRIGGER_PRESET.type
  is '类型';
comment on column ALARM_TRIGGER_PRESET.preset_index_trigger
  is '在报警过程中将设置联动的 camera 的预置点为该值';
comment on column ALARM_TRIGGER_PRESET.preset_index_normal
  is '报警结束后将设置联动的 camera 的预置点为该值';
comment on column ALARM_TRIGGER_PRESET.operator_id
  is '操作员ID';
comment on column ALARM_TRIGGER_PRESET.create_time
  is '创建时间';
comment on column ALARM_TRIGGER_PRESET.int_rev_1
  is '整型保留字段';
comment on column ALARM_TRIGGER_PRESET.int_rev_2
  is '整型保留字段';
comment on column ALARM_TRIGGER_PRESET.str_rev_1
  is '字符串保留字段';
comment on column ALARM_TRIGGER_PRESET.str_rev_2
  is '字符串保留字段';
create index INDX_ATP_ALARM_EVENT_ID on ALARM_TRIGGER_PRESET (ALARM_EVENT_ID);
alter table ALARM_TRIGGER_PRESET
  add constraint PK_ALARM_TRIGGER_PRESET primary key (TRIGGER_PRESET_ID);
alter table ALARM_TRIGGER_PRESET
  add constraint FK_ALARM_TRIGGER_PRESET foreign key (ALARM_EVENT_ID)
  references ALARM_EVENT (EVENT_ID);

prompt
prompt Creating table ALARM_TRIGGER_SNAPSHOT
prompt =====================================
prompt
create table ALARM_TRIGGER_SNAPSHOT
(
  trigger_snapshot_id NUMBER not null,
  alarm_event_id      NUMBER not null,
  valid_flag          NUMBER(1) not null,
  camera_id           NUMBER not null,
  operator_id         NUMBER not null,
  create_time         TIMESTAMP(6) not null,
  update_time         TIMESTAMP(6) not null,
  int_rev_1           NUMBER,
  int_rev_2           NUMBER,
  str_rev_1           VARCHAR2(64),
  str_rev_2           VARCHAR2(128)
)
;
comment on column ALARM_TRIGGER_SNAPSHOT.alarm_event_id
  is '报警事件编号';
comment on column ALARM_TRIGGER_SNAPSHOT.camera_id
  is '将抓取该编号的 camera 的图像';
comment on column ALARM_TRIGGER_SNAPSHOT.operator_id
  is '操作员ID';
comment on column ALARM_TRIGGER_SNAPSHOT.create_time
  is '创建时间';
comment on column ALARM_TRIGGER_SNAPSHOT.int_rev_1
  is '整型保留字段';
comment on column ALARM_TRIGGER_SNAPSHOT.int_rev_2
  is '整型保留字段';
comment on column ALARM_TRIGGER_SNAPSHOT.str_rev_1
  is '字符串保留字段';
comment on column ALARM_TRIGGER_SNAPSHOT.str_rev_2
  is '字符串保留字段';
create index INDX_ATS_ALARM_EVENT_ID on ALARM_TRIGGER_SNAPSHOT (ALARM_EVENT_ID);
alter table ALARM_TRIGGER_SNAPSHOT
  add constraint PK_ALARM_TRIGGER_SNAPSHOT primary key (TRIGGER_SNAPSHOT_ID);
alter table ALARM_TRIGGER_SNAPSHOT
  add constraint FK_ALARM_TRIGGER_SNAPSHOT foreign key (ALARM_EVENT_ID)
  references ALARM_EVENT (EVENT_ID);

prompt
prompt Creating table ALARM_TRIGGER_SOUND
prompt ==================================
prompt
create table ALARM_TRIGGER_SOUND
(
  trigger_sound_id NUMBER not null,
  alarm_event_id   NUMBER not null,
  valid_flag       NUMBER(1) not null,
  user_id          NUMBER not null,
  sound_text       VARCHAR2(64),
  promt            NUMBER(1) not null,
  operator_id      NUMBER not null,
  create_time      TIMESTAMP(6) not null,
  update_time      TIMESTAMP(6),
  int_rev_1        NUMBER,
  int_rev_2        NUMBER,
  str_rev_1        VARCHAR2(64),
  str_rev_2        VARCHAR2(128)
)
;
comment on column ALARM_TRIGGER_SOUND.alarm_event_id
  is '报警事件编号';
comment on column ALARM_TRIGGER_SOUND.user_id
  is '接受联动动作的用户的编号';
comment on column ALARM_TRIGGER_SOUND.promt
  is '是否允许用户手动干预（即在联动之前给予用户报警联动提示）';
comment on column ALARM_TRIGGER_SOUND.operator_id
  is '操作员ID';
comment on column ALARM_TRIGGER_SOUND.create_time
  is '创建时间';
comment on column ALARM_TRIGGER_SOUND.int_rev_1
  is '整型保留字段';
comment on column ALARM_TRIGGER_SOUND.int_rev_2
  is '整型保留字段';
comment on column ALARM_TRIGGER_SOUND.str_rev_1
  is '字符串保留字段';
comment on column ALARM_TRIGGER_SOUND.str_rev_2
  is '字符串保留字段';
create index INDX_SOU_ALARM_EVENT_ID on ALARM_TRIGGER_SOUND (ALARM_EVENT_ID);
alter table ALARM_TRIGGER_SOUND
  add constraint PK_ALARM_TRIGGER_SOUND primary key (TRIGGER_SOUND_ID);
alter table ALARM_TRIGGER_SOUND
  add constraint FK_ALARM_TRIGGER_SOUND foreign key (ALARM_EVENT_ID)
  references ALARM_EVENT (EVENT_ID);

prompt
prompt Creating table ALARM_TRIGGER_TALK
prompt =================================
prompt
create table ALARM_TRIGGER_TALK
(
  trigger_talk_id NUMBER not null,
  alarm_event_id  NUMBER not null,
  valid_flag      NUMBER(1) not null,
  device_id       NUMBER not null,
  chan_num        NUMBER not null,
  user_id         NUMBER not null,
  promt           NUMBER(1) not null,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column ALARM_TRIGGER_TALK.alarm_event_id
  is '报警事件编号';
comment on column ALARM_TRIGGER_TALK.valid_flag
  is '该联动是否有效的标志';
comment on column ALARM_TRIGGER_TALK.device_id
  is '待对讲的设备的编号';
comment on column ALARM_TRIGGER_TALK.chan_num
  is '对讲通道编号（从 1 开始）';
comment on column ALARM_TRIGGER_TALK.user_id
  is '接受联动动作的用户的编号';
comment on column ALARM_TRIGGER_TALK.promt
  is '是否允许用户手动干预（即在联动之前给予用户报警联动提示）';
comment on column ALARM_TRIGGER_TALK.operator_id
  is '操作员ID';
comment on column ALARM_TRIGGER_TALK.create_time
  is '创建时间';
comment on column ALARM_TRIGGER_TALK.int_rev_1
  is '整型保留字段';
comment on column ALARM_TRIGGER_TALK.int_rev_2
  is '整型保留字段';
comment on column ALARM_TRIGGER_TALK.str_rev_1
  is '字符串保留字段';
comment on column ALARM_TRIGGER_TALK.str_rev_2
  is '字符串保留字段';
create index INDX_TALK_ALARM_EVENT_ID on ALARM_TRIGGER_TALK (ALARM_EVENT_ID);
alter table ALARM_TRIGGER_TALK
  add constraint PK_ALARM_TRIGGER_TALK primary key (TRIGGER_TALK_ID);
alter table ALARM_TRIGGER_TALK
  add constraint FK_ALARM_TRIGGER_TALK foreign key (ALARM_EVENT_ID)
  references ALARM_EVENT (EVENT_ID);

prompt
prompt Creating table ALARM_TRIGGER_TEXT_OVERLAY
prompt =========================================
prompt
create table ALARM_TRIGGER_TEXT_OVERLAY
(
  trigger_text_overlay_id NUMBER not null,
  alarm_event_id          NUMBER not null,
  valid_flag              NUMBER(1) not null,
  camera_id               NUMBER not null,
  overlay_text            VARCHAR2(1024) not null,
  operator_id             NUMBER not null,
  create_time             TIMESTAMP(6) not null,
  update_time             TIMESTAMP(6) not null,
  int_rev_1               NUMBER,
  int_rev_2               NUMBER,
  str_rev_1               VARCHAR2(64),
  str_rev_2               VARCHAR2(128)
)
;
comment on column ALARM_TRIGGER_TEXT_OVERLAY.alarm_event_id
  is '报警事件编号';
comment on column ALARM_TRIGGER_TEXT_OVERLAY.valid_flag
  is '该联动是否有效的标志';
comment on column ALARM_TRIGGER_TEXT_OVERLAY.camera_id
  is '将在该编号的 camera 的图像上叠加文字信息';
comment on column ALARM_TRIGGER_TEXT_OVERLAY.overlay_text
  is '待叠加的文本信息';
comment on column ALARM_TRIGGER_TEXT_OVERLAY.operator_id
  is '操作员ID';
comment on column ALARM_TRIGGER_TEXT_OVERLAY.create_time
  is '创建时间';
comment on column ALARM_TRIGGER_TEXT_OVERLAY.int_rev_1
  is '整型保留字段';
comment on column ALARM_TRIGGER_TEXT_OVERLAY.int_rev_2
  is '整型保留字段';
comment on column ALARM_TRIGGER_TEXT_OVERLAY.str_rev_1
  is '字符串保留字段';
comment on column ALARM_TRIGGER_TEXT_OVERLAY.str_rev_2
  is '字符串保留字段';
create index INDX_TEXT_ALARM_EVENT_ID on ALARM_TRIGGER_TEXT_OVERLAY (ALARM_EVENT_ID);
alter table ALARM_TRIGGER_TEXT_OVERLAY
  add constraint PK_ALARM_TRIGGER_TEXT_OVERLAY primary key (TRIGGER_TEXT_OVERLAY_ID);
alter table ALARM_TRIGGER_TEXT_OVERLAY
  add constraint FK_TRIGGER_TEXT_OVERLAY foreign key (ALARM_EVENT_ID)
  references ALARM_EVENT (EVENT_ID);

prompt
prompt Creating table ALARM_TRIGGER_TVWALL
prompt ===================================
prompt
create table ALARM_TRIGGER_TVWALL
(
  trigger_tvwall_id NUMBER not null,
  alarm_event_id    NUMBER not null,
  valid_flag        NUMBER(1) not null,
  camera_id         NUMBER not null,
  tvwall_id         NUMBER not null,
  operator_id       NUMBER not null,
  create_time       TIMESTAMP(6) not null,
  update_time       TIMESTAMP(6) not null,
  int_rev_1         NUMBER,
  int_rev_2         NUMBER,
  str_rev_1         VARCHAR2(64),
  str_rev_2         VARCHAR2(128)
)
;
comment on column ALARM_TRIGGER_TVWALL.alarm_event_id
  is '接受联动动作的用户的编号';
comment on column ALARM_TRIGGER_TVWALL.valid_flag
  is '该联动是否有效的标志';
comment on column ALARM_TRIGGER_TVWALL.camera_id
  is '该编号的 camera 的图像将被弹上墙';
comment on column ALARM_TRIGGER_TVWALL.tvwall_id
  is '电视墙的编号';
comment on column ALARM_TRIGGER_TVWALL.operator_id
  is '操作员ID';
comment on column ALARM_TRIGGER_TVWALL.create_time
  is '创建时间';
comment on column ALARM_TRIGGER_TVWALL.int_rev_1
  is '整型保留字段';
comment on column ALARM_TRIGGER_TVWALL.int_rev_2
  is '整型保留字段';
comment on column ALARM_TRIGGER_TVWALL.str_rev_1
  is '字符串保留字段';
comment on column ALARM_TRIGGER_TVWALL.str_rev_2
  is '字符串保留字段';
create index INDX_ATT_ALARM_EVENT_ID on ALARM_TRIGGER_TVWALL (ALARM_EVENT_ID);
alter table ALARM_TRIGGER_TVWALL
  add constraint PK_ALARM_TRIGGER_TVWALL primary key (TRIGGER_TVWALL_ID);
alter table ALARM_TRIGGER_TVWALL
  add constraint FK_ALARM_TRIGGER_TVWALL foreign key (ALARM_EVENT_ID)
  references ALARM_EVENT (EVENT_ID);

prompt
prompt Creating table ALARM_TRIGGER_VRM
prompt ================================
prompt
create table ALARM_TRIGGER_VRM
(
  trigger_vrm_id NUMBER not null,
  alarm_event_id NUMBER not null,
  valid_flag     NUMBER(1) not null,
  camera_id      NUMBER not null,
  vrm_id         NUMBER not null,
  record_style   NUMBER,
  rec_index_code VARCHAR2(255),
  operator_id    NUMBER not null,
  create_time    TIMESTAMP(6) not null,
  update_time    TIMESTAMP(6) not null,
  int_rev_1      NUMBER,
  int_rev_2      NUMBER,
  str_rev_1      VARCHAR2(64),
  str_rev_2      VARCHAR2(128)
)
;
comment on column ALARM_TRIGGER_VRM.alarm_event_id
  is '报警事件编号';
comment on column ALARM_TRIGGER_VRM.valid_flag
  is '该联动是否有效的标志';
comment on column ALARM_TRIGGER_VRM.camera_id
  is '将对该编号的 camera 进行录像';
comment on column ALARM_TRIGGER_VRM.vrm_id
  is '接受联动动作的 vrm 的编号';
comment on column ALARM_TRIGGER_VRM.record_style
  is 'record style';
comment on column ALARM_TRIGGER_VRM.rec_index_code
  is 'index code';
comment on column ALARM_TRIGGER_VRM.operator_id
  is '操作员ID';
comment on column ALARM_TRIGGER_VRM.create_time
  is '创建时间';
comment on column ALARM_TRIGGER_VRM.int_rev_1
  is '整型保留字段';
comment on column ALARM_TRIGGER_VRM.int_rev_2
  is '整型保留字段';
comment on column ALARM_TRIGGER_VRM.str_rev_1
  is '字符串保留字段';
comment on column ALARM_TRIGGER_VRM.str_rev_2
  is '字符串保留字段';
create index INDX_ATV_ALARM_EVENT_ID on ALARM_TRIGGER_VRM (ALARM_EVENT_ID);
alter table ALARM_TRIGGER_VRM
  add constraint PK_ALARM_TRIGGER_VRM primary key (TRIGGER_VRM_ID);
alter table ALARM_TRIGGER_VRM
  add constraint FK_ALARM_TRIGGER_VRM foreign key (ALARM_EVENT_ID)
  references ALARM_EVENT (EVENT_ID);

prompt
prompt Creating table ASSET_APPLY_BUY
prompt ==============================
prompt
create table ASSET_APPLY_BUY
(
  asset_apply_buy_id NUMBER not null,
  apply_petitioner   VARCHAR2(64) not null,
  apply_price        NUMBER(10,2),
  apply_reason       VARCHAR2(255),
  apply_time         TIMESTAMP(6) not null,
  asset_info_id      NUMBER not null,
  asset_scope_id     NUMBER not null,
  finish_time        TIMESTAMP(6),
  remark             VARCHAR2(255),
  status             NUMBER not null
)
;
alter table ASSET_APPLY_BUY
  add constraint PK_ASSET_APPLY_BUY primary key (ASSET_APPLY_BUY_ID);

prompt
prompt Creating table ASSET_CONTRACTORS
prompt ================================
prompt
create table ASSET_CONTRACTORS
(
  contractors_id     NUMBER not null,
  address            VARCHAR2(255),
  contacter          VARCHAR2(64),
  contractors_name   VARCHAR2(255) not null,
  contractors_status NUMBER,
  contractors_type   VARCHAR2(32),
  mobile             VARCHAR2(16),
  remark             VARCHAR2(1024),
  telphone           VARCHAR2(32)
)
;
alter table ASSET_CONTRACTORS
  add constraint PK_ASSET_CONTRACTORS primary key (CONTRACTORS_ID);

prompt
prompt Creating table LOGIN_RESTRICTION
prompt ================================
prompt
create table LOGIN_RESTRICTION
(
  restrict_id   NUMBER not null,
  time_restrict NUMBER(1) not null,
  begin_time    DATE,
  end_time      DATE,
  ip_restrict   NUMBER(1) not null,
  ip_addr       VARCHAR2(16),
  operator_id   NUMBER not null,
  create_time   TIMESTAMP(6) not null,
  update_time   TIMESTAMP(6) not null,
  int_rev_1     NUMBER,
  int_rev_2     NUMBER,
  str_rev_1     VARCHAR2(64),
  str_rev_2     VARCHAR2(128)
)
;
comment on column LOGIN_RESTRICTION.time_restrict
  is '是否进行时间限制登录，0表示不限制，1表示限制';
comment on column LOGIN_RESTRICTION.begin_time
  is '每天的登录开始时间';
comment on column LOGIN_RESTRICTION.end_time
  is '每天登录的结束时间';
comment on column LOGIN_RESTRICTION.ip_addr
  is 'IP地址限制';
comment on column LOGIN_RESTRICTION.operator_id
  is '操作预员ID,如果是根用户则为0';
comment on column LOGIN_RESTRICTION.int_rev_1
  is '整型保留字段';
comment on column LOGIN_RESTRICTION.int_rev_2
  is '整型保留字段';
comment on column LOGIN_RESTRICTION.str_rev_1
  is '字符串保留字段';
comment on column LOGIN_RESTRICTION.str_rev_2
  is '字符串保留字段';
alter table LOGIN_RESTRICTION
  add constraint PK_LOGIN_RESTRICTION primary key (RESTRICT_ID);

prompt
prompt Creating table USER_GROUP
prompt =========================
prompt
create table USER_GROUP
(
  user_group_id   NUMBER not null,
  control_unit_id NUMBER not null,
  index_code      VARCHAR2(64) not null,
  name            VARCHAR2(32) not null,
  share_mode      NUMBER not null,
  description     VARCHAR2(128),
  sequence_idx    NUMBER not null,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column USER_GROUP.control_unit_id
  is '控制单元ID';
comment on column USER_GROUP.index_code
  is '用户组索引编号';
comment on column USER_GROUP.name
  is '用户组名称';
comment on column USER_GROUP.share_mode
  is '共享模式,1-共享/0-私有';
comment on column USER_GROUP.operator_id
  is '操作预员ID,如果是根用户则为0';
comment on column USER_GROUP.int_rev_1
  is '整型保留字段';
comment on column USER_GROUP.int_rev_2
  is '整型保留字段';
comment on column USER_GROUP.str_rev_1
  is '字符串保留字段';
comment on column USER_GROUP.str_rev_2
  is '字符串保留字段';
create index INDX_UG_CONTROL_UNIT_ID on USER_GROUP (CONTROL_UNIT_ID);
alter table USER_GROUP
  add constraint PK_USER_GROUP primary key (USER_GROUP_ID);
alter table USER_GROUP
  add constraint FK_USER_GROUP foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table USER_INFO
prompt ========================
prompt
create table USER_INFO
(
  user_id         NUMBER not null,
  restrict_id     NUMBER,
  user_group_id   NUMBER,
  name            VARCHAR2(64) not null,
  pwd             VARCHAR2(64) not null,
  status          NUMBER not null,
  control_unit_id NUMBER not null,
  real_name       VARCHAR2(32),
  card_id         VARCHAR2(32),
  phone_no        VARCHAR2(32),
  email           VARCHAR2(32),
  login_times     NUMBER,
  last_login_time TIMESTAMP(6),
  expire_time     TIMESTAMP(6) not null,
  priority        NUMBER(5) not null,
  sequence_idx    NUMBER,
  description     VARCHAR2(128),
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128),
  telphone_num    VARCHAR2(128),
  login_type      NUMBER,
  mac_address     VARCHAR2(1000),
  ip_address      VARCHAR2(1000),
  defaultunit     NUMBER
)
;
comment on column USER_INFO.restrict_id
  is '登录限制表ID';
comment on column USER_INFO.user_group_id
  is '用户所在部门';
comment on column USER_INFO.pwd
  is '用户密码';
comment on column USER_INFO.status
  is '0-启用，1-停用，2-注销';
comment on column USER_INFO.control_unit_id
  is '用户所属的区域ID';
comment on column USER_INFO.real_name
  is '用户的真实姓名';
comment on column USER_INFO.card_id
  is '用户身份证号';
comment on column USER_INFO.phone_no
  is '用户的电话号码';
comment on column USER_INFO.email
  is '用户的E-MAIL地址';
comment on column USER_INFO.last_login_time
  is '最后登录时间';
comment on column USER_INFO.expire_time
  is '失效时间';
comment on column USER_INFO.sequence_idx
  is '排序索引';
comment on column USER_INFO.operator_id
  is '操作预员ID,如果是根用户则为0';
comment on column USER_INFO.int_rev_1
  is '整型保留字段';
comment on column USER_INFO.int_rev_2
  is '整型保留字段';
comment on column USER_INFO.str_rev_1
  is '字符串保留字段';
comment on column USER_INFO.str_rev_2
  is '字符串保留字段';
comment on column USER_INFO.telphone_num
  is '分机号';
comment on column USER_INFO.login_type
  is '登陆类型，“0：普通登陆，1：KPI登陆，2：MAC登陆，3：IP段登陆”';
comment on column USER_INFO.mac_address
  is '绑定的MAC地址，支持多个，格式“,08-00-27-00-5C-15,08-00-27-00-5C-15,”';
comment on column USER_INFO.ip_address
  is '绑定的IP地址，支持多个，格式“,127.1.1.1~127.1.1.254,127.1.2.4~127.1.2.200,”';
comment on column USER_INFO.defaultunit
  is '默认展开层级';
create index INDX_UI_RESTRICT_ID on USER_INFO (RESTRICT_ID);
create index INDX_UI_USER_GROUP_ID on USER_INFO (USER_GROUP_ID);
alter table USER_INFO
  add constraint PK_USER_INFO primary key (USER_ID);
alter table USER_INFO
  add constraint FK_USER_INFO1 foreign key (RESTRICT_ID)
  references LOGIN_RESTRICTION (RESTRICT_ID);
alter table USER_INFO
  add constraint FK_USER_INFO2 foreign key (USER_GROUP_ID)
  references USER_GROUP (USER_GROUP_ID);

prompt
prompt Creating table ASSET_INFO
prompt =========================
prompt
create table ASSET_INFO
(
  asset_info_id          NUMBER not null,
  asset_discount         NUMBER(10,2),
  asset_mode             VARCHAR2(255),
  asset_name             VARCHAR2(64),
  asset_price            NUMBER(10,2),
  asset_type             VARCHAR2(255),
  bean_id                VARCHAR2(255),
  bean_name              VARCHAR2(255),
  camera_index_code      VARCHAR2(255),
  image_url              BLOB,
  index_code             VARCHAR2(255),
  install_place          VARCHAR2(255),
  install_time           TIMESTAMP(6),
  install_way            VARCHAR2(255),
  isrepair               NUMBER,
  isscrap                NUMBER,
  latitude               NUMBER(10,5),
  longitude              NUMBER(10,5),
  monitoring_range       VARCHAR2(255),
  object_id              NUMBER not null,
  object_type            NUMBER not null,
  police_room            VARCHAR2(255),
  record_time            TIMESTAMP(6),
  repair_count           NUMBER,
  status                 NUMBER,
  street_light           VARCHAR2(255),
  asset_contractor_id    NUMBER,
  asset_manager          NUMBER,
  control_unit_id        NUMBER,
  engineer_contractor_id NUMBER
)
;
alter table ASSET_INFO
  add constraint PK_ASSET_INFO primary key (ASSET_INFO_ID);
alter table ASSET_INFO
  add constraint UK_ASSET_INFO unique (INDEX_CODE);
alter table ASSET_INFO
  add constraint FK_ASSET_INFO1 foreign key (ASSET_MANAGER)
  references USER_INFO (USER_ID);
alter table ASSET_INFO
  add constraint FK_ASSET_INFO2 foreign key (ASSET_CONTRACTOR_ID)
  references ASSET_CONTRACTORS (CONTRACTORS_ID);
alter table ASSET_INFO
  add constraint FK_ASSET_INFO3 foreign key (ENGINEER_CONTRACTOR_ID)
  references ASSET_CONTRACTORS (CONTRACTORS_ID);
alter table ASSET_INFO
  add constraint FK_ASSET_INFO4 foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table ASSET_REPAIR
prompt ===========================
prompt
create table ASSET_REPAIR
(
  asset_repair_id         NUMBER not null,
  actual_repair_time      TIMESTAMP(6),
  asset_repair_number     VARCHAR2(64),
  control_unit_name       VARCHAR2(255),
  forecast_repair_time    TIMESTAMP(6),
  remark                  VARCHAR2(255),
  repair_apply_time       TIMESTAMP(6) not null,
  repair_company          VARCHAR2(32),
  repair_company_tel      VARCHAR2(16),
  repair_price            NUMBER(10,2),
  repair_reason           VARCHAR2(255) not null,
  status                  NUMBER not null,
  asset_info_id           NUMBER,
  repair_apply_petitioner NUMBER not null,
  repair_contractor_id    NUMBER
)
;
alter table ASSET_REPAIR
  add constraint PK_ASSET_REPAIR primary key (ASSET_REPAIR_ID);
alter table ASSET_REPAIR
  add constraint UK_ASSET_REPAIR unique (ASSET_REPAIR_NUMBER);
alter table ASSET_REPAIR
  add constraint FK_ASSET_REPAIR1 foreign key (ASSET_INFO_ID)
  references ASSET_INFO (ASSET_INFO_ID);
alter table ASSET_REPAIR
  add constraint FK_ASSET_REPAIR2 foreign key (REPAIR_APPLY_PETITIONER)
  references USER_INFO (USER_ID);
alter table ASSET_REPAIR
  add constraint FK_ASSET_REPAIR3 foreign key (REPAIR_CONTRACTOR_ID)
  references ASSET_CONTRACTORS (CONTRACTORS_ID);

prompt
prompt Creating table ASSET_SCRAP
prompt ==========================
prompt
create table ASSET_SCRAP
(
  asset_scrap_id         NUMBER not null,
  asset_scrap_number     VARCHAR2(255),
  control_unit_name      VARCHAR2(255),
  finish_time            TIMESTAMP(6),
  remark                 VARCHAR2(255),
  scrap_apply_time       TIMESTAMP(6) not null,
  scrap_reason           VARCHAR2(255) not null,
  status                 NUMBER not null,
  asset_info_id          NUMBER,
  scrap_apply_petitioner NUMBER not null
)
;
alter table ASSET_SCRAP
  add constraint PK_ASSET_SCRAP primary key (ASSET_SCRAP_ID);
alter table ASSET_SCRAP
  add constraint UK_ASSET_SCRAP unique (ASSET_SCRAP_NUMBER);
alter table ASSET_SCRAP
  add constraint FK_ASSET_SCRAP1 foreign key (ASSET_INFO_ID)
  references ASSET_INFO (ASSET_INFO_ID);
alter table ASSET_SCRAP
  add constraint FK_ASSET_SCRAP2 foreign key (SCRAP_APPLY_PETITIONER)
  references USER_INFO (USER_ID);

prompt
prompt Creating table ASSET_SEQUENCES
prompt ==============================
prompt
create table ASSET_SEQUENCES
(
  sequences_id NUMBER not null,
  currentdate  DATE not null,
  current_no   NUMBER not null,
  descriptions VARCHAR2(255),
  type         VARCHAR2(255) not null
)
;
alter table ASSET_SEQUENCES
  add constraint PK_ASSET_SEQUENCES primary key (SEQUENCES_ID);

prompt
prompt Creating table DEVICE_INFO
prompt ==========================
prompt
create table DEVICE_INFO
(
  device_id         NUMBER not null,
  control_unit_id   NUMBER not null,
  index_code        VARCHAR2(64),
  type_code         NUMBER not null,
  name              VARCHAR2(64) not null,
  register_type     NUMBER,
  network_addr      VARCHAR2(128),
  network_port      NUMBER,
  pag_server_id     NUMBER,
  user_name         VARCHAR2(64),
  user_pwd          VARCHAR2(64),
  dns_addr          VARCHAR2(128),
  dns_port          NUMBER,
  serial_no         VARCHAR2(64),
  camera_chan_count NUMBER not null,
  ip_chan_count     NUMBER,
  alarm_in_count    NUMBER not null,
  alarm_out_count   NUMBER not null,
  talk_chan_count   NUMBER not null,
  sequence_idx      NUMBER not null,
  status            NUMBER,
  operator_id       NUMBER not null,
  matrix_id         NUMBER,
  create_time       TIMESTAMP(6) not null,
  update_time       TIMESTAMP(6) not null,
  police_id         NUMBER,
  voice_data_type   NUMBER,
  int_rev_1         NUMBER,
  int_rev_2         NUMBER,
  str_rev_1         VARCHAR2(64),
  str_rev_2         VARCHAR2(128),
  cms_cascade_id    NUMBER,
  power_channel_id  NUMBER,
  inter_region      NUMBER(1),
  privilege_num     NUMBER,
  police_name       VARCHAR2(128),
  police_no         VARCHAR2(64),
  is_normal         NUMBER(1),
  start_time        TIMESTAMP(6),
  persistence       NUMBER,
  phone_no          VARCHAR2(64)
)
;
comment on column DEVICE_INFO.control_unit_id
  is '所属的控制单元';
comment on column DEVICE_INFO.index_code
  is '索引编号';
comment on column DEVICE_INFO.type_code
  is '类型编号';
comment on column DEVICE_INFO.name
  is '设备名称';
comment on column DEVICE_INFO.network_port
  is '网络端口';
comment on column DEVICE_INFO.pag_server_id
  is '设备接入服务器ID';
comment on column DEVICE_INFO.user_name
  is '设备登录的用户名';
comment on column DEVICE_INFO.user_pwd
  is '用户密码';
comment on column DEVICE_INFO.dns_addr
  is 'dns的IP地址';
comment on column DEVICE_INFO.dns_port
  is 'dns的端口号';
comment on column DEVICE_INFO.status
  is '设备状态 0：在线 1：不在线';
comment on column DEVICE_INFO.operator_id
  is '操作员ID';
comment on column DEVICE_INFO.matrix_id
  is '操作员ID';
comment on column DEVICE_INFO.create_time
  is '创建时间';
comment on column DEVICE_INFO.police_id
  is '关联警员ID';
comment on column DEVICE_INFO.voice_data_type
  is '对讲数据类型';
comment on column DEVICE_INFO.int_rev_1
  is '整型保留字段';
comment on column DEVICE_INFO.int_rev_2
  is '整型保留字段';
comment on column DEVICE_INFO.str_rev_1
  is '字符串保留字段';
comment on column DEVICE_INFO.str_rev_2
  is '字符串保留字段';
comment on column DEVICE_INFO.cms_cascade_id
  is '下级级联配置id';
comment on column DEVICE_INFO.inter_region
  is '是否跨组';
comment on column DEVICE_INFO.privilege_num
  is '权限大小';
comment on column DEVICE_INFO.police_name
  is '警员姓名';
comment on column DEVICE_INFO.police_no
  is '警号';
comment on column DEVICE_INFO.is_normal
  is '是否正常';
comment on column DEVICE_INFO.start_time
  is '开始通话时间';
comment on column DEVICE_INFO.persistence
  is '持续通话时间(分钟)';
comment on column DEVICE_INFO.phone_no
  is '联系方式';
create index INDX_DI_CONTROL_UNIT_ID on DEVICE_INFO (CONTROL_UNIT_ID);
alter table DEVICE_INFO
  add constraint PK_DEVICE_INFO primary key (DEVICE_ID);
alter table DEVICE_INFO
  add constraint UK_DEVICE_INFO unique (INDEX_CODE);
alter table DEVICE_INFO
  add constraint FK_DEVICE_INFO foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table REGION_INFO
prompt ==========================
prompt
create table REGION_INFO
(
  region_id             NUMBER not null,
  control_unit_id       NUMBER not null,
  index_code            VARCHAR2(64),
  name                  VARCHAR2(128) not null,
  parent_id             NUMBER not null,
  region_level          NUMBER not null,
  use_stream_sv         NUMBER not null,
  stream_sv_id          NUMBER,
  use_stream_sv_for_vrm NUMBER,
  stream_sv_for_vrm_id  NUMBER,
  sequence_idx          NUMBER,
  operator_id           NUMBER not null,
  create_time           TIMESTAMP(6) not null,
  update_time           TIMESTAMP(6) not null,
  int_rev_1             NUMBER,
  int_rev_2             NUMBER,
  str_rev_1             VARCHAR2(64),
  str_rev_2             VARCHAR2(128),
  cascade_share_ids     VARCHAR2(255),
  cms_cascade_id        NUMBER
)
;
comment on column REGION_INFO.control_unit_id
  is '所属的控制单元';
comment on column REGION_INFO.index_code
  is '索引编号';
comment on column REGION_INFO.name
  is '区域名称';
comment on column REGION_INFO.parent_id
  is '上级域的ID,0表示根域';
comment on column REGION_INFO.region_level
  is '区域所属的级别，根单元为0，其他表示所属的层次';
comment on column REGION_INFO.use_stream_sv
  is '是否使用流媒体';
comment on column REGION_INFO.use_stream_sv_for_vrm
  is 'VRM是否使用流媒体';
comment on column REGION_INFO.operator_id
  is '操作员ID';
comment on column REGION_INFO.create_time
  is '创建时间';
comment on column REGION_INFO.int_rev_1
  is '整型保留字段';
comment on column REGION_INFO.int_rev_2
  is '整型保留字段';
comment on column REGION_INFO.str_rev_1
  is '字符串保留字段';
comment on column REGION_INFO.str_rev_2
  is '字符串保留字段';
comment on column REGION_INFO.cascade_share_ids
  is '共享平台编号';
comment on column REGION_INFO.cms_cascade_id
  is '下级级联配置id';
create index IDX_RI_PARENT_ID on REGION_INFO (PARENT_ID);
create index INDX_RI_CONTROL_UNIT_ID on REGION_INFO (CONTROL_UNIT_ID);
alter table REGION_INFO
  add constraint PK_REGION_INFO primary key (REGION_ID);
alter table REGION_INFO
  add constraint UK_REGION_INFO unique (INDEX_CODE);
alter table REGION_INFO
  add constraint FK_REGION_INFO foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table CAMERA_INFO
prompt ==========================
prompt
create table CAMERA_INFO
(
  camera_id             NUMBER not null,
  region_id             NUMBER,
  device_id             NUMBER not null,
  index_code            VARCHAR2(64),
  name                  VARCHAR2(64) not null,
  chan_num              NUMBER not null,
  camera_type           NUMBER not null,
  share_flag            NUMBER(5) not null,
  connect_type          NUMBER,
  stream_type           NUMBER,
  ptz_control_type      NUMBER,
  matrix_code           VARCHAR2(255),
  pixel                 NUMBER,
  ptz_type              NUMBER,
  sound                 NUMBER(1),
  main_bit_rate         NUMBER not null,
  main_rate_type        NUMBER not null,
  sub_bit_rate          NUMBER not null,
  sub_rate_type         NUMBER not null,
  sequence_idx          NUMBER,
  operator_id           NUMBER not null,
  update_time           TIMESTAMP(6) not null,
  create_time           TIMESTAMP(6) not null,
  int_rev_1             NUMBER,
  int_rev_2             NUMBER,
  str_rev_1             VARCHAR2(64),
  str_rev_2             VARCHAR2(128),
  privilege_code        VARCHAR2(255),
  rec_lacation          VARCHAR2(128),
  is_enable             NUMBER,
  group_id              NUMBER,
  status                NUMBER,
  longitude             FLOAT,
  latitude              FLOAT,
  elevation             FLOAT,
  cascade_share_ids     VARCHAR2(255),
  target_type           VARCHAR2(255),
  target_sub_type       VARCHAR2(255),
  decode_tag            VARCHAR2(64),
  cms_cascade_id        NUMBER,
  pcode                 VARCHAR2(128),
  stream_svr_id         NUMBER,
  stream_svr_id_for_vrm NUMBER,
  install_place         VARCHAR2(255),
  map_type              NUMBER,
  vqd_id                NUMBER,
  power_channel_id      NUMBER
)
;
comment on column CAMERA_INFO.region_id
  is '区域ID';
comment on column CAMERA_INFO.device_id
  is '设备ID';
comment on column CAMERA_INFO.index_code
  is '索引编号';
comment on column CAMERA_INFO.name
  is '监控点名称';
comment on column CAMERA_INFO.chan_num
  is '通道号，从1开始';
comment on column CAMERA_INFO.camera_type
  is '监控点类型，0-枪机/1-半球/2-快球/3-带云台枪击';
comment on column CAMERA_INFO.share_flag
  is '0表示不与其它共享，1表示与其它控制单元共享';
comment on column CAMERA_INFO.connect_type
  is '监控点连接协议,0-TCP/1-UDP/2-MCAST/3-RTP';
comment on column CAMERA_INFO.stream_type
  is '0-主码流 1-子码流';
comment on column CAMERA_INFO.ptz_control_type
  is '云台控制类型';
comment on column CAMERA_INFO.matrix_code
  is '矩阵编号';
comment on column CAMERA_INFO.pixel
  is '摄像头像素：1：普通、2：130万高清、3：200万高清、4：300万高清';
comment on column CAMERA_INFO.ptz_type
  is '1：全方位云台（带转动和变焦）；2：只有变焦，不带转动3：只有转动，不带变焦4：无云台，无变焦';
comment on column CAMERA_INFO.sound
  is '音频';
comment on column CAMERA_INFO.sequence_idx
  is '排队序号';
comment on column CAMERA_INFO.operator_id
  is '操作员ID';
comment on column CAMERA_INFO.create_time
  is '创建时间';
comment on column CAMERA_INFO.int_rev_1
  is '整型保留字段';
comment on column CAMERA_INFO.int_rev_2
  is '整型保留字段';
comment on column CAMERA_INFO.str_rev_1
  is '字符串保留字段';
comment on column CAMERA_INFO.str_rev_2
  is '字符串保留字段';
comment on column CAMERA_INFO.privilege_code
  is '下级权限';
comment on column CAMERA_INFO.rec_lacation
  is '下级存储位置';
comment on column CAMERA_INFO.is_enable
  is '是否在线，0不在线，1在线';
comment on column CAMERA_INFO.group_id
  is '单兵所属分组，未分组则为null';
comment on column CAMERA_INFO.status
  is '状态';
comment on column CAMERA_INFO.longitude
  is '经度';
comment on column CAMERA_INFO.latitude
  is '纬度';
comment on column CAMERA_INFO.elevation
  is '海拔';
comment on column CAMERA_INFO.cascade_share_ids
  is '共享平台编号';
comment on column CAMERA_INFO.target_type
  is '监控点目标属性';
comment on column CAMERA_INFO.target_sub_type
  is '监控点目标子属性';
comment on column CAMERA_INFO.decode_tag
  is '解码标识';
comment on column CAMERA_INFO.cms_cascade_id
  is '下级级联配置id';
comment on column CAMERA_INFO.pcode
  is '下级级联权限';
comment on column CAMERA_INFO.stream_svr_id
  is '流媒体服务器ID';
comment on column CAMERA_INFO.stream_svr_id_for_vrm
  is 'VRM使用流媒体ID';
comment on column CAMERA_INFO.install_place
  is '安装地点';
comment on column CAMERA_INFO.map_type
  is '地图类型';
comment on column CAMERA_INFO.vqd_id
  is 'VQD服务器ID';
create index INDX_CI_DEVICE_ID on CAMERA_INFO (DEVICE_ID);
create index INDX_CI_REGION_ID on CAMERA_INFO (REGION_ID);
alter table CAMERA_INFO
  add constraint PK_CAMERA_INFO primary key (CAMERA_ID);
alter table CAMERA_INFO
  add constraint UK_CAMERA_INFO unique (INDEX_CODE);
alter table CAMERA_INFO
  add constraint FK_CAMERA_INFO1 foreign key (DEVICE_ID)
  references DEVICE_INFO (DEVICE_ID);
alter table CAMERA_INFO
  add constraint FK_CAMERA_INFO2 foreign key (REGION_ID)
  references REGION_INFO (REGION_ID);

prompt
prompt Creating table STREAMSERVER_LB_CONFIG
prompt =====================================
prompt
create table STREAMSERVER_LB_CONFIG
(
  lb_config_id      NUMBER not null,
  heartcycle        NUMBER not null,
  hard_info_cycle   NUMBER not null,
  chan_upload_cycle NUMBER not null,
  cpu_coeff         NUMBER not null,
  mem_coeff         NUMBER not null,
  net_coeff         NUMBER not null,
  chan_coeff        NUMBER not null,
  chan_cache_time   NUMBER not null,
  max_load          NUMBER not null,
  max_chan          NUMBER not null,
  cache_coeff       NUMBER not null,
  cache_coeff_cycle NUMBER not null
)
;
alter table STREAMSERVER_LB_CONFIG
  add constraint PK_STREAMSERVER_LB_CONFIG primary key (LB_CONFIG_ID);

prompt
prompt Creating table STREAM_SERVER
prompt ============================
prompt
create table STREAM_SERVER
(
  stream_sv_id      NUMBER not null,
  control_unit_id   NUMBER not null,
  share_flag        NUMBER(5) not null,
  index_code        VARCHAR2(64) not null,
  name              VARCHAR2(64) not null,
  ip_addr           VARCHAR2(16) not null,
  out_ip_addr       VARCHAR2(16) default '0.0.0.0',
  second_ip         VARCHAR2(16),
  net_zone_id       NUMBER not null,
  ctrl_port         NUMBER not null,
  multi_cast_ipaddr VARCHAR2(16),
  rtsp_port         NUMBER,
  data_port         NUMBER,
  net_agent_port    NUMBER,
  rtp_tcp_base_port NUMBER,
  rtp_udp_base_port NUMBER,
  udp_port_count    NUMBER,
  tcp_port_count    NUMBER,
  matrix_port       NUMBER,
  relative_path     VARCHAR2(256),
  sequence_idx      NUMBER not null,
  operator_id       NUMBER not null,
  create_time       TIMESTAMP(6) not null,
  update_time       TIMESTAMP(6) not null,
  enable_lb         NUMBER(1),
  enable_lu         NUMBER(1),
  int_rev_1         NUMBER,
  int_rev_2         NUMBER,
  str_rev_1         VARCHAR2(64),
  str_rev_2         VARCHAR2(128),
  lb_config_id      NUMBER,
  server_type       NUMBER,
  http_port         NUMBER
)
;
comment on column STREAM_SERVER.control_unit_id
  is '控制单元ID';
comment on column STREAM_SERVER.index_code
  is '索引编号';
comment on column STREAM_SERVER.name
  is '服务器名称';
comment on column STREAM_SERVER.net_zone_id
  is 'net zone id';
comment on column STREAM_SERVER.ctrl_port
  is '控制端口';
comment on column STREAM_SERVER.rtsp_port
  is 'RTSP侦听端口';
comment on column STREAM_SERVER.data_port
  is '数据端口';
comment on column STREAM_SERVER.net_agent_port
  is '网络代理端口';
comment on column STREAM_SERVER.rtp_tcp_base_port
  is '本级RTP/TCP的端口';
comment on column STREAM_SERVER.rtp_udp_base_port
  is 'RTP/UDB 基本端口';
comment on column STREAM_SERVER.udp_port_count
  is 'UDP端口数';
comment on column STREAM_SERVER.tcp_port_count
  is 'TCP端口数';
comment on column STREAM_SERVER.matrix_port
  is '矩阵端口号';
comment on column STREAM_SERVER.relative_path
  is '文件点播相对路径';
comment on column STREAM_SERVER.operator_id
  is '操作员ID';
comment on column STREAM_SERVER.create_time
  is '创建时间';
comment on column STREAM_SERVER.enable_lb
  is '是否启用负载均衡服务器功能';
comment on column STREAM_SERVER.enable_lu
  is '是否允许被负载均衡';
comment on column STREAM_SERVER.int_rev_1
  is '整型保留字段';
comment on column STREAM_SERVER.int_rev_2
  is '整型保留字段';
comment on column STREAM_SERVER.str_rev_1
  is '字符串保留字段';
comment on column STREAM_SERVER.str_rev_2
  is '字符串保留字段';
comment on column STREAM_SERVER.server_type
  is '服务器类型';
comment on column STREAM_SERVER.http_port
  is '本地web端口';
create index INDX_SS_CONTROL_UNIT_ID on STREAM_SERVER (CONTROL_UNIT_ID);
alter table STREAM_SERVER
  add constraint PK_STREAM_SERVER primary key (STREAM_SV_ID);
alter table STREAM_SERVER
  add constraint UK_STREAM_SERVER unique (INDEX_CODE);
alter table STREAM_SERVER
  add constraint FK_STREAM_SERVER1 foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);
alter table STREAM_SERVER
  add constraint FK_STREAM_SERVER2 foreign key (LB_CONFIG_ID)
  references STREAMSERVER_LB_CONFIG (LB_CONFIG_ID);

prompt
prompt Creating table BAYONET_SNAP_SITE
prompt ================================
prompt
create table BAYONET_SNAP_SITE
(
  site_id       NUMBER not null,
  site_name     VARCHAR2(64) not null,
  site_no       VARCHAR2(16) not null,
  place_name    VARCHAR2(16),
  unit_id       NUMBER not null,
  camera_id     NUMBER not null,
  enablestream  NUMBER not null,
  streamlsh     NUMBER,
  vrplsh        NUMBER not null,
  alarmplace_id NUMBER
)
;
alter table BAYONET_SNAP_SITE
  add constraint PK_BAYONET_SNAP_SITE primary key (SITE_ID);
alter table BAYONET_SNAP_SITE
  add constraint FK_BAYONET_SNAP_SITE1 foreign key (STREAMLSH)
  references STREAM_SERVER (STREAM_SV_ID);
alter table BAYONET_SNAP_SITE
  add constraint FK_BAYONET_SNAP_SITE2 foreign key (UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);
alter table BAYONET_SNAP_SITE
  add constraint FK_BAYONET_SNAP_SITE3 foreign key (CAMERA_ID)
  references CAMERA_INFO (CAMERA_ID);
alter table BAYONET_SNAP_SITE
  add constraint FK_BAYONET_SNAP_SITE4 foreign key (VRPLSH)
  references SERVER_INFO (SERVER_ID);

prompt
prompt Creating table BMS_CROSSING_SERVER
prompt ==================================
prompt
create table BMS_CROSSING_SERVER
(
  crossing_server_id   NUMBER not null,
  cascade_id           NUMBER,
  control_port         NUMBER,
  control_unit_id      NUMBER,
  data_port            NUMBER,
  index_code           VARCHAR2(64),
  parent_index_code    VARCHAR2(256),
  ip_addr              VARCHAR2(64),
  crossing_server_name VARCHAR2(64),
  net_port             NUMBER,
  net_zone_id          NUMBER,
  image_server_id      NUMBER
)
;
alter table BMS_CROSSING_SERVER
  add constraint PK_BMS_CROSSING_SERVER primary key (CROSSING_SERVER_ID);

prompt
prompt Creating table BMS_CROSSING_INFO
prompt ================================
prompt
create table BMS_CROSSING_INFO
(
  crossing_id         NUMBER not null,
  altitude            VARCHAR2(64),
  cascade_id          NUMBER,
  control_unit_id     NUMBER,
  crossing_mode       NUMBER,
  crossing_type       NUMBER,
  enable_relate       NUMBER,
  front_type          NUMBER,
  crossing_index_code VARCHAR2(64),
  intercity           NUMBER,
  internal_code       NUMBER,
  lane_num            NUMBER,
  latitude            VARCHAR2(64),
  longitude           VARCHAR2(64),
  crossing_name       VARCHAR2(256),
  photo               VARCHAR2(256),
  related_camera_name VARCHAR2(255),
  related_camera_path VARCHAR2(255),
  crossing_server_id  NUMBER,
  jccrossingindexcode VARCHAR2(64)
)
;
alter table BMS_CROSSING_INFO
  add constraint PK_BMS_CROSSING_INFO primary key (CROSSING_ID);
alter table BMS_CROSSING_INFO
  add constraint FK_BMS_CROSSING_INFO foreign key (CROSSING_SERVER_ID)
  references BMS_CROSSING_SERVER (CROSSING_SERVER_ID);

prompt
prompt Creating table BMS_CROSSING_DEVICE
prompt ==================================
prompt
create table BMS_CROSSING_DEVICE
(
  crossing_device_id         NUMBER not null,
  cascade_id                 NUMBER,
  crossing_device_index_code VARCHAR2(255),
  ip_addr                    VARCHAR2(255),
  crossing_device_name       VARCHAR2(255),
  network_port               NUMBER,
  crossing_id                NUMBER,
  crossing_server_id         NUMBER
)
;
alter table BMS_CROSSING_DEVICE
  add constraint PK_BMS_CROSSING_DEVICE primary key (CROSSING_DEVICE_ID);
alter table BMS_CROSSING_DEVICE
  add constraint FK_BMS_CROSSING_DEVICE1 foreign key (CROSSING_SERVER_ID)
  references BMS_CROSSING_SERVER (CROSSING_SERVER_ID);
alter table BMS_CROSSING_DEVICE
  add constraint FK_BMS_CROSSING_DEVICE2 foreign key (CROSSING_ID)
  references BMS_CROSSING_INFO (CROSSING_ID);

prompt
prompt Creating table BMS_LANE_INFO
prompt ============================
prompt
create table BMS_LANE_INFO
(
  lane_id                   NUMBER not null,
  cascade_id                NUMBER,
  direction_name            VARCHAR2(256),
  direction_no              NUMBER,
  enable_relate             NUMBER,
  lane_index_code           VARCHAR2(64),
  lane_number               NUMBER,
  lane_type                 NUMBER,
  lane_name                 VARCHAR2(255),
  related_camera_index_code VARCHAR2(255),
  related_camera_name       VARCHAR2(255),
  related_camera_path       VARCHAR2(255),
  speed_limit               NUMBER,
  trigger_type              NUMBER,
  crossing_id               NUMBER,
  jclaneno                  NUMBER,
  directtype                NUMBER
)
;
alter table BMS_LANE_INFO
  add constraint PK_BMS_LANE_INFO primary key (LANE_ID);
alter table BMS_LANE_INFO
  add constraint FK_BMS_LANE_INFO foreign key (CROSSING_ID)
  references BMS_CROSSING_INFO (CROSSING_ID);

prompt
prompt Creating table BMS_CROSSING_CAMERA
prompt ==================================
prompt
create table BMS_CROSSING_CAMERA
(
  crossing_camera_id         NUMBER not null,
  cascade_id                 NUMBER,
  conn_type                  NUMBER,
  crossing_camera_index_code VARCHAR2(64),
  camera_ip                  VARCHAR2(64),
  user_password              VARCHAR2(64),
  camera_port                NUMBER,
  user_name                  VARCHAR2(64),
  crossing_device_id         NUMBER,
  crossing_id                NUMBER,
  bayonet_server_id          NUMBER,
  lane_id                    NUMBER,
  chan_num                   INTEGER default -1
)
;
alter table BMS_CROSSING_CAMERA
  add constraint PK_BMS_CROSSING_CAMERA primary key (CROSSING_CAMERA_ID);
alter table BMS_CROSSING_CAMERA
  add constraint FK_BMS_CROSSING_CAMERA1 foreign key (CROSSING_DEVICE_ID)
  references BMS_CROSSING_DEVICE (CROSSING_DEVICE_ID);
alter table BMS_CROSSING_CAMERA
  add constraint FK_BMS_CROSSING_CAMERA2 foreign key (BAYONET_SERVER_ID)
  references BMS_CROSSING_SERVER (CROSSING_SERVER_ID);
alter table BMS_CROSSING_CAMERA
  add constraint FK_BMS_CROSSING_CAMERA3 foreign key (CROSSING_ID)
  references BMS_CROSSING_INFO (CROSSING_ID);
alter table BMS_CROSSING_CAMERA
  add constraint FK_BMS_CROSSING_CAMERA4 foreign key (LANE_ID)
  references BMS_LANE_INFO (LANE_ID);

prompt
prompt Creating table BMS_CROSSING_CAMERA_MAP
prompt ======================================
prompt
create table BMS_CROSSING_CAMERA_MAP
(
  crossing_camera_map_id    NUMBER not null,
  related_camera_index_code VARCHAR2(64),
  crossing_id               NUMBER
)
;
alter table BMS_CROSSING_CAMERA_MAP
  add constraint PK_BMS_CROSSING_CAMERA_MAP primary key (CROSSING_CAMERA_MAP_ID);
alter table BMS_CROSSING_CAMERA_MAP
  add constraint FK_BMS_CROSSING_CAMERA_MAP foreign key (CROSSING_ID)
  references BMS_CROSSING_INFO (CROSSING_ID);

prompt
prompt Creating table BMS_CROSSING_INTERVAL
prompt ====================================
prompt
create table BMS_CROSSING_INTERVAL
(
  interval_id       NUMBER(10) not null,
  crossing_start    NUMBER(10),
  crossing_stop     NUMBER(10),
  control_unit_id   NUMBER(10),
  interval_distance NUMBER(10),
  max_speed         NUMBER(10),
  max_time          NUMBER(10),
  min_speed         NUMBER(10),
  min_time          NUMBER(10),
  interval_name     VARCHAR2(256 CHAR)
)
;
comment on column BMS_CROSSING_INTERVAL.interval_id
  is '主键';
comment on column BMS_CROSSING_INTERVAL.crossing_start
  is '起始卡口';
comment on column BMS_CROSSING_INTERVAL.crossing_stop
  is '结束卡口';
comment on column BMS_CROSSING_INTERVAL.control_unit_id
  is '控制中心';
comment on column BMS_CROSSING_INTERVAL.interval_distance
  is '区间距离';
comment on column BMS_CROSSING_INTERVAL.max_speed
  is '最大速度';
comment on column BMS_CROSSING_INTERVAL.max_time
  is '最大行驶时间';
comment on column BMS_CROSSING_INTERVAL.min_speed
  is '最小速度';
comment on column BMS_CROSSING_INTERVAL.min_time
  is '最小行驶时间';
comment on column BMS_CROSSING_INTERVAL.interval_name
  is '区间名称';
alter table BMS_CROSSING_INTERVAL
  add constraint PK_BMS_CROSSING_INTERVAL primary key (INTERVAL_ID);

prompt
prompt Creating table BMS_DATA_DICTIONARY
prompt ==================================
prompt
create table BMS_DATA_DICTIONARY
(
  type_list_id          NUMBER not null,
  type_list_code        NUMBER,
  type_list_group       VARCHAR2(64),
  type_list_name        VARCHAR2(64),
  type_list_remark      VARCHAR2(64),
  type_list_update_time TIMESTAMP(6)
)
;
alter table BMS_DATA_DICTIONARY
  add constraint PK_BMS_DATA_DICTIONARY primary key (TYPE_LIST_ID);

prompt
prompt Creating table BMS_ELECTROMOBILE
prompt ================================
prompt
create table BMS_ELECTROMOBILE
(
  electromobile_id NUMBER not null,
  crossing_id      NUMBER,
  image_server_id  NUMBER,
  pass_time        TIMESTAMP(6),
  pic_abbreviate   VARCHAR2(256),
  pic_plate        VARCHAR2(256),
  pic_vehicle      VARCHAR2(256),
  serial_number    VARCHAR2(255)
)
;
alter table BMS_ELECTROMOBILE
  add constraint PK_BMS_ELECTROMOBILE primary key (ELECTROMOBILE_ID);

prompt
prompt Creating table BMS_FIRST_TIME_CROSS
prompt ===================================
prompt
create table BMS_FIRST_TIME_CROSS
(
  first_time_cross_id NUMBER not null,
  crossing_id         NUMBER,
  crossing_name       VARCHAR2(255)
)
;
alter table BMS_FIRST_TIME_CROSS
  add constraint PK_BMS_FIRST_TIME_CROSS primary key (FIRST_TIME_CROSS_ID);

prompt
prompt Creating table BMS_IMAGE_SERVER
prompt ===============================
prompt
create table BMS_IMAGE_SERVER
(
  image_server_id   NUMBER not null,
  cascade_id        NUMBER,
  control_port      NUMBER,
  control_unit_id   NUMBER,
  data_port         NUMBER,
  index_code        VARCHAR2(64),
  ip_addr           VARCHAR2(64),
  image_server_name VARCHAR2(64),
  net_port          NUMBER,
  net_zone_id       NUMBER
)
;
alter table BMS_IMAGE_SERVER
  add constraint PK_BMS_IMAGE_SERVER primary key (IMAGE_SERVER_ID);

prompt
prompt Creating table BMS_OPERATION_LOG
prompt ================================
prompt
create table BMS_OPERATION_LOG
(
  operation_log_id NUMBER not null,
  create_time      TIMESTAMP(6),
  ip               VARCHAR2(255),
  log_txt          VARCHAR2(2000),
  menu             VARCHAR2(255),
  user_name        VARCHAR2(255)
)
;
alter table BMS_OPERATION_LOG
  add constraint PK_BMS_OPERATION_LOG primary key (OPERATION_LOG_ID);

prompt
prompt Creating table BMS_PLATE_ALARM
prompt ==============================
prompt
create table BMS_PLATE_ALARM
(
  plate_alarm_id   NUMBER not null,
  alarm_type       NUMBER,
  contect_info     VARCHAR2(255),
  crossing_id      NUMBER,
  identify         NUMBER,
  plate_info       VARCHAR2(64),
  reason           NUMBER,
  alarm_remark     VARCHAR2(256),
  alarm_start_time TIMESTAMP(6),
  alarm_stop_time  TIMESTAMP(6),
  alarm_person     NUMBER,
  user_name        VARCHAR2(255)
)
;
alter table BMS_PLATE_ALARM
  add constraint PK_BMS_PLATE_ALARM primary key (PLATE_ALARM_ID);

prompt
prompt Creating table BMS_UNALARM_INFO
prompt ===============================
prompt
create table BMS_UNALARM_INFO
(
  un_alarm_id     NUMBER not null,
  cancel_time     TIMESTAMP(6),
  cancel_reason   VARCHAR2(256),
  user_name       VARCHAR2(255),
  plate_alarm_id  NUMBER,
  un_alarm_person NUMBER
)
;
alter table BMS_UNALARM_INFO
  add constraint PK_BMS_UNALARM_INFO primary key (UN_ALARM_ID);
alter table BMS_UNALARM_INFO
  add constraint FK_BMS_UNALARM_INFO foreign key (PLATE_ALARM_ID)
  references BMS_PLATE_ALARM (PLATE_ALARM_ID);

prompt
prompt Creating table BMS_VEHICLE_AUDIT
prompt ================================
prompt
create table BMS_VEHICLE_AUDIT
(
  vehicle_audit_id NUMBER not null,
  alarm_action     NUMBER,
  audit_time       TIMESTAMP(6),
  audit_user_id    NUMBER,
  veh_color_depth  NUMBER,
  confirm_time     TIMESTAMP(6),
  confirm_user_id  NUMBER,
  crossing_id      NUMBER,
  image_server_id  NUMBER,
  lane_id          NUMBER,
  pass_time        TIMESTAMP(6),
  pic_abbreviate   VARCHAR2(256),
  pic_plate        VARCHAR2(256),
  pic_vehicle_1    VARCHAR2(256),
  pic_vehicle_2    VARCHAR2(256),
  pic_vehicle_3    VARCHAR2(256),
  pic_vehicle_4    VARCHAR2(256),
  plate_color      NUMBER,
  plate_info       VARCHAR2(64),
  plate_type       NUMBER,
  update_time      TIMESTAMP(6),
  vehicle_audit    NUMBER,
  vehicle_color    NUMBER,
  vehicle_speed    NUMBER,
  vehicle_state    NUMBER,
  vehicle_type     NUMBER
)
;
alter table BMS_VEHICLE_AUDIT
  add constraint PK_BMS_VEHICLE_AUDIT primary key (VEHICLE_AUDIT_ID);

prompt
prompt Creating table BMS_VEHICLE_INTERVAL
prompt ===================================
prompt
create table BMS_VEHICLE_INTERVAL
(
  vehicle_id           NUMBER(10) not null,
  crossing_start       NUMBER(10),
  crossing_stop        NUMBER(10),
  image_server_id      NUMBER(10),
  pass_time_start      TIMESTAMP(6),
  pass_time_stop       TIMESTAMP(6),
  pic_abbreviate       VARCHAR2(256 CHAR),
  pic_plate            VARCHAR2(256 CHAR),
  pic_vehicle          VARCHAR2(256 CHAR),
  plate_info           VARCHAR2(64 CHAR),
  vehicle_speed        NUMBER(10),
  crossing_interval_id NUMBER(10),
  pass_time            TIMESTAMP(6)
)
;
comment on column BMS_VEHICLE_INTERVAL.vehicle_id
  is '过车ID';
comment on column BMS_VEHICLE_INTERVAL.crossing_start
  is '卡口区间起始';
comment on column BMS_VEHICLE_INTERVAL.crossing_stop
  is '卡口区间结束';
comment on column BMS_VEHICLE_INTERVAL.image_server_id
  is '图片服务器ID';
comment on column BMS_VEHICLE_INTERVAL.pass_time_start
  is '起始时间';
comment on column BMS_VEHICLE_INTERVAL.pass_time_stop
  is '结束时间';
comment on column BMS_VEHICLE_INTERVAL.pic_abbreviate
  is '略缩图';
comment on column BMS_VEHICLE_INTERVAL.pic_plate
  is '车牌图片';
comment on column BMS_VEHICLE_INTERVAL.pic_vehicle
  is '过车图片';
comment on column BMS_VEHICLE_INTERVAL.plate_info
  is '车牌号码';
comment on column BMS_VEHICLE_INTERVAL.vehicle_speed
  is '过车速度';
comment on column BMS_VEHICLE_INTERVAL.crossing_interval_id
  is '区间ID';
comment on column BMS_VEHICLE_INTERVAL.pass_time
  is '中间时间';
create index INDEX_VEHICLE_INTERVAL_STOP on BMS_VEHICLE_INTERVAL (PASS_TIME_STOP);
alter table BMS_VEHICLE_INTERVAL
  add constraint PK_BMS_VEHICLE_INTERVAL primary key (VEHICLE_ID);

prompt
prompt Creating table BMS_VEHICLE_PASS
prompt ===============================
prompt
create table BMS_VEHICLE_PASS
(
  vehicle_id      NUMBER not null,
  veh_color_depth NUMBER,
  crossing_id     NUMBER,
  image_server_id NUMBER,
  lane_id         NUMBER,
  pass_time       TIMESTAMP(6),
  pic_abbreviate  VARCHAR2(256),
  pic_plate       VARCHAR2(256),
  pic_vehicle     VARCHAR2(256),
  plate_color     NUMBER,
  plate_info      VARCHAR2(64),
  plate_type      NUMBER,
  update_time     TIMESTAMP(6) default SYSDATE,
  vehicle_audit   NUMBER default 0,
  vehicle_color   NUMBER,
  vehicle_speed   NUMBER,
  vehicle_state   NUMBER,
  vehicle_type    NUMBER
)
partition by range (PASS_TIME)
(
  partition P_12_01 values less than (TIMESTAMP' 2012-02-01 00:00:00')
    tablespace USERS,
  partition P_12_02 values less than (TIMESTAMP' 2012-03-01 00:00:00')
    tablespace USERS,
  partition P_12_03 values less than (TIMESTAMP' 2012-04-01 00:00:00')
    tablespace USERS,
  partition P_12_04 values less than (TIMESTAMP' 2012-05-01 00:00:00')
    tablespace USERS,
  partition P_12_05 values less than (TIMESTAMP' 2012-06-01 00:00:00')
    tablespace USERS,
  partition P_12_06 values less than (TIMESTAMP' 2012-07-01 00:00:00')
    tablespace USERS,
  partition P_12_07 values less than (TIMESTAMP' 2012-08-01 00:00:00')
    tablespace USERS,
  partition P_12_08 values less than (TIMESTAMP' 2012-09-01 00:00:00')
    tablespace USERS,
  partition P_12_09 values less than (TIMESTAMP' 2012-10-01 00:00:00')
    tablespace USERS,
  partition P_12_10 values less than (TIMESTAMP' 2012-11-01 00:00:00')
    tablespace USERS,
  partition P_12_11 values less than (TIMESTAMP' 2012-12-01 00:00:00')
    tablespace USERS,
  partition P_12_12 values less than (TIMESTAMP' 2013-01-01 00:00:00')
    tablespace USERS,
  partition P_13_01 values less than (TIMESTAMP' 2013-02-01 00:00:00')
    tablespace USERS,
  partition P_13_02 values less than (TIMESTAMP' 2013-03-01 00:00:00')
    tablespace USERS,
  partition P_13_03 values less than (TIMESTAMP' 2013-04-01 00:00:00')
    tablespace USERS,
  partition P_13_04 values less than (TIMESTAMP' 2013-05-01 00:00:00')
    tablespace USERS,
  partition P_13_05 values less than (TIMESTAMP' 2013-06-01 00:00:00')
    tablespace USERS,
  partition P_13_06 values less than (TIMESTAMP' 2013-07-01 00:00:00')
    tablespace USERS,
  partition P_13_07 values less than (TIMESTAMP' 2013-08-01 00:00:00')
    tablespace USERS,
  partition P_13_08 values less than (TIMESTAMP' 2013-09-01 00:00:00')
    tablespace USERS,
  partition P_13_09 values less than (TIMESTAMP' 2013-10-01 00:00:00')
    tablespace USERS,
  partition P_13_10 values less than (TIMESTAMP' 2013-11-01 00:00:00')
    tablespace USERS,
  partition P_13_11 values less than (TIMESTAMP' 2013-12-01 00:00:00')
    tablespace USERS,
  partition P_13_12 values less than (TIMESTAMP' 2014-01-01 00:00:00')
    tablespace USERS,
  partition P_14_01 values less than (TIMESTAMP' 2014-02-01 00:00:00')
    tablespace USERS,
  partition P_14_02 values less than (TIMESTAMP' 2014-03-01 00:00:00')
    tablespace USERS,
  partition P_14_03 values less than (TIMESTAMP' 2014-04-01 00:00:00')
    tablespace USERS,
  partition P_14_04 values less than (TIMESTAMP' 2014-05-01 00:00:00')
    tablespace USERS,
  partition P_14_05 values less than (TIMESTAMP' 2014-06-01 00:00:00')
    tablespace USERS,
  partition P_14_06 values less than (TIMESTAMP' 2014-07-01 00:00:00')
    tablespace USERS,
  partition P_14_07 values less than (TIMESTAMP' 2014-08-01 00:00:00')
    tablespace USERS,
  partition P_14_08 values less than (TIMESTAMP' 2014-09-01 00:00:00')
    tablespace USERS,
  partition P_14_09 values less than (TIMESTAMP' 2014-10-01 00:00:00')
    tablespace USERS,
  partition P_14_10 values less than (TIMESTAMP' 2014-11-01 00:00:00')
    tablespace USERS,
  partition P_14_11 values less than (TIMESTAMP' 2014-12-01 00:00:00')
    tablespace USERS,
  partition P_14_12 values less than (TIMESTAMP' 2015-01-01 00:00:00')
    tablespace USERS,
  partition P_15_01 values less than (TIMESTAMP' 2015-02-01 00:00:00')
    tablespace USERS,
  partition P_15_02 values less than (TIMESTAMP' 2015-03-01 00:00:00')
    tablespace USERS,
  partition P_15_03 values less than (TIMESTAMP' 2015-04-01 00:00:00')
    tablespace USERS,
  partition P_15_04 values less than (TIMESTAMP' 2015-05-01 00:00:00')
    tablespace USERS,
  partition P_15_05 values less than (TIMESTAMP' 2015-06-01 00:00:00')
    tablespace USERS,
  partition P_15_06 values less than (TIMESTAMP' 2015-07-01 00:00:00')
    tablespace USERS,
  partition P_15_07 values less than (TIMESTAMP' 2015-08-01 00:00:00')
    tablespace USERS,
  partition P_15_08 values less than (TIMESTAMP' 2015-09-01 00:00:00')
    tablespace USERS,
  partition P_15_09 values less than (TIMESTAMP' 2015-10-01 00:00:00')
    tablespace USERS,
  partition P_15_10 values less than (TIMESTAMP' 2015-11-01 00:00:00')
    tablespace USERS,
  partition P_15_11 values less than (TIMESTAMP' 2015-12-01 00:00:00')
    tablespace USERS,
  partition P_15_12 values less than (TIMESTAMP' 2016-01-01 00:00:00')
    tablespace USERS
);
create index INDEX_VEHICLE_PASS on BMS_VEHICLE_PASS (PASS_TIME, CROSSING_ID, PLATE_INFO)
  local;
create index INDEX_VEHICLE_PASS_UPDATE on BMS_VEHICLE_PASS (UPDATE_TIME);
alter table BMS_VEHICLE_PASS
  add constraint PK_BMS_VEHICLE_PASS primary key (VEHICLE_ID);

prompt
prompt Creating table BMS_VEHICLE_PASS_FIRST
prompt =====================================
prompt
create table BMS_VEHICLE_PASS_FIRST
(
  vehicle_id      NUMBER not null,
  veh_color_depth NUMBER,
  crossing_id     NUMBER,
  image_server_id NUMBER,
  lane_id         NUMBER,
  pass_time       TIMESTAMP(6),
  pic_abbreviate  VARCHAR2(256),
  pic_plate       VARCHAR2(256),
  pic_vehicle     VARCHAR2(256),
  plate_color     NUMBER,
  plate_info      VARCHAR2(64),
  plate_type      NUMBER,
  vehicle_color   NUMBER,
  vehicle_speed   NUMBER,
  vehicle_state   NUMBER,
  vehicle_type    NUMBER
)
;
create index INDEX_VEHICLE_PASS_FIRST on BMS_VEHICLE_PASS_FIRST (PASS_TIME, CROSSING_ID, PLATE_INFO);
create unique index U_PLATE_INFO on BMS_VEHICLE_PASS_FIRST (PLATE_INFO);
alter table BMS_VEHICLE_PASS_FIRST
  add constraint PK_BMS_VEHICLE_PASS_FIRST primary key (VEHICLE_ID);

prompt
prompt Creating table BMS_VEHICLE_VIOLATION
prompt ====================================
prompt
create table BMS_VEHICLE_VIOLATION
(
  vehicle_violation_id NUMBER not null,
  alarm_action         NUMBER,
  veh_color_depth      NUMBER,
  crossing_id          NUMBER,
  image_server_id      NUMBER,
  lane_id              NUMBER,
  alarm_time           TIMESTAMP(6),
  pic_abbreviate       VARCHAR2(256),
  pic_plate            VARCHAR2(256),
  pic_vehicle_1        VARCHAR2(256),
  pic_vehicle_2        VARCHAR2(256),
  pic_vehicle_3        VARCHAR2(256),
  pic_vehicle_4        VARCHAR2(256),
  plate_color          NUMBER,
  plate_info           VARCHAR2(64),
  plate_type           NUMBER,
  update_time          TIMESTAMP(6) default SYSDATE,
  vehicle_audit        NUMBER default 0,
  vehicle_color        NUMBER,
  vehicle_id           NUMBER,
  vehicle_speed        NUMBER,
  vehicle_state        NUMBER,
  vehicle_type         NUMBER
)
partition by range (ALARM_TIME)
(
  partition V_12_01 values less than (TIMESTAMP' 2012-02-01 00:00:00')
    tablespace USERS,
  partition V_12_02 values less than (TIMESTAMP' 2012-03-01 00:00:00')
    tablespace USERS,
  partition V_12_03 values less than (TIMESTAMP' 2012-04-01 00:00:00')
    tablespace USERS,
  partition V_12_04 values less than (TIMESTAMP' 2012-05-01 00:00:00')
    tablespace USERS,
  partition V_12_05 values less than (TIMESTAMP' 2012-06-01 00:00:00')
    tablespace USERS,
  partition V_12_06 values less than (TIMESTAMP' 2012-07-01 00:00:00')
    tablespace USERS,
  partition V_12_07 values less than (TIMESTAMP' 2012-08-01 00:00:00')
    tablespace USERS,
  partition V_12_08 values less than (TIMESTAMP' 2012-09-01 00:00:00')
    tablespace USERS,
  partition V_12_09 values less than (TIMESTAMP' 2012-10-01 00:00:00')
    tablespace USERS,
  partition V_12_10 values less than (TIMESTAMP' 2012-11-01 00:00:00')
    tablespace USERS,
  partition V_12_11 values less than (TIMESTAMP' 2012-12-01 00:00:00')
    tablespace USERS,
  partition V_12_12 values less than (TIMESTAMP' 2013-01-01 00:00:00')
    tablespace USERS,
  partition V_13_01 values less than (TIMESTAMP' 2013-02-01 00:00:00')
    tablespace USERS,
  partition V_13_02 values less than (TIMESTAMP' 2013-03-01 00:00:00')
    tablespace USERS,
  partition V_13_03 values less than (TIMESTAMP' 2013-04-01 00:00:00')
    tablespace USERS,
  partition V_13_04 values less than (TIMESTAMP' 2013-05-01 00:00:00')
    tablespace USERS,
  partition V_13_05 values less than (TIMESTAMP' 2013-06-01 00:00:00')
    tablespace USERS,
  partition V_13_06 values less than (TIMESTAMP' 2013-07-01 00:00:00')
    tablespace USERS,
  partition V_13_07 values less than (TIMESTAMP' 2013-08-01 00:00:00')
    tablespace USERS,
  partition V_13_08 values less than (TIMESTAMP' 2013-09-01 00:00:00')
    tablespace USERS,
  partition V_13_09 values less than (TIMESTAMP' 2013-10-01 00:00:00')
    tablespace USERS,
  partition V_13_10 values less than (TIMESTAMP' 2013-11-01 00:00:00')
    tablespace USERS,
  partition V_13_11 values less than (TIMESTAMP' 2013-12-01 00:00:00')
    tablespace USERS,
  partition V_13_12 values less than (TIMESTAMP' 2014-01-01 00:00:00')
    tablespace USERS,
  partition V_14_01 values less than (TIMESTAMP' 2014-02-01 00:00:00')
    tablespace USERS,
  partition V_14_02 values less than (TIMESTAMP' 2014-03-01 00:00:00')
    tablespace USERS,
  partition V_14_03 values less than (TIMESTAMP' 2014-04-01 00:00:00')
    tablespace USERS,
  partition V_14_04 values less than (TIMESTAMP' 2014-05-01 00:00:00')
    tablespace USERS,
  partition V_14_05 values less than (TIMESTAMP' 2014-06-01 00:00:00')
    tablespace USERS,
  partition V_14_06 values less than (TIMESTAMP' 2014-07-01 00:00:00')
    tablespace USERS,
  partition V_14_07 values less than (TIMESTAMP' 2014-08-01 00:00:00')
    tablespace USERS,
  partition V_14_08 values less than (TIMESTAMP' 2014-09-01 00:00:00')
    tablespace USERS,
  partition V_14_09 values less than (TIMESTAMP' 2014-10-01 00:00:00')
    tablespace USERS,
  partition V_14_10 values less than (TIMESTAMP' 2014-11-01 00:00:00')
    tablespace USERS,
  partition V_14_11 values less than (TIMESTAMP' 2014-12-01 00:00:00')
    tablespace USERS,
  partition V_14_12 values less than (TIMESTAMP' 2015-01-01 00:00:00')
    tablespace USERS,
  partition V_15_01 values less than (TIMESTAMP' 2015-02-01 00:00:00')
    tablespace USERS,
  partition V_15_02 values less than (TIMESTAMP' 2015-03-01 00:00:00')
    tablespace USERS,
  partition V_15_03 values less than (TIMESTAMP' 2015-04-01 00:00:00')
    tablespace USERS,
  partition V_15_04 values less than (TIMESTAMP' 2015-05-01 00:00:00')
    tablespace USERS,
  partition V_15_05 values less than (TIMESTAMP' 2015-06-01 00:00:00')
    tablespace USERS,
  partition V_15_06 values less than (TIMESTAMP' 2015-07-01 00:00:00')
    tablespace USERS,
  partition V_15_07 values less than (TIMESTAMP' 2015-08-01 00:00:00')
    tablespace USERS,
  partition V_15_08 values less than (TIMESTAMP' 2015-09-01 00:00:00')
    tablespace USERS,
  partition V_15_09 values less than (TIMESTAMP' 2015-10-01 00:00:00')
    tablespace USERS,
  partition V_15_10 values less than (TIMESTAMP' 2015-11-01 00:00:00')
    tablespace USERS,
  partition V_15_11 values less than (TIMESTAMP' 2015-12-01 00:00:00')
    tablespace USERS,
  partition V_15_12 values less than (TIMESTAMP' 2016-01-01 00:00:00')
    tablespace USERS
);
create index INDEX_VEHICLE_VIOLATION on BMS_VEHICLE_VIOLATION (ALARM_TIME, CROSSING_ID, PLATE_INFO)
  local;
create index INDEX_VEHICLE_VIO_UPDATE on BMS_VEHICLE_VIOLATION (UPDATE_TIME);
alter table BMS_VEHICLE_VIOLATION
  add constraint PK_BMS_VEHICLE_VIOLATION primary key (VEHICLE_VIOLATION_ID);

prompt
prompt Creating table BULLETIN
prompt =======================
prompt
create table BULLETIN
(
  bulletin_id      NUMBER not null,
  titile           VARCHAR2(128) not null,
  content          VARCHAR2(1024) not null,
  begin_valid_time TIMESTAMP(6) not null,
  end_valid_time   TIMESTAMP(6) not null,
  isvalid          NUMBER(1),
  operator_id      NUMBER not null,
  create_time      TIMESTAMP(6) not null,
  update_time      TIMESTAMP(6) not null
)
;
comment on column BULLETIN.titile
  is '公告标题';
comment on column BULLETIN.content
  is '公告内容';
comment on column BULLETIN.begin_valid_time
  is '有效开始时间';
comment on column BULLETIN.end_valid_time
  is '有效结束时间';
comment on column BULLETIN.isvalid
  is '是否有效的标志';
comment on column BULLETIN.operator_id
  is '操作员ID';
comment on column BULLETIN.create_time
  is '创建时间';
alter table BULLETIN
  add constraint PK_BULLETIN primary key (BULLETIN_ID);

prompt
prompt Creating table BULLETIN_SCOPE
prompt =============================
prompt
create table BULLETIN_SCOPE
(
  control_unit_id NUMBER not null,
  bulletin_id     NUMBER not null
)
;
comment on column BULLETIN_SCOPE.control_unit_id
  is '所属的控制单元';
comment on column BULLETIN_SCOPE.bulletin_id
  is 'ID';
alter table BULLETIN_SCOPE
  add constraint PK_BULLETIN_SCOPE primary key (BULLETIN_ID, CONTROL_UNIT_ID);
alter table BULLETIN_SCOPE
  add constraint FK_BULLETIN_SCOPE1 foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);
alter table BULLETIN_SCOPE
  add constraint FK_BULLETIN_SCOPE2 foreign key (BULLETIN_ID)
  references BULLETIN (BULLETIN_ID);

prompt
prompt Creating table CAMERA_COLLECTION_MCU
prompt ====================================
prompt
create table CAMERA_COLLECTION_MCU
(
  group_id    NUMBER not null,
  camera_id   NUMBER not null,
  operator_id NUMBER not null,
  create_time TIMESTAMP(6) not null
)
;
comment on column CAMERA_COLLECTION_MCU.camera_id
  is '监控点编号';
comment on column CAMERA_COLLECTION_MCU.operator_id
  is '操作预员ID,如果是根用户则为0';
alter table CAMERA_COLLECTION_MCU
  add constraint PK_CAMERA_COLLECTION_MCU primary key (GROUP_ID);

prompt
prompt Creating table CAMERA_CONFIG
prompt ============================
prompt
create table CAMERA_CONFIG
(
  camera_id       NUMBER not null,
  xml_config_info CLOB,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column CAMERA_CONFIG.operator_id
  is '操作员ID';
comment on column CAMERA_CONFIG.create_time
  is '创建时间';
comment on column CAMERA_CONFIG.int_rev_1
  is '整型保留字段';
comment on column CAMERA_CONFIG.int_rev_2
  is '整型保留字段';
comment on column CAMERA_CONFIG.str_rev_1
  is '字符串保留字段';
comment on column CAMERA_CONFIG.str_rev_2
  is '字符串保留字段';
alter table CAMERA_CONFIG
  add constraint PK_CAMERA_CONFIG primary key (CAMERA_ID);
alter table CAMERA_CONFIG
  add constraint FK_CAMERA_CONFIG foreign key (CAMERA_ID)
  references CAMERA_INFO (CAMERA_ID);

prompt
prompt Creating table CASCADE_SHARE
prompt ============================
prompt
create table CASCADE_SHARE
(
  share_id         NUMBER not null,
  object_id        NUMBER,
  object_type      NUMBER,
  share_index_code VARCHAR2(128),
  privilege_code   VARCHAR2(128),
  cms_cascade_id   NUMBER
)
;
comment on column CASCADE_SHARE.object_id
  is '组织、区域、监控点的编号';
comment on column CASCADE_SHARE.object_type
  is '组织：1、区域：2、监控点：3';
comment on column CASCADE_SHARE.share_index_code
  is '共享编号';
comment on column CASCADE_SHARE.privilege_code
  is '权限码';
comment on column CASCADE_SHARE.cms_cascade_id
  is '级联配置编码';
alter table CASCADE_SHARE
  add constraint PK_CASCADE_SHARE primary key (SHARE_ID);

prompt
prompt Creating table CASE_DANGER_PERSON
prompt =================================
prompt
create table CASE_DANGER_PERSON
(
  id               NUMBER not null,
  alias            VARCHAR2(64),
  birthday         TIMESTAMP(6),
  create_by        NUMBER,
  create_time      TIMESTAMP(6),
  crime_character  CLOB,
  crime_desc       CLOB,
  danger_level     NUMBER,
  danger_type      NUMBER,
  education        NUMBER,
  gender           NUMBER,
  household_regist VARCHAR2(256),
  id_num           VARCHAR2(32),
  live_place       VARCHAR2(256),
  marriage         NUMBER,
  name             VARCHAR2(64),
  nation           VARCHAR2(32),
  phone_num        VARCHAR2(32),
  photo            BLOB,
  regist_from      VARCHAR2(128),
  regist_reason    VARCHAR2(256),
  resume           CLOB,
  update_by        NUMBER,
  update_time      TIMESTAMP(6),
  used_name        VARCHAR2(64),
  work_place       VARCHAR2(256)
)
;
alter table CASE_DANGER_PERSON
  add constraint PK_CASE_DANGER_PERSON primary key (ID);

prompt
prompt Creating table CASE_CONTACT_PEOPLE
prompt ==================================
prompt
create table CASE_CONTACT_PEOPLE
(
  id               NUMBER not null,
  age              NUMBER,
  id_num           VARCHAR2(255),
  name             VARCHAR2(32),
  phone_num        VARCHAR2(255),
  political_status VARCHAR2(8),
  title            VARCHAR2(8),
  wok_place        VARCHAR2(128),
  danger_person_id NUMBER
)
;
alter table CASE_CONTACT_PEOPLE
  add constraint PK_CASE_CONTACT_PEOPLE primary key (ID);
alter table CASE_CONTACT_PEOPLE
  add constraint FK_CASE_CONTACT_PEOPLE foreign key (DANGER_PERSON_ID)
  references CASE_DANGER_PERSON (ID);

prompt
prompt Creating table CASE_HANDLE_NOTE
prompt ===============================
prompt
create table CASE_HANDLE_NOTE
(
  id          NUMBER not null,
  content     CLOB,
  create_by   NUMBER,
  create_time TIMESTAMP(6),
  serial_no   VARCHAR2(255)
)
;
create index INX_HANDLE_NOTE_SERIAL_NO on CASE_HANDLE_NOTE (SERIAL_NO);
alter table CASE_HANDLE_NOTE
  add constraint PK_CASE_HANDLE_NOTE primary key (ID);

prompt
prompt Creating table CASE_RELATION_INFO
prompt =================================
prompt
create table CASE_RELATION_INFO
(
  id                  NUMBER not null,
  create_time         TIMESTAMP(6),
  detection_dept      VARCHAR2(32),
  detection_situation VARCHAR2(1024),
  detection_time      TIMESTAMP(6),
  handle_dept         VARCHAR2(32),
  handler             VARCHAR2(128),
  memo                VARCHAR2(1024),
  reason              VARCHAR2(512),
  serial              VARCHAR2(32),
  status              NUMBER,
  version             NUMBER
)
;
alter table CASE_RELATION_INFO
  add constraint PK_CASE_RELATION_INFO primary key (ID);

prompt
prompt Creating table CASE_INFO
prompt ========================
prompt
create table CASE_INFO
(
  id               NUMBER not null,
  accepted_time    TIMESTAMP(6),
  assist_handler   VARCHAR2(128),
  brief_desc       CLOB,
  case_closed_time TIMESTAMP(6),
  case_no          VARCHAR2(32),
  case_status      NUMBER,
  charge           VARCHAR2(128),
  create_by        NUMBER,
  create_time      TIMESTAMP(6),
  detection_time   TIMESTAMP(6),
  gist             VARCHAR2(128),
  handle_dept      VARCHAR2(128),
  law_officer      VARCHAR2(32),
  major_handler    VARCHAR2(32),
  serial_no        VARCHAR2(32),
  type             NUMBER,
  update_by        NUMBER,
  update_time      TIMESTAMP(6),
  version          NUMBER,
  case_relation_id NUMBER
)
;
alter table CASE_INFO
  add constraint PK_CASE_INFO primary key (ID);
alter table CASE_INFO
  add constraint FK_CASE_INFO foreign key (CASE_RELATION_ID)
  references CASE_RELATION_INFO (ID);

prompt
prompt Creating table CASE_LOG
prompt =======================
prompt
create table CASE_LOG
(
  id             NUMBER not null,
  operation_time TIMESTAMP(6),
  operation_type VARCHAR2(16),
  operation_user NUMBER,
  remark         CLOB,
  serial_no      VARCHAR2(255)
)
;
create index INX_CASE_LOG_SERIAL_NO on CASE_LOG (SERIAL_NO);
alter table CASE_LOG
  add constraint PK_CASE_LOG primary key (ID);

prompt
prompt Creating table CASE_MODULE
prompt ==========================
prompt
create table CASE_MODULE
(
  id    NUMBER not null,
  sort  NUMBER,
  text  VARCHAR2(255),
  url   VARCHAR2(255),
  valid NUMBER,
  p_id  NUMBER
)
;
alter table CASE_MODULE
  add constraint PK_CASE_MODULE primary key (ID);
alter table CASE_MODULE
  add constraint FK_CASE_MODULE foreign key (P_ID)
  references CASE_MODULE (ID);

prompt
prompt Creating table CASE_PERSON_CHARACTER
prompt ====================================
prompt
create table CASE_PERSON_CHARACTER
(
  id               NUMBER not null,
  accent           VARCHAR2(16),
  blood_type       NUMBER,
  dna              VARCHAR2(64),
  face_type        VARCHAR2(8),
  finger_print     VARCHAR2(64),
  foot_length      FLOAT,
  height           FLOAT,
  others           VARCHAR2(64),
  weight           FLOAT,
  danger_person_id NUMBER
)
;
alter table CASE_PERSON_CHARACTER
  add constraint PK_CASE_PERSON_CHARACTER primary key (ID);
alter table CASE_PERSON_CHARACTER
  add constraint FK_CASE_PERSON_CHARACTER foreign key (DANGER_PERSON_ID)
  references CASE_DANGER_PERSON (ID);

prompt
prompt Creating table CASE_REPORT_INFO
prompt ===============================
prompt
create table CASE_REPORT_INFO
(
  id               NUMBER not null,
  alarm_no         VARCHAR2(32),
  brief_desc       VARCHAR2(1024),
  control_unit_id  VARCHAR2(255),
  create_by        NUMBER,
  create_time      TIMESTAMP(6),
  gist             VARCHAR2(128),
  handle_person    VARCHAR2(255),
  handle_result    VARCHAR2(1024),
  handle_suggest   NUMBER,
  handle_time      TIMESTAMP(6),
  happened_address VARCHAR2(255),
  happened_area    NUMBER,
  happened_time    TIMESTAMP(6),
  key_word         VARCHAR2(128),
  report_no        VARCHAR2(32),
  report_time      TIMESTAMP(6),
  report_type      NUMBER,
  serial_no        VARCHAR2(32),
  source           NUMBER,
  status           NUMBER,
  type             NUMBER,
  update_by        NUMBER,
  update_time      TIMESTAMP(6),
  version          NUMBER,
  case_id          NUMBER
)
;
alter table CASE_REPORT_INFO
  add constraint PK_CASE_REPORT_INFO primary key (ID);
alter table CASE_REPORT_INFO
  add constraint FK_CASE_REPORT_INFO foreign key (CASE_ID)
  references CASE_INFO (ID);

prompt
prompt Creating table CASE_REPORTER_INFO
prompt =================================
prompt
create table CASE_REPORTER_INFO
(
  id             NUMBER not null,
  birthday       TIMESTAMP(6),
  gender         NUMBER,
  id_num         VARCHAR2(32),
  live_place     VARCHAR2(512),
  name           VARCHAR2(64),
  tel            VARCHAR2(32),
  work_place     VARCHAR2(512),
  case_report_id NUMBER
)
;
alter table CASE_REPORTER_INFO
  add constraint PK_CASE_REPORTER_INFO primary key (ID);
alter table CASE_REPORTER_INFO
  add constraint FK_CASE_REPORTER_INFO foreign key (CASE_REPORT_ID)
  references CASE_REPORT_INFO (ID);

prompt
prompt Creating table CASE_SUSPECT
prompt ===========================
prompt
create table CASE_SUSPECT
(
  id             NUMBER not null,
  execute_reason VARCHAR2(256),
  execute_status NUMBER,
  execute_time   TIMESTAMP(6),
  id_num         VARCHAR2(32),
  legal_basis    VARCHAR2(256),
  name           VARCHAR2(64),
  release_time   TIMESTAMP(6),
  case_id        NUMBER,
  case_report_id NUMBER
)
;
alter table CASE_SUSPECT
  add constraint PK_CASE_SUSPECT primary key (ID);
alter table CASE_SUSPECT
  add constraint FK_CASE_SUSPECT1 foreign key (CASE_REPORT_ID)
  references CASE_REPORT_INFO (ID);
alter table CASE_SUSPECT
  add constraint FK_CASE_SUSPECT2 foreign key (CASE_ID)
  references CASE_INFO (ID);

prompt
prompt Creating table CASE_UPLOAD_MATERIAL
prompt ===================================
prompt
create table CASE_UPLOAD_MATERIAL
(
  id               NUMBER not null,
  brif_desc        VARCHAR2(1024),
  create_by        NUMBER,
  create_time      TIMESTAMP(6),
  last_modify_time TIMESTAMP(6),
  last_modify_by   NUMBER,
  name             VARCHAR2(512),
  path             VARCHAR2(256),
  serial_no        VARCHAR2(255),
  file_size        VARCHAR2(255),
  type             VARCHAR2(255),
  vrb_server_id    NUMBER
)
;
create index INX_UPLOAD_MAT_SERIAL_NO on CASE_UPLOAD_MATERIAL (SERIAL_NO);
alter table CASE_UPLOAD_MATERIAL
  add constraint PK_CASE_UPLOAD_MATERIAL primary key (ID);

prompt
prompt Creating table CMS_CASCADE
prompt ==========================
prompt
create table CMS_CASCADE
(
  cms_cascade_id NUMBER not null,
  name           VARCHAR2(128),
  db33_ip        VARCHAR2(15),
  db33_port      NUMBER,
  cms_url        VARCHAR2(64),
  user_name      VARCHAR2(64),
  user_pwd       VARCHAR2(64),
  type           NUMBER,
  domain_code    VARCHAR2(128),
  priority       NUMBER,
  net_zone_id    NUMBER,
  status         NUMBER,
  protocol       NUMBER,
  bauth          VARCHAR2(128),
  login          VARCHAR2(128),
  passwd         VARCHAR2(128),
  realm          VARCHAR2(128)
)
;
comment on column CMS_CASCADE.name
  is '名称';
comment on column CMS_CASCADE.user_name
  is '用户名';
comment on column CMS_CASCADE.user_pwd
  is '密码';
comment on column CMS_CASCADE.type
  is '级联类型:0 上级 1 下级';
comment on column CMS_CASCADE.status
  is '状态:0 在线 1 不在线';
alter table CMS_CASCADE
  add constraint PK_CMS_CASCADE primary key (CMS_CASCADE_ID);

prompt
prompt Creating table CMS_MENU
prompt =======================
prompt
create table CMS_MENU
(
  cms_menu_id NUMBER not null,
  menu_code   VARCHAR2(32) not null,
  name        VARCHAR2(64) not null,
  parent_id   NUMBER not null,
  menu_level  NUMBER(5) not null,
  path        VARCHAR2(64),
  create_time TIMESTAMP(6) not null,
  description VARCHAR2(128)
)
;
comment on column CMS_MENU.menu_code
  is '菜单项编码';
comment on column CMS_MENU.name
  is '菜单项名称';
comment on column CMS_MENU.parent_id
  is '父菜单项ID，0代表根菜单项';
comment on column CMS_MENU.menu_level
  is '菜单层次，从0开始编号';
comment on column CMS_MENU.path
  is '菜单路径，以"/"进行分割';
alter table CMS_MENU
  add constraint PK_CMS_MENU primary key (CMS_MENU_ID);

prompt
prompt Creating table DB_NOTIFY
prompt ========================
prompt
create table DB_NOTIFY
(
  db_notify_id NUMBER not null,
  table_name   VARCHAR2(64) not null,
  key_value    NUMBER not null,
  action       NUMBER not null,
  action_time  TIMESTAMP(6) not null
)
;
comment on column DB_NOTIFY.key_value
  is '键值';
comment on column DB_NOTIFY.action
  is '1增加；2删除；3修改';
comment on column DB_NOTIFY.action_time
  is '变动登记时间';
alter table DB_NOTIFY
  add constraint PK_DB_NOTIFY primary key (DB_NOTIFY_ID);

prompt
prompt Creating table DECODE_DEVICE
prompt ============================
prompt
create table DECODE_DEVICE
(
  decode_dev_id   NUMBER not null,
  index_code      VARCHAR2(64) not null,
  name            VARCHAR2(64) not null,
  ip_addr         VARCHAR2(16),
  control_unit_id NUMBER,
  second_ip       VARCHAR2(16),
  register_type   NUMBER,
  dns_addr        VARCHAR2(128),
  dns_port        NUMBER,
  serial_no       VARCHAR2(64),
  net_zone_id     NUMBER not null,
  port            NUMBER,
  user_name       VARCHAR2(50),
  password        VARCHAR2(50),
  decode_mod      NUMBER,
  dev_type        VARCHAR2(50),
  is_get          NUMBER,
  matrix_id       NUMBER,
  pag_server_id   NUMBER,
  status          NUMBER,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column DECODE_DEVICE.ip_addr
  is '解码器IP地址';
comment on column DECODE_DEVICE.control_unit_id
  is '控制单元ID表';
comment on column DECODE_DEVICE.dns_addr
  is 'dns的IP地址';
comment on column DECODE_DEVICE.dns_port
  is 'dns的端口号';
comment on column DECODE_DEVICE.net_zone_id
  is 'net zone id';
comment on column DECODE_DEVICE.port
  is '解码器端口';
comment on column DECODE_DEVICE.user_name
  is '解码器用户名';
comment on column DECODE_DEVICE.password
  is '解码器密码';
comment on column DECODE_DEVICE.decode_mod
  is '解码模式（0：CIF 1：4CIF 2：720P 3: 1080P）';
comment on column DECODE_DEVICE.dev_type
  is '解码器型号';
comment on column DECODE_DEVICE.is_get
  is '是否得到详细信息';
comment on column DECODE_DEVICE.matrix_id
  is '操作员ID';
comment on column DECODE_DEVICE.pag_server_id
  is '关联设备代理服务器id';
comment on column DECODE_DEVICE.status
  is '是否在线，1-在线，2-不在线';
comment on column DECODE_DEVICE.int_rev_1
  is '关联的电视墙的ID';
comment on column DECODE_DEVICE.int_rev_2
  is '关联的视频综合平台的ID';
comment on column DECODE_DEVICE.str_rev_1
  is '字符串保留字段';
comment on column DECODE_DEVICE.str_rev_2
  is '字符串保留字段';
alter table DECODE_DEVICE
  add constraint PK_DECODE_DEVICE primary key (DECODE_DEV_ID);
alter table DECODE_DEVICE
  add constraint UK_DECODE_DEVICE unique (INDEX_CODE);

prompt
prompt Creating table MONITOR_SCREEN_GROUP
prompt ===================================
prompt
create table MONITOR_SCREEN_GROUP
(
  group_id        NUMBER not null,
  control_unit_id NUMBER,
  index_code      VARCHAR2(64) not null,
  group_name      VARCHAR2(64) not null,
  proxy_ip        VARCHAR2(16) not null,
  second_ip       VARCHAR2(16),
  net_zone_id     NUMBER not null,
  proxy_port      NUMBER not null,
  config_port     NUMBER,
  net_agent_port  NUMBER,
  alarm_time      NUMBER,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column MONITOR_SCREEN_GROUP.control_unit_id
  is '控制单元ID表';
comment on column MONITOR_SCREEN_GROUP.group_name
  is '监视屏组名称';
comment on column MONITOR_SCREEN_GROUP.proxy_ip
  is '代理服务器 IP';
comment on column MONITOR_SCREEN_GROUP.net_zone_id
  is 'net zone id';
comment on column MONITOR_SCREEN_GROUP.proxy_port
  is '代理服务器端口';
comment on column MONITOR_SCREEN_GROUP.config_port
  is '配置端口';
comment on column MONITOR_SCREEN_GROUP.net_agent_port
  is '网管端口';
comment on column MONITOR_SCREEN_GROUP.alarm_time
  is '报警持续时间';
comment on column MONITOR_SCREEN_GROUP.operator_id
  is '操作员ID';
comment on column MONITOR_SCREEN_GROUP.create_time
  is '创建时间';
comment on column MONITOR_SCREEN_GROUP.int_rev_1
  is '整型保留字段';
comment on column MONITOR_SCREEN_GROUP.int_rev_2
  is '整型保留字段';
comment on column MONITOR_SCREEN_GROUP.str_rev_1
  is '字符串保留字段';
comment on column MONITOR_SCREEN_GROUP.str_rev_2
  is '字符串保留字段';
create index INDX_MSG_CONTROL_UNIT_ID on MONITOR_SCREEN_GROUP (CONTROL_UNIT_ID);
alter table MONITOR_SCREEN_GROUP
  add constraint PK_MONITOR_SCREEN_GROUP primary key (GROUP_ID);
alter table MONITOR_SCREEN_GROUP
  add constraint UK_MONITOR_SCREEN_GROUP unique (INDEX_CODE);
alter table MONITOR_SCREEN_GROUP
  add constraint FK_MONITOR_SCREEN_GROUP foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table DECODE_RESOURCE
prompt ==============================
prompt
create table DECODE_RESOURCE
(
  decode_res_id   NUMBER not null,
  group_id        NUMBER,
  control_unit_id NUMBER not null,
  type_mod        NUMBER not null,
  decoder_id      NUMBER not null,
  dec_num         NUMBER,
  vga_num         NUMBER,
  bnc_num         NUMBER,
  audio_num       NUMBER,
  dvi_num         NUMBER,
  hdmi_num        NUMBER,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column DECODE_RESOURCE.group_id
  is '监视屏组编号';
comment on column DECODE_RESOURCE.control_unit_id
  is '控制单元ID表';
comment on column DECODE_RESOURCE.type_mod
  is '解码资源类型（0：解码管理服务器;1：解码器;2：矩阵）';
comment on column DECODE_RESOURCE.decoder_id
  is '根据type_mod来决定该ID所对应的表是decode_server还是devode_device';
comment on column DECODE_RESOURCE.dec_num
  is '解码通道个数';
comment on column DECODE_RESOURCE.vga_num
  is 'VGA输出个数';
comment on column DECODE_RESOURCE.bnc_num
  is 'BNC输出个数';
comment on column DECODE_RESOURCE.audio_num
  is '声音输出个数';
comment on column DECODE_RESOURCE.dvi_num
  is 'dvi输出个数';
comment on column DECODE_RESOURCE.hdmi_num
  is 'hdmi输出个数';
comment on column DECODE_RESOURCE.int_rev_1
  is '整型保留字段';
comment on column DECODE_RESOURCE.int_rev_2
  is '整型保留字段';
comment on column DECODE_RESOURCE.str_rev_1
  is '字符串保留字段';
comment on column DECODE_RESOURCE.str_rev_2
  is '字符串保留字段';
create index INDX_DR_CONTROL_UNIT_ID on DECODE_RESOURCE (CONTROL_UNIT_ID);
create index INDX_DR_GROUP_ID on DECODE_RESOURCE (GROUP_ID);
alter table DECODE_RESOURCE
  add constraint PK_DECODE_RESOURCE primary key (DECODE_RES_ID);
alter table DECODE_RESOURCE
  add constraint FK_DECODE_RESOURCE1 foreign key (GROUP_ID)
  references MONITOR_SCREEN_GROUP (GROUP_ID);
alter table DECODE_RESOURCE
  add constraint FK_DECODE_RESOURCE2 foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table DECODE_SERVER
prompt ============================
prompt
create table DECODE_SERVER
(
  decode_svr_id   NUMBER not null,
  index_code      VARCHAR2(64) not null,
  name            VARCHAR2(64) not null,
  control_unit_id NUMBER not null,
  ip_addr         VARCHAR2(16) not null,
  second_ip       VARCHAR2(16),
  net_zone_id     NUMBER not null,
  ctrl_port       NUMBER not null,
  net_agent_port  NUMBER,
  config_port     NUMBER not null,
  data_port       NUMBER not null,
  decode_mod      NUMBER(5) not null,
  work_mod        NUMBER not null,
  video_sys       NUMBER(5) not null,
  real_buf        NUMBER(5) not null,
  playback_buf    NUMBER(5) not null,
  use_soft_dog    NUMBER(1) not null,
  sequence_idx    NUMBER not null,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column DECODE_SERVER.index_code
  is '索引编号';
comment on column DECODE_SERVER.name
  is '服务器名称';
comment on column DECODE_SERVER.control_unit_id
  is '控制单元ID表';
comment on column DECODE_SERVER.net_zone_id
  is 'net zone id';
comment on column DECODE_SERVER.ctrl_port
  is '控制端口';
comment on column DECODE_SERVER.net_agent_port
  is '网管端口';
comment on column DECODE_SERVER.decode_mod
  is '解码模式（0-CIF，1-4CIF）';
comment on column DECODE_SERVER.work_mod
  is '电视墙服务器的工作模式（0：硬解/1：软解）';
comment on column DECODE_SERVER.video_sys
  is '视频制式（0-PAL，1-NTSC）';
comment on column DECODE_SERVER.real_buf
  is '实时缓冲（0-4）';
comment on column DECODE_SERVER.playback_buf
  is '回放缓冲（0-4）';
comment on column DECODE_SERVER.use_soft_dog
  is '是否启用软件狗';
comment on column DECODE_SERVER.operator_id
  is '操作员ID';
comment on column DECODE_SERVER.create_time
  is '创建时间';
comment on column DECODE_SERVER.int_rev_1
  is '整型保留字段';
comment on column DECODE_SERVER.int_rev_2
  is '整型保留字段';
comment on column DECODE_SERVER.str_rev_1
  is '字符串保留字段';
comment on column DECODE_SERVER.str_rev_2
  is '字符串保留字段';
alter table DECODE_SERVER
  add constraint PK_DECODE_SERVER primary key (DECODE_SVR_ID);
alter table DECODE_SERVER
  add constraint UK_DECODE_SERVER unique (INDEX_CODE);

prompt
prompt Creating table DEVICE_CONFIG
prompt ============================
prompt
create table DEVICE_CONFIG
(
  device_id       NUMBER not null,
  xml_config_info CLOB,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column DEVICE_CONFIG.operator_id
  is '操作员ID';
comment on column DEVICE_CONFIG.create_time
  is '创建时间';
comment on column DEVICE_CONFIG.int_rev_1
  is '整型保留字段';
comment on column DEVICE_CONFIG.int_rev_2
  is '整型保留字段';
comment on column DEVICE_CONFIG.str_rev_1
  is '字符串保留字段';
comment on column DEVICE_CONFIG.str_rev_2
  is '字符串保留字段';
alter table DEVICE_CONFIG
  add constraint PK_DEVICE_CONFIG primary key (DEVICE_ID);
alter table DEVICE_CONFIG
  add constraint FK_DEVICE_CONFIG foreign key (DEVICE_ID)
  references DEVICE_INFO (DEVICE_ID);

prompt
prompt Creating table VRM_SERVER
prompt =========================
prompt
create table VRM_SERVER
(
  vrm_id             NUMBER not null,
  control_unit_id    NUMBER not null,
  share_flag         NUMBER(5) not null,
  index_code         VARCHAR2(64) not null,
  name               VARCHAR2(64) not null,
  ip_addr            VARCHAR2(16) not null,
  out_ip_addr        VARCHAR2(16) default '0.0.0.0',
  second_ip          VARCHAR2(16),
  net_zone_id        NUMBER not null,
  ctrl_port          NUMBER not null,
  nvt_store_port     NUMBER not null,
  search_port        NUMBER not null,
  rtsp_port          NUMBER,
  net_agent_port     NUMBER,
  rtp_tcp_base_port  NUMBER,
  rtp_udp_base_port  NUMBER,
  udp_port_count     NUMBER,
  web_port           NUMBER,
  total_space        NUMBER not null,
  free_space         NUMBER not null,
  max_record_task    NUMBER(5),
  max_task_per_ipsan NUMBER(5) not null,
  max_vod_per_ipsan  NUMBER(5) not null,
  use_soft_dog       NUMBER(1) not null,
  sequence_idx       NUMBER not null,
  operator_id        NUMBER not null,
  create_time        TIMESTAMP(6) not null,
  update_time        TIMESTAMP(6) not null,
  int_rev_1          NUMBER,
  int_rev_2          NUMBER,
  str_rev_1          VARCHAR2(64),
  str_rev_2          VARCHAR2(128)
)
;
comment on column VRM_SERVER.control_unit_id
  is '控制单元ID';
comment on column VRM_SERVER.index_code
  is '索引编号';
comment on column VRM_SERVER.name
  is '服务器名称';
comment on column VRM_SERVER.net_zone_id
  is 'net zone id';
comment on column VRM_SERVER.ctrl_port
  is '控制端口';
comment on column VRM_SERVER.net_agent_port
  is '网管端口';
comment on column VRM_SERVER.rtp_tcp_base_port
  is 'Tcp方式下RTP的baseport';
comment on column VRM_SERVER.rtp_udp_base_port
  is 'UDP方式下RTP的baseport';
comment on column VRM_SERVER.udp_port_count
  is 'UDP方式下RTP的端口数';
comment on column VRM_SERVER.total_space
  is '存储空间总容量';
comment on column VRM_SERVER.free_space
  is '未分配的存储空间';
comment on column VRM_SERVER.max_record_task
  is '最大的同时录像任务数';
comment on column VRM_SERVER.max_task_per_ipsan
  is '每台IP-SAN最多同时执行的录像任务数';
comment on column VRM_SERVER.use_soft_dog
  is '是否启用软件狗';
comment on column VRM_SERVER.operator_id
  is '操作员ID';
comment on column VRM_SERVER.create_time
  is '创建时间';
comment on column VRM_SERVER.int_rev_1
  is '整型保留字段';
comment on column VRM_SERVER.int_rev_2
  is '整型保留字段';
comment on column VRM_SERVER.str_rev_1
  is '字符串保留字段';
comment on column VRM_SERVER.str_rev_2
  is '字符串保留字段';
create index INDX_VS_CONTROL_UNIT_ID on VRM_SERVER (CONTROL_UNIT_ID);
alter table VRM_SERVER
  add constraint PK_VRM_SERVER primary key (VRM_ID);
alter table VRM_SERVER
  add constraint UK_VRM_SERVER unique (INDEX_CODE);
alter table VRM_SERVER
  add constraint FK_VRM_SERVER foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table IPSAN_INFO
prompt =========================
prompt
create table IPSAN_INFO
(
  ipsan_id        NUMBER not null,
  name            VARCHAR2(64),
  index_code      VARCHAR2(64) not null,
  ip_addr         VARCHAR2(16) not null,
  username        VARCHAR2(64),
  pwd             VARCHAR2(64),
  data_port       NUMBER not null,
  ctrl_port       NUMBER not null,
  vrm_id          NUMBER,
  control_unit_id NUMBER not null,
  operator_id     NUMBER not null,
  net_zone_id     NUMBER,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null
)
;
comment on column IPSAN_INFO.name
  is '名称';
comment on column IPSAN_INFO.index_code
  is '编号';
comment on column IPSAN_INFO.pwd
  is '密码';
comment on column IPSAN_INFO.data_port
  is '数据端口';
comment on column IPSAN_INFO.ctrl_port
  is '控制端口';
comment on column IPSAN_INFO.vrm_id
  is '存储管理服务器ID';
comment on column IPSAN_INFO.control_unit_id
  is '控制单元ID';
comment on column IPSAN_INFO.operator_id
  is '操作员ID';
comment on column IPSAN_INFO.net_zone_id
  is 'net zone id';
comment on column IPSAN_INFO.create_time
  is '创建时间';
comment on column IPSAN_INFO.update_time
  is '更新时间';
alter table IPSAN_INFO
  add constraint PK_IPSAN_INFO primary key (IPSAN_ID);
alter table IPSAN_INFO
  add constraint FK_IPSAN_INFO1 foreign key (VRM_ID)
  references VRM_SERVER (VRM_ID);
alter table IPSAN_INFO
  add constraint FK_IPSAN_INFO2 foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table DEVICE_IPSAN_MAP
prompt ===============================
prompt
create table DEVICE_IPSAN_MAP
(
  device_id       NUMBER not null,
  vrm_id          NUMBER not null,
  ipsan_id        NUMBER not null,
  code_rate       VARCHAR2(64),
  record_day      VARCHAR2(64),
  assign_space    NUMBER,
  status          NUMBER,
  username        VARCHAR2(64),
  pwd             VARCHAR2(64),
  control_unit_id NUMBER not null,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null
)
;
comment on column DEVICE_IPSAN_MAP.vrm_id
  is '存储管理服务器ID';
comment on column DEVICE_IPSAN_MAP.ipsan_id
  is 'ipsan ID';
comment on column DEVICE_IPSAN_MAP.code_rate
  is '码率';
comment on column DEVICE_IPSAN_MAP.record_day
  is '录像天数';
comment on column DEVICE_IPSAN_MAP.assign_space
  is '分配空间';
comment on column DEVICE_IPSAN_MAP.status
  is '1-启用用户名密码，0-不启用';
comment on column DEVICE_IPSAN_MAP.pwd
  is '密码';
comment on column DEVICE_IPSAN_MAP.control_unit_id
  is '控制单元ID';
comment on column DEVICE_IPSAN_MAP.operator_id
  is '操作员ID';
comment on column DEVICE_IPSAN_MAP.create_time
  is '创建时间';
comment on column DEVICE_IPSAN_MAP.update_time
  is '更新时间';
alter table DEVICE_IPSAN_MAP
  add constraint PK_DEVICE_IPSAN_MAP primary key (DEVICE_ID);
alter table DEVICE_IPSAN_MAP
  add constraint FK_DEVICE_IPSAN_MAP1 foreign key (IPSAN_ID)
  references IPSAN_INFO (IPSAN_ID);
alter table DEVICE_IPSAN_MAP
  add constraint FK_DEVICE_IPSAN_MAP2 foreign key (VRM_ID)
  references VRM_SERVER (VRM_ID);
alter table DEVICE_IPSAN_MAP
  add constraint FK_DEVICE_IPSAN_MAP3 foreign key (DEVICE_ID)
  references DEVICE_INFO (DEVICE_ID);
alter table DEVICE_IPSAN_MAP
  add constraint FK_DEVICE_IPSAN_MAP4 foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table DIC_TYPE
prompt =======================
prompt
create table DIC_TYPE
(
  id     NUMBER not null,
  code   VARCHAR2(16),
  status NUMBER default 1 not null,
  text   VARCHAR2(128) not null
)
;
alter table DIC_TYPE
  add constraint PK_DIC_TYPE primary key (ID);

prompt
prompt Creating table DIC_ITEM
prompt =======================
prompt
create table DIC_ITEM
(
  id      NUMBER not null,
  p_id    NUMBER,
  sort    NUMBER default 1 not null,
  status  NUMBER default 1 not null,
  text    VARCHAR2(32) not null,
  value   NUMBER not null,
  type_id NUMBER
)
;
alter table DIC_ITEM
  add constraint PK_DIC_ITEM primary key (ID);
alter table DIC_ITEM
  add constraint FK_DIC_ITEM foreign key (TYPE_ID)
  references DIC_TYPE (ID);

prompt
prompt Creating table ELEMENT_GROUP
prompt ============================
prompt
create table ELEMENT_GROUP
(
  element_group_id NUMBER not null,
  name             VARCHAR2(128) not null,
  region_id        NUMBER
)
;
comment on column ELEMENT_GROUP.region_id
  is '区域ID';
alter table ELEMENT_GROUP
  add constraint PK_ELEMENT_GROUP primary key (ELEMENT_GROUP_ID);
alter table ELEMENT_GROUP
  add constraint FK_ELEMENT_GROUP foreign key (REGION_ID)
  references REGION_INFO (REGION_ID);

prompt
prompt Creating table ELEMENT_GROUP_RESOURCE
prompt =====================================
prompt
create table ELEMENT_GROUP_RESOURCE
(
  element_group_resource_id NUMBER not null,
  group_id                  NUMBER not null,
  type                      NUMBER not null,
  resource_id               NUMBER not null
)
;
comment on column ELEMENT_GROUP_RESOURCE.group_id
  is '所属分组ID';
comment on column ELEMENT_GROUP_RESOURCE.type
  is '元素类型';
comment on column ELEMENT_GROUP_RESOURCE.resource_id
  is '资源ID';
alter table ELEMENT_GROUP_RESOURCE
  add constraint PK_ELEMENT_GROUP_RESOURCE primary key (ELEMENT_GROUP_RESOURCE_ID);
alter table ELEMENT_GROUP_RESOURCE
  add constraint FK_ELEMENT_GROUP_RESOURCE foreign key (GROUP_ID)
  references ELEMENT_GROUP (ELEMENT_GROUP_ID);

prompt
prompt Creating table EMAP_INFO
prompt ========================
prompt
create table EMAP_INFO
(
  emap_id        NUMBER not null,
  parent_emap_id NUMBER not null,
  save_path      VARCHAR2(255) not null,
  max_scale      FLOAT,
  min_scale      FLOAT,
  xml_rev        VARCHAR2(255),
  map_type       NUMBER,
  region_id      NUMBER not null,
  emap_name      VARCHAR2(50) not null,
  description    VARCHAR2(128),
  version        NUMBER not null,
  int_rev_1      NUMBER,
  int_rev_2      NUMBER,
  str_rev_1      VARCHAR2(64),
  str_rev_2      VARCHAR2(128)
)
;
comment on column EMAP_INFO.parent_emap_id
  is '上一级地图流水号';
comment on column EMAP_INFO.save_path
  is '保存路径';
comment on column EMAP_INFO.max_scale
  is '地图最大显示比例';
comment on column EMAP_INFO.min_scale
  is '地图最小显示比例';
comment on column EMAP_INFO.xml_rev
  is 'xml扩展字段';
comment on column EMAP_INFO.map_type
  is '地图类型';
comment on column EMAP_INFO.region_id
  is '区域ID';
comment on column EMAP_INFO.version
  is 'version';
comment on column EMAP_INFO.int_rev_1
  is '整型保留字段';
comment on column EMAP_INFO.int_rev_2
  is '整型保留字段';
comment on column EMAP_INFO.str_rev_1
  is '字符串保留字段';
comment on column EMAP_INFO.str_rev_2
  is '字符串保留字段';
alter table EMAP_INFO
  add constraint PK_EMAP_INFO primary key (EMAP_ID);

prompt
prompt Creating table EMAP_ELEMENT_INFO
prompt ================================
prompt
create table EMAP_ELEMENT_INFO
(
  feature_type         VARCHAR2(31) not null,
  element_id           NUMBER not null,
  x_pos                FLOAT,
  y_pos                FLOAT,
  control_unit_id      NUMBER not null,
  description          VARCHAR2(128),
  element_name         VARCHAR2(64) not null,
  element_sub_type     VARCHAR2(255),
  element_type         NUMBER not null,
  grounding            NUMBER,
  icon_id              NUMBER,
  int_rev_1            NUMBER,
  int_rev_2            NUMBER,
  is_custom_icon       NUMBER(1),
  link_id              NUMBER,
  link_sub_id          NUMBER,
  outline_border_color NUMBER,
  region_id            NUMBER not null,
  shape_xml            CLOB,
  str_rev_1            VARCHAR2(255),
  str_rev_2            VARCHAR2(255),
  style_1              NUMBER,
  style_2              NUMBER,
  xml_rev              CLOB,
  emap_id              NUMBER not null,
  direction            VARCHAR2(8 CHAR)
)
;
create unique index IDX_ELEMENT_INFO on EMAP_ELEMENT_INFO (LINK_ID, ELEMENT_TYPE);
create index INDX_EEI_EMAP_ID on EMAP_ELEMENT_INFO (EMAP_ID);
alter table EMAP_ELEMENT_INFO
  add constraint PK_EMAP_ELEMENT_INFO primary key (ELEMENT_ID);
alter table EMAP_ELEMENT_INFO
  add constraint FK_EMAP_ELEMENT_INFO foreign key (EMAP_ID)
  references EMAP_INFO (EMAP_ID);

prompt
prompt Creating table EMAP_GROUP
prompt =========================
prompt
create table EMAP_GROUP
(
  group_id    NUMBER not null,
  description VARCHAR2(128),
  emap_id     NUMBER not null,
  group_name  VARCHAR2(128)
)
;
alter table EMAP_GROUP
  add constraint PK_EMAP_GROUP primary key (GROUP_ID);

prompt
prompt Creating table EMAP_PRESET_INFO
prompt ===============================
prompt
create table EMAP_PRESET_INFO
(
  emap_preset_id NUMBER not null,
  x_pos          FLOAT not null,
  y_pos          FLOAT not null,
  gis_id         NUMBER,
  preset_index   NUMBER not null,
  preset_name    VARCHAR2(32) not null,
  user_id        NUMBER,
  xml            VARCHAR2(512),
  zoom_rate      FLOAT not null,
  emap_id        NUMBER not null
)
;
create index INDX_EPI_EMAP_ID on EMAP_PRESET_INFO (EMAP_ID);
alter table EMAP_PRESET_INFO
  add constraint PK_EMAP_PRESET_INFO primary key (EMAP_PRESET_ID);
alter table EMAP_PRESET_INFO
  add constraint FK_EMAP_PRESET_INFO foreign key (EMAP_ID)
  references EMAP_INFO (EMAP_ID);

prompt
prompt Creating table FTP_PICTURE
prompt ==========================
prompt
create table FTP_PICTURE
(
  nlsh          NUMBER not null,
  dvr_id        NUMBER,
  dvr_ip        VARCHAR2(45),
  dvr_name      VARCHAR2(45),
  dvr_port      NUMBER,
  serial_no     VARCHAR2(64) not null,
  trig_channel  NUMBER not null,
  pic_size      NUMBER not null,
  ftp_ip        VARCHAR2(45) not null,
  ftp_file_name VARCHAR2(1024) not null,
  ftp_file_addr VARCHAR2(128) not null,
  cardid        VARCHAR2(45) not null,
  create_time   TIMESTAMP(6) not null,
  update_time   TIMESTAMP(6) not null,
  int_rev_1     NUMBER,
  int_rev_2     NUMBER,
  str_rev_1     VARCHAR2(64),
  str_rev_2     VARCHAR2(128)
)
;
comment on column FTP_PICTURE.create_time
  is '创建时间';
comment on column FTP_PICTURE.int_rev_1
  is '整型保留字段';
comment on column FTP_PICTURE.int_rev_2
  is '整型保留字段';
comment on column FTP_PICTURE.str_rev_1
  is '字符串保留字段';
comment on column FTP_PICTURE.str_rev_2
  is '字符串保留字段';
alter table FTP_PICTURE
  add constraint PK_FTP_PICTURE primary key (NLSH);

prompt
prompt Creating table GPS_INFO
prompt =======================
prompt
create table GPS_INFO
(
  gps_id            NUMBER not null,
  altitude          VARCHAR2(64),
  create_time       TIMESTAMP(6) not null,
  device_index_code VARCHAR2(64) not null,
  direction         VARCHAR2(64) not null,
  int_rev_1         NUMBER,
  int_rev_2         NUMBER,
  latitude          VARCHAR2(64) not null,
  longitude         VARCHAR2(64) not null,
  speed             VARCHAR2(64) not null,
  str_rev_1         VARCHAR2(64),
  str_rev_2         VARCHAR2(128)
)
;
create index IDX_GPS_INFO on GPS_INFO (CREATE_TIME, DEVICE_INDEX_CODE);
alter table GPS_INFO
  add constraint PK_GPS_INFO primary key (GPS_ID);

prompt
prompt Creating table GROUP_ELEMENT
prompt ============================
prompt
create table GROUP_ELEMENT
(
  id            NUMBER not null,
  element_id    NUMBER not null,
  element_order NUMBER,
  group_id      NUMBER not null
)
;
alter table GROUP_ELEMENT
  add constraint PK_GROUP_ELEMENT primary key (ID);

prompt
prompt Creating table GUEST_CONFIG
prompt ===========================
prompt
create table GUEST_CONFIG
(
  guest_config_id NUMBER not null,
  json_config     CLOB,
  user_id         NUMBER
)
;
alter table GUEST_CONFIG
  add constraint PK_GUEST_CONFIG primary key (GUEST_CONFIG_ID);

prompt
prompt Creating table INTER_AREA
prompt =========================
prompt
create table INTER_AREA
(
  inter_area_id   NUMBER not null,
  control_unit_id NUMBER,
  index_code      VARCHAR2(64),
  name            VARCHAR2(64),
  net_zone_id     NUMBER,
  ip_addr         VARCHAR2(20),
  ctrl_port       NUMBER,
  port            NUMBER,
  ua_port         NUMBER,
  net_agent_port  NUMBER,
  client_port     NUMBER,
  stream_port     NUMBER,
  ptz_port        NUMBER,
  type            NUMBER,
  is_route        NUMBER,
  media_ip        VARCHAR2(20),
  media_port      NUMBER,
  user_code       VARCHAR2(64),
  operator_id     NUMBER,
  create_time     TIMESTAMP(6),
  update_time     TIMESTAMP(6)
)
;
comment on column INTER_AREA.control_unit_id
  is '控制单元ID表';
comment on column INTER_AREA.index_code
  is '编号';
comment on column INTER_AREA.name
  is '名称';
comment on column INTER_AREA.net_zone_id
  is '网域ID';
comment on column INTER_AREA.ip_addr
  is '服务器IP';
comment on column INTER_AREA.ctrl_port
  is '配置端口';
comment on column INTER_AREA.port
  is '服务器端口';
comment on column INTER_AREA.ua_port
  is '联网网关服务器UA端口';
comment on column INTER_AREA.net_agent_port
  is '网管端口';
comment on column INTER_AREA.type
  is '级联服务器所属平台0：公安专网1：监控专网';
comment on column INTER_AREA.media_ip
  is '转发IP';
comment on column INTER_AREA.media_port
  is '转发Port';
comment on column INTER_AREA.user_code
  is '用户编号';
comment on column INTER_AREA.operator_id
  is '操作用户ID';
comment on column INTER_AREA.create_time
  is '修建时间';
comment on column INTER_AREA.update_time
  is '更新时间';
alter table INTER_AREA
  add constraint PK_INTER_AREA primary key (INTER_AREA_ID);
alter table INTER_AREA
  add constraint UK_INTER_AREA unique (INDEX_CODE);
alter table INTER_AREA
  add constraint FK_INTER_AREA foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table IO_INFO
prompt ======================
prompt
create table IO_INFO
(
  io_id       NUMBER not null,
  region_id   NUMBER,
  device_id   NUMBER not null,
  type        NUMBER not null,
  index_code  VARCHAR2(64) not null,
  name        VARCHAR2(64) not null,
  channel     NUMBER not null,
  map_type    NUMBER,
  operator_id NUMBER not null,
  create_time TIMESTAMP(6) not null,
  update_time TIMESTAMP(6) not null,
  int_rev_1   NUMBER,
  int_rev_2   NUMBER,
  str_rev_1   VARCHAR2(64),
  str_rev_2   VARCHAR2(128)
)
;
comment on column IO_INFO.region_id
  is '区域ID';
comment on column IO_INFO.device_id
  is '设备ID';
comment on column IO_INFO.type
  is 'IO类型，0-in/1-out';
comment on column IO_INFO.index_code
  is 'IO编号';
comment on column IO_INFO.name
  is 'IO名称';
comment on column IO_INFO.channel
  is 'IO通道号';
comment on column IO_INFO.map_type
  is '地图类型';
comment on column IO_INFO.operator_id
  is '操作员ID';
comment on column IO_INFO.create_time
  is '创建时间';
comment on column IO_INFO.int_rev_1
  is '整型保留字段';
comment on column IO_INFO.int_rev_2
  is '整型保留字段';
comment on column IO_INFO.str_rev_1
  is '字符串保留字段';
comment on column IO_INFO.str_rev_2
  is '字符串保留字段';
create index INDX_II_DEVICE_ID on IO_INFO (DEVICE_ID);
create index INDX_II_REGION_ID on IO_INFO (REGION_ID);
alter table IO_INFO
  add constraint PK_IO_INFO primary key (IO_ID);
alter table IO_INFO
  add constraint UK_IO_INFO unique (INDEX_CODE);
alter table IO_INFO
  add constraint FK_IO_INFO1 foreign key (DEVICE_ID)
  references DEVICE_INFO (DEVICE_ID);
alter table IO_INFO
  add constraint FK_IO_INFO2 foreign key (REGION_ID)
  references REGION_INFO (REGION_ID);

prompt
prompt Creating table IPSERVER_DEVICE_INFO
prompt ===================================
prompt
create table IPSERVER_DEVICE_INFO
(
  ipserver_device_id NUMBER not null,
  network_addr       VARCHAR2(128) not null,
  network_port       NUMBER,
  isonline           NUMBER(1),
  serial_no          VARCHAR2(64) not null,
  unit_id            NUMBER,
  name               VARCHAR2(64),
  type               VARCHAR2(32),
  create_time        VARCHAR2(32),
  int_rev_1          NUMBER,
  int_rev_2          NUMBER,
  str_rev_1          VARCHAR2(64),
  str_rev_2          VARCHAR2(128)
)
;
comment on column IPSERVER_DEVICE_INFO.isonline
  is '设备是否在线';
alter table IPSERVER_DEVICE_INFO
  add constraint PK_IPSERVER_DEVICE_INFO primary key (IPSERVER_DEVICE_ID);

prompt
prompt Creating table IPSERVER_DEVICE_INFO_HISTORY
prompt ===========================================
prompt
create table IPSERVER_DEVICE_INFO_HISTORY
(
  ipserver_device_history_id NUMBER not null,
  network_addr               VARCHAR2(128) not null,
  network_port               NUMBER,
  isonline                   NUMBER(1),
  serial_no                  VARCHAR2(64) not null,
  unit_id                    NUMBER,
  unit_name                  VARCHAR2(128),
  name                       VARCHAR2(64),
  type                       VARCHAR2(32),
  create_time                VARCHAR2(32),
  int_rev_1                  NUMBER,
  int_rev_2                  NUMBER,
  str_rev_1                  VARCHAR2(64),
  str_rev_2                  VARCHAR2(128)
)
;
comment on column IPSERVER_DEVICE_INFO_HISTORY.isonline
  is '设备是否在线';
alter table IPSERVER_DEVICE_INFO_HISTORY
  add constraint PK_IP_DEVICE_INFO_HISTORY primary key (IPSERVER_DEVICE_HISTORY_ID);

prompt
prompt Creating table KEYBOARD_SERVER
prompt ==============================
prompt
create table KEYBOARD_SERVER
(
  keyboard_server_id NUMBER not null,
  control_port       NUMBER,
  create_time        TIMESTAMP(6),
  hb_times           NUMBER,
  index_code         VARCHAR2(255),
  interval           NUMBER,
  ip                 VARCHAR2(255),
  max_num            NUMBER,
  name               VARCHAR2(255),
  net_port           NUMBER,
  net_zone_id        NUMBER,
  operator_id        NUMBER,
  tv_wall_server_id  NUMBER,
  update_time        TIMESTAMP(6),
  control_unit_id    NUMBER,
  hb_interval        NUMBER,
  net_agent_port     NUMBER
)
;
alter table KEYBOARD_SERVER
  add constraint PK_KEYBOARD_SERVER primary key (KEYBOARD_SERVER_ID);
alter table KEYBOARD_SERVER
  add constraint FK_KEYBOARD_SERVER foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table LB_STREAMSERVERS
prompt ===============================
prompt
create table LB_STREAMSERVERS
(
  lb_config_id     NUMBER not null,
  stream_server_id NUMBER not null
)
;
alter table LB_STREAMSERVERS
  add constraint PK_LB_STREAMSERVERS primary key (LB_CONFIG_ID, STREAM_SERVER_ID);
alter table LB_STREAMSERVERS
  add constraint FK_LB_STREAMSERVERS1 foreign key (LB_CONFIG_ID)
  references STREAMSERVER_LB_CONFIG (LB_CONFIG_ID);
alter table LB_STREAMSERVERS
  add constraint FK_LB_STREAMSERVERS2 foreign key (STREAM_SERVER_ID)
  references STREAM_SERVER (STREAM_SV_ID);

prompt
prompt Creating table MAG_SERVER
prompt =========================
prompt
create table MAG_SERVER
(
  mag_server_id   NUMBER not null,
  control_unit_id NUMBER,
  index_code      VARCHAR2(64),
  name            VARCHAR2(64),
  net_zone_id     NUMBER,
  ip_addr         VARCHAR2(16),
  ctrl_port       NUMBER,
  port            NUMBER,
  net_agent_port  NUMBER,
  operator_id     NUMBER,
  create_time     TIMESTAMP(6),
  update_time     TIMESTAMP(6),
  btrans          NUMBER,
  out_encode_type NUMBER,
  out_bit_rate    NUMBER,
  out_frame_rate  NUMBER,
  out_resolution  NUMBER,
  sub_stream      NUMBER
)
;
comment on column MAG_SERVER.control_unit_id
  is '控制单元ID表';
comment on column MAG_SERVER.index_code
  is '编号';
comment on column MAG_SERVER.name
  is '名称';
comment on column MAG_SERVER.net_zone_id
  is '所在的域';
comment on column MAG_SERVER.ip_addr
  is '服务器IP';
comment on column MAG_SERVER.ctrl_port
  is '配置端口';
comment on column MAG_SERVER.port
  is '服务器端口';
comment on column MAG_SERVER.net_agent_port
  is '网管端口';
comment on column MAG_SERVER.operator_id
  is '操作用户';
comment on column MAG_SERVER.create_time
  is '新建时间';
comment on column MAG_SERVER.update_time
  is '更新时间';
comment on column MAG_SERVER.btrans
  is '1-转码，0-不转码';
comment on column MAG_SERVER.out_encode_type
  is '编码类型 0-HIK，1-3GPP';
comment on column MAG_SERVER.out_bit_rate
  is '输出码率';
comment on column MAG_SERVER.out_frame_rate
  is '输出帧率';
comment on column MAG_SERVER.out_resolution
  is '1-QCIF,2-QVGA,3-CIF';
comment on column MAG_SERVER.sub_stream
  is '0-主码流，1-子码流';
create index INDX_MAG_SERVER_CONUNITID on MAG_SERVER (CONTROL_UNIT_ID);
alter table MAG_SERVER
  add constraint PK_MAG_SERVER primary key (MAG_SERVER_ID);
alter table MAG_SERVER
  add constraint UK_MAG_SERVER unique (INDEX_CODE);
alter table MAG_SERVER
  add constraint FK_MAG_SERVER foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table MAP_COORDINATE
prompt =============================
prompt
create table MAP_COORDINATE
(
  id       NUMBER not null,
  ogc_geom CLOB,
  name     VARCHAR2(255),
  type     NUMBER
)
;
alter table MAP_COORDINATE
  add constraint PK_MAP_COORDINATE primary key (ID);

prompt
prompt Creating table MATRIX
prompt =====================
prompt
create table MATRIX
(
  matrix_id     NUMBER not null,
  unit_id       NUMBER,
  index_code    VARCHAR2(64) not null,
  name          VARCHAR2(64) not null,
  ip_addr       VARCHAR2(16) not null,
  port          NUMBER not null,
  username      VARCHAR2(64) not null,
  password      VARCHAR2(64) not null,
  operator_id   NUMBER not null,
  serial_no     VARCHAR2(255),
  create_time   TIMESTAMP(6) not null,
  update_time   TIMESTAMP(6) not null,
  pag_server_id NUMBER,
  status        NUMBER,
  tvwall_id     NUMBER
)
;
comment on column MATRIX.unit_id
  is '控制中心ID';
comment on column MATRIX.index_code
  is '索引编号';
comment on column MATRIX.name
  is '矩阵名称';
comment on column MATRIX.ip_addr
  is 'IP地址';
comment on column MATRIX.port
  is '端口';
comment on column MATRIX.username
  is '用户名';
comment on column MATRIX.operator_id
  is '操作预员ID,如果是根用户则为0';
comment on column MATRIX.serial_no
  is '0-直连转发 1-MQ转发';
comment on column MATRIX.pag_server_id
  is '关联设备代理服务器id';
comment on column MATRIX.status
  is '是否在线，1-在线，2-不在线';
comment on column MATRIX.tvwall_id
  is '电视墙ID';
alter table MATRIX
  add constraint PK_MATRIX primary key (MATRIX_ID);

prompt
prompt Creating table MONITOR_LOG
prompt ==========================
prompt
create table MONITOR_LOG
(
  monitor_log_id  NUMBER not null,
  main_type       NUMBER not null,
  minor_type      NUMBER,
  control_unit_id NUMBER not null,
  object_type     NUMBER not null,
  object_id       NUMBER,
  log_txt         VARCHAR2(512) not null,
  log_title       VARCHAR2(64),
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column MONITOR_LOG.main_type
  is '监控日志类型';
comment on column MONITOR_LOG.minor_type
  is '违规类型编码';
comment on column MONITOR_LOG.control_unit_id
  is '监控中心ID值';
comment on column MONITOR_LOG.object_type
  is '关联的对象类型,0表示没有关联对象;1表示关联视频通道;';
comment on column MONITOR_LOG.object_id
  is '关联对象ID';
comment on column MONITOR_LOG.log_txt
  is '违规日志的文本信息';
comment on column MONITOR_LOG.log_title
  is '违规日志标题';
comment on column MONITOR_LOG.operator_id
  is '操作员ID';
comment on column MONITOR_LOG.create_time
  is '创建时间';
comment on column MONITOR_LOG.int_rev_1
  is '整型保留字段';
comment on column MONITOR_LOG.int_rev_2
  is '整型保留字段';
comment on column MONITOR_LOG.str_rev_1
  is '字符串保留字段';
comment on column MONITOR_LOG.str_rev_2
  is '字符串保留字段';
alter table MONITOR_LOG
  add constraint PK_MONITOR_LOG primary key (MONITOR_LOG_ID);

prompt
prompt Creating table MONITOR_LOG_MAIN_TYPE
prompt ====================================
prompt
create table MONITOR_LOG_MAIN_TYPE
(
  main_type   NUMBER not null,
  name        VARCHAR2(255) not null,
  create_time TIMESTAMP(6) not null,
  int_rev_1   NUMBER,
  int_rev_2   NUMBER,
  str_rev_1   VARCHAR2(64),
  str_rev_2   VARCHAR2(128)
)
;
comment on column MONITOR_LOG_MAIN_TYPE.name
  is '违规类型名称';
comment on column MONITOR_LOG_MAIN_TYPE.create_time
  is '创建时间';
comment on column MONITOR_LOG_MAIN_TYPE.int_rev_1
  is '整型保留字段';
comment on column MONITOR_LOG_MAIN_TYPE.int_rev_2
  is '整型保留字段';
comment on column MONITOR_LOG_MAIN_TYPE.str_rev_1
  is '字符串保留字段';
comment on column MONITOR_LOG_MAIN_TYPE.str_rev_2
  is '字符串保留字段';
alter table MONITOR_LOG_MAIN_TYPE
  add constraint PK_MONITOR_LOG_MAIN_TYPE primary key (MAIN_TYPE);

prompt
prompt Creating table MONITOR_LOG_MINOR_TYPE
prompt =====================================
prompt
create table MONITOR_LOG_MINOR_TYPE
(
  minor_type  NUMBER not null,
  name        VARCHAR2(64) not null,
  create_time TIMESTAMP(6) not null,
  int_rev_1   NUMBER,
  int_rev_2   NUMBER,
  str_rev_1   VARCHAR2(64),
  str_rev_2   VARCHAR2(128)
)
;
comment on column MONITOR_LOG_MINOR_TYPE.name
  is '关键字';
comment on column MONITOR_LOG_MINOR_TYPE.create_time
  is '创建时间';
comment on column MONITOR_LOG_MINOR_TYPE.int_rev_1
  is '整型保留字段';
comment on column MONITOR_LOG_MINOR_TYPE.int_rev_2
  is '整型保留字段';
comment on column MONITOR_LOG_MINOR_TYPE.str_rev_1
  is '字符串保留字段';
comment on column MONITOR_LOG_MINOR_TYPE.str_rev_2
  is '字符串保留字段';
create index IDX_MLMT_NAME on MONITOR_LOG_MINOR_TYPE (NAME);
alter table MONITOR_LOG_MINOR_TYPE
  add constraint PK_MONITOR_LOG_MINOR_TYPE primary key (MINOR_TYPE);

prompt
prompt Creating table MTA_MONITOR
prompt ==========================
prompt
create table MTA_MONITOR
(
  mta_monitor_id  NUMBER not null,
  index_code      VARCHAR2(64) not null,
  name            VARCHAR2(64) not null,
  sector_count    NUMBER not null,
  control_unit_id NUMBER not null
)
;
alter table MTA_MONITOR
  add constraint PK_MTA_MONITOR primary key (MTA_MONITOR_ID);
alter table MTA_MONITOR
  add constraint FK_MTA_MONITOR foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table MTA_SECTOR
prompt =========================
prompt
create table MTA_SECTOR
(
  mta_sector_id NUMBER not null,
  name          VARCHAR2(64) not null,
  sector_num    NUMBER not null,
  monitor_id    NUMBER not null,
  region_id     NUMBER
)
;
alter table MTA_SECTOR
  add constraint PK_MTA_SECTOR primary key (MTA_SECTOR_ID);
alter table MTA_SECTOR
  add constraint FK_MTA_SECTOR1 foreign key (REGION_ID)
  references REGION_INFO (REGION_ID);
alter table MTA_SECTOR
  add constraint FK_MTA_SECTOR2 foreign key (MONITOR_ID)
  references MTA_MONITOR (MTA_MONITOR_ID);

prompt
prompt Creating table NETWORK_MONITOR_SERVER
prompt =====================================
prompt
create table NETWORK_MONITOR_SERVER
(
  net_mon_sv_id         NUMBER not null,
  control_unit_id       NUMBER not null,
  index_code            VARCHAR2(64) not null,
  name                  VARCHAR2(64) not null,
  ip_addr               VARCHAR2(16) not null,
  second_ip             VARCHAR2(16),
  net_zone_id           NUMBER not null,
  ctrl_port             NUMBER not null,
  data_port             NUMBER,
  msg_queue             VARCHAR2(255) not null,
  use_soft_dog          NUMBER(1) not null,
  sequence_idx          NUMBER not null,
  operator_id           NUMBER not null,
  create_time           TIMESTAMP(6) not null,
  update_time           TIMESTAMP(6) not null,
  entries_time_span     NUMBER,
  upentries_time_span   NUMBER,
  entry_check_time_span NUMBER,
  int_rev_1             NUMBER,
  int_rev_2             NUMBER,
  str_rev_1             VARCHAR2(64),
  str_rev_2             VARCHAR2(128)
)
;
comment on column NETWORK_MONITOR_SERVER.control_unit_id
  is '控制单元ID';
comment on column NETWORK_MONITOR_SERVER.index_code
  is '索引编号';
comment on column NETWORK_MONITOR_SERVER.name
  is '服务器名称';
comment on column NETWORK_MONITOR_SERVER.net_zone_id
  is 'net zone id';
comment on column NETWORK_MONITOR_SERVER.ctrl_port
  is '控制端口';
comment on column NETWORK_MONITOR_SERVER.data_port
  is '数字通讯端口';
comment on column NETWORK_MONITOR_SERVER.msg_queue
  is '消息队列';
comment on column NETWORK_MONITOR_SERVER.use_soft_dog
  is '是否启用软件狗';
comment on column NETWORK_MONITOR_SERVER.operator_id
  is '操作员ID';
comment on column NETWORK_MONITOR_SERVER.create_time
  is '创建时间';
comment on column NETWORK_MONITOR_SERVER.entries_time_span
  is '获取资源时间间隔';
comment on column NETWORK_MONITOR_SERVER.upentries_time_span
  is '全部上传时间间隔';
comment on column NETWORK_MONITOR_SERVER.entry_check_time_span
  is '检查时间间隔';
comment on column NETWORK_MONITOR_SERVER.int_rev_1
  is '整型保留字段';
comment on column NETWORK_MONITOR_SERVER.int_rev_2
  is '整型保留字段';
comment on column NETWORK_MONITOR_SERVER.str_rev_1
  is '字符串保留字段';
comment on column NETWORK_MONITOR_SERVER.str_rev_2
  is '字符串保留字段';
create index INDX_NMS_CONTROL_UNIT_ID on NETWORK_MONITOR_SERVER (CONTROL_UNIT_ID);
alter table NETWORK_MONITOR_SERVER
  add constraint PK_NETWORK_MONITOR_SERVER primary key (NET_MON_SV_ID);
alter table NETWORK_MONITOR_SERVER
  add constraint UK_NETWORK_MONITOR_SERVER unique (INDEX_CODE);
alter table NETWORK_MONITOR_SERVER
  add constraint FK_NETWORK_MONITOR_SERVER foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table NET_ZONE
prompt =======================
prompt
create table NET_ZONE
(
  net_zone_id   NUMBER not null,
  net_zone_name VARCHAR2(64) not null,
  cms_url       VARCHAR2(255) not null,
  cms_port      NUMBER,
  operator_id   NUMBER not null,
  create_time   TIMESTAMP(6) not null,
  update_time   TIMESTAMP(6) not null
)
;
comment on column NET_ZONE.net_zone_name
  is '网域名称';
comment on column NET_ZONE.operator_id
  is '操作员ID';
comment on column NET_ZONE.create_time
  is '创建时间';
alter table NET_ZONE
  add constraint PK_NET_ZONE primary key (NET_ZONE_ID);

prompt
prompt Creating table NMS_CHAN_RESULT
prompt ==============================
prompt
create table NMS_CHAN_RESULT
(
  id              NUMBER not null,
  chanindex       VARCHAR2(255),
  channelid       NUMBER,
  checktime       VARCHAR2(255),
  height          NUMBER,
  ip              VARCHAR2(255),
  result          NUMBER,
  resultofblur    NUMBER,
  resultofchroma  NUMBER,
  resultoffreeze  NUMBER,
  resultofluma    NUMBER,
  resultofptz     NUMBER,
  resultofsignal  NUMBER,
  resultofsnow    NUMBER,
  resultofstreak  NUMBER,
  snapshoturl     VARCHAR2(255),
  width           NUMBER,
  nameofallresult VARCHAR2(255)
)
;
alter table NMS_CHAN_RESULT
  add constraint PK_NMS_CHAN_RESULT primary key (ID);

prompt
prompt Creating table NMS_HISTORICAL_STATISTICS
prompt ========================================
prompt
create table NMS_HISTORICAL_STATISTICS
(
  id                          NUMBER not null,
  online_crossing_info_number NUMBER,
  online_device_number        NUMBER,
  time                        VARCHAR2(255),
  total_crossing_info_number  NUMBER,
  total_device_number         NUMBER,
  unit_id                     NUMBER
)
;
alter table NMS_HISTORICAL_STATISTICS
  add constraint PK_NMS_HISTORICAL_STATISTICS primary key (ID);

prompt
prompt Creating table NVT_PROXY_SERVER
prompt ===============================
prompt
create table NVT_PROXY_SERVER
(
  nvt_proxy_id    NUMBER not null,
  vrm_id          NUMBER,
  name            VARCHAR2(64),
  index_code      VARCHAR2(64) not null,
  ip_addr         VARCHAR2(16) not null,
  net_zone_id     NUMBER not null,
  listen_port     NUMBER not null,
  ctrl_port       NUMBER not null,
  max_record_task NUMBER(5),
  use_soft_dog    NUMBER(1) not null,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column NVT_PROXY_SERVER.vrm_id
  is '服务器ID号';
comment on column NVT_PROXY_SERVER.name
  is '名称';
comment on column NVT_PROXY_SERVER.index_code
  is '索引编号';
comment on column NVT_PROXY_SERVER.net_zone_id
  is 'net zone id';
comment on column NVT_PROXY_SERVER.listen_port
  is '服务监听端口';
comment on column NVT_PROXY_SERVER.ctrl_port
  is '控制端口';
comment on column NVT_PROXY_SERVER.max_record_task
  is '最大的同时录像任务数';
comment on column NVT_PROXY_SERVER.use_soft_dog
  is '是否启用软件狗';
comment on column NVT_PROXY_SERVER.operator_id
  is '操作员ID';
comment on column NVT_PROXY_SERVER.create_time
  is '创建时间';
comment on column NVT_PROXY_SERVER.int_rev_1
  is '整型保留字段';
comment on column NVT_PROXY_SERVER.int_rev_2
  is '整型保留字段';
comment on column NVT_PROXY_SERVER.str_rev_1
  is '字符串保留字段';
comment on column NVT_PROXY_SERVER.str_rev_2
  is '字符串保留字段';
create index INDX_NPS_VRM_ID on NVT_PROXY_SERVER (VRM_ID);
alter table NVT_PROXY_SERVER
  add constraint PK_NVT_PROXY_SERVER primary key (NVT_PROXY_ID);
alter table NVT_PROXY_SERVER
  add constraint UK_NVT_PROXY_SERVER unique (INDEX_CODE);
alter table NVT_PROXY_SERVER
  add constraint FK_NVT_PROXY_SERVER foreign key (VRM_ID)
  references VRM_SERVER (VRM_ID);

prompt
prompt Creating table OPERATION_LOG
prompt ============================
prompt
create table OPERATION_LOG
(
  operation_log_id NUMBER not null,
  main_type        NUMBER not null,
  minor_type       NUMBER not null,
  control_unit_id  NUMBER not null,
  operator_id      NUMBER not null,
  create_time      TIMESTAMP(6) not null,
  resource_id      NUMBER not null,
  log_txt          VARCHAR2(256),
  int_rev_1        NUMBER,
  int_rev_2        NUMBER,
  str_rev_1        VARCHAR2(256),
  str_rev_2        VARCHAR2(128)
)
;
comment on column OPERATION_LOG.main_type
  is '日志主类型';
comment on column OPERATION_LOG.minor_type
  is '日志子类型';
comment on column OPERATION_LOG.control_unit_id
  is '控制单元ID';
comment on column OPERATION_LOG.operator_id
  is '用户ID';
comment on column OPERATION_LOG.create_time
  is '触发时间';
comment on column OPERATION_LOG.resource_id
  is '操作资源ID';
comment on column OPERATION_LOG.log_txt
  is '日志文本';
comment on column OPERATION_LOG.int_rev_1
  is '整型保留字段';
comment on column OPERATION_LOG.int_rev_2
  is '整型保留字段';
comment on column OPERATION_LOG.str_rev_1
  is '字符串保留字段';
comment on column OPERATION_LOG.str_rev_2
  is '字符串保留字段';
alter table OPERATION_LOG
  add constraint PK_OPERATION_LOG primary key (OPERATION_LOG_ID);

prompt
prompt Creating table PAG_SERVER
prompt =========================
prompt
create table PAG_SERVER
(
  pag_server_id   NUMBER not null,
  control_unit_id NUMBER,
  net_zone_id     NUMBER,
  index_code      VARCHAR2(64),
  name            VARCHAR2(64),
  ip_addr         VARCHAR2(16),
  ctrl_port       NUMBER,
  data_port       NUMBER,
  ehome_port      NUMBER,
  pic_port        NUMBER,
  net_agent_port  NUMBER,
  operator_id     NUMBER,
  create_time     TIMESTAMP(6),
  update_time     TIMESTAMP(6),
  talk_server_id  NUMBER,
  share_flag      NUMBER(5),
  ehomeip         VARCHAR2(64),
  vag_code        VARCHAR2(2)
)
;
comment on column PAG_SERVER.control_unit_id
  is '控制单元ID表';
comment on column PAG_SERVER.net_zone_id
  is 'net zone id';
comment on column PAG_SERVER.index_code
  is '编号';
comment on column PAG_SERVER.name
  is '名称';
comment on column PAG_SERVER.ip_addr
  is '服务器IP';
comment on column PAG_SERVER.ctrl_port
  is '配置端口';
comment on column PAG_SERVER.data_port
  is '服务器端口';
comment on column PAG_SERVER.ehome_port
  is '设备接入端口';
comment on column PAG_SERVER.pic_port
  is '抓图端口';
comment on column PAG_SERVER.net_agent_port
  is '网管端口';
comment on column PAG_SERVER.operator_id
  is '操作用户';
comment on column PAG_SERVER.create_time
  is '新建时间';
comment on column PAG_SERVER.update_time
  is '更新时间';
comment on column PAG_SERVER.talk_server_id
  is '关联对讲服务器';
comment on column PAG_SERVER.ehomeip
  is 'Ehome接入IP';
comment on column PAG_SERVER.vag_code
  is 'VAG编号';
alter table PAG_SERVER
  add constraint PK_PAG_SERVER primary key (PAG_SERVER_ID);
alter table PAG_SERVER
  add constraint UK_PAG_SERVER unique (INDEX_CODE);
alter table PAG_SERVER
  add constraint FK_PAG_SERVER foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table PCNVR_SERVER
prompt ===========================
prompt
create table PCNVR_SERVER
(
  pcnvr_id          NUMBER not null,
  control_unit_id   NUMBER not null,
  vrm_id            NUMBER,
  name              VARCHAR2(64),
  index_code        VARCHAR2(64) not null,
  ip_addr           VARCHAR2(16) not null,
  second_ip         VARCHAR2(16),
  net_zone_id       NUMBER not null,
  ctrl_port         NUMBER not null,
  search_port       NUMBER not null,
  rtsp_port         NUMBER,
  web_port          NUMBER,
  net_agent_port    NUMBER,
  rtp_tcp_base_port NUMBER,
  rtp_udp_base_port NUMBER,
  udp_port_count    NUMBER,
  max_record_task   NUMBER(5),
  use_soft_dog      NUMBER(1) not null,
  operator_id       NUMBER not null,
  create_time       TIMESTAMP(6) not null,
  update_time       TIMESTAMP(6) not null,
  net_server_ip     VARCHAR2(16),
  net_server_port   NUMBER,
  use_repair        NUMBER(1) default 0,
  is_backup         NUMBER(1) default 0,
  int_rev_1         NUMBER,
  int_rev_2         NUMBER,
  str_rev_1         VARCHAR2(64),
  str_rev_2         VARCHAR2(128)
)
;
comment on column PCNVR_SERVER.control_unit_id
  is '控制单元ID';
comment on column PCNVR_SERVER.vrm_id
  is '服务器ID号';
comment on column PCNVR_SERVER.name
  is '名称';
comment on column PCNVR_SERVER.index_code
  is '索引编号';
comment on column PCNVR_SERVER.net_zone_id
  is 'net zone id';
comment on column PCNVR_SERVER.ctrl_port
  is '控制端口';
comment on column PCNVR_SERVER.net_agent_port
  is '网管端口';
comment on column PCNVR_SERVER.rtp_tcp_base_port
  is 'Tcp方式下RTP的baseport';
comment on column PCNVR_SERVER.rtp_udp_base_port
  is 'UDP方式下RTP的baseport';
comment on column PCNVR_SERVER.udp_port_count
  is 'UDP方式下RTP的端口数';
comment on column PCNVR_SERVER.max_record_task
  is '最大的同时录像任务数';
comment on column PCNVR_SERVER.use_soft_dog
  is '是否启用软件狗';
comment on column PCNVR_SERVER.operator_id
  is '操作员ID';
comment on column PCNVR_SERVER.create_time
  is '创建时间';
comment on column PCNVR_SERVER.net_server_ip
  is '网管服务器Ip';
comment on column PCNVR_SERVER.net_server_port
  is '网管服务器Port';
comment on column PCNVR_SERVER.use_repair
  is '是否执行补录';
comment on column PCNVR_SERVER.is_backup
  is '是否热备NVR';
comment on column PCNVR_SERVER.int_rev_1
  is '整型保留字段';
comment on column PCNVR_SERVER.int_rev_2
  is '整型保留字段';
comment on column PCNVR_SERVER.str_rev_1
  is '字符串保留字段';
comment on column PCNVR_SERVER.str_rev_2
  is '字符串保留字段';
create index INDX_PS_VRM_ID on PCNVR_SERVER (VRM_ID);
alter table PCNVR_SERVER
  add constraint PK_PCNVR_SERVER primary key (PCNVR_ID);
alter table PCNVR_SERVER
  add constraint UK_PCNVR_SERVER unique (INDEX_CODE);
alter table PCNVR_SERVER
  add constraint FK_PCNVR_SERVER1 foreign key (VRM_ID)
  references VRM_SERVER (VRM_ID);
alter table PCNVR_SERVER
  add constraint FK_PCNVR_SERVER2 foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table PDC_ALARM
prompt ========================
prompt
create table PDC_ALARM
(
  pdc_alarm_id  NUMBER not null,
  modes         NUMBER,
  camera_id     NUMBER,
  relative_time NUMBER,
  abs_time      NUMBER,
  start_time    TIMESTAMP(6),
  end_time      TIMESTAMP(6),
  leave_num     NUMBER,
  enter_num     NUMBER,
  create_time   TIMESTAMP(6)
)
;
comment on column PDC_ALARM.modes
  is '0 单帧统计结果 1最小时间段统计结果';
comment on column PDC_ALARM.camera_id
  is '道通ID';
comment on column PDC_ALARM.relative_time
  is '相对时标';
comment on column PDC_ALARM.abs_time
  is '绝对时标';
comment on column PDC_ALARM.start_time
  is '统计起始时间';
comment on column PDC_ALARM.end_time
  is '统计结束时间';
comment on column PDC_ALARM.leave_num
  is '离开人数';
comment on column PDC_ALARM.enter_num
  is '进入人数';
comment on column PDC_ALARM.create_time
  is '统计结束时间';
alter table PDC_ALARM
  add constraint PK_PDC_ALARM primary key (PDC_ALARM_ID);

prompt
prompt Creating table POLICE_INFO
prompt ==========================
prompt
create table POLICE_INFO
(
  id            NUMBER not null,
  cardid        VARCHAR2(255),
  controlunitid NUMBER,
  gender        NUMBER,
  interregion   NUMBER(1),
  interphoneno  VARCHAR2(255),
  isnormal      NUMBER(1),
  persistence   NUMBER,
  phoneno       VARCHAR2(255),
  policename    VARCHAR2(255),
  policeno      VARCHAR2(255),
  privilegenum  NUMBER,
  starttime     TIMESTAMP(6)
)
;
alter table POLICE_INFO
  add constraint PK_POLICE_INFO primary key (ID);

prompt
prompt Creating table PREPLAN
prompt ======================
prompt
create table PREPLAN
(
  preplan_id   NUMBER(10) not null,
  preplan_name VARCHAR2(64 CHAR),
  user_id      NUMBER(10)
)
;
alter table PREPLAN
  add primary key (PREPLAN_ID);

prompt
prompt Creating table PREPLAN_ITEM
prompt ===========================
prompt
create table PREPLAN_ITEM
(
  preplan_item_id NUMBER(10) not null,
  element_id      NUMBER(10),
  preplan_id      NUMBER(10)
)
;
alter table PREPLAN_ITEM
  add primary key (PREPLAN_ITEM_ID);

prompt
prompt Creating table PRESET_INFO
prompt ==========================
prompt
create table PRESET_INFO
(
  preset_id   NUMBER not null,
  camera_id   NUMBER not null,
  type        NUMBER not null,
  index_no    NUMBER not null,
  name        VARCHAR2(32) not null,
  is_used     NUMBER(1) not null,
  operator_id NUMBER not null,
  update_time TIMESTAMP(6) not null,
  create_time TIMESTAMP(6) not null,
  int_rev_1   NUMBER,
  int_rev_2   NUMBER,
  str_rev_1   VARCHAR2(64),
  str_rev_2   VARCHAR2(128)
)
;
comment on column PRESET_INFO.camera_id
  is '摄像头ID';
comment on column PRESET_INFO.index_no
  is '预制点序号';
comment on column PRESET_INFO.name
  is '预制点名称';
comment on column PRESET_INFO.is_used
  is '是否启用';
comment on column PRESET_INFO.operator_id
  is '操作员ID';
comment on column PRESET_INFO.create_time
  is '创建时间';
comment on column PRESET_INFO.int_rev_1
  is '整型保留字段';
comment on column PRESET_INFO.int_rev_2
  is '整型保留字段';
comment on column PRESET_INFO.str_rev_1
  is '字符串保留字段';
comment on column PRESET_INFO.str_rev_2
  is '字符串保留字段';
create index INDX_PI_CAMERA_ID on PRESET_INFO (CAMERA_ID);
alter table PRESET_INFO
  add constraint PK_PRESET_INFO primary key (PRESET_ID);
alter table PRESET_INFO
  add constraint FK_PRESET_INFO foreign key (CAMERA_ID)
  references CAMERA_INFO (CAMERA_ID);

prompt
prompt Creating table PREVIEW_GROUP
prompt ============================
prompt
create table PREVIEW_GROUP
(
  preview_group_id NUMBER not null,
  create_time      TIMESTAMP(6) not null,
  description      VARCHAR2(128),
  index_code       VARCHAR2(64),
  interval_time    NUMBER,
  name             VARCHAR2(255) not null,
  operator_id      NUMBER not null,
  parent_group_id  NUMBER,
  screen_layout_id NUMBER,
  sequence_idx     NUMBER not null,
  share_mode       NUMBER not null,
  type             NUMBER not null,
  update_time      TIMESTAMP(6) not null,
  user_id          NUMBER not null
)
;
create index INDX_PG_USER_ID on PREVIEW_GROUP (USER_ID);
alter table PREVIEW_GROUP
  add constraint PK_PREVIEW_GROUP primary key (PREVIEW_GROUP_ID);
alter table PREVIEW_GROUP
  add constraint FK_PREVIEW_GROUP foreign key (USER_ID)
  references USER_INFO (USER_ID);

prompt
prompt Creating table PREVIEW_GROUP_RESOURCE
prompt =====================================
prompt
create table PREVIEW_GROUP_RESOURCE
(
  preview_group_res_id   NUMBER not null,
  create_time            TIMESTAMP(6) not null,
  cycle_preview_group_id NUMBER,
  index_code             VARCHAR2(64),
  operator_id            NUMBER not null,
  order_num              NUMBER,
  preset_no              NUMBER,
  resource_id            NUMBER not null,
  resource_type          NUMBER,
  sequence_idx           NUMBER not null,
  update_time            TIMESTAMP(6) not null
)
;
create index INDX_PGR_CYC_PREVGROUPID on PREVIEW_GROUP_RESOURCE (CYCLE_PREVIEW_GROUP_ID);
alter table PREVIEW_GROUP_RESOURCE
  add constraint PK_PREVIEW_GROUP_RESOURCE primary key (PREVIEW_GROUP_RES_ID);
alter table PREVIEW_GROUP_RESOURCE
  add constraint FK_PREVIEW_GROUP_RESOURCE foreign key (CYCLE_PREVIEW_GROUP_ID)
  references PREVIEW_GROUP (PREVIEW_GROUP_ID);

prompt
prompt Creating table PRIVILEGE_CODE
prompt =============================
prompt
create table PRIVILEGE_CODE
(
  privilege_code NUMBER not null,
  type           NUMBER not null,
  name           VARCHAR2(64) not null,
  create_time    TIMESTAMP(6) not null,
  int_rev_1      NUMBER,
  int_rev_2      NUMBER,
  str_rev_1      VARCHAR2(64),
  str_rev_2      VARCHAR2(128),
  sequence_idx   NUMBER
)
;
comment on column PRIVILEGE_CODE.type
  is '权限类别,分为三种:设备操作权限/报警操作权限/CMS菜单操作权限';
comment on column PRIVILEGE_CODE.name
  is '全县名称';
comment on column PRIVILEGE_CODE.create_time
  is '创建时间';
comment on column PRIVILEGE_CODE.int_rev_1
  is '整型保留字段';
comment on column PRIVILEGE_CODE.int_rev_2
  is '整型保留字段';
comment on column PRIVILEGE_CODE.str_rev_1
  is '字符串保留字段';
comment on column PRIVILEGE_CODE.str_rev_2
  is '字符串保留字段';
comment on column PRIVILEGE_CODE.sequence_idx
  is '排队序号';
alter table PRIVILEGE_CODE
  add constraint PK_PRIVILEGE_CODE primary key (PRIVILEGE_CODE);

prompt
prompt Creating table ROLE_INFO
prompt ========================
prompt
create table ROLE_INFO
(
  role_id         NUMBER not null,
  index_code      VARCHAR2(64),
  control_unit_id NUMBER not null,
  name            VARCHAR2(64) not null,
  description     VARCHAR2(128),
  sequence_idx    NUMBER not null,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  expire_time     TIMESTAMP(6),
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       CLOB,
  str_rev_2       VARCHAR2(128)
)
;
comment on column ROLE_INFO.index_code
  is '索引编号';
comment on column ROLE_INFO.control_unit_id
  is '用户所属的区域ID';
comment on column ROLE_INFO.name
  is '用户组名称';
comment on column ROLE_INFO.sequence_idx
  is '排序索引';
comment on column ROLE_INFO.operator_id
  is '操作预员ID,如果是根用户则为0';
comment on column ROLE_INFO.int_rev_1
  is '整型保留字段';
comment on column ROLE_INFO.int_rev_2
  is '整型保留字段';
comment on column ROLE_INFO.str_rev_1
  is '字符串保留字段';
comment on column ROLE_INFO.str_rev_2
  is '字符串保留字段';
alter table ROLE_INFO
  add constraint PK_ROLE_INFO primary key (ROLE_ID);

prompt
prompt Creating table PRIVILEGE_INFO
prompt =============================
prompt
create table PRIVILEGE_INFO
(
  privilege_info_id NUMBER not null,
  role_id           NUMBER,
  object_type       NUMBER not null,
  object_ids        CLOB,
  operator_id       NUMBER not null,
  update_time       TIMESTAMP(6) not null,
  create_time       TIMESTAMP(6) not null,
  int_rev_1         NUMBER,
  int_rev_2         NUMBER,
  str_rev_1         VARCHAR2(4000),
  str_rev_2         CLOB,
  privilege_code    NUMBER
)
;
comment on column PRIVILEGE_INFO.role_id
  is '该用户所属的用户组ID';
comment on column PRIVILEGE_INFO.object_type
  is '权限类型，0-监控点、1-I/O通道、2-对讲设备通道、3-用户、4-菜单权限';
comment on column PRIVILEGE_INFO.object_ids
  is '权限点集合，以","分开，格式为： ",1,2,3," 注意前后都有逗号';
comment on column PRIVILEGE_INFO.operator_id
  is '操作预员ID,如果是根用户则为0';
comment on column PRIVILEGE_INFO.create_time
  is '创建时间';
comment on column PRIVILEGE_INFO.int_rev_1
  is '整型保留字段';
comment on column PRIVILEGE_INFO.int_rev_2
  is '整型保留字段';
comment on column PRIVILEGE_INFO.str_rev_1
  is '字符串保留字段1, 用于权限点所在中心，以",1,2,3,5,"格式使用';
comment on column PRIVILEGE_INFO.str_rev_2
  is '字符串保留字段2, 用于权限点所在区域，以",1,2,3,5,"格式使用';
comment on column PRIVILEGE_INFO.privilege_code
  is '权限类型';
create index IDX_PI_OBJECT_TYPE on PRIVILEGE_INFO (OBJECT_TYPE);
create index INDX_PI_ROLE_ID on PRIVILEGE_INFO (ROLE_ID);
alter table PRIVILEGE_INFO
  add constraint PK_PRIVILEGE_INFO primary key (PRIVILEGE_INFO_ID);
alter table PRIVILEGE_INFO
  add constraint FK_PRIVILEGE_INFO1 foreign key (ROLE_ID)
  references ROLE_INFO (ROLE_ID);
alter table PRIVILEGE_INFO
  add constraint FK_PRIVILEGE_INFO2 foreign key (PRIVILEGE_CODE)
  references PRIVILEGE_CODE (PRIVILEGE_CODE);

prompt
prompt Creating table PTZ_LOCK
prompt =======================
prompt
create table PTZ_LOCK
(
  ptz_lock_id    NUMBER not null,
  camera_id      NUMBER,
  user_status_id NUMBER,
  update_time    TIMESTAMP(6),
  int_rev_1      NUMBER,
  str_rev_1      VARCHAR2(64)
)
;
comment on column PTZ_LOCK.int_rev_1
  is '整型保留字段';
comment on column PTZ_LOCK.str_rev_1
  is '字符串保留字段';
alter table PTZ_LOCK
  add constraint PK_PTZ_LOCK primary key (PTZ_LOCK_ID);

prompt
prompt Creating table PTZ_SERVER
prompt =========================
prompt
create table PTZ_SERVER
(
  ptz_server_id   NUMBER not null,
  control_unit_id NUMBER,
  index_code      VARCHAR2(64),
  name            VARCHAR2(64),
  net_zone_id     NUMBER,
  ip_addr         VARCHAR2(16),
  ctrl_port       NUMBER,
  port            NUMBER,
  net_agent_port  NUMBER,
  operator_id     NUMBER,
  create_time     TIMESTAMP(6),
  update_time     TIMESTAMP(6)
)
;
comment on column PTZ_SERVER.control_unit_id
  is '控制单元ID表';
comment on column PTZ_SERVER.index_code
  is '编号';
comment on column PTZ_SERVER.name
  is '名称';
comment on column PTZ_SERVER.net_zone_id
  is '所在的域';
comment on column PTZ_SERVER.ip_addr
  is '服务器IP';
comment on column PTZ_SERVER.ctrl_port
  is '配置端口';
comment on column PTZ_SERVER.port
  is '服务器端口';
comment on column PTZ_SERVER.net_agent_port
  is '网管端口';
comment on column PTZ_SERVER.operator_id
  is '操作用户';
comment on column PTZ_SERVER.create_time
  is '新建时间';
comment on column PTZ_SERVER.update_time
  is '更新时间';
alter table PTZ_SERVER
  add constraint PK_PTZ_SERVER primary key (PTZ_SERVER_ID);
alter table PTZ_SERVER
  add constraint UK_PTZ_SERVER unique (INDEX_CODE);
alter table PTZ_SERVER
  add constraint FK_PTZ_SERVER foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table RECORD_INSPECT_DETAIL
prompt ====================================
prompt
create table RECORD_INSPECT_DETAIL
(
  detail_id      NUMBER(10) not null,
  camera_id      NUMBER(10) not null,
  record_style   NUMBER(10) not null,
  detail_date    TIMESTAMP(6) not null,
  inspect_detail VARCHAR2(384 CHAR) not null,
  inspect_time   TIMESTAMP(6) not null
)
;
alter table RECORD_INSPECT_DETAIL
  add primary key (DETAIL_ID);

prompt
prompt Creating table RECORD_INSPECT_EXCEPTION
prompt =======================================
prompt
create table RECORD_INSPECT_EXCEPTION
(
  exception_id NUMBER(10) not null,
  camera_id    NUMBER(10) not null,
  record_style NUMBER(10) not null,
  start_time   TIMESTAMP(6) not null,
  end_time     TIMESTAMP(6) not null
)
;
alter table RECORD_INSPECT_EXCEPTION
  add primary key (EXCEPTION_ID);

prompt
prompt Creating table RECORD_INSPECT_TEMPLATE
prompt ======================================
prompt
create table RECORD_INSPECT_TEMPLATE
(
  template_id      NUMBER(10) not null,
  camera_id        NUMBER(10) not null,
  inspect_template VARCHAR2(48 CHAR) not null,
  record_style     NUMBER(10) not null,
  template_date    TIMESTAMP(6) not null
)
;
alter table RECORD_INSPECT_TEMPLATE
  add primary key (TEMPLATE_ID);

prompt
prompt Creating table SCHEDULE_TEMPLATE
prompt ================================
prompt
create table SCHEDULE_TEMPLATE
(
  sched_templ_id  NUMBER not null,
  type            NUMBER not null,
  name            VARCHAR2(64) not null,
  monday_sched    VARCHAR2(48) not null,
  tuesday_sched   VARCHAR2(48) not null,
  wednesday_sched VARCHAR2(48) not null,
  thursday_sched  VARCHAR2(48) not null,
  friday_sched    VARCHAR2(48) not null,
  saturday_sched  VARCHAR2(48) not null,
  sunday_sched    VARCHAR2(48) not null,
  description     VARCHAR2(64),
  sequence_idx    NUMBER not null,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  camera_id       NUMBER,
  int_rev_1       NUMBER,
  int_rev_2       NUMBER,
  str_rev_1       VARCHAR2(64),
  str_rev_2       VARCHAR2(128)
)
;
comment on column SCHEDULE_TEMPLATE.type
  is '模板类型';
comment on column SCHEDULE_TEMPLATE.name
  is '录像计划模版名称';
comment on column SCHEDULE_TEMPLATE.monday_sched
  is '星期一的录像计划ID';
comment on column SCHEDULE_TEMPLATE.tuesday_sched
  is '星期二的计划ID';
comment on column SCHEDULE_TEMPLATE.wednesday_sched
  is '星期三录像计划ID';
comment on column SCHEDULE_TEMPLATE.thursday_sched
  is '星期四计划ID';
comment on column SCHEDULE_TEMPLATE.friday_sched
  is '星期五的计划ID';
comment on column SCHEDULE_TEMPLATE.saturday_sched
  is '星期六计划ID';
comment on column SCHEDULE_TEMPLATE.sunday_sched
  is '星期天计划ID';
comment on column SCHEDULE_TEMPLATE.description
  is '描述信息';
comment on column SCHEDULE_TEMPLATE.sequence_idx
  is '排序索引';
comment on column SCHEDULE_TEMPLATE.operator_id
  is '操作员ID';
comment on column SCHEDULE_TEMPLATE.create_time
  is '创建时间';
comment on column SCHEDULE_TEMPLATE.camera_id
  is 'cameraId(通过远程获取)';
comment on column SCHEDULE_TEMPLATE.int_rev_1
  is '整型保留字段';
comment on column SCHEDULE_TEMPLATE.int_rev_2
  is '整型保留字段';
comment on column SCHEDULE_TEMPLATE.str_rev_1
  is '字符串保留字段';
comment on column SCHEDULE_TEMPLATE.str_rev_2
  is '字符串保留字段';
alter table SCHEDULE_TEMPLATE
  add constraint PK_SCHEDULE_TEMPLATE primary key (SCHED_TEMPL_ID);

prompt
prompt Creating table SERVER_AID_MAP
prompt =============================
prompt
create table SERVER_AID_MAP
(
  device_id NUMBER not null,
  server_id NUMBER
)
;
alter table SERVER_AID_MAP
  add constraint PK_SERVER_AID_MAP primary key (DEVICE_ID);

prompt
prompt Creating table SERVER_MULTI_IP
prompt ==============================
prompt
create table SERVER_MULTI_IP
(
  svr_multi_ip_id NUMBER not null,
  net_zone_id     NUMBER not null,
  svr_type        NUMBER not null,
  svr_id          NUMBER not null,
  svr_ip          VARCHAR2(32) not null,
  port_type       NUMBER not null,
  svr_port        NUMBER not null,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null
)
;
comment on column SERVER_MULTI_IP.net_zone_id
  is '网域配置ID';
comment on column SERVER_MULTI_IP.svr_type
  is '服务器类型';
comment on column SERVER_MULTI_IP.svr_id
  is '服务器ID';
comment on column SERVER_MULTI_IP.svr_ip
  is '服务器IP地址';
comment on column SERVER_MULTI_IP.port_type
  is '端口类型，1-数据端口，2-查询端口';
comment on column SERVER_MULTI_IP.svr_port
  is '服务器端口';
comment on column SERVER_MULTI_IP.operator_id
  is '操作员ID';
comment on column SERVER_MULTI_IP.create_time
  is '创建时间';
alter table SERVER_MULTI_IP
  add constraint PK_SERVER_MULTI_IP primary key (SVR_MULTI_IP_ID);
alter table SERVER_MULTI_IP
  add constraint FK_SERVER_MULTI_IP foreign key (NET_ZONE_ID)
  references NET_ZONE (NET_ZONE_ID);

prompt
prompt Creating table SERVER_STATUS
prompt ============================
prompt
create table SERVER_STATUS
(
  server_id      NUMBER not null,
  cpu_usage      NUMBER,
  heartbeat_time TIMESTAMP(6),
  memory_info    VARCHAR2(32),
  status         NUMBER
)
;
alter table SERVER_STATUS
  add constraint PK_SERVER_STATUS primary key (SERVER_ID);

prompt
prompt Creating table SERVER_STATUS_HIS
prompt ================================
prompt
create table SERVER_STATUS_HIS
(
  statuslsh      NUMBER not null,
  cpu_usage      NUMBER,
  heartbeat_time TIMESTAMP(6),
  memory_info    VARCHAR2(32),
  server_id      NUMBER,
  status         NUMBER
)
;
alter table SERVER_STATUS_HIS
  add constraint PK_SERVER_STATUS_HIS primary key (STATUSLSH);

prompt
prompt Creating table SINGLE_MOBILE_GROUP
prompt ==================================
prompt
create table SINGLE_MOBILE_GROUP
(
  id          NUMBER not null,
  group_level NUMBER not null,
  name        VARCHAR2(255) not null,
  parent_id   NUMBER not null
)
;
alter table SINGLE_MOBILE_GROUP
  add constraint PK_SINGLE_MOBILE_GROUP primary key (ID);

prompt
prompt Creating table SPEC_DAY
prompt =======================
prompt
create table SPEC_DAY
(
  spec_day_id    NUMBER not null,
  sched_templ_id NUMBER,
  day            TIMESTAMP(6) not null,
  style          NUMBER not null,
  copy_day       NUMBER,
  day_sched      VARCHAR2(48),
  operator_id    NUMBER not null,
  create_time    TIMESTAMP(6) not null,
  update_time    TIMESTAMP(6) not null,
  int_rev_1      NUMBER,
  int_rev_2      NUMBER,
  str_rev_1      VARCHAR2(64),
  str_rev_2      VARCHAR2(128)
)
;
comment on column SPEC_DAY.day
  is '特殊日期';
comment on column SPEC_DAY.style
  is '处理类型,0表示不录像,1表示跟另一天的计划相同,2表示有自己的计划';
comment on column SPEC_DAY.copy_day
  is '使用特定天的录像计划，1-7分别表示星期一到星期天';
comment on column SPEC_DAY.operator_id
  is '操作员ID';
comment on column SPEC_DAY.create_time
  is '创建时间';
comment on column SPEC_DAY.int_rev_1
  is '整型保留字段';
comment on column SPEC_DAY.int_rev_2
  is '整型保留字段';
comment on column SPEC_DAY.str_rev_1
  is '字符串保留字段';
comment on column SPEC_DAY.str_rev_2
  is '字符串保留字段';
create index INDX_SD_SCHED_TEMPL_ID on SPEC_DAY (SCHED_TEMPL_ID);
alter table SPEC_DAY
  add constraint PK_SPEC_DAY primary key (SPEC_DAY_ID);
alter table SPEC_DAY
  add constraint FK_SPEC_DAY foreign key (SCHED_TEMPL_ID)
  references SCHEDULE_TEMPLATE (SCHED_TEMPL_ID);

prompt
prompt Creating table SYS_INFO
prompt =======================
prompt
create table SYS_INFO
(
  sys_info_id    NUMBER not null,
  parameter_name VARCHAR2(64) not null,
  value_type     NUMBER not null,
  int_value      NUMBER,
  str_value      VARCHAR2(256),
  create_time    TIMESTAMP(6) not null
)
;
comment on column SYS_INFO.parameter_name
  is '参数名称';
comment on column SYS_INFO.value_type
  is '值类型，0代表数字，1代表字符串';
comment on column SYS_INFO.int_value
  is '数值参数';
comment on column SYS_INFO.str_value
  is '字符串参数';
comment on column SYS_INFO.create_time
  is '创建时间';
alter table SYS_INFO
  add constraint PK_SYS_INFO primary key (SYS_INFO_ID);

prompt
prompt Creating table SYS_LOG
prompt ======================
prompt
create table SYS_LOG
(
  sys_log_id   NUMBER not null,
  main_type    NUMBER not null,
  minor_type   NUMBER not null,
  ctrl_unit_id NUMBER,
  trigger_time TIMESTAMP(6) not null,
  resource_id  NUMBER not null,
  log_txt      VARCHAR2(256) not null,
  int_rev_1    NUMBER,
  int_rev_2    NUMBER,
  str_rev_1    VARCHAR2(64),
  str_rev_2    VARCHAR2(128)
)
;
comment on column SYS_LOG.main_type
  is '日志主类型';
comment on column SYS_LOG.minor_type
  is '日志子类型';
comment on column SYS_LOG.ctrl_unit_id
  is '控制单元ID';
comment on column SYS_LOG.trigger_time
  is '触发时间';
comment on column SYS_LOG.resource_id
  is '操作资源ID';
comment on column SYS_LOG.log_txt
  is '日志文本';
comment on column SYS_LOG.int_rev_1
  is '整型保留字段';
comment on column SYS_LOG.int_rev_2
  is '整型保留字段';
comment on column SYS_LOG.str_rev_1
  is '字符串保留字段';
comment on column SYS_LOG.str_rev_2
  is '字符串保留字段';
alter table SYS_LOG
  add constraint PK_SYS_LOG primary key (SYS_LOG_ID);

prompt
prompt Creating table TABLE_MAP_LAYER
prompt ==============================
prompt
create table TABLE_MAP_LAYER
(
  id     NUMBER not null,
  mapsrc VARCHAR2(255)
)
;
alter table TABLE_MAP_LAYER
  add constraint PK_TABLE_MAP_LAYER primary key (ID);

prompt
prompt Creating table TALK_SERVER
prompt ==========================
prompt
create table TALK_SERVER
(
  talk_server_id  NUMBER not null,
  control_unit_id NUMBER,
  index_code      VARCHAR2(64),
  name            VARCHAR2(64),
  net_zone_id     NUMBER,
  ip_addr         VARCHAR2(16),
  ctrl_port       NUMBER,
  port            NUMBER,
  net_agent_port  NUMBER,
  over_time       NUMBER,
  operator_id     NUMBER,
  create_time     TIMESTAMP(6),
  update_time     TIMESTAMP(6)
)
;
comment on column TALK_SERVER.control_unit_id
  is '控制单元ID表';
comment on column TALK_SERVER.index_code
  is '编号';
comment on column TALK_SERVER.name
  is '名称';
comment on column TALK_SERVER.net_zone_id
  is '所在的域';
comment on column TALK_SERVER.ip_addr
  is '服务器IP';
comment on column TALK_SERVER.ctrl_port
  is '配置端口';
comment on column TALK_SERVER.port
  is '服务器端口';
comment on column TALK_SERVER.net_agent_port
  is '网管端口';
comment on column TALK_SERVER.over_time
  is '超时时间';
comment on column TALK_SERVER.operator_id
  is '操作用户';
comment on column TALK_SERVER.create_time
  is '新建时间';
comment on column TALK_SERVER.update_time
  is '更新时间';
alter table TALK_SERVER
  add constraint PK_TALK_SERVER primary key (TALK_SERVER_ID);
alter table TALK_SERVER
  add constraint UK_TALK_SERVER unique (INDEX_CODE);
alter table TALK_SERVER
  add constraint FK_TALK_SERVER foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table TASK_CONFIG
prompt ==========================
prompt
create table TASK_CONFIG
(
  taskconfigid NUMBER not null,
  objectid     NUMBER,
  taskobject   NUMBER
)
;
alter table TASK_CONFIG
  add constraint PK_TASK_CONFIG primary key (TASKCONFIGID);

prompt
prompt Creating table TASK_OBJECT
prompt ==========================
prompt
create table TASK_OBJECT
(
  task_type       VARCHAR2(31) not null,
  id              NUMBER not null,
  express         VARCHAR2(512),
  name            VARCHAR2(64),
  remark          VARCHAR2(1024),
  scheduletype    NUMBER,
  preset_num      NUMBER,
  preservedays    NUMBER,
  control_unit_id NUMBER not null
)
;
alter table TASK_OBJECT
  add constraint PK_TASK_OBJECT primary key (ID);
alter table TASK_OBJECT
  add constraint FK_TASK_OBJECT foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table THIRD_PLATFORM
prompt =============================
prompt
create table THIRD_PLATFORM
(
  id       NUMBER not null,
  img_url  VARCHAR2(255) not null,
  link_url VARCHAR2(255) not null,
  name     VARCHAR2(255) not null
)
;
alter table THIRD_PLATFORM
  add constraint PK_THIRD_PLATFORM primary key (ID);

prompt
prompt Creating table TVWALL_INFO
prompt ==========================
prompt
create table TVWALL_INFO
(
  id                      NUMBER not null,
  index_code              VARCHAR2(64),
  name                    VARCHAR2(64),
  monitor_screen_group_id NUMBER
)
;
alter table TVWALL_INFO
  add constraint PK_TVWALL_INFO primary key (ID);
alter table TVWALL_INFO
  add foreign key (MONITOR_SCREEN_GROUP_ID)
  references MONITOR_SCREEN_GROUP (GROUP_ID);

prompt
prompt Creating table USER_ROLE_MAP
prompt ============================
prompt
create table USER_ROLE_MAP
(
  role_id NUMBER not null,
  user_id NUMBER not null
)
;
comment on column USER_ROLE_MAP.role_id
  is '该用户所属的用户组ID';
comment on column USER_ROLE_MAP.user_id
  is '用户id值';
create index INDX_URM_ROLE_ID on USER_ROLE_MAP (ROLE_ID);
create index INDX_URM_USER_ID on USER_ROLE_MAP (USER_ID);
alter table USER_ROLE_MAP
  add constraint PK_USER_ROLE_MAP primary key (USER_ID, ROLE_ID);
alter table USER_ROLE_MAP
  add constraint FK_USER_ROLE_MAP1 foreign key (ROLE_ID)
  references ROLE_INFO (ROLE_ID);
alter table USER_ROLE_MAP
  add constraint FK_USER_ROLE_MAP2 foreign key (USER_ID)
  references USER_INFO (USER_ID);

prompt
prompt Creating table VIDEO_TAG
prompt ========================
prompt
create table VIDEO_TAG
(
  video_tag_id NUMBER not null,
  camera_id    NUMBER,
  end_time     TIMESTAMP(6),
  operator     VARCHAR2(255),
  remark       VARCHAR2(255),
  start_time   TIMESTAMP(6),
  store_place  VARCHAR2(255)
)
;
alter table VIDEO_TAG
  add constraint PK_VIDEO_TAG primary key (VIDEO_TAG_ID);

prompt
prompt Creating table VIOLATION_TYPE
prompt =============================
prompt
create table VIOLATION_TYPE
(
  violation_code NUMBER not null,
  name           NUMBER,
  create_time    TIMESTAMP(6),
  int_rev_1      NUMBER,
  int_rev_2      NUMBER,
  str_rev_1      VARCHAR2(64),
  str_rev_2      VARCHAR2(128)
)
;
comment on column VIOLATION_TYPE.name
  is '违规类型名称';
comment on column VIOLATION_TYPE.create_time
  is '创建时间';
comment on column VIOLATION_TYPE.int_rev_1
  is '整型保留字段';
comment on column VIOLATION_TYPE.int_rev_2
  is '整型保留字段';
comment on column VIOLATION_TYPE.str_rev_1
  is '字符串保留字段';
comment on column VIOLATION_TYPE.str_rev_2
  is '字符串保留字段';
alter table VIOLATION_TYPE
  add constraint PK_VIOLATION_TYPE primary key (VIOLATION_CODE);

prompt
prompt Creating table VIOLATION_LOG
prompt ============================
prompt
create table VIOLATION_LOG
(
  violation_log_id NUMBER not null,
  violation_code   NUMBER,
  log_txt          VARCHAR2(512),
  operator_id      NUMBER,
  create_time      TIMESTAMP(6),
  int_rev_1        NUMBER,
  int_rev_2        NUMBER,
  str_rev_1        VARCHAR2(64),
  str_rev_2        VARCHAR2(128)
)
;
comment on column VIOLATION_LOG.violation_code
  is '违规类型编码';
comment on column VIOLATION_LOG.log_txt
  is '违规日志的文本信息';
comment on column VIOLATION_LOG.operator_id
  is '操作员ID';
comment on column VIOLATION_LOG.create_time
  is '创建时间';
comment on column VIOLATION_LOG.int_rev_1
  is '整型保留字段';
comment on column VIOLATION_LOG.int_rev_2
  is '整型保留字段';
comment on column VIOLATION_LOG.str_rev_1
  is '字符串保留字段';
comment on column VIOLATION_LOG.str_rev_2
  is '字符串保留字段';
alter table VIOLATION_LOG
  add constraint PK_VIOLATION_LOG primary key (VIOLATION_LOG_ID);
alter table VIOLATION_LOG
  add constraint FK_VIOLATION_LOG foreign key (VIOLATION_CODE)
  references VIOLATION_TYPE (VIOLATION_CODE);

prompt
prompt Creating table VRB_SERVER
prompt =========================
prompt
create table VRB_SERVER
(
  vrb_server_id    NUMBER not null,
  create_time      TIMESTAMP(6) not null,
  ctrl_port        NUMBER not null,
  ftp_port         NUMBER not null,
  http_min_size    NUMBER not null,
  http_port        NUMBER not null,
  index_code       VARCHAR2(64) not null,
  ip_addr          VARCHAR2(64) not null,
  name             VARCHAR2(256),
  net_zone_id      NUMBER not null,
  operator_id      NUMBER not null,
  password         VARCHAR2(255),
  rtsp_port        NUMBER not null,
  rtsp_tcp_port    NUMBER not null,
  rtsp_udp_count   NUMBER not null,
  rtsp_udp_port    NUMBER not null,
  share_flag       NUMBER(5) not null,
  snmp_port        NUMBER not null,
  store_path       VARCHAR2(255),
  str_rev_1        VARCHAR2(255),
  update_time      TIMESTAMP(6) not null,
  user_name        VARCHAR2(255),
  web_service_port NUMBER not null,
  control_unit_id  NUMBER not null,
  store_time       VARCHAR2(255)
)
;
alter table VRB_SERVER
  add constraint PK_VRB_SERVER primary key (VRB_SERVER_ID);
alter table VRB_SERVER
  add constraint FK_VRB_SERVER foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table VOICE_SERVER
prompt ===========================
prompt
create table VOICE_SERVER
(
  voice_server_id NUMBER not null,
  vrb_id          NUMBER,
  control_unit_id NUMBER,
  index_code      VARCHAR2(64),
  name            VARCHAR2(64),
  net_zone_id     NUMBER,
  ip_addr         VARCHAR2(16),
  ctrl_port       NUMBER,
  port            NUMBER,
  net_agent_port  NUMBER,
  operator_id     NUMBER,
  create_time     TIMESTAMP(6),
  update_time     TIMESTAMP(6),
  start_phonenum  VARCHAR2(64),
  local_storepath VARCHAR2(128)
)
;
comment on column VOICE_SERVER.vrb_id
  is '服务器ID号';
comment on column VOICE_SERVER.control_unit_id
  is '控制单元ID表';
comment on column VOICE_SERVER.index_code
  is '编号';
comment on column VOICE_SERVER.name
  is '名称';
comment on column VOICE_SERVER.net_zone_id
  is '所在的域';
comment on column VOICE_SERVER.ip_addr
  is '服务器IP';
comment on column VOICE_SERVER.ctrl_port
  is '配置端口';
comment on column VOICE_SERVER.port
  is '服务器端口';
comment on column VOICE_SERVER.net_agent_port
  is '网管端口';
comment on column VOICE_SERVER.operator_id
  is '操作用户';
comment on column VOICE_SERVER.create_time
  is '新建时间';
comment on column VOICE_SERVER.update_time
  is '更新时间';
comment on column VOICE_SERVER.start_phonenum
  is '起始号码';
comment on column VOICE_SERVER.local_storepath
  is '本地存储路径';
alter table VOICE_SERVER
  add constraint PK_VOICE_SERVER primary key (VOICE_SERVER_ID);
alter table VOICE_SERVER
  add constraint UK_VOICE_SERVER unique (INDEX_CODE);
alter table VOICE_SERVER
  add constraint FK_VOICE_SERVER1 foreign key (VRB_ID)
  references VRB_SERVER (VRB_SERVER_ID);
alter table VOICE_SERVER
  add constraint FK_VOICE_SERVER2 foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table VQD_SERVER
prompt =========================
prompt
create table VQD_SERVER
(
  id              NUMBER not null,
  control_unit_id NUMBER not null,
  index_code      VARCHAR2(64) not null,
  name            VARCHAR2(64) not null,
  ip_addr         VARCHAR2(16) not null,
  net_zone_id     NUMBER not null,
  ctrl_port       NUMBER not null,
  operator_id     NUMBER not null,
  create_time     TIMESTAMP(6) not null,
  update_time     TIMESTAMP(6) not null,
  user_name       VARCHAR2(32),
  password        VARCHAR2(32)
)
;
comment on column VQD_SERVER.control_unit_id
  is '控制单元ID';
comment on column VQD_SERVER.index_code
  is '索引编号';
comment on column VQD_SERVER.name
  is '服务器名称';
comment on column VQD_SERVER.net_zone_id
  is 'net zone id';
comment on column VQD_SERVER.ctrl_port
  is '控制端口';
comment on column VQD_SERVER.operator_id
  is '操作员ID';
comment on column VQD_SERVER.create_time
  is '创建时间';
alter table VQD_SERVER
  add constraint PK_VQD_SERVER primary key (ID);
alter table VQD_SERVER
  add unique (INDEX_CODE);
alter table VQD_SERVER
  add constraint FK_VQD_SERVER foreign key (CONTROL_UNIT_ID)
  references CONTROL_UNIT (CONTROL_UNIT_ID);

prompt
prompt Creating table VRB_RECORD_SCHEDULE
prompt ==================================
prompt
create table VRB_RECORD_SCHEDULE
(
  vrb_record_sched_id    VARCHAR2(255) not null,
  create_time            TIMESTAMP(6) not null,
  frame_type             NUMBER,
  need_backup            NUMBER not null,
  operator_id            NUMBER not null,
  record_begin_time      NUMBER,
  record_day             NUMBER,
  record_end_time        NUMBER,
  record_friday_sched    VARCHAR2(255),
  record_monday_sched    VARCHAR2(255),
  record_saturday_sched  VARCHAR2(255),
  record_sunday_sched    VARCHAR2(255),
  record_thursday_sched  VARCHAR2(255),
  record_tuesday_sched   VARCHAR2(255),
  record_type            NUMBER,
  record_wednesday_sched VARCHAR2(255),
  run_begin_time         NUMBER not null,
  run_end_time           NUMBER not null,
  run_type               NUMBER not null,
  store_period           NUMBER,
  update_time            TIMESTAMP(6),
  week_day               NUMBER,
  camera_id              NUMBER not null,
  vrb_server_id          NUMBER not null
)
;
alter table VRB_RECORD_SCHEDULE
  add constraint PK_VRB_RECORD_SCHEDULE primary key (VRB_RECORD_SCHED_ID);
alter table VRB_RECORD_SCHEDULE
  add constraint FK_VRB_RECORD_SCHEDULE1 foreign key (VRB_SERVER_ID)
  references VRB_SERVER (VRB_SERVER_ID);
alter table VRB_RECORD_SCHEDULE
  add constraint FK_VRB_RECORD_SCHEDULE2 foreign key (CAMERA_ID)
  references CAMERA_INFO (CAMERA_ID);

prompt
prompt Creating table VRM_RECORD_SCHEDULE
prompt ==================================
prompt
create table VRM_RECORD_SCHEDULE
(
  vrm_rec_sched_id  NUMBER not null,
  pcnvr_id          NUMBER,
  nvt_proxy_id      NUMBER,
  sched_templ_id    NUMBER,
  vrm_id            NUMBER not null,
  is_valid          NUMBER(1) not null,
  camera_id         NUMBER not null,
  stream_type       NUMBER not null,
  record_style      NUMBER not null,
  storage_index     NUMBER,
  need_record       NUMBER(1) not null,
  store_time        NUMBER,
  store_space       NUMBER,
  delay_record_time NUMBER not null,
  pre_record_time   NUMBER not null,
  operator_id       NUMBER not null,
  create_time       TIMESTAMP(6) not null,
  update_time       TIMESTAMP(6) not null,
  int_rev_1         NUMBER,
  int_rev_2         NUMBER,
  str_rev_1         VARCHAR2(64),
  str_rev_2         VARCHAR2(128),
  last_down_time    TIMESTAMP(6)
)
;
comment on column VRM_RECORD_SCHEDULE.nvt_proxy_id
  is 'NVT代理服务器ID';
comment on column VRM_RECORD_SCHEDULE.record_style
  is '0表示IPSAN;1表示前端本地录像；2表示PC-NVR录像；3表示NVR_BN录像；';
comment on column VRM_RECORD_SCHEDULE.storage_index
  is '存储位置索引，用来在PC-NVR录像时，设定保存到哪一个磁盘分组中；用在IP-SAN存储中，则表示存储在那个Target中；';
comment on column VRM_RECORD_SCHEDULE.need_record
  is '是否录像，用来暂时停止录像计划的执行';
comment on column VRM_RECORD_SCHEDULE.store_time
  is '存储时间,以天为单位';
comment on column VRM_RECORD_SCHEDULE.store_space
  is '存储空间，单位为MB';
comment on column VRM_RECORD_SCHEDULE.operator_id
  is '操作员ID';
comment on column VRM_RECORD_SCHEDULE.create_time
  is '创建时间';
comment on column VRM_RECORD_SCHEDULE.int_rev_1
  is '整型保留字段';
comment on column VRM_RECORD_SCHEDULE.int_rev_2
  is '整型保留字段';
comment on column VRM_RECORD_SCHEDULE.str_rev_1
  is '字符串保留字段';
comment on column VRM_RECORD_SCHEDULE.str_rev_2
  is '字符串保留字段';
create index INDX_VRS_CAMERA_ID on VRM_RECORD_SCHEDULE (CAMERA_ID);
create index INDX_VRS_NVT_PROXY_ID on VRM_RECORD_SCHEDULE (NVT_PROXY_ID);
create index INDX_VRS_PCNVR_ID on VRM_RECORD_SCHEDULE (PCNVR_ID);
create index INDX_VRS_SCHED_TEMPL_ID on VRM_RECORD_SCHEDULE (SCHED_TEMPL_ID);
create index INDX_VRS_VRM_ID on VRM_RECORD_SCHEDULE (VRM_ID);
alter table VRM_RECORD_SCHEDULE
  add constraint PK_VRM_RECORD_SCHEDULE primary key (VRM_REC_SCHED_ID);
alter table VRM_RECORD_SCHEDULE
  add constraint FK_VRM_RECORD_SCHEDULE1 foreign key (PCNVR_ID)
  references PCNVR_SERVER (PCNVR_ID);
alter table VRM_RECORD_SCHEDULE
  add constraint FK_VRM_RECORD_SCHEDULE2 foreign key (SCHED_TEMPL_ID)
  references SCHEDULE_TEMPLATE (SCHED_TEMPL_ID);
alter table VRM_RECORD_SCHEDULE
  add constraint FK_VRM_RECORD_SCHEDULE3 foreign key (NVT_PROXY_ID)
  references NVT_PROXY_SERVER (NVT_PROXY_ID);
alter table VRM_RECORD_SCHEDULE
  add constraint FK_VRM_RECORD_SCHEDULE4 foreign key (CAMERA_ID)
  references CAMERA_INFO (CAMERA_ID);
alter table VRM_RECORD_SCHEDULE
  add constraint FK_VRM_RECORD_SCHEDULE5 foreign key (VRM_ID)
  references VRM_SERVER (VRM_ID);

prompt
prompt Creating table c3p0TestTable
prompt ============================
prompt
create table c3p0TestTable
(
  a CHAR(1)
)
;

prompt
prompt Creating sequence S_ALARM_CHANNEL_INFO
prompt ======================================
prompt
create sequence S_ALARM_CHANNEL_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_DEVICE
prompt ================================
prompt
create sequence S_ALARM_DEVICE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_EVENT
prompt ===============================
prompt
create sequence S_ALARM_EVENT
minvalue 1
maxvalue 9999999999999999999999999999
start with 61
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_EVENT_INPUT
prompt =====================================
prompt
create sequence S_ALARM_EVENT_INPUT
minvalue 1
maxvalue 9999999999999999999999999999
start with 61
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_HOST_CHANNEL_INFO
prompt ===========================================
prompt
create sequence S_ALARM_HOST_CHANNEL_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_HOST_INFO
prompt ===================================
prompt
create sequence S_ALARM_HOST_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_MSG
prompt =============================
prompt
create sequence S_ALARM_MSG
minvalue 1
maxvalue 9999999999999999999999999999
start with 8610
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_SERVER
prompt ================================
prompt
create sequence S_ALARM_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_SNAPSHOT_INFO
prompt =======================================
prompt
create sequence S_ALARM_SNAPSHOT_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_TRIGGER_CLIENT
prompt ========================================
prompt
create sequence S_ALARM_TRIGGER_CLIENT
minvalue 1
maxvalue 9999999999999999999999999999
start with 141
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_TRIGGER_EMAIL
prompt =======================================
prompt
create sequence S_ALARM_TRIGGER_EMAIL
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_TRIGGER_IO
prompt ====================================
prompt
create sequence S_ALARM_TRIGGER_IO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_TRIGGER_MESSAGE
prompt =========================================
prompt
create sequence S_ALARM_TRIGGER_MESSAGE
minvalue 1
maxvalue 9999999999999999999999999999
start with 141
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_TRIGGER_POPUP_VIDEO
prompt =============================================
prompt
create sequence S_ALARM_TRIGGER_POPUP_VIDEO
minvalue 1
maxvalue 9999999999999999999999999999
start with 141
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_TRIGGER_PRESET
prompt ========================================
prompt
create sequence S_ALARM_TRIGGER_PRESET
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_TRIGGER_SNAPSHOT
prompt ==========================================
prompt
create sequence S_ALARM_TRIGGER_SNAPSHOT
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_TRIGGER_SOUND
prompt =======================================
prompt
create sequence S_ALARM_TRIGGER_SOUND
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_TRIGGER_TALK
prompt ======================================
prompt
create sequence S_ALARM_TRIGGER_TALK
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_TRIGGER_TEXT_OVERLAY
prompt ==============================================
prompt
create sequence S_ALARM_TRIGGER_TEXT_OVERLAY
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_TRIGGER_TVWALL
prompt ========================================
prompt
create sequence S_ALARM_TRIGGER_TVWALL
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ALARM_TRIGGER_VRM
prompt =====================================
prompt
create sequence S_ALARM_TRIGGER_VRM
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_ASSET_APPLY_BUY
prompt ===================================
prompt
create sequence S_ASSET_APPLY_BUY
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ASSET_INFO
prompt ==============================
prompt
create sequence S_ASSET_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ASSET_REPAIR
prompt ================================
prompt
create sequence S_ASSET_REPAIR
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ASSET_SCRAP
prompt ===============================
prompt
create sequence S_ASSET_SCRAP
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ASSET_SEQUENCES
prompt ===================================
prompt
create sequence S_ASSET_SEQUENCES
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_BAYONET_SNAP_SITE_SEQ
prompt =========================================
prompt
create sequence S_BAYONET_SNAP_SITE_SEQ
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_BMS_OPERATION_LOG
prompt =====================================
prompt
create sequence S_BMS_OPERATION_LOG
minvalue 1
maxvalue 9999999999999999999999999999
start with 482
increment by 1
cache 20;

prompt
prompt Creating sequence S_BULLETIN
prompt ============================
prompt
create sequence S_BULLETIN
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_CAMERA_COLLECTION_MCU
prompt =========================================
prompt
create sequence S_CAMERA_COLLECTION_MCU
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CAMERA_INFO
prompt ===============================
prompt
create sequence S_CAMERA_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 2401
increment by 1
cache 20;

prompt
prompt Creating sequence S_CASCADE_SHARE
prompt =================================
prompt
create sequence S_CASCADE_SHARE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CASE_CONTACT_PEOPLE
prompt =======================================
prompt
create sequence S_CASE_CONTACT_PEOPLE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CASE_DANGER_PERSON
prompt ======================================
prompt
create sequence S_CASE_DANGER_PERSON
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CASE_HANDLE_NOTE
prompt ====================================
prompt
create sequence S_CASE_HANDLE_NOTE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CASE_INFO
prompt =============================
prompt
create sequence S_CASE_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CASE_LOG
prompt ============================
prompt
create sequence S_CASE_LOG
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CASE_MODULE
prompt ===============================
prompt
create sequence S_CASE_MODULE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CASE_PERSON_CHARACTER
prompt =========================================
prompt
create sequence S_CASE_PERSON_CHARACTER
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CASE_RELATION_INFO
prompt ======================================
prompt
create sequence S_CASE_RELATION_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CASE_REPORTER_INFO
prompt ======================================
prompt
create sequence S_CASE_REPORTER_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CASE_REPORT_INFO
prompt ====================================
prompt
create sequence S_CASE_REPORT_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CASE_SUSPECT
prompt ================================
prompt
create sequence S_CASE_SUSPECT
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CASE_UPLOAD_MATERIAL
prompt ========================================
prompt
create sequence S_CASE_UPLOAD_MATERIAL
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CHAN_RESULT
prompt ===============================
prompt
create sequence S_CHAN_RESULT
minvalue 1
maxvalue 9999999999999999999999999999
start with 1615
increment by 1
cache 20;

prompt
prompt Creating sequence S_CMS_CASCADE
prompt ===============================
prompt
create sequence S_CMS_CASCADE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CMS_MENU
prompt ============================
prompt
create sequence S_CMS_MENU
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CONTRACTORS
prompt ===============================
prompt
create sequence S_CONTRACTORS
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CONTROL_UNIT
prompt ================================
prompt
create sequence S_CONTROL_UNIT
minvalue 1
maxvalue 9999999999999999999999999999
start with 61
increment by 1
cache 20;

prompt
prompt Creating sequence S_CROSSING_CAMERA
prompt ===================================
prompt
create sequence S_CROSSING_CAMERA
minvalue 1
maxvalue 9999999999999999999999999999
start with 61
increment by 1
cache 20;

prompt
prompt Creating sequence S_CROSSING_CAMERA_MAP
prompt =======================================
prompt
create sequence S_CROSSING_CAMERA_MAP
minvalue 1
maxvalue 9999999999999999999999999999
start with 50
increment by 1
cache 20;

prompt
prompt Creating sequence S_CROSSING_DEVICE
prompt ===================================
prompt
create sequence S_CROSSING_DEVICE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_CROSSING_INFO
prompt =================================
prompt
create sequence S_CROSSING_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 61
increment by 1
cache 20;

prompt
prompt Creating sequence S_CROSSING_INTERVAL
prompt =====================================
prompt
create sequence S_CROSSING_INTERVAL
minvalue 1
maxvalue 9999999999999999999999999999
start with 61
increment by 1
cache 20;

prompt
prompt Creating sequence S_CROSSING_SERVER
prompt ===================================
prompt
create sequence S_CROSSING_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_DATA_DICTIONARY
prompt ===================================
prompt
create sequence S_DATA_DICTIONARY
minvalue 1
maxvalue 9999999999999999999999999999
start with 121
increment by 1
cache 20;

prompt
prompt Creating sequence S_DB_NOTIFY
prompt =============================
prompt
create sequence S_DB_NOTIFY
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_DECODE_DEVICE
prompt =================================
prompt
create sequence S_DECODE_DEVICE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_DECODE_RESOURCE
prompt ===================================
prompt
create sequence S_DECODE_RESOURCE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_DECODE_SERVER
prompt =================================
prompt
create sequence S_DECODE_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_DEVICE_INFO
prompt ===============================
prompt
create sequence S_DEVICE_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1067
increment by 1
cache 20;

prompt
prompt Creating sequence S_DIC_ITEM
prompt ============================
prompt
create sequence S_DIC_ITEM
minvalue 1
maxvalue 9999999999999999999999999999
start with 101
increment by 1
cache 20;

prompt
prompt Creating sequence S_DIC_TYPE
prompt ============================
prompt
create sequence S_DIC_TYPE
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_ELECTROMOBILE
prompt =================================
prompt
create sequence S_ELECTROMOBILE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ELEMENT_GROUP
prompt =================================
prompt
create sequence S_ELEMENT_GROUP
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_ELEMENT_GROUP_RESOURCE
prompt ==========================================
prompt
create sequence S_ELEMENT_GROUP_RESOURCE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_EMAP_ELEMENT_INFO
prompt =====================================
prompt
create sequence S_EMAP_ELEMENT_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1687
increment by 1
cache 20;

prompt
prompt Creating sequence S_EMAP_GROUP
prompt ==============================
prompt
create sequence S_EMAP_GROUP
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_EMAP_INFO
prompt =============================
prompt
create sequence S_EMAP_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 41
increment by 1
cache 20;

prompt
prompt Creating sequence S_EMAP_PRESET_INFO
prompt ====================================
prompt
create sequence S_EMAP_PRESET_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_FIRST_TIME_CROSS
prompt ====================================
prompt
create sequence S_FIRST_TIME_CROSS
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_FTP_PICTURE
prompt ===============================
prompt
create sequence S_FTP_PICTURE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_GPS_INFO
prompt ============================
prompt
create sequence S_GPS_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_GROUP_ELEMENT
prompt =================================
prompt
create sequence S_GROUP_ELEMENT
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_GUEST_CONFIG
prompt ================================
prompt
create sequence S_GUEST_CONFIG
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_HISTORICAL_STATISTICS
prompt =========================================
prompt
create sequence S_HISTORICAL_STATISTICS
minvalue 1
maxvalue 9999999999999999999999999999
start with 489
increment by 1
cache 20;

prompt
prompt Creating sequence S_IMAGE_SERVER
prompt ================================
prompt
create sequence S_IMAGE_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_INTER_AREA
prompt ==============================
prompt
create sequence S_INTER_AREA
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_IO_INFO
prompt ===========================
prompt
create sequence S_IO_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 4709
increment by 1
cache 20;

prompt
prompt Creating sequence S_IPSAN
prompt =========================
prompt
create sequence S_IPSAN
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_IPSERVER_DEVICE_INFO
prompt ========================================
prompt
create sequence S_IPSERVER_DEVICE_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_IPSERVER_DEVICE_INFO_HISTORY
prompt ================================================
prompt
create sequence S_IPSERVER_DEVICE_INFO_HISTORY
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_KEYBOARD_SERVER
prompt ===================================
prompt
create sequence S_KEYBOARD_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_LANE_INFO
prompt =============================
prompt
create sequence S_LANE_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 81
increment by 1
cache 20;

prompt
prompt Creating sequence S_LB_CONFIG
prompt =============================
prompt
create sequence S_LB_CONFIG
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_LOGIN_RESTRICTION
prompt =====================================
prompt
create sequence S_LOGIN_RESTRICTION
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_MAG_SERVER
prompt ==============================
prompt
create sequence S_MAG_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_MAP_COORDINATE
prompt ==================================
prompt
create sequence S_MAP_COORDINATE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_MATRIX
prompt ==========================
prompt
create sequence S_MATRIX
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_MONITOR_LOG
prompt ===============================
prompt
create sequence S_MONITOR_LOG
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_MONITOR_LOG_MINOR_TYPE
prompt ==========================================
prompt
create sequence S_MONITOR_LOG_MINOR_TYPE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_MONITOR_SCREEN_GROUP
prompt ========================================
prompt
create sequence S_MONITOR_SCREEN_GROUP
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_MTA_MONITOR
prompt ===============================
prompt
create sequence S_MTA_MONITOR
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_MTA_SECTOR
prompt ==============================
prompt
create sequence S_MTA_SECTOR
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_NETWORK_MONITOR_SERVER
prompt ==========================================
prompt
create sequence S_NETWORK_MONITOR_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_NET_ZONE
prompt ============================
prompt
create sequence S_NET_ZONE
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_NVT_PROXY_SERVER
prompt ====================================
prompt
create sequence S_NVT_PROXY_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_OPERATION_LOG
prompt =================================
prompt
create sequence S_OPERATION_LOG
minvalue 1
maxvalue 9999999999999999999999999999
start with 66011
increment by 1
cache 20;

prompt
prompt Creating sequence S_PAG_SERVER
prompt ==============================
prompt
create sequence S_PAG_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_PCNVR_SERVER
prompt ================================
prompt
create sequence S_PCNVR_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 43
increment by 1
cache 20;

prompt
prompt Creating sequence S_PDC_ALARM
prompt =============================
prompt
create sequence S_PDC_ALARM
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_PLATE_ALARM
prompt ===============================
prompt
create sequence S_PLATE_ALARM
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_POLICE_INFO
prompt ===============================
prompt
create sequence S_POLICE_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_PREPLAN
prompt ===========================
prompt
create sequence S_PREPLAN
minvalue 1
maxvalue 9999999999999999999999999999
start with 41
increment by 1
cache 20;

prompt
prompt Creating sequence S_PREPLAN_ITEM
prompt ================================
prompt
create sequence S_PREPLAN_ITEM
minvalue 1
maxvalue 9999999999999999999999999999
start with 41
increment by 1
cache 20;

prompt
prompt Creating sequence S_PRESET_INFO
prompt ===============================
prompt
create sequence S_PRESET_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 120
increment by 1
cache 20;

prompt
prompt Creating sequence S_PREVIEW_GROUP
prompt =================================
prompt
create sequence S_PREVIEW_GROUP
minvalue 1
maxvalue 9999999999999999999999999999
start with 61
increment by 1
cache 20;

prompt
prompt Creating sequence S_PREVIEW_GROUP_RESOURCE
prompt ==========================================
prompt
create sequence S_PREVIEW_GROUP_RESOURCE
minvalue 1
maxvalue 9999999999999999999999999999
start with 111
increment by 1
cache 20;

prompt
prompt Creating sequence S_PRIVILEGE_INFO
prompt ==================================
prompt
create sequence S_PRIVILEGE_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 412
increment by 1
cache 20;

prompt
prompt Creating sequence S_PTZ_LOCK
prompt ============================
prompt
create sequence S_PTZ_LOCK
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_PTZ_SERVER
prompt ==============================
prompt
create sequence S_PTZ_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_RECORD_INSPECT_DETAIL
prompt =========================================
prompt
create sequence S_RECORD_INSPECT_DETAIL
minvalue 1
maxvalue 9999999999999999999999999999
start with 36508
increment by 1
cache 20;

prompt
prompt Creating sequence S_RECORD_INSPECT_EXCEPTION
prompt ============================================
prompt
create sequence S_RECORD_INSPECT_EXCEPTION
minvalue 1
maxvalue 9999999999999999999999999999
start with 133477
increment by 1
cache 20;

prompt
prompt Creating sequence S_RECORD_INSPECT_TEMPLATE
prompt ===========================================
prompt
create sequence S_RECORD_INSPECT_TEMPLATE
minvalue 1
maxvalue 9999999999999999999999999999
start with 33727
increment by 1
cache 20;

prompt
prompt Creating sequence S_REGION_INFO
prompt ===============================
prompt
create sequence S_REGION_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 70
increment by 1
cache 20;

prompt
prompt Creating sequence S_ROLE_INFO
prompt =============================
prompt
create sequence S_ROLE_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 102
increment by 1
cache 20;

prompt
prompt Creating sequence S_SCHEDULE_TEMPLATE
prompt =====================================
prompt
create sequence S_SCHEDULE_TEMPLATE
minvalue 1
maxvalue 9999999999999999999999999999
start with 41
increment by 1
cache 20;

prompt
prompt Creating sequence S_SERVER_INFO
prompt ===============================
prompt
create sequence S_SERVER_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_SERVER_MULTI_IP
prompt ===================================
prompt
create sequence S_SERVER_MULTI_IP
minvalue 1
maxvalue 9999999999999999999999999999
start with 110
increment by 1
cache 20;

prompt
prompt Creating sequence S_SERVER_STATUS_HIS
prompt =====================================
prompt
create sequence S_SERVER_STATUS_HIS
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_SINGLE_MOBILE_GROUP
prompt =======================================
prompt
create sequence S_SINGLE_MOBILE_GROUP
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_SPEC_DAY
prompt ============================
prompt
create sequence S_SPEC_DAY
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_STREAM_SERVER
prompt =================================
prompt
create sequence S_STREAM_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_SYS_INFO
prompt ============================
prompt
create sequence S_SYS_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 41
increment by 1
cache 20;

prompt
prompt Creating sequence S_SYS_LOG
prompt ===========================
prompt
create sequence S_SYS_LOG
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_TABLE_MAP_LAYER
prompt ===================================
prompt
create sequence S_TABLE_MAP_LAYER
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_TALK_SERVER
prompt ===============================
prompt
create sequence S_TALK_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_TASK_CONFIG
prompt ===============================
prompt
create sequence S_TASK_CONFIG
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_TASK_OBJECT
prompt ===============================
prompt
create sequence S_TASK_OBJECT
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_THIRD_PLATFORM
prompt ==================================
prompt
create sequence S_THIRD_PLATFORM
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_TVWALL_INFO
prompt ===============================
prompt
create sequence S_TVWALL_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_UNALARM_INFO
prompt ================================
prompt
create sequence S_UNALARM_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_USER_GROUP
prompt ==============================
prompt
create sequence S_USER_GROUP
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_USER_INFO
prompt =============================
prompt
create sequence S_USER_INFO
minvalue 1
maxvalue 9999999999999999999999999999
start with 107
increment by 1
cache 20;

prompt
prompt Creating sequence S_VEHICLE_AUDIT
prompt =================================
prompt
create sequence S_VEHICLE_AUDIT
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_VEHICLE_INTERVAL
prompt ====================================
prompt
create sequence S_VEHICLE_INTERVAL
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_VEHICLE_PASS
prompt ================================
prompt
create sequence S_VEHICLE_PASS
minvalue 1
maxvalue 9999999999999999999999999999
start with 195296
increment by 1
cache 20;

prompt
prompt Creating sequence S_VEHICLE_PASS_FIRST
prompt ======================================
prompt
create sequence S_VEHICLE_PASS_FIRST
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_VEHICLE_VIOLATION
prompt =====================================
prompt
create sequence S_VEHICLE_VIOLATION
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_VIDEO_TAG
prompt =============================
prompt
create sequence S_VIDEO_TAG
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_VIOLATION_LOG
prompt =================================
prompt
create sequence S_VIOLATION_LOG
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_VIOLATION_TYPE
prompt ==================================
prompt
create sequence S_VIOLATION_TYPE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_VOICE_SERVER
prompt ================================
prompt
create sequence S_VOICE_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_VQD_SERVER
prompt ==============================
prompt
create sequence S_VQD_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating sequence S_VRB_SERVER
prompt ==============================
prompt
create sequence S_VRB_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 1
increment by 1
cache 20;

prompt
prompt Creating sequence S_VRM_RECORD_SCHEDULE
prompt =======================================
prompt
create sequence S_VRM_RECORD_SCHEDULE
minvalue 1
maxvalue 9999999999999999999999999999
start with 1801
increment by 1
cache 20;

prompt
prompt Creating sequence S_VRM_SERVER
prompt ==============================
prompt
create sequence S_VRM_SERVER
minvalue 1
maxvalue 9999999999999999999999999999
start with 21
increment by 1
cache 20;

prompt
prompt Creating function F_GET_CONTROLTREE
prompt ===================================
prompt
create or replace function F_GET_CONTROLTREE( in_control_id number) return varchar2 is v_contrl_tree_path VARCHAR2(1024); v_control_pid number; v_name  VARCHAR2(64); v_control_id number; begin select control_unit_id,name,parent_id INTO  v_control_id,v_name,v_control_pid from control_unit where control_unit_id = in_control_id ; v_contrl_tree_path := 'controlUnit_'||v_control_id; WHILE v_control_pid >0 loop select control_unit_id,name,parent_id  INTO  v_control_id,v_name,v_control_pid  from control_unit where control_unit_id = v_control_pid ; v_contrl_tree_path := 'controlUnit_'||v_control_id||'/'||v_contrl_tree_path; end loop; return v_contrl_tree_path; end f_get_controltree;
/

prompt
prompt Creating function F_GET_REGIONTREE
prompt ==================================
prompt
create or replace function F_GET_REGIONTREE( in_region_id number) return varchar2 is v_region_tree_path VARCHAR2(1024); v_region_pid number; v_name  VARCHAR2(64); v_region_id number; begin select region_id,name,parent_id  INTO  v_region_id,v_name,v_region_pid  from region_info where region_id = in_region_id ; v_region_tree_path := 'regionInfo_'||v_region_id; WHILE v_region_pid >0 loop select region_id,name,parent_id  INTO  v_region_id,v_name,v_region_pid  from region_info where region_id = v_region_pid ; v_region_tree_path := 'regionInfo_'||v_region_id||'/'||v_region_tree_path; end loop; return v_region_tree_path; end f_get_regiontree;
/

prompt
prompt Creating function GET_MD5
prompt =========================
prompt
CREATE OR REPLACE FUNCTION GET_MD5 ( p_str in varchar2) RETURN varchar2 IS BEGIN RETURN Upper(Utl_Raw.Cast_To_Raw(DBMS_OBFUSCATION_TOOLKIT.MD5(input_string =>P_Str ))); END;
/

prompt
prompt Creating function NOW
prompt =====================
prompt
create or replace function NOW return date is v_date  date; begin  select sysdate into v_date from dual; return v_date; end now;
/


spool off
