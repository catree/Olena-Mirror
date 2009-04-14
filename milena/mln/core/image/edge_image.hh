// Copyright (C) 2009 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_IMAGE_EDGE_IMAGE_HH
# define MLN_CORE_IMAGE_EDGE_IMAGE_HH

/// \file mln/core/image/edge_image.hh
///
/// Image based on graph edges.

# include <mln/core/concept/graph.hh>
# include <mln/core/site_set/p_edges.hh>
# include <mln/core/image/graph_elt_window.hh>
# include <mln/core/image/graph_elt_neighborhood.hh>
# include <mln/pw/internal/image_base.hh>
# include <mln/fun/i2v/array.hh>



namespace mln
{
  // Forward declaration.
  template <typename P, typename V, typename G> class edge_image;
  namespace util { class graph; }


  // Properties

  namespace trait
  {

    template <typename P, typename V, typename G>
    struct image_< edge_image<P,V,G> >
      : pw_image_<fun::i2v::array<V>,
		  p_edges<util::graph, fun::i2v::array<P> >,
		  edge_image<P,V,G> >
    {
    };

  } // end of namespace mln::traits.



  namespace internal
  {

    /// Data structure for mln::pw::internal::image
    template <typename P, typename V, typename G>
    struct data< mln::edge_image<P,V,G> >
    {
      data(const fun::i2v::array<V>& edge_values,
	   const p_edges<G,fun::i2v::array<P> >& pv);

      fun::i2v::array<V> f_;
      p_edges<G,fun::i2v::array<P> > pset_;
    };

  } // end of namespace mln::internal




  /// Construct a edge image from a fun::i2v::array and a p_edges.
  /// image = fun::i2v::array | p_edges.
  template <typename V, typename G, typename P>
  edge_image<P,V,G>
  operator | (const fun::i2v::array<V>& edge_values,
	      const p_edges<G,fun::i2v::array<P> >& pv);




  template <typename P, typename V, typename G = util::graph>
  class edge_image
    : public pw::internal::image_base<fun::i2v::array<V>,
				      p_edges<G, fun::i2v::array<P> >,
				      edge_image<P,V,G> >
  {
    typedef pw::internal::image_base<fun::i2v::array<V>,
				     p_edges<G, fun::i2v::array<P> >,
				     edge_image<P,V,G> > super_;

  public:
    /// Skeleton type.
    typedef edge_image< tag::psite_<P>,
			tag::value_<V>,
			tag::graph_<G> > skeleton;

    /// Function mapping graph elements to sites.
    typedef fun::i2v::array<P> site_function_t;

    /// Window type
    typedef graph_elt_window<G,p_edges<G,site_function_t> > win_t;
    /// Neighborhood type.
    typedef graph_elt_neighborhood<G,p_edges<G,site_function_t> > nbh_t;

    /// Constructors.
    /// @{
    edge_image();

    edge_image(const Graph<G>& g,
	       const Function_i2v< fun::i2v::array<P> >& edge_sites,
	       const Function_i2v< fun::i2v::array<V> >& edge_values);

    edge_image(const p_edges<G,fun::i2v::array<P> >& pe,
	       const Function_i2v< fun::i2v::array<V> >& edge_values);

    template <typename FP, typename FV>
    edge_image(const Graph<G>& g,
	       const Function_i2v<FP>& edge_sites,
	       const Function_i2v<FV>& edge_values);

    template <typename FV>
    edge_image(const p_edges<G,fun::i2v::array<P> >& pv,
	       const Function_i2v<FV>& edge_values);
    /// @}

  };

  template <typename P, typename V, typename G, typename J>
  void init_(tag::image_t, edge_image<P,V,G>& target, const Image<J>& model);


# ifndef MLN_INCLUDE_ONLY

  template <typename P, typename V, typename G, typename J>
  void init_(tag::image_t, edge_image<P,V,G>& target, const Image<J>& model)
  {
    fun::i2v::array<V> f;
    init_(tag::function, f, exact(model));
    p_edges<G,fun::i2v::array<P> > s;
    init_(tag::domain, s, exact(model));
    target.init_(f, s);
  }

  // Operator.

  template <typename V, typename G, typename P>
  inline
  edge_image<P,V,G>
  operator | (const fun::i2v::array<V>& edge_values,
	      const p_edges<G,fun::i2v::array<P> >& pv)
  {
    edge_image<P,V,G> tmp(edge_values, pv);
    return tmp;
  }



  // data< pw::image >

  namespace internal
  {

    template <typename P, typename V, typename G>
    inline
    data< mln::edge_image<P,V,G> >::data(const fun::i2v::array<V>& f,
					 const p_edges<G,fun::i2v::array<P> >& ps)
      : f_(exact(f)),
	pset_(ps)
    {
    }

  } // end of namespace mln::internal



  // vertex_image<P,V,G>
  template <typename P, typename V, typename G>
  inline
  edge_image<P,V,G>::edge_image()
  {
  }


  template <typename P, typename V, typename G>
  inline
  edge_image<P,V,G>::edge_image(const Graph<G>& g,
				const Function_i2v< fun::i2v::array<P> >& edge_sites,
				const Function_i2v< fun::i2v::array<V> >& edge_values)
    : super_(exact(edge_values),
	     p_edges<G,fun::i2v::array<P> >(g, exact(edge_sites)))
  {
  }


  template <typename P, typename V, typename G>
  inline
  edge_image<P,V,G>::edge_image(const p_edges<G,fun::i2v::array<P> >& pv,
				const Function_i2v< fun::i2v::array<V> >& edge_values)
    : super_(exact(edge_values), pv)
  {
  }


  template <typename P, typename V, typename G>
  template <typename FP, typename FV>
  inline
  edge_image<P,V,G>::edge_image(const Graph<G>& g,
				const Function_i2v<FP>& edge_sites,
				const Function_i2v<FV>& edge_values)
    : super_(convert::to<fun::i2v::array<V> >(exact(edge_values)),
	     p_edges<G,fun::i2v::array<P> >(g, exact(edge_sites)))
  {
    mlc_converts_to(FV,fun::i2v::array<V>)::check();
    mlc_equal(mln_result(FP),P)::check();
    mlc_equal(mln_result(FV),V)::check();
  }



  template <typename P, typename V, typename G>
  template <typename FV>
  inline
  edge_image<P,V,G>::edge_image(const p_edges<G,fun::i2v::array<P> >& pv,
				const Function_i2v<FV>& edge_values)
    : super_(convert::to<fun::i2v::array<V> >(exact(edge_values)), pv)
  {
    mlc_converts_to(FV,fun::i2v::array<V>)::check();
    mlc_equal(mln_result(FV),V)::check();
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_EDGE_IMAGE_HH