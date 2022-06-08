#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <cstring>
#include "utils.cpp"
using namespace std;

int main(){
    string allLines;
    string input = "7.txt";
    int blockSize = 16;
    ifstream file(input);

    while(!file.fail()){
        string line;
        file >> line;
        if(file.eof()){
            break;
        }else{
            copy(begin(line), end(line), std::back_inserter(allLines));
        }
    }

    byte_vector byteLines = base64Decode(allLines);
    string key = "YELLOW SUBMARINE"s;
    byte_vector keyBytes = convertToBytes(key);
    byte_vector convertedLines = aes_ecb_decrypt(byteLines, keyBytes);

    string convertedString = convertToStr(convertedLines);
    cout << "Converted String: " << convertedString << endl;
    return 0;
}

