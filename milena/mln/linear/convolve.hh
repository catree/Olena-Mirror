// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_LINEAR_CONVOLVE_HH
# define MLN_LINEAR_CONVOLVE_HH

/*! \file mln/linear/convolve.hh
 *
 * \brief Convolution.
 *
 * \todo Introduce an accumulator.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/border/resize.hh>
# include <mln/border/duplicate.hh>



namespace mln
{

  namespace linear
  {

    /*! Convolution of an image \p input by the weighted window \p w_win.
     *
     * \warning Computation of \p output(p) is performed with the
     * value type of \p output.
     *
     * \warning The weighted window is used as-is, considering that
     * its symmetrization is handled by the client.
     *
     * \pre output.domain = input.domain
     */
    template <typename I, typename W>
    mln_concrete(I)
    convolve(const Image<I>& input, const Weighted_Window<W>& w_win);



# ifndef MLN_INCLUDE_ONLY

    // Tests.

    namespace internal
    {

      template <typename I, typename W>
      void
      convolve_tests(const Image<I>& input,
		     const Weighted_Window<W>& w_win)
      {
	mln_precondition(exact(input).has_data());
	// mln_precondition(exact(w_win).is_valid());
      }

    }  // end of namespace mln::linear::internal


    // Implementation.

    namespace impl
    {

      namespace generic
      {

	template <typename I, typename W>
	mln_concrete(I)
	convolve(const Image<I>& input_,
		 const Weighted_Window<W>& w_win_)
	{
	  trace::entering("linear::impl::generic::convolve");

	  const I& input = exact(input_);
	  const W& w_win = exact(w_win_);
	  internal::convolve_tests(input, w_win);

	  // extension::adjust_duplicate(input, w_win);
	  
	  typedef mln_concrete(I) O;
	  O output;
	  initialize(output, input);

	  mln_piter(I) p(input.domain());
	  mln_qiter(W) q(w_win, p);

	  for_all(p)
	  {
	    mln_value(O) v = literal::zero;
	    for_all(q) if (input.has(q))
	      v += input(q) * q.w();
	    output(p) = v;
	  }
	  
	  trace::exiting("linear::impl::generic::convolve");
	  return output;
	}

      } // end of namespace mln::linear::impl::generic


      template <typename I, typename W>
      mln_concrete(I)
      convolve_fastest(const Image<I>& input_,
		       const Weighted_Window<W>& w_win_)
      {
	trace::entering("linear::impl::convolve_fastest");

	const I& input = exact(input_);
	const W& w_win = exact(w_win_);
	internal::convolve_tests(input, w_win);

	// extension::adjust_duplicate(input, w_win);

	typedef mln_concrete(I) O;
	O output;
	initialize(output, input);
 	mln_pixter(O) p_out(output);

	mln_pixter(const I)    p(input);
	mln_qixter(const I, W) q(p, w_win);

 	for_all_2(p, p_out)
	  {
	    mln_value(O) v = literal::zero;
	    unsigned i = 0;
	    for_all(q)
	      v += w_win.w(i++) * q.val();
 	    p_out.val() = v;
	  }

	trace::exiting("linear::impl::convolve_fastest");
	return output;
      }

    } // end of namespace mln::linear::impl


    // Dispatch.

    namespace internal
    {

      template <typename I, typename W>
      mln_concrete(I)
      convolve_dispatch(trait::image::speed::any,
			const Image<I>& input,
			const Weighted_Window<W>& w_win)
      {
	return impl::generic::convolve(input, w_win);
      }

      template <typename I, typename W>
      mln_concrete(I)
      convolve_dispatch(trait::image::speed::fastest,
			const Image<I>& input,
			const Weighted_Window<W>& w_win)
      {
	return impl::convolve_fastest(input, w_win);
      }

      template <typename I, typename W>
      mln_concrete(I)
      convolve_dispatch(const Image<I>& input,
			const Weighted_Window<W>& w_win)
      {
	return convolve_dispatch(mln_trait_image_speed(I)(),
				 input, w_win);
      }

    }  // end of namespace mln::linear::internal


    // Facade.

    template <typename I, typename W>
    mln_concrete(I)
    convolve(const Image<I>& input, const Weighted_Window<W>& w_win)
    {
      trace::entering("linear::convolve");

      internal::convolve_tests(input, w_win);

      mln_concrete(I) output;
      output = internal::convolve_dispatch(mln_trait_image_speed(I)(),
					   input, w_win);

      trace::exiting("linear::convolve");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::linear

} // end of namespace mln


#endif // ! MLN_LINEAR_CONVOLVE_HH
