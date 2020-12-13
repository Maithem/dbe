#!/bin/bash
set -x

docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -t -d -v $(pwd)/targetfs:/targetfs --name runner1 kv-runner-image