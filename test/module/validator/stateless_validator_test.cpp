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
#include <validator/stateless_validator.hpp>
#include <consensus/block_builder.hpp>
#include <flatbuffers/flatbuffers.h>
#include <actions_generated.h>
#include <transaction_generated.h>
#include "../generator/tx_generator.h"

using sumeragi::Block;
using sumeragi::BlockBuilder;

TEST(stateless_validator_test, random_tx) {

  flatbuffers::FlatBufferBuilder fbb;
  auto actions = std::vector<flatbuffers::Offset<protocol::ActionWrapper>> {
    generator::random_AccountAddAccount(fbb)
  };
  auto attachment = generator::random_attachment(fbb);
  std::vector<std::vector<uint8_t>> txs {
    generator::random_tx(fbb, actions, attachment)
  };

  auto block = BlockBuilder()
    .setTxs(txs)
    .build();

  validator::stateless::receive([](const Block& block) {
    std::cout << "receive\n";
    for (auto const& tx: block.txs) {
      std::cout << dump::toString(*flatbuffers::GetRoot<protocol::Transaction>(tx.data())) << std::endl;
    }

    auto curr_time = datetime::unixtime();
    ASSERT_TRUE(curr_time - 2 <= block.created);
    ASSERT_TRUE(block.created <= curr_time);
    ASSERT_TRUE(block.state == sumeragi::BlockState::uncommitted);
    ASSERT_TRUE(block.peer_sigs.empty());
  });

  ASSERT_TRUE(validator::stateless::test(block));
}


TEST(stateless_validator_test, tx_string_broken) {

  flatbuffers::FlatBufferBuilder fbb;
  auto actions = std::vector<flatbuffers::Offset<protocol::ActionWrapper>> {
    generator::random_AssetCreate(fbb)
  };
  auto attachment = generator::random_attachment(fbb);
  std::vector<std::vector<uint8_t>> txs {
    generator::random_tx(fbb, actions, attachment)
  };

  //auto root = flatbuffers::GetRoot<protocol::Transaction>(txs[0].data());

  auto block = BlockBuilder()
    .setTxs(txs)
    .build();

  validator::stateless::receive([](const Block& block) {});

  ASSERT_TRUE(validator::stateless::test(block));
}