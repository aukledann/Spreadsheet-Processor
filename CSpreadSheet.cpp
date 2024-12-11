//
// Created by annab on 16.10.2024.
//

#include "CSpreadSheet.h"
#include "CPos.h"
#include "MyParser.h"

using namespace std;
using CValue = std::variant<std::monostate, double, std::string>;

 CSpreadSheet::CSpreadSheet(){
     //cout << "Default constr" << endl;
 };

    //copy constr
CSpreadSheet::CSpreadSheet(const CSpreadSheet &other)
        : m_cells(other.m_cells), cell_index(other.cell_index), matrix(other.matrix){
        //cout << "Copy constr" << endl;
    }

    //move constr
CSpreadSheet::CSpreadSheet(CSpreadSheet &&other) noexcept
        : m_cells(std::move(other.m_cells)),
          cell_index(std::move(other.cell_index)),
          matrix(std::move(other.matrix)) {
        //cout << "Move constr" << endl;
        other.m_cells.clear();
        other.cell_index.clear();
        other.matrix.clear();

    }


    // = operator overload
    CSpreadSheet& CSpreadSheet::operator=(const CSpreadSheet& sheet)  {
        if (this != &sheet) { // Check for self-assignment
            this->m_cells = sheet.m_cells; // Copy cells
            // Copy other members if needed
        }
        return *this; // Return the current object
    }

    // destructor(no need simce vector and map do it automatically
    CSpreadSheet::~CSpreadSheet(){
       // cout << "Destruct" << endl;
    };

    bool CSpreadSheet::load ( std::istream & is ){
        // Clear the current data
        m_cells.clear();
        size_t count;

        // Deserialize the cells map
        is >>count;
        for (size_t i = 0; i < count; ++i) {
            CPos pos("");
            std::string content;

            is >> pos;  // Assuming operator>> is overloaded for CPos
            std::getline(is, content);  // Read the cell content

            m_cells[pos] = content;  // Store the cell
        }

        cell_index.clear();
        size_t count1;
        is >> count1;
        is.ignore(); // To skip the newline after count1

        for (size_t i = 0; i < count1; ++i) {
            CPos pos("");
            std::vector<int> v;

            is >> pos;  // Assuming operator>> is overloaded for CPos
            is.ignore(); // Skip the space or newline after pos

            std::string line;
            std::getline(is, line);  // Read the rest of the line containing integers

            std::istringstream lineStream(line);
            int value;
            while (lineStream >> value) {
                v.push_back(value);
            }
            cell_index[pos] = v;  // Store the vector in cell_index
        }


        return is.good();
    }


    bool CSpreadSheet::save ( std::ostream& os )const  {//store the content of this class in oss
        // Serialize the m_cells map
        os << m_cells.size();
        for (const auto& cell : m_cells) {
            os << cell.first << " " << cell.second << "\n";  // Assuming CPos has operator<< overloaded
            if (os.fail()){
                return false;
            }
        }

        os << cell_index.size();
        for (const auto& cell : cell_index) {
            os << cell.first << " " << cell.second[0] <<" " << cell.second[1] << "\n";  // Assuming CPos has operator<< overloaded
            if (os.fail()){
                return false;
            }
        }

        os << matrix.size();
        for (const auto& row : matrix) {
            for (const auto& col : row) {
                os << col << "\n";  // Assuming CPos has operator<< overloaded
                if (os.fail()) {
                    return false;
                }
            }
        }


        return os.good();

    }

    bool CSpreadSheet::setCell(CPos pos,std::string contents ){
        string sub_give = pos.getIdent().substr(1);

        double num_given = stod(sub_give);
        int num_plus_ten = num_given + 10;
        if(matrix.empty()){//if matrix empty, add 10 rows
            for (int i = 0; i <= num_plus_ten; ++i) {
                std::vector<std::string> row;
                for (char c = 'A'; c <= 'Z'; ++c) {
                    row.push_back(std::string(1, c) + std::to_string(i)); // Create string like "A1", "B1", etc.
                }
                matrix.push_back(row);
            }

        }
        else {//here check num, and add it + 10. A11, will create up to A21
            string sub_give = pos.getIdent().substr(1);
            double num_given = stod(sub_give);
            double max_table = matrix.size();
            int size_to_add = num_given + 10;

            if (num_given >= max_table) {// pos doest exist in the table, add it(add+10 rows)
                for (int i = matrix.size(); i <= size_to_add; ++i) {
                    std::vector<std::string> row;
                    for (char c = 'A'; c <= 'Z'; ++c) {
                        row.push_back(std::string(1, c) + std::to_string(i)); // Create string like "A1", "B1", etc.
                    }
                    matrix.push_back(row);
                }
            }
        }


        MyParser prs(*this);
        if(contents[0] == '=' && !prs.isStr(contents)){
            contents.erase(remove(contents.begin(), contents.end(), ' '), contents.end());
        }
        m_cells[pos] = contents;
        vector<int> rc(2);

        int stop = 0;
        for(int i = 0; i <= matrix.size(); ++i){
            for(int j = 0; j < matrix[i].size(); j++) {
                if(matrix[i][j] == pos.getIdent()){
                    rc[0] = i;
                    rc[1] = j;
                    cell_index[pos] = rc;
                    stop = 1;
                    break;
                }
            }
            if(stop == 1){
                break;
            }
        }

        return true;


    }

    string CSpreadSheet::check_quotes_slashes(string str){

        std::string toRemove = "\"";
        size_t pos = str.find(toRemove);
        while (pos != std::string::npos) {//to find all occurrences
            if(str[pos+1] != '\"'){//if  its actually a double quote, leave it
                str.erase(pos, toRemove.length()); //if single,  remove the found occurrence
                pos = str.find(toRemove, pos); // Find the next occurrence
            }
            else{
                pos++;// if double, move forward
            }

        }

        // Replace occurrences of \"\" with \"
        std::string toReplace = "\\\"\\\"";
        std::string replacement = "\\\"";
        pos = str.find(toReplace);
        while (pos != std::string::npos) {
            str.replace(pos, toReplace.length(), replacement); // Replace with single escaped quote
            pos = str.find(toReplace, pos + replacement.length()); // Find the next occurrence
        }

        return str;

    }

    string CSpreadSheet::remove_symbols(string exp){//from =F$0 + 5 to F0

        exp.erase(remove(exp.begin(), exp.end(),'!'),exp.end());
        exp.erase(remove(exp.begin(), exp.end(),'$'),exp.end());
        if(exp[0] == '='){
            exp = exp.substr(1);
        }


        MyParser prs(*this);

        auto found = exp.find(' ');
        if(found == string::npos){
            prs.add_delim(exp);
        }

        stringstream ss(exp);
        string token;

        while(getline(ss, token,' ')) {
            if (token.empty()) {
                continue;
            }
            else if (isalpha(token[0])) {
                return token;
            }
        }

    }

    CValue CSpreadSheet::getValue (CPos  pos){
        string tolow = pos.getIdent();
        for (int i = 0; i < tolow.length(); ++i) {
           tolow[i] = tolower(tolow[i]);

        }

        string toup = pos.getIdent();
        for (int i = 0; i < toup.length(); ++i) {
            toup[i] = toupper(toup[i]);
        }

        string &cell_cont = m_cells[pos];

        for(auto [pos, val]: m_cells){
            if(pos.getIdent() == tolow){
                cell_cont = m_cells[pos];
                break;
            }
            if(pos.getIdent() == toup){
                cell_cont = m_cells[pos];
                break;
            }
        }

        auto found0 = cell_cont.find('!');
        MyParser prs(*this);set<string>vis;


        if(found0 != string::npos){//if cell has !
            string copy = remove_symbols(cell_cont);//F0 from !=$F0 + 5

            vector<int>v1 =helper_double_search(copy);//if F0 has a value in m_cells
            if(v1[0] == -1 || v1[1] == -1){//not found in m_cells
                return monostate{};
            }
            else{
                cell_cont.erase(remove(cell_cont.begin(), cell_cont.end(),'!'),cell_cont.end());//remove all !
            }

        }

        if(!prs.isStr(cell_cont)){
            auto found = cell_cont.find(' ');
            cell_cont.erase(remove(cell_cont.begin(), cell_cont.end(),' '),cell_cont.end());
        }

        if(cell_cont.empty()){
            return monostate{};
        }
        if(cell_cont[0] == '='){
            MyParser prs(*this);// add Current list of objs of Spreadsheet
            if(prs.isStr(cell_cont) == 1 && !isdigit(cell_cont[1])){// is a string without the first =
                string str_from_sec = cell_cont.substr(1);
                string fin_str = check_quotes_slashes(str_from_sec);
                return fin_str;
            }
            else{
                 if (isdigit(cell_cont[1])) {// it is a num
                    if(prs.isNum(cell_cont) != 0){
                        return prs.isNum(cell_cont);
                    }
                }
                string str_from_sec = cell_cont.substr(1);
                prs.add_delim(str_from_sec);
                set<string>vis;
                double result = prs.parse_exp(str_from_sec,vis);  //parse the expression
                if(result == INFINITY){//division by 0
                    CValue result1 = monostate{};
                    return result1;
                }
                return result;
            }
        }
        else if(isdigit(cell_cont[0] )|| (isdigit(cell_cont[1]) && cell_cont[0] == '-')){
            return stod(cell_cont);
        }
        else {
            return cell_cont;

        }

    }

    vector <int> CSpreadSheet:: helper_double_search(string fin_str){
        vector<int>v {-1,-1};

        int found_cell = 0;
        bool is_first_value = true;
        for(const auto& [pos, val] : m_cells){
            if(fin_str == pos.getIdent()){
                for (int val : cell_index[pos]) {
                    if(is_first_value){
                        v[0] = val;
                        is_first_value = false;
                        found_cell =1;
                    }
                    else{
                        v[1] = val;
                        break;
                    }
                }
            }
        }


        return v;
    }
    vector<int> CSpreadSheet::helper_search_index(CPos pos){
        bool is_first_value = true;
        int start_row =0, start_col= 0;
        vector<int>v1(2);

        for (int val : cell_index[pos]) {
            if(is_first_value){
                start_row = val;
                is_first_value = false;
            }
            else{
                start_col = val;
            }
        }

        v1[0] = start_row;
        v1[1] = start_col;
        return v1;
    }

    vector<string> CSpreadSheet::tokenize(string &res){
        vector<string>v1(2);
        MyParser prs(*this);
        set<string>vis;

        if(res[0] == '='){
            res = res.substr(1);
        }
        if(res[0] == '!'){
            res = res.substr(1);
        }
        res.erase(remove(res.begin(), res.end(), '$'), res.end());//read D0 from D$0+5
        prs.add_delim(res);
        stringstream ss(res);
        string token;
        string to_save;
        string fin_str;
        while(getline(ss, token,' ')){
            if (token.empty() || isdigit(token[0]) || !isalpha(token[0])) {
                to_save = to_save + token;
                continue;
            }
            else{
                fin_str = token;
            }
        }

        v1[0] = to_save;
        v1[1] = fin_str;

        return v1;
    }


vector<int> CSpreadSheet::search_table(string fin_str){
    int del_after = 0;
    vector<int> v(2);

    for(int m = 0; m < matrix.size(); m++) {
        for (int l = 0; l < matrix[m].size(); l++) {
            if (matrix[m][l] == fin_str) {
                v[0] = m;
                v[1] = l;
                del_after = 1;
                break;
            }
        }
        if (del_after == 1) {
            break;
        }

    }

    return v;
}


    int CSpreadSheet::check_alt_helper_none(int i, int j,int dest_start_row, int dest_start_col, int col_count, int row_count,map<vector<int>, string> alt){
        MyParser prs(*this);
        string exp_result1;
        vector<string>v2(2);
        int fin_col = 0, fin_row = 0;
        int stop = 0;
        for(auto[str, val] : alt){
            if(str[0] == i && str[1] == j){
                string s = Find_position_by_str(matrix[i][j]);//content of G11
                auto fnd = val.find('$');
                if((s != val) && fnd == string::npos){    //content of H9 before != content of H9 now
                    if(prs.isNum(val)){
                        string ident_dest =  matrix[dest_start_row+row_count][dest_start_col+col_count] ;
                        CPos pos(ident_dest);
                        setCell(pos, val);
                        stop = 1;
                        continue;
                    }//no $
                    vector<string> vect1 = tokenize(val);
                    string save = vect1[0];
                    int diff_rows = 0,  diff_cols  = 0;

                    diff_rows = (dest_start_row+row_count) - i;
                    diff_cols = (dest_start_col+col_count) - j;

                    CPos p(matrix[i][j]);
                    auto k = cell_index[p];//coordinates of G11

                    int final_pos_rw = k[0] + diff_rows;//H12 coordinates
                    int final_pos_cl = k[1] + diff_cols;

                    string removed_pos = remove_symbols(val);//E1, G9
                    CPos pos1(removed_pos);
                    vector<int> coord =  cell_index[pos1];
                    int idk = coord[0] + diff_rows;
                    int idk1 = coord[1] + diff_cols;// coordinates of F2

                    CPos pos2(matrix[idk][idk1]);//F2
                    exp_result1 = "=!" +matrix[idk][idk1] + save;

                    string ident_dest =  matrix[dest_start_row+row_count][dest_start_col+col_count] ;
                    CPos pos(ident_dest);
                    setCell(pos, exp_result1);
                    stop = 1;
                    continue;
                }
                else if((s != val) && fnd != string::npos){
                    if((val[1] =='!'&& fnd == 2) || (val[1] !='!' && fnd==1)){//$ in row
                        int fnd2;
                        if(val[1] == '!'){
                            fnd2 = val.find('$',3);
                        }
                        else{
                            fnd2 = val.find('$',2);
                        }
                        if(fnd2 != string::npos){//two$
                            string ident_dest = matrix[dest_start_row+row_count][dest_start_col+col_count];
                            CPos pos(ident_dest);
                            setCell(pos, val);
                            stop = 1;
                            continue;
                        }
                        else{//Repeated in row $
                            if(row_count + dest_start_row == i){
                                string cont = val;
                                CPos new_p(matrix[dest_start_row+row_count][dest_start_col+col_count]);//dest mat
                                setCell(new_p,cont);
                            }
                            else{
                                v2 = tokenize(val);
                                string to_save = v2[0];
                                string fin_str = v2[1];
                                string removed_val = remove_symbols(val);
                                vector<int>vv=  helper_double_search(removed_val);
                                fin_col = vv[1];
                                if(row_count + dest_start_row < i){//row is smaller
                                    int dif_row =  i - row_count + dest_start_row;
                                    fin_row =  vv[0] - dif_row;
                                }

                                else{//row is bigger
                                    int dif_row = row_count + dest_start_row - i;
                                    fin_row =  vv[0] + dif_row;
                                }
                                string add_doll = matrix[fin_row][fin_col];
                                add_doll.insert(add_doll.begin()+1,'$');
                                exp_result1 = "=!" + add_doll + to_save;
                                CPos new_p( matrix[dest_start_row+row_count][dest_start_col+col_count]);
                                setCell(new_p,exp_result1);
                                stop = 1;
                            }

                            stop = 1;
                            continue;
                        }
                    }
                    else{//Repetition in column
                        //

                        if(col_count + dest_start_col == j){//col of this and col of dest is the same
                            string cont = val;
                            CPos new_p(matrix[dest_start_row+row_count][dest_start_col+col_count]);//dest mat
                            setCell(new_p,cont);
                        }
                        else{
                            v2 = tokenize(val);
                            string to_save = v2[0];
                            string fin_str = v2[1];
                            string removed_val = remove_symbols(val);
                            vector<int>vv=  helper_double_search(removed_val);
                            fin_row = vv[0];
                            if(col_count + dest_start_col < j){//col is smaller
                                int dif_col =  j - col_count + dest_start_col;
                                fin_col =  vv[1] - dif_col;
                            }

                            else{//col is bigger
                                int dif_col = col_count + dest_start_col - j;
                                fin_col =  vv[1] + dif_col;
                            }
                            string add_doll = matrix[fin_row][fin_col];
                            add_doll.insert(add_doll.begin()+1,'$');
                            exp_result1 = "=!" + add_doll + to_save;
                            CPos new_p( matrix[dest_start_row+row_count][dest_start_col+col_count]);
                            setCell(new_p,exp_result1);
                            stop = 1;
                        }

                        stop = 1;
                        continue;
                    }
                }
            }
        }

        return stop;
    }


    int CSpreadSheet::check_alt_helper_row_col(int i, int j, int dest_start_row, int dest_start_col, int col_count, int row_count,map<vector<int>, string> alt){
        MyParser prs(*this);
        int stop = 0;
        for (auto [str, val]: alt) {
            if (str[0] == i && str[1] == j) {
                string s = Find_position_by_str(matrix[i][j]);//content of G11
                auto fnd = val.find('$');
                if ((s != val) &&
                    fnd == string::npos) {    //content of H9 before != content of H9 now
                    if (prs.isNum(val)) {
                        string ident_dest = matrix[dest_start_row + row_count][dest_start_col +col_count];
                        CPos pos(ident_dest);
                        setCell(pos, val);
                        stop = 1;
                        continue;
                    }
                }
            }
        }
        return stop;
    }


    void CSpreadSheet::create_cell_helper(int dest_start_row, int dest_start_col, int row_count, int col_count,string exp_result1){
        string ident_dest = matrix[dest_start_row+row_count][dest_start_col+col_count];
        CPos pos(ident_dest);
        setCell(pos, exp_result1);
    }



    void CSpreadSheet::copyRect( CPos dst,CPos src,int w = 1, int h = 1 ) {//src is upper left, dest is upperl eft. They may overlap

       vector<int>ve = helper_search_index(src);
       int start_row = ve[0];
       int start_col= ve[1];

       int fin_row = start_row + h;
       int fin_col = start_col+ w;
       int row_count = 0, col_count = 0;

       if(cell_index[dst].empty()){//it doesnt exist yet
           setCell(dst, "");//create it
       }

       vector<int>ve2 = helper_search_index(dst);
       int dest_start_row = ve2[0];
       int dest_start_col= ve2[1];


        map<vector<int>, string> alt;
        vector<int> vec_alt(2);
        int end_r = dest_start_row + h;
        int end_c = dest_start_col + w;

        int up = 0, bot = 0, right = 0, left = 0;
        (dest_start_row >= start_row)? up = dest_start_row, bot = fin_row : up = start_row, bot = end_r;
        (dest_start_col >= start_col)? left = dest_start_col, right = fin_col : left = start_col, right = end_c;


        for(int i = up; i < bot; i++){
            for(int j = left; j < right; j++){
                vec_alt[0] = i;
                vec_alt[1] = j;
                alt[vec_alt] = Find_position_by_str(matrix[i][j]);
            }
        }

       string res, exp_result1;
       int rw = 0, cl = 0;
       vector<string>v2(2);
       MyParser prs(*this);
       set<string> vis;
       int row_neigb_d = 0,col_neigb_r= 0;
       int stop = 0;
       int found1;

        for(int i = start_row; i < fin_row; i++){
            for(int j = start_col; j < fin_col; j++){
                 res = Find_position_by_str(matrix[i][j]);
                auto found = res.find('$');
                if(found != string::npos){
                    if((res[1] =='!'&& found == 2) || (res[1] !='!' && found==1)){
                        (res[1] == '!') ? found1 = res.find('$',3) : found1 = res.find('$',2);

                        if(found1 != string::npos){//there are 2 $. set the value as value of original.
                            create_cell_helper(dest_start_row,dest_start_col,row_count,col_count,res);
                            col_count++;
                            continue;
                        }
                        else{//there is one $(row)

                            if(i >= up && i <= bot && j >= left && j <= right) {//check the alt
                                stop = check_alt_helper_row_col(i,j,dest_start_row,dest_start_col,col_count,row_count,alt);
                                if (stop == 1) {
                                    stop = 0;
                                    col_count++;
                                    continue;
                                }
                            }

                            string orig = res;
                            v2 = tokenize(res);
                            string to_save = v2[0];
                            string fin_str = v2[1];

                            vector<int>v1 =  helper_double_search(fin_str);
                            rw = v1[0];
                            cl = v1[1];

                            if(rw == -1 || cl == -1 ){//(content is empty)
                                vector<int> vv = search_table(fin_str);
                                rw =vv[0];
                                cl = vv[1];
                            }

                                if(row_count + dest_start_row == i){//rows are equal
                                    exp_result1 = orig;
                                    create_cell_helper(dest_start_row,dest_start_col,row_count,col_count,exp_result1);
                                    col_count++;
                                    continue;
                                }
                                //row is smaller or bigger
                                (row_count + dest_start_row < i) ? row_neigb_d = rw -(i - (row_count + dest_start_row )) : row_neigb_d = rw + ((row_count + dest_start_row) - i);

                                string add_doll = matrix[row_neigb_d][cl];
                                exp_result1 = "=!$" + matrix[row_neigb_d][cl] + to_save;//add ! as sign
                                create_cell_helper(dest_start_row,dest_start_col,row_count,col_count,exp_result1);
                                col_count++;
                                continue;
                            }

                    }

                    else{//there is only one (col)

                        if(i >= up && i <= bot && j >= left && j <= right) {//check the alt
                            stop = check_alt_helper_row_col(i,j,dest_start_row,dest_start_col,col_count,row_count,alt);
                            if (stop == 1) {
                                stop = 0;
                                col_count++;
                                continue;
                            }
                        }

                        string orig = res;
                        v2 = tokenize(res);
                        string to_save = v2[0];
                        string fin_str = v2[1];

                        vector<int>v1 = helper_double_search(fin_str);
                        rw = v1[0];
                        cl = v1[1];

                        if(rw == -1 || cl == -1 ){//(content is empty)
                            vector<int> vv = search_table(fin_str);
                            rw =vv[0];
                            cl = vv[1];
                        }
                            if(col_count + dest_start_col == j){//cols are equal
                                 string exp_result1 = orig;
                                 create_cell_helper(dest_start_row,dest_start_col,row_count,col_count,exp_result1);
                                 col_count++;
                                 continue;
                            }//col is smaller or bigger
                            (col_count + dest_start_col < j) ?  col_neigb_r = cl - (j - (col_count + dest_start_col)) : col_neigb_r =cl + ((col_count + dest_start_col) - j);

                            string add_doll = matrix[rw][col_neigb_r];
                            add_doll.insert(add_doll.begin()+1,'$');
                            exp_result1 = "=!" + add_doll + to_save;//add ! as sign
                            create_cell_helper(dest_start_row,dest_start_col,row_count,col_count,exp_result1);
                            col_count++;
                    }

                }
                else{//there are none

                    if(i >= up && i <= bot && j >= left && j <= right){//check for overlapping(alt)
                        stop = check_alt_helper_none(i,j,dest_start_row,dest_start_col,col_count,row_count,alt);//check for overlapped values
                        if(stop == 1){
                            stop = 0;
                            col_count++;
                            continue;
                        }
                    }

                    string orig = res;
                    if(prs.isStr(res) || prs.isNum(res) != 0){    //if content of src is just str or val, copy it straight into dest
                        exp_result1 = res;
                        string ident_dest = matrix[dest_start_row+row_count][dest_start_col+col_count];
                        create_cell_helper(dest_start_row,dest_start_col,row_count,col_count,exp_result1);
                        col_count++;
                        continue;
                    }

                    v2 = tokenize(res);
                    string to_save = v2[0];
                    string fin_str = v2[1];

                    vector<int>v1 =  helper_double_search(fin_str);
                    rw = v1[0];
                    cl = v1[1];

                    if(rw == -1 || cl == -1 ){//(content is empty)
                        vector<int>vv = search_table(fin_str);
                        rw =vv[0];
                        cl = vv[1];

                        int diff_rows = 0,  diff_cols  = 0;
                        int final_pos_rw = 0,  final_pos_cl = 0;
                        (i >= rw) ? diff_rows = i - rw,  final_pos_rw = (dest_start_row + row_count)- diff_rows : diff_rows = rw - i,final_pos_rw =  (dest_start_row + row_count)+ diff_rows;
                        (j >= cl) ? diff_cols = j - cl, final_pos_cl = (dest_start_col + col_count)- diff_cols :  diff_cols = cl - j, final_pos_cl = (dest_start_col + col_count)+ diff_cols;

                        create_cell_helper(dest_start_row,dest_start_col,row_count,col_count,exp_result1);
                        col_count++;
                        continue;

                    }
                        int diff_rows = 0,  diff_cols  = 0;

                        diff_rows = (dest_start_row+row_count) - i;
                        diff_cols = (dest_start_col+col_count) - j;

                        CPos p(fin_str); //find position thats in 1 1 from DO
                        auto k = cell_index[p];//D0 coordinates
                        int final_pos_rw = k[0] + diff_rows;//E1 coordinates
                        int final_pos_cl = k[1] + diff_cols;

                        exp_result1 = "=!" + matrix[final_pos_rw][final_pos_cl] + to_save;//add ! as sign
                        create_cell_helper(dest_start_row,dest_start_col,row_count,col_count,exp_result1);
                        col_count++;
                }
            }
            col_count = 0;
            row_count++;
        }
    }


vector<string> CSpreadSheet::range_helper(string str) {


    MyParser prs(*this);
    set<string> vis;

    string cp_str = str;

    cp_str.erase(remove(cp_str.begin(), cp_str.end(), ' '), cp_str.end());
    int fr = 0;
    int sc = 0;

    string first_val;
    string sec_val;
    // get tge range
    for (int i = 0; i < cp_str.length(); i++) {
        if (fr == 1 && cp_str[i] != ':') {
            first_val.push_back(cp_str[i]);
        }
        if (cp_str[i] == ':' && fr == 1) {
            fr = 0;
            sc = 1;
        }
        if (cp_str[i] == '(' && fr == 0) {
            fr = 1;
        }
        if (cp_str[i] != ')' && cp_str[i] != ':' && sc == 1) {
            sec_val.push_back(cp_str[i]);
        }

        if (cp_str[i] == ')') {
            sc = 0;
        }

    }
    vector<string>v(2);
    v[0] = first_val;
    v[1] = sec_val;

    return v;
}
double CSpreadSheet::sum_range(string str){

    MyParser prs(*this);
    set<string> vis;

    vector<string> v0 = range_helper(str);
    string first_val = v0[0];
    string sec_val = v0[1];

    vector<int>v1 = helper_double_search(first_val);
    vector<int>v2 = helper_double_search(sec_val);

    //loop through matrix, sum up all non-str elements
    int start_row = v1[0];
    int start_col = v1[1];
    int end_row = v2[0];
    int end_col= v2[1];

    double sum = 0;

    for(int i = start_row; i <= end_row; i++){
        for(int j = start_col; j <= end_col; j++){
            auto res = Find_position_by_str(matrix[i][j]);
            if(!prs.isStr(res)){
                auto parsed = prs.parse_exp(matrix[i][j],vis);
                if(prs.isNum(to_string(parsed)) && parsed != INFINITY){
                    sum  = sum +  parsed;
                }
            }

        }
    }

    return sum;
}

double CSpreadSheet::max_range(string str){

    MyParser prs(*this);
    set<string> vis;

    vector<string> v0 = range_helper(str);
    string first_val = v0[0];
    string sec_val = v0[1];

    vector<int>v1 = helper_double_search(first_val);
    vector<int>v2 = helper_double_search(sec_val);

    int start_row = v1[0];
    int start_col = v1[1];
    int end_row = v2[0];
    int end_col= v2[1];

    double sum = 0;
    double max = 0;

    for(int i = start_row; i <= end_row; i++){
        for(int j = start_col; j <= end_col; j++){
            auto res = Find_position_by_str(matrix[i][j]);
            if(!prs.isStr(res)) {
                sum = prs.parse_exp(matrix[i][j], vis);
                if (prs.isNum(to_string(sum)) && sum != INFINITY) {
                    if (max < sum) {
                        max = sum;
                    }
                }
            }
        }
    }

    return max;
}

double CSpreadSheet::min_range(string str){

    MyParser prs(*this);
    set<string> vis;

    vector<string> v0 = range_helper(str);
    string first_val = v0[0];
    string sec_val = v0[1];

    vector<int>v1 = helper_double_search(first_val);
    vector<int>v2 = helper_double_search(sec_val);

    int start_row = v1[0];
    int start_col = v1[1];
    int end_row = v2[0];
    int end_col= v2[1];

    double sum = 0;
    double min = DBL_MAX;

    for(int i = start_row; i <= end_row; i++){
        for(int j = start_col; j <= end_col; j++){
            auto res = Find_position_by_str(matrix[i][j]);
            if(!prs.isStr(res)) {
                sum = prs.parse_exp(matrix[i][j], vis);
                if (prs.isNum(to_string(sum)) && sum != INFINITY) {
                    if (min > sum) {
                        min = sum;
                    }
                }
            }
        }
    }

    return min;
}

double CSpreadSheet::count_range(string str){

    MyParser prs(*this);
    set<string> vis;

    vector<string> v0 = range_helper(str);
    string first_val = v0[0];
    string sec_val = v0[1];

    vector<int>v1 = helper_double_search(first_val);
    vector<int>v2 = helper_double_search(sec_val);

    int start_row = v1[0];
    int start_col = v1[1];
    int end_row = v2[0];
    int end_col= v2[1];

    int count = 0;

    for(int i = start_row; i <= end_row; i++){
        for(int j = start_col; j <= end_col; j++){
            auto res = Find_position_by_str(matrix[i][j]);
            if(prs.isNum(res)){
                count++;
            }

        }
    }

    return count;
}

double CSpreadSheet::countval_range(string str){


    MyParser prs(*this);
    set<string> vis;

    string cp_str = str;

    cp_str.erase(remove(cp_str.begin(),cp_str.end(),' '),cp_str.end());
    int vl = 0;
    int fr = 0;
    int sc = 0;

    string val;

    string first_val;
    string sec_val;
    // get the  value and range
    for(int i = 0; i < cp_str.length(); i++){
        if (fr == 1 && cp_str[i] != ':'){
            first_val.push_back(cp_str[i]);
        }
        if(cp_str[i] == ':' && fr == 1){
            fr = 0;
            sc = 1;
        }
        if(cp_str[i] == '('){
            vl = 1;
        }
        if(vl == 1 && cp_str[i] !=',' && cp_str[i] !='(') {
            val.push_back(cp_str[i]);;
        }
        if(cp_str[i] == ',' && fr == 0){
            vl = 0;
            fr = 1;
        }
        if(cp_str[i] != ')' && cp_str[i] != ':' &&  sc == 1){
            sec_val.push_back(cp_str[i]);
        }

        if(cp_str[i] == ')'){
            sc = 0;
        }

    }

    vector<int>v1 = helper_double_search(first_val);
    vector<int>v2 = helper_double_search(sec_val);

    int start_row = v1[0];
    int start_col = v1[1];
    int end_row = v2[0];
    int end_col= v2[1];

    int count = 0;
    double sum;
    double val_db = stod(val);


    for(int i = start_row; i <= end_row; i++){
        for(int j = start_col; j <= end_col; j++){
            auto res = Find_position_by_str(matrix[i][j]);
             sum = prs.parse_exp(matrix[i][j],vis);

            if (sum == val_db) {
              count++;
            }

        }
    }

    return count;
}


string CSpreadSheet::Find_position_by_str(string &str){
        for(const auto& [pos, val] : m_cells){// loop over all Cpos objects
            if(str == pos.getIdent()){ // if input string == position in Cpos
                 return val; //return string thats in this pos
            }
        }

    }


