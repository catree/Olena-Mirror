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

#ifndef MLN_CORE_IMAGE_EXTENSION_IMA_HH
# define MLN_CORE_IMAGE_EXTENSION_IMA_HH

/// \file mln/core/image/extension_ima.hh
///
/// Definition of a morpher that extends the domain of an image
/// with a function.
///
/// \todo Use an envelop as lvalue to test extension writing.
///
/// \todo Handle the couple of cases: either J is value_io::read_write
/// or value_io::read_only; then ext_io can be read_write...

# include <mln/core/internal/image_identity.hh>
# include <mln/data/fill_with_value.hh>



namespace mln
{

  // Forward declaration.
  template <typename I, typename J> struct extension_ima;


  namespace internal
  {

    /// Data structure for \c mln::extension_ima<I, J>.
    template <typename I, typename J>
    struct data< extension_ima<I, J> >
    {
      data(I& ima, J& ext);

      I ima_;
      J ext_;
    };

  } // end of namespace mln::internal


  namespace trait
  {

    template <typename I, typename J>
    struct image_< extension_ima<I, J> > : image_< I > // Same as I except...
    {
      // ...these changes.
      typedef trait::image::category::identity_morpher category;
      typedef mln_internal_trait_image_speed_from(I) speed; // Un-fastest.
      typedef trait::image::value_access::indirect value_access;

      // extended domain
      typedef trait::image::ext_domain::extendable ext_domain;
      typedef trait::image::ext_value::multiple    ext_value;
      typedef trait::image::ext_io::read_only      ext_io; // FIXME: Too restrictive?
    };

    template <typename I, typename J, typename V>
    struct ch_value< extension_ima<I, J>, V >
    {
      typedef mlc_converts_to(mln_value(J), V) keep_ext;
      typedef mln_ch_value(I, V)   Iv;
      typedef extension_ima<Iv, J> Iv_ext;
      typedef mlc_if(keep_ext, Iv_ext, Iv) ret;
    };

  } // end of namespace mln::trait



  // FIXME: Doc!

  template <typename I, typename J>
  class extension_ima

    : public internal::image_identity< I, mln_pset(I), extension_ima<I, J> >,
      private mlc_converts_to(mln_value(J), mln_value(I))::check_t
  {
  public:

    /// Skeleton.
    typedef extension_ima< tag::image_<I>, tag::ext_<J> > skeleton;


    /// Return type of read-only access.
    typedef mln_value(I) rvalue;


    /// Constructor without argument.
    extension_ima();

    /// Constructor from an image \p ima and a function \p ext.
    extension_ima(I& ima, J& ext);

    /// Deferred initialization from an image \p ima and a function \p
    /// ext.
    void init_(I& ima, J& ext);


    /// Test if \p p is valid.
    // Tech note: the 'template' allows for multiple input.
    template <typename P>
    bool has(const P& p) const;


    /// Read-only access to the image value located at site \p p;
    mln_value(I) operator()(const mln_psite(I)& p) const;

    /// Read-write access to the image value located at site \p p.
    mln_morpher_lvalue(I) operator()(const mln_psite(I)& p);


    /// Read-only access to the extension domain (image).
    mlc_const(J)& extension() const;

    /// Mutable access to the extension domain (image).  This domain
    /// can be modified if J a read-write image type.
    J& extension();

    /// Change the value in the extension domain (image).
    void change_extension(const mln_value(I)& v);
  };


  // init_

  template <typename I, typename J, typename M>
  void init_(tag::image_t, extension_ima<I,J>& target, const M& model);

  template <typename J, typename I>
  void init_(tag::extension_t, J& target, const extension_ima<I,J>& model);

  template <typename J, typename I>
  void init_(tag::extension_t, J& target, const extension_ima<I,const J>& model);




# ifndef MLN_INCLUDE_ONLY

  // internal::data< extension_ima<I,S> >

  namespace internal
  {

    template <typename I, typename J>
    inline
    data< extension_ima<I, J> >::data(I& ima, J& ext)
      : ima_(ima),
	ext_(ext)
    {
    }

  } // end of namespace mln::internal

  // extension_ima<I, J>

  template <typename I, typename J>
  inline
  extension_ima<I, J>::extension_ima()
  {
  }

  template <typename I, typename J>
  inline
  extension_ima<I, J>::extension_ima(I& ima, J& ext)
  {
    init_(ima, ext);
  }

  template <typename I, typename J>
  inline
  void
  extension_ima<I, J>::init_(I& ima, J& ext)
  {
    this->data_ = new internal::data< extension_ima<I, J> >(ima, ext);
  }

  template <typename I, typename J>
  template <typename P>
  inline
  bool
  extension_ima<I, J>::has(const P& p) const
  {
    mln_precondition(this->has_data());
    mln_precondition(this->data_->ext_.has_data());
    return
      this->data_->ima_.domain().has(p)
      || this->data_->ext_.has(p);
  }

  template <typename I, typename J>
  inline
  mln_value(I)
  extension_ima<I, J>::operator()(const mln_psite(I)& p) const
  {
    mln_precondition(this->has_data());
    mln_precondition(has(p));
    // if-else is preferred to the ternary op to allow for the
    // function result to convert towards the expected return type.
    if (this->data_->ima_.domain().has(p))
      return this->data_->ima_(p);
    else
      return this->data_->ext_(p);
  }

  template <typename I, typename J>
  inline
  mln_morpher_lvalue(I)
  extension_ima<I, J>::operator()(const mln_psite(I)& p)
  {
    static mln_value(I) cpy;
    mln_precondition(this->has_data());
    mln_precondition(has(p));
    // See the above comment about if-else v. ternary.
    if (this->data_->ima_.domain().has(p))
      return this->data_->ima_(p);
    else
      {
	// This hack makes this signature valid both in the image
	// domain and in its extension.  It works even if
	// mln_morpher_lvalue(I) is a mutable reference.
	cpy = this->data_->ext_(p);
	return cpy;
      }
  }

  template <typename I, typename J>
  inline
  mlc_const(J)&
  extension_ima<I, J>::extension() const
  {
    mln_precondition(this->has_data());
    return this->data_->ext_;
  }

  template <typename I, typename J>
  inline
  J&
  extension_ima<I, J>::extension()
  {
    mln_precondition(this->has_data());
    return this->data_->ext_;
  }

  template <typename I, typename J>
  inline
  void
  extension_ima<I, J>::change_extension(const mln_value(I)& v)
  {
    mlc_equal(mln_trait_image_value_io(J),
	      trait::image::value_io::read_write)::check();
    data::fill_with_value(v);
  }

  // init_

  template <typename I, typename J, typename M>
  void init_(tag::image_t, extension_ima<I,J>& target, const M& model)
  {
    I ima;
    init_(tag::image, ima, model);
    J ext;
    init_(tag::extension, ext, model);
    target.init_(ima, ext);
  }

  template <typename J, typename I>
  void init_(tag::extension_t, J& target, const extension_ima<I,J>& model)
  {
    target = model.extension();
  }

  template <typename J, typename I>
  void init_(tag::extension_t, J& target, const extension_ima<I,const J>& model)
  {
    target = model.extension();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_EXTENSION_IMA_HH
