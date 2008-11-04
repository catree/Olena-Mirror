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

#ifndef MLN_MORPHO_HIT_OR_MISS_HH
# define MLN_MORPHO_HIT_OR_MISS_HH

/*! \file mln/morpho/hit_or_miss.hh
 *
 * \brief Morphological gradient.
 *
 * \todo Save memory.
 */

# include <mln/morpho/includes.hh>
# include <mln/arith/revert.hh>

namespace mln
{

  namespace morpho
  {

    /*! Morphological hit or miss.
     *
     */
    template <typename I, typename Wout, typename Win>
    mln_concrete(I) hit_or_miss(const Image<I>& input, const Window<Wout>& win_out, const Window<Win>& win_in);


    // FIXME : epaississement et amincissement


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename Wout, typename Win>
    inline
    mln_concrete(I) hit_or_miss(const Image<I>& input, const Window<Wout>& win_out, const Window<Win>& win_in)
    {
      trace::entering("morpho::hit_or_miss");
      mln_precondition(exact(input).has_data());
      mln_precondition(! exact(win_out).is_empty());
      mln_precondition(! exact(win_in).is_empty());

      mln_concrete(I) output = morpho::min(erosion(arith::revert(input), win_out),
					   erosion(input, win_in));

      mln_postcondition(test::positive(output));
      trace::exiting("morpho::hit_or_miss");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_HIT_OR_MISS_HH