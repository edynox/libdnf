/*
 * Copyright (C) 2017 Red Hat, Inc.
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

#ifndef LIBDNF_SWDB_ITEM_RPM_HPP
#define LIBDNF_SWDB_ITEM_RPM_HPP

#include <memory>
#include <vector>

#include "item.hpp"
#include "transactionitem.hpp"

class RPMItem : public Item {
public:
    RPMItem(SQLite3 & conn);
    RPMItem(SQLite3 & conn, int64_t pk);
    virtual ~RPMItem() = default;

    const std::string & getName() const noexcept { return name; }
    void setName(const std::string & value) { name = value; }

    int32_t getEpoch() const noexcept { return epoch; }
    void setEpoch(int32_t value) { epoch = value; }

    const std::string & getVersion() const noexcept { return version; }
    void setVersion(const std::string & value) { version = value; }

    const std::string & getRelease() const noexcept { return release; }
    void setRelease(const std::string & value) { release = value; }

    const std::string & getArch() const noexcept { return arch; }
    void setArch(const std::string & value) { arch = value; }

    const std::string & getChecksumType() const noexcept { return checksumType; }
    void setChecksumType(const std::string & value) { checksumType = value; }

    const std::string & getChecksumData() const noexcept { return checksumData; }
    void setChecksumData(const std::string & value) { checksumData = value; }

    std::string getNEVRA();
    virtual std::string toStr();
    virtual const std::string & getItemType() const noexcept { return itemType; }
    virtual void save();
    static std::vector<std::shared_ptr<TransactionItem> > getTransactionItems(
        SQLite3 & conn,
        int64_t transaction_id);

protected:
    const std::string itemType = "rpm";
    std::string name;
    int32_t epoch = 0;
    std::string version;
    std::string release;
    std::string arch;
    std::string checksumType;
    std::string checksumData;

private:
    void dbSelect(int64_t transaction_id);
    void dbInsert();
    void dbUpdate();
    void dbSelectOrInsert();
};

#endif // LIBDNF_SWDB_ITEM_RPM_HPP
