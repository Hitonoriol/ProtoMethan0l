#include "headers/Methan0l.h"

// (entry)[def a,b,c; get a,b,c; inc a; dmp a;](sub:arg1,arg2,arg3)[arg1+arg2-arg3=sub; ret;]

Runner::Runner(){
    srand (time(NULL));
    this->rfunc = "\\((.*?)\\)";
    this->rbody = "\\[(.*?)\\]";
    this->rtimes = "times.*?\\((.*?)\\)\\[(.*?)\\]";
    this->rwhile = "while.*?\\((.*?)\\)\\[(.*?)\\]";
    this->rif = "if\\s*\\((.*?)\\)\\s*\\s*\\[(.*?)\\]\\s*(else)?(\\s*\\[(.*?)\\])?";
}

void Runner::purge(bool everything){
    if (everything)
        this->var.clear();
    this->modules.clear();
}

std::string Runner::escapeStructs(std::string str, std::regex rgx, std::string structname){
    std::smatch rtm;
    std::string tmid, delim = "{$}";
    while(std::regex_search(str, rtm, rgx)){
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
        }
        str.erase(rtm.position(), rtm.length());
        str.insert(rtm.position(), tmid);
    }
    return str;
}

void Runner::parseStructs(std::string code){
    std::smatch raw;
    std::string tmp = code, tname, tcode;
    std::pair<std::string, Program> placeholder;
    Program temp;
    std::vector<std::string> arl;
    tmp = escapeStructs(tmp,rtimes, "times");
    tmp = escapeStructs(tmp,rif, "if");
    tmp = escapeStructs(tmp,rwhile, "while");
    while(std::regex_search(tmp, raw, rfunc)){
            tname = raw[1];
            if (std::regex_search(tmp, raw, rbody)){
                tcode = raw[1];
                if (contains(tname, ":")){
                    arl = split(tname,"\\:");
                    tname = arl[0];
                    arl = parseList(arl[1]);
                }
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

std::vector<std::string> parseList(std::string rawlist){
    std::smatch res;
    bool nst = false;
    std::string rpl;
    std::regex nest("(\\((.*?)\\^(.*?)\\))");
    while (std::regex_search(rawlist,res,nest)){
        nst = true;
        rpl = strim(replaceall(res.str(0),",","{c}"));
        rawlist.erase(res.position(),res.length());
        rawlist.insert(res.position(),rpl);
    }
    auto tr = split(rawlist,",");
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

void Runner::varSet(std::string var, auto what){
    if (varExists(var)) this->var.erase(var);
    this->var.insert(std::make_pair(var, str(what)));
}

std::string Runner::varGet(std::string var){
    if (varExists(var))
        return this->var[var];
    else
        return "none";
}

bool isNum(std::string s){
    s = replaceall(s,".","");
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

bool isMathExp(std::string arg){
    std::string sexp[] = {"**","+","-","*","/","sqrt","sin","cos","pi"};
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
    logicparser.compile(str,expression);
    return expression.value();
}

std::string Runner::parseBasicExpressions(std::string str, std::string excl){
    if (excl == "out" && str[0] == '$'){
        std::cout<<'\n';
        return "";

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

void Runner::exec(Program program, std::vector<std::string> args){
    std::size_t len = program.queued.size(), i = 0;
    std::smatch res;
    std::string cmd, arglist, op;
    std::vector<std::string> explist;
    std::string varlist, tmp;
    try {
        while(i < len){
            cmd = program.queued[i];
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
                if (op == "times") {
                    std::string params = rbf[1], rcode = rbf[2];
                    trim(params);
                    trim(rcode);
                    auto plst = parseList(params);  //0->counter(variable)   1->end value(everything)
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
                } else if (op == "while"){
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
            } else if (op == "return"){
                ret = parseBasicExpressions(arglist);
            } else if (contains(cmd,"%")){            //var%var2+500*foo;
                auto rexpr = split(cmd,"\\%");
                std::string var = rexpr[0], expr = rexpr[1];
                trim(var);
                trim(expr);
                varSet(var,parseBasicExpressions(expr));
            } else if (contains(cmd,"^")){    //func^arg1,arg2;
                auto bf = split(cmd,"\\^");
                std::string fname = bf[0];
                std::string argss = cmd.substr(fname.size()+1);
                trim(fname);
                if (!funcExists(fname)){
                    std::cout<<"Function "<<fname<<" does not exist."<<'\n';
                    continue;
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
