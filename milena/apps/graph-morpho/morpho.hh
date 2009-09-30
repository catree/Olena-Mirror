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

#ifndef APPS_GRAPH_MORPHO_MORPHO_HH
# define APPS_GRAPH_MORPHO_MORPHO_HH

/** \file apps/graph-morpho/morpho.hh
    \brief Morphological operators on graphs.

    Reference:

      Jean Cousty, Laurent Najman and Jean Serra.  Some morphological
      operators in graph spaces.  In: Proceedings of the Ninth
      International Symposium on Mathematical Morphology (ISMM), 2009,
      Groningen, The Netherlands.  */

# include <mln/core/alias/complex_image.hh>
# include <mln/core/image/image2d.hh>

# include <mln/core/image/dmorph/image_if.hh>

# include <mln/core/image/dmorph/extension_ima.hh>

# include <mln/core/routine/extend.hh>
# include <mln/core/routine/duplicate.hh>

# include <mln/core/site_set/p_n_faces_piter.hh>
# include <mln/core/image/complex_neighborhoods.hh>
# include <mln/core/image/complex_neighborhood_piter.hh>

# include <mln/world/inter_pixel/dim2/is_pixel.hh>
# include <mln/world/inter_pixel/dim2/is_edge.hh>
# include <mln/world/inter_pixel/neighb2d.hh>

# include <mln/data/paste.hh>

# include <mln/morpho/dilation.hh>
# include <mln/morpho/erosion.hh>

# include <mln/topo/is_n_face.hh>

// FIXME: Instead of providing several implementation, move specific
// parts (neighborhoods, etc.) to a graph_traits class, and write
// generic version of combine, dilation_e2v, erosion_v2e, etc.


/*----------------------------.
| Vertices-edges combinator.  |
`----------------------------*/

namespace impl
{
  // ------------------------------------------ //
  // Implementations on (general) 2-complexes.  //
  // ------------------------------------------ //

  /// Combine the vertices and the edges of two
  /// mln::bin_1complex_image2d images to create a new graph image
  /// (``operator'' \f$\ovee\f$)
  inline
  mln::bin_1complex_image2d
  combine(const mln::bin_1complex_image2d& vertices,
	  const mln::bin_1complex_image2d& edges)
  {
    mln_precondition(vertices.domain() == edges.domain());

    mln::bin_1complex_image2d output;
    mln::initialize(output, vertices);
    typedef mln_geom_(mln::bin_1complex_image2d) geom_t;
    mln::p_n_faces_fwd_piter<1, geom_t> v(output.domain(), 0);
    for_all(v)
      output(v) = vertices(v);
    mln::p_n_faces_fwd_piter<1, geom_t> e(output.domain(), 1);
    for_all(e)
      output(e) = edges(e);
    return output;
  }

  // ------------------------------------------------------------- //
  // Implementations on (mln::image2d-based) cubical 2-complexes.  //
  // ------------------------------------------------------------- //

  /// Combine the vertices and the edges of two
  /// mln::image2d<T> images to create a new graph image
  /// (``operator'' \f$\ovee\f$)
  template <typename T>
  inline
  mln::image2d<T>
  combine(const mln::image2d<T>& vertices,
	  const mln::image2d<T>& edges)
  {
    mln_precondition(vertices.domain() == edges.domain());
    mln::image2d<T> output;
    mln::initialize(output, vertices);
    mln::data::fill(output, false);
    mln::data::paste(vertices | mln::world::inter_pixel::dim2::is_pixel(),
		     output);
    mln::data::paste(edges | mln::world::inter_pixel::dim2::is_edge(),
		     output);
    return output;
  }
}

// -------- //
// Facade.  //
// -------- //

/// Combine the vertices and the edges of two images to create a new
/// graph image (``operator'' \f$\ovee\f$).
template <typename I>
inline
mln_concrete(I)
combine(const mln::Image<I>& vertices, const mln::Image<I>& edges)
{
  return impl::combine(mln::exact(vertices), mln::exact(edges));
}


/*-------------------------.
| Dilations and erosions.  |
`-------------------------*/

namespace impl
{
  // ------------------------------------------ //
  // Implementations on (general) 2-complexes.  //
  // ------------------------------------------ //

  /* FIXME: By constraining the domain of the input and passing the
     neighborhood, one should be able to use a truly generic dilation
     (resp. erosion), or even use Milena's standard morpho::dilation
     (resp. morpho::erosion).
  
     It'd be convenient to write something like this:
  
       dilation(ima | vertices);
  
     We can /actually/ write this, but `vertices' has to be a predicate
     on sites (p2b function), which is not efficient, since both
     vertices and edges will be browsed.
  
     It would be very nice if `vertices' could be an actual site set,
     so that `ima | vertices' creates a morpher smart enough to
     browse /only/ vertices.  */

  /// Dilation from edges to vertices (\f$\delta^\bullet\f$) on
  /// mln::bin_1complex_image2d.
  inline
  mln::bin_1complex_image2d
  dilation_e2v(const mln::bin_1complex_image2d& input)
  {
    mln::topo::is_n_face<0> is_0_face;
    typedef mln_geom_(mln::bin_1complex_image2d) geom_t;
    typedef mln::complex_higher_window<1, geom_t> v2e_t;
    const v2e_t v2e;
    mln::bin_1complex_image2d output(input.domain());
    mln::data::fill(output, false);
    mln::data::paste(mln::morpho::dilation(mln::extend(input | is_0_face,
    						       input),
    					   v2e),
    		     output);
    return output;
  }

  /// Erosion from vertices to edges (\f$\epsilon^\times\f$) on
  /// mln::bin_1complex_image2d.
  inline
  mln::bin_1complex_image2d
  erosion_v2e(const mln::bin_1complex_image2d& input)
  {
    mln::topo::is_n_face<1> is_1_face;
    typedef mln_geom_(mln::bin_1complex_image2d) geom_t;
    typedef mln::complex_lower_window<1, geom_t> e2v_t;
    const e2v_t e2v;
    mln::bin_1complex_image2d output(input.domain());
    mln::data::fill(output, false);
    mln::data::paste(mln::morpho::erosion(mln::extend(input | is_1_face,
						      input),
					  e2v),
    		     output);
    return output;
  }

  /// Erosion from edges to vertices (\f$\epsilon^\bullet\f$) on
  /// mln::bin_1complex_image2d.
  inline
  mln::bin_1complex_image2d
  erosion_e2v(const mln::bin_1complex_image2d& input)
  {
    mln::topo::is_n_face<0> is_0_face;
    typedef mln_geom_(mln::bin_1complex_image2d) geom_t;
    typedef mln::complex_higher_window<1, geom_t> v2e_t;
    const v2e_t v2e;
    mln::bin_1complex_image2d output(input.domain());
    mln::data::fill(output, false);
    mln::data::paste(mln::morpho::erosion(mln::extend(input | is_0_face,
						      input),
					  v2e),
    		     output);
    return output;
  }

  /// Dilation from vertices to edges (\f$\delta^\times\f$) on
  /// mln::bin_1complex_image2d.
  inline
  mln::bin_1complex_image2d
  dilation_v2e(const mln::bin_1complex_image2d& input)
  {
    mln::topo::is_n_face<1> is_1_face;
    typedef mln_geom_(mln::bin_1complex_image2d) geom_t;
    typedef mln::complex_lower_window<1, geom_t> e2v_t;
    const e2v_t e2v;
    mln::bin_1complex_image2d output(input.domain());
    mln::data::fill(output, false);
    mln::data::paste(mln::morpho::dilation(mln::extend(input | is_1_face,
						      input),
					  e2v),
    		     output);
    return output;
  }

  // ------------------------------------------------------------- //
  // Implementations on (mln::image2d-based) cubical 2-complexes.  //
  // ------------------------------------------------------------- //

  /// Dilation from edges to vertices (\f$\delta^\bullet\f$) on
  /// an mln::image2d<T>-based cubical complex.
  template <typename T>
  inline
  mln::image2d<T>
  dilation_e2v(const mln::image2d<T>& input)
  {
    using mln::world::inter_pixel::dim2::is_pixel;
    using mln::world::inter_pixel::v2e;

    mln::image2d<T> output(input.domain());
    mln::data::fill(output, false);
    mln::data::paste(mln::morpho::dilation(mln::extend(input | is_pixel(),
						       input),
					   v2e().win()),
		     output);
    return output;
  }

  /// Erosion from vertices to edges (\f$\epsilon^\times\f$) on an
  /// mln::image2d<T>-based cubical complex.
  template <typename T>
  inline
  mln::image2d<T>
  erosion_v2e(const mln::image2d<T>& input)
  {
    using mln::world::inter_pixel::dim2::is_edge;
    using mln::world::inter_pixel::e2v;

    mln::image2d<T> output(input.domain());
    mln::data::fill(output, false);
    mln::data::paste(mln::morpho::erosion(mln::extend(input | is_edge(),
						      input),
					  e2v().win()),
		     output);
    return output;
  }

  /// Erosion from edges to vertices (\f$\epsilon^\bullet\f$) on an
  /// mln::image2d<T>-based cubical complex.
  template <typename T>
  inline
  mln::image2d<T>
  erosion_e2v(const mln::image2d<T>& input)
  {
    using mln::world::inter_pixel::dim2::is_pixel;
    using mln::world::inter_pixel::v2e;

    mln::image2d<T> output(input.domain());
    mln::data::fill(output, false);
    mln::data::paste(mln::morpho::erosion(mln::extend(input | is_pixel(),
						      input),
					  v2e().win()),
		     output);
    return output;
  }

  /// Dilation from vertices to edges (\f$\delta^\times\f$) on an
  /// mln::image2d<T>-based cubical complex.
  template <typename T>
  inline
  mln::image2d<T>
  dilation_v2e(const mln::image2d<T>& input)
  {
    using mln::world::inter_pixel::dim2::is_edge;
    using mln::world::inter_pixel::e2v;

    mln::image2d<T> output(input.domain());
    mln::data::fill(output, false);
    mln::data::paste(mln::morpho::dilation(mln::extend(input | is_edge(),
						       input),
					   e2v().win()),
		     output);
    return output;
  }

}


// ------------------------------------------ //
// Facades of (core) dilations and erosions.  //
// ------------------------------------------ //

/// Dilation from edges to vertices (\f$\delta^\bullet\f$).
template <typename I>
inline
mln_concrete(I)
dilation_e2v(const mln::Image<I>& input)
{
  return impl::dilation_e2v(mln::exact(input));
}

/// Erosion from vertices to edges (\f$\epsilon^\times\f$).
template <typename I>
inline
mln_concrete(I)
erosion_v2e(const mln::Image<I>& input)
{
  return impl::erosion_v2e(mln::exact(input));
}

/// Erosion from edges to vertices (\f$\epsilon^\bullet\f$).
template <typename I>
inline
mln_concrete(I)
erosion_e2v(const mln::Image<I>& input)
{
  return impl::erosion_e2v(mln::exact(input));
}

/// Dilation from vertices to edges (\f$\delta^\times\f$).
template <typename I>
inline
mln_concrete(I)
dilation_v2e(const mln::Image<I>& input)
{
  return impl::dilation_v2e(mln::exact(input));
}


// ------------------------------ //
// Other dilations and erosions.  //
// ------------------------------ //

/// Vertex dilation (\f$delta\f$).
template <typename I>
inline
mln_concrete(I)
dilation_vertex(const mln::Image<I>& input)
{
  return dilation_e2v(dilation_v2e(input));
}

/// Vertex erosion (\f$epsilon\f$).
template <typename I>
inline
mln_concrete(I)
erosion_vertex(const mln::Image<I>& input)
{
  return erosion_e2v(erosion_v2e(input));
}


/// Edge dilation (\f$Delta\f$).
template <typename I>
inline
mln_concrete(I)
dilation_edge(const mln::Image<I>& input)
{
  return dilation_v2e(dilation_e2v(input));
}

/// Edge erosion (\f$Epsilon\f$).
template <typename I>
inline
mln_concrete(I)
erosion_edge(const mln::Image<I>& input)
{
  return erosion_v2e(erosion_e2v(input));
}


/// Graph dilation (\f$delta \ovee Delta\f$).
template <typename I>
inline
mln_concrete(I)
dilation_graph(const mln::Image<I>& input)
{
  return combine(dilation_vertex(input), dilation_edge(input));
}

/// Graph erosion (\f$epsilon \ovee Epsilon\f$).
template <typename I>
inline
mln_concrete(I)
erosion_graph(const mln::Image<I>& input)
{
  return combine(erosion_vertex(input), erosion_edge(input));
}


/*-------------------------.
| Additional adjunctions.  |
`-------------------------*/

template <typename I>
inline
mln_concrete(I)
alpha1(const mln::Image<I>& input)
{
  mln_concrete(I) vertices;
  mln::initialize(vertices, input);
  mln::data::fill(vertices, true);
  return combine(vertices, input);
}

template <typename I>
inline
mln_concrete(I)
beta1(const mln::Image<I>& input)
{
  return combine(dilation_e2v(input), input);
}

template <typename I>
inline
mln_concrete(I)
alpha2(const mln::Image<I>& input)
{
  return combine(input, erosion_v2e(input));
}

template <typename I>
inline
mln_concrete(I)
beta2(const mln::Image<I>& input)
{
  mln_concrete(I) edges;
  mln::initialize(edges, input);
  mln::data::fill(edges, false);
  return combine(input, edges);
}

template <typename I>
inline
mln_concrete(I)
alpha3(const mln::Image<I>& input)
{
  return combine(erosion_e2v(input), erosion_v2e(erosion_e2v(input)));
}

template <typename I>
inline
mln_concrete(I)
beta3(const mln::Image<I>& input)
{
  return combine(dilation_e2v(dilation_v2e(input)), dilation_v2e(input));
}


/*------------------------.
| Openings and closings.  |
`------------------------*/

/// Vertex opening (\f$\gamma_1\f$).
template <typename I>
inline
mln_concrete(I)
opening_vertex(const mln::Image<I>& input)
{
  return dilation_vertex(erosion_vertex(input));
}

/// Vertex closing (\f$\phi_1\f$).
template <typename I>
inline
mln_concrete(I)
closing_vertex(const mln::Image<I>& input)
{
  return erosion_vertex(dilation_vertex(input));
}


/// Edge opening (\f$\Gamma_1\f$).
template <typename I>
inline
mln_concrete(I)
opening_edge(const mln::Image<I>& input)
{
  return dilation_edge(erosion_edge(input));
}

/// Edge closing (\f$\Phi_1\f$).
template <typename I>
inline
mln_concrete(I)
closing_edge(const mln::Image<I>& input)
{
  return erosion_edge(dilation_edge(input));
}


/// Graph opening (\f${\gamma \ovee \Gamma}_1\f$).
template <typename I>
inline
mln_concrete(I)
opening_graph(const mln::Image<I>& input)
{
  return combine(opening_vertex(input), opening_edge(input));
}

/// Graph closing (\f${\phi \ovee \Phi}_1\f$).
template <typename I>
inline
mln_concrete(I)
closing_graph(const mln::Image<I>& input)
{
  return combine(closing_vertex(input), closing_edge(input));
}


/*----------------------------------.
| Half-openings and half-closings.  |
`----------------------------------*/

/// Vertex half-opening (\f$\gamma_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_opening_vertex(const mln::Image<I>& input)
{
  return dilation_e2v(erosion_v2e(input));
}

/// Vertex half-closing (\f$\phi_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_closing_vertex(const mln::Image<I>& input)
{
  return erosion_e2v(dilation_v2e(input));
}


/// Edge half-opening (\f$\Gamma_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_opening_edge(const mln::Image<I>& input)
{
  return dilation_v2e(erosion_e2v(input));
}

/// Edge half-closing (\f$\Phi_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_closing_edge(const mln::Image<I>& input)
{
  return erosion_v2e(dilation_e2v(input));
}


/// Graph half-opening (\f${\gamma \ovee \Gamma}_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_opening_graph(const mln::Image<I>& input)
{
  return combine(half_opening_vertex(input), half_opening_edge(input));
}

/// Graph half-closing (\f${\phi \ovee \Phi}_{1/2}\f$).
template <typename I>
inline
mln_concrete(I)
half_closing_graph(const mln::Image<I>& input)
{
  return combine(half_closing_vertex(input), half_closing_edge(input));
}


/*-------------------------------------------------------.
| Parameterized openings and closings (granulometries).  |
`-------------------------------------------------------*/

/// Opening (\f${\gamma \ovee \Gamma}_{\lambda/2}\f$).
template <typename I>
inline
mln_concrete(I)
opening(const mln::Image<I>& input, unsigned lambda)
{
  unsigned i = lambda / 2;
  unsigned j = lambda % 2;
  mln_concrete(I) output = mln::duplicate(input);
  for (unsigned m = 0; m < i; ++m)
    output = erosion_graph(output);
  for (unsigned m = 0; m < j; ++m)
    output = half_opening_graph(output);
  for (unsigned m = 0; m < i; ++m)
    output = dilation_graph(output);
  return output;
}

/// Opening (\f${\phi \ovee \Phi}_{\lambda/2}\f$).
template <typename I>
inline
mln_concrete(I)
closing(const mln::Image<I>& input, unsigned lambda)
{
  unsigned i = lambda / 2;
  unsigned j = lambda % 2;
  mln_concrete(I) output = mln::duplicate(input);
  for (unsigned m = 0; m < i; ++m)
    output = dilation_graph(output);
  for (unsigned m = 0; m < j; ++m)
    output = half_closing_graph(output);
  for (unsigned m = 0; m < i; ++m)
    output = erosion_graph(output);
  return output;
}

/*-------------------------------.
| Alternate Sequential Filters.  |
`-------------------------------*/

/// Alternate Sequential Filter (ASF) (\f${ASF}_{\lambda/2}\f$).
template <typename I>
inline
mln_concrete(I)
asf(const mln::Image<I>& input, unsigned lambda)
{
  mln_concrete(I) output = mln::duplicate(input);
  for (unsigned m = 1; m <= lambda; ++m)
    output = opening(closing(output, m), m);
  return output;
}

#endif // ! APPS_GRAPH_MORPHO_MORPHO_HH
