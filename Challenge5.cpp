#include <iostream>
#include <cstring>
#include <iomanip>
#include <sstream>
using namespace std;


void single_repeating_xor(const char* src, char* dst, int n);
void byte_decoded(const char* src, char* dst, int n);
char dec_value(char hex);
void hex_encoded(const char* src, char* dst, int n);


int main() { 
  cout << "-------------------CHALLENGE 5 -------------------" << endl;
  char src[] = R"(Burning 'em, if you ain't quick and nimble
I go crazy when I hear a cymbal)";
  char result[BUFSIZ];
  int n = strnlen(src, BUFSIZ);
  byte_decoded(src, result, n);
  single_repeating_xor(src, result, n);
  char hexResult[BUFSIZ];
  hex_encoded(result, hexResult, n);

  cout << "Message: " << src << endl;
  cout << endl;
  cout << "Hex output: " << hexResult << endl;

}

// single repeating xor usin the key ICE
void single_repeating_xor(const char* src, char* dst, int n){
  char repeatingKey[] = "ICE";
  int repeatingCounter = 0;

  for (int i = 0; i < n; i++) { 
    if(repeatingCounter == 3){
      repeatingCounter = 0;
    }
    dst[i] = int(src[i]) ^ int(repeatingKey[repeatingCounter]);
    repeatingCounter++;
  }
}

// Getting the decimal value of a hex character
char dec_value(char hex) {  
  if (!isxdigit(hex)) { throw new std::invalid_argument("c is not a valid hex character\n"); }

  char value = 0;
  hex = toupper(hex);
  if ('A' <= hex && hex <= 'F') { value = hex - 'A' + 10; }
  else { value = hex - '0'; }

  return value;
}

// converting string to bytes
void byte_decoded(const char* src, char* dst, int n){
    for (int i = 0, j = 0; i < n; i += 2, ++j) { 
    dst[j] = int(src[i]) << 8 | int(src[i + 1]); 
  }
}

// converting bytes to hex
void hex_encoded(const char* src, char* dst, int n){
  string temp = "";
  
  for(int i=0; i<n; i++){
    stringstream sstream;
    sstream << hex << int(src[i]);
    string result = sstream.str();
    temp += result;
  }
  strcpy(dst, temp.c_str());
}


