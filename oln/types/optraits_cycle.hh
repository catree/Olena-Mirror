// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_VALUE_OPTRAITS_CYCLE_HH
# define OLENA_VALUE_OPTRAITS_CYCLE_HH

# include <oln/config/system.hh>
# include <oln/meta/type.hh>
# include <oln/types/to_oln.hh>
# include <oln/types/cycle.hh>
# include <oln/types/optraits.hh>
# include <oln/types/typetraits.hh>
# include <oln/types/global_ops_traits.hh>

namespace oln
{

  template<class T,
	   class interval>
  struct optraits<cycle<T, interval> > : public optraits<T>
  {
  public:
    typedef cycle<T, interval> self;

  private:
    typedef typename typetraits<self>::store store_type;
    typedef typename interval::store_type interval_type;

  public:
    //
    //  Properties
    //
    ////

    static interval_type min()
    { return interval::min(); }
    
    static interval_type max()
    { return interval::max(); }

    static interval_type inf()
    { return interval::inf(); }
    
    static interval_type sup()
    { return interval::sup(); }

    struct cycle_fmod
    {
      static double exec (double lhs, double rhs)
      {
	return fmod(lhs, rhs);
      }
    };

    template <class U>
    struct cycle_mod
    {
      static U exec (const U& lhs, const U& rhs)
      {
	return lhs % rhs;
      }
    };    

    // behaviour's check

    template <class P>
    static store_type check(const P& rhs)
    { 
      typedef typename meta::if_<is_a(optraits<P>, optraits_float)::ret,
	                         cycle_fmod,
	                         cycle_mod<P> >::ret_t cycle_op;;
      typename internal::to_oln<P>::ret 
	tmp = cycle_op::exec(abs(rhs), max() - min());
      if (rhs < 0) tmp = -tmp;

      if (tmp < min())
	return max() - min() + tmp;
      else if (tmp >= max())
	return min() - max() + tmp;
      return tmp;
    }
  };

} // end of namespace oln

#endif // ndef OLENA_VALUE_OPTRAITS_CYCLE_HH
