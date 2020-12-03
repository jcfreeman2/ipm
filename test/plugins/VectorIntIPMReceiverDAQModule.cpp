/**
 * @file VectorIntIPMReceiverDAQModule.cc VectorIntIPMReceiverDAQModule class
 * implementation
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "VectorIntIPMReceiverDAQModule.hpp"

#include "appfwk/cmd/Nljs.hpp"
#include "ipm/vectorintipmreceiverdaqmodule/Nljs.hpp"

#include <chrono>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <TRACE/trace.h>
/**
 * @brief Name used by TRACE TLOG calls from this source file
 */
#define TRACE_NAME "VectorIntIPMReceiver" // NOLINT

namespace dunedaq {
namespace ipm {

VectorIntIPMReceiverDAQModule::VectorIntIPMReceiverDAQModule(const std::string& name)
  : appfwk::DAQModule(name)
  , thread_(std::bind(&VectorIntIPMReceiverDAQModule::do_work, this, std::placeholders::_1))
  , outputQueue_(nullptr)
{

  register_command("conf", &VectorIntIPMReceiverDAQModule::do_configure);
  register_command("start", &VectorIntIPMReceiverDAQModule::do_start);
  register_command("stop", &VectorIntIPMReceiverDAQModule::do_stop);
}

void
VectorIntIPMReceiverDAQModule::init(const data_t& init_data)
{
  auto ini = init_data.get<appfwk::cmd::ModInit>();
  for (const auto& qi : ini.qinfos) {
    if (qi.name == "output") {
      ERS_INFO("VIIRDM: output queue is " << qi.inst);
      outputQueue_.reset(new appfwk::DAQSink<std::vector<int>>(qi.inst));
    }
  }
}

void
VectorIntIPMReceiverDAQModule::do_configure(const data_t& config_data)
{
  cfg_ = config_data.get<vectorintipmreceiverdaqmodule::Conf>();

  nIntsPerVector_ = cfg_.nIntsPerVector;
  queueTimeout_ = static_cast<std::chrono::milliseconds>(cfg_.queue_timeout_ms);

  input_ = makeIPMReceiver(cfg_.receiver_type);

  input_->connect_for_receives(cfg_.connection_info);
}

void
VectorIntIPMReceiverDAQModule::do_start(const data_t& /*args*/)
{
  thread_.start_working_thread();
}

void
VectorIntIPMReceiverDAQModule::do_stop(const data_t& /*args*/)
{
  thread_.stop_working_thread();
}

void
VectorIntIPMReceiverDAQModule::do_work(std::atomic<bool>& running_flag)
{
  size_t counter = 0;
  std::ostringstream oss;

  while (running_flag.load()) {
    if (input_->can_receive()) {

      TLOG(TLVL_TRACE) << get_name() << ": Creating output vector";
      std::vector<int> output(nIntsPerVector_);

      try {

        auto recvd = input_->receive(queueTimeout_);

        if (recvd.data.size() == 0) {
          TLOG(TLVL_TRACE) << "No data received, moving to next loop iteration";
          continue;
        }

        assert(recvd.data.size() == nIntsPerVector_ * sizeof(int));
        memcpy(&output[0], &recvd.data[0], sizeof(int) * nIntsPerVector_);
      } catch (ReceiveTimeoutExpired const& rte) {
        TLOG(TLVL_TRACE) << "ReceiveTimeoutExpired: " << rte.what();
        continue;
      }
      oss << ": Received vector " << counter << " with size " << output.size();
      ers::info(ReceiverProgressUpdate(ERS_HERE, get_name(), oss.str()));
      oss.str("");

      TLOG(TLVL_TRACE) << get_name() << ": Pushing vector into outputQueue";
      try {
        outputQueue_->push(std::move(output), queueTimeout_);
      } catch (const appfwk::QueueTimeoutExpired& ex) {
        ers::warning(ex);
      }

      TLOG(TLVL_TRACE) << get_name() << ": End of do_work loop";
      counter++;
    } else {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
}

} // namespace ipm
} // namespace dunedaq

DEFINE_DUNE_DAQ_MODULE(dunedaq::ipm::VectorIntIPMReceiverDAQModule)
