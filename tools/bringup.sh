#!/bin/bash
set -x

docker run -t -d -v $(pwd)/targetfs:/targetfs --name runner1 kv-runner-image