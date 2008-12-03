// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_TREE_COMPUTE_ATTRIBUTE_IMAGE_HH
# define MLN_MORPHO_TREE_COMPUTE_ATTRIBUTE_IMAGE_HH

/// \file mln/morpho/tree/compute_attribute_image.hh
///
/// Compute a canonized tree from an image.
///
/// \todo Specialize for low quant (and try fastest).

# include <mln/core/concept/image.hh>
# include <mln/util/pix.hh>
# include <mln/level/fill.hh>



namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      /// Compute a tree with a parent relationship between sites.
      ///
      /// Warning: \p s translates the ordering related to the
      /// "natural" childhood relationship.  The parenthood is thus
      /// inverted w.r.t. to \p s.
      ///
      /// It is very convenient since all processing upon the parent
      /// tree are performed following \p s (in the default "forward"
      /// way).
      ///
      /// FIXME: Put it more clearly...
      ///
      /// The parent result image verifies: \n
      /// - p is root iff parent(p) == p \n
      /// - p is a node iff either p is root or f(parent(p)) != f(p).

      template <typename A, typename T>
      mln_ch_value(typename T::function, mln_result(A))
	compute_attribute_image(const Accumulator<A>& a, const T& t);



# ifndef MLN_INCLUDE_ONLY


      // Facade.

      template <typename A, typename T>
      inline
      mln_ch_value(typename T::function, mln_result(A))
      compute_attribute_image(const Accumulator<A>& a_, const T& t)
      {
	trace::entering("morpho::tree::compute_attribute_image");

	typedef typename T::function I;
	mln_ch_value(I, A) acc;
	initialize(acc, t.f());

	{
	  // Transmit "dynamic data" (state) of 'a' to every values of
	  // 'acc'.  It is usually a no-op (so useless) except for a
	  // few accumulators, e.g., for accu::rank which has the 'k'
	  // attribute.
	  A a = exact(a_);
	  level::fill(acc, a);
	}
	{
	  // Initialize every attribute with the corresponding pixel.
	  mln_piter(I) p(t.f().domain());
	  for_all(p)
	    acc(p).take_as_init(make::pix(t.f(), p));
	}
	{
	  mln_piter(T) p(t.domain());
	  // Propagate attribute from a site to its parent.
	  for_all(p)
	    if (! t.is_root(p))
	      acc(t.parent(p)).take(acc(p));
	  // Back-propagate attribute from a node to sites of its
	  // component.  Below, p is a non-node component site and
	  // parent(p) is a node, that is, the site representative of
	  // the component p belongs to.
	  for_all(p)
	    if (! t.is_a_node(p))
	      {
		mln_assertion(t.is_a_node(t.parent(p)));
		acc(p) = acc(t.parent(p));
	      }
	}

	typedef typename T::function I;
	mln_ch_value(I, mln_result(A)) output;
	initialize(output, acc);
	level::fill(output, acc);

	trace::exiting("morpho::tree::compute_attribute_image");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    }  // end of namespace mln::morpho::tree

  }  // end of namespace mln::morpho

}  // end of namespace mln


#endif // ! MLN_MORPHO_TREE_COMPUTE_ATTRIBUTE_IMAGE_HH