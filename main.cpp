#include <iostream>
#include <fstream>
#include <string>

#define DEBUG

int readFilenamesFromStdIO(const int argc, char* argv[],
                           std::ifstream& fin, std::ofstream& fout,
                           const std::string& ifkeyword, const std::string& ofkeyword) {
#ifdef DEBUG
    std::cout << "DEBUG: argc = " << argc << "; argv = [\n" << argv[0];
    for (size_t i = 1; i < argc; ++i)
        std::cout << ", " << argv[i];
    std::cout << ']' << std::endl;
#endif

    for (size_t i = 0, ofpos = std::string::npos, ifpos = std::string::npos; i < argc-1; ++i) {
        std::string buff = argv[argc - 1 - i];

        if (ifpos == std::string::npos) {
            ifpos = buff.find(ifkeyword + '=');
            if (ifpos != std::string::npos && ifpos == 0) {
                fin.open(buff.substr(ifkeyword.length() + 1));
                if (!fin.is_open())
                    ifpos = std::string::npos;
#ifdef DEBUG
                std::cout << "DEBUG: input filename - " << buff.substr(ifkeyword.length() + 1) << std::endl;
#endif
            }
        }

        if (ofpos == std::string::npos) {
            ofpos = buff.find(ofkeyword + '=');
            if (ofpos != std::string::npos && ofpos == 0) {
                fout.open(buff.substr(ofkeyword.length() + 1));
                if (!fout.is_open())
                    ofpos = std::string::npos;
#ifdef DEBUG
                std::cout << "DEBUG: output filename - " << buff.substr(ofkeyword.length() + 1) << std::endl;
#endif
            }
        }

        if (ofpos != std::string::npos && ifpos != std::string::npos)
            break;
    }

    if (!fin.is_open() || !fout.is_open()) {
#ifdef DEBUG
        std::cout << "DEBUG: " << (!fin.is_open() ?
                                   ((!fout.is_open() ?
                                     "input and output files" :
                                     "input file")) :
                                   "output file")
                << " not opened" << std::endl;
#endif
        return 1;
    }

    return 0;
}

using namespace std;

/**
 * options:
 * if=%filename% - set input file
 * of=%filename% - set output file
 * last option has higher priority (if it valueable)
 */

int main(int argc, char* argv[]) {
    ifstream fin;
    ofstream fout;
    int errCode = readFilenamesFromStdIO(argc, argv, fin, fout, "if", "of");
    if (errCode) return errCode;


    return 0;
}