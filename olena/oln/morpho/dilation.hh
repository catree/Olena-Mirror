// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this filek as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_MORPHO_DILATION_HH
# define OLENA_MORPHO_DILATION_HH

# include <oln/basics.hh>
# include <oln/morpho/stat.hh>
# include <oln/morpho/fast_morpho.hh>
# include <mlc/cmp.hh>

namespace oln {

  namespace morpho {

    /*=processing dilation
     * ns: morpho, morpho::fast
     * what: Morphological dilation.
     * arg: const abstract::image<I>&, input, IN, input image
     * arg: const abstract::struct_elt<E>&, se, IN, structural element
     * ret: Concrete(I)
     * doc:
     *   Compute the morphological dilation of \var{input} using \var{se}
     *   as structural element.
     *
     *   On grey-scale images, each point is replaced by the maximum value
     *   of its neighbors, as indicated by \var{se}.  On binary images,
     *   a logical \code{or} is performed between neighbors.
     *
     *   The \code{morpho::fast} version of this function use a different
     *   algorithm: This algorith; is described in
     * Implementation of morphological operations from:
     * M. Van Droogenbroeck and H. Talbot.
     * "Fast computation of morphological operations with arbitrary
     * structuring elements". Pattern Recognition Letters,
     * 17(14):1451-1460, 1996.
     *
     *   An
     *   histogram of the value of the neighborhood indicated by
     *   \var{se} is updated while iterating over all point of the
     *   image.  Doing so is more efficient  when the
     *   structural element is large.
     *
     * see: morpho::n_dilation
     * see: morpho::erosion
     * ex:
     * $ image2d<ntg::bin> im = load("object.pbm");
     * $ save(morpho::dilation(im, win_c8p()), "out.pbm");
     * exi: object.pbm
     * exo: out.pbm
    =*/
    template<class I, class E>
    Concrete(I) dilation(const abstract::image<I> &input, const abstract::struct_elt<E>& se)
    {
      mlc::eq<I::dim, E::dim>::ensure();

      Concrete(I) output(input.size());
      input.border_adapt_copy(se.delta());
      Iter(I) p(input);

      for_all (p)
	output[p] = morpho::max(input, p, se);
      return output;
    }

    /*=processing n_dilation
     * ns: morpho
     * what: Morphological dilation itered n times.
     * arg: const abstract::image<I>&, input, IN, input image
     * arg: const abstract::struct_elt<E>&, se, IN, structural element
     * arg: unsigned, n, IN, number of iterations
     * ret: Concrete(I)
     * doc:
     *   Apply \code{morpho::dilation} \var{n} times.
     * see: morpho::dilation
     * see: morpho::n_erosion
    =*/
    template<class I, class E>
    Concrete(I) n_dilation(const abstract::image<I> & input,
			   const abstract::struct_elt<E>& se,
			   unsigned n)
    {
      precondition(n > 0);
      Concrete(I) output = input.clone();
      for (unsigned i = 0; i < n; ++i)
	{
	  Concrete(I) work = dilation(output, se);
	  output = work;
 	}
      return output;
    }

    namespace fast {
      template<class I, class E>
      Concrete(I) dilation(const abstract::image<I>& input, const abstract::struct_elt<E>& se)
      {
	return fast_morpho<I, E, utils::histogram_max>(input, se);
      }
    }
  } // end of morpho

} // end of oln

#endif // ! OLENA_MORPHO_DILATION_HH
