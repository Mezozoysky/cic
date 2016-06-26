# CICheck
"CI" is for Continuous Intergation and "Check" for checking!.

## Dependencies

* POCO C++ Libraries 1.7.3+

Download from http://pocoproject.org/download/index.html
or ``` git clone git@github.com:pocoproject/poco.git ```

We need _PocoFoundation_, _PocoXML_, _PocoJSON_, _PocoUtil_ libraries
in order to build __cicheck__ command line tool.

* fmt formatting library 3.0.0+

Download latest from http://fmtlib.net/latest/index.html
or ``` git clone git@github.com:fmtlib/fmt.git ```

## Build

### OS X
#### Create project for XCode
```bash
cd /path/to/CICheck
mkdir build
cd build
cmake -DPoco_DIR=/path/to/poco/dir -DFMT_DIR=/path/to/fmt/dir ..
```
#### Build XCode project from command line
```bash
# assuming we still in /path/to/CICheck/build
xcodebuild -target BUILD_ALL
```
