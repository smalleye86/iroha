/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
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
#include <transaction_generated.h>
#include "tx_generator.h"

TEST(tx_generator_test, AccountAddAccount) {

  ASSERT_NO_THROW({
    flatbuffers::FlatBufferBuilder fbb;
    auto actions = std::vector<flatbuffers::Offset<protocol::ActionWrapper>>{
      generator::random_AccountAddAccount(fbb)
    };
    auto attachment = generator::random_attachment(fbb);
    auto tx = generator::random_tx(fbb, actions, attachment);
    std::cout << dump::toString(
      *flatbuffers::GetRoot<protocol::Transaction>(tx.data())
    ) << std::endl;
  });
}

//#define DEF_TEST_COMMAND
