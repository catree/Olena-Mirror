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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_SITE_SET_P_VERTICES_PSITE_HH
# define MLN_CORE_SITE_SET_P_VERTICES_PSITE_HH

/// \file mln/core/site_set/p_vertices_psite.hh
///
/// Implementation of p_vertices psite.

# include <mln/core/concept/pseudo_site.hh>
# include <mln/core/internal/graph_psite_base.hh>
# include <mln/util/vertex.hh>


namespace mln
{

  // Forward declaration.
  template <typename G, typename F> class p_vertices;



  template <typename G, typename F>
  class p_vertices_psite :

    public internal::graph_psite_base< p_vertices<G,F>, p_vertices_psite<G,F> >
  {
    typedef p_vertices_psite<G,F> self_;
    typedef internal::graph_psite_base<p_vertices<G,F>, self_> super_;

  public:

    p_vertices_psite();
    p_vertices_psite(const p_vertices<G,F>& s);
    p_vertices_psite(const p_vertices<G,F>& s, unsigned id);

    const util::vertex<G>& v() const;
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename G, typename F>
  inline
  p_vertices_psite<G, F>::p_vertices_psite()
  {
  }

  template <typename G, typename F>
  inline
  p_vertices_psite<G, F>::p_vertices_psite(const p_vertices<G,F>& s)
    : super_(s)
  {
  }

  template <typename G, typename F>
  inline
  p_vertices_psite<G, F>::p_vertices_psite(const p_vertices<G,F>& s, unsigned i)
    : super_(s, i)
  {
  }

  template <typename G, typename F>
  inline
  const util::vertex<G>&
  p_vertices_psite<G, F>::v() const
  {
    return this->elt_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_VERTICES_PSITE_HH
