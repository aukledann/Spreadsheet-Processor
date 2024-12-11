//
// Created by annab on 16.10.2024.
//
#pragma once
#ifndef BIG_PROJECT_EXCEL_MYPARSER_H
#define BIG_PROJECT_EXCEL_MYPARSER_H
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>


class CExprBuilder
{
public:
    void opAdd() {

    }
    /*
     virtual void opEq() = 0;
     virtual void opNe() = 0;
     virtual void opLt() = 0;
     virtual void opLe() = 0;
     virtual void opGt() = 0;
     virtual void opGe() = 0;
     virtual void valNumber( double val ) = 0;
     virtual void valString( std::string val ) = 0;
     virtual void valReference ( std::string val ) = 0;
     virtual void valRange( std::string val ) = 0;
     virtual void funcCall( std::string fnName, int paramCount ) = 0;*/
};


class MyParser: CExprBuilder {
    CSpreadSheet& m_sheet;

public:

    //MyParser();
    MyParser(CSpreadSheet& sheet);


    double isNum(std::string str);

    bool isStr(std::string  str);

    bool is_op(char ch);
    bool  is_logic_op(char ch);
    bool Apply_logic_op(CValue a, CValue b, char c);

    void find_and_remove_spaces(string &str, const string &search_term);
    std::string  add_delim(std::string  &str);

    int priority(char &ch);

    double applyOp(double a, double b, char op);

    double parse_exp(std::string  &exp,set<string> &visited);
};


#endif //BIG_PROJECT_EXCEL_MYPARSER_H
