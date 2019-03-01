#include "headers/Methan0l.h"

Program::Program(std::string contents){
    this->contents = contents;
    this->args = 0;
    this->queueExec();
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
    auto lbuf = split(contents,"(?![\\w\\s]*[\\\"]);(?!\\\")");
    for (auto line : lbuf){
        trim(line);
        this->queued.push_back(line);
    }
}
