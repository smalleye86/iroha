/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

                 http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef IROHA_DOMAIN_ACTION_BUILDER_HPP
#define IROHA_DOMAIN_ACTION_BUILDER_HPP

#include <primitive_builder.hpp>
#include <map>
#include <vector>

namespace builder{

    using Timestamp = unsigned long;
    using ubyte     = unsigned char;
    using Signature   = protocol::Signature;
    using Transaction = protocol::Transaction;

    class DomainActionBuilder{

        std::string                  target_asset;      // AssetDelete / AssetMove
        std::string                  target_domain;     // AccountCreate / AccountRegister / AccountUnregister / DomainDelete

        std::string                  source_domain;     // DomainMove
        std::string                  destination_domain;// DomainMove

        std::string                  username;          // AccountCreate
        std::vector<SignatureHolder> signatories;       // AccountCreate
        ubyte                        quorum;            // AccountCreate
        std::string                  domain;            // DomainCreate / DomainSetPermission
        std::string                  root_account;      // DomainCreate
        std::string                  target_account;    // AccountDelete / AccountRegister / AccountUnregister / DomainSetPermission

        std::vector<ubyte>           permission;        // AssetSetPermission / AccountRegister / DomainSetPermission
        std::string                  asset_uuid;        // AssetSetPermission

        enum class State{
            UnderConstruction,
            AfterConstruction
        } state;

        protocol::Action action;
        static std::map<
                protocol::Action,
                std::function<flatbuffers::Offset<void>(
                flatbuffers::FlatBufferBuilder&,
                const AccountActionBuilder&
        )>> buildFunctions;

    public:

        DomainActionBuilder();

        DomainActionBuilder& setTargetAsset(const std::string&);       // AssetDelete / AssetMove
        DomainActionBuilder& setTargetDomain(const std::string&);      // AccountCreate / AccountRegister / AccountUnregister / DomainDelete

        DomainActionBuilder& setSourceDomain(const std::string&);      // DomainMove
        DomainActionBuilder& setDestinationDomain(const std::string&); // DomainMove

        DomainActionBuilder& setUsername(const std::string&);          // AccountCreate
        DomainActionBuilder& setSignatories(const std::vector<SignatureHolder>&);    // AccountCreate
        DomainActionBuilder& setQuorum(const ubyte&);                  // AccountCreate
        DomainActionBuilder& setDomain(const std::string&);            // DomainCreate / DomainSetPermission
        DomainActionBuilder& setRootAccount(const std::string&);       // DomainCreate
        DomainActionBuilder& setTargetAccount(const std::string&);     // AccountDelete / AccountRegister / AccountUnregister / DomainSetPermission

        DomainActionBuilder& setPermission(const std::vector<ubyte>&); // AssetSetPermission / AccountRegister / DomainSetPermission
        DomainActionBuilder& setAssetUuid(const std::string&);         // AssetSetPermission

        DomainActionBuilder& as(protocol::Action);
        flatbuffers::Offset<void> buildOffset(flatbuffers::FlatBufferBuilder& fbb);
        flatbuffers::Offset<DomainActionWrapper> buildWrapperOffset(flatbuffers::FlatBufferBuilder& fbb);
    };
};

#endif