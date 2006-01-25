// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and Development Laboratory
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

#ifndef METALIC_BOOL_HH
# define METALIC_BOOL_HH

# include <mlc/value.hh>


/*! \macro mlc_bool(T)
**
** Macro that retrieves a Boolean value from a Boolean expression type
** T.  Its result is either true or false.
*/
# define mlc_bool(Type)  mlc::internal::get_bool<Type>::value


namespace mlc
{

  namespace abstract {

    /*! \class mlc::abstract::boolean
    **
    ** Abstract base class for Booleans represented as types.  This
    ** class has two important sub-classes: mlc::true_ and
    ** mlc::false_.
    **
    ** Any mlc Boolean expression type derives from either mlc::true_
    ** or mlc::false_ so derives from mlc::abstract::boolean.
    */
    
    struct boolean : public value {};

  } // end of mlc::abstract


  // fwd decl
  template <bool> struct bool_;


  namespace internal
  {

    /*! \class mlc::get_bool<T>
    **
    ** Internal class so do not use it.  This class gives access to a
    ** Boolean value from a Boolean expression type; it is used in the
    ** mlc_bool(T) macro.
    **
    ** \see mlc_bool(T)
    */
    template <typename T>
    struct get_bool
    {
      static const bool value = T::bool_value_;
    };


    /*! \class mlc::value_<bool, b>
    **
    ** Specialization of value_<T, val> for the Boolean case.
    **
    ** Design notes: 1) This specialization is defined so that mlc
    ** Booleans derive from mlc::abstract::boolean.  2) This
    ** specialization conforms to the interface of the generic version
    ** of mlc::internal::value_.
    */

    template <bool b>
    struct value_ <bool, b> : public abstract::boolean
    {
      /*! \typedef type
      **
      ** Gives the regular type, that is bool, corresponding to this
      ** class.
      */
      typedef bool type;

      /*! \member value
      **
      ** Gives the regular value of the Boolean represented by this
      ** class.  The result is either true or false.
      **
      ** This member is provided for any mlc value class such as
      ** mlc::int_<51> and mlc::char_<'c'>.  However, to access the
      ** regular value of a type T that you know to be a mlc Boolean
      ** expression, you should prefer to use the macro mlc_bool(T).
      ** This macro also ensures that T really is an expression.
      ** 
      */
      static const bool value = b;

      /*! \typedef ret
      **
      ** Returns mlc::true_ or mlc::false_.
      **
      ** Since Boolean expression types derive from either mlc::true_
      ** or mlc::false_, these expression types are never exactly
      ** mlc::true_ nor mlc::false_.  This typedef thus allows for the
      ** expression evaluation.
      **
      ** Please note that, however, we usually do not need expression
      ** evaluation.  The most common use of a Boolean expression is
      ** to check that it is verified (true) and, for that, we provide
      ** ::ensure() and ::ensure_t.
      **
      ** \see mlc::true_ and mlc::false_
      */
      typedef bool_<b> ret;

    private:

      /*! \member bool_value_
      **
      ** This member is redundant with the member 'value'.  It is used
      ** by the mlc_bool(T) macro to ensure that T actually derives
      ** from mlc::true_ or mlc::false_.
      */
      static const bool bool_value_ = b;

      template <typename T> friend class get_bool;
    };

  } // end of namespace mlc::internal


  /*! \class mlc::bool_<true>
  **
  ** Specialization of mlc::bool_<b> for b set to true.  This type
  ** is also known as mlc::true_.  Every Boolean expression types
  ** derive either from this type or from mlc::false_.
  **
  ** Design notes: 1) This specialization is defined so that mlc
  ** Booleans derive from mlc::abstract::boolean.  2) This
  ** specialization conforms to the interface of the generic version
  ** of mlc::internal::value_.
  **
  ** \see mlc::bool_<false>
  */

  template <>
  struct bool_<true> : public internal::value_<bool, true>
  {
    /*! \member ensure()
    **
    ** This member is inherited in every Boolean expression types that
    ** derive from mlc::true_.  This member is not provided in
    ** mlc::false_.
    ** 
    ** A static check to ensure that a Boolean expression type, say
    ** Expr, is verified can thus be written as an instruction:
    ** "Expr::ensure();"
    **
    ** When there is no room in code for an instruction, use
    ** ensure_t instead.
    **
    ** Design note:  This member is a no-op (it has no cost at
    ** run-time).
    */
    static void ensure() {}

    /*! \typedef ensure_t
    **
    ** This typedef is inherited in every Boolean expression types that
    ** derive from mlc::true_.  This typedef is not provided in
    ** mlc::false_.  The type returned by this typedef has no meaning
    ** (and thus no significant value).
    ** 
    ** A static check to ensure that a Boolean expression type, say
    ** Expr, is verified can thus be written through the typedef
    ** access: "Expr::ensure_t".  This is a replacement for the
    ** instruction "Expr::ensure();" when there is no room in code for
    ** an instruction.
    **
    ** Sample use:
    **   template <class T>
    **   struct foo
    **     : private mlc_is_a(T, base)::ensure_t
    **   {
    **     //...
    **   };
    ** meaning that foo's parameter is constrainted to be a sub-class
    ** of base.
    **
    ** Limitation: when using ensure_t to express several
    ** constraints through private inheritance, such as in the sample
    ** use above, one should express the set of constraints by a
    ** single expression.  The following code is not ok:
    **   template <class T1, class T2>
    **   struct foo
    **     : private mlc_is_a(T1, base)::ensure_t,
    **       private mlc_is_a(T2, base)::ensure_t
    **   { ... };
    ** the problem being that we mulitply inherit from the same
    ** class.  The correct code should be written with a single
    ** Boolean; practically the following code is ok:
    **   template <class T1, class T2>
    **   struct foo
    **     : private mlc::and_<mlc_is_a(T1, base), 
    **                         mlc_is_a(T2, base)>::ensure_t
    **   { ... };
    */
    typedef struct{} ensure_t;
  };

  typedef bool_<true>  true_;


  /*! \class mlc::bool_<false>
  **
  ** Specialization of mlc::bool_<b> for b set to false.  This type
  ** is also known as mlc::false_.  Every Boolean expression types
  ** derive either from this type or from mlc::true_.
  **
  ** Conversely to mlc::true_, this class does not feature ensure()
  ** nor ensure_t.  So, when a Boolean expression type, say Expr,
  ** is evaluated to false, the static checks "Expr::ensure();" and
  ** "Expr::ensure_t" do not compile.
  **
  ** Design notes: 1) This specialization is defined so that mlc
  ** Booleans derive from mlc::abstract::boolean.  2) This
  ** specialization conforms to the interface of the generic version
  ** of mlc::internal::value_.
  **
  ** \see mlc::bool_<true>
  */

  template <>
  struct bool_<false> : public internal::value_<bool, false>
  {
  };

  typedef bool_<false> false_;


  /*! \class mlc::not_<T>
  **
  ** Logical unary 'not' operator on a Boolean expression type.  This
  ** class is also a Boolean expression type.
  */
  template <typename T> struct not_ : public bool_<(!mlc_bool(T))> {};


  /*! \class mlc::and_<L,R>
  **
  ** Logical binary 'and' operator on a couple of Boolean expression
  ** types.  This class is also a Boolean expression type.
  **
  ** \see mlc::ands_<..>
  */
  template <typename L, typename R> struct and_  : public bool_<  (mlc_bool(L) && mlc_bool(R)) > {};

  /*! \class mlc::nand_<L,R>
  **
  ** Logical binary 'not and' operator on a couple of Boolean
  ** expression types.  This class is also a Boolean expression type.
  **
  ** Design note: an equivalent is mlc::not_< mlc::and_<L,R> >.
  */
  template <typename L, typename R> struct nand_ : public bool_<(!(mlc_bool(L) && mlc_bool(R)))> {}; 

  /*! \class mlc::or_<L,R>
  **
  ** Logical binary 'or' operator on a couple of Boolean expression
  ** types.  This class is also a Boolean expression type.
  **
  ** \see mlc::ors_<..>
  */
  template <typename L, typename R> struct or_   : public bool_<  (mlc_bool(L) || mlc_bool(R)) > {};

  /*! \class mlc::nor_<L,R>
  **
  ** Logical binary 'not or' operator on a couple of Boolean
  ** expression types.  This class is also a Boolean expression type.
  **
  ** Design note: an equivalent is mlc::not_< mlc::or_<L,R> >.
  */
  template <typename L, typename R> struct nor_  : public bool_<(!(mlc_bool(L) || mlc_bool(R)))> {};

  /*! \class mlc::xor_<L,R>
  **
  ** Logical binary 'exclusive or' operator on a couple of Boolean
  ** expression types.  This class is also a Boolean expression type.
  */
  template <typename L, typename R> struct xor_  : public bool_<  (mlc_bool(L) != mlc_bool(R)) > {};

  /*! \class mlc::xnor_<L,R>
  **
  ** Logical binary 'exclusive not or' operator on a couple of Boolean
  ** expression types.  This class is also a Boolean expression type.
  */
  template <typename L, typename R> struct xnor_ : public bool_<(!(mlc_bool(L) != mlc_bool(R)))> {};


  /// Internal helpers for logical operators between several Boolean types

  namespace internal
  {
    struct none_;

    // ors_2_
    template <typename A1, typename A2> struct ors_2_                : public or_<A1, A2> {};
    template <>                         struct ors_2_ <none_, none_> : public true_ {};
    template <typename A1>              struct ors_2_ <A1, none_>    : public A1 {};
    template <typename A2>              struct ors_2_ <none_, A2>;
    // ors_4_
    template <typename A1, typename A2,
	      typename A3, typename A4>
    struct ors_4_ : public ors_2_< ors_2_<A1, A2>,
				   ors_2_<A3, A4> > {};
    template <>
    struct ors_4_ <none_, none_, none_, none_> : public true_ {};
    // ors_8_
    template <typename A1, typename A2, typename A3, typename A4,
	      typename A5, typename A6, typename A7, typename A8>
    struct ors_8_ : public ors_2_< ors_4_<A1, A2, A3, A4>,
				   ors_4_<A5, A6, A7, A8> > {};
    // ands_2_
    template <typename A1, typename A2> struct ands_2_                : public and_<A1, A2> {};
    template <>                         struct ands_2_ <none_, none_> : public true_ {};
    template <typename A1>              struct ands_2_ <A1, none_>    : public A1 {};
    template <typename A2>              struct ands_2_ <none_, A2>;
    // ands_4_
    template <typename A1, typename A2,
	      typename A3, typename A4>
    struct ands_4_ : public ands_2_< ands_2_<A1, A2>,
				     ands_2_<A3, A4> > {};
    template <>
    struct ands_4_ <none_, none_, none_, none_> : public true_ {};
    // ands_8_
    template <typename A1, typename A2, typename A3, typename A4,
	      typename A5, typename A6, typename A7, typename A8>
    struct ands_8_ : public ands_2_< ands_4_<A1, A2, A3, A4>,
				     ands_4_<A5, A6, A7, A8> > {};

  } // end of mlc::internal


  /*! \class mlc::ors_<..>
  **
  ** Logical n-ary 'or' operator on a set of Boolean expression types.
  ** The number of arguments (parameters) should be at least 3 and at
  ** most 8.  This class is also a Boolean expression type.
  **
  ** Sample use:
  ** mlc::ors_< mlc::eq_<T, int>,
  **            mlc_is_a(T, mlc::int_),
  **            mlc_is_a(T, my::integer) >
  **
  ** \see mlc::or_<L,R> mlc::ands_<..>
  */

  template <typename A1,
	    typename A2,
	    typename A3,
	    typename A4 = internal::none_,
	    typename A5 = internal::none_,
	    typename A6 = internal::none_,
	    typename A7 = internal::none_,
	    typename A8 = internal::none_>
  struct ors_ : public internal::ors_8_< A1, A2, A3, A4, A5, A6, A7, A8 >
  {
  };


  /*! \class mlc::ands_<..>
  **
  ** Logical n-ary 'and' operator on a set of Boolean expression types.
  ** The number of arguments (parameters) should be at least 3 and at
  ** most 8.  This class is also a Boolean expression type.
  **
  ** \see mlc::and_<L,R> mlc::ors_<..>
  */

  template <typename A1,
	    typename A2,
	    typename A3,
	    typename A4 = internal::none_,
	    typename A5 = internal::none_,
	    typename A6 = internal::none_,
	    typename A7 = internal::none_,
	    typename A8 = internal::none_>
  struct ands_ : public internal::ands_8_< A1, A2, A3, A4, A5, A6, A7, A8 >
  {
  };


} // end of namespace mlc


#endif // ! METALIC_BOOL_HH
