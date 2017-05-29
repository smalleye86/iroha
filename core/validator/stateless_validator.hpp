/**
 * Copyright 2016 Soramitsu Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <functional>
#include <transaction_generated.h>

namespace sumeragi { class Block; }

namespace validator {
namespace stateless {

template <typename T>
using validator_t = std::function<bool(const T&)>;

template <typename T>
class StatelessValidator {
public:
  void addValidator(const validator_t<T>& validator) {
    validators_.push_back(validator);
  }

  bool test(const T& obj) {
    bool res = true;
    for (const auto& verify: validators_) {
      res = res && verify(obj);
    }
    return res;
  }

private:
  std::vector<validator_t<T>> validators_;
};

// Set block dispatcher if validation succeeded.
void receive(std::function<void(const sumeragi::Block&)> dispatcher);

bool test(const sumeragi::Block& block);

} // namespace stateless
} // namespace validator