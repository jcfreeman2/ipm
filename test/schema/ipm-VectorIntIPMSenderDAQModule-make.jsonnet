// hand written helpers to make object compliant with appfwk-fdc-schema
{
    // The internally known name of the only queue used
    queue: "input",

    // Make a conf object for FDC
    conf(nper, conninfo, sender="ZmqSender", tpc="", toms=100) :: {
        nIntsPerVector: nper,
        queue_timeout_ms: toms,
        sender_type: sender,
        connection_info: conninfo,
        topic: tpc
    },
}
