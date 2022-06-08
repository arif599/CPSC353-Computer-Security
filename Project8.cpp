#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <string.h>
#include <cstring>
#include "utils.cpp"
using namespace std;

int main() {
    string input = "8.txt"s;
    ifstream file(input);
    int i = 0;
    
    while(!file.fail()){
        string line;
        file >> line;

        if (file.eof()) {
            break;
        }else{
            if (detect_ecb(hex_to_bytes(line))) {
                cout << "ECB encripyted ciphertext: " << endl;
                cout << line << endl;
            }
        }
        i += 1;
    }

    return 0;
}

