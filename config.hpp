/* config.h: A simple configuration reader class
 *
 * Author: Gaute Hope <eg@gaute.vetsj.com>
 * Date:   2012-08-27
 *
 * Format:


  # Lines starting with # are ignored
  #
  # A key value pair must be of the format:
  # keyname,type=value
  # type is one of:
  #   - s: string
  #   - f: float
  #   - i: integer
  #   - b: bool (value yes or no (default))

  stringkey,s=asdfasdf
  intkey,i=1232
  floatkey,f=12312.123
  boolkey,b=yes

 */

# pragma once

# include <iostream>
# include <fstream>
# include <string>
# include <string.h>
# include <vector>
# include <exception>

using namespace std;

/* Macros for stringifying defines */
# define STRINGIFY_I(s) #s
# define STRINGIFY(s)   STRINGIFY_I(s)

# define MAX_LINE_LENGTH 1024

class Config {
  private:
    bool   hasfname;
    string config_fname;
    bool   loaded = false;

  public:
    class NoSuchKeyException : public exception {
      virtual const char * what () const throw ();
    };

    class LoadError : public exception {
      virtual const char * what () const throw ();
    };

    typedef enum _Type {
      UNKNOWN,
      STRING,
      FLOAT,
      INTEGER,
      BOOL,
    } Type;

    typedef struct _Item {
      string  key;
      Type    type;

      union {
        char * val_str;
        float  val_float;
        int    val_int;
        bool   val_bool;
      };
    } Item;

    vector<Item>   items;
    vector<string> comments;

    Config ();
    Config (const char*);

    void load_config ();
    void load_config (const char *);
    void print_config ();
    bool is_loaded ();
    string get_type_s (Type);

    string get_string (const char*);
    float  get_float  (const char*);
    int    get_int    (const char*);
    bool   get_bool   (const char*);
};

