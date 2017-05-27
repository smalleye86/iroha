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

#include <gtest/gtest.h>
#include <consensus/block_builder.hpp>

using namespace sumeragi;

TEST(block_builder_test, to_block) {

  std::vector<std::vector<uint8_t>> txs;

  auto block = BlockBuilder()
    .setTxs(txs)
    .build();

  assert(block.peer_sigs.size() == 0);
  assert(block.txs.size() == txs.size());
  assert(block.state == BlockState::uncommitted);
}

TEST(block_builder_test, over_capacity) {
//  std::vector<byte_array_t> txs(1LL << 32); // but block can have maximum tx size is 2^32-1. over!
  std::vector<std::vector<uint8_t>> txs(MaxTxs);

  ASSERT_THROW({
  auto block = BlockBuilder()
    .setTxs(txs)
    .build();
  }, std::runtime_error);
}

TEST(block_builder_test, no_signature) {
  std::vector<std::vector<uint8_t>> txs; // None
  auto block = sumeragi::BlockBuilder()
    .setTxs(txs)
    .build();

  assert(block.txs.size() == 0); // ToDo Should we think none?
}
