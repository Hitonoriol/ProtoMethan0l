#include "headers/Methan0l.h"

int main(int argc, char *argv[]){
std::string in;
Runner methan0l;
if (argc > 1){
    auto contents = Files::readFile(str(argv[1]));
    stripReserved(contents);
    methan0l.run(contents);
    getline(std::cin, in);
    exit(0);
} else {
    while(true){
        std::cout<<"Methan0l::";
        getline(std::cin, in);
        stripReserved(in);
        if (in == "dump"){
            methan0l.dump();
        }
        else if (isMathExp(in)){
            std::cout<<methan0l.parseBasicExpressions(in);
        }
        else if (containsAll(in, {"(",")","[","]","entry"}))
            methan0l.run(in);
        else
            methan0l.run("(entry)[" + in + ";]");
        methan0l.purge(false);
        std::cout<<std::endl;
    }
}
}
