Bourgeon [![Build status](https://ci.appveyor.com/api/projects/status/aqtwv6lfq6g1pbxq?svg=true)](https://ci.appveyor.com/project/L1nkZ/bourgeon)
--------
Bourgeon is a C++14 library whose purpose is to provide an easy-to-use python
interface to implement plugins for Ragnarok Online clients.  

*Note: This is a work in progress.*

Clients supported
-----------------
* 2017-06-14a

Requirements
------------
* Python >= 3.5 (32-bit)
* CMake >= 3.4
* Visual Studio >= 2015

Clone and build
-----
```shell
$ git clone https://github.com/L1nkZ/Bourgeon --recurse-submodules
$ cd Bourgeon
$ mkdir build && cd build
# You may need to do "cmake .. -A Win32", if you're using Visual Studio 2019
$ cmake ..
$ cmake --build . --config MinSizeRel
```
Note: The project has to be built in Release mode for it work.
This is required to ensure ABI compatibility of our C++ std objects' and those
of the game client's.

How to use
----------
* Build the module (*bourgeon.dll*)
* Move the module to your Ragnarok folder and rename it to **bourgeon.mix**
* Create a folder named **plugins** in your Ragnarok folder and put your plugins into it
* Run your Ragnarok client

Plugins
-------
* You can get a sense of how plugins work by consulting scripts located in
the `examples` folder.  
* When injected into a client, Bourgeon fetches plugins from the `plugins`
subfolder. You should put your plugins into this folder if you want them to be
loaded at runtime.
