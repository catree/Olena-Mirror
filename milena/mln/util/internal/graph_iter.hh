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

#ifndef MLN_UTIL_INTERNAL_GRAPH_ITER_HH
# define MLN_UTIL_INTERNAL_GRAPH_ITER_HH

# include <mln/core/concept/iterator.hh>
# include <mln/util/internal/graph_vertex.hh>
# include <mln/util/internal/graph_edge.hh>
# include <mln/util/internal/graph_iter_base.hh>

/// \file   mln/util/internal/graph_iter.hh
/// \brief  Implementation for graph iterators.

namespace mln
{

  namespace internal
  {

    template<typename G>
    class vertex_fwd_iterator
      : public graph_iter_base<G, util::vertex<G>, vertex_fwd_iterator<G> >
    {
      typedef graph_iter_base<G, util::vertex<G>, vertex_fwd_iterator<G> > super_;

      public:
	/// Constructors.
	/// \{
	vertex_fwd_iterator();
	vertex_fwd_iterator(const G& g);
	/// \}

      protected:
	/// Returns the id of the first element.
	/// Called in start();
	unsigned start_id_() const;

	/// Returns the next element id.
	/// Called in next();
	unsigned next_id_() const;

	using super_::p_;
	friend class graph_iter_base<G, util::vertex<G>, vertex_fwd_iterator<G> >;
    };


    template<typename G>
    class vertex_bkd_iterator
      : public graph_iter_base<G, util::vertex<G>, vertex_bkd_iterator<G> >
    {
      typedef graph_iter_base<G, util::vertex<G>, vertex_bkd_iterator<G> > super_;

      public:
	/// Constructors.
	/// \{
	vertex_bkd_iterator();
	vertex_bkd_iterator(const G& g);
	/// \}

      protected:
	/// Returns the id of the first element.
	/// Called in start();
	unsigned start_id_() const;

	/// Returns the next element id.
	/// Called in next();
	unsigned next_id_() const;

	using super_::p_;
	friend class graph_iter_base<G, util::vertex<G>, vertex_bkd_iterator<G> >;
    };

    /// Forward edge iterator.
    template <typename G>
    class edge_fwd_iterator
      : public graph_iter_base<G, util::edge<G>, edge_fwd_iterator<G> >
    {
      typedef graph_iter_base<G, util::edge<G>, edge_fwd_iterator<G> > super_;

      public:
	/// Constructors.
	/// \{
	edge_fwd_iterator();
	edge_fwd_iterator(const G& g);
	/// \}

      protected:
	/// Returns the id of the first element.
	/// Called in start();
	unsigned start_id_() const;

	/// Returns the next element id.
	/// Called in next();
	unsigned next_id_() const;

	using super_::p_;
	friend class graph_iter_base<G, util::edge<G>, edge_fwd_iterator<G> >;
    };

    template <typename G>
    class edge_bkd_iterator
      : public graph_iter_base<G, util::edge<G>, edge_bkd_iterator<G> >
    {
      typedef graph_iter_base<G, util::edge<G>, edge_bkd_iterator<G> > super_;

      public:
	/// Constructors.
	/// \{
	edge_bkd_iterator();
	edge_bkd_iterator(const G& g);
	/// \}

      protected:
	/// Returns the id of the first element.
	/// Called in start();
	unsigned start_id_() const;

	/// Returns the next element id.
	/// Called in next();
	unsigned next_id_() const;

	using super_::p_;
	friend class graph_iter_base<G, util::edge<G>, edge_bkd_iterator<G> >;
    };

# ifndef MLN_INCLUDE_ONLY

    /*--------------------`
    | vertex_fwd_iterator |
    \--------------------*/

    template <typename G>
    inline
    vertex_fwd_iterator<G>::vertex_fwd_iterator()
    {
    }

    template <typename G>
    inline
    vertex_fwd_iterator<G>::vertex_fwd_iterator(const G& g)
      : super_(g)
    {
    }

    template <typename G>
    inline
    unsigned
    vertex_fwd_iterator<G>::start_id_() const
    {
      return 0;
    }

    template <typename G>
    inline
    unsigned
    vertex_fwd_iterator<G>::next_id_() const
    {
      return p_.id() + 1;
    }



    /*--------------------`
    | vertex_bkd_iterator |
    \--------------------*/

    template <typename G>
    inline
    vertex_bkd_iterator<G>::vertex_bkd_iterator()
    {
    }

    template <typename G>
    inline
    vertex_bkd_iterator<G>::vertex_bkd_iterator(const G& g)
      : super_(g)
    {
    }

    template <typename G>
    inline
    unsigned
    vertex_bkd_iterator<G>::start_id_() const
    {
      return p_.g().v_nmax() - 1;
    }

    template <typename G>
    inline
    unsigned
    vertex_bkd_iterator<G>::next_id_() const
    {
      return p_.id() - 1;
    }



    /*------------------`
    | edge_fwd_iterator |
    \------------------*/

    template <typename G>
    inline
    edge_fwd_iterator<G>::edge_fwd_iterator()
    {
    }

    template <typename G>
    inline
    edge_fwd_iterator<G>::edge_fwd_iterator(const G& g)
      : super_(g)
    {
    }

    template <typename G>
    inline
    unsigned
    edge_fwd_iterator<G>::start_id_() const
    {
      return 0;
    }

    template <typename G>
    inline
    unsigned
    edge_fwd_iterator<G>::next_id_() const
    {
      return p_.id() + 1;
    }



    /*------------------`
    | edge_bkd_iterator |
    \------------------*/

    template <typename G>
    inline
    edge_bkd_iterator<G>::edge_bkd_iterator()
    {
    }

    template <typename G>
    inline
    edge_bkd_iterator<G>::edge_bkd_iterator(const G& g)
      : super_(g)
    {
    }

    template <typename G>
    inline
    unsigned
    edge_bkd_iterator<G>::start_id_() const
    {
      return p_.g().e_nmax() - 1;
    }

    template <typename G>
    inline
    unsigned
    edge_bkd_iterator<G>::next_id_() const
    {
      return p_.id() - 1;
    }

# endif // !MLN_INCLUDE_ONLY

  } // End of namespace mln::internal

} // End of namespace mln

#endif // !MLN_UTIL_INTERNAL_GRAPH_ITER_HH
