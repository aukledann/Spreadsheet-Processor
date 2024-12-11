#pragma once
#ifndef __PROGTEST__
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
#include "CSpreadSheet.h"
#include "CPos.h"
#include "MyParser.h"

using namespace std;
using namespace std::literals;
using CValue = std::variant<std::monostate, double, std::string>;

constexpr unsigned                     SPREADSHEET_CYCLIC_DEPS                 = 0x01;
constexpr unsigned                     SPREADSHEET_FUNCTIONS                   = 0x02;
constexpr unsigned                     SPREADSHEET_FILE_IO                     = 0x04;
constexpr unsigned                     SPREADSHEET_SPEED                       = 0x08;
constexpr unsigned                     SPREADSHEET_PARSER                      = 0x10;
#endif /* __PROGTEST__ */





#ifndef __PROGTEST__

bool                                   valueMatch                              ( const CValue                        & r,
                                                                                 const CValue                        & s )

{
  if ( r . index () != s . index () )
    return false;
  if ( r . index () == 0 )
    return true;
  if ( r . index () == 2 )
    return std::get<std::string> ( r ) == std::get<std::string> ( s );
  if ( std::isnan ( std::get<double> ( r ) ) && std::isnan ( std::get<double> ( s ) ) )
    return true;
  if ( std::isinf ( std::get<double> ( r ) ) && std::isinf ( std::get<double> ( s ) ) )
    return ( std::get<double> ( r ) < 0 && std::get<double> ( s ) < 0 )
           || ( std::get<double> ( r ) > 0 && std::get<double> ( s ) > 0 );
  return fabs ( std::get<double> ( r ) - std::get<double> ( s ) ) <= 1e8 * DBL_EPSILON * fabs ( std::get<double> ( r ) );
}
int main (){
  CSpreadSheet x0, x1;
  std::ostringstream oss;
  std::istringstream iss;
  std::string data;

    //check copy constr,destr,move const- all good
    //assert ( x0 . setCell ( CPos ( "A1" ), "10" ) );
    //CSpreadSheet x2(x0);
    //CSpreadSheet x3 = move(x0);

    /*assert ( x0 . setCell ( CPos ( "A1" ), "10" ) );
    assert ( x0 . setCell ( CPos ( "A2" ), "10" ) );
    assert ( x0 . setCell ( CPos ( "A3" ), "=A1=A2" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A3" ) ), CValue ( 1.0 ) ) );//true
    assert ( x0 . setCell ( CPos ( "A4" ), "=A1<>A2" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A4" ) ), CValue ( 0.0 ) ) );//true */



    /*assert ( x0 . setCell ( CPos ( "A1" ), "10" ) );
    assert ( x0 . setCell ( CPos ( "A6" ), "abcdefg" ) );
    assert(x0.setCell(CPos("A2"),"=A1+A6"));//restrict operations with strings

    assert ( valueMatch ( x0 . getValue ( CPos ( "A2" ) ), CValue() ) );*/


    /*assert ( x0 . setCell ( CPos ( "A1" ), "10" ) );
    assert ( x0 . setCell ( CPos ( "A2" ), "20" ) );
    assert ( x0 . setCell ( CPos ( "A3" ), "abcdefg" ) );
    assert ( x0 . setCell ( CPos ( "A16" ), "=sum(A1:A3)" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A16" ) ), CValue ( 30.0 ) ) );//true

    assert ( x0 . setCell ( CPos ( "A4" ), "=A1*5" ) );
    assert ( x0 . setCell ( CPos ( "A17" ), "=sum(A1:A4)" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A17" ) ), CValue ( 80.0 ) ) );//true
     */


    /*assert ( x0 . setCell ( CPos ( "A1" ), "10" ) );
    assert ( x0 . setCell ( CPos ( "A2" ), "20" ) );
    assert ( x0 . setCell ( CPos ( "A3" ), "30" ) );
    assert ( x0 . setCell ( CPos ( "A16" ), "=sum(A1:A3) + 5" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A16" ) ), CValue(65.0) ) );
    assert ( x0 . setCell ( CPos ( "A20" ), "=max(A1:A3)" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A20" ) ), CValue(30.0) ) );
    assert ( x0 . setCell ( CPos ( "A19" ), "=min(A1:A3)" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A19" ) ), CValue(10.0) ) );
    assert ( x0 . setCell ( CPos ( "A18" ), "=count(A1:A3)" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A18" ) ), CValue(3.0) ) );
    assert ( x0 . setCell ( CPos ( "A12" ), "=countval(20,A1:A3)" ) );
    assert ( valueMatch ( x0 . getValue ( CPos ( "A12" ) ), CValue(1.0) ) );*/


 /*assert ( x0 . setCell ( CPos ( "A1" ), "10" ) );
  assert ( x0 . setCell ( CPos ( "A2" ), "20.5" ) );
  assert ( x0 . setCell ( CPos ( "A3" ), "3e1" ) );
  assert ( x0 . setCell ( CPos ( "A4" ), "=40" ) );
  assert ( x0 . setCell ( CPos ( "A5" ), "=5e+1" ) );
  assert ( x0 . setCell ( CPos ( "A6" ), "raw text with any characters, including a quote \" or a newline\n" ) );
  assert ( x0 . setCell ( CPos ( "A7" ), "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\"" ) );

  assert ( valueMatch ( x0 . getValue ( CPos ( "A1" ) ), CValue ( 10.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A2" ) ), CValue ( 20.5 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A3" ) ), CValue ( 30.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A4" ) ), CValue ( 40.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A5" ) ), CValue ( 50.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A6" ) ), CValue ( "raw text with any characters, including a quote \" or a newline\n" ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A7" ) ), CValue ( "quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++." ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A8" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "AAAA9999" ) ), CValue() ) );

  assert ( x0 . setCell ( CPos ( "B1" ), "=A1+A2*A3" ) );
  assert ( x0 . setCell ( CPos ( "B2" ), "= -A1 ^ 2 - A2 / 2   " ) );
  assert ( x0 . setCell ( CPos ( "B3" ), "= 2 ^ $A$1" ) );
  assert ( x0 . setCell ( CPos ( "B4" ), "=($A1+A$2)^2" ) );
  assert ( x0 . setCell ( CPos ( "B5" ), "=B1+B2+B3+B4" ) );
  assert ( x0 . setCell ( CPos ( "B6" ), "=B1+B2+B3+B4+B5" ) );

  assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 625.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -110.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 1024.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 930.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 2469.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 4938.0 ) ) );

  assert ( x0 . setCell ( CPos ( "A1" ), "12" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 627.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -154.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 1056.25 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 5625.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 11250.0 ) ) );
  x1 = x0;

  assert ( x0 . setCell ( CPos ( "A2" ), "100" ) );
  assert ( x1 . setCell ( CPos ( "A2" ), "=A3+A5+A4" ) );

  assert ( valueMatch ( x0 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );

  assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3612.0 ) ) );//A1 = 12, A2 = A3+A5+A4(120), A3 = 30
  assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -204.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 17424.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 24928.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 49856.0 ) ) );

  oss . clear ();
  oss . str ( "" );
  assert ( x0 . save ( oss ) );
  data = oss . str ();
  iss . clear ();
  iss . str ( data );
  assert ( x1 . load ( iss ) );

  assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) ); //data of x0, now must be in x1
  assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );
  assert ( x0 . setCell ( CPos ( "A3" ), "4e1" ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B1" ) ), CValue ( 3012.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B2" ) ), CValue ( -194.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B3" ) ), CValue ( 4096.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B4" ) ), CValue ( 12544.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B5" ) ), CValue ( 19458.0 ) ) );
  assert ( valueMatch ( x1 . getValue ( CPos ( "B6" ) ), CValue ( 38916.0 ) ) );


  oss . clear ();
  oss . str ( "" );
  assert ( x0 . save ( oss ) );
  data = oss . str ();
  for ( size_t i = 0; i < std::min<size_t> ( data . length (), 10 ); i ++ )
    data[i] ^=0x5a;
  iss . clear ();
  iss . str ( data );


  assert ( ! x1 . load ( iss ) );
  assert ( x0 . setCell ( CPos ( "D0" ), "10" ) );
  assert ( x0 . setCell ( CPos ( "D1" ), "20" ) );
  assert ( x0 . setCell ( CPos ( "D2" ), "30" ) );
  assert ( x0 . setCell ( CPos ( "D3" ), "40" ) );
  assert ( x0 . setCell ( CPos ( "D4" ), "50" ) );
  assert ( x0 . setCell ( CPos ( "E0" ), "60" ) );
  assert ( x0 . setCell ( CPos ( "E1" ), "70" ) );
  assert ( x0 . setCell ( CPos ( "E2" ), "80" ) );
  assert ( x0 . setCell ( CPos ( "E3" ), "90" ) );
  assert ( x0 . setCell ( CPos ( "E4" ), "100" ) );
  assert ( x0 . setCell ( CPos ( "F10" ), "=D0+5" ) );
  assert ( x0 . setCell ( CPos ( "F11" ), "=$D0+5" ) );
  assert ( x0 . setCell ( CPos ( "F12" ), "=D$0+5" ) );
  assert ( x0 . setCell ( CPos ( "F13" ), "=$D$0+5" ) );

  x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 1, 4 );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );
    //G11 - F10 = 1;1. D+1, O+1 = E1
  assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );// E1 + 5 (diagonal to the down-left is E1.)
  assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) ); //$D1 + 5 (since F11 is $DO+5), we copy -1 down
  assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );// E$0 + 5(since F12 is D$O+5), we copy +1 right
  assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) ); // D0 + 5


  x0 . copyRect ( CPos ( "G11" ), CPos ( "F10" ), 2, 4 );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F10" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F11" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F12" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F13" ) ), CValue ( 15.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "F14" ) ), CValue() ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "G10" ) ), CValue() ) );


  assert ( valueMatch ( x0 . getValue ( CPos ( "G11" ) ), CValue ( 75.0 ) ) );//now = E1+5
  assert ( valueMatch ( x0 . getValue ( CPos ( "G12" ) ), CValue ( 25.0 ) ) );// now = $D1+5
  assert ( valueMatch ( x0 . getValue ( CPos ( "G13" ) ), CValue ( 65.0 ) ) );//now = E$0 + 5
  assert ( valueMatch ( x0 . getValue ( CPos ( "G14" ) ), CValue ( 15.0 ) ) );//now $D$0 + 1
  assert ( valueMatch ( x0 . getValue ( CPos ( "H10" ) ), CValue() ) );//not include int rect

  assert ( valueMatch ( x0 . getValue ( CPos ( "H11" ) ), CValue() ) ); //must be val of G10 is empty
  assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue() ) ); // must be val of F2 =" " (2 steps to diag)    (saves !G2+5 in I12)
  assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( 35.0 ) ) );//must be val of D2(30) + 5(2 steps to down)
  assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue() ) );// must be val of F$0 + 5 = " "(2 steps to right)

  assert ( x0 . setCell ( CPos ( "F0" ), "-27" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );


  x0 . copyRect ( CPos ( "H12" ), CPos ( "H13" ), 1, 2 );
  assert ( valueMatch ( x0 . getValue ( CPos ( "H12" ) ), CValue ( 25.0 ) ) );//now $D1+5(20+5) since we go higher up to 1 row
  assert ( valueMatch ( x0 . getValue ( CPos ( "H13" ) ), CValue ( -22.0 ) ) );//must be F$0+5 too? I think because column is the same
  assert ( valueMatch ( x0 . getValue ( CPos ( "H14" ) ), CValue ( -22.0 ) ) );


  x0 . copyRect ( CPos ( "A4" ), CPos ( "A6" ), 1, 1 );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A6" ) ), CValue ( "raw text with any characters, including a quote \" or a newline\n" ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A4" ) ), CValue ( "raw text with any characters, including a quote \" or a newline\n" ) ) );


  assert ( x0 . setCell ( CPos ( "A1" ), "100" ) );
  assert ( x0 . setCell ( CPos ( "A2" ), "=$A$1+50" ) );
  x0 . copyRect ( CPos ( "A3" ), CPos ( "A2" ), 1, 2 );

  assert ( valueMatch ( x0 . getValue ( CPos ( "A1" ) ), CValue ( 100.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A2" ) ), CValue ( 150.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A3" ) ), CValue ( 150.0 ) ) );

  assert ( x0 . setCell ( CPos ( "A1" ), "150" ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A1" ) ), CValue ( 150.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A2" ) ), CValue ( 200.0 ) ) );
  assert ( valueMatch ( x0 . getValue ( CPos ( "A3" ) ), CValue ( 200.0 ) ) );*/




    assert(x0.setCell(CPos("G10"), "10"));
    assert(x0.setCell(CPos("G11"), "20"));
    assert(x0.setCell(CPos("G12"), "30"));
    assert(x0.setCell(CPos("H10"), "=G10+5"));
    assert(x0.setCell(CPos("H11"), "=G$11"));
    assert(x0.setCell(CPos("H12"), "=$G12+$G$11"));
    assert(valueMatch(x0.getValue(CPos("h10")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("h11")), CValue(20.0)));
    assert(valueMatch(x0.getValue(CPos("h12")), CValue(50.0)));
    x0.copyRect(CPos("G9"), CPos("G10"), 2, 2);
    assert(valueMatch(x0.getValue(CPos("g9")), CValue(10.0)));
    assert(valueMatch(x0.getValue(CPos("g10")), CValue(20.0)));
    assert(valueMatch(x0.getValue(CPos("g11")), CValue(20.0)));
    assert(valueMatch(x0.getValue(CPos("h9")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("h10")), CValue(20.0)));
    assert(valueMatch(x0.getValue(CPos("h11")), CValue(20.0)));
    x0.copyRect(CPos("H9"), CPos("G9"), 2, 2);
    assert(valueMatch(x0.getValue(CPos("h9")), CValue(10.0)));
    assert(valueMatch(x0.getValue(CPos("h10")), CValue(20.0)));
    assert(valueMatch(x0.getValue(CPos("i9")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("i10")), CValue(20.0)));
    x0.copyRect(CPos("F9"), CPos("H9"), 2, 2);
    assert(valueMatch(x0.getValue(CPos("f9")), CValue(10.0)));
    assert(valueMatch(x0.getValue(CPos("f10")), CValue(20.0)));
    assert(valueMatch(x0.getValue(CPos("g9")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("g10")), CValue()));
    x0.copyRect(CPos("G10"), CPos("G9"), 2, 2);
    assert(valueMatch(x0.getValue(CPos("g10")), CValue(25.0)));
    assert(valueMatch(x0.getValue(CPos("g11")), CValue()));
    assert(valueMatch(x0.getValue(CPos("h9")), CValue(10.0)));
    assert(valueMatch(x0.getValue(CPos("h11")), CValue(20.0)));


    x0.copyRect(CPos("H8"), CPos("G9"), 3, 3);

    assert(valueMatch(x0.getValue(CPos("F8")), CValue()));//empty, not included
    assert(valueMatch(x0.getValue(CPos("G8")), CValue()));//empty, not included
    assert(valueMatch(x0.getValue(CPos("H8")), CValue()));//G8+5 (from G9(F9+5))
    assert(valueMatch(x0.getValue(CPos("I8")), CValue(10.0)));//from H9
    assert(valueMatch(x0.getValue(CPos("J8")), CValue(15.0)));//I8+5(from I9(H9+5)

    assert(valueMatch(x0.getValue(CPos("F9")), CValue(10.0)));//unchanged
    assert(valueMatch(x0.getValue(CPos("G9")), CValue(15.0)));//unchanged
    assert(valueMatch(x0.getValue(CPos("H9")), CValue(20.0)));//G9+5
    assert(valueMatch(x0.getValue(CPos("I9")), CValue(10.0)));//from H10
    assert(valueMatch(x0.getValue(CPos("J9")), CValue()));//I$11

    assert(valueMatch(x0.getValue(CPos("F10")), CValue(20.0)));
    assert(valueMatch(x0.getValue(CPos("G10")), CValue(25.0)));
    assert(valueMatch(x0.getValue(CPos("H10")), CValue()));//G$11(G11 had F$11) correct is  empty, my returns 0.0
    assert(valueMatch(x0.getValue(CPos("I10")), CValue(20.0)));
    assert(valueMatch(x0.getValue(CPos("J10")), CValue()));

    assert(valueMatch(x0.getValue(CPos("F11")), CValue()));//all unchanged
    assert(valueMatch(x0.getValue(CPos("G11")), CValue()));
    assert(valueMatch(x0.getValue(CPos("H11")), CValue(20.0)));
    assert(valueMatch(x0.getValue(CPos("I11")), CValue()));
    assert(valueMatch(x0.getValue(CPos("J11")), CValue()));

    assert(valueMatch(x0.getValue(CPos("F12")), CValue()));
    assert(valueMatch(x0.getValue(CPos("G12")), CValue(30.0)));
    assert(valueMatch(x0.getValue(CPos("H12")), CValue()));
    assert(valueMatch(x0.getValue(CPos("I12")), CValue()));
    assert(valueMatch(x0.getValue(CPos("J12")), CValue()));



    x0.copyRect(CPos("J8"), CPos("G10"), 2, 2);
    assert(valueMatch(x0.getValue(CPos("J8")), CValue(15.0)));//I8+5
    assert(valueMatch(x0.getValue(CPos("K8")), CValue()));//J$11
    assert(valueMatch(x0.getValue(CPos("J9")), CValue()));//I$11
    assert(valueMatch(x0.getValue(CPos("K9")), CValue(20.0)));//from H11

    x0.copyRect(CPos("I11"), CPos("J8"), 1, 1);
    assert(valueMatch(x0.getValue(CPos("I11")), CValue(25.0)));//H11+5
    assert(valueMatch(x0.getValue(CPos("J9")), CValue(25.0)));//I$11
    assert(valueMatch(x0.getValue(CPos("H11")), CValue(20.0)));



    x0.copyRect(CPos("F9"), CPos("H8"), 4, 4);

    assert(valueMatch(x0.getValue(CPos("F8")), CValue()));//not includeed
    assert(valueMatch(x0.getValue(CPos("F9")), CValue()));//E9+5
    assert(valueMatch(x0.getValue(CPos("F10")), CValue()));//E10+5
    assert(valueMatch(x0.getValue(CPos("F11")), CValue()));//E$11
    assert(valueMatch(x0.getValue(CPos("F12")), CValue(20.0)));//from H11

    assert(valueMatch(x0.getValue(CPos("G8")), CValue()));//not included
    assert(valueMatch(x0.getValue(CPos("G9")), CValue(10.0)));//from
    assert(valueMatch(x0.getValue(CPos("G10")), CValue(10.0)));//from I9
    assert(valueMatch(x0.getValue(CPos("G11")), CValue(20.0)));//from I10
    assert(valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));//F12+5

    assert(valueMatch(x0.getValue(CPos("H8")), CValue()));//not changed
    assert(valueMatch(x0.getValue(CPos("H9")), CValue(15.0)));//G9+5
    assert(valueMatch(x0.getValue(CPos("H10")), CValue(20.0)));//G$11
    assert(valueMatch(x0.getValue(CPos("H11")), CValue()));//from J11
    assert(valueMatch(x0.getValue(CPos("H12")), CValue()));//from J12

    assert(valueMatch(x0.getValue(CPos("I8")), CValue(10.0)));//not changed
    assert(valueMatch(x0.getValue(CPos("I9")), CValue()));//H$11
    assert(valueMatch(x0.getValue(CPos("I10")), CValue(20.0)));//from  K9
    assert(valueMatch(x0.getValue(CPos("I11")), CValue()));//from K10
    assert(valueMatch(x0.getValue(CPos("I12")), CValue()));//from K12

    assert(valueMatch(x0.getValue(CPos("j8")), CValue(15.0)));
    assert(valueMatch(x0.getValue(CPos("j9")), CValue()));
    assert(valueMatch(x0.getValue(CPos("j10")), CValue()));
    assert(valueMatch(x0.getValue(CPos("j11")), CValue()));
    assert(valueMatch(x0.getValue(CPos("j12")), CValue()));

    assert(valueMatch(x0.getValue(CPos("k8")), CValue()));
    assert(valueMatch(x0.getValue(CPos("k9")), CValue(20.0)));
    assert(valueMatch(x0.getValue(CPos("k10")), CValue()));
    assert(valueMatch(x0.getValue(CPos("k11")), CValue()));
    assert(valueMatch(x0.getValue(CPos("k12")), CValue()));


    assert(x0.setCell(CPos("Z50"), "-5"));
    assert(valueMatch(x0.getValue(CPos("Z50")), CValue(-5.0)));
    assert(x0.setCell(CPos("Z800"), "=-(10+5)"));
    assert(valueMatch(x0.getValue(CPos("Z800")), CValue(-15.0)));

    assert(x0.setCell(CPos("Z100"), "10"));
    assert(x0.setCell(CPos("Z200"), "0"));
    assert(x0.setCell(CPos("Z300"), "=Z100/Z200"));
    assert(valueMatch(x0.getValue(CPos("Z300")), CValue()));

    assert(x0.setCell(CPos("Z400"), "=-(Z100<>Z200)"));
    assert(valueMatch(x0.getValue(CPos("Z400")), CValue(-1.0)));




  return EXIT_SUCCESS;

}
#endif /* __PROGTEST__ */



//CopyReact: now only works with only letter + number, improve it(store in cpos its num part and str part, so its easier to compare)






/* string digits = "0123456789";
    auto pos =  dst.getIdent();
    auto found = pos.find_first_of(digits);
   // cout << "First number at position " << found << endl;
    string str_part = pos.substr(0,found-0);
    cout << "String part: " << str_part << endl;

    string sec_part = pos.substr(found, pos.length());
    int num_part = stoi(sec_part);
    cout << "Num part: " << num_part << endl;*/