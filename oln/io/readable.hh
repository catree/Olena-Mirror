// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_IO_READABLE_HH_
# define OLENA_IO_READABLE_HH_

# include <oln/config/system.hh>
# include <string>

namespace oln {

  namespace io {

    namespace internal {

      template<typename T>
      bool read_any(T& output, const std::string& name);

      class anything
      {
      public:
	anything(const std::string& str) : _str(str) {}

	anything(const char* c) : _str(c) {}

	template< typename T >
	T& assign(T& output) const
	{
	  read_any(output, _str);
	  return output;
	}

      private:
	std::string _str;
      };

    } // end of internal

  } // end of io

} // end of oln


#endif // OLENA_IO_READABLE_HH_
