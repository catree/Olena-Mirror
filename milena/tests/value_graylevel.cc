// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

/*! \file tests/value_graylevel.cc
 *
 * \brief Tests on mln::value::graylevel. Tests operations between
 * graylevel of different encodings.
 *
 */


#include <mln/literal/black.hh>

#include <mln/literal/white.hh>

#include <mln/value/graylevel.hh>


int main()
{
  using namespace mln::value;

  using  mln::literal::white;
  using  mln::literal::black;

  // Constructions
  {
    gl8 x;

    gl8 a = 12;
    gl8 b(12);
    mln_assertion(a == b);

    gl16 c = 2335;
    gl8 d = c;
    mln_assertion(c == d);

    gl8 e = gray(white);
    mln_assertion(e == white);

    gl8 f = 12;
    gl8 g = f;
    gl8 h(f);

    mln_assertion(f == g);
    mln_assertion(h == g);
  }

  // Literals
  {
    gl8  a = white;
    gl16 b = white;

    mln_assertion(a == b);
    mln_assertion(a.value() == float(255));
    mln_assertion(b.value() == float(65535));
    mln_assertion(a == white);
    mln_assertion(b == white);

    gl8 c = (gl16(white) + white) / 2;
    std::cout << "gl8 c = (white + white) / 2; => c.value() == "
	      << int(c.value()) << std::endl;
    mln_assertion(c == white);
    mln_assertion(c.value() == float(255));

    c = (gl8(white) + white) / 2;
    mln_assertion(c == white);
    mln_assertion(c.value() == float(255));

    c = (gray(white) + white) / 2;
    mln_assertion(c == white);
    mln_assertion(c.value() == float(255));

    a = black;
    b = black;

    mln_assertion(a == b);
    mln_assertion(a.value() == float(0));
    mln_assertion(b.value() == float(0));

    c = (black + black) / 2;
    mln_assertion(c == black);
    mln_assertion(c.value() == float(0));
  }

  // Assigment
  {
    gl8 a;
    gl16 b;

    a = white;
    mln_assertion(a == white);
    mln_assertion(a.value() == float(255));

    a = 23;
    mln_assertion(a != white);
    mln_assertion(a != black);
    mln_assertion(a.value() == float(23));

    b = 2;
    mln_assertion(b != white);
    mln_assertion(b != black);
    mln_assertion(b.value() == float(2));

    a = b;
    mln_assertion(a.value() == float(2 / 257));

    signed char c = 51;
    a = c;
    mln_assertion(a.value() == float(51));

    // bounds
    a = 255;
    mln_assertion(a.value() == float(255));
    a = 0;
    mln_assertion(a.value() == float(0));
  }

  // Addition
  {
    gl8 a;
    gl16 b;

    // gl8 <- gl8 + gl8
    a = 42;
    a += a;
    mln_assertion(a.value() == float(84));

    a = 42;
    a = a + a;
    mln_assertion(a.value() == float(84));

    // gl8 <- gl8 + gl16
    a = 42;
    b = 16969;
    a = a + b;
    mln_assertion(a.value() == float((42 + b.value() / 257) ));
    a = 42;
    b = 16969;
    a += b;
    mln_assertion(a.value() == float((42 + b.value() / 257) ));


    // gl16 <- gl8 + gl16
    a = 42;
    b = 16969;
    b += a;
    mln_assertion(b.value() == float((42 * 257 + 16969) ));

    a = 42;
    b = 16969;
    b = a + b;
    mln_assertion(b.value() == float((42 * 257 + 16969) ));

    // misc
    a = 255;
    b = 0;
    a = a + b;
    mln_assertion(a.value() == float(255));

    a = 0;
    b = 65535;
    a = a + b;
    mln_assertion(a.value() == float(255));
  }


  // Soustraction
  {
    gl8 a;
    gl16 b;

    // gl8 <- gl8 - gl8
    a = 42;
    a -= a;
    mln_assertion(a == black);

    a = 42;
    a = a - a;
    mln_assertion(a == black);

    // gl8 <- gl8 - gl16
    a = 42;
    b = 5969;

    a = b;
    std::cout << "a.value() = " << int(a.value()) << std::endl;
    std::cout << "should be " << (b.value() / 257) << std::endl;

    a = 42;
    a = a - b;
    std::cout << int(a.value()) << ":" <<  (42 - b.value() / 257) << std::endl;
    mln_assertion(a.value() == float((42 - b.value() / 257) ));
    a = 42;
    b = 16969;
    a -= b;
    mln_assertion(a.value() == float((42 - b.value() / 257) ));


    // gl16 <- gl8 - gl16
    a = 100;
    b = 16969;
    b -= a;
    mln_assertion(b.value() == float((100 * 257 - 16969) ));

    a = 100;
    b = 16969;
    b = a - b;
    mln_assertion(b.value() == float((100 * 257 - 16969) ));

    // misc
    a = 255;
    b = 0;
    a = a - b;
    mln_assertion(a.value() == float(255));

    a = 0;
    b = 65535;
    a = a - b;
    mln_assertion(a.value() == float(255));

    // ...
    {
      graylevel<2> a = 2;
      graylevel<3> b = 5;
      graylevel<2> c;
      graylevel<3> d;

      c = a - b;
      d = a - b;
      mln_assertion(c == d);
    }

    {

      // ...
      gl8 a = 42;
      gl16 b = 5969;
      gl8 p;
      p = b;
      gl8 q;
      gl8 r;

      q = a - p;
      r = a - b;
      std::cout << int(q.value()) << " " << int(r.value()) << std::endl;
      mln_assertion(q == r);

    }

  }



  // Multiplication
  {
    gl8 a;
    gl16 b;

    // gl8 <- gl8 * gl8
    a = 21;
    a *= a;
    mln_assertion(a.value() == 42);

    a = 21;
    a = a * a;
    mln_assertion(a.value() == 42);

    // gl8 <- gl8 * gl16
    a = 10;
    b = 5969;
    a = a * b;
    mln_assertion(a.value() == float((10 * b.value() / 257) ));

    a = 10;
    b = 16969;
    a *= b;
    mln_assertion(a.value() == float((10 * b.value() / 257) ));


    // gl16 <- gl8 * gl16
    a = 10;
    b = 5969;
    b *= a;
    mln_assertion(b.value() == float((10 * 257 * 5969) ));

    a = 10;
    b = 5969;
    b = a * b;
    mln_assertion(b.value() == float((10 * 257 * 5969) ));

    // misc
    a = 255;
    b = 0;
    a = a * b;
    mln_assertion(a == black);

    a = 0;
    b = 65535;
    a = a * b;
    mln_assertion(a == black);

    // ...
    {
      graylevel<2> a = 2;
      graylevel<3> b = 5;
      graylevel<2> c;
      graylevel<3> d;

      c = a * b;
      d = a * b;
      mln_assertion(c == d);
    }

    {

      // ...
      gl8 a = 7;
      gl16 b = 5969;

      gl8 p;
      p = b;

      gl8 q;
      gl8 r;

      q = a * p;
      r = a * b;
      std::cout << int(q.value()) << " " << int(r.value()) << std::endl;
      mln_assertion(q == r);

    }

  }

  // Not exhaustive

  //   gl8  a = white;
  //   gl16 b = white;
  //   mln_assertion(a == b);
  //   //  mln_assertion(-a == -b);

  //   mln_assertion(a.value() != b.value());

  //   {
  //     gl8 c = (a + b) / 2;
  //     mln_assertion(c == white);
  //     mln_assertion(c.value() == 255);

  //     c = a;
  //     mln_assertion(c == white);

  //     c = (a * 2) / 2;
  //     mln_assertion(c == white);

  //     c = c / 6;
  //   }

  //   {
  //     gl8 c = (white + white) / 2;
  //     mln_assertion(c == white);
  //     mln_assertion(c.value() == float(255));
  //   }

  //   gray g = black;
}
