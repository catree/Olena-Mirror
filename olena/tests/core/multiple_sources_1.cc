#include <oln/basics2d.hh>
#include <oln/morpho/extrema_killer.hh>
#if HAVE_FFTW
# include <oln/transforms/fft.hh>
#endif

#include <ntg/all.hh>

#include "check.hh"
#include "data.hh"

#include <iostream>

using namespace oln;
using namespace ntg;
#if HAVE_FFTW
using namespace oln::transforms;
#endif
using namespace oln::level;
using namespace oln::morpho;

// defined in multiple_sources_2.cc
bool second_check();

bool first_check()
{
  image2d<int_u8> ima = io::load(rdata("lena"));
  fast_minima_killer(ima, 5, neighb_c4());
#if HAVE_FFTW
  fft<int_u8> fourier(ima);
#endif
  return false;
}

int main()
{
  bool fail = false;
  fail |= first_check();
  fail |= second_check();
  return fail;
}
