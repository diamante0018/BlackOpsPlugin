#include "string.hpp"
#include "cryptography.hpp"

namespace jenkins_one_at_a_time {
unsigned int jenkins_one_at_a_time::compute(const std::string& data) {
  return compute(data.data(), data.size());
}

unsigned int jenkins_one_at_a_time::compute(const char* key, const size_t len) {
  unsigned int hash, i;
  for (hash = i = 0; i < len; ++i) {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash;
}
} // namespace jenkins_one_at_a_time
