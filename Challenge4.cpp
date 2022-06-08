#include <vector>
#include <iostream>
#include <cstring>
#include <fstream>
#include <iterator>
#include <map>
#include <string>
using namespace std;

int find_sentence(vector<string> text, vector<int> &keyByte, string* sentence, int* key);
void single_xor(vector<string> &text, vector<int> &keyByte, const char* dec, int n);
void hex_decoded(const char* src, char* dst, int n);
char dec_value(char hex);


int main() { 
  cout << "-------------------CHALLENGE 4 -------------------" << endl;
  int globalMax = -1;
  int globalKey = 0;
  string globalHex;
  string globalIndex = "";
  string line = "";

  ifstream file ("4.txt");
  if(file.is_open()){
    while (getline(file,line)){
      vector<string> text;
      vector<int> keyByte; 
      char dec[BUFSIZ];
      char hexsrc[line.length() + 1]; 
      strcpy(hexsrc, line.c_str());
      int n = strnlen(hexsrc, BUFSIZ);

      hex_decoded(hexsrc, dec, n);
      single_xor(text, keyByte, dec, n);
    
      string sentence = "";
      int key = -1;
      int currCount = find_sentence(text, keyByte, &sentence, &key);

      if(currCount > globalMax){
        globalMax = currCount;
        globalIndex = sentence;
        globalKey = key;
        globalHex = hexsrc;
      }
    }
    file.close();
  }

  cout << "Encrypted message: " << globalHex << endl;
  cout << endl;
  cout << "Key: " << globalKey << endl;
  cout << endl;
  cout << "Original message: " << globalIndex << endl;
  return 0;
}

int find_sentence(vector<string> text, vector<int> &keyByte, string* sentence, int* key){
  int maxCount = 0;
  int keyCounter = 0;
  string index = "";
  string letters = "abcedfghijklmnopqrstuvwxyz";
  map<char, float> frequency_english;

  for(int i=0; i<letters.length(); i++){
    frequency_english[letters[i]] = 1;
  }

  for (vector<string>::iterator it = text.begin() ; it != text.end(); ++it){
    string sentence = *it;
    int count = 0;
    for(int i=0; i<sentence.length(); i++){
      if (frequency_english.count(sentence[i]) > 0){
        count += 1;
      } 
    }
    if(count > maxCount){
        maxCount = count;
        index = sentence;
        *key = keyByte[keyCounter];
    }

    keyCounter += 1;
  }
  *sentence = index;
  return maxCount;
}

void single_xor(vector<string> &text, vector<int> &keyByte, const char* dec, int n){
  string temp = "";
  for (unsigned char k = 0; k < 255; k++){
    for(int i = 0; i<n/2; i++){
      temp += (dec[i] ^ k);
    }
    keyByte.push_back(k);
    text.push_back(temp);
    temp = "";
  }
}

char dec_value(char hex) {  
  if (!isxdigit(hex)) { throw new std::invalid_argument("c is not a valid hex character\n"); }

  char value = 0;
  hex = toupper(hex);
  if ('A' <= hex && hex <= 'F') { value = hex - 'A' + 10; }
  else { value = hex - '0'; }

  return value;
}

void hex_decoded(const char* src, char* dst, int n){
  for (int i = 0, j = 0; i < n; i += 2, ++j) { 
    dst[j] = dec_value(src[i]) << 4 | dec_value(src[i + 1]); 
  }
}




