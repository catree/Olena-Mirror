// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_VALUE_FLOAT01__HH
# define MLN_CORE_VALUE_FLOAT01__HH

# include <iostream>
# include <mln/core/contract.hh>
# include <mln/metal/math.hh>
# include <mln/metal/bexpr.hh>
# include <mln/value/internal/value_like.hh>
# include <mln/value/internal/encoding.hh>
# include <mln/value/float01.hh>
# include <mln/value/props.hh>

namespace mln
{

  namespace value
  {

    /// Fwd decl.
    class float01;


    /// General float01-level class on n bits.
    template <unsigned n>
    class float01_
      : public internal::value_like_< float,
				      float01_<n> >
    {
    public:

      /// Encoding associated type.
      typedef typename internal::encoding_unsigned_<n>::ret enc;

      /// Ctor.
      float01_();

      /// Ctor.
      float01_(const float val);

      /// Access to std type.
      float value() const;

      void set_ind(unsigned long val);

      enc value_ind() const;

      /// Op encoding_t.
      operator float() const;

      /// Op float01.
      operator float01() const;

      /// Op<.
      bool operator<(const float01_<n>& rhs) const;

      float01_<n>& operator=(const float val);
      /// Op==.
      // bool operator==(const float01_<n>& rhs) const;

    protected:
      enc val_;
    };


    namespace internal
    {

      template <unsigned n>
      struct convert_< float01_<n> >
      {
	static float01_<n> value_at_index(std::size_t i)
	{
	  float01_<n> tmp;
	  tmp.set_ind(i);
	  return tmp;
	}

	static std::size_t index_of_value(const float01_<n>& v)
	{
	  return v.value_ind();
	}
      };
    }

    template <unsigned n>
    struct props< float01_<n> >
    {
      static const std::size_t card_ = metal::pow<2, n>::value;
      static const int min() { return 0; }
      static const int max() { return 1; }
      static const unsigned nbits = n;
      typedef trait::kind::data kind;
      typedef float sum;
      typedef float interop;
    };


    /// Op<<.
    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const float01_<n>& g);


    template <unsigned n, unsigned m>
    bool approx_equal(const float01_<n>& lhs, const float01_<m>& rhs);

    template <unsigned n>
    bool operator==(const float01_<n>& lhs, const float01_<n>& rhs);

    template <unsigned n>
    bool approx_equal(const float01_<n>& lhs, const float f);

# ifndef MLN_INCLUDE_ONLY

    // Float01_<n>.

    template <unsigned n>
    float01_<n>::float01_()
      : val_(0)
    {
    }

    template <unsigned n>
    float01_<n>::float01_(const float val)
      : val_( int(val * (mln_card_(float01_<n>) - 1)) )
    {
      mln_precondition(val >= 0);
      mln_precondition(val <= 1);
    }

    template <unsigned n>
    float
    float01_<n>::value() const
    {
      return (float(val_) / (mln_card_(float01_<n>) - 1));
    }

    template <unsigned n>
    void
    float01_<n>::set_ind(unsigned long val)
    {
      val_ = val;
    }

    template <unsigned n>
    typename float01_<n>::enc
    float01_<n>::value_ind() const
    {
      return (val_);
    }

    template <unsigned n>
    float01_<n>&
    float01_<n>::operator=(const float val)
    {
      mln_precondition(val >= 0);
      mln_precondition(val <= 1);
      this->val_ = long(val * (mln_card_(float01_<n>) - 1));
      return *this;
    }

    template <unsigned n>
    float01_<n>::operator float01() const
    {
      float01 tmp(n, val_);
      return tmp;
    }

    template <unsigned n>
    float01_<n>::operator float() const
    {
      return float(val_) / (mln_card_(float01_<n>) - 1);
    }

    template <unsigned n>
    bool float01_<n>::operator<(const float01_<n>& rhs) const
    {
      return val_ < rhs.val_;
    }

    template <unsigned n>
    std::ostream& operator<<(std::ostream& ostr, const float01_<n>& g)
    {
      return ostr << g.value();
    }

    template <unsigned n>
    bool operator==(const float01_<n>& lhs, const float01_<n>& rhs)
    {
      return lhs.value_ind() == rhs.value_ind();
    }

    template <unsigned n, unsigned m>
    bool approx_equal(const float01_<n>& lhs, const float01_<m>& rhs)
    {
      return float01(lhs) == float01(rhs);
    }

    template <unsigned n>
    bool approx_equal(const float01_<n>& lhs, const float f)
    {
      return float01(lhs) == float01_<n>(f);
    }

# endif


  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_CORE_VALUE_FLOAT01__HH
