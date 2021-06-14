#!/bin/bash

IP=${1:-192.168.0.16}

send ()
{
    local DATA=$1
    echo $DATA
    curl http://$IP/json -H 'Content-Type: application/json' -d "$DATA" &>/dev/null
}

send "{\"commands\":[[\"queue\", \"lock\"]]}"

send "{\"commands\":[
  [\"forward\", 1000],
  [\"pen\", 1],
  [\"backward\", 1000],
  [\"pen\", 0],
  [\"right\", 1000],
  [\"pen\", 1],
  [\"left\", 1000],
  [\"pen\", 0]
]}"

send "{\"commands\":[[\"queue\", \"unlock\"]]}"

