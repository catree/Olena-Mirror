// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_RECONSTRUCTIONS_ON_FUNCTION_HH
# define MLN_RECONSTRUCTIONS_ON_FUNCTION_HH

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/level/fill.hh>
# include <mln/level/compare.hh>
# include <mln/level/paste.hh>
# include <mln/level/sort_psites.hh>
# include <mln/literal/zero.hh>
# include <mln/math/max.hh>
# include <mln/math/min.hh>

# include "canvas/reconstruction_on_function.hh"

namespace mln
{

  namespace impl
  {

    template <typename I_, typename J_>
    struct reconstruction_on_function_by_dilation_t
    {
      typedef I_ I;
      typedef J_ J;
      typedef mln_site(I) P;
      typedef p_array<mln_psite(I)> S;

      reconstruction_on_function_by_dilation_t (const I& marker, const J& mask, mln_concrete(I)& output)
	: marker(marker),
	  mask(mask),
	  output(output),
	  s(level::sort_psites_decreasing(marker))
      {
      }

      bool is_active(const P& p) { return mask(p) >= output(p); }
      void set_default_output() { level::fill(output, literal::zero); }
      void init(const P& p) { output(p) = marker(p); }
      void merge(const P& r, const P& p) { output(p) = math::max(output(p), output(r)); }
      void visit_ext_border(const P& n, const P& p) {	(void) n; (void) p; }

      void set_output_value(const P& p)
      {
	if (!is_active(p))
	  output(p) = mask(p);
      }

      const I& marker; // F
      const J& mask; // G
      mln_concrete(I)& output; // O
      S s;
    };


    template <typename I_, typename J_>
    struct reconstruction_on_function_by_erosion_t
    {
      typedef I_ I;
      typedef J_ J;
      typedef mln_site(I) P;
      typedef p_array<mln_psite(I)> S;

      reconstruction_on_function_by_erosion_t (const I& marker, const J& mask, mln_concrete(I)& output)
	: marker(marker),
	  mask(mask),
	  output(output),
	  s(level::sort_psites_increasing(marker))
      {
      }

      bool is_active(const P& p) { return mask(p) <= output(p); }
      void set_default_output() { level::fill(output, literal::zero); }
      void init(const P& p) { output(p) = marker(p); }
      void merge(const P& r, const P& p) { output(p) = math::min(output(p), output(r)); }
      void visit_ext_border(const P& n, const P& p) {	(void) n; (void) p; }

      void set_output_value(const P& p)
      {
	if (!is_active(p))
	  output(p) = mask(p);
      }

      const I& marker; // F
      const J& mask; // G
      mln_concrete(I)& output; // O
      S s;
    };


  } // end of namespace mln::impl.

  template <typename I, typename J, typename N>
  mln_concrete(I)
  reconstruction_on_function_by_dilation(const Image<I>& marker_,
					 const Image<J>& mask_,
					 const Neighborhood<N>& nbh_)
  {
    trace::entering("morpho::reconstruction_on_function_by_dilation");

    const I& marker = exact(marker_);
    const J& mask   = exact(mask_);
    const N& nbh    = exact(nbh_);

    mln_precondition(marker.has_data());
    mln_precondition(mask.has_data());
    mln_precondition(mask.domain() == marker.domain());

    mln_concrete(I) output;
    initialize(output, marker);

    typedef impl::reconstruction_on_function_by_dilation_t<I, J> F;
    F f(marker, mask, output);
    canvas::morpho::reconstruction_on_function(nbh, f);

    mln_precondition(marker <= output && output <= mask);

    trace::exiting("morpho::reconstruction_on_function_by_dilation");
    return output;
  }


  template <typename I, typename J, typename N>
  mln_concrete(I)
  reconstruction_on_function_by_erosion(const Image<I>& marker_,
					const Image<J>& mask_,
					const Neighborhood<N>& nbh_)
  {
    trace::entering("morpho::reconstruction_on_function_by_dilation");

    const I& marker = exact(marker_);
    const J& mask   = exact(mask_);
    const N& nbh    = exact(nbh_);

    mln_precondition(marker.has_data());
    mln_precondition(mask.has_data());
    mln_precondition(mask.domain() == marker.domain());

    mln_concrete(I) output;
    initialize(output, marker);

    typedef impl::reconstruction_on_function_by_erosion_t<I, J> F;
    F f(marker, mask, output);
    canvas::morpho::reconstruction_on_function(nbh, f);

    mln_precondition(marker >= output && output >= mask);

    trace::exiting("morpho::reconstruction_on_function_by_dilation");
    return output;
  }

} // end of namespace mln.


#endif // ! MLN_RECONSTRUCTIONS_ON_FUNCTION_HH
