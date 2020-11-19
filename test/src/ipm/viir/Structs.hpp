/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains struct and other type definitions for shema in 
 * namespace dunedaq::ipm::viir.
 */
#ifndef DUNEDAQ_IPM_VIIR_STRUCTS_HPP
#define DUNEDAQ_IPM_VIIR_STRUCTS_HPP

#include <cstdint>

#include <nlohmann/json.hpp>
#include <string>

namespace dunedaq::ipm::viir {

    // @brief Same as a size_t in gcc v8.2.0
    using Size_t = uint64_t;

    // @brief Same as an int in gcc v8.2.0
    using Int = int32_t;

    // @brief String in gcc v8.2.0
    using String = std::string;

    // @brief Connection Info passed to connect_for_receives
    using ConnectionInfo = nlohmann::json;

    // @brief VectorIntIPMReceiverDAQModule Configuration
    struct Conf {

        // @brief Number of numbers
        Size_t nIntsPerVector;

        // @brief Milliseconds to wait on queue before timing out
        Int queue_timeout_ms;

        // @brief Optional metadata to include in sends
        String topic;

        // @brief IPMReceiver Implementation Plugin to load
        String receiver_type;

        // @brief Conneection Info
        ConnectionInfo connection_info;
    };

} // namespace dunedaq::ipm::viir

#endif // DUNEDAQ_IPM_VIIR_STRUCTS_HPP