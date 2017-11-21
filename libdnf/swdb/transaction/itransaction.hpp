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

#ifndef LIBDNF_ITRANSACTION_HPP
#define LIBDNF_ITRANSACTION_HPP

#include <string>
#include <vector>

#include "../item/transactionitem.hpp"

class ITransaction
{
    // TODO create contructors AI:ecuba
    virtual ~ITransaction() = default;

    virtual std::vector<TransactionItem *> listTransactionItems() const = 0;
    virtual std::vector<std::string> listLogMessages(int fileDescriptor) const = 0;
    virtual std::string getSoftwarePerformedWith() const = 0;

    virtual void addTransactionItem(TransactionItem *transactionItem) = 0;
    virtual void setSoftwarePerformedWith(std::string &software) = 0;

    virtual void begin(long long rpmDBVersion) = 0;
    virtual void logOutput(std::string *message, int fileDescriptor) = 0;
    virtual void end(long long rpmDBVersion) = 0;

};


#endif //LIBDNF_ITRANSACTION_HPP
