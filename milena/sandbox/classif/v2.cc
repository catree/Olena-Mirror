#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/histo/data.hh>

#include <mln/value/all.hh>

#include <mln/level/fill.hh>
#include <mln/geom/all.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/arith/revert.hh>
#include <mln/core/alias/neighb3d.hh>

#include "min_tree.hh"

using namespace mln;

template <typename I>
mln::image3d<unsigned>
fill_histo(const I& ima, int f)
{
  const value::int_u8 v = 255 / f; // FIXME
  image3d<unsigned> histo(v,v,v);
  level::fill(histo, 0);
  unsigned i = 0;

  mln_piter(I) p(ima.domain());
  for_all(p)
  {
    point3d p3(ima(p).red() / f, ima(p).green() / f, ima(p).blue() / f);
    histo(p3)++;
  }
  return histo;
}

template <typename I, typename N>
unsigned
compute_min_tree(const I& ima, const N& nbh)
{
  min_tree_<I,N> run(ima, nbh);

  mln_piter(I) p(ima.domain());
  unsigned nnodes = 0;
  for_all(p)
    if (run.is_node(p))
      ++nnodes;

  return nnodes;
}

bool usage(int argc, char ** argv)
{
  if (argc != 3)
  {
    std::cout << "usage: " << argv[0] << " image div_factor" << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char* argv[])
{
  if (not usage(argc, argv))
    return 1;
  image2d<value::rgb8> ima;
  ima = io::ppm::load<value::rgb8>(argv[1]);
  const int div_factor = atoi(argv[2]);

  //make histo
  image3d<unsigned> histo = fill_histo(ima,div_factor);

  //revert histo
  image3d<unsigned> rhisto = arith::revert(histo);

  // Compute min_tree
  compute_min_tree(rhisto, c6());
}
