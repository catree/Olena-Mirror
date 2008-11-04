// Copyright (C) 2008 EPITA Research and Development Laboratory
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
// software library without restriction.  Specificstopy, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_TRACE_STOP_HH
# define MLN_TRACE_STOP_HH

/// \file mln/trace/stop.hh
/// Stop printing traces except if trace::full_trace is enabled.
/// \sa mln::trace::resume

# include <iostream>

# include <mln/trace/quiet.hh>


namespace mln
{

  namespace trace
  {

    void stop();

# ifndef MLN_INCLUDE_ONLY

    inline
    void stop()
    {
      internal::is_quiet = quiet;
      if (!full_trace)
	quiet = true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::trace

} // end of namespace mln


#endif // ! MLN_TRACE_STOP_HH