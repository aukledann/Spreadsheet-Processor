//
// Created by annab on 16.10.2024.
//

#include "CSpreadSheet.h"
#include "CPos.h"


using namespace std;


CPos::CPos( std::string_view  str ): m_identifier(str) {

}
bool CPos::operator<(const CPos& other) const {
    return m_identifier < other.m_identifier;  // Compare based on m_identifier
}

std::string CPos::getIdent()const{
    return m_identifier;
}

ostream &operator<<(ostream &out, const CPos& pos){
    out << pos.m_identifier;
    return out;
}

 istream &operator>>(istream &in,  CPos& pos){
    in >> pos.m_identifier;
    return in;
}


