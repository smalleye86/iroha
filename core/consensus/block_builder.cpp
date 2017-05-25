/**
 * Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <utils/datetime.hpp>
#include "block_builder.hpp"
#include <stdexcept>

namespace sumeragi {

BlockBuilder::BlockBuilder() {}
BlockBuilder& BlockBuilder::setTxs(const std::vector<byte_array_t>& txs)
{
  if (txs.size() >= MaxTxs)
    throw std::runtime_error("");
  txs_ = txs;
  buildStatus_ |= BuildStatus::initWithTxs;
  return *this;
}

BlockBuilder& BlockBuilder::setBlock(const Block& block)
{
  block_ = block;
  buildStatus_ |= BuildStatus::initWithBlock;
  return *this;
}

BlockBuilder& BlockBuilder::addSignature(const Signature& sig) {
  if (buildStatus_ & initWithBlock == 0)
    throw std::runtime_error("");
  if (buildStatus_ & BuildStatus::withSignature)
    throw std::runtime_error("");
  peer_sigs.push_back(sig);
  return *this;
}

Block BlockBuilder::build() {

  if (buildStatus_ & BuildStatus::initWithTxs) {
    if (buildStatus_ != BuildStatus::blockFromTxs) {
      throw std::runtime_error("");
    }
    return Block {txs_, {}, datetime::unixtime(), State::uncommitted};
  }
  else if (buildStatus_ & BuildStatus::initWithBlock) {
    if (buildStatus_ != BuildStatus::blockFromBlock) {
      throw std::runtime_error("");
    }
    return block_;
  }
  else {
    throw std::runtime_error("");
  }
}

Block BlockBuilder::buildCommit() {
  if (buildStatus_ & BuildStatus::initWithBlock) {
    if (buildStatus_ != BuildStatus::commit) {
      throw std::runtime_error("");
    }
    block_.state = State::committed;
    return block_;
  }
}

};
