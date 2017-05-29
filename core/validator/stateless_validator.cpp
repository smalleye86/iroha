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

#include <flatbuffers/flatbuffers.h>
#include <consensus/block_builder.hpp>
#include <functional>
#include <transaction_generated.h>

#include "stateless_validator.hpp"

namespace validator {
namespace stateless {

std::function<void(const sumeragi::Block&)> dispatchToSumeragi;

/**
 * receive(block)
 * @brief sets block dispatcher if validation succeeded.
 * @param block
 */
void receive(std::function<void(const sumeragi::Block&)> dispatcher) {
  dispatchToSumeragi = dispatcher;
}

void StatelessValidator::addValidator(const validator_t& validator) {
  validators_.push_back(validator);
}

bool StatelessValidator::test(const sumeragi::Block& block) {
  bool res = true;
  for (const auto& verify: validators_) {
    res = res && verify(block);
  }
  return res;
}

bool test(const sumeragi::Block& block) {

  StatelessValidator validator;

  validator.addValidator([](const sumeragi::Block& block) -> bool {
    for (auto const& tx: block.txs) {
      flatbuffers::Verifier verifier(tx.data(), tx.size());
      if (!verifier.VerifyBuffer<protocol::Transaction>()) {
        return false;
      }
    }
    return true;
  });

  validator.addValidator([](const sumeragi::Block& block) -> bool {
    return block.created <= datetime::unixtime();
  });

  auto valid = validator.test(block);

  if (valid) {
    dispatchToSumeragi(block);
    return true;
  }
  return false;
}

} // namespace stateless
} // namespace validator