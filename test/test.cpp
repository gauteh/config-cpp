/* test.cpp: A simple test of the configuration reader class
 *
 * Author: Gaute Hope <eg@gaute.vetsj.com>
 * Date:   2014-01-13
 *
 *  Scons is used for building.
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

# include "config.hpp"

using namespace std;

int main (int argc, char ** argv)
{
  const char * conffile = "test.conf";

  cout << "Test program for the small config file library" << endl << endl;

  cout << "=> Loading config.." << endl;
  Config *c = new Config (conffile);

  c->load_config ();
  c->print_config ();

  if (c->get_bool("calculate_area")) {
    cout << "=> Calculating area.." << endl;

    float radius = c->get_float ("radius");
    float pi     = c->get_float ("pi");

    string name  = c->get_string ("name");

    int no       = c->get_int ("number");

    float area   = radius * radius * pi * (float) no;

    cout << "Area of " << no << " " << name << " is: " << area << endl;
  }

  return 0;
}

