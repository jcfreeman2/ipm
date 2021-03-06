/**
 *
 * @file ZmqReceiverImpl.hpp Implementations of common routines for ZeroMQ Receivers
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef IPM_PLUGINS_ZMQRECEIVERIMPL_HPP_
#define IPM_PLUGINS_ZMQRECEIVERIMPL_HPP_

#include "ipm/Subscriber.hpp"
#include "ipm/ZmqContext.hpp"

#include "TRACE/trace.h"
#include "zmq.hpp"

#include <string>
#include <vector>

namespace dunedaq {
namespace ipm {

// Remember that Subscriber is a superset of Receiver
class ZmqReceiverImpl : public Subscriber
{
public:
  enum class ReceiverType
  {
    Subscriber,
    Pull,
  };

  explicit ZmqReceiverImpl(ReceiverType type)
    : m_socket(ZmqContext::instance().GetContext(),
               type == ReceiverType::Pull ? zmq::socket_type::pull : zmq::socket_type::sub)
  {}
  bool can_receive() const noexcept override { return m_socket_connected; }
  void connect_for_receives(const nlohmann::json& connection_info) override
  {
    std::string connection_string = connection_info.value<std::string>("connection_string", "inproc://default");
    TLOG(TLVL_INFO) << "Connection String is " << connection_string;
    m_socket.setsockopt(ZMQ_RCVTIMEO, 1); // 1 ms, we'll repeat until we reach timeout
    m_socket.connect(connection_string);
    m_socket_connected = true;
  }

  void subscribe(std::string const& topic) override { m_socket.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size()); }
  void unsubscribe(std::string const& topic) override
  {
    m_socket.setsockopt(ZMQ_UNSUBSCRIBE, topic.c_str(), topic.size());
  }

protected:
  Receiver::Response receive_(const duration_t& timeout) override
  {
    Receiver::Response output;
    zmq::message_t hdr, msg;
    size_t res = 0;

    auto start_time = std::chrono::steady_clock::now();
    do {

      try {
        TLOG(TLVL_TRACE + 3) << "Going to receive header";
        res = m_socket.recv(&hdr);
        TLOG(TLVL_TRACE + 3) << "Recv res=" << res << " for header (hdr.size() == " << hdr.size() << ")";
      } catch (zmq::error_t const& err) {
        // Throw ERS-ified exception
      }
      if (res > 0 || hdr.more()) {
        TLOG(TLVL_TRACE + 3) << "Going to receive data";
        output.m_metadata.resize(hdr.size());
        memcpy(&output.m_metadata[0], hdr.data(), hdr.size());

        // ZMQ guarantees that the entire message has arrived
        res = m_socket.recv(&msg);
        TLOG(TLVL_TRACE + 3) << "Recv res=" << res << " for data (msg.size() == " << msg.size() << ")";
        output.m_data.resize(msg.size());
        memcpy(&output.m_data[0], msg.data(), msg.size());
      } else {
        usleep(1000);
      }
    } while (std::chrono::steady_clock::now() - start_time < timeout && res == 0);

    if (res == 0) {
      throw ReceiveTimeoutExpired(ERS_HERE, timeout.count());
    }

    TLOG(TLVL_TRACE + 2) << "Returning output with metadata size " << output.m_metadata.size() << " and data size "
                         << output.m_data.size();
    return output;
  }

private:
  zmq::socket_t m_socket;
  bool m_socket_connected{ false };
};

} // namespace ipm
} // namespace dunedaq

#endif // IPM_PLUGINS_ZMQRECEIVERIMPL_HPP_
