/* sqlite3.hpp
 *
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

#ifndef _SQLITE3_HPP
#define _SQLITE3_HPP

#include <sqlite3.h>

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

class SQLite3
{
public:
    class Exception : public std::runtime_error
    {
    public:
        Exception(int code, const std::string & msg) : std::runtime_error(msg), ecode{code} {}
        Exception(int code, const char * msg) : std::runtime_error(msg), ecode{code} {}
        int code() const noexcept { return ecode; }
        const char * codeStr() const noexcept { return sqlite3_errstr(ecode); }

    protected:
        int ecode;
    };

    struct Blob {
        size_t size;
        const void * data;
    };

    class Statement {
    public:
        enum class StepResult {DONE, ROW, BUSY};
        
        Statement(SQLite3 & db, const char * sql)
        : db{db}
        {
            auto result = sqlite3_prepare_v2(db.db, sql, -1, &stmt, nullptr);
            if (result != SQLITE_OK)
                throw Exception(result, "Statement");
        };

        Statement(SQLite3 & db, const std::string & sql)
        : db{db}
        {
            auto result = sqlite3_prepare_v2(db.db, sql.c_str(), sql.length() + 1, &stmt, nullptr);
            if (result != SQLITE_OK)
                throw Exception(result, "Statement");
        };

        void bind(int pos, int val)
        {
            auto result = sqlite3_bind_int(stmt, pos, val);
            if (result != SQLITE_OK)
                throw Exception(result, "Integer bind failed");
        }

        void bind(int pos, std::int64_t val)
        {
            auto result = sqlite3_bind_int64(stmt, pos, val);
            if (result != SQLITE_OK)
                throw Exception(result, "Integer64 bind failed");
        }

        void bind(int pos, double val)
        {
            auto result = sqlite3_bind_double(stmt, pos, val);
            if (result != SQLITE_OK)
                throw Exception(result, "Double bind failed");
        }

        void bind(int pos, bool val)
        {
            auto result = sqlite3_bind_int(stmt, pos, val ? 1 : 0);
            if (result != SQLITE_OK)
                throw Exception(result, "Bool bind failed");
        }

        void bind(int pos, const char * val)
        {
            auto result = sqlite3_bind_text(stmt, pos, val, -1, SQLITE_TRANSIENT);
            if (result != SQLITE_OK)
                throw Exception(result, "Text bind failed");
        }

        void bind(int pos, const std::string & val)
        {
            auto result = sqlite3_bind_text(stmt, pos, val.c_str(), -1, SQLITE_TRANSIENT);
            if (result != SQLITE_OK)
                throw Exception(result, "Text bind failed");
        }

        void bind(int pos, const Blob & val)
        {
            auto result = sqlite3_bind_blob(stmt, pos, val.data, val.size, SQLITE_TRANSIENT);
            if (result != SQLITE_OK)
                throw Exception(result, "Blob bind failed");
        }

        void bind(int pos, const std::vector<unsigned char> & val)
        {
            auto result = sqlite3_bind_blob(stmt, pos, val.data(), val.size(), SQLITE_TRANSIENT);
            if (result != SQLITE_OK)
                throw Exception(result, "Blob bind failed");
        }

        template<typename... Args>
        Statement & bindv(Args && ... args)
        {
            size_t pos{1};
            Pass{(bind(pos++, args), 0)...};
            return *this;
        }

        StepResult step()
        {
            auto result = sqlite3_step(stmt);
            switch (result) {
                case SQLITE_DONE:
                    return StepResult::DONE;
                case SQLITE_ROW:
                    return StepResult::ROW;
                case SQLITE_BUSY:
                    return StepResult::BUSY;
                default:
                    throw Exception(result, "Step failed");
            }
        }

        size_t getColumnCount() const
        {
            return sqlite3_column_count(stmt);
        }

        const char * getColumnDatabaseName(size_t idx) const
        {
            return sqlite3_column_database_name(stmt, idx);
        }

        const char * getColumnTableName(size_t idx) const
        {
            return sqlite3_column_table_name(stmt, idx);
        }

        const char * getColumnOriginName(size_t idx) const
        {
            return sqlite3_column_origin_name(stmt, idx);
        }

        const char * getColumnAsName(size_t idx) const
        {
            return sqlite3_column_name(stmt, idx);
        }

        const char * getSql() const
        {
            return sqlite3_sql(stmt);
        }

        const char * getExpandedSql()
        {
            expandSql = sqlite3_expanded_sql(stmt);
            if (!expandSql) 
                throw Exception(SQLITE_NOMEM, "getExpandedSql(): insufficient memory or result "
                                              "exceed the the maximum SQLite3 string length");
            return expandSql;
        }

        void freeExpandedSql()
        {
            sqlite3_free(expandSql);
        }

        void cacheColumnNames()
        {
            for (size_t idx = 0; idx < getColumnCount(); ++idx)
            {
                getColumnAsName(idx);
            }
        }

        void reset()
        {
            sqlite3_reset(stmt);
        }

        void clearBindings()
        {
            sqlite3_clear_bindings(stmt);
        }
        
        template<typename T>
        T get(size_t idx)
        {
            return get(idx, identity<T>{});
        }

        ~Statement()
        {
            freeExpandedSql();
            sqlite3_finalize(stmt);
        };

    private:
        struct Pass {
            template<typename ...T> Pass(T...) {}
        };

        template<typename T>
        struct identity {
            typedef T type;
        };

        /*template<typename TL>
        TL get(size_t, identity<TL>)
        {
            static_assert(sizeof(TL) == 0, "Not implemented");
        }*/

        int get(size_t idx, identity<int>)
        {
            return sqlite3_column_int(stmt, idx);
        }

        int64_t get(size_t idx, identity<int64_t>)
        {
            return sqlite3_column_int64(stmt, idx);
        }

        double get(size_t idx, identity<double>)
        {
            return sqlite3_column_double(stmt, idx);
        }

        bool get(size_t idx, identity<bool>)
        {
            return sqlite3_column_int(stmt, idx) != 0;
        }

        const char * get(size_t idx, identity<const char *>)
        {
            return reinterpret_cast<const char *>(sqlite3_column_text(stmt, idx));
        }

        std::string get(size_t idx, identity<std::string>)
        {
            auto ret = reinterpret_cast<const char *>(sqlite3_column_text(stmt, idx));
            return ret ? ret : "";
        }

        Blob get(size_t idx, identity<Blob>)
        {
            return {static_cast<size_t>(sqlite3_column_bytes(stmt, idx)), sqlite3_column_blob(stmt, idx)};
        }

        SQLite3 & db;
        sqlite3_stmt * stmt;
        std::map<std::string, size_t> name2idx;
        char * expandSql{nullptr};
    };

    SQLite3(const char *dbPath)
    : path{dbPath}, db{nullptr}
    {
        open();
    }

    ~SQLite3() { close(); }

    const std::string & getPath() const { return path; }

    void exec(const char *sql)
    {
        open();
        auto result = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
        if (result != SQLITE_OK) {
            throw Exception(result, "Exec failed");
        }
    }

    int64_t lastInsertRowID()
    {
        open();
        return sqlite3_last_insert_rowid (db);
    }

  protected:
    void open();
    void close();

    std::string path;

    sqlite3 *db;
};

#endif
