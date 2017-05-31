/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language gover
ning permissions and
limitations under the License.
*/

//
// Created by Takumi Yamashita on 2017/03/16.
//

#include <algorithm>
#include <deque>
#include <regex>

#include <flatbuffers/flatbuffers.h>
#include <infra/config/peer_service_with_json.hpp>
#include <membership_service/peer_service.hpp>
#include <service/connection.hpp>
#include <utils/datetime.hpp>

namespace peer {

  using PeerServiceConfig = config::PeerServiceConfig;
  Nodes peerList;
  bool is_active;

  namespace myself {

    std::string getPublicKey() { return PeerServiceConfig::getInstance().getMyPublicKey(); }
    std::string getPrivateKey() { return PeerServiceConfig::getInstance().getMyPrivateKey(); }
    std::string getIp() { return PeerServiceConfig::getInstance().getMyIp(); }

    bool isActive() { return is_active; }
    void activate() { is_active = true; }
    void stop() { is_active = false; }

    bool isLeader() {
      auto sorted_peers = service::getActivePeerList();
      if (sorted_peers.empty()) return false;
      auto peer = *sorted_peers.begin();
      return peer->publicKey == getPublicKey() && peer->ip == getIp();
    }

  }  // namespace myself

  namespace service {

    // this function must be invoked before use of peer-service.
    void initialize() {
      if (!peerList.empty()) {
        return;
      }
      for (const auto &json_peer : PeerServiceConfig::getInstance().getGroup()) {
        peerList.push_back(std::make_shared<Node>(
            json_peer["ip"].get<std::string>(),
            json_peer["publicKey"].get<std::string>(),
            PeerServiceConfig::getInstance().getMaxTrustScore()));
      }
      is_active = false;
    }

    size_t getMaxFaulty() {
      return (size_t)std::max(0, ((int)getActivePeerList().size() - 1) / 3);
    }

    Nodes getAllPeerList() {
      initialize();
      return peerList;
    }

    Nodes getActivePeerList() {
      initialize();

      Nodes nodes;
      for (const auto &node : peerList) {
        if (node->active) {
          nodes.push_back(
              std::make_unique<peer::Node>(node->ip, node->publicKey, node->trust));
        }
      }

      // TODO: maintain nodes already sorted
      sort(nodes.begin(), nodes.end(),
           [](const auto &a, const auto &b) { return a->trust > b->trust; });

      return nodes;
    }

    std::vector<std::string> getIpList() {
      std::vector<std::string> ret_ips;
      for (const auto &node : getActivePeerList()) {
        ret_ips.push_back(node->ip);
      }
      return ret_ips;
    }

    // is exist which peer?
    bool isExistIP(const std::string &ip) {
      return findPeerIP(ip) != peerList.end();
    }

    bool isExistPublicKey(const std::string &publicKey) {
      return findPeerPublicKey(publicKey) != peerList.end();
    }

    Nodes::iterator findPeerIP(const std::string &ip) {
      initialize();
      return std::find_if(peerList.begin(), peerList.end(),
                          [&ip](const auto &p) { return p->ip == ip; });
    }

    Nodes::iterator findPeerPublicKey(const std::string &publicKey) {
      initialize();
      return std::find_if(
          peerList.begin(), peerList.end(),
          [&publicKey](const auto &p) { return p->publicKey == publicKey; });
    }

    std::shared_ptr<peer::Node> leaderPeer() {
      return std::move(*getActivePeerList().begin());
    }

  }  // namespace service

  namespace transaction {

    namespace issue {
      // invoke to issue transaction
      void add(const std::string &ip, const peer::Node &peer) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

      void remove(const std::string &ip, const std::string &publicKey) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

      void setTrust(const std::string &ip, const std::string &publicKey,
                    const double &trust) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

      void changeTrust(const std::string &ip, const std::string &publicKey,
                       const double &trust) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

      void setActive(const std::string &ip, const std::string &publicKey,
                     const bool active) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

    }  // namespace issue

    namespace executor {
      // invoke when execute transaction
      bool add(const peer::Node &peer) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

      bool remove(const std::string &publicKey) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

      bool setTrust(const std::string &publicKey, const double &trust) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

      bool changeTrust(const std::string &publicKey, const double &trust) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

      bool setActive(const std::string &publicKey, const bool active) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }
    }  // namespace executor

    namespace validator {
      // invoke when validator transaction
      bool add(const peer::Node &peer) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

      bool remove(const std::string &publicKey) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

      bool setTrust(const std::string &publicKey, const double &trust) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

      bool changeTrust(const std::string &publicKey, const double &trust) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

      bool setActive(const std::string &publicKey, const bool active) {
        throw std::runtime_error("deleted temporarily to specify sumeragi logic.");
      }

    }  // namespace validator
  }  // namespace transaction
}  // namespace peer
