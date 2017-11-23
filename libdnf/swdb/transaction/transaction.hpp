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

#ifndef LIBDNF_TRANSACTION_HPP
#define LIBDNF_TRANSACTION_HPP

#include <string>

#include "../item/transactionitem.hpp"
#include "readonlytransaction.hpp"

class Transaction : public ReadOnlyTransaction
{
    Transaction(long long id, long long uid, std::string &cliCommand, std::string &releasever);
    Transaction(long long id, long long uid, std::string &cliCommand, std::string &releasever,
                long timeOfTransactionBegin, long timeOfTransactionEnd, long databaseVersionBegin,
                long databaseVersionEnd, std::string &softwarePerformedWith);

    void addTransactionItem(TransactionItem *transactionItem) override;
    void setSoftwarePerformedWith(std::string &software) override;

    void begin(long long rpmDBVersion = -1) override;
    void logOutput(std::string *message, int fileDescriptor) override;
    void end(long long rpmDBVersion) override;
};


#endif //LIBDNF_TRANSACTION_HPP
