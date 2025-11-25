#pragma once
class DBManager{
public:
    virtual ~DateBase() = default;
    virtual void insert() = 0;
    virtual void query() = 0;
    virtual void update() = 0;
    virtual void delete_() = 0;
};