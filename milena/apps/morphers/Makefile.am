# Copyright (C) 2011, 2012, 2013, 2014 EPITA Research and Development
# Laboratory (LRDE).
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

# Illustrations of morphers.

include $(top_srcdir)/milena/apps/apps.mk

noinst_PROGRAMS =				\
  mask+channel					\
  recorder					\
  recorder-bft					\
  recorder-wst					\
  mask+recorder					\
  lazy_recorder

noinst_HEADERS = recorder.hh lazy_recorder.hh

recorder_bft_SOURCES = recorder-bft.cc image2d-skel.hh

EXTRA_DIST = iz_input.pbm

if HAVE_MAGICKXX
noinst_PROGRAMS += iz
iz_CPPFLAGS = $(AM_CPPFLAGS) $(MAGICKXX_CPPFLAGS)
iz_LDFLAGS = $(AM_LDFLAGS) $(MAGICKXX_LDFLAGS)

# This target is phony: `iz' never writes `iz_output.ppm' actually.
iz_output.ppm: iz$(EXEEXT) iz_input.pbm
	./iz$(EXEEXT) $(srcdir)/iz_input.pbm $@
# Likewise, the dependency of this target is a (wrong) shortcut.
tmp_iz.avi: iz_output.ppm
	mencoder "mf://tmp_iz*.png" -o tmp_iz.avi -ovc lavc -lavcopts vcodec=mjpeg -speed 50
endif HAVE_MAGICKXX

MOSTLYCLEANFILES =				\
  lena-mask-channel.ppm				\
  lena-fill??????.ppm				\
  lena-fill-lazy??????.ppm			\
  picasso-bft??????.pbm				\
  lena-wst??????.ppm				\
  lena-roi-fill??????.ppm			\
  tmp_clo.pgm					\
  tmp_dmap.pgm					\
  tmp_ws_superpose.ppm				\
  tmp_ws.ppm					\
  tmp_lab.pgm					\
  tmp_iz.pgm					\
  tmp_iz??????.png				\
  tmp_iz.ppm					\
  tmp_iz_input.ppm

# FIXME: Also produce movies (see comments in recorder.cc and
# mask+recorder.cc.)
