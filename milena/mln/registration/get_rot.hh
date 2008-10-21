// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_REGISTRATION_GET_ROT_HH
# define MLN_REGISTRATION_GET_ROT_HH

# include <mln/fun/x2x/all.hh>
# include <mln/algebra/quat.hh>
# include <mln/math/jacobi.hh>

namespace mln
{

  namespace registration
  {

    template <typename P, typename M>
    fun::x2x::rotation<P::dim, float>
    get_rot(const p_array<P>& c,
            const algebra::vec<P::dim,float>& mu_c,
            const p_array<P>& ck,
            const M& map,
            const algebra::vec<P::dim,float>& mu_xk);


# ifndef MLN_INCLUDE_ONLY


    template <typename P, typename M>
    fun::x2x::rotation<2u, float>
    get_rot(const p_array<P>& c,
            const algebra::vec<2u,float>& mu_c,
            const p_array<P>& ck,
            const M& map,
            const algebra::vec<2u,float>& mu_xk)
    {
      //FIXME: Write 2d version of rotation computation betwenn two p_array
      return fun::x2x::rotation<2u, float>();
    }

    template <typename P, typename M>
    fun::x2x::rotation<3u, float>
    get_rot(const p_array<P>& c,
            const algebra::vec<3u,float>& mu_c,
            const p_array<P>& ck,
            const M& map,
            const algebra::vec<3u,float>& mu_xk)
    {
      //FIXME: Make assertion static
      mln_precondition(3u == 3);

      // FIXME: Make use of a cross_covariance accu (maybe not because of map(ck[i]))
      algebra::mat<3u,3u,float> Mk(literal::zero);
      for (unsigned i = 0; i < c.nsites(); ++i)
        {
          algebra::vec<3u,float> ci  = c[i];
          algebra::vec<3u,float> xki = map(ck[i]);
          Mk += make::mat(ci - mu_c) * trans(make::mat(xki - mu_xk));
        }
      Mk /= c.nsites();

      algebra::vec<3u,float> a;
      a[0] = Mk(1,2) - Mk(2,1);
      a[1] = Mk(2,0) - Mk(0,2);
      a[2] = Mk(0,1) - Mk(1,0);

      algebra::mat<4u,4u,float> Qk(literal::zero);
      float t = tr(Mk);

      Qk(0,0) = t;
      for (int i = 1; i < 4; i++)
      {
        Qk(i,0) = a[i - 1];
        Qk(0,i) = a[i - 1];
        for (int j = 1; j < 4; j++)
          if (i == j)
            Qk(i,j) = 2 * Mk(i - 1,i - 1) - t;
      }

      Qk(1,2) = Mk(0,1) + Mk(1,0);
      Qk(2,1) = Mk(0,1) + Mk(1,0);

      Qk(1,3) = Mk(0,2) + Mk(2,0);
      Qk(3,1) = Mk(0,2) + Mk(2,0);

      Qk(2,3) = Mk(1,2) + Mk(2,1);
      Qk(3,2) = Mk(1,2) + Mk(2,1);

      algebra::quat qR(literal::zero);
      qR = math::jacobi(Qk);

      return fun::x2x::rotation<3u, float>(qR);
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace registration

} // end of namespace mln

#endif /* MLN_REGISTRATION_GET_ROT_HH */

