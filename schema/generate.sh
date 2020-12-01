#!/usr/bin/env bash

# Really ugly and temporary glue to run moo code generator.
# This will simplify and move into CMake.

mydir=$(dirname $(realpath $BASH_SOURCE))
srcdir=$(dirname $mydir)
appfwkdir=$(realpath $mydir/../../appfwk/schema)
echo "Mydir $mydir, Appfwkdir $appfwkdir"


compile () {
    local name="$1"; shift

    moo -M $mydir -T $mydir -M $appfwkdir -T $appfwkdir compile ipm-$name-job.jsonnet >ipm-$name-job.json
}


compile singleprocess
compile singleprocess-pubsub
compile zmqpublisher
compile zmqreceiver
compile zmqsender
compile zmqsubscriber
