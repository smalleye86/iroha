#include "random.hpp"

namespace random_service {

std::random_device seed_gen;
std::mt19937    rnd_gen_32(seed_gen());
std::mt19937_64 rnd_gen_64(seed_gen());

std::string makeHashByMT19937() {
  return hash::sha3_256_hex(std::to_string(rnd_gen_32()));
}

// return random value of [min, max]
int32_t random_value_32(int32_t min, int32_t max) {
  assert(min <= max);
  std::uniform_int_distribution<int32_t> dist(min, max);
  return dist(rnd_gen_32);
}

// return random value of [min, max]
uint64_t random_value_64(uint64_t min, uint64_t max) {
  assert(min <= max);
  std::uniform_int_distribution<uint64_t> dist(min, max);
  return dist(rnd_gen_64);
}

};  // namespace random_service
