# Copyright (C) 2008, 2009, 2010, 2011 EPITA Research and Development
# Laboratory (LRDE)
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

include $(top_srcdir)/milena/tests/tests.mk

SUBDIRS = skeleton

## FIXME: Also add tests for is_simple_cell, detach_cell,
## is_simple_point2d and detach_point.

check_PROGRAMS =				\
  complex					\
  connectivity_numbers_2d			\
  detach_pair					\
  is_facet					\
  is_simple_pair


TESTS = $(check_PROGRAMS)

MOSTLYCLEANFILES = detach_pair-out.vtk
