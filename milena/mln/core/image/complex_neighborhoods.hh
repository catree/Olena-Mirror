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

#ifndef MLN_CORE_IMAGE_COMPLEX_NEIGHBORHOODS_HH
# define MLN_CORE_IMAGE_COMPLEX_NEIGHBORHOODS_HH

/// \file mln/core/image/complex_neighborhoods.hh
/// \brief Definitions of some neighborhoods of complex images.

# include <mln/core/internal/complex_neighborhood_base.hh>

# include <mln/topo/adj_lower_face_iter.hh>
# include <mln/topo/adj_higher_face_iter.hh>
# include <mln/topo/adj_lower_higher_face_iter.hh>
# include <mln/topo/adj_lower_dim_connected_n_face_iter.hh>
# include <mln/topo/adj_higher_dim_connected_n_face_iter.hh>


namespace mln
{

  /// \brief Neighborhood centered on an n-face of complex returning
  /// its adjacent (n-1)-faces.
  template <unsigned D, typename G>
  struct complex_lower_neighborhood
    : internal::complex_neighborhood_base< D, G,
					   topo::adj_lower_face_fwd_iter<D>,
					   topo::adj_lower_face_bkd_iter<D>,
					   complex_lower_neighborhood<D, G> >
  {
  };

  /// \brief Neighborhood centered on an n-face of complex returning
  /// its adjacent (n+1)-faces.
  template <unsigned D, typename G>
  struct complex_higher_neighborhood
    : internal::complex_neighborhood_base< D, G,
					   topo::adj_higher_face_fwd_iter<D>,
					   topo::adj_higher_face_bkd_iter<D>,
					   complex_higher_neighborhood<D, G> >
  {
  };

  /// \brief Neighborhood centered on an n-face of complex returning
  /// its adjacent (n-1)-faces and (n+1)-faces.
  template <unsigned D, typename G>
  struct complex_lower_higher_neighborhood
    : internal::complex_neighborhood_base<
        D, G,
        topo::adj_lower_higher_face_fwd_iter<D>,
        topo::adj_lower_higher_face_bkd_iter<D>,
        complex_lower_higher_neighborhood<D, G>
      >
  {
  };

  /// \brief Neighborhood centered on an n-face of complex returning
  /// the n-faces sharing an (n-1)-face with the center n-face.
  template <unsigned D, typename G>
  struct complex_lower_dim_connected_n_face_neighborhood
    : internal::complex_neighborhood_base<
        D, G,
        topo::adj_lower_dim_connected_n_face_fwd_iter<D>,
        topo::adj_lower_dim_connected_n_face_bkd_iter<D>,
        complex_lower_dim_connected_n_face_neighborhood<D, G>
      >
  {
  };

  /// \brief Neighborhood centered on an n-face of complex returning
  /// the n-faces sharing an (n+1)-face with the center n-face.
  template <unsigned D, typename G>
  struct complex_higher_dim_connected_n_face_neighborhood
    : internal::complex_neighborhood_base<
        D, G,
        topo::adj_higher_dim_connected_n_face_fwd_iter<D>,
        topo::adj_higher_dim_connected_n_face_bkd_iter<D>,
        complex_higher_dim_connected_n_face_neighborhood<D, G>
      >
  {
  };

} // end of namespace mln

#endif // ! MLN_CORE_IMAGE_COMPLEX_NEIGHBORHOODS_HH
