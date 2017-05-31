/**
 * Copyright 2017 Soramitsu Co., Ltd.
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


#include <grpc++/grpc++.h>
#include <consensus/block_builder.hpp> // sumeragi::Block
#include <membership_service/peer_service.hpp>
#include <endpoint_generated.h>
#include <endpoint.grpc.fb.h>

namespace connection {

enum ResponseType {
  RESPONSE_OK,
  RESPONSE_INVALID_SIG,  // wrong signature
  RESPONSE_ERRCONN,      // connection error
};

/**
 * SumeragiClient
 * @brief RPC client for connecting with other sumeragi RPC server.
 */
class SumeragiClient {
public:
  explicit SumeragiClient(std::shared_ptr<Channel> channel)
    : stub_(Sumeragi::NewStub(channel)) {}

  /**
   * Consensus
   * @detail sumeragi -> interface -> [RPC Client] -> RPC Server -> other sumeragi
   *
   * @param block_ - block to consensus
   * @param response - to return SumeragiServer response
   *
   * @return grpc::Status
   */
  grpc::Status Consensus(const Block &block_,
                 flatbuffers::BufferRef<Response> *response) const {
    grpc::ClientContext context;
    auto block_buf = sumeragi::BlockBuilder(block_).build();
    flatbuffers::
    auto request = flatbuffer_service::CreateBufferRef<sumeragi::Block>(block);
    return stub_->Consensus(&context, request, response);
  }

  /**
   * Torii
   * @brief Cient for sending tx to sumeragi. This method is for peer service.
   * @details peer service -> interface -> [RPC Cient] -> RPC Server -> other sumeragi
   *
   * @param tx [description]
   * @param responseRef [description]
   *
   * @return VoidHandler. Returns exception if error occurs.
   */
  grpc::Status Torii(const ::protocol::Transaction &tx,
                    flatbuffers::BufferRef<Response> *responseRef) const {
    ClientContext context;
    auto request = flatbuffer_service::CreateBufferRef<::iroha::Transaction>(tx);
    return stub_->Torii(&clientContext, request, response);
  }

private:
  std::unique_ptr<Sumeragi::Stub> stub_;
};

namespace with_sumeragi {

/**
 * unicasts block to a validating peer.
 * @param block - block to consensus.
 * @param index - validating peer's index.
 */
void unicast(const sumeragi::Block& block, size_t index) {
  auto peers = ::peer::service::getActivePeerList();
  if (::peer::service::isExistIP(peers[index]->ip)) {

    SumeragiClient client;
    flatbuffers::BufferRef<protocol::SumeragiResponse> response;
    client.Consensus(block, &response);

    if (response.GetRoot()->code() == protocol::ResponseCode::FAIL) {
      std::cout << "errcode: " << protocol::EnumNameResponseCode(response.GetRoot()->code())
                << "message: " << response.GetRoot()->message() << std::endl;
    }
  } else {
    std::cout << "IP doesn't exist." << std::endl;
  }
}

/**
 * multicasts block to [beginIndex, endIndex) peers.
 * @param block - block to consensus.
 * @param beginIndex - validating peer's index except leader (usually = 1)
 * @param endIndex - validatinng peer's tail index + 1
 */
void multicast(const sumeragi::Block& block, size_t beginIndex, size_t endIndex) {

}

/**
 * commits block to all peers.
 */
void commit() {

}

} // namespace with_sumeragi
} // namespace connection
