#include <oln/basics2d.hh>

#include <oln/morpho/opening.hh>
#include <oln/morpho/geodesic_dilation.hh>
#include <oln/morpho/geodesic_erosion.hh>
#include <oln/morpho/reconstruction.hh>
#include <oln/morpho/extrema.hh>
#include <oln/level/fill.hh>
#include <ntg/all.hh>
#include "check.hh"
#include "data.hh"

using namespace oln;
using namespace oln::level;
using namespace ntg;

#define OK_OR_FAIL				\
      std::cout << "OK" << std::endl;		\
    else					\
      {						\
	std::cout << "FAIL" << std::endl;	\
	fail = true;				\
      }

bool
check()
{
  bool fail = false;

  image2d<int_u8> lena = load(rdata("lena128.pgm"));
  image2d<int_u8> lena_open = morpho::opening(lena, win_c4p());
  image2d<ntg::bin> minima_map(lena.size());
  image2d<int_u8> max_map(lena.size());

  level::fill (minima_map, false);
  level::fill (max_map, 255);
  minima_map(10,10) = true;
  minima_map(100,100) = true;

  // test regional minima
  {
    std::cout << "regional minima sequential ... " << std::flush;
    if (level::is_equal(morpho::sure::regional_minima(lena, neighb_c4()),
			morpho::sequential::regional_minima(lena, neighb_c4())))
      OK_OR_FAIL;

    std::cout << "regional minima hybrid ... " << std::flush;
    if (level::is_equal(morpho::sure::regional_minima(lena, neighb_c4()),
			morpho::hybrid::regional_minima(lena, neighb_c4())))
      OK_OR_FAIL;
  }

  // test minima imposition
  {
    std::cout << "minima impos sequential ... " << std::flush;
    if (level::is_equal(morpho::sure::minima_imposition(lena,
						       minima_map, neighb_c4()),
			morpho::sequential::minima_imposition(lena,
							     minima_map,
							     neighb_c4())))
      OK_OR_FAIL;

    std::cout << "minima impos hybrid ... " << std::flush;
    if (level::is_equal(morpho::sure::minima_imposition(lena,
						       minima_map, neighb_c4()),
			morpho::hybrid::minima_imposition(lena,
							 minima_map,
							 neighb_c4())))
      OK_OR_FAIL;
  }

  // test geodesic erosion and geodesic dilation
  {
    std::cout << "geodesic erosion ... " << std::flush;
    if (level::is_equal(morpho::geodesic_erosion(lena, lena_open, neighb_c4()),
			morpho::sure::geodesic_erosion(lena, lena_open,
							neighb_c4())))
      OK_OR_FAIL;

    std::cout << "geodesic dilation ... " << std::flush;
    if (level::is_equal(morpho::geodesic_dilation(lena_open, lena, neighb_c4()),
			morpho::sure::geodesic_dilation(lena_open,
							 lena, neighb_c4())))
      OK_OR_FAIL;
  }



  {
    std::cout << "sequential G.R. erosion ... " << std::flush;
    if (level::is_equal(morpho::sequential::geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4()),
			morpho::sure::geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4())))
      OK_OR_FAIL;

    std::cout << "hybrid G.R. erosion ... " << std::flush;
    if (level::is_equal(morpho::hybrid::geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4()),
			morpho::sure::geodesic_reconstruction_erosion
			(max_map, lena_open, neighb_c4())))
      OK_OR_FAIL;
  }

  {
    std::cout << "sequential G.R. dilation ... " << std::flush;
    if (level::is_equal(morpho::sequential::geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4()),
			morpho::sure::geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4())))
      OK_OR_FAIL;

    std::cout << "hybrid G.R. dilation ... " << std::flush;
    if (level::is_equal(morpho::hybrid::geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4()),
			morpho::sure::geodesic_reconstruction_dilation
			(lena_open, lena, neighb_c4())))
      OK_OR_FAIL;
  }

  return fail;
}
