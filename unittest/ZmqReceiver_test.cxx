/**
 *
 * @file ZmqReceiver_test.cxx ZmqReceiver class Unit Tests
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "../src/ZmqReceiver.cpp"

#define BOOST_TEST_MODULE ZmqReceiver_test // NOLINT
#include <boost/test/unit_test.hpp>

#include <boost/asio/signal_set.hpp>

#include <chrono>
#include <memory>

BOOST_AUTO_TEST_CASE(sanity_checks)
{

  std::unique_ptr<dunedaq::ipm::ZmqReceiver> umth_ptr = nullptr;

  auto starttime = std::chrono::steady_clock::now();
  BOOST_REQUIRE_NO_THROW(umth_ptr = std::make_unique<dunedaq::ipm::ZmqReceiver>());
  auto construction_time_in_ms =
    std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - starttime).count();
  BOOST_TEST_MESSAGE("Construction time was " << construction_time_in_ms << " ms");
}