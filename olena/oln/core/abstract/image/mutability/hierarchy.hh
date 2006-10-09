// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_ABSTRACT_IMAGE_MUTABILITY_HIERARCHY_HH
# define OLN_CORE_ABSTRACT_IMAGE_MUTABILITY_HIERARCHY_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/automatic/image_being_mutable.hh>



namespace oln
{
 
  /*-------------------------.
    | Dimension abstractions.  |
    `-------------------------*/

  namespace abstract
  {

    /// Image being mutable.
    template <typename E>
    struct image_being_mutable :
      public virtual image<E>,
      public automatic::get_impl<image_being_mutable, E>
    {
    private:

      typedef oln_type_of(E, lvalue) lvalue_t;
      typedef oln_type_of(E, psite)  psite_t;

    public:

      struct decl
      {
	oln_virtual_typedef(lvalue);
      };

      /*! \brief Gives writable access to the value stored at \a p in
      ** the current image.
      */

      using image<E>::operator();

      lvalue_t& operator()(const psite_t& p);

    protected:

      /// Constructor (protected, empty).
      image_being_mutable();

      /// Destructor (protected).
      ~image_being_mutable();

    };


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    typename image_being_mutable<E>::lvalue_t&
    image_being_mutable<E>::operator()(const typename image_being_mutable<E>::psite_t& p)
    {
      return this->exact().impl_op_readwrite(p);
    }

    template <typename E>
    image_being_mutable<E>::image_being_mutable()
    {
    }

    template <typename E>
    image_being_mutable<E>::~image_being_mutable()
    {
      decl();
    }

# endif

  } // end of namespace oln::abstract


  /*-------------------.
    | Dimension switch.  |
    `-------------------*/

  /// With mutability.
  template <typename E>
  struct case_< image_hierarchy_wrt_mutability, E, 1 > :
    where_< mlc::eq_< oln_type_of(E, is_mutable), mlc::true_ > >
  {
    typedef abstract::image_being_mutable<E> ret;
  };

} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_IMAGE_MUTABILITY_HIERARCHY_HH
