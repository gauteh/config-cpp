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

Licence
=======
```
  Author: Gaute Hope <eg@gaute.vetsj.com>
  Date:   2012-08-27

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

```
