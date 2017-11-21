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

#ifndef LIBDNF_SWDBPRIVATE_HPP
#define LIBDNF_SWDBPRIVATE_HPP

#include "swdb.hpp"
#include "types/reason.hpp"

class HyPackage;

namespace privateAPI {

class SWDB : public publicAPI::SWDB
{
public:
    SWDB(ITransactionFactory *transactionFactory);

    Item *getRpmItem(HyPackage *package) const;
    TransactionItem *createTransactionItem(Item *item, std::string &repoID, Reason reason, bool obsoleting);
    ITransaction *createTransaction(long uid, std::string &cliCommand);
    void add(Item *item);
};
};

#endif //LIBDNF_SWDBPRIVATE_HPP
