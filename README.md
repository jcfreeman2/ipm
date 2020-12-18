# ipm
Inter-Process Messaging


The IPM library provides the low-level library for for sending messages between DUNE DAQ processes. IPM deals with messages consisting of arrays of bytes: higher-level concepts such as object serialization/deserialization will be handled by other libraries and processes building on IPM.

_JCF, Nov-25-2020: In order to build from the current head of develop,
you'll need to set up the "zmq v4_3_1 -q e19:prof" ups product, which is
not available in the standard DUNE DAQ ups area,
/cvmfs/dune.opensciencegrid.org/dunedaq/DUNE/products but rather in /cvmfs/dune.opensciencegrid.org/dunedaq/DUNE/products_dev. How to modify
your work area so it automatically sets up a not-yet-standard ups
product is described in the "Adding extra UPS products and product
pools" section of
https://github.com/DUNE-DAQ/appfwk/wiki/Compiling-and-running-under-v2.0.0_


IPM provides two communication patterns:

1. `Sender`/`Receiver`, a pattern in which one sender talks to one receiver
2. `Publisher`/`Subscriber`, a pattern in which one sender talks to zero or more receivers. Each message goes to all subscribers

Users should interact with IPM via the interfaces `dunedaq::ipm::Sender`, `dunedaq::ipm::Receiver` and `dunedaq::ipm::Subscriber`, which are created using the factory functions `dunedaq::ipm::makeIPM(Sender|Receiver|Subscriber)`, which each take a string argument giving the implementation type. The currently-available implementation types all use ZeroMQ, and are:

* `ZmqSender` implementing `dunedaq::ipm::Sender` in the sender/receiver pattern
* `ZmqReceiver` implementing `dunedaq::ipm::Receiver`
* `ZmqPublisher` implementing `dunedaq::ipm::Sender` in the publisher/subscriber pattern
* `ZmqSubscriber` implementing `dunedaq::ipm::Subscriber`

Basic example of the sender/receiver pattern:

```c++
// Sender side
std::shared_ptr<dunedaq::ipm::Sender> sender=dunedaq::ipm::makeIPMSender("ZmqSender");
sender->connect_for_sends({ {"connection_string", "tcp://127.0.0.1:12345"} });
void* message= ... ;
// Last arg is send timeout
sender->send(message, message_size, std::chrono::milliseconds(10));

// Receiver side
std::shared_ptr<dunedaq::ipm::Receiver> receiver=dunedaq::ipm::makeIPMReceiver("ZmqReceiver");
receiver->connect_for_receives({ {"connection_string", "tcp://127.0.0.1:12345"} });
// Arg is receive timeout
Receiver::Response response=receiver->receive(std::chrono::milliseconds(10));
// ... do something with response.data or response.metadata
```

Basic example of the publisher/subscriber pattern:

```c++
// Publisher side
std::shared_ptr<dunedaq::ipm::Sender> publisher=dunedaq::ipm::makeIPMSender("ZmqPublisher");
publisher->connect_for_sends({ {"connection_string", "tcp://127.0.0.1:12345"} });
void* message= ... ;
// Third arg is send timeout; last arg is topic for subscribers to subscribe to
publisher->send(message, message_size, std::chrono::milliseconds(10), "topic");

// Subscriber side
std::shared_ptr<dunedaq::ipm::Subscriber> subscriber=dunedaq::ipm::makeIPMReceiver("ZmqSubscriber");
subscriber->connect_for_receives({ {"connection_string", "tcp://127.0.0.1:12345"} });
subscriber->subscribe("topic");
// Arg is receive timeout
Receiver::Response response=subscriber->receive(std::chrono::milliseconds(10));
// ... do something with response.data or response.metadata
```

More complete examples can be found in the `test/plugins` directory.
