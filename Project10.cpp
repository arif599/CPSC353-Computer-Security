#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "utils.cpp"


int main(){
    string filename = "10.txt";
    string lines;
    ifstream file(filename);

    while(!file.fail()){
        string line;
        file >> line;
        if(file.eof()){
            break;
        }
        copy(begin(line), end(line), back_inserter(lines));
    }

    byte_vector convertedLines = base64Decode(lines);
    string key = "YELLOW SUBMARINE"s;
    byte_vector convertedKey = convertToBytes(key);
    byte_vector dec = aes_cbc_decrypt(convertedLines, convertedKey, zero_iv());
    string str = convertToStr(dec);
    cout << "Decrypted String: " << str << endl;

    return 0;
}

