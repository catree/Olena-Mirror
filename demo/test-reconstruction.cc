/* FIXME: This is not really a demo.  */

#include <oln/basics2d.hh>

#include <oln/morpho/erosion.hh>
#include <oln/morpho/dilation.hh>
#include <oln/morpho/opening.hh>
#include <oln/morpho/geodesic_dilation.hh>
#include <oln/morpho/geodesic_erosion.hh>
#include <oln/morpho/reconstruction.hh>
#include <oln/morpho/extrema_killer.hh>

#include <oln/utils/timer.hh>

#include <iostream>

using namespace oln;
using namespace oln::level;
using std::cout;
using std::endl;

int main()
{
  image2d<int_u8> lena = load("../img/lena128.pgm");
  image2d<int_u8> lena_open = morpho::opening(lena, win_c4p());

  misc::timer t;

  t.start();
  morpho::sure::geodesic_reconstruction_erosion(lena, lena_open, neighb_c4());
  cout << "rec ero sure " << t.stop() << endl;

  t.start();
  morpho::sequential::geodesic_reconstruction_erosion(lena, lena_open,
						     neighb_c4());
  cout << "rec ero sequential " << t.stop() << endl;

  t.start();
  morpho::hybrid::geodesic_reconstruction_erosion(lena, lena_open, neighb_c4());
  cout << "rec ero hybrid " << t.stop() << endl;

  t.start();
  morpho::sure::geodesic_reconstruction_dilation(lena_open, lena, neighb_c4());
  cout << "rec dil sure " << t.stop() << endl;

  t.start();
  morpho::sequential::geodesic_reconstruction_dilation(lena_open, lena,
						      neighb_c4());
  cout << "rec dil sequential " << t.stop() << endl;

  t.start();
  morpho::hybrid::geodesic_reconstruction_dilation(lena_open, lena,
						  neighb_c4());
  cout << "rec dil hybrid " << t.stop() << endl;


  t.start();
  morpho::sure_minima_killer(lena, 5, neighb_c4());
  cout << "sure minima killer  5 " << t.stop() << endl;

  t.start();
  morpho::sure_minima_killer(lena, 60, neighb_c4());
  cout << "sure minima killer  30 " << t.stop() << endl;
  t.start();

  morpho::fast_minima_killer(lena, 5, neighb_c4());
  cout << "minima killer  5 " << t.stop() << endl;

  t.start();
  morpho::fast_minima_killer(lena, 60, neighb_c4());
  cout << "minima killer  30 " << t.stop() << endl;

}
