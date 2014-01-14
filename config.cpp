/* config.cpp: A simple configuration reader class
 *
 * Author: Gaute Hope <eg@gaute.vetsj.com>
 * Date:   2012-08-27
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

# include <iostream>
# include <iomanip>
# include <fstream>
# include <string>
# include <string.h>
# include <exception>

# include "config.hpp"

using namespace std;

const char * Config::NoSuchKeyException::what () const throw () {
  return "No such key with this type";
}

const char * Config::LoadError::what () const throw () {
  return "Error while loading config file";
}

Config::Config () {
  config_fname = "";
  loaded = false;
}

Config::Config (const char * fname) {
  config_fname = fname;
  hasfname     = true;
  loaded       = false;
}

void Config::load_config () {
  if (hasfname) {
    ifstream cf (config_fname.c_str());

    if (cf.bad () || !cf.is_open ()) {
      cerr << "Config: Could not open file: " << config_fname << endl;
      throw LoadError ();
      return;
    }

    while (!cf.eof()) {
      char line[MAX_LINE_LENGTH];
      cf.getline (line, MAX_LINE_LENGTH);

      int start = 0;

      // Strip leading spaces
      for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ' ') start++;
        else break;
      }

      // Comment
      if (line[start] == '#') {
        comments.push_back (string(line));
        continue;
      }

      // Empty line
      if (strlen(&line[start]) < 1) {
        continue;
      }

      // Parse line
      Item i;
      string sline (&line[start]);
      int delim = sline.find(',');
      if (delim < 1) {
        cerr << "Config: Could not parse line: " << sline << endl;
        throw LoadError ();
      } else {
        i.key = sline.substr(0, delim);
        char type = sline[delim+1];
        switch (type) {
          case 's': i.type = STRING; break;
          case 'f': i.type = FLOAT; break;
          case 'i': i.type = INTEGER; break;
          case 'b': i.type = BOOL; break;
          default:
            {
              i.type = UNKNOWN;
              cerr << "Config: Unknown type while parsing line: " << sline << endl;
              throw LoadError ();
              continue;
            }
        }

        int delim = sline.find ('=');
        if (delim < 1) {
          cerr << "Config: Could not parse line: " << sline << endl;
          throw LoadError ();
        } else {
          string val = sline.substr (delim+1, sline.length());
          switch (i.type) {
            case STRING:
              i.val_str = new char[val.length()];
              strcpy (i.val_str, val.c_str());
              break;
            case FLOAT:
              sscanf (val.c_str(), "%f", &(i.val_float));
              break;
            case INTEGER:
              sscanf (val.c_str(), "%d", &(i.val_int));
              break;
            case BOOL:
              // strip spaces
              while (*val.begin() == ' ')
                val.erase (val.begin());

              while (*--val.end() == ' ')
                val.erase (--val.end());

              if (val == "yes") {
                i.val_bool = true;
              } else {
                i.val_bool = false;
              }
          }

          items.push_back (i);
        }
      }

    }

    cf.close ();
    loaded = true;

  } else {
    cerr << "Config: No config file specified." << endl;
    throw LoadError ();
    return;
  }
}

void Config::load_config (const char *fname) {
  config_fname = fname;
  hasfname = true;
  load_config ();
}

bool Config::is_loaded () {
  return loaded;
}

void Config::print_config () {
  cout << "Configuration (" << config_fname << "):" << endl;
  for (int j = 0; j < (3 * 15); j++)
    cout << "-";
  cout << endl;
  for (vector<Item>::iterator i = items.begin (); i < items.end (); i++) {
    cout << setw (10) << get_type_s (i->type)
         << setw (15) << i->key << setw (15);

    switch (i->type) {
      case UNKNOWN: cout << "??" << endl; break;
      case STRING:  cout << i->val_str << endl; break;
      case FLOAT:   cout << i->val_float << endl; break;
      case INTEGER: cout << i->val_int << endl; break;
      case BOOL:    cout << i->val_bool << endl; break;
      default:      cout << "??" << endl; break;
    }

    for (int j = 0; j < (3 * 15); j++)
      cout << "-";
    cout << endl;

  }
}

string Config::get_type_s (Type t) {
  switch (t) {
    case UNKNOWN: return "UNKNOWN";
    case STRING:  return "STRING";
    case FLOAT:   return "FLOAT";
    case INTEGER: return "INTEGER";
    case BOOL:    return "BOOL";
    default:      return "????";
  }
}

string Config::get_string (const char *key) {
  string k = key;
  for (vector<Item>::iterator i = items.begin (); i < items.end(); i++) {
    if (k == i->key)
      if (i->type == STRING)
        return string (i->val_str);
  }

  cerr << "Config: No such key: " << key << " with type: string." << endl;
  throw NoSuchKeyException ();
}

float Config::get_float (const char *key) {
  string k = key;
  for (vector<Item>::iterator i = items.begin (); i < items.end(); i++) {
    if (k == i->key)
      if (i->type == FLOAT)
        return i->val_float;
  }

  cerr << "Config: No such key: " << key << " with type: float." << endl;
  throw NoSuchKeyException ();
}

int Config::get_int (const char *key) {
  string k = key;
  for (vector<Item>::iterator i = items.begin (); i < items.end(); i++) {
    if (k == i->key)
      if (i->type == INTEGER)
        return i->val_int;
  }

  cerr << "Config: No such key: " << key << " with type: integer." << endl;
  throw NoSuchKeyException ();
}

bool Config::get_bool (const char *key) {
  string k = key;
  for (vector<Item>::iterator i = items.begin (); i < items.end(); i++) {
    if (k == i->key)
      if (i->type == BOOL)
        return i->val_bool;
  }

  cerr << "Config: No such key: " << key << " with type: bool." << endl;
  throw NoSuchKeyException ();
}

