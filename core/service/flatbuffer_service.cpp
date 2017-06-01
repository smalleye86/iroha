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

#include <vector>

namespace flatbuffer_service {

sumeragi::Block unpackBlock(const protocol::Block& flatbuf) {
  sumeragi::Block ret;
  ret.txs = {};
  for (const auto& txw: *flatbuf.txs()) {
    ret.txs.emplace_back(
      txw->tx()->begin(),
      txw->tx()->end()
    );
  }
  ret.state = sumeragi::BlockState::uncommitted;
  ret.peer_sigs = {};
  ret.created = datetime::unixtime();
  return ret;
}

std::vector<uint8_t> packBlock(
  const sumeragi::Block& block,
  const std::vector<uint8_t>& prev_hash,
  int length,
  const std::vector<uint8_t>& merkle_root,
  int height
) {
  flatbuffers::FlatBufferBuilder fbb;

  std::vector<flatbuffers::Offset<protocol::TransactionWrapper>> txs;
  for (auto& tx: block.txs) {
    txs.push_back(protocol::CreateTransactionWrapperDirect(fbb, &tx));
  }

  std::vector<flatbuffers::Offset<protocol::Signature>> peer_sigs;
  for (auto& sig: block.peer_sigs) {
    auto sigoffset = protocol::CreateSignatureDirect(
      fbb,
      &sig.publicKey,
      &sig.signature
    );
    peer_sigs.push_back(sigoffset);
  }

  auto blockoffset = protocol::CreateBlockDirect(
    fbb, &txs, &peer_sigs,
    &prev_hash, length, &merkle_root,
    height, block.created,
    block.state == sumeragi::BlockState::committed ?
      protocol::BlockState::COMMITTED :
      protocol::BlockState::UNCOMMITTED
  );

  fbb.Finish(blockoffset);

  auto buf = fbb.GetBufferPointer();
  return {buf, buf + fbb.GetSize()};
}

} // namespace flatbuffer_service
