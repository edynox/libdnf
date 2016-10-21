/* dnf-swdb-test.c
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

/** TODO
* dnf_swdb_search
* dnf_swdb_checksums_by_nvras
* dnf_swdb_select_user_installed
* dnf_swdb_user_installed
* ----- GROUPS -----
* dnf_swdb_group_add_package
* dnf_swdb_group_add_exclude
* dnf_swdb_env_add_exclude
* dnf_swdb_env_add_group
* dnf_swdb_add_group
* dnf_swdb_add_env
* dnf_swdb_get_group
* dnf_swdb_get_env
* dnf_swdb_groups_by_pattern
* dnf_swdb_env_by_pattern
* dnf_swdb_group_get_exclude
* dnf_swdb_group_get_full_list
* dnf_swdb_group_update_full_list
* dnf_swdb_uninstall_group
* dnf_swdb_env_get_grp_list
* dnf_swdb_env_is_installed
* dnf_swdb_env_get_exclude
* dnf_swdb_groups_commit
* dnf_swdb_log_group_trans
* ------ REPOS -----
* dnf_swdb_repo_by_pattern
* dnf_swdb_set_repo
* ----- PACKAGE -----
* dnf_swdb_add_package_nevracht
* dnf_swdb_get_pid_by_nevracht
* dnf_swdb_log_package_data
* dnf_swdb_get_pkg_attr
* dnf_swdb_attr_by_pattern
* dnf_swdb_get_packages_by_tid
* dnf_swdb_pkg_get_ui_from_repo
* dnf_swdb_package_by_pattern
* dnf_swdb_package_data_by_pattern
* dnf_swdb_set_reason
* dnf_swdb_mark_user_installed
* dnf_swdb_add_package #
* ------ RPM ------
* dnf_swdb_log_rpm_data
* ------ TRANS -----
* dnf_swdb_trans_data_beg
* dnf_swdb_trans_data_end
* dnf_swdb_trans_data_pid_end
* dnf_swdb_trans_beg #
* dnf_swdb_trans_end #
* dnf_swdb_trans_cmdline
* dnf_swdb_get_old_trans_data
* dnf_swdb_trans_old
* dnf_swdb_last * repair this in dnf - complete only called automaticaly
* dnf_swdb_log_error
* dnf_swdb_log_output
* dnf_swdb_load_error
* dnf_swdb_load_output
* ------ UTIL -----
* dnf_swdb_get_path
* dnf_swdb_set_path
* dnf_swdb_exist
* dnf_swdb_create_db
* dnf_swdb_reset_db
* ------ intern ------
* dnf_swdb_open #
* dnf_swdb_close #
* dnf_swdb_get_package_type
* dnf_swdb_get_output_type
* dnf_swdb_get_reason_type
* dnf_swdb_get_state_type
* # = covered
*/

#include <glib.h>
#include <glib-object.h>
#include "libdnf/dnf-swdb.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define INIT_PACAKGES 1000

/**
 * Generate some random string with given length
 */
static gchar * generate_str(guint len)
{
    gchar *str = malloc (sizeof(gchar)*(len+1));
    for (guint i = 0; i< len; ++i)
    {
        str[i] = rand() % 26 + 97;
    }
    str[len] = 0;
    return str;
}

/**
 * Generate ordinary package with some random attributes
 */
static DnfSwdbPkg * generate_package ()
{
    gchar* name = generate_str(8);
    const gchar* epoch = (rand() % 2) ? "0":"1";
    gchar* version = generate_str(4);
    gchar* release = generate_str(6);
    gchar* arch = generate_str(4);
    gchar* checksum_data = generate_str(64);
    const gchar* checksum_type = (rand() % 10) ? "sha256" : "sha1";
    const gchar *type = (rand() % 10) ? "rpm" : "source";
    DnfSwdbPkg *pkg = dnf_swdb_pkg_new(
        name,
        epoch,
        version,
        release,
        arch,
        checksum_data,
        checksum_type,
        type
    );
    g_free(name);
    g_free(version);
    g_free(release);
    g_free(arch);
    g_free(checksum_data);
    g_assert(pkg);
    return pkg;
}

/**
 * Generate some package data with random attributes
 */
static DnfSwdbPkgData * generate_package_data ()
{
	const gchar *from_repo = (rand() % 2) ? "testaconda" : "testdora";
    gchar *from_repo_revision = generate_str(16);
  	gchar *from_repo_timestamp = malloc (11*sizeof(gchar));
    snprintf(from_repo_timestamp, 11, "%d", (gint)time(NULL));
  	const gchar *installed_by = "1000";
  	const gchar *changed_by = (rand()% 25) ? NULL : "4242" ;
  	const gchar *installonly = (rand()% 100) ? NULL : "True";
  	const gchar *origin_url =
        (rand()% 10) ? NULL : "https://github.com/edynox/libhif";

    DnfSwdbPkgData *pkg_data = dnf_swdb_pkgdata_new(
                        from_repo_revision,
                        from_repo_timestamp,
                        installed_by,
                        changed_by,
                        installonly,
                        origin_url,
                        from_repo
    );
    g_free(from_repo_revision);
    g_free(from_repo_timestamp);
    return pkg_data;
}

static gint begin_trans( DnfSwdb *self)
{
    gchar *time_str = malloc (11*sizeof(gchar));
    snprintf(time_str, 11, "%d", (gint)time(NULL));

    gchar *rpmdb_version = generate_str(32);
    gchar *cmdline = generate_str(16);

    //open initial transaction
    guint tid =  dnf_swdb_trans_beg(    self,
                                        time_str,
                                        rpmdb_version,
    					                cmdline,
    					                 "1000",
    					                "42");
    g_free(time_str);
    g_free(rpmdb_version);
    g_free(cmdline);
    return tid;
}

static gint end_trans ( DnfSwdb *self,
                        gint tid)
{
    gint rc;
    gchar *time_str = malloc (11*sizeof(gchar));
    gchar *rpmdb_version = generate_str(32);

    snprintf(time_str, 11, "%d", (gint)time(NULL));

    //close transaction as success
    rc =  dnf_swdb_trans_end(   self,
                                tid,
                                time_str,
                                rpmdb_version,
    					        0);
    g_free(time_str);
    g_free(rpmdb_version);
    return rc;
}


gint main ()
{
    //create DB object
    DnfSwdb *self = dnf_swdb_new( "/tmp", "42");

    //reset database
    dnf_swdb_reset_db(self);

    gint rc;
    srand(time(NULL));

    //lets do some intial transaction
    guint tid = begin_trans(self);
    g_assert(tid); //tid not 0

    //now add some packages
    for (guint i = 0; i < INIT_PACAKGES; ++i)
    {
        DnfSwdbPkg *pkg = generate_package();

        //add pkg to database
        g_assert(dnf_swdb_add_package(self, pkg));

        DnfSwdbPkgData *pkg_data = generate_package_data();

        //add package data
        g_assert(!dnf_swdb_log_package_data(self, pkg->pid, pkg_data));

        //dont forget trans data
        g_assert(!dnf_swdb_trans_data_beg(  self,
                                            tid,
                                            pkg->pid,
                                            "user", //reason
                                            "Installed" //state
        ));
        g_object_unref(pkg);
        g_object_unref(pkg_data);
    }





    //close transaction
    rc = end_trans(self, tid);
    g_assert(!rc);



    return 0;
}
