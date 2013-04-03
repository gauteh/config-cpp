/* config.cpp: A simple configuration reader class
 *
 * Author: Gaute Hope <eg@gaute.vetsj.com>
 * Date:   2012-08-27
 *
 */

# include <iostream>
# include <iomanip>
# include <fstream>
# include <string>
# include <string.h>

# include "config.h"

using namespace std;

Config::Config () {
  config_fname = "";
}

Config::Config (const char * fname) {
  config_fname = fname;
  hasfname = true;
}

void Config::load_config () {
  if (hasfname) {
    ifstream cf (config_fname.c_str());

    if (cf.bad () || !cf.is_open ()) {
      cerr << "Config: Could not open file: " << config_fname << endl;
      return;
    }

    while (!cf.eof()) {
      char line[1024];
      cf.getline (line, 1024);

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
              continue;
            }
        }

        int delim = sline.find ('=');
        if (delim < 1) {
          cerr << "Config: Could not parse line: " << sline << endl;
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
              // strip
              for (auto i = val.begin(); i < val.end (); i++)
                if (*i == ' ') val.erase(i);
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

  } else {
    cerr << "Config: No config file specified." << endl;
    return;
  }
}

void Config::load_config (const char *fname) {
  config_fname = fname;
  hasfname = true;
  load_config ();
}

void Config::print_config () {
  cout << "Configuration (" << config_fname << "):" << endl;
  for (int j = 0; j < (3 * 15); j++)
    cout << "-";
  cout << endl;
  for (auto i = items.begin (); i < items.end (); i++) {
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
  return string ("");
}

float Config::get_float (const char *key) {
  string k = key;
  for (vector<Item>::iterator i = items.begin (); i < items.end(); i++) {
    if (k == i->key)
      if (i->type == FLOAT)
        return i->val_float;
  }

  cerr << "Config: No such key: " << key << " with type: float." << endl;
  return .0;
}

int Config::get_int (const char *key) {
  string k = key;
  for (vector<Item>::iterator i = items.begin (); i < items.end(); i++) {
    if (k == i->key)
      if (i->type == INTEGER)
        return i->val_int;
  }

  cerr << "Config: No such key: " << key << " with type: integer." << endl;
  return 0;
}

bool Config::get_bool (const char *key) {
  string k = key;
  for (vector<Item>::iterator i = items.begin (); i < items.end(); i++) {
    if (k == i->key)
      if (i->type == BOOL)
        return i->val_bool;
  }

  cerr << "Config: No such key: " << key << " with type: bool." << endl;
  return false;
}

