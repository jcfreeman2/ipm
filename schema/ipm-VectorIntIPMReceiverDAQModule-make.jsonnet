// hand written helpers to make object compliant with appfwk-fdc-schema
{
    // The internally known name of the only queue used
    queue: "output",

    // Make a conf object for FDC
    conf(nper, conninfo, receiver="ZmqReceiver", tpc="", toms=100) :: {
        nIntsPerVector: nper,
        queue_timeout_ms: toms,
        receiver_type: receiver,
        connection_info: conninfo,
        topic: tpc
    },
}
