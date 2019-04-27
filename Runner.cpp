#include "headers/Methan0l.h"

// (entry)[def a,b,c; get a,b,c; inc a; dmp a;](sub:arg1,arg2,arg3)[arg1+arg2-arg3=sub; ret;]

Runner::Runner(){
    srand (time(NULL));
    this->rfunc = "\\((.*?)\\)";
    this->rbody = "\\[(.*?)\\]";
    this->rtimes = "times.*?\\((.*?)\\)\\[(.*?)\\]";
    this->rwhile = "while.*?\\((.*?)\\)\\[(.*?)\\]";
    this->rif = "if\\s*\\((.*?)\\)\\s*\\s*\\[(.*?)\\]\\s*(else)?(\\s*\\[(.*?)\\])?";
    this->rnest = "(\\((.*?)\\^(.*?)\\))";
    this->rsubstr = "(\\((.*?)\\))\\s*\\^";
    this->rarr = "(\\(\\s*\\[(.*?)\\])\\s*\\^\\s*(.+)\\)";
}

void Runner::varSet(std::string var, auto what){
    if (isReadOnly(var))
        return;
    if (varExists(var)) this->var.erase(var);
    this->var.insert(std::make_pair(var, str(what)));
}

void Runner::varForceSet(std::string var, auto what){
    if (varExists(var)) this->var.erase(var);
    this->var.insert(std::make_pair(var, str(what)));
}

void Runner::purge(bool everything){
    if (everything)
        this->var.clear();
    this->modules.clear();
}

std::string Runner::escapeStruct(std::string str, std::regex rgx, std::string structname){
    std::smatch rtm;
    std::string tmid, delim = "{$}";
    std::regex_search(str, rtm, rgx);
        tmid = "${"+randString()+"};";
        if (structname == "times" || structname == "while")
            varSet(tmid, structname+delim+rtm.str(1)+delim+rtm.str(2));
        else if (structname == "if") {
            std::string cond = rtm[1], ops = rtm[2], els="no", scond;
            try {
                els = rtm[3];
                scond = rtm[5];
            } catch(const std::exception& e){
                //no else block (can't be detected another way... i guess.)
            }
            if (els!="no"){
                varSet(tmid, structname+delim+cond+delim+ops+delim+els+delim+scond);
            } else {
                varSet(tmid, structname+delim+cond+delim+ops);
            }
        }   else if (structname == "array"){
            tmid = "${arr$"+randString()+"}";
            varSet(tmid, rtm.str(0));
            packed.push_back(tmid);
        }
        str.erase(rtm.position(), rtm.length());
        str.insert(rtm.position(), tmid);
    return str;
}

std::string escapeStructs(std::string arg){
    Blocks blocks;
    size_t j = 0;
    size_t opl = 0, cll = 0;
    int opos, clpos, tokpos;
    for (size_t i = 0; i < arg.size(); i++) {
        if (arg[i] == '[') {
            opos = i;
            j = i;
            while(arg[j] != '#') j--;
            tokpos = j+1;
            opl++;
            if (opl > 1)
                blocks.add(tokpos, opos, blocks.id(blocks.size()-1).start);
            else
                blocks.add(tokpos, opos);
        }
        else if (arg[i] == ']') {
            clpos = i;
            opl--;
        }
    }
    return arg;
}

void Runner::parseStructs(std::string code){
    std::smatch raw;
    std::string tmp = code, tname, tcode;
    std::pair<std::string, Program> placeholder;
    Program temp;
    std::vector<std::string> arl;
    //tmp = escapeStructs(tmp,rarr, "array");
    //tmp = escapeStructs(tmp,rif, "if");
    //tmp = escapeStructs(tmp,rwhile, "while");
    //tmp = escapeStructs(tmp,rtimes, "times");
    while(std::regex_search(tmp, raw, rfunc)){
            tname = raw[1];
            if (std::regex_search(tmp, raw, rbody)){
                tcode = raw[1];
                if (contains(tname, ":")){
                    arl = split(tname,"\\:");
                    tname = arl[0];
                    arl = parseList(arl[1]);
                }
                for (auto un : packed){
                    tcode = replaceall(tcode,un,varGet(un));
                    varDel(un);
                }
                packed.clear();
                temp = Program(tcode);
                temp.setArgNames(arl);
                placeholder = std::make_pair(tname,temp);
                modules.insert(placeholder);
            }
            tmp = raw.suffix();
    }
}

std::string randString(size_t length) {
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

void Runner::dump(){
    std::cout<<"Variable dump"<<'\n';
    for (auto dmp : this->var){
        std::cout<<dmp.first<<" -> "<<dmp.second<<std::endl;
    }
}

std::vector<std::string> split(std::string input, std::string regex) {
    std::regex re(regex);
    std::sregex_token_iterator
        first{input.begin(), input.end(), re, -1},
        last;
    return {first, last};
}

std::vector<std::string> Runner::parseList(std::string rawlist){
    std::smatch res;
    bool nst = false;
    std::string rpl;
    while (std::regex_search(rawlist,res,rnest)){
        nst = true;
        rpl = strim(replaceall(res.str(0),",","{c}"));
        rawlist.erase(res.position(),res.length());
        rawlist.insert(res.position(),rpl);
    }
    auto tr = split(rawlist,",");
    if (!nst && tr.size() == 0) return {rawlist};
    if (!nst) return tr;
    if (tr.size() == 0) tr = {rawlist};
    for (std::string &tmp : tr){
        if (contains(tmp,"{c}")){
            tmp = replaceall(tmp,"{c}",",");
            trim(tmp);
        }
    }
    return tr;
}

bool Runner::varExists(std::string arg){
    if (this->var.find(arg) == this->var.end())
        return false;
    else
        return true;
}

void Runner::varDel(std::string var) {
    this->var.erase(var);
}

void stripReserved(std::string& arg){
    std::vector<std::string> lst = {"{","}"};
    for (auto ch : lst){
        arg = replaceall(arg,ch,"");
    }
}

std::string Runner::varGet(std::string var){
    if (varExists(var))
        return this->var[var];
    else
        return "";
}

bool isNum(std::string s){
    s = replaceall(s,".","");
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

bool isMathExp(std::string arg){
    std::string sexp[] = {"**","+","-","*","/","sqrt","sin","cos","pi","abs"};
    for (auto s : sexp){
        if (arg.find(s)!=std::string::npos){
            return true;
        }
    }
    return false;
}

bool contains(std::string str, std::string what){
    if (str.find(what)!=std::string::npos)
        return true;
    else
        return false;
}

bool containsAll(std::string str, std::vector<std::string> wlist){
    for (auto what : wlist){
        if (str.find(what)==std::string::npos)
            return false;
    }
    return true;
}

std::string replaceall(std::string subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

std::string strim (std::string str){
    return str.substr(1,str.length()-2);
}

std::string Runner::solveMathExpr(std::string exprs){
    expression_t expression;
    exprs = replaceall(exprs,"**","^");
    mathparser.compile(exprs,expression);
    return str(expression.value());
}

std::string Runner::strExpConcat(std::string str){
    std::string ret = "";
    std::vector<std::string> sbf = split(str,"\\$");
    for (auto var : sbf){
        trim(var);
        if (var[0] == '"' && var.back() == '"'){
            ret+=strim(var);
        }
        else if (var[0] == '"'){
            ret+=strim(var);
        } else {
            ret+=parseBasicExpressions(var);
        }
    }
    return ret;
}

std::string Runner::replaceVarExp(std::string str, bool ifs){
    std::string key,val;
    for (std::pair<std::string, std::string> unit : this->var){
        key = unit.first;
        val = unit.second;
        if (ifs && !isNum(val)){
            val = "'"+val+"'";
        }
        str = replaceall(str, key, val);
    }
    return str;
}

bool Runner::funcExists(std::string func){
    if (this->modules.find(func) == this->modules.end())
        return false;
    else
        return true;
}

bool Runner::parseLogicalExpression(std::string str){
    logicalexpr_t expression;
    str = replaceVarExp(str,true);
    str = replaceall(str,"\"","'");
    logicparser.compile(str,expression);
    return expression.value();
}

std::string Runner::parseBasicExpressions(std::string str, std::string excl){
    trim(str);
    std::string arr = getArrayCell("("+str+")");
    if (excl == "out" && str[0] == '$'){
        std::cout<<'\n';
        return "";

    }
    else if (arr != ""){
        return varGet(arr);
    }
    else if (str[0] != '"' && contains(str, "$")){
        return strExpConcat(str);
    }
    else if (str[0] == '"'){
        if (contains(str,"$")){
            return strExpConcat(str);
        }
        else
            return strim(str);
    }
    else if (contains(str,"^")){  // func^a,b,func^c
        exec(Program(str+";"));
        return ret;
    }
    else if (isMathExp(str)){
        str = replaceVarExp(str);
        return solveMathExpr(str);
    }
    else if (isNum(str)){
        return str;
    }
    else if (!isNum(str)){
        return varGet(str);
    }
    else return "nil";
}

void Runner::setReadOnly(std::string var){
    trim(var);
    if (!isReadOnly(var)){
        ro.push_back(var);
    }
}

bool Runner::isReadOnly(std::string var){
    for (auto el : ro){
        if (el == var)
            return true;
    }
    return false;
}

std::string Runner::getArrayCell(std::string cmd){
std::string rs = "", op, szb, ceb,arrname;
std::smatch res;
cmd = replaceall(cmd," ","");
if (std::regex_search(cmd, res, this->rarr)){    //([n]^array) <--nth element of array
    arrname = res.str(3);       //array name
    szb = varGet(arrname);      //array size
    setReadOnly(arrname);
    ceb = res.str(2);   //current element number
    auto dims = parseList(szb), cebs = parseList(ceb);
    if (dims[0] == "")
        dims = cebs;
    if (dims.size() != cebs.size())
        return "";
    int i = 0;
    std::string newsize = "", edim, ecebs;
    for (auto &dim : dims) {            //([1]^arr)%100
        if (i>0)
            newsize+=",";
        edim = parseBasicExpressions(dim);
        ecebs = parseBasicExpressions(cebs[i]);
        if (atoi(edim.c_str())<atoi(ecebs.c_str()))
            dim = ecebs;
        else
            dim = edim;
        newsize+=dim;
        i++;
    }
    rs = "{(["+newsize+"]^"+arrname+")}";
    varForceSet(arrname, newsize);
}
return rs;
}

void Runner::panic(std::string err){
    std::cout<<"\n"<<err;
    getline(std::cin, err);
    exit(-1);
}

void Runner::exec(Program program, std::vector<std::string> args){
    std::size_t len = program.queued.size(), i = 0;
    std::smatch res;
    std::string cmd, arglist, op;
    std::vector<std::string> explist;
    std::string varlist, tmp;
    std::string arr;
    try {
        while(i < len){
            cmd = program.queued[i];
            trim(cmd);
            arr = getArrayCell(cmd);
            if (contains(cmd,"^")){
                op = split(cmd,"\\^")[0];
                arglist = cmd.substr(op.size()+1);
                trim(op);
                trim(arglist);
            } else {
                op = "";
                arglist = "";
            }
            if (cmd.substr(0,2) == "${"){
                cmd = varGet(cmd+";");
                auto rbf = split(cmd,"\\{\\$\\}");
                std::string op = rbf[0];
                trim(op);
                if (op == "times") {    //times(i, 100)[]
                    std::string params = rbf[1], rcode = rbf[2];
                    trim(params);
                    trim(rcode);
                    auto plst = parseList(params);  //0->counter variable   1->end value
                    if (plst.size() < 2)
                        panic();
                    trim(plst[0]);
                    trim(plst[1]);
                    if (!varExists(plst[0]))
                        varSet(plst[0], 0);
                    int ci = std::stoi(parseBasicExpressions(plst[1])), dc=0;
                    Program cprg(rcode);
                    for (int cc = std::stoi(varGet(plst[0])); cc<ci; cc++){
                        exec(cprg);
                        dc++;
                        varSet(plst[0], dc);
                    }
                } else if (op == "while"){      //while(i<j and foo!="bar")[]
                    std::string cond = rbf[1], wcode = rbf[2];
                    trim(cond);
                    trim(wcode);
                    while(parseLogicalExpression(cond)){
                        exec(wcode);
                    }
                }
                 else if (op == "if"){
                    int iflen = rbf.size();
                        if (parseLogicalExpression(rbf[1])){
                            Program ifprog(rbf[2]);
                            exec(ifprog);
                        } else {
                            if (iflen != 4){//then else block exists
                                Program ifprog(rbf[4]);
                                exec(ifprog);
                            }
                        }
                }

            }
            else if (op == "def"){     //def^a%10,b%a,c%"test string";
                varlist = arglist;
                explist = parseList(varlist);
                if (explist.size() == 0)
                    explist = {varlist};
                for (auto var : explist){
                    trim(var);
                    auto tmplist = split(var,"%");
                    if (tmplist.size() == 0) varSet(tmplist[0],"");
                    else {
                        trim(tmplist[1]);
                        trim(tmplist[0]);
                        tmp = tmplist[1];
                        varSet(tmplist[0], parseBasicExpressions(tmp));
                    }
                }
            } else if (op == "round"){  //round^expr,precision;
                explist = parseList(arglist);
                trim(explist[0]);
                trim(explist[1]);
                ret = str(stod(parseBasicExpressions(explist[0])), atoi(parseBasicExpressions(explist[1]).c_str()));
            }
            else if (op == "out") {   //out^a," ",228;
                varlist = arglist;
                explist = parseList(varlist);
                if (explist.size() == 0)
                    explist = {varlist};
                for (auto var : explist){
                        trim(var);
                        var = parseBasicExpressions(var, "out");
                        if (var == "") continue;
                        std::cout<<var;
                }
            }
            else if (op == "in") {  //in^"Input: ",var;
                varlist = arglist;
                explist = parseList(varlist);
                std::string buf;
                if (explist.size() == 0){
                    explist = {varlist};
                }
                for (auto var : explist){
                    trim(var);
                    if (var[0] == '"'){
                        std::cout<<strim(var);
                        continue;
                    }
                    getline(std::cin, buf);
                    varSet(var, buf);
                }
            } else if (op == "return"){     //return^expr;
                ret = parseBasicExpressions(arglist);
            } else if (op == "len"){    //len^expr;
                trim(arglist);
                ret = str(parseBasicExpressions(arglist).size());
            } else if (op == "replace") {   //replace^source,old,new;
                auto gr = parseList(arglist);
                for (auto &pt : gr){
                    pt = parseBasicExpressions(pt);
                }
                ret = replaceall(gr[0],gr[1],gr[2]);
            } else if (contains(cmd,"%")){            //var%var2+500*foo;
                auto rexpr = split(cmd,"\\%");
                std::string var = rexpr[0], expr = rexpr[1];
                trim(var);
                trim(expr);
                std::string arr = getArrayCell(var), pexpr = parseBasicExpressions(expr);
                if (arr != ""){
                    varSet(arr, pexpr);
                } else
                    varSet(var,pexpr);
            } else if (std::regex_search(cmd, res, this->rsubstr)){   //(start[,end])^var; -- substr
                op = arglist;
                trim(op);
                arglist = strim(res.str(1));
                auto ls = parseList(arglist);
                for (auto &arg : ls){
                    trim(arg);
                    arg = parseBasicExpressions(arg);
                }
                if (ls.size() == 1){
                    ret = parseBasicExpressions(op).substr(std::stoi(ls[0]));
                } else {
                    ret = parseBasicExpressions(op).substr(std::stoi(ls[0]), std::stoi(ls[1]));
                }
            } else if (arr!=""){
                    ret = varGet(arr);
            } else if (contains(cmd,"^")){    //func^arg1,arg2;
                auto bf = split(cmd,"\\^");
                std::string fname = bf[0];
                std::string argss = cmd.substr(fname.size()+1);
                trim(fname);
                if (!funcExists(fname)){
                    std::cout<<"Function "<<fname<<" does not exist."<<'\n';
                    panic();
                }
                auto args = parseList(argss);
                exec(modules[fname], args);
            }
            i++;
        }
    }
    catch (const std::exception& e) {
        std::cout<<"Fatal error has occurred near "<<cmd<<std::endl<<e.what();
    }
}

void Runner::run(std::string code){
    this->parseStructs(code);
    std::map<std::string,Program>::const_iterator entrypt = modules.find("entry");
    if (entrypt == modules.end()) {
        std::cout<<"Entry function not found."<<std::endl;
    } else {
        exec(entrypt->second);
    }
}
