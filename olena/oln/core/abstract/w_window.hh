// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_CORE_ABSTRACT_W_WINDOW_HH
# define OLENA_CORE_ABSTRACT_W_WINDOW_HH

# include <oln/core/abstract/struct_elt.hh>

namespace oln 
{

  namespace abstract 
  {
    
    template<class Exact>
    struct w_window; // fwd_decl
    
  } // end of abstract
  
  template<class Exact>
  struct struct_elt_traits<abstract::w_window<Exact> > : public
  struct_elt_traits<abstract::struct_elt<Exact> >
  {

  };

  namespace abstract 
  {

    template<class Exact>
    struct w_window : public struct_elt< Exact >
    {
      typedef Exact exact_type;
      typedef struct_elt<Exact> super_type;     
      typedef typename struct_elt_traits<Exact>::dpoint_type dpoint_type;
      typedef typename struct_elt_traits<Exact>::weight_type weight_type;
       

      static std::string name()
      {
	return std::string("w_window<") + Exact::name() + ">";
      }

      exact_type& add(const dpoint_type& dp, const weight_type& w)
      {
	return to_exact(this)->add_(dp, w);
      }

      const weight_type& set(const dpoint_type& dp, const weight_type& weight)
      {
	return to_exact(this)->set_(dp, weight);
      }

    protected:
      w_window() : super_type() {}
    };
    
# define Weight(WinType)                               \
Exact(WinType)::weight_type

  } // end of abstract

} // end of oln

#endif // ! OLENA_CORE_ABSTRACT_W_WINDOW_HH
