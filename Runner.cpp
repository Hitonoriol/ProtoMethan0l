#include "headers/Methan0l.h"

// (entry)[def a,b,c; get a,b,c; inc a; dmp a;](sub:arg1,arg2,arg3)[arg1+arg2-arg3=sub; ret;]

Runner::Runner(){
    this->rfunc = "\\((.*?)\\)";
    this->rbody = "\\[(.*?)\\]";
    this->rtimes = "times.*?\\((.*?)\\)\\[(.*?)\\]";
}

void Runner::purge(bool everything){
    if (everything)
        this->var.clear();
    this->modules.clear();
}

std::string Runner::escapeStructs(std::string str, std::regex rtimes){
    std::smatch rtm;
    std::string tmid, delim = "{$}";
    while(std::regex_search(str, rtm, rtimes)){
        tmid = "${"+randString()+"};";
        varSet(tmid, rtm.str(1)+delim+rtm.str(2));
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
    tmp = escapeStructs(tmp,rtimes);
    while(std::regex_search(tmp, raw, rfunc)){
            tname = raw[1];
            if (std::regex_search(tmp, raw, rbody)){
                tcode = raw[1];
                placeholder = std::make_pair(tname,Program(tcode));
                modules.insert(placeholder);
            }
            tmp = raw.suffix();
    }
}

std::string randString(int len) {
     std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
     std::random_device rd;
     std::mt19937 generator(rd());
     std::shuffle(str.begin(), str.end(), generator);
     return str.substr(0, len);
}

void Runner::dump(){
    for (auto dmp : this->modules){
        std::cout<<dmp.first<<" -> "<<dmp.second.contents<<std::endl;
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
    return split(rawlist,",");
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
        } else{
            ret+=varGet(var);
        }
    }
    return ret;
}

std::string Runner::replaceVarExp(std::string str){
    for (std::pair<std::string, std::string> unit : this->var){
        str = replaceall(str, unit.first, unit.second);
    }
    return str;
}

bool Runner::funcExists(std::string func){
    if (this->modules.find(func) == this->modules.end())
        return false;
    else
        return true;
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
    else return "err";
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
                std::cout<<cmd<<std::endl;
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
            }
            else if (std::regex_search(cmd, res, this->rtimes)){
                std::string times = res[1], code = res[2];
                std::cout<<times<<" "<<code<<std::endl;
            }
            else if (contains(cmd,"^")){    //func^arg1,arg2;
                auto bf = split(cmd,"\\^");
                std::string fname = bf[0];
                std::string argss = cmd.substr(fname.size()+1);
                trim(fname);
                if (!funcExists(fname))
                    continue;
                auto args = parseList(argss);
                exec(modules[fname], args);
            }

            else if (contains(cmd,"%")){            //var%var2+500*foo;
                auto rexpr = split(cmd,"%");
                std::string var = rexpr[0], expr = rexpr[1];
                trim(var);
                trim(expr);
                varSet(var,parseBasicExpressions(expr));
            }
            i++;
        }
    }
    catch (const std::exception& e) {
        std::cout<<"Fatal error has occurred."<<std::endl<<e.what();
    }
}

void Runner::run(std::string code){
    this->parseStructs(code);
    //this->dump();
    std::map<std::string,Program>::const_iterator entrypt = modules.find("entry");
    if (entrypt == modules.end()) {
        std::cout<<"Entry function not found."<<std::endl;
    } else {
        exec(entrypt->second);
    }
}
