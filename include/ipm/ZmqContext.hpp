#ifndef IPM_INCLUDE_IPM_ZMQCONTEXT_HPP_
#define IPM_INCLUDE_IPM_ZMQCONTEXT_HPP_

/**
 *
 * @file ZmqContext.hpp ZmqContext Singleton class for hosting 0MQ context
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include <zmq.hpp>

namespace dunedaq::ipm {
class ZmqContext
{
public:
  static ZmqContext& instance()
  {
    static ZmqContext s_ctx;
    return s_ctx;
  }

  zmq::context_t& GetContext() { return m_context; }

private:
  ZmqContext() {}
  ~ZmqContext() { m_context.close(); }
  zmq::context_t m_context;

  ZmqContext(ZmqContext const&) = delete;
  ZmqContext(ZmqContext&&) = delete;
  ZmqContext& operator=(ZmqContext const&) = delete;
  ZmqContext& operator=(ZmqContext&&) = delete;
};
} // namespace dunedaq::ipm

#endif // IPM_INCLUDE_IPM_ZMQCONTEXT_HPP_
