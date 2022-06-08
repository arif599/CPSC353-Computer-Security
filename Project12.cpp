#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "utils.cpp"

int main() {
    string filename = "12.txt";
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

  byte_vector ciphertext = encrypt_ecb(plaintext + line, true);
  
  int block_size;
  int length = ciphertext.size();
  for (int i = 1; i < 128; i++) {
    byte_vector input(i, 'A');
    byte_vector encrypted = encrypt(input);
    if (length < encrypted.size()) {
      block_size = encrypted.size() - length;
      break;
    }
  }
  cout << "Block size: " << block_size << endl;

  byte_vector input2(block_size * 2, 'A');
  byte_vector encrypted = encrypt(input2);
  vector<byte_vector> blocks = encrypted.split_into_blocks(block_size);
  
  cout << "Block mode: ";
  if (blocks[0] == blocks[1]) {
    cout << "ECB" << endl;
  } else {
    cout << "CBC" << endl;
  }

  cout << "Start length: " << length << endl << endl;
  byte_vector decrypted;
  for (unsigned i = 0; i < length; i++) {
    int block_offset = i % block_size;
    int block_index = i / block_size;

    byte_vector padding(16 - block_offset - 1, 'q');
    encrypted = encrypt(padding);
    vector<byte_vector> enc_blocks = encrypted.split_into_blocks(block_size);

    byte b;
    for (b = 10; b <= 126; b++) {
      byte_vector result = encrypt(padding + decrypted + b);
      vector<byte_vector> blocks = result.split_into_blocks(block_size);
      if (blocks[block_index] == enc_blocks[block_index]) {
        decrypted.push_back(b);
        break;
      }
    }
    if (b == 126) break;
  }

  cout << "Decrypted: " << decrypted.to_string() << endl;
  return 0;
}