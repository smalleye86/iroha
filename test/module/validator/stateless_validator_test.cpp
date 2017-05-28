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
//#include <../test/generator/tx_generator.h>
#include <validator/stateless_validator.hpp>
#include <consensus/block_builder.hpp>
#include <flatbuffers/flatbuffers.h>

using sumeragi::Block;
using sumeragi::BlockBuilder;

TEST(stateless_validator_test, failed_to_dispatch) {

  std::vector<std::vector<uint8_t>> txs {
    //tx
  };

  auto block = BlockBuilder()
    .setTxs(txs)
    .build();

  validator::stateless::test(block);
}

TEST(stateless_validator_test, tx) {
/*
auto creator = tx_builder::CreateSignature(
  "pubkey",
  "signature"
);

auto action = tx_builder::CreateAction(
  protocol::Action::AccountAddAccount
);

auto actions = std::vector<std::vector<uint8_t>> {
  action
};

auto tx = tx_buiilder::TxBuilder()
  .setCreator(creator)
  .setActions(actions)
  .build();
*/
}