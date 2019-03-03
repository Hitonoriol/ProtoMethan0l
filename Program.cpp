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

void Program::queueExec(){
    std::stringstream tstream(this->contents);
    std::string buf;
    auto lbuf = split(contents,"\\;");
    for (auto line : lbuf){
        this->queued.push_back(line);
    }
}
