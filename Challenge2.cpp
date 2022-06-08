#include <iostream>
#include <cstring>
using namespace std;

void hex_decoded(const char* src, char* dst, int n);
char dec_value(char hex);
void block_xor(const char* src1, const char* src2, char* dst, int n);
char hex_char(char value);
void masking(const char* xor_dst, char *result, int n);

int main() { 
  cout << "-------------------CHALLENGE 2 -------------------" << endl;
  char src1[] = "1c0111001f010100061a024b53535009181c";
  char src2[] = "686974207468652062756c6c277320657965";
  char hexsrc1[BUFSIZ];
  char hexsrc2[BUFSIZ];
  char result[BUFSIZ];
  char xor_dst[BUFSIZ];
  int n = strnlen(src1, BUFSIZ);

  hex_decoded(src1, hexsrc1, strnlen(src1, BUFSIZ));
  hex_decoded(src2, hexsrc2, strnlen(src2, BUFSIZ));
  block_xor(hexsrc1, hexsrc2, xor_dst, n);
  masking(xor_dst, result, n);
  
  cout << "Input 1: " << src1 << endl;
  cout << "Input 2: " << src2 << endl;
  cout << endl;
  cout << "Result: " << result << endl;


  return 0;
}

// Using masking to extract the hex chars
void masking(const char* xor_dst, char *result, int n){
  int mask = 15;                                      
  for (int i = 0; i < n/2; ++i) { 

    result[2 * i] = hex_char((xor_dst[i] >> 4) & mask);     
    result[2 * i + 1] = hex_char(xor_dst[i] & mask);
  }
}

// XORing two blocks of bytes
void block_xor(const char* src1, const char* src2, char* dst, int n) {
  for (int i = 0; i < n; ++i) { 
    dst[i] = (char)((unsigned)src1[i] ^ (unsigned)src2[i]);
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
