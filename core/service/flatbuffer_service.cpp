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

#include <consensus/block_builder.hpp>
#include <flatbuffers/flatbuffers.h>
#include <main_generated.h>

namespace flatbuffer_service {

sumeragi::Block unpackBlock(const protocol::Block& block) {
  sumeragi::Block ret;
  ret.txs = std::vector<sumeragi::byte_array_t>(
    block.txs()->begin(),
    block.txs()->end()
  );
  ret.state = sumeragi::State::uncommitted;
  ret.peer_sigs = {};
  //ret.
  return ret;
}

}