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

#ifndef MLN_UTIL_TREE_HH
# define MLN_UTIL_TREE_HH

# include <vector>

/*!
 * \file   mln/util/tree.hh
 *
 * \brief  Definition of a generic general tree.
 *
 */

namespace mln
{

  namespace util
  {

    template <typename T>
    struct node
    {
      node();
      node(T& elt);

      T& content();
      const T& content() const;
      void add_child(T& elt);
      void set_parent(node<T>* parent);
      node<T>* get_parent();
      void print_rec(int n) const;
      void print() const;
      int  search_rec(node<T>** res, T& elt);
      node<T>* search(T& elt);

      T elt_;
      node<T>* parent_;
      std::vector< node<T>* > child_;
    };

    template <typename T>
    struct tree
    {
      typedef node<T> node_t;
      tree();
      tree(node<T>* root);

      tree<T>* tree_get(T& elt);
      void add_tree_up (T& elt);
      void add_tree_down (T& elt);

      node<T>* root_;
    };

# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    tree<T>::tree()
      : root_ (0)
    {
    }

    template <typename T>
    tree<T>::tree(node<T>* root)
      : root_ (root)
    {
      mln_assertion (root != 0);
    }

    template <typename T>
    tree<T>*
    tree<T>::tree_get(T& elt)
    {
      node<T>* n = root_->search(elt);
      tree<T>* res = new tree (n);

      return res;
    }

    template <typename T>
    void
    tree<T>::add_tree_up(T& elt)
    {
      node<T>* n = new node<T> (elt);
      root_->parent_ = n;
      n->child_.push_back (root_);
      root_ = n;
    }

    template <typename T>
    void
    tree<T>::add_tree_down(T& elt)
    {
      node<T>* n = new node<T> (elt);
      root_->child_.push_back (n);
    }


    template <typename T>
    node<T>::node()
      : parent_ (0)
    {
    }

    template <typename T>
    node<T>::node(T& elt)
      : elt_ (elt),
	parent_ (0)
    {
    }

    template <typename T>
    const T&
    node<T>::content() const
    {
      return elt_;
    }

    template <typename T>
    T&
    node<T>::content()
    {
      return elt_;
    }

    template <typename T>
    void
    node<T>::add_child(T& elt)
    {
      node<T>* s = new node<T>(elt);

      s->parent_ = this;
      this->child_.push_back(s);
    }

    template <typename T>
    void
    node<T>::set_parent(node<T>* parent)
    {
      mln_assertion(parent != 0);
      parent_ = parent;
      parent->child_.push_back(this);
    }


    template <typename T>
    node<T>*
    node<T>::get_parent()
    {
      return parent_;
    }

    template <typename T>
    int
    node<T>::search_rec(node<T>** res, T& elt)
    {
      if (elt == this->elt_)
	{
	  *res = this;
	  return 1;
	}
      else
	{
	  for (typename std::vector<node<T>* >::iterator it = this->child_.begin();
	       it != this->child_.end(); ++it)
	    {
	      if ((**it).search_rec(res, elt))
		return 1;
	    }
	}
      return 0;
    }

    template <typename T>
    node<T>*
    node<T>::search(T& elt)
    {
      node<T>* res = 0;

      if (search_rec(&res, elt))
	return res;
      return 0;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util


} // end of namespace mln


#endif // !MLN_UTIL_TREE_HH
