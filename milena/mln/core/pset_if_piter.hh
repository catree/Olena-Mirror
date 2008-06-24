// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_PSET_IF_PITER_HH
# define MLN_CORE_PSET_IF_PITER_HH

/*! \file mln/core/pset_if_piter.hh
 *
 * \brief Definition of iterators on pset_if<S,F>.
 *
 * \todo pset_if_bkd_piter_.
 */

# include <mln/core/internal/piter_adaptor.hh>
# include <mln/core/pset_if.hh>


namespace mln
{


  /*! \brief A generic forward iterator on points of subsets.
   *
   * Parameter \c S is a point set type; parameter F is a function
   * from point to Boolean.
   *
   * \see mln::pset_if
   */
  template <typename S, typename F>
  class pset_if_fwd_piter_
    : public internal::piter_adaptor_< mln_fwd_piter(S),         // Adaptee.
				       pset_if<S,F>,             // Site_Set.
				       pset_if_fwd_piter_<S,F> > // Exact.
  {
  public:

    /// Constructor without argument.
    pset_if_fwd_piter_();

    /// Constructor from a site set.
    pset_if_fwd_piter_(const pset_if<S,F>& s);

    /// Start an iteration.
    void start_();

    /// Go to the next point.
    void next_();

    /// The set site targeted by pi_.
    const S& pi_set_from_(const pset_if<S,F>& s) const;
  };


  // FIXME:
  template <typename S, typename F>
  class pset_if_bkd_piter_
    :
    public mln::internal::fixme
  {};



# ifndef MLN_INCLUDE_ONLY


  // pset_if_fwd_piter_<S,F>

  template <typename S, typename F>
  inline
  pset_if_fwd_piter_<S,F>::pset_if_fwd_piter_()
  {
  }

  template <typename S, typename F>
  inline
  pset_if_fwd_piter_<S,F>::pset_if_fwd_piter_(const pset_if<S,F>& s)
  {
    this->change_target(s);
  }

  template <typename S, typename F>
  inline
  void
  pset_if_fwd_piter_<S,F>::start_()
  {
    this->pi_.start();
    while (this->pi_.is_valid() && ! this->s_->pred(this->pi_))
      this->pi_.next();
  }

  template <typename S, typename F>
  inline
  void
  pset_if_fwd_piter_<S,F>::next_()
  {
    do
      this->pi_.next();
    while (this->pi_.is_valid() && ! this->s_->pred(this->pi_));
  }
  
  template <typename S, typename F>
  inline
  const S&
  pset_if_fwd_piter_<S,F>::pi_set_from_(const pset_if<S,F>& s) const
  {
    return s.overset();
  }

  // FIXME: pset_if_bkd_piter_<S,F>


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_PSET_IF_PITER_HH
