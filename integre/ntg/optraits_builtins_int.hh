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

#ifndef OPTRAITS_BUILTINS_INT_HH
# define OPTRAITS_BUILTINS_INT_HH

# include <ntg/config/system.hh>

# include <ntg/optraits_scalar.hh>
# include <ntg/optraits_builtins_int_defs.hh>
# include <ntg/typetraits_builtins_int.hh>

// FIXME: isn't this included by system.hh ?
# include <limits.h>

# include <string>

//
//  optraits for builtins
//
//  dev note : optraits<builtins> artihmetic operators are only called
//  when there is an operator_xxx_traits defined, else buitlins are
//  converted into oln type.
//
///////////////////////////////////////////////////////////////////////////////

namespace ntg
{

  OPTRAITS_BUILTIN_INT_U(unsigned long,  ULONG_MAX);
  OPTRAITS_BUILTIN_INT_U(unsigned int,   UINT_MAX);
  OPTRAITS_BUILTIN_INT_U(unsigned short, USHRT_MAX);
  OPTRAITS_BUILTIN_INT_U(unsigned char,  UCHAR_MAX);

  OPTRAITS_BUILTIN_INT_S(signed long,  LONG_MIN, LONG_MAX);
  OPTRAITS_BUILTIN_INT_S(signed int,   INT_MIN,  INT_MAX);
  OPTRAITS_BUILTIN_INT_S(signed short, SHRT_MIN, SHRT_MAX);
  OPTRAITS_BUILTIN_INT_S(signed char,  CHAR_MIN, CHAR_MAX);

} // end of ntg

#endif // ndef OPTRAITS_BUILTINS_INT_HH
