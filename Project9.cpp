#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include "utils.cpp"


int main() {
    string str = "YELLOW SUBMARINE"s;
    string paddedStr = padded_pkcs7(str, 20);
    cout << "PKCS7 Padded String: " << convertToBytes(paddedStr) << endl;
    return 0;
}

