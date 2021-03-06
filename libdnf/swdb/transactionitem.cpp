/*
 * Copyright (C) 2017-2018 Red Hat, Inc.
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

#include "transactionitem.hpp"

// TODO: translations
static const std::map< TransactionItemAction, std::string > transactionItemActionName = {
    {TransactionItemAction::INSTALL, "Install"},
    {TransactionItemAction::DOWNGRADE, "Downgrade"},
    {TransactionItemAction::DOWNGRADED, "Downgraded"},
    {TransactionItemAction::OBSOLETE, "Obsolete"},
    {TransactionItemAction::OBSOLETED, "Obsoleted"},
    {TransactionItemAction::UPGRADE, "Upgrade"},
    {TransactionItemAction::UPGRADED, "Upgraded"},
    {TransactionItemAction::REMOVE, "Removed"},
    {TransactionItemAction::REINSTALL, "Reinstall"},
    {TransactionItemAction::REINSTALLED, "Reinstalled"},
    {TransactionItemAction::REASON_CHANGE, "Reason Change"},
};

static const std::map< TransactionItemAction, std::string > transactionItemActionShort = {
    {TransactionItemAction::INSTALL, "I"},
    {TransactionItemAction::DOWNGRADE, "D"},
    {TransactionItemAction::DOWNGRADED, "D"},
    {TransactionItemAction::OBSOLETE, "O"},
    {TransactionItemAction::OBSOLETED, "O"},
    {TransactionItemAction::UPGRADE, "U"},
    {TransactionItemAction::UPGRADED, "U"},
    // "R" is for Reinstall, therefore use "E" for rEmove (or Erase)
    {TransactionItemAction::REMOVE, "E"},
    {TransactionItemAction::REINSTALL, "R"},
    {TransactionItemAction::REINSTALLED, "R"},
    // TODO: replace "?" with something better
    {TransactionItemAction::REASON_CHANGE, "?"},
};

/*
static const std::map<std::string, TransactionItemReason> nameTransactionItemReason = {
    {, "I"},
    {TransactionItemAction::DOWNGRADE, "D"},
    {TransactionItemAction::DOWNGRADED, "D"},
    {TransactionItemAction::OBSOLETE, "O"},
    {TransactionItemAction::OBSOLETED, "O"},
    {TransactionItemAction::UPGRADE, "U"},
    {TransactionItemAction::UPGRADED, "U"},
    // "R" is for Reinstall, therefore use "E" for rEmove (or Erase)
    {TransactionItemAction::REMOVE, "E"},
    {TransactionItemAction::REINSTALL, "R"},
};
TransactionItemReason to_TransactionItemReason(const std::string & s) {
    return
*/

const std::string &
TransactionItemBase::getActionName()
{
    return transactionItemActionName.at(getAction());
}

const std::string &
TransactionItemBase::getActionShort()
{
    return transactionItemActionShort.at(getAction());
}

TransactionItem::TransactionItem(libdnf::Transaction *trans)
  : trans(trans)
  , transID(0)
  , conn(trans->conn)
{
}

TransactionItem::TransactionItem(SQLite3Ptr conn, int64_t transID)
  : trans(nullptr)
  , transID(transID)
  , conn(conn)
{
}

void
TransactionItem::save()
{
    getItem()->save();
    if (getId() == 0) {
        dbInsert();
    } else {
        dbUpdate();
    }
}

void
TransactionItem::dbInsert()
{
    if (trans == nullptr) {
        throw std::runtime_error("Attempt to insert transaction item into completed transaction");
    }

    const char *sql = R"**(
        INSERT INTO
          trans_item (
            id,
            trans_id,
            item_id,
            repo_id,
            action,
            reason,
            done
          )
        VALUES
          (null, ?, ?, ?, ?, ?, ?)
    )**";

    // save the transaction item
    SQLite3::Statement query(*(conn.get()), sql);
    query.bindv(trans->getId(),
                getItem()->getId(),
                SwdbPrivate::Repo::getCached(conn, getRepoid())->getId(),
                static_cast< int >(getAction()),
                static_cast< int >(getReason()),
                getDone());
    query.step();
    setId(conn->lastInsertRowID());
}

void
TransactionItem::saveReplacedBy()
{
    if (replacedBy.empty()) {
        return;
    }
    const char *sql = "INSERT INTO item_replaced_by VALUES (?, ?)";
    SQLite3::Statement replacedByQuery(*(conn.get()), sql);
    bool first = true;
    for (const auto &newItem : replacedBy) {
        if (!first) {
            // reset the prepared statement, so it can be executed again
            replacedByQuery.reset();
        }
        replacedByQuery.bindv(getId(), newItem->getId());
        replacedByQuery.step();
        first = false;
    }
}

void
TransactionItem::dbUpdate()
{
    if (trans == nullptr) {
        throw std::runtime_error("Attempt to update transaction item in completed transaction");
    }

    const char *sql = R"**(
        UPDATE
          trans_item
        SET
          trans_id=?,
          item_id=?,
          repo_id=?,
          action=?,
          reason=?,
          done=?
        WHERE
          id = ?
    )**";

    SQLite3::Statement query(*(conn.get()), sql);
    query.bindv(trans->getId(),
                getItem()->getId(),
                SwdbPrivate::Repo::getCached(trans->conn, getRepoid())->getId(),
                static_cast< int >(getAction()),
                static_cast< int >(getReason()),
                getDone(),
                getId());
    query.step();
}
