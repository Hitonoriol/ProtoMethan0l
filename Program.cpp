#include "headers/Methan0l.h"

Program::Program(std::string contents){
    this->contents = contents;
    this->queueExec();
}

void Program::setCont(std::string arg){
    this->contents = arg;
    this->queueExec();
}

void Program::queueExec(){
    std::stringstream tstream(this->contents);
    std::string buf;
    while(getline(tstream, buf, ';')) {
        trim(buf);
        this->queued.push_back(buf);
    }
}
