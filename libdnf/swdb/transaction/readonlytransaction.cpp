/*
 * Copyright (C) 2017 Red Hat, Inc.
 * Author: Eduard Cuba <ecuba@redhat.com>
 *         Martin Hatina <mhatina@redhat.com>
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

#include "readonlytransaction.hpp"

ReadOnlyTransaction::ReadOnlyTransaction(long long id, long long uid, std::string &cliCommand, std::string &releasever)
    : id(id)
    , uid(uid)
    , cliCommand(cliCommand)
    , releasever(releasever)
    , timeOfTransactionBegin(-1)
    , timeOfTransactionEnd(-1)
    , databaseVersionBegin(-1)
    , databaseVersionEnd(-1)
{}

ReadOnlyTransaction::ReadOnlyTransaction(long long id, long long uid, std::string &cliCommand, std::string &releasever,
                                         long timeOfTransactionBegin, long timeOfTransactionEnd,
                                         long databaseVersionBegin, long databaseVersionEnd,
                                         std::string &softwarePerformedWith)
    : ReadOnlyTransaction(id, uid, cliCommand, releasever)
    , timeOfTransactionBegin(timeOfTransactionBegin)
    , timeOfTransactionEnd(timeOfTransactionEnd)
    , databaseVersionBegin(databaseVersionBegin)
    , databaseVersionEnd(databaseVersionEnd)
    , softwarePerformedWith(softwarePerformedWith)
{}