#pragma once
#include "DBManager.hpp"
class MySqlDBManager : public DBManager {
public:
    ~MySqlDBManager() override = default;
    void insert() override {
        // MySQL specific insert implementation
    }
    void query() override {
        // MySQL specific query implementation
    }
    void update() override {
        // MySQL specific update implementation
    }
    void delete_() override {
        // MySQL specific delete implementation
    }
};