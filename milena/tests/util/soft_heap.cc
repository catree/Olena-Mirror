// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#include <mln/util/soft_heap.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/value/int_u8.hh>

using mln::point2d;

point2d p[] = { point2d(4,5), point2d(3,4), point2d(3,2),
		point2d(1,6), point2d(2,3), point2d(3,5),
		point2d(2,4), point2d(7,2), point2d(9,6),
		point2d(7,3) };

point2d res[] = { point2d(1,6), point2d(2,3), point2d(2,4),
		  point2d(3,2), point2d(3,4), point2d(3,5),
		  point2d(4,5), point2d(7,2), point2d(7,3),
		  point2d(9,6) };


int main()
{
  using namespace mln;

  util::soft_heap<point2d, value::int_u8> fh;

  for (unsigned i = 0; i < 5; ++i)
    fh.push(p[i]);

  util::soft_heap<point2d, value::int_u8> fh2;
  for (unsigned i = 5; i < 10; ++i)
    fh2.push(p[i]);

  // Merge fh in fh2.
  fh2.push(fh);

  // fh2 now holds both its elements and fh's.
  unsigned i = 0;
  while (!fh2.is_empty())
    mln_assertion(fh2.pop_front() == res[i++]);

  // fh must be empty, fh2 now holds its elements.
  mln_assertion(fh.is_empty());

  for (unsigned i = 5; i < 10; ++i)
    fh2.push(p[i]);

  fh2.clear();
  mln_assertion(fh2.nelements() == 0);
  mln_assertion(fh2.is_empty());
}
