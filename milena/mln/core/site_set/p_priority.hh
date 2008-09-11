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

#ifndef MLN_CORE_P_PRIORITY_HH
# define MLN_CORE_P_PRIORITY_HH

/*! \file mln/core/site_set/p_priority.hh
 *
 * \brief Definition of a priority queue of sites.
 *
 * \todo Rename file as p_priority.hh
 */

# include <map>
# include <mln/core/site_set/p_double.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/util/set.hh>


namespace mln
{

  // Forward declaration.
  template <typename P, typename Q> class p_priority;



  namespace trait
  {

    template <typename P, typename Q>
    struct site_set_< p_priority<P,Q> >
    {
      typedef trait::site_set::nsites::known     nsites;
      typedef trait::site_set::bbox::unknown     bbox;
      typedef trait::site_set::contents::growing contents;
      typedef trait::site_set::arity::multiple   arity;
    };

  } // end of namespace trait




  /*! \brief Priority queue class.
   *
   * FIXME
   */
  template <typename P, typename Q>
  class p_priority : public internal::site_set_base_< mln_site(Q),
						      p_priority<P,Q> >
  {
    typedef p_priority<P,Q> self_;
  public:

    /// Element associated type.
    typedef mln_element(Q) element;


    /// Psite associated type.
    typedef p_double_psite<self_, Q> psite;

    /// Forward Site_Iterator associated type.
    typedef p_double_piter< self_,
			    mln_bkd_eiter(util::set<P>),
			    mln_fwd_piter(Q) > fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_double_piter< self_,
			    mln_fwd_eiter(util::set<P>),
			    mln_bkd_piter(Q) > bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor.
    p_priority();

    /// Test is the psite \p p belongs to this site set.
    bool has(const psite&) const;

    /// Test this set validity so returns always true.
    bool is_valid() const;

    /// Give the number of sites.
    unsigned nsites() const;


    /// Push in the queue with \p priority the element \p e.
    void push(const P& priority, const element& e);

    /// Insertion element associated type. 
    typedef std::pair<P, element> i_element;

    /// Insert a pair \p p_e (priority p, element e).
    void insert(const i_element& p_e);


    /// Pop (remove) from the queue an element with highest priority.
    /// If several elements have this priority, the least recently
    /// inserted is chosen.  \pre ! is_empty()
    void pop();

    /// Give an element with highest priority.  If several elements
    /// have this priority, the least recently inserted is chosen.
    /// \pre ! is_empty()
    const mln_element(Q)& front() const;

    /// Return an element with highest priority and remove it from the
    /// set.  If several elements have this priority, the least
    /// recently inserted is chosen.  \pre ! is_empty()
    mln_element(Q) pop_front();


    /// Clear the queue.
    void clear();


    /// Give the queue with the priority \p priority.  This method
    /// always works: if the priority is not in this set, an empty
    /// queue is returned.
    const Q& operator()(const P& priority) const;

    /// Give the set of priorities.
    const util::set<P>& priorities() const;

    /// Test if the \p priority exists.
    bool exists_priority(const P& priority) const;

    /// Give the highest priority.
    /// \pre ! is_empty()
    const P highest_priority() const;

    /// Give the lowest priority.
    /// \pre ! is_empty()
    const P lowest_priority() const;


    // Required by p_double-related classes.
    const util::set<P>& set_1_() const;
    const Q& set_2_(const P& priority) const;


    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

  protected:

    util::set<P>  p_;
    std::map<P,Q> q_;
    unsigned      n_;

    // Run invariance tests and return the result.
    bool run_() const;
  };



  template <typename P, typename Q>
  std::ostream& operator<<(std::ostream& ostr, const p_priority<P,Q>& pq);



# ifndef MLN_INCLUDE_ONLY

  template <typename P, typename Q>
  inline
  p_priority<P,Q>::p_priority()
  {
    n_ = 0;
    mln_invariant(run_());
  }

  template <typename P, typename Q>
  inline
  bool
  p_priority<P,Q>::has(const psite&) const
  {
    mln_invariant(run_());
    // FIXME
    return true;
  }

  template <typename P, typename Q>
  inline
  bool
  p_priority<P,Q>::is_valid() const
  {
    mln_invariant(run_());
    return true;
  }
  
  template <typename P, typename Q>
  inline
  unsigned
  p_priority<P,Q>::nsites() const
  {
    mln_invariant(run_());
    return n_;
  }

  template <typename P, typename Q>
  inline
  void
  p_priority<P,Q>::push(const P& priority, const element& e)
  {
    mln_invariant(run_());
    p_.insert(priority); // No-op if this priority already exists.
    q_[priority].push(e);
    ++n_;
    mln_invariant(run_());
  }

  template <typename P, typename Q>
  inline
  void
  p_priority<P,Q>::insert(const i_element& p_e)
  {
    this->push(p_e.first, p_e.second); // Also test invariants.
  }

  template <typename P, typename Q>
  inline
  void
  p_priority<P,Q>::pop()
  {
    mln_precondition(! this->is_empty()); // Also test invariants.
    P prior = highest_priority();
    q_[prior].pop();
    if (q_[prior].is_empty())
      {
	q_.erase(prior);
	p_.remove(prior);
      }
    --n_;
    mln_invariant(run_());
  }

  template <typename P, typename Q>
  inline
  const mln_element(Q)&
  p_priority<P,Q>::front() const
  {
    mln_precondition(! this->is_empty()); // Also test invariants.
    return q_[highest_priority()].front();
  }

  template <typename P, typename Q>
  inline
  mln_element(Q)
  p_priority<P,Q>::pop_front()
  {
    mln_precondition(! this->is_empty()); // Also test invariants.
    // FIXME: can be speeded up, return const& when possible...
    mln_element(Q) e = this->front();
    this->pop();
    return e;
  }

  template <typename P, typename Q>
  inline
  void
  p_priority<P,Q>::clear()
  {
    mln_invariant(run_());
    p_.clear();
    q_.clear();
    n_ = 0;
    mln_invariant(run_());
  }

  template <typename P, typename Q>
  inline
  std::size_t
  p_priority<P,Q>::memory_size() const
  {
    mln_invariant(run_());
    std::size_t mem_q = 0;
    typename std::map<P, Q>::const_iterator i;
    for (i = q_.begin(); i != q_.end(); ++i)
      mem_q += i->second.memory_size();
    return p_.memory_size() + sizeof(q_) + sizeof(n_);
  }
  
  template <typename P, typename Q>
  inline
  const Q&
  p_priority<P,Q>::operator()(const P& priority) const
  {
    static const Q nil_ = Q();
    if (exists_priority(priority)) // Also test invariants.
      {
	std::map<P,Q>& mq = const_cast<std::map<P,Q>&>(q_);
	mln_assertion(mq[priority].nsites() > 0);
	return mq[priority];
      }
    else
      return nil_;
  }

  template <typename P, typename Q>
  inline
  const util::set<P>&
  p_priority<P,Q>::priorities() const
  {
    mln_invariant(run_());
    return p_;
  }

  template <typename P, typename Q>
  inline
  bool
  p_priority<P,Q>::exists_priority(const P& priority) const
  {
    mln_invariant(run_());
    return p_.has(priority);
  }

  template <typename P, typename Q>
  inline
  const P
  p_priority<P,Q>::highest_priority() const
  {
    mln_precondition(! this->is_empty()); // Also test invariants.
    return p_.last_element();
  }

  template <typename P, typename Q>
  inline
  const P
  p_priority<P,Q>::lowest_priority() const
  {
    mln_precondition(! this->is_empty()); // Also test invariants.
    return p_.first_element();
  }
  
  template <typename P, typename Q>
  inline
  const util::set<P>&
  p_priority<P,Q>::set_1_() const
  {
    return p_;
  }

  template <typename P, typename Q>
  inline
  const Q&
  p_priority<P,Q>::set_2_(const P& priority) const
  {
    mln_precondition(p_.has(priority));
    std::map<P,Q>& mq = const_cast<std::map<P,Q>&>(q_);
    mln_precondition(mq[priority].nsites() > 0);
    return mq[priority];
  }

  template <typename P, typename Q>
  inline
  bool
  p_priority<P,Q>::run_() const
  {
    if (! implies(n_ == 0, p_.is_empty()))
      return false;

    if (! (p_.nelements() == q_.size()))
      // Containers p_ and q_ are not consistent in size!
      return false;

    mln_eiter(util::set<P>) p(p_);
    for_all(p)
      if (q_.find(p) == q_.end())
	// We have an empty queue (with priority p)!
	return false;

    typename std::map<P,Q>::const_iterator i;
    for (i = q_.begin(); i != q_.end(); ++i)
      if (! p_.has(i->first))
	// A priority is unknown (for a known queue)! 
	return false;

    return true;
  }


  // Operator<<.

  template <typename P, typename Q>
  std::ostream& operator<<(std::ostream& ostr, const p_priority<P,Q>& pq)
  {
    ostr << '{';
    mln_bkd_eiter(util::set<P>) p(pq.priorities());
    for_all(p)
      {
	ostr << ' ' << p << ':';
	ostr << pq.set_2_(p);
      }
    ostr << '}';
    return ostr;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_P_PRIORITY_HH
