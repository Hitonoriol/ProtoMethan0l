#include <iostream>
#include <string>
#include <stack>
#include <regex>
#include <vector>
#include <utility>
#include <functional>
#include <map>
#include <sstream>
#include <algorithm>
#include <functional>
#include <fstream>
#include <random>
#include <ctime>
#include <set>
#include "Str.h"

bool containsAll(std::string str, std::vector<std::string> wlist);
std::string randString(size_t len = 32);
bool isMathExp(std::string arg);
bool contains(std::string str, std::string what);
std::string replaceall(std::string subject, const std::string& search, const std::string& replace);
std::vector<std::string> split(std::string input, std::string regex);
std::vector<std::string> parseList(std::string rawlist);
std::string strim (std::string str);

class Files{
    public:
        static std::string readFile(std::string path);
        static bool writeFile(std::string filename, std::string arg);
};

class Program{
    public:
    std::string contents;
    int args;
    std::vector<std::string> queued;
    std::vector<std::string> argn;
        void setArgNames(std::vector<std::string> argsn);
        Program(std::string contents="nop;");
        void queueExec();
        void setCont(std::string arg);
    private:
};

class Runner{
    public:
        typedef exprtk::symbol_table<double> symbol_table_t;
        typedef exprtk::expression<double>     expression_t;
        typedef exprtk::expression<double>     logicalexpr_t;
        typedef exprtk::parser<double>             parser_t;
        parser_t mathparser;
        parser_t logicparser;
        bool funcExists(std::string func);
        bool parseLogicalExpression(std::string str);
        std::string escapeStructs(std::string str, std::regex rgx, std::string structname);
        std::string parseBasicExpressions(std::string str, std::string excl = "none");
        std::string solveMathExpr(std::string exprs);
        std::regex rfunc, rbody, rtimes, rif, rwhile;
        std::string strExpConcat(std::string str);
        std::string replaceVarExp(std::string str, bool ifs = false);
        void varSet(std::string var, auto what);
        std::string varGet(std::string var);
        Runner();
        bool varExists(std::string arg);
        void dump();
        void purge(bool everything = true);
        void run(std::string code);
        void exec(Program program, std::vector<std::string> args = {});
    private:
        int floatprecision = 6;
        std::string ret;
        varlist_t var;
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
