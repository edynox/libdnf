/* dnf-swdb-sql.h
*
* Copyright (C) 2016 Red Hat, Inc.
* Author: Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
*
* Licensed under the GNU Lesser General Public License Version 2.1
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser GeneDral Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef _DNF_SWDB_SQL
#define _DNF_SWDB_SQL

#define I_GROUP "Insert into GROUPS values(null, @name_id, @name, @ui_name, @is_installed, @pkg_types, @grp_types)"
#define I_ENV "Insert into ENVIRONMENTS values(null, @name_id, @name, @ui_name, @pkg_types, @grp_types)"
#define I_ENV_GROUP "insert into ENVIRONMENTS_GROUPS values(null, @eid, @gid)"
#define INSERT_PKG "insert into PACKAGE values(null,@name,@epoch,@version,@release,@arch,@cdata,@ctype,@type)"
#define INSERT_OUTPUT "insert into OUTPUT values(null,@tid,@msg,@type)"
#define INSERT_TRANS_BEG "insert into TRANS values(null,@beg,null,@rpmdbv,null,@cmdline,@loginuid,@releasever,null)"
#define INSERT_TRANS_END "UPDATE TRANS SET end_timestamp=@end,end_RPMDB_version=@rpmdbv, return_code=@rc WHERE T_ID=@tid"
#define INSERT_REPO "insert into REPO values(null,@name,null,null)"

#define I_TRANS_GROUP_DATA  "insert into TRANS_GROUP_DATA values(null,@tid,@gid,@name_id,@name,@ui_name,"\
                            "@is_installed,@pkg_types,@grp_types)"

#define UPDATE_PKG_DATA "UPDATE PACKAGE_DATA SET R_ID=@rid,from_repo_revision=@repo_r,from_repo_timestamp=@repo_t,"\
                        "installed_by=@installed_by,changed_by=@changed_by,installonly=@installonly,"\
                        "origin_url=@origin_url where P_ID=@pid"

#define INSERT_PKG_DATA "Insert into PACKAGE_DATA values (null, @pid, @rid, @repo_r, @repo_t,"\
                        "@installed_by,@changed_by,@installonly, @origin_url)"

#define INSERT_RPM_DATA "INSERT into RPM_DATA values(null,@pid,@buildtime,@buildhost,@license,@packager,@size,@sourcerpm,@url,"\
                        "@vendor,@committer,@committime)"

#define INSERT_TRANS_DATA_BEG "insert into TRANS_DATA values(null,@tid,@pdid,null,@done,null,@reason,@state)"
#define UPDATE_TRANS_DATA_PID_END "UPDATE TRANS_DATA SET done=@done WHERE T_ID=@tid and PD_ID=@pdid and state=@state"

#define FIND_REPO_BY_NAME "SELECT R_ID FROM REPO WHERE name=@name"
#define FIND_PDID_FROM_PID "SELECT PD_ID FROM PACKAGE_DATA WHERE P_ID=@pid ORDER by PD_ID DESC LIMIT 1"
#define FIND_ALL_PDID_FOR_PID "SELECT PD_ID FROM PACKAGE_DATA WHERE P_ID=@pid"
#define GET_TRANS_CMDLINE "SELECT cmdline FROM TRANS WHERE T_ID=@tid"

#define INSERT_PDID "insert into PACKAGE_DATA values(null,@pid,null,null,null,null,null,null,'#')"
#define FIND_TID_FROM_PDID "SELECT T_ID FROM TRANS_DATA WHERE PD_ID=@pdid LIMIT 1"
#define FIND_TIDS_FROM_PDID "SELECT T_ID FROM TRANS_DATA WHERE PD_ID=@pdid"
#define LOAD_OUTPUT "SELECT msg FROM OUTPUT WHERE T_ID=@tid and type=@type"
#define PKG_DATA_ATTR_BY_PID "FROM PACKAGE_DATA WHERE P_ID=@pid"
#define TRANS_DATA_ATTR_BY_PDID "FROM TRANS_DATA WHERE PD_ID=@pdid"
#define TRANS_ATTR_BY_TID "FROM TRANS WHERE T_ID=@tid"
#define RPM_ATTR_BY_PID    "FROM RPM_DATA WHERE P_ID=@pid"
#define PID_BY_TID  "select P_ID from TRANS_DATA join PACKAGE_DATA using(PD_ID) where T_ID=@tid"

#define FIND_PIDS_BY_NAME "SELECT P_ID FROM PACKAGE WHERE NAME LIKE @pattern"

#define SIMPLE_SEARCH "SELECT P_ID FROM PACKAGE WHERE name LIKE @pat"
#define SEARCH_PATTERN "SELECT P_ID,name,epoch,version,release,arch,"\
  "name || '.' || arch AS sql_nameArch,"\
  "name || '-' || version || '-' || release || '.' || arch AS sql_nameVerRelArch,"\
  "name || '-' || version AS sql_nameVer,"\
  "name || '-' || version || '-' || release AS sql_nameVerRel,"\
  "epoch || ':' || name || '-' || version || '-' || release || '-' || arch AS sql_envra,"\
  "name || '-' || epoch || '-' || version || '-' || release || '-' || arch AS sql_nevra "\
  "FROM PACKAGE WHERE sql_nameArch LIKE @pat OR sql_nameVerRelArch LIKE @pat OR sql_nameVer LIKE @pat OR"\
  " sql_nameVerRel LIKE @pat OR sql_nevra LIKE @pat OR sql_envra LIKE @pat"

#define S_PDID_TDID_BY_PID  "SELECT PD_ID,TD_ID from PACKAGE_DATA join TRANS_DATA"\
                            " using (PD_ID) where P_ID=@pid ORDER by PD_ID DESC LIMIT 1"
#define S_PACKAGE_BY_PID "SELECT * FROM PACKAGE WHERE P_ID=@pid"
#define S_NAME_BY_PID "SELECT name FROM PACKAGE WHERE P_ID=@pid"
#define S_LAST_TDID_BY_NAME "SELECT TD_ID FROM PACKAGE join PACKAGE_DATA using(P_ID) "\
                            "join TRANS_DATA using(PD_ID) WHERE name=@name "\
                            "and P_ID!=@pid and T_ID!=@tid ORDER BY TD_ID DESC LIMIT 1"

#define S_LAST_W_TDID_BY_NAME   "SELECT TD_ID FROM PACKAGE join PACKAGE_DATA using(P_ID) "\
                                "join TRANS_DATA using(PD_ID) WHERE name=@name "\
                                "and T_ID!=@tid ORDER BY TD_ID DESC LIMIT 1"

#define S_PACKAGE_DATA_BY_PID "SELECT * FROM PACKAGE_DATA WHERE P_ID=@pid"
#define S_REPO_BY_RID "select name from REPO where R_ID=@rid"
#define S_PREV_AUTO_PD "SELECT PD_ID FROM PACKAGE_DATA where P_ID=@pid and origin_url='#'"

#define S_TRANS "SELECT * from TRANS ORDER BY T_ID DESC"
#define S_TRANS_W_LIMIT "SELECT * from TRANS ORDER BY T_ID DESC LIMIT @limit"
#define S_TRANS_COMP "SELECT * from TRANS WHERE end_timestamp is not null or end_timestamp!='' ORDER BY T_ID DESC"
#define S_TRANS_COMP_W_LIMIT "SELECT * from TRANS WHERE end_timestamp is not null or end_timestamp!='' ORDER BY T_ID DESC LIMIT @limit"

#define S_TRANS_DATA_BY_TID "SELECT * FROM TRANS_DATA WHERE T_ID=@tid"
#define S_PACKAGE_STATE "select TD_ID,done,state from PACKAGE_DATA join TRANS_DATA using (PD_ID) where P_ID=@pid order by TD_ID desc limit 1"
#define S_GID_BY_NAME_ID "Select G_ID from GROUPS where name_id LIKE @id"
#define S_GROUP_BY_NAME_ID "select * from GROUPS where name_id LIKE @id"
#define S_ENV_BY_NAME_ID "select * from ENVIRONMENTS where name_id LIKE @id"
#define S_GROUPS_BY_PATTERN "SELECT * from GROUPS where name_id LIKE @pat or name LIKE @pat or ui_name LIKE @pat"
#define S_ENV_BY_PATTERN "SELECT * from ENVIRONMENTS where name_id LIKE @pat or name LIKE @pat or ui_name LIKE @pat"
#define S_GROUP_EXCLUDE_BY_ID "SELECT name FROM GROUPS_EXCLUDE where G_ID=@gid"
#define S_GROUP_PACKAGE_BY_ID "SELECT name FROM GROUPS_PACKAGE where G_ID=@gid"
#define S_ENV_EXCLUDE_BY_ID "SELECT name FROM ENVIRONMENTS_EXCLUDE where E_ID=@eid"
#define S_IS_INSTALLED_BY_EID "SELECT is_installed FROM ENVIRONMENTS_GROUPS join GROUPS using(G_ID) where E_ID=@eid"
#define S_GROUP_NAME_ID_BY_EID "SELECT name_id FROM ENVIRONMENTS_GROUPS join GROUPS using(G_ID) where E_ID=@eid"
#define S_REPO_FROM_PID "SELECT name, PD_ID FROM PACKAGE_DATA join REPO using(R_ID) where P_ID=@pid"
#define S_REPO_FROM_PID2 "SELECT name FROM PACKAGE_DATA join REPO using(R_ID) where P_ID=@pid"
#define S_RELEASEVER_FROM_PDID "SELECT releasever from TRANS_DATA join TRANS using(T_ID) where PD_ID=@pdid"
#define S_CHECKSUMS_BY_NVRA "SELECT checksum_data, checksum_type FROM PACKAGE WHERE name || '-' || version || '-' || release || '.' || arch=@nvra"
#define S_PID_BY_NVRA "SELECT P_ID FROM PACKAGE WHERE name || '-' || version || '-' || release || '.' || arch=@nvra"
#define U_GROUP_COMMIT "UPDATE GROUPS SET is_installed=1 where name_id=@id"
#define S_REASON_ID_BY_PDID "SELECT reason FROM PACKAGE_DATA join TRANS_DATA using(PD_ID) where PD_ID=@pdid"

#define U_GROUP "UPDATE GROUPS SET name=@name,ui_name=@ui_name,is_installed=@is_installed,pkg_types=@pkg_types,"\
                "grp_types=@grp_types where G_ID=@gid"

#define U_REASON_BY_PDID "UPDATE TRANS_DATA SET reason=@reason where PD_ID=@pdid"

#define R_FULL_LIST_BY_ID "DELETE FROM GROUPS_PACKAGE WHERE G_ID=@gid"

#define U_REPO_BY_PID "UPDATE PACKAGE_DATA SET R_ID=@rid where P_ID=@pid"

#define U_ORIGINAL_TDID_BY_TDID "UPDATE TRANS_DATA set ORIGINAL_TD_ID=@orig where TD_ID=@tdid"

//CREATION OF tables

#define C_PKG_DATA 		"CREATE TABLE PACKAGE_DATA ( PD_ID integer PRIMARY KEY,"\
                    	"P_ID integer, R_ID integer, from_repo_revision text,"\
                    	"from_repo_timestamp text, installed_by text, changed_by text,"\
                    	"installonly text, origin_url text)"

#define C_PKG 			"CREATE TABLE PACKAGE ( P_ID integer primary key, name text,"\
						"epoch text, version text, release text, arch text,"\
						"checksum_data text, checksum_type text, type integer)"

#define C_REPO			"CREATE TABLE REPO (R_ID INTEGER PRIMARY KEY, name text,"\
                    	"last_synced text, is_expired text)"

#define C_TRANS_DATA	"CREATE TABLE TRANS_DATA (TD_ID INTEGER PRIMARY KEY,"\
                        "T_ID integer,PD_ID integer, G_ID integer, done INTEGER,"\
                        "ORIGINAL_TD_ID integer, reason integer, state integer)"

#define C_TRANS 		"CREATE TABLE TRANS (T_ID integer primary key, beg_timestamp text,"\
                        "end_timestamp text, beg_RPMDB_version text, end_RPMDB_version ,cmdline text,"\
                        "loginuid text, releasever text, return_code integer)"

#define C_OUTPUT		"CREATE TABLE OUTPUT (O_ID integer primary key,T_ID INTEGER,"\
						"msg text, type integer)"


#define C_STATE_TYPE	"CREATE TABLE STATE_TYPE (ID INTEGER PRIMARY KEY, description text)"
#define C_REASON_TYPE	"CREATE TABLE REASON_TYPE (ID INTEGER PRIMARY KEY, description text)"
#define C_OUTPUT_TYPE	"CREATE TABLE OUTPUT_TYPE (ID INTEGER PRIMARY KEY, description text)"
#define C_PKG_TYPE		"CREATE TABLE PACKAGE_TYPE (ID INTEGER PRIMARY KEY, description text)"

#define C_GROUPS		"CREATE TABLE GROUPS (G_ID INTEGER PRIMARY KEY, name_id text, name text,"\
                        "ui_name text, is_installed integer, pkg_types integer, grp_types integer)"

#define C_T_GROUP_DATA	"CREATE TABLE TRANS_GROUP_DATA (TG_ID INTEGER PRIMARY KEY,"\
						"T_ID integer, G_ID integer, name_id text, name text, ui_name text,"\
                        "is_installed integer, pkg_types integer, grp_types integer)"

#define C_GROUPS_PKG	"CREATE TABLE GROUPS_PACKAGE (GP_ID INTEGER PRIMARY KEY,"\
                        "G_ID integer, name text)"

#define C_GROUPS_EX		"CREATE TABLE GROUPS_EXCLUDE (GE_ID INTEGER PRIMARY KEY,"\
                        "G_ID integer, name text)"

#define C_ENV_GROUPS	"CREATE TABLE ENVIRONMENTS_GROUPS (EG_ID INTEGER PRIMARY KEY,"\
                        "E_ID integer, G_ID integer)"

#define C_ENV			"CREATE TABLE ENVIRONMENTS (E_ID INTEGER PRIMARY KEY, name_id text,"\
                        "name text, ui_name text, pkg_types integer, grp_types integer)"

#define C_ENV_EX		"CREATE TABLE ENVIRONMENTS_EXCLUDE (EE_ID INTEGER PRIMARY KEY,"\
                        "E_ID integer, name text)"
#define C_RPM_DATA      "CREATE TABLE RPM_DATA (RPM_ID INTEGER PRIMARY KEY, P_ID INTEGER,"\
                        "buildtime TEXT, buildhost TEXT, license TEXT, packager TEXT, size TEXT,"\
                        "sourcerpm TEXT, url TEXT, vendor TEXT, committer TEXT, committime TEXT)"
#define C_INDEX_NVRA    "create index nvra on PACKAGE(name || '-' || version || '-' || release || '.' || arch)"

#endif
