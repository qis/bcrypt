#include <bcrypt.hpp>
#include <array>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <cstdint>

extern "C" {
#include "crypt_blowfish.h"
};

namespace bcrypt {
namespace {

constexpr int part_info_size = 7;
constexpr int part_salt_size = 22;
constexpr int part_hash_size = 31;

constexpr int salt_size = part_info_size + part_salt_size;
constexpr int hash_size = part_info_size + part_salt_size + part_hash_size;

}  // namespace

std::string hash(const std::string& pass, unsigned long rounds)
{
  // Generate random bytes.
  std::array<char, salt_size + 1> rand;
  static thread_local std::random_device rd;
  static thread_local std::uniform_int_distribution<int> dist(
    std::numeric_limits<char>::min(), std::numeric_limits<char>::max());
  for (size_t i = 0; i < salt_size + 1; i++) {
    rand[i] = dist(rd);
  }

  // Generate salt.
  std::array<char, salt_size + 1> salt;
  if (!_crypt_gensalt_blowfish_rn("$2b$", rounds, rand.data(), salt_size + 1, &salt[0], salt_size + 1))
  {
    throw std::runtime_error("password salt error");
  }

  // Generate hash.
  std::string hash;
  hash.resize(hash_size + 1);
  if (!_crypt_blowfish_rn(pass.data(), salt.data(), &hash[0], hash_size + 1)) {
    throw std::runtime_error("password hash error");
  }
  hash.resize(hash_size);
  return hash;
}

bool verify(const std::string& pass, const std::string& hash) noexcept
{
  // Generate hash.
  std::array<char, hash_size + 1> test;
  if (!_crypt_blowfish_rn(pass.data(), hash.data(), test.data(), hash_size + 1)) {
    return false;
  }

  // Compare supplied hash to the generated hash.
  return hash == std::string_view(test.data(), hash_size);
}

}  // namespace bcrypt
