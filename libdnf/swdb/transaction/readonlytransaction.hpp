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

#ifndef LIBDNF_READONLYTRANSACTION_HPP
#define LIBDNF_READONLYTRANSACTION_HPP


#include "itransaction.hpp"

class ReadOnlyTransaction : public ITransaction
{
    ReadOnlyTransaction() = default;
    virtual ~ReadOnlyTransaction() = default;

    virtual std::vector<TransactionItem *> listTransactionItems() const override;
    virtual std::vector<std::string> listLogMessages(int fileDescriptor = -1) const override;
    virtual std::string getSoftwarePerformedWith() const override;

    virtual void addTransactionItem(TransactionItem *transactionItem) {};
    virtual void setSoftwarePerformedWith(std::string &software) {};

    virtual void begin(long long rpmDBVersion = -1) {};
    virtual void logOutput(std::string *message, int fileDescriptor) {};
    virtual void end(long long rpmDBVersion) {};
};


#endif //LIBDNF_READONLYTRANSACTION_HPP
