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

#ifndef TOPO_CMAP_INTERNAL_FUNCTOR_HH
# define TOPO_CMAP_INTERNAL_FUNCTOR_HH

#include <mlc/contract.hh>

#include <vector>

namespace oln {

  namespace topo {

    namespace cmap {

      namespace internal {

	template <class Inf>
	class any
	{
	public:
	  const Inf & self() const { return static_cast<const Inf &>(*this); }
	  Inf & self() { return static_cast<Inf &>(*this); }
	};

	template <class U, class V, class Inf>
	class anyfunc : public any<Inf>
	{
	public:
	  anyfunc() : _f(1) {}

	public:
	  void _resize(const unsigned new_size)
	  {
	    assertion (new_size >= _f.size());

	    _f.resize(new_size + 1);
	  }

	protected:
	  std::vector<V> _f;
	};

	//
	// Start sets: Labels & Darts
	//
	/////////

	template <class U>
	class labels
	{
	public:
	  static void resize(const unsigned new_size)
	  {
	    assertion(new_size >= _labels.size());

	    for (unsigned i = _labels.size(); i <= new_size; ++i)
	      _labels.push_back(i);
	  }

	  static void substitute(const U & l1, const U & l2)
	  {
	    assertion(l1 < _labels.size());

	    _labels[l1] = l2;
	  }

	protected:
	  static std::vector<U> _labels;
	};

	template <class U>
	class darts
	{
	public:
	  static void resize(const unsigned new_size)
	  {
	    assertion (new_size >= _darts.size());

	    for (unsigned i = _darts.size(); i <= new_size; ++i)
	      _darts.push_back(i);
	  }

	  static void erase(const U & d)
	  {
	    assertion (d < _darts.size());

	    _darts[d] = 0;
	  }

	protected:
	  static std::vector<U> _darts;
	};

	//
	// Functors defined upon Dart & Label sets.
	//
	///////////////

	// Class Alpha:
	// Alpha dart -> dart
	template <class U, class V = U>
	class alpha : public darts<U>, public anyfunc< U, V, alpha<U, V> >
	{
	public:
	  V & operator()(const U & d)
	  {
	    assertion (d < _f.size());

	    return _f[d];
	  }
	  const V & operator()(const U & d) const
	  {
	    assertion (d < _f.size());

	    return _f[d];
	  }

	public:
	  std::ostream & print(std::ostream & ostr) const
	  {
	    for (unsigned i = 1; i < _darts.size(); ++i)
	      if (_darts[i])
		ostr << "alpha(" << i << ") = " << (*this)(i) << std::endl;
	    return ostr;
	  }
	};

	// Class Sigma:
	// Sigma dart -> dart
	template <class U, class V = U>
	class sigma : public darts<U>, public anyfunc< U, V, sigma<U, V> >
	{
	public:
	  V & operator()(const U & d)
	  {
	    assertion (d < _f.size());

	    return _f[d];
	  }
	  const V operator()(const U & d) const
	  {
	    assertion(d < _f.size());

	    if (!_darts[d])
	      return 0;

	    U tmp = d;

	    do
	      tmp = _f[tmp];
	    while (!_darts[tmp]);

	    return tmp;
	  }

	public:
	  std::ostream & print(std::ostream & ostr) const
	  {
	    for (unsigned i = 1; i < _darts.size(); ++i)
	      if (_darts[i])
		ostr << "sigma(" << i << ") = " << (*this)(i) << std::endl;
	    return ostr;
	  }
	};

	// Class Beta:
	// Beta label -> dart
	template <class U, class V = U>
	class beta : public labels<U>, public anyfunc< U, V, beta<U, V> >
	{
	public:
	  V & operator()(const U & l)
	  {
	    assertion (l < _f.size());

	    return _f[l];
	  }
	  const V operator()(const U & l) const
	  {
	    assertion(l < _f.size());

	    const U & tmp = _labels[l];

	    return l == tmp ? _f[l] : (*this)(tmp);
	  }

	public:
	  std::ostream & print(std::ostream & ostr) const
	  {
	    for (unsigned i = 1; i < _labels.size(); ++i)
	      if (_labels[i])
		ostr << "beta(" << i << ") = " << (*this)(i) << std::endl;
	    return ostr;
	  }
	};

	// Class Lambda:
	// Lambda(d) = beta-1(d)
	// Lambda dart -> label
	template <class U, class V = U>
	class lambda : public darts<U>, public anyfunc< U, V, lambda<U, V> >
	{
	public:
	  lambda(beta<V, U> & beta) : _beta(beta) {}

	  V & operator()(const U & d)
	  {
	    assertion (d < _f.size());

	    return _f[d];
	  }
	  const V operator()(const U & d) const
	  {
	    assertion(d < _f.size());

	    return _f[_beta(_f[d])];
	  }

	public:
	  std::ostream & print(std::ostream & ostr) const
	  {
	    for (unsigned i = 1; i < _darts.size(); ++i)
	      if (_darts[i])
		ostr << "lambda(" << i << ") = " << (*this)(i) << std::endl;
	    return ostr;
	  }

	private:
	  beta<V, U> & _beta;
	};

	// Class Moth:
	// Moth label -> label
	template <class U, class V = U>
	class moth : public labels<U>, public anyfunc< U, V, moth<U, V> >
	{
	public:
	  V & operator()(const U & l)
	  {
	    assertion (l < _f.size());

	    return _f[l];
	  }
	  const V operator()(const U & l) const
	  {
	    assertion(l < _f.size());

	    const V & tmp = _f[l];

	    return _labels[tmp] == tmp ? tmp : (*this)(_labels[tmp]);
	  }

	public:
	  std::ostream & print(std::ostream & ostr) const
	  {
	    for (unsigned i = 1; i < _labels.size(); ++i)
	      if (_labels[i])
		ostr << "moth(" << i << ") = " << (*this)(i) << std::endl;
	    return ostr;
	  }
	};

	// Class Daugh
	// Daugh label -> {label}*
	template <class U, class V>
	class daugh : public labels<U>, public anyfunc< U, V, daugh<U, V> >
	{
	public:
	  V & operator()(const U & l)
	  {
	    assertion (l < _f.size());

	    return _f[l];
	  }
	  const V operator()(const U & l) const
	  {
	    assertion(l < _f.size());

	    const U & tmp = _labels[l];

	    return l == tmp ? _f[l] : (*this)(tmp);
	  }

	public:
	  std::ostream & print(std::ostream & ostr) const
	  {
	    for (unsigned i = 1; i < _labels.size(); ++i)
	      if (_labels[i])
		ostr << "daugh(" << i << ") = " << (*this)(i) << std::endl;
	    return ostr;
	  }
	};

      } // end internal
    } // end cmap
  } // end topo
} // end oln

template <class U, class V, class Inf>
inline std::ostream &
operator<<(std::ostream & ostr,
	   const oln::topo::cmap::internal::anyfunc<U, V, Inf> & f);

# include <oln/topo/cmap-functor.hxx>

#endif // !TOPO_CMAP_INTERNAL_FUNCTOR_HH
