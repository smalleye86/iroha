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

#include <flatbuffers/flatbuffers.h>
#include <main_generated.h> // pack()

namespace sumeragi {

flatbuffers::Offset<protocol::Signature> Signature::packOffset(
  flatbuffers::FlatBufferBuilder &fbb) const {
  return protocol::CreateSignature(
    fbb, fbb.CreateVector(publicKey), fbb.CreateVector(signature)
  );
}

std::vector<uint8_t> Block::pack() const {
  flatbuffers::FlatBufferBuilder fbb;

  // Create tx wrapper offset vector
  std::vector<flatbuffers::Offset<protocol::TransactionWrapper>> txs_o;
  for (const auto& tx: txs) {
    auto root = flatbuffers::GetRoot<protocol::TransactionWrapper>(tx.data());
    std::vector<uint8_t> txbuf(root->tx()->begin(), root->tx()->end());
    txs_o.push_back(
      protocol::CreateTransactionWrapper(fbb, fbb.CreateVector(txbuf))
    );
  }

  // Create peer sigs offset vector
  std::vector<flatbuffers::Offset<protocol::Signature>> peer_sigs_o;
  for (const auto& sig: peer_sigs) {
    peer_sigs_o.push_back(sig.packOffset(fbb));
  }

  auto block_o = protocol::CreateBlock(
    fbb, fbb.CreateVector(txs_o),
    fbb.CreateVector(peer_sigs_o),
    /* prev_hash */ nullptr,
    /* length */ 0,
    /* merkle_root */ nullptr,
    /* height */ 0,
    datetime::unixtime(),
    protocol::BlockState::UNCOMMITTED
  );

  fbb.Finish(block_o);

  auto buf = fbb.GetBufferPointer();
  return {buf, buf + fbb.GetSize()};
}

BlockBuilder::BlockBuilder() {}
BlockBuilder& BlockBuilder::setTxs(const std::vector<std::vector<uint8_t>>& txs)
{
  if (txs.size() >= MaxTxs)
    throw std::runtime_error("overflow txs.size");
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
    throw std::runtime_error("not initialized with setBlock()");
  if (buildStatus_ & BuildStatus::withSignature)
    throw std::runtime_error("duplicate addSignature()");
  peer_sigs_.push_back(sig);
  return *this;
}

Block BlockBuilder::build() {

  if (buildStatus_ & BuildStatus::initWithTxs) {
    if (buildStatus_ & BuildStatus::completeFromTxs != buildStatus_) {
      throw std::runtime_error("not completed init with txs");
    }
    return Block {txs_, {}, datetime::unixtime(), BlockState::uncommitted};
  }
  else if (buildStatus_ & BuildStatus::initWithBlock) {
    if (buildStatus_ & BuildStatus::completeFromBlock != buildStatus_) {
      throw std::runtime_error("not completed init with block");
    }
    return block_;
  }
  else {
    throw std::runtime_error("not completed any build");
  }
}

Block BlockBuilder::buildCommit() {
  if (buildStatus_ & BuildStatus::initWithBlock) {
    if (buildStatus_ != BuildStatus::commit) {
      throw std::runtime_error("");
    }
    block_.state = BlockState::committed;
    return block_;
  }
}

}  // namespace sumeragi
