#include "headers/Methan0l.h"

Program::Program(std::string contents){
    setCont(contents);
}

void Program::setArgNames(std::vector<std::string> argsn){
    this->argn = argsn;
}

void Program::setCont(std::string arg){
    this->contents = arg;
    this->queueExec();
}

void Program::replaceLiterals(std::string *where) {
    std::regex elt("\\{s(.*?)\\}");
    std::smatch lm;
    int pos;
    while (std::regex_search(*where, lm, elt)) {
            pos = std::atoi(lm.str(1).c_str());
            where->erase(lm.position(), lm.length());
            where->insert(lm.position(), lts[pos]);
    }
}

void Program::queueExec(){
    std::stringstream tstream(this->contents);
    std::string buf;
    std::regex lit("\\\"(.*?)\\\"");
    std::smatch mt;
    int z = 0;
    while (std::regex_search(contents, mt, lit)) {
        lts.push_back("\""+mt.str(1)+"\"");
        contents.erase(mt.position(), mt.length());
        contents.insert(mt.position(), "{s"+std::to_string(z)+"}");
    }
    auto lbuf = split(contents,"\\;");
    for (auto line : lbuf){
        replaceLiterals(&line);
        this->queued.push_back(line);
    }
}
