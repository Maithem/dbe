#!/bin/bash
set -x

docker run -t -d -v targetfs:/targetfs --name runner1 kv-runner-image