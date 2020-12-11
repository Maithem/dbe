# dbe 

The purpose of this repo is to provide tools to easily gather per-process block level IO statistics.
This is useful when evaluating the read, write and space amplification for different storage engines. Since many storage
engines rely on "non-explicit" IO (i.e., mmap), mining /proc/[PID]/io is not sufficient. Alternatively, this tool
creates a filesystem backed by a loopback device. The storage engine can then exclusively use the "virtual filesystem",
and block level stats can mined from https://www.kernel.org/doc/Documentation/block/stat.txt