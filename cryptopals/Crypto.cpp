

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <random>
#include <stdexcept>
#include <vector>
#include <random>

#include "MT19937.h"
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

typedef unsigned char byte;

class bytevector {
public:
  typedef std::vector<byte>::iterator iterator;
  typedef std::vector<byte>::const_iterator const_iterator;

  enum bv_mode {
    HEX,
    BASE64,
    ASCII,
    PLAIN
  };

  bytevector();
  bytevector(size_t count, byte value);
  bytevector(const std::string &s, bv_mode mode);
  bytevector(std::ifstream& is, bv_mode mode=BASE64);

  template <class T>
  explicit bytevector(T input)
  {
    static_assert(std::is_integral<T>::value, 
                  "bytevector(T input) requires integral type");
    for (unsigned i = 0; i < sizeof(T); i++) {
      byte b = static_cast<byte>((input >> (8 * i)) & 0xFF);
      data_.push_back(b);
    }
  }

  template <class InputIt>
  bytevector(InputIt first, InputIt last) 
    : data_(first, last)
  {
  }

  std::string to_string(bv_mode mode) const;
  uint32_t to_uint32() const;

  void resize(size_t size, byte value = 0); 
  size_t size() const;
  void push_back(byte value);
  byte *data();

  template <class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last)
  {
    return data_.insert(pos, first, last); 
  }

  template <class InputIt>
  void insert(iterator pos, InputIt first, InputIt last)
  {
    data_.insert(pos, first, last); 
  }

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;

  bytevector operator ^=(const bytevector &other);
  bytevector operator +=(const bytevector &other);
  bytevector operator +=(char c);

  byte& operator [](size_t idx) {
    return data_[idx];
  }

  byte operator [](size_t idx) const {
    return data_[idx];
  }

  friend bool operator ==(const bytevector &lhs, const bytevector &rhs)
  {
    return lhs.data_ == rhs.data_;
  }

  friend bytevector operator ^(bytevector lhs, const bytevector &rhs) {
    return lhs ^= rhs;
  }

  friend bytevector operator +(bytevector lhs, const bytevector &rhs) {
    return lhs += rhs;
  }

  friend bytevector operator +(bytevector lhs, char rhs) {
    return lhs += rhs;
  }

  void pad_to_length(size_t new_length);
  void pad_to_block(size_t blocksize);
  bool check_padding() const;
  void strip_padding();

  void repeating_key_xor(bytevector key);
  std::vector<bytevector> split_into_blocks(size_t blocksize) const;

private:
  std::vector<byte> data_;
};

std::ostream& operator<<(std::ostream& os, bytevector b);

// =====================

std::vector<bytevector> transpose(std::vector<bytevector> input);

std::array<double, 27> letter_frequencies(bytevector b);
bytevector solve_repeating_key_xor(bytevector ciphertext, size_t key_size);
int hamming_distance(bytevector a, bytevector b);
double squared_error(std::array<double, 27> freq);
#include "MT19937.h"
class Crypto {
public:
  Crypto();
  ~Crypto();

  void handle_errors();
  bytevector encrypt_ecb(bytevector plaintext, bytevector key, bool pad);
  bytevector decrypt_ecb(bytevector ciphertext, bytevector key, bool pad);

  bytevector encrypt_cbc(bytevector plaintext, bytevector key, bytevector iv);
  bytevector decrypt_cbc(bytevector ciphertext, bytevector key, bytevector iv);

  bytevector encrypt_ctr(bytevector plaintext, bytevector key, uint64_t nonce);
  bytevector decrypt_ctr(bytevector ciphertext, bytevector key, uint64_t nonce);

  bytevector encrypt_mt(bytevector plaintext, uint16_t key);
  bytevector decrypt_mt(bytevector ciphertext, uint16_t key);
private:
  bytevector do_ctr(bytevector input, bytevector key, uint64_t nonce);
  bytevector do_mt(bytevector input, uint16_t seed);

  unsigned instances_;
};

bytevector random_bytevector(size_t length=16);
bytevector random_string(size_t min, size_t max);
bytevector mt19937_token(size_t length, uint32_t seed);
using std::invalid_argument;
using std::vector;

namespace {
  std::random_device rd;
  std::mt19937 gen(rd());
}

Crypto::Crypto()
{
  if (instances_++ == 0) {
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();
    OPENSSL_config(NULL);
  }
}

Crypto::~Crypto()
{
  if (--instances_ == 0) {
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
  }
}

void Crypto::handle_errors()
{
  ERR_print_errors_fp(stderr);
}

bytevector Crypto::encrypt_ecb(bytevector data, bytevector key, bool pad)
{
  if (key.size() != 16) {
    throw invalid_argument("encrypt_ecb: key must have length 16");
  }

  byte ciphertext[data.size() + 16];
  byte plaintext[data.size()];
  int i = 0;
  for (byte b : data) {
    plaintext[i++] = b;
  }

  int len;
  int ciphertext_len;
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

  if (ctx == nullptr) handle_errors();

  if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key.data(), NULL) != 1)
    handle_errors();
  if (!pad) EVP_CIPHER_CTX_set_padding(ctx, 0);

  if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, data.size()) != 1)
    handle_errors();
  ciphertext_len = len;

  /* Finalise the encryption. Further ciphertext bytes may be written at
   * this stage.
   */
  if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1)
    handle_errors();
  ciphertext_len += len;

  EVP_CIPHER_CTX_free(ctx);

  bytevector output;
  for (int i = 0; i < ciphertext_len; i++) {
    output += ciphertext[i];
  }
  return output;
}

bytevector Crypto::decrypt_ecb(bytevector data, bytevector key, bool pad)
{
  if (key.size() != 16) {
    throw invalid_argument("decrypt_ecb: key must have length 16");
  }

  byte ciphertext[data.size()];
  byte plaintext[data.size()];
  int i = 0;
  for (byte b : data) {
    ciphertext[i++] = b;
  }

  int len;
  int plaintext_len;
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

  if (ctx == nullptr) {
    handle_errors();
    return bytevector();
  }

  if (EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key.data(), NULL) != 1)
    handle_errors();
  if (!pad) EVP_CIPHER_CTX_set_padding(ctx, 0);

  if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, data.size()) != 1)
    handle_errors();

  plaintext_len = len;

  /* Finalise the decryption. Further plaintext bytes may be written at
   * this stage.
   */
  if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1)
    handle_errors();
  plaintext_len += len;

  EVP_CIPHER_CTX_free(ctx);

  bytevector output;
  for (int i = 0; i < plaintext_len; i++) {
    output += plaintext[i];
  }

  return output;
}

bytevector Crypto::encrypt_cbc(
    bytevector plaintext, bytevector key, bytevector iv)
{
  if (!plaintext.check_padding()) {
    throw invalid_argument(
        "encrypt_cbc: input to encrypt_cbc must be padded to 16 bytes");
  }

  if (key.size() != 16) {
    throw invalid_argument("encrypt_cbc: key must have length 16");
  }

  vector<bytevector> blocks = plaintext.split_into_blocks(16);
  bytevector ciphertext;
  bytevector prev_block = iv;

  for (const bytevector &block : blocks) {
    bytevector cipher_block = encrypt_ecb(block ^ prev_block, key, false);
    ciphertext += cipher_block;
    prev_block = cipher_block;
  }
  return ciphertext;
}

bytevector Crypto::decrypt_cbc(
    bytevector ciphertext, bytevector key, bytevector iv)
{
  if (key.size() != 16) {
    throw invalid_argument("decrypt_cbc: key must have length 16");
  }

  vector<bytevector> blocks = ciphertext.split_into_blocks(16);
  bytevector plaintext;
  bytevector prev_block = iv;

  for (const bytevector &block : blocks) {
    bytevector plain_block = decrypt_ecb(block, key, false);
    plaintext += (plain_block ^ prev_block);
    prev_block = block;
  }
  return plaintext;
}

bytevector Crypto::do_ctr(bytevector input, bytevector key, uint64_t nonceInt)
{
  bytevector nonce(nonceInt);
  uint64_t block_counter = 0;
  vector<bytevector> blocks = input.split_into_blocks(16);

  bytevector output;
  for (const bytevector &block : blocks) {
    bytevector key_block = nonce + bytevector(block_counter);
    bytevector key_stream = encrypt_ecb(key_block, key, false);
    key_stream.resize(block.size());
    output += key_stream ^ block;
    block_counter++;
  }
  return output;
}

bytevector Crypto::encrypt_ctr(
    bytevector plaintext, bytevector key, uint64_t nonce)
{
  if (key.size() != 16) {
    throw invalid_argument("encrypt_ctr: key must have length 16");
  }

  return do_ctr(plaintext, key, nonce);
}

bytevector Crypto::decrypt_ctr(
    bytevector ciphertext, bytevector key, uint64_t nonce)
{
  if (key.size() != 16) {
    throw invalid_argument("decrypt_ctr: key must have length 16");
  }

  return do_ctr(ciphertext, key, nonce);
}

bytevector Crypto::do_mt(bytevector input, uint16_t key)
{
  MT19937 m(static_cast<uint32_t>(key));
  bytevector output;
  vector<bytevector> blocks = input.split_into_blocks(4);
  for (const bytevector &block : blocks) {
    bytevector ks = bytevector(m());
    ks.resize(block.size());
    output += block ^ ks; 
  }
  return output;
}

bytevector Crypto::encrypt_mt(bytevector plaintext, uint16_t key)
{
  return do_mt(plaintext, key);
}

bytevector Crypto::decrypt_mt(bytevector ciphertext, uint16_t key)
{
  return do_mt(ciphertext, key);
}

bytevector random_bytevector(size_t length)
{
  std::uniform_int_distribution<> dis(0, 255);

  bytevector bv;
  for (size_t i = 0; i < length; i++) {
    bv += dis(gen);
  }
  return bv;
}

bytevector random_string(size_t min, size_t max)
{
  if (min > max) {
    throw invalid_argument("random_string: min must be less than max");
  }

  std::uniform_int_distribution<> random_len(min, max);
  size_t length = random_len(gen);

  std::uniform_int_distribution<> dis(32, 126);
  bytevector s;
  for (size_t i = 0; i < length; i++) {
    s += dis(gen);
  }
  return s;
}

bytevector mt19937_token(size_t length, uint32_t seed)
{
  bytevector output;
  MT19937 m(seed);
  for (size_t i = 0; i < length; i+=4) {
    output += bytevector(m()); 
  }
  output.resize(length);
  return output;
}
