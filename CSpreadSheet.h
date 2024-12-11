//
// Created by annab on 16.10.2024.
//
#pragma once#pragma once
#ifndef BIG_PROJECT_EXCEL_CSPREADSHEET_H
#define BIG_PROJECT_EXCEL_CSPREADSHEET_H

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
#include "CPos.h"
//#include "MyParser.h"

using namespace std;
using CValue = std::variant<std::monostate, double, std::string>;


class CSpreadSheet {

    map<CPos, string> m_cells;//position and its content

    vector<vector<string>> matrix;// cells stored in table order

    map<CPos, vector<int>> cell_index;//position ant its row-column coordinates






public:
    CSpreadSheet();

    //copy constr
    CSpreadSheet(const CSpreadSheet &other);

    //move constr

    CSpreadSheet(CSpreadSheet &&other)noexcept;

    // = operator overload
    CSpreadSheet& operator=(const CSpreadSheet& sheet);
    ~CSpreadSheet();


    bool load ( std::istream & is );
    bool save ( std::ostream& os ) const;

    bool setCell(CPos pos,std::string contents );

    std::string check_quotes_slashes(std::string str);
    string remove_symbols(string str);

    CValue getValue (CPos  pos);

    vector<string> range_helper(string str);
    double sum_range(string str);
    double max_range(string str);
    double min_range(string str);
    double count_range(string str);
    double countval_range(string str);

    vector<int> helper_double_search(string fin_str);
    vector<int> helper_search_index(CPos pos);
    vector<int> search_table(string fin_str);
    vector<string> tokenize(string &res);//helper func to copy
    int check_alt_helper_none(int a, int b, int dest_start_row, int dest_start_col, int col_count, int row_count,map<vector<int>, string> alt);
    int check_alt_helper_row_col(int a, int b, int dest_start_row, int dest_start_col, int col_count, int row_count,map<vector<int>, string> alt);
    void create_cell_helper(int dest_start_row, int dest_start_col, int row_count, int col_count,string exp_result1);
    void copyRect( CPos dst,CPos src,int w, int h  );

    string Find_position_by_str(string &str);

};


#endif //BIG_PROJECT_EXCEL_CSPREADSHEET_H
