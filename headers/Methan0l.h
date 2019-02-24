#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <utility>
#include <functional>
#include <map>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <random>
#include "Str.h"

bool containsAll(std::string str, std::vector<std::string> wlist);
std::string randString(int len = 32);

class Files{
    public:
        static std::string readFile(std::string path);
        static bool writeFile(std::string filename, std::string arg);
};

class Program{
    public:
    std::string contents;
    std::vector<std::string> queued;
        Program(std::string contents="nop;");
        void queueExec();
        void setCont(std::string arg);
    private:
};

class Runner{
    public:
        typedef exprtk::symbol_table<double> symbol_table_t;
        typedef exprtk::expression<double>     expression_t;
        typedef exprtk::parser<double>             parser_t;
        parser_t mathparser;
        bool funcExists(std::string func);
        std::string escapeStructs(std::string str, std::regex rgx, std::string structname);
        std::string parseBasicExpressions(std::string str, std::string excl = "none");
        std::string solveMathExpr(std::string exprs);
        std::regex rfunc, rbody, rtimes, rif, rwhen;
        std::string strExpConcat(std::string str);
        std::string replaceVarExp(std::string str);
        void varSet(std::string var, auto what);
        std::string varGet(std::string var);
        Runner();
        bool varExists(std::string arg);
        void dump();
        void purge(bool everything = true);
        void run(std::string code);
        void exec(Program program, std::vector<std::string> args = {});
    private:
        std::map<std::string,std::string> var;
        std::map<std::string,Program> modules;

        void parseStructs(std::string code);
};

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}
