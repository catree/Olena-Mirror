# include <mln/core/var.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/image_if.hh>
# include <mln/core/image/extended.hh>
# include <mln/core/routine/extend.hh>

# include <mln/core/alias/window2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/make/double_neighb2d.hh>
# include <mln/core/site_set/p_centered.hh>


# include <mln/literal/black.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/int_u16.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>

# include <mln/value/rgb8.hh>
# include <mln/io/ppm/load.hh>
# include <mln/io/ppm/save.hh>

# include <mln/accu/min_max.hh>

# include <mln/fun/i2v/array.hh>
# include <mln/fun/p2v/iota.hh>

# include <mln/level/paste.hh>
# include <mln/level/fill.hh>
# include <mln/level/transform.hh>
# include <mln/extension/fill.hh>

# include <mln/morpho/closing_area.hh>


# include <mln/debug/println.hh>

# include "src/distance.hh"

namespace mln
{
  template <typename I, typename N, typename Ic, typename Nc>
  struct min_tree_
  {
    typedef mln_site(I) point;
    typedef p_array<point> S;

    // in:
    const I& f;
    const N& nbh;
    const Ic& ref;
    const Nc& nbhc;

    // aux:
    S s;
    mln_ch_value(I, bool)  deja_vu;
    mln_ch_value(I, point) parent;
    mln_ch_value(I, bool) resp;
    mln_ch_value(I, point) zpar;

    // attached data:
    unsigned lambda;
    mln_ch_value(I, value::rgb8) color;
    //mln_ch_value(Ic, value::rgb8) values;
    //initialize(values, ref);
    //mln_ch_value(I, int) comp;

    min_tree_(const I& f, const N& nbh, const Ic& ref, const Nc& nbhc,
              int lambda)
      : f(f),
	nbh(nbh),
	ref(ref),
	nbhc(nbhc),
        lambda(lambda)
    {
      run();
    }

    void run()
    {
      // init
      {
	initialize(deja_vu, f);
	initialize(parent, f);
	initialize(resp, f);
	initialize(zpar, f);
	initialize(color, f);

	mln::level::fill(deja_vu, false);
        mln::level::fill(color, value::rgb8(255, 255, 255));

	s = level::sort_psites_increasing(f);
      }

      // first pass
      {
	mln_fwd_piter(S) p(s);
	mln_niter(N) n(nbh, p);
	for_all(p)
	{
	  make_set(p);
	  for_all(n)
	    if (f.has(n) && deja_vu(n))
	      do_union(n, p);
	  deja_vu(p) = true;
	}
      }

      // second pass: canonization
      {
	mln_bkd_piter(S) p(s);
	for_all(p)
	{
	  point q = parent(p);
	  if (f(parent(q)) == f(q))
          {
	    parent(p) = parent(q);
            resp(q) = false;
          }
	}
      }

      // third pass: Merging region with distance(color) < lambda
      {
	mln_fwd_piter(S) p(s);
	for_all(p)
	{
          point q = parent(p);
          if (resp(p) && distance(color(p), color(q)) < lambda)
          {
            resp(p) = false;
            update_data(q, color(p));
          }
	}
      }

    } // end of run()

    void make_set(const point& p)
    {
      parent(p) = p;
      zpar(p) = p;
      init_data(p);
    }

    void set_parent(const point& r, const point& p)
    {
      parent(r) = p;
      merge_data(r, p);
    }

    bool is_root(const point& p) const
    {
      return parent(p) == p;
    }

    bool is_node(const point& p) const
    {
      //return is_root(p) || f(parent(p)) != f(p);
      return (is_root(p) || resp(p));
    }

    point find_root(const point& x)
    {
      if (zpar(x) == x)
	return x;
      else
	return zpar(x) = find_root(zpar(x));
    }

    point find_representative(const point& x)
    {
      if (parent(x) == x || resp(x))
	return x;
      else
	return find_representative(parent(x));
    }

    void do_union(const point& n, const point& p)
    {
       point r = find_root(n);
      if (r != p)
      {
	set_parent(r, p);
	zpar(r) = p;
      }
    }

    void init_data(const point& p)
    {
      int red =0, green = 0, blue = 0;

      mln_niter(Nc) n(nbhc, p);
      for_all(n)
      {
        red += ref(n).red();
        green += ref(n).green();
        blue += ref(n).blue();
      }

      red /= 2;
      green /= 2;
      blue /= 2;

      color(p).red() = red;
      color(p).green() = green;
      color(p).blue() = blue;

      resp(p) = true;
    }

    void merge_data(const point& r, const point& p)
    {
      if (f(p) == f(r))
      {
        resp(p) = false;
        color(r) = (color(r) + color(p)) / 2;
      }
    }

    void update_data(const point& p, value::rgb8 val)
    {
      color(p) = (color(p) + val) / 2;
      if (parent(p) != p && !resp(p))
        update_data(parent(p), color(p));
    }

  };
}

namespace mln
{
  image2d<value::int_u16> convert_to_grey(const image2d<value::rgb8>& data)
  {
    image2d<value::int_u16> output(data.domain());
    mln_piter_(image2d<value::int_u16>) p(output.domain());
    for_all(p)
      output(p) = (int) (data(p).red() * 0.3 + data(p).green() * 0.58 + data(p).blue()) * 0.12;
    return output;
  }
}  // end of mln

namespace mln
{

  struct colorize : Function_v2v< colorize >
  {
    typedef value::rgb8 result;
    colorize(unsigned max)
      : lut(max + 1)
    {
      lut[0] = literal::black;
      for (unsigned i = 1; i <= max; ++i)
	lut[i] = result(100 + std::rand() % 150,
			100 + std::rand() % 150,
			100 + std::rand() % 150);
    }
    result operator()(unsigned i) const
    {
      return lut[i];
    }
    std::vector<result> lut;
  };

  template <typename I>
  I display_edge(const I& ima, mln_value(I) bg, unsigned zoom)
  {
    unsigned nrows = ima.nrows() / 2 + 1;
    unsigned ncols = ima.ncols() / 2 + 1;
    I output(nrows * (zoom + 1) - 1,
	     ncols * (zoom + 1) - 1);
    level::fill(output, bg);

    mln_VAR(edge, ima | is_edge);
    mln_piter(edge_t) p(edge.domain());
    for_all(p)
      if (p.row() % 2) // horizontal edge
	{
	  unsigned row = (p.row() / 2 + 1) * (zoom + 1) - 1;
	  unsigned col = (p.col() / 2) * (zoom + 1);
	  for (unsigned i = 0; i < zoom; ++i)
	    output.at(row, col + i) = ima(p);
	}
      else // vertical edge
	{
	  unsigned row = (p.row() / 2) * (zoom + 1);
	  unsigned col = (p.col() / 2 + 1) * (zoom + 1) - 1;
	  for (unsigned i = 0; i < zoom; ++i)
	    output.at(row + i, col) = ima(p);
	}
    return output;
  }

  template <typename I>
  I display_edge(const I& ima, unsigned zoom)
  {
    unsigned nrows = ima.nrows() / 2 + 1;
    unsigned ncols = ima.ncols() / 2 + 1;
    I output(nrows * (zoom + 1) - 1,
             ncols * (zoom + 1) - 1);

    mln_VAR( cell, ima | is_cell );
    mln_piter(cell_t) q(cell.domain());
    for_all(q)
    {
      unsigned row = (q.row() / 2) * (zoom + 1);
      unsigned col = (q.col() / 2) * (zoom + 1);
      for (unsigned i = 0; i < zoom; ++i)
        for (unsigned j = 0; j < zoom; ++j)
          output.at(row + i, col + j) = ima(q);
    }

    mln_VAR( edge, ima | is_edge );
    mln_piter(edge_t) p(edge.domain());
    for_all(p)
      if (p.row() % 2) // horizontal edge
        {
          unsigned row = (p.row() / 2 + 1) * (zoom + 1) - 1;
          unsigned col = (p.col() / 2) * (zoom + 1);
          for (unsigned i = 0; i < zoom; ++i)
            output.at(row, col + i) = ima(p);
        }
      else // vertical edge
        {
          unsigned row = (p.row() / 2) * (zoom + 1);
          unsigned col = (p.col() / 2 + 1) * (zoom + 1) - 1;
          for (unsigned i = 0; i < zoom; ++i)
            output.at(row + i, col) = ima(p);
        }
    return output;
  }


  namespace morpho
  {

    template <typename I, typename N>
    mln_concrete(I)
    dilation(const I& input, const N& nbh)
    {
      typedef mln_value(I) V;

      mln_concrete(I) output;
      initialize(output, input);

      mln_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
      {
	for_all(n)
	  if (input.has(n) && input(n) != value::rgb8(0,0,0))
	    output(p) = input(n);
      }
      return output;
    }
  } // mln::morpho

} // mln



template <typename T>
mln::image2d<T>
image2cells(const mln::image2d<T>& input)
{
  mln::image2d<T> output(2 * input.nrows() - 1,
			 2 * input.ncols() - 1);
  for (unsigned row = 0; row < input.nrows(); ++row)
    for (unsigned col = 0; col < input.ncols(); ++col)
      output.at(2 * row, 2 * col) = input.at(row, col);
  return output;
}


template <typename T>
mln::image2d<T>
cells2image(const mln::image2d<T>& input)
{
  mln::image2d<T> output((input.nrows() + 1) / 2,
			 (input.ncols() + 1) / 2);
  for (unsigned row = 0; row < input.nrows(); row += 2)
    for (unsigned col = 0; col < input.ncols(); col += 2)
      output.at(row / 2, col / 2) = input.at(row, col);
  return output;
}


template <typename I, typename N, typename Ic, typename Nc>
unsigned min_tree(const I& f, const N& nbh, const Ic& ref, const Nc& nbhc,
                  int lambda)
{
  using namespace mln;

  min_tree_<I,N,Ic,Nc> run(f, nbh, ref, nbhc, lambda);


  mln_piter(I) p(f.domain());
  unsigned nnodes = 0;
  for_all(p)
  {
    if (run.is_node(p))
      ++nnodes;
  }

  colorize colors(nnodes);
  image2d<value::rgb8> tmp(ref.domain());
  level::fill(tmp, literal::black);
  image2d<value::rgb8> tmp2(ref.domain());
  level::fill(tmp, ref);

  mln_piter(I) q(f.domain());
  unsigned int i = 0;
  for_all(q)
  {
    if (run.is_node(q))
    {
      tmp(q) = run.color(q);
      tmp2(q) = colors(i);
      i++;
    }
  }

  mln_piter(I) r(f.domain());
  for_all(r)
  {
    if (!run.is_node(r))
    {
      tmp(r) = tmp(run.find_representative(r));
      tmp2(r) = tmp2(run.find_representative(r));
    }
  }

  image2d<value::rgb8> to_display(tmp.domain());
  image2d<value::rgb8> to_display2(tmp2.domain());

  level::fill(to_display, literal::black);
  level::paste((tmp | is_edge), to_display);
  level::paste(morpho::dilation(to_display, c4()), to_display);

  level::fill(to_display2, literal::black);
  level::paste((tmp2 | is_edge), to_display2);
  level::paste(morpho::dilation(to_display2, c4()), to_display2);


  io::ppm::save(display_edge(tmp, literal::black, 3), "edge.ppm");
  io::ppm::save(tmp, "full.ppm");
  io::ppm::save(cells2image(to_display), "colorize.ppm");

  io::ppm::save(display_edge(tmp2, literal::black, 3), "edge2.ppm");
  io::ppm::save(cells2image(to_display2), "colorize2.ppm");

  return nnodes;
}


template <typename I>
I
do_it(I& input, int lambda, unsigned& nbasins)
{
  using namespace mln;

  /// Graph creation
  I graph;
  create_graph(input, graph, value::rgb8(0, 0, 0));

  // Initialization
  image2d<value::int_u16> ima = convert_to_grey(graph);

  // Neigbhorhood
  // e2c
  bool e2c_h[] = { 0, 1, 0,
		   0, 0, 0,
		   0, 1, 0 };
  bool e2c_v[] = { 0, 0, 0,
		   1, 0, 1,
		   0, 0, 0 };

  mln_VAR(e2c, make::double_neighb2d(is_row_odd, e2c_h, e2c_v));

  bool e2e_h[] = { 0, 0, 1, 0, 0,
		   0, 1, 0, 1, 0,
		   0, 0, 0, 0, 0,
		   0, 1, 0, 1, 0,
		   0, 0, 1, 0, 0 };

  bool e2e_v[] = { 0, 0, 0, 0, 0,
		   0, 1, 0, 1, 0,
		   1, 0, 0, 0, 1,
		   0, 1, 0, 1, 0,
		   0, 0, 0, 0, 0 };
  mln_VAR(e2e, make::double_neighb2d(is_row_odd, e2e_h, e2e_v));

  // Algorithm
  distance(extend((graph | is_edge).rw(), pw::value(graph)), e2c, ima);

  io::pgm::save(ima, "edge.pgm");

  nbasins = min_tree((ima | is_edge), e2e, graph, e2c, lambda);

  return graph;
}

void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda" << std::endl;
  std::cerr << "  lambda >= 0" << std::endl;
  abort();
}

int main(int argc, char* argv[])
{
  using namespace mln;

  if (argc != 3)
    usage(argv);

  int lambda = atoi(argv[2]);
  if (lambda < 0)
    usage(argv);

  image2d<value::rgb8> ima;
  io::ppm::load(ima, argv[1]);

  unsigned nbasins;
  image2d<value::rgb8> output = do_it(ima, lambda, nbasins);

  //io::ppm::save(output, argv[3]);
}