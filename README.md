
## Combined *Flow Analytics Repository

### Dependencies

* libraft [[1]](https://github.com/RaftLib/RaftLib)
* libpcap [[2]](http://www.tcpdump.org)
* libprotobuf & protoc [[3]](https://github.com/google/protobuf)
* catch [[4]](https://github.com/catchorg/Catch2) (is automatically integrated when running cmake)

### Build

Requires C++14 compatible compiler.

    mkdir build && cd build
    cmake ..
    make

### Kernels

|Kernel Name         |Input                                 |Output                                |
|--------------------|--------------------------------------|--------------------------------------|
|CLFRTable           |Key, Packet                           |CLFR                                  |
|LiveCapture         |*none*                                |RawPacket                             |
|PCAPFileReader      |*none*                                |RawPacket                             |
|RawPacketParser     |RawPacket                             |Key, Packet                           |
|TZSPReceiver        |*none*                                |RawPacket                             |

### References

[1] https://github.com/RaftLib/RaftLib

[2] http://www.tcpdump.org

[3] https://github.com/google/protobuf

[4] https://github.com/catchorg/Catch2