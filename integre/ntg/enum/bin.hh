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

#ifndef NTG_ENUM_BIN_HH
# define NTG_ENUM_BIN_HH

# include <mlc/contract.hh>

# include <ntg/enum/enumerated.hh>

// --

# include <ntg/core/global_ops.hh>
# include <ntg/core/value.hh>
# include <ntg/core/typetraits.hh>
# include <ntg/core/optraits.hh>
# include <ntg/core/predecls.hh>

# include <string>

namespace ntg
{

  //
  //  Typetraits
  //
  ///////////////

  template <>
  struct typetraits<bin>
  {
    typedef bin				self;
    typedef optraits<self>		optraits;

    typedef self			base_type;
    typedef bool			storage_type;
    typedef bin				signed_type;
    typedef bin				unsigned_type;
    typedef bin				cumul_type;
    typedef bin				largest_type;
    typedef bin				signed_largest_type;
    typedef bin				signed_cumul_type;
    typedef bin				unsigned_largest_type;
    typedef bin				unsigned_cumul_type;
    typedef unsigned int		integer_type;

    typedef self op_traits;
  };


  //
  //  Class bin
  //
  //////////////


  class bin : public enumerated<bin>
  {
  public:
    bin () { val_ = 0; }

    // FIXME: create a template constructor and check into it if T
    // is_a real or whatever ?
    
    bin (unsigned char val)
    {
      precondition (val < 2);
      val_ = val;
    }

    bin& operator=(unsigned char val)
    {
      precondition (val < 2);
      val_ = val;
      return *this;
    }

    template <class T>
    bin (const real<T>& val)
    {
      precondition (val < 2);
      val_ = val;
    }
    template <class T>
    bin& operator=(const real<T>& val)
    {
      precondition (val < 2);
      val_ = val;
      return *this;
    }

    operator unsigned char() const { return val_; }
  };

  inline std::ostream&
  operator<<(std::ostream& stream, const bin& rhs)
  {
    stream << (unsigned int) rhs.val();
    return stream;
  }
  
  //
  //  optraits for bin
  //
  /////////////////////
  
  template <>
  struct optraits<bin> : public optraits_enum<bin>
  {
    typedef typetraits<bin>::storage_type storage_type;

    static storage_type zero() { return 0; }
    static storage_type unit() { return 1; }
    static storage_type min()  { return 0; }
    static storage_type max()  { return 1; }
    static storage_type inf()  { return min(); }
    static storage_type sup()  { return max(); }

    // logical assignement operators

    static bin& logical_or_equal(bin& lhs, const bin& rhs)
    {
      lhs = lhs.val() | rhs.val();
      return lhs;
    }

    static bin& logical_and_equal(bin& lhs, const bin& rhs)
    {
      lhs = lhs.val() & rhs.val();
      return lhs;
    }

    static bin& logical_xor_equal(bin& lhs, const bin& rhs)
    {
      lhs = lhs.val() ^ rhs.val();
      return lhs;
    }

    // logical binary ops

    static bin logical_or(const bin& lhs, const bin& rhs)
    {
      bin tmp(lhs);
      tmp |= rhs;
      return tmp;
    }

    static bin logical_and(const bin& lhs, const bin& rhs)
    {
      bin tmp(lhs);
      tmp &= rhs;
      return tmp;
    }

    static bin logical_xor(const bin& lhs, const bin& rhs)
    {
      bin tmp(lhs);
      tmp ^= rhs;
      return tmp;
    }


    // comparisons

    static bool cmp_lt(const bin& lhs, const bin& rhs)
    {
      return lhs.val() < rhs.val();
    }

    static bool cmp_eq(const bin& lhs, const bin& rhs)
    {
      return lhs.val() == rhs.val();
    }

    // debug
    static std::string name() { return "bin"; }
  };


  namespace internal
  {

    //
    //
    //  Operators traits
    //
    /////////////////////

    //
    //  Logical operators
    //

    template <class T>
    struct operator_logical_traits<bin, T>
    {
      enum { commutative = true };
      typedef bin ret;
      typedef bin impl;
    };

    //
    //  Comparison operators
    //

    template <>
    struct operator_cmp_traits<bin, bin>
    {
      enum { commutative = true };
      typedef bin ret;
      typedef bin impl;
    };
    
    //
    //  Max
    //

    template <>
    struct operator_max_traits<bin, bin>
    {
      enum { commutative = true };
      typedef bin ret;
      typedef bin impl;
    };

    //
    //  Min
    //

    template <>
    struct operator_min_traits<bin, bin>
    {
      enum { commutative = true };
      typedef bin ret;
      typedef bin impl;
    };

  } // end of internal

} // end of ntg

#endif // ndef NTG_ENUM_BIN_HH
