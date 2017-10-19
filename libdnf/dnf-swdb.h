/* dnf-swdb.h
 *
 * Copyright (C) 2016 Red Hat, Inc.
 * Author: Eduard Cuba <ecuba@redhat.com>
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
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef DNF_SWDB_H
#define DNF_SWDB_H

#define DNF_SWDB_DEFAULT_PATH           "/var/lib/dnf/history/swdb.sqlite"

#define DB_PREP(db, sql, res)           g_assert(_db_prepare(db, sql, &res))
#define DB_BIND(res, id, source)        g_assert(_db_bind(res, id, source))
#define DB_BIND_INT(res, id, source)    g_assert(_db_bind_int(res, id, source))
#define DB_STEP(res)                    g_assert(_db_step(res))
#define DB_FIND(res)                    _db_find(res)
#define DB_FIND_MULTI(res)              _db_find_multi(res)
#define DB_FIND_STR(res)                _db_find_str(res)
#define DB_FIND_STR_MULTI(res)          _db_find_str_multi(res)

#include <glib-object.h>
#include <glib.h>
#include <sqlite3.h>

G_BEGIN_DECLS

#define DNF_TYPE_SWDB dnf_swdb_get_type ()

// structure,function prefix,namespace,object name,inherits
G_DECLARE_FINAL_TYPE (DnfSwdb, dnf_swdb, DNF, SWDB, GObject)

#include "dnf-swdb-pkg.h"
#include "dnf-swdb-obj.h"
#include "dnf-swdb-groups.h"
#include "dnf-swdb-trans.h"
#include "dnf-swdb-db.h"

struct _DnfSwdb
{
    GObject parent_instance;
    gchar   *path;
    sqlite3 *db;
    gboolean ready; //db opened
    gchar *releasever;
};

DnfSwdb        *dnf_swdb_new                    (const gchar    *db_path,
                                                 const gchar    *releasever);
const gchar    *dnf_swdb_get_path               (DnfSwdb        *self);
void            dnf_swdb_set_path               (DnfSwdb        *self,
                                                 const gchar    *path);
gboolean        dnf_swdb_exist                  (DnfSwdb        *self);
gint            dnf_swdb_create_db              (DnfSwdb        *self);
gint            dnf_swdb_reset_db               (DnfSwdb        *self);
gint            dnf_swdb_open                   (DnfSwdb        *self);
void            dnf_swdb_close                  (DnfSwdb        *self);
gint            dnf_swdb_get_package_type       (DnfSwdb        *self,
                                                 const gchar    *type);
gint            dnf_swdb_get_output_type        (DnfSwdb        *self,
                                                 const gchar    *type);
gint            dnf_swdb_get_reason_type        (DnfSwdb        *self,
                                                 const gchar    *type);
gchar          *_reason_by_pid                  (sqlite3        *db,
                                                 gint            pid);
gchar          *dnf_swdb_reason_by_nevra        (DnfSwdb        *self,
                                                 const gchar    *nevra);
gint            dnf_swdb_get_state_type         (DnfSwdb        *self,
                                                 const gchar    *type);
gint            dnf_swdb_add_package            (DnfSwdb        *self,
                                                 DnfSwdbPkg     *pkg);
gint            dnf_swdb_log_error              (DnfSwdb        *self,
                                                 gint            tid,
                                                 const gchar    *msg);
gint            dnf_swdb_log_output             (DnfSwdb        *self,
                                                 gint           tid,
                                                 const gchar    *msg);
gint            dnf_swdb_trans_beg              (DnfSwdb        *self,
                                                 const gchar    *timestamp,
                                                 const gchar    *rpmdb_version,
                                                 const gchar    *cmdline,
                                                 const gchar    *loginuid,
                                                 const gchar    *releasever);
gint            dnf_swdb_trans_end              (DnfSwdb        *self,
                                                 gint            tid,
                                                 const gchar    *end_timestamp,
                                                 const gchar    *end_rpmdb_version,
                                                 gint            return_code);
gint            dnf_swdb_log_package_data       (DnfSwdb        *self,
                                                 gint            pid,
                                                 DnfSwdbPkgData *pkgdata );
gint            dnf_swdb_trans_data_beg         (DnfSwdb        *self,
                                                 gint            tid,
                                                 gint            pid,
                                                 const gchar    *reason,
                                                 const gchar    *state );
gint            dnf_swdb_trans_data_pid_end     (DnfSwdb        *self,
                                                 gint            pid,
                                                 gint            tid,
                                                 const gchar    *state);
GPtrArray      *dnf_swdb_load_error             (DnfSwdb        *self,
                                                 gint            tid);
GPtrArray      *dnf_swdb_load_output            (DnfSwdb        *self,
                                                 gint            tid);
gchar          *_get_description                (sqlite3        *db,
                                                 gint            id,
                                                 const gchar    *sql);
GArray         *dnf_swdb_search                 (DnfSwdb        *self,
                                                 GPtrArray      *patterns);
gint            _pdid_from_pid                  (sqlite3        *db,
                                                 gint            pid);
gchar          *_repo_by_rid                    (sqlite3        *db,
                                                 gint            rid);
GPtrArray      *dnf_swdb_get_packages_by_tid    (DnfSwdb        *self,
                                                 gint            tid);
gchar          *dnf_swdb_trans_cmdline          (DnfSwdb        *self,
                                                 gint            tid);
GPtrArray      *dnf_swdb_trans_old              (DnfSwdb        *self,
                                                 GArray         *tids,
                                                 gint            limit,
                                                 gboolean        complete_only);
gint            dnf_swdb_add_group              (DnfSwdb        *self,
                                                 DnfSwdbGroup   *group);
gint            dnf_swdb_add_env                (DnfSwdb        *self,
                                                 DnfSwdbEnv     *env);
gboolean        dnf_swdb_removable_pkg          (DnfSwdb        *self,
                                                 const gchar    *pkg_name);
DnfSwdbGroup   *dnf_swdb_get_group              (DnfSwdb        *self,
                                                 const gchar    *name_id);
GPtrArray      *dnf_swdb_groups_by_pattern      (DnfSwdb        *self,
                                                 const gchar    *pattern);
gint            dnf_swdb_uninstall_group        (DnfSwdb        *self,
                                                 DnfSwdbGroup   *group);
DnfSwdbEnv     *dnf_swdb_get_env                (DnfSwdb        *self,
                                                 const gchar    *name_id);
gint            dnf_swdb_groups_commit          (DnfSwdb        *self,
                                                 GPtrArray      *groups);
GPtrArray      *dnf_swdb_env_by_pattern         (DnfSwdb        *self,
                                                 const gchar    *pattern);
gint            dnf_swdb_log_group_trans        (DnfSwdb        *self,
                                                 gint            tid,
                                                 GPtrArray      *installing,
                                                 GPtrArray      *removing);
DnfSwdbTrans   *dnf_swdb_last                   (DnfSwdb        *self,
                                                 gboolean        complete_only);
DnfSwdbPkg     *dnf_swdb_package_by_nevra       (DnfSwdb        *self,
                                                 const gchar    *nevra);
DnfSwdbPkgData *dnf_swdb_package_data_by_nevra  (DnfSwdb        *self,
                                                 const gchar    *nevra);
gchar          *dnf_swdb_repo_by_nevra          (DnfSwdb        *self,
                                                 const gchar    *nevra);
gint            dnf_swdb_mark_user_installed    (DnfSwdb        *self,
                                                 const gchar    *nevra,
                                                 gboolean        user_installed);
GPtrArray      *dnf_swdb_checksums_by_nevras    (DnfSwdb        *self,
                                                 GPtrArray      *nevras);
gint            dnf_swdb_set_reason             (DnfSwdb        *self,
                                                 const gchar    *nevra,
                                                 const gchar    *reason);
gint            dnf_swdb_set_repo               (DnfSwdb        *self,
                                                 const gchar    *nevra,
                                                 const gchar    *repo);
gboolean        dnf_swdb_user_installed         (DnfSwdb        *self,
                                                 const gchar    *nevra);
gchar          *_repo_by_pid                    (sqlite3        *db,
                                                 gint            pid);
GArray         *dnf_swdb_select_user_installed  (DnfSwdb        *self,
                                                 GPtrArray      *nevras);
gint            _get_description_id             (sqlite3        *db,
                                                 const gchar    *desc,
                                                 const gchar    *sql);
gint            dnf_swdb_add_rpm_data           (DnfSwdb        *self,
                                                 DnfSwdbRpmData *rpm_data);
gint            dnf_swdb_pid_by_nevra           (DnfSwdb        *self,
                                                 const gchar    *nevra);
void            dnf_swdb_trans_with             (DnfSwdb        *self,
                                                 int             tid,
                                                 int             pid);
DnfSwdbPkg     *_get_package_by_pid             (sqlite3        *db,
                                                 gint            pid);
void            dnf_swdb_trans_with_libdnf      (DnfSwdb        *self,
                                                 int             tid);
gchar          *dnf_swdb_get_erased_reason      (DnfSwdb        *self,
                                                 gchar          *nevra,
                                                 gint            first_trans,
                                                 gboolean        rollback);

G_END_DECLS

#endif
