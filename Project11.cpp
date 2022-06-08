#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "utils.cpp"

byte_vector encrypt(byte_vector data);

int main() {
  byte_vector input(43, 'q'); 
  byte_vector encrypted = encrypt(input);
  vector<byte_vector> blocks = encrypted.getBlocks(16);
  if (blocks[1] == blocks[2]) {
    cout << "ECB" << endl;
  } else {
    cout << "CBC" << endl;
  }
  return 0;
}


byte_vector encrypt(byte_vector data) {
  byte_vector key = random_byte_vector();

  byte_vector original;
  original += random_string(5, 10);
  original += data;
  original += random_string(5, 10);
  original.padding(16);

  byte_vector ciphertext;
  if (rand() % 2) {
    ciphertext = encrypt_ecb(original, key, false);
    cout << "Real:  ECB" << endl;
    cout << ciphertext << endl;
  } else {
    byte_vector cbc = random_byte_vector();
    ciphertext = encrypt_cbc(original, key, cbc);
    cout << "Real:  CBC" << endl;
    cout << ciphertext << endl;
  }

  return ciphertext;
}