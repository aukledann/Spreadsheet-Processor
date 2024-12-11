//
// Created by annab on 16.10.2024.
//
#include "CSpreadSheet.h"
#include "CPos.h"
#include "MyParser.h"
using namespace std;

   // MyParser::MyParser() {};
    MyParser::MyParser(CSpreadSheet& sheet) : m_sheet(sheet){};



    double MyParser:: isNum(string str){
        string sub;
        if(str[0] == '='){
             sub = str.substr(1,str.length());
        }
        else{
            sub = str;
        }

        sub.erase(remove(sub.begin(), sub.end(), ' '), sub.end());


        int start =0;
        if(sub[0] == '-' ) {
            start = 1;
        }
        string operators = "+-*/^()";

        for( int i = start; i < sub.length(); i++ ) {
            if(sub[i] == 'e' && i == sub.length() - 1){
                continue;
            }
            if(sub[i] == 'e' && (isdigit(sub[i+1]))|| sub[i+1] == '+'){
                i++;
                continue;
            }
            if((!isdigit( sub[i]) || is_op(sub[i])) && (sub[i] != '.' )) {
                return 0;
            }
        }

        return stod(sub);
    }

    // check if its just a string
    bool MyParser:: isStr(string str){
        //check if there is atl 1 operator, atl num_of_oper + 1 digit

        const string operators = "+-*/^><=";
        const string digits = "0123456789";

        auto found_op =  str.find_first_of(operators);
        if(found_op != string::npos){// found an operator
            auto found_num = str.find_first_of(digits);
            if(found_num != string::npos){//found a digit
                return false;
            }
            return true;
        }//not an expr4ession
        auto found_num = str.find_first_of(digits);
        if(found_num == 0){//its a number
            return false;
        }

        return true;
    }

    bool MyParser:: is_op(char ch){
        if(ch =='+' ||  ch =='-' || ch =='*' || ch =='/' || ch =='^' || ch ==')' || ch =='('){
            return true;
        }
        return false;
    }

void MyParser::find_and_remove_spaces(string &str, const string &search_term) {
    auto found = str.find(search_term, 0);
    int diff;
    if (found != string::npos) {
        for (int i = found; i < str.length(); i++) {
            if (str[i] == ')') {
                diff = str.length() - i - 1;
                break;
            }
        }
        str.erase(remove(str.begin() + found, str.end() - diff, ' '), str.end() - diff);
    }
}

     string MyParser::add_delim(string &str){
        int pos = 0;
        while (pos < str.length()) {
            if (is_op(str[pos]) || is_logic_op(str[pos])) {
                if((pos == 0 && str[pos] == '-') || (str[pos-2] == '(' && !is_op(str[pos+1])) ||(str[pos]== '=' && str[pos-1] == '>')  ||(str[pos]== '=' && str[pos-1] == '<') ||(str[pos]== '>' && str[pos-1] == '<')){
                    pos++;
                    continue;
                }
                if (pos > 0 && str[pos - 1] != ' ' ) { // add space before op
                    str.insert(pos, " ");
                    pos++;
                }
                if (pos + 1 < str.length() && str[pos + 1] != ' ' ) {// add space after op
                    if((str[pos]== '>' && str[pos+1] == '=')  ||(str[pos]== '<' && str[pos+1] == '=') ||(str[pos]== '<' && str[pos+1] == '>')){
                        pos++;
                    }
                    str.insert(pos + 1, " ");
                    pos++;
                }
            }
            pos++;
        }

         find_and_remove_spaces(str, "sum");
         find_and_remove_spaces(str, "max");
         find_and_remove_spaces(str, "min");
         find_and_remove_spaces(str, "count");
         find_and_remove_spaces(str, "countval");

            return str;
    }

    int MyParser:: priority(char &ch){
        if(ch == '+' || ch == '-'){
            return 1;
        }
        else if(ch == '*' || ch == '/'){
            return 2;
        }
        else if(ch == '^'){
            return 3;
        }
        //else if(ch == function call){
        //return 4;
        //}
        return 0;
    }


    double MyParser:: applyOp(double a, double b, char op){
        switch (op) {
            case '+':
                return a + b;
            case '-':
                return a - b;
            case '*':
                return a * b;
            case '/':
                return a / b;
            case '^':
                if(a < 0){
                    return -pow(a,b);
                }
                else{
                    return pow(a, b);
                }
            default:
                return 0;
        }
    }

    bool MyParser::is_logic_op(char ch) {
        if(ch =='>' ||  ch =='<' || ch =='=' ){
            return true;
        }
         return false;
    }

    bool  MyParser::Apply_logic_op(CValue a, CValue b, char op){
        switch (op) {
            case '>':
                return a > b;
            case '<':
                return a < b;
            case '=':
                return a == b;
            case ',':
                return a >= b;
            case '#':
                return a <= b;
            case '?':
                return a != b;
            default:
                return 0;
        }
    }


    double MyParser:: parse_exp(string &exp,set<string> &visited){

        auto found0 = exp.find('!');
        if(exp.empty() || found0 != string::npos){//if F0 is still empty
           return INFINITY;
        }


        string subexpr;
        if(exp[0] == '='){
            subexpr = exp.substr(1);
        }
        else{
            subexpr = exp;
        }

        auto found = subexpr.find(' ');
        if(found == string::npos){
            add_delim(subexpr);
        }

        int count = 0;

        stack<double> nums;
        stack<char> operators;
        stringstream ss(subexpr);
        string token;
        int is_logic_oper = 0;
        int negative_exp = 0;

        while(getline(ss, token,' ')){
            if (token.empty()) {
                continue;
            }

            else if (isalpha(token[0]) || token[0] == '$') {
                if(token.find('$') != string::npos){//there is a reference
                    token.erase(remove(token.begin(),token.end(), '$'),token.end());
                }

                if(token[0] == 's' && token[1] == 'u' && token[2] == 'm'){
                    double k =  0;
                    k = m_sheet.sum_range(exp);
                    nums.push(k);
                    count++;
                    continue;
                }
                if(token[0] == 'm' && token[1] == 'a' && token[2] == 'x'){
                    double k =  0;
                    k = m_sheet.max_range(exp);
                    nums.push(k);
                    count++;
                    continue;
                }
                if(token[0] == 'm' && token[1] == 'i' && token[2] == 'n'){
                    double k =  0;
                    k = m_sheet.min_range(exp);
                    nums.push(k);
                    count++;
                    continue;
                }

                if(token[0] == 'c' && token[1] == 'o' && token[2] == 'u' && token[3] == 'n' && token[4] == 't'  && token[5] == 'v' && token[6] == 'a' && token[7] == 'l'){
                    double k =  0;
                    k = m_sheet.countval_range(exp);
                    nums.push(k);
                    count++;
                    continue;
                }
                if(token[0] == 'c' && token[1] == 'o' && token[2] == 'u' && token[3] == 'n' && token[4] == 't'){
                    double k =  0;
                    k = m_sheet.count_range(exp);
                    nums.push(k);
                    count++;
                    continue;
                }

                if (visited.find(token) != visited.end()) {  // check for circular reference
                    throw std::runtime_error("Circular reference detected for " + token);
                }
                visited.insert(token); // Add to visited

                string res;
                double n = 0;
                res = m_sheet.Find_position_by_str(token);//get a content of the cell

                if(res.empty()){
                    return INFINITY;
                }


                if (!isStr(res) && isNum(res) == 0) {
                    double num = parse_exp(res, visited);// recursively an parse expression
                    nums.push(num);

                } else {
                    if(res[0] == '='){
                        res = res.substr(1);
                    }

                    if (istringstream(res) >> n) {
                        nums.push(n);
                    }
                    else{//if content is string, we restrict operations with it
                        return INFINITY;
                    }
                }

                visited.erase(token); // remove from visited
            }

            else if(isdigit(token[0])){
                double n = 0;
                stringstream (token) >> n;
                nums.push(n);
            }
            else if (is_logic_op(token[0]) == 1) {// >,> , =
                if(token[1] == '=' && token[0] == '>'){//>=
                    token[0] = ',';
                }
                else if(token[1] == '=' && token[0] == '<'){//<=
                    token[0] = '#';
                }
                else if(token[1] == '>' && token[0] == '<'){//!=
                    token[0] = '?';
                }
                char op = token[0];
                operators.push(op);
                is_logic_oper = 1;
                count++;
                continue;
            }

                else if (is_op(token[0]) == 1) { //operator
                char op = token[0];

                if((isalpha(token[1]))|| (count == 0 && token[0] == '-')){//case for unary -
                    string tok_min = token.substr(1);
                    double n;
                    if(token.length() == 1){//its a negative sign before exp, everything in () must be evaluated and then returned negative
                        negative_exp = 1;
                    }
                    else{
                        string res = m_sheet.Find_position_by_str(tok_min);
                        if (istringstream(res) >> n) {
                            nums.push(-n); // Push the value onto the stack
                            count++;
                            continue;
                        }
                    }
                }
                while (!operators.empty() && priority(operators.top())>= priority(op)) {//if it has a smaller priority then top sign
                    if(operators.size() == 1 && operators.top() == '(' && nums.size() == 1){
                        break;
                    }
                    if(operators.size() == 1 && operators.top() == '-' && nums.size() == 0){
                        break;
                    }
                    if(negative_exp==1 && operators.top() != ')'){
                        break;
                    }
                    double b = nums.top();
                    nums.pop();
                    double a = nums.top();
                    nums.pop();
                    char op = operators.top();
                    operators.pop();
                    nums.push(applyOp(a, b, op));
                }
                count++;
                operators.push(op);
            }
            else if (token[0] == '(') {
                operators.push('(');
            }
            else if (token[0] == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    double b = nums.top();
                    nums.pop();
                    double a = nums.top();
                    nums.pop();
                    char op = operators.top();
                    operators.pop();
                    nums.push(applyOp(a, b, op));
                }
                operators.pop();
            }

        }
    if(is_logic_oper == 0) {
        while (!operators.empty()) {
            if (operators.size() == 2 && operators.top() == ')' && operators.top() - 1 == '(' && nums.size() == 1) {
                break;
            }
            if (operators.top() == ')' && negative_exp == 1) {
                operators.pop();
            }
            if (operators.top() == '(' && negative_exp == 1 && nums.size() == 1) {
                operators.pop();
                return -nums.top();
            }
            double b = nums.top();
            nums.pop();
            double a = nums.top();
            nums.pop();
            char op = operators.top();
            operators.pop();
            if(op == '/' && b  == 0){
                return INFINITY;
            }
            nums.push(applyOp(a, b, op));// add to the top of stack sol of expr
        }
        return nums.top();
    }

    else{//for logical exps
        while (!operators.empty()) {
            if (operators.size() == 2 && operators.top() == ')' && operators.top() - 1 == '(' && nums.size() == 1) {
                break;
            }
            if (operators.top() == ')' && negative_exp == 1) {
                operators.pop();
            }
            if (operators.top() == '(' && negative_exp == 1 && nums.size() == 1) {
                operators.pop();
                return -nums.top();
            }
            auto b = nums.top();
            nums.pop();
            auto a = nums.top();
            nums.pop();
            char op = operators.top();
            operators.pop();
            nums.push(Apply_logic_op(a, b, op));// add to the top of stack sol of expr
        }
        return nums.top();
    }

    }



