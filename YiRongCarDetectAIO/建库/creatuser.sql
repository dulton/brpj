create tablespace yrvm datafile 'D:\app\Administrator\oradata\orcl\yrvm\yrvm.dbf' size 1000m;
create tablespace yrvm_ind datafile 'D:\app\Administrator\oradata\orcl\yrvm\yrvm_ind.dbf' size 1000m;
alter database datafile 'D:\app\Administrator\oradata\orcl\yrvm\yrvm.dbf' autoextend on  next 50m maxsize 30000m;
alter database datafile 'D:\app\Administrator\oradata\orcl\yrvm\yrvm_ind.dbf' autoextend on  next 50m maxsize 10000m;
create user ushow2 identified by ushow2 default tablespace yrvm ;
grant dba to ushow2;



