/* dnf-swdb-obj.h
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
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef DNF_SWDB_OBJ_H
#define DNF_SWDB_OBJ_H

#include <glib-object.h>

G_BEGIN_DECLS

#define DNF_TYPE_SWDB_PKGDATA   dnf_swdb_pkgdata_get_type()
#define DNF_TYPE_SWDB_TRANSDATA dnf_swdb_transdata_get_type()
#define DNF_TYPE_SWDB_RPMDATA   dnf_swdb_rpmdata_get_type()

G_DECLARE_FINAL_TYPE (DnfSwdbPkgData, dnf_swdb_pkgdata, DNF, SWDB_PKGDATA, GObject)
G_DECLARE_FINAL_TYPE (DnfSwdbTransData, dnf_swdb_transdata, DNF, SWDB_TRANSDATA, GObject)
G_DECLARE_FINAL_TYPE (DnfSwdbRpmData, dnf_swdb_rpmdata, DNF, SWDB_RPMDATA, GObject)

#include "dnf-swdb.h"
#include "dnf-swdb-types.h"

struct _DnfSwdbPkgData
{
    GObject parent_instance;
    gchar *from_repo;
    gchar *from_repo_revision;
    gchar *from_repo_timestamp;
    gchar *installed_by;
    gchar *changed_by;
    gchar *installonly;
    gint pdid;
    gint pid;
};

struct _DnfSwdbTransData
{
    GObject parent_instance;
    gint tdid;
    gint tid;
    gint pdid;
    gint tgid;
    gint done;
    gint ORIGINAL_TD_ID;
    DnfSwdbReason reason;
    gchar *state;
};

struct _DnfSwdbRpmData
{
    GObject parent_instance;
    gint   pid;
    gchar *buildtime;
    gchar *buildhost;
    gchar *license;
    gchar *packager;
    gchar *size;
    gchar *sourcerpm;
    gchar *url;
    gchar *vendor;
    gchar *committer;
    gchar *committime;
};

DnfSwdbPkgData     *dnf_swdb_pkgdata_new            (const gchar   *from_repo_revision,
                                                     const gchar   *from_repo_timestamp,
                                                     const gchar   *installed_by,
                                                     const gchar   *changed_by,
                                                     const gchar   *installonly,
                                                     const gchar   *from_repo);
DnfSwdbTransData   *dnf_swdb_transdata_new          (gint           tdid,
                                                     gint           tid,
                                                     gint           pdid,
                                                     gint           tgid,
                                                     gint           done,
                                                     gint           ORIGINAL_TD_ID,
                                                     DnfSwdbReason  reason,
                                                     gchar         *state);
DnfSwdbRpmData     *dnf_swdb_rpmdata_new            (gint           pid,
                                                     const gchar   *buildtime,
                                                     const gchar   *buildhost,
                                                     const gchar   *license,
                                                     const gchar   *packager,
                                                     const gchar   *size,
                                                     const gchar   *sourcerpm,
                                                     const gchar   *url,
                                                     const gchar   *vendor,
                                                     const gchar   *committer,
                                                     const gchar   *committime);
G_END_DECLS

#endif
