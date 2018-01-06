# cic
"ci" is for Continuous Intergation and the last "c" for checking!

## Dependencies

* POCO C++ Libraries 1.7.8+

Download from http://pocoproject.org/download/index.html
or ``` git clone git@github.com:pocoproject/poco.git ```

We need _PocoFoundation_, _PocoXML_, _PocoJSON_, _PocoUtil_ libraries
in order to build __check__ command line tool.

* fmt formatting library 3.0.0+

Download latest from http://fmtlib.net/latest/index.html
or ``` git clone git@github.com:fmtlib/fmt.git ```

## Build / install

### OS X

#### Build with XCode

Create project for XCode
```bash
cd /path/to/cic
mkdir build
cd build
# add -DCMAKE_INSTALL_PREFIX=/my/install/prefix into the line below for custom install location
cmake -GXcode ..
```
Build XCode project from command line
```bash
# assuming we still in /path/to/CICheck/build
xcodebuild -target ALL_BUILD
```
Install from command line (default prefix path is /usr/local)
```bash
# assuming we still in /path/to/CICheck/build
xcodebuild -target install
```

#### Build with Make

Generate Makefile and build
```bash
cd /path/to/cic
mkdir build
cd build
# add -DCMAKE_INSTALL_PREFIX=/my/install/prefix into the line below for custom install location
cmake -G"Unix Makefiles" ..
make
```
Install (default prefix path is /usr/local)
```bash
# assuming we still in /path/to/cic/build
make install
```


### Linux

Build
```bash
cd /path/to/cic
mkdir build
cd build
# add -DCMAKE_INSTALL_PREFIX=/my/install/prefix into the line below for custom install location
cmake ..
make
```
Install (default prefix path is /usr/local)
```bash
# assuming we are still within /path/to/cic/build directory
make install
```
