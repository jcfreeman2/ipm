/**
 * @file VectorIntIPMSubscriberDAQModule.hpp
 *
 * VectorIntIPMSubscriberDAQModule receives vectors of integers from VectorIntIPMSenderDAQModule
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef IPM_TEST_PLUGINS_VECTORINTIPMSUBSCRIBERDAQMODULE_HPP_
#define IPM_TEST_PLUGINS_VECTORINTIPMSUBSCRIBERDAQMODULE_HPP_

#include "appfwk/DAQModule.hpp"
#include "appfwk/DAQSink.hpp"
#include "appfwk/ThreadHelper.hpp"
#include "ipm/Subscriber.hpp"

#include "ipm/vectorintipmreceiverdaqmodule/Structs.hpp"

#include <future>
#include <memory>
#include <string>
#include <vector>

namespace dunedaq {
namespace ipm {
/**
 * @brief VectorIntIPMSubscriberDAQModule creates vectors of ints and sends them
 * downstream
 */
class VectorIntIPMSubscriberDAQModule : public appfwk::DAQModule
{
public:
  /**
   * @brief VectorIntIPMSubscriberDAQModule Constructor
   * @param name Instance name for this VectorIntIPMSubscriberDAQModule instance
   */
  explicit VectorIntIPMSubscriberDAQModule(const std::string& name);

  VectorIntIPMSubscriberDAQModule(const VectorIntIPMSubscriberDAQModule&) =
    delete; ///< VectorIntIPMSubscriberDAQModule is not copy-constructible
  VectorIntIPMSubscriberDAQModule& operator=(const VectorIntIPMSubscriberDAQModule&) =
    delete; ///< VectorIntIPMSubscriberDAQModule is not copy-assignable
  VectorIntIPMSubscriberDAQModule(VectorIntIPMSubscriberDAQModule&&) =
    delete; ///< VectorIntIPMSubscriberDAQModule is not move-constructible
  VectorIntIPMSubscriberDAQModule& operator=(VectorIntIPMSubscriberDAQModule&&) =
    delete; ///< VectorIntIPMSubscriberDAQModule is not move-assignable

  void init(const data_t& ) override;

private:
  // Commands
  void do_configure(const data_t& );
  void do_start(const data_t& );
  void do_stop(const data_t& );

  // Threading
  appfwk::ThreadHelper thread_;
  void do_work(std::atomic<bool>& running_flag);

  // Configuration
  vectorintipmreceiverdaqmodule::Conf cfg_;
  std::shared_ptr<Subscriber> input_;
  std::unique_ptr<appfwk::DAQSink<std::vector<int>>> outputQueue_;
  std::chrono::milliseconds queueTimeout_;
  size_t nIntsPerVector_ = 999;

};
} // namespace ipm

ERS_DECLARE_ISSUE_BASE(ipm,
                       SubscriberProgressUpdate,
                       appfwk::GeneralDAQModuleIssue,
                       message,
                       ((std::string)name),
                       ((std::string)message))
} // namespace dunedaq

#endif // IPM_TEST_PLUGINS_VECTORINTIPMSUBSCRIBERDAQMODULE_HPP_
