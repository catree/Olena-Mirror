#include <mln/essential/2d.hh>
#include <mln/binarization/binarization.hh>
#include <mln/labeling/background.hh>

#include <mln/debug/colorize.hh>

#include <mln/registration/registration.hh>
#include <mln/registration/multiscale.hh>

struct threshold : mln::Function_p2b<threshold>
{
  bool operator()(const mln::value::rgb8& val) const
  {
    unsigned inty = (val.red() + val.blue() + val.green());
    return inty > 100 && inty < 600;
  }
};




template <typename I>
mln_concrete(I)
fill_holes(const mln::Image<I>& ima)
{
  using namespace mln;

  mln_concrete(I) output;
  initialize(output, ima);
  data::fill(output, true);

  value::label_16 nlabels;
  mln_ch_value(I, value::label_16) lbl = labeling::background(ima, c4(), nlabels);

  mln_VAR(lbl_count, labeling::compute(accu::meta::count(), ima, lbl, nlabels));

  unsigned max_id = 1;
  for (unsigned i = 2; i <= nlabels; ++i)
    if (lbl_count[max_id] < lbl_count[i])
      max_id = i;

  data::fill((output | pw::value(lbl) == max_id).rw(), false);

  return output;
}


template <typename I>
mln_concrete(I)
keep_largest_component(const mln::Image<I>& ima)
{
  using namespace mln;

  image2d<bool> in_bw_cleaned = fill_holes(ima);
  io::pbm::save(in_bw_cleaned, "0x_in_bw_cleaned.pbm");

  logical::not_inplace(in_bw_cleaned);
  image2d<bool> in_bw_cleaned_full = fill_holes(in_bw_cleaned);
  io::pbm::save(in_bw_cleaned_full, "0x_in_bw_cleaned_full.pbm");

  logical::not_inplace(in_bw_cleaned_full);
  return in_bw_cleaned_full;
}

int main(int, char* argv[])
{
  using namespace mln;
  using value::rgb8;
  using value::int_u8;
  using value::label_16;

  //Load image
  typedef image2d<rgb8> I;
  typedef image2d<bool> J;
  I in;
  io::ppm::load(in, argv[1]);

  threshold f;
  J in_bw = binarization::binarization(in, f);
  io::pbm::save(in_bw, "01_in_bw.pbm");

  J ima = keep_largest_component(in_bw);
  io::pbm::save(in_bw, "02_ima.pbm");

  std::cout << "Compute gradient" << std::endl;
  J ima_grad = morpho::gradient(ima, win_c4p());
  io::pbm::save(ima_grad, "03_ima_grad.pbm");

  I ref;
  io::ppm::load(ref, argv[2]);
  J ref_bw = binarization::binarization(ref, f);

  io::pbm::save(ref_bw, "ref_bw.pbm");

  J surf = keep_largest_component(ref_bw);
  J surf_grad = morpho::gradient(surf, win_c4p());

  std::cout << "Computing registration..." << std::endl;
  J cloud = ima_grad | pw::value(ima_grad) == true; //FIXME: cannot use pw::image with registration.
  //  mln_VAR(surface, (surf_grad | pw::value(surf_grad) == true));
  mln_VAR(registration, registration::multiscale(cloud, surf_grad, 5, 3));

  std::cout << "Build result image" << std::endl;
  J res;
  initialize(res, ima_grad);

  data::fill(res, false);

  mln_VAR(ig, (ima_grad | pw::value(ima_grad) == true));
  mln_piter_(ig_t) p(ig.domain());
  for_all(p)
    res(registration(p.to_site().to_vec())) = true;


  io::pbm::save(res, "04_registered.pbm");

}