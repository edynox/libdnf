/* statement.cpp
 *
 * Copyright (C) 2017 Red Hat, Inc.
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

#include "statement.hpp"

Statement::Statement ()
  : res (res)
{
}

void
Statement::exec ()
{
    // TODO handle error
    sqlite3_step (res);
};

void
Statement::bind (const int pos, const std::string &val)
{
    // TODO handle error
    sqlite3_bind_text (res, pos, val.c_str (), -1, SQLITE_STATIC);
}

void
Statement::bind (const int pos, const int val)
{
    // TODO handle error
    sqlite3_bind_int (res, pos, val);
}
