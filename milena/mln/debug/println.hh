// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_DEBUG_PRINTLN_HH
# define MLN_DEBUG_PRINTLN_HH

/*! \file mln/debug/println.hh
 *
 * \brief Print an image on the standard output.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/debug/format.hh>


namespace mln
{

  namespace debug
  {

    /// Print the image \p input on the standard output.
    template <typename I>
    void println(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // generic version
      template <typename S, typename I>
      void println(const S&, const Image<I>& input_)
      {
	const I& input = exact(input_);
	mln_piter(I) p(input.domain());
	for_all(p)
	  std::cout << format( input(p) ) << ' ';
	std::cout << std::endl;
      }

# ifdef MLN_CORE_BOX2D_HH

      // 2D version
      template <typename I>
      void println(const box2d& b, const I& input)
      {
	point2d p;
	int& row = p.row();
	int& col = p.col();
	const int
	  max_row = b.max_row(),
	  max_col = b.max_col();

	for (row = b.min_row(); row <= max_row; ++row)
	  {
	    for (col = b.min_col(); col <= max_col; ++col)
	      if (input.has(p))
		std::cout << format( input(p) ) << ' ';
	      else
		std::cout << "  ";
	    std::cout << std::endl;
	  }
	std::cout << std::endl;
      }

# endif // MLN_CORE_BOX2D_HH

# ifdef MLN_CORE_BOX3D_HH

      template <typename I>
      void println(const box3d& b, const I& input)
      {
	point3d p;
	int& sli = p.sli();
	int& row = p.row();
	int& col = p.col();
	const int
	  max_row = b.max_row(),
	  max_sli = b.max_sli(),
	  max_col = b.max_col();

	for (sli = b.min_sli(); sli <= max_sli; ++sli)
	{
	  for (row = b.min_row(); row <= max_row; ++row)
	  {
	    for (int i = max_row; i >= row; --i)
	      std::cout << ' ';
	    for (col = b.min_col(); col <= max_col; ++col)
	      if (input.has(p))
		std::cout << format( input(p) ) << ' ';
	      else
		std::cout << "  ";
	    std::cout << std::endl;
	  }
	  std::cout << std::endl;
	}
      }

# endif // MLN_CORE_BOX3D_HH

    } // end of namespace mln::debug::impl


    // Facade.

    template <typename I>
    void println(const Image<I>& input)
    {
      impl::println(exact(input).bbox(), exact(input));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_PRINTLN_HH
