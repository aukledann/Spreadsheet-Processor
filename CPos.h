//
// Created by annab on 16.10.2024.
//
#pragma once

#ifndef BIG_PROJECT_EXCEL_CPOS_H
#define BIG_PROJECT_EXCEL_CPOS_H
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <string>
#include "CSpreadSheet.h"

using namespace std;


class CPos {
private:
    std::string m_identifier;

public:
    CPos(std::string_view str);

    bool operator<(const CPos &other) const;

    string getIdent() const;

    friend ostream &operator<<(ostream &out, const CPos& pos);

    friend istream &operator>>(istream &in,  CPos& pos);

};


#endif //BIG_PROJECT_EXCEL_CPOS_H
