/* dnf-swdb-pkg.h
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

#ifndef DNF_SWDB_PKG_H
#define DNF_SWDB_PKG_H

#include <glib-object.h>

G_BEGIN_DECLS

#define DNF_TYPE_SWDB_PKG       dnf_swdb_pkg_get_type()

G_DECLARE_FINAL_TYPE (DnfSwdbPkg, dnf_swdb_pkg, DNF, SWDB_PKG, GObject)

#include "dnf-swdb.h"
#include "dnf-swdb-types.h"

struct _DnfSwdbPkg
{
    GObject parent_instance;
    gchar *name;
    gint epoch;
    gchar *version;
    gchar *release;
    gchar *arch;
    gchar *checksum_data;
    gchar *checksum_type;
    DnfSwdbItem type;
    gboolean done;
    gchar *state;
    gint obsoleting;
    gint pid;
    gchar *_ui_repo;
    gchar *nevra;
    DnfSwdb *swdb;
};

DnfSwdbPkg          *dnf_swdb_pkg_new               (const gchar *name,
                                                     gint         epoch,
                                                     const gchar *version,
                                                     const gchar *release,
                                                     const gchar *arch,
                                                     const gchar *checksum_data,
                                                     const gchar *checksum_type,
                                                     DnfSwdbItem  type);
gchar              *dnf_swdb_pkg_ui_from_repo       (DnfSwdbPkg    *self);
DnfSwdbReason       dnf_swdb_pkg_get_reason         (DnfSwdbPkg    *self);
gint64              dnf_swdb_pkg_compare            (DnfSwdbPkg    *pkg1,
                                                     DnfSwdbPkg    *pkg2);
gboolean            dnf_swdb_pkg___lt__             (DnfSwdbPkg    *first,
                                                     DnfSwdbPkg    *second);
gboolean            dnf_swdb_pkg___gt__             (DnfSwdbPkg    *first,
                                                     DnfSwdbPkg    *second);
gchar              *dnf_swdb_pkg___str__            (DnfSwdbPkg    *pkg);
gboolean            dnf_swdb_pkg_match              (DnfSwdbPkg    *pkg,
                                                     const gchar   *pattern);

G_END_DECLS

#endif
