config-cpp
==========

A small cpp library designed for direct inclusion for loading simple key, value configuration files.

Configuration files are of the format: 

* Lines starting with # are ignored

A key value pair must be of the format:

keyname,type=value
type is one of:
 - s: string
 - f: float
 - i: integer
 - b: bool (value yes or no (default))

Examples:
```
stringkey,s=asdfasdf
intkey,i=1232
floatkey,f=12312.123
boolkey,b=yes
```

Usage:

1. Include config.hpp in your program and compile config.cpp.
2. In your program; create an instance of the Config class
3. Use load_config to load a specific config file
4. use the get_* functions to get the value
5. or the print_config function to list the loaded key value pairs.
