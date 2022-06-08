#include <iostream>
#include <vector>
#include <iostream>
#include <cstring>
#include <iterator>
#include <map>
using namespace std;

void find_sentence(vector<string> text, vector<int> &keyByte, string* sentence, int* key);
void single_xor(vector<string> &text, vector<int> &keyByte, const char* dec, int n);
void hex_decoded(const char* src, char* dst, int n);
char dec_value(char hex);
void block_xor(const char* src1, const char* src2, char* dst, int n);
char hex_char(char value);


int main() { 
  cout << "-------------------CHALLENGE 3 -------------------" << endl;
  char hexsrc[] = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  char dec[BUFSIZ];
  int n = strnlen(hexsrc, BUFSIZ);
  vector<string> text;
  vector<int> keyByte;

  hex_decoded(hexsrc, dec, n);
  single_xor(text, keyByte, dec, n);

  string sentence = "";
  int key = -1;
  find_sentence(text, keyByte, &sentence, &key);

  cout << "Encrypted message: " << hexsrc << endl;
  cout << endl;
  cout << "Key: " << key << endl;
  cout << endl;
  cout << "Original message: " << sentence << endl;

  return 0;
}

// Find the sentence with the highest score
void find_sentence(vector<string> text, vector<int> &keyByte, string* sentence, int* key){
  float maxCount = 0;
  int keyCounter = 0;
  string index = "";
  // frequency of english characters
  string letters = "abcedfghijklmnopqrstuvwxyz";
  map<char, float> frequency_english;
  for(int i=0; i<letters.length(); i++){
    frequency_english[letters[i]] = 1;
  }

  // calculate the frequency of each character
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
}

// Perform single-byte XOR on the given string
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

// Getting the decimal value of a hex character
char dec_value(char hex) {  
  if (!isxdigit(hex)) { throw new std::invalid_argument("c is not a valid hex character\n"); }

  char value = 0;
  hex = toupper(hex);
  if ('A' <= hex && hex <= 'F') { value = hex - 'A' + 10; }
  else { value = hex - '0'; }

  return value;
}

// Convert hex string to integer
void hex_decoded(const char* src, char* dst, int n) {
    for (int i = 0, j = 0; i < n; i += 2, ++j) { 
      dst[j] = dec_value(src[i]) << 4 | dec_value(src[i + 1]); 
    }
}

// Getting the hex value of a character
char hex_char(char value) {
  if (value > 15) { throw new std::invalid_argument("dec is not a valid hex value"); }
  if (value < 10) { return '0' + value; }
  else { return 'A' + value - 10; }
}
