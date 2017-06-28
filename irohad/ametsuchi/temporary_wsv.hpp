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

#ifndef IROHA_TEMPORARYWSV_HPP
#define IROHA_TEMPORARYWSV_HPP

#include <ametsuchi/command_executor.hpp>
#include <ametsuchi/wsv_query.hpp>
#include <dao/block.hpp>
#include <dao/transaction.hpp>
#include <rxcpp/rx-observable.hpp>

namespace iroha {

  namespace ametsuchi {

    /**
     * Temporary world state view
     * Allows to query the temporal world state view
     */
    class TemporaryWsv : public WsvQuery {
     public:
      /**
       * Applies a transaction to current state using @see CommandExecutor
       * @param transaction Transaction to be applied
       * @return Observable with applied commands
       * Stops calling onNext() if there are no subscribers
       * Calls onError() if it failed to apply a command
       */
      virtual rxcpp::observable<dao::Command> apply(
          const dao::Transaction &transaction) = 0;
    };

  }  // namespace ametsuchi

}  // namespace iroha

#endif  // IROHA_TEMPORARYWSV_HPP
