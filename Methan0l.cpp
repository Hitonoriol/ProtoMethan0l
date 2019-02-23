#include "headers/Methan0l.h"

int main(int argc, char *argv[]){
std::string in;
Runner methan0l;
if (argc > 1){
    methan0l.run(Files::readFile(str(argv[1])));
    getline(std::cin, in);
    exit(0);
} else {
    while(true){
        std::cout<<"Methan0l::";
        getline(std::cin, in);
        if (containsAll(in, {"(",")","[","]","entry"}))
            methan0l.run(in);
        else
            methan0l.run("(entry)[" + in + ";]");
        methan0l.purge(false);
    }
}
}
