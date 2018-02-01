#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "Salsa20.h"

#define DEBUG

int set_io(int argc, char* argv[],
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
            if (ifpos == 0) {
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
            if (ofpos == 0) {
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

int set_key(int argc, char* argv[], std::string& keybuffer, const std::string& keyword) {
#ifdef DEBUG
    std::cout << "DEBUG: argc = " << argc << "; argv = [\n" << argv[0];
    for (size_t i = 1; i < argc; ++i)
        std::cout << ", " << argv[i];
    std::cout << ']' << std::endl;
#endif

    for (size_t i = 0, keypos = std::string::npos; i < argc-1; ++i) {
        std::string buff = argv[argc - 1 - i];

        if (keypos == std::string::npos) {
            keypos = buff.find(keyword + '=');
            if (keypos == 0) {
                keybuffer = buff.substr(keyword.length() + 1);
                if (keybuffer.length() != 16 && keybuffer.length() != 32) {
                    keybuffer.clear();
                    keypos = std::string::npos;
                }
#ifdef DEBUG
                std::cout << "DEBUG: input filename - " << buff.substr(keyword.length() + 1) << std::endl;
#endif
            }
        } else break;
    }

    if (!keybuffer.length()) {
#ifdef DEBUG
        std::cout << "DEBUG: key not set" << std::endl;
#endif
        return 1;
    }

    return 0;
}

using namespace std;

/**
 * options:
 * [must have]
 * if=%filename% - set input file
 * of=%filename% - set output file
 * key=%key% - set key (16 or 32 byte)
 * last option has higher priority (if it valueable)
 */

int testF(int argc, char* argv[]) {
    ifstream fin;
    ofstream fout;
    int errCode = 0;
    std::string key;

    errCode = set_io(argc, argv, fin, fout, "if", "of") || set_key(argc, argv, key, "key");

    if (errCode) return errCode;

    uint8_t *buffer = new uint8_t[Salsa20::CHUNK_SIZE];

    do {
        fin.read(reinterpret_cast<char *>(buffer), Salsa20::CHUNK_SIZE * sizeof(uint8_t));
        size_t read_bytes = fin.gcount();
        uint8_t nonce[8] = {
                'a', 'b', 'c', 'd',
                'a', 'b', 'c', 'd'
        };
        if (key.length() == 16)
            Salsa20::crypt16(reinterpret_cast<const uint8_t *>(key.c_str()), nonce, buffer);
        else
            Salsa20::crypt32(reinterpret_cast<const uint8_t *>(key.c_str()), nonce, buffer);
        fout.write(reinterpret_cast<char *>(buffer), read_bytes * sizeof(uint8_t));
    } while (!fin.eof());

    delete[] buffer;

    return 0;
}

int main(int argc, char* argv[]) {
    int errCode = testF(argc, argv);
    return errCode;
}