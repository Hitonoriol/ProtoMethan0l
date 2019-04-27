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
void stripReserved(std::string& arg);
std::string randString(size_t len = 32);
bool isMathExp(std::string arg);
bool contains(std::string str, std::string what);
std::string replaceall(std::string subject, const std::string& search, const std::string& replace);
std::vector<std::string> split(std::string input, std::string regex);
std::vector<std::string> parseList(std::string rawlist);
std::string strim (std::string str);

class Block{
    public:
        int tok, start, bend, parent;
        Block(int tok, int start, int bend, int parent = -1){
            this->tok = tok;
            this->start = start;
            this->bend = bend;
            this->parent = parent;
        }
};

class Blocks{
    public:
        std::vector<Block> blocks;
        void add(int tok, int start, int bend, int parent = -1){
            Block block(tok, start, bend, parent);
            blocks.push_back(block);
        }
        void add(int tok, int start, int parent = -1){
            Block block(tok, start, -1, parent);
            blocks.push_back(block);
        }
        Block getByStart(int start){
            for (size_t i = 0; i < blocks.size(); i++)
                if (blocks[i].start == start)
                    return blocks[i];
        }
        Block getByEnd(int bend){
            for (size_t i = 0; i < blocks.size(); i++)
                if (blocks[i].bend == bend)
                    return blocks[i];
        }
        Block id(int id){
            return blocks[id];
        }
};

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
        void varForceSet(std::string var, auto what);
        bool isReadOnly(std::string var);
        void setReadOnly(std::string var);
        bool funcExists(std::string func);
        void varDel(std::string var);
        bool parseLogicalExpression(std::string str);
        void panic(std::string err = "An error has occurred.");
        std::string getArrayCell(std::string cmd);
        std::vector<std::string> parseList(std::string rawlist);
        std::string escapeStruct(std::string str, std::regex rgx, std::string structname);
        std::string parseBasicExpressions(std::string str, std::string excl = "none");
        std::string solveMathExpr(std::string exprs);
        std::regex rfunc, rbody, rtimes, rif, rwhile, rnest, rsubstr, rarr;
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
        std::vector<std::string> ro;
        std::vector<std::string> packed;
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
