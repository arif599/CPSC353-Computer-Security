#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

char tobase64(char c);
void combine_base64(const char* src, char* dst);
void convert_to_base64(const char* src, char* dst, int n);
void hex_decoded(const char* src, char* dst, int n);
char dec_value(char hex);


int main() {
  cout << "-------------------CHALLENGE 1 -------------------" << endl;

  char hexInput[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  char dec[BUFSIZ];
  char base64Output[BUFSIZ];

  int n = strlen(hexInput);
  hex_decoded(hexInput, dec, n);

  convert_to_base64(dec, base64Output, strlen(dec));

  cout << "Input: " << hexInput << endl;
  cout << endl;
  cout << "Result: " << base64Output << endl;
  
  return 0;
}

// Convert hex string to base64
char tobase64(char c) { 
  if (c < 26) { return c + 65; 
  } else if (c < 52) { return c + 71; 
  } else if (c <= 61) { return c - 4; 
  } else if (c == 62) { return 43; 
  } else { return 47; }
}

// combine bits using shift and OR operation
void combine_base64(const char* src, char* dst) {
  unsigned long combined = (src[0] << 16) | (src[1] << 8) | src[2];
  unsigned mask = 63;   
  
  for (int i = 18, j = 0; i >= 0; i -= 6, ++j) {
    dst[j] = tobase64((combined >> i) & mask);
  }
}

// Convert hex string to base64
void convert_to_base64(const char* src, char* dst, int n) {
  for (int i = 0, j = 0; i < n; i += 3, j += 4) {
    combine_base64(src + i, dst + j);
  }
}

// Gets the decimal value of a hex character
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
