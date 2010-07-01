// Test de l'opérateur de Millet TSVal (HSV)
//
// Val = max(R,G,B)
// Sat = (max(R,G,B) - min(R,G,B))/max(R,G,B)
// si R = max(R,G,B) alors Hue = 60 * [(V-B)/(max(R,G,B)-min(R,G,B))]
// si G = max(R,G,B) alors Hue = 60 * [2 + (B-R)/(max(R,G,B)-min(R,G,B))]
// si B = max(R,G,B) alors Hue = 60 * [4 + (R-G)/(max(R,G,B)-min(R,G,B))]


#include <iostream>
#include <fstream>

#include <mln/accu/max_site.hh>
#include <mln/accu/math/count.hh>
#include <mln/accu/stat/histo1d.hh>

#include <mln/binarization/threshold.hh>

#include <mln/core/alias/point1d.hh>
#include <mln/core/alias/box1d.hh>
#include <mln/core/concept/image.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/transform.hh>
#include <mln/data/compute.hh>
#include <mln/data/stretch.hh>

#include <mln/debug/println.hh>

#include <mln/literal/colors.hh>
#include <mln/literal/grays.hh>

#include <mln/fun/v2v/rgb_to_hsv.hh>
#include <mln/fun/v2v/rgb_to_achromatism_map.hh>
#include <mln/fun/v2v/achromatism.hh>
#include <mln/fun/v2v/hue_concentration.hh>
#include <mln/fun/p2b/component_equals.hh>
#include <mln/fun/p2b/achromatic.hh>
#include <mln/fun/v2v/component.hh>

#include <mln/geom/nsites.hh>

#include <mln/img_path.hh>

#include <mln/io/plot/save_image_sh.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>

#include <mln/pw/cst.hh>
#include <mln/pw/value.hh>
//#include <mln/trace/quiet.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/hsv.hh>


mln::value::rgb8 label_color(const mln::value::rgb8 rgb)
{
  mln::value::hsv_f hsv = mln::fun::v2v::f_rgb_to_hsv_f(rgb);

  mln::value::rgb8 result;

  // Is it a gray level ?
  if (0 == hsv.sat())
  {
    // which result one ?
    if (82 > hsv.sat())
      result = mln::literal::black;
    else if (179 > hsv.sat())
      result= mln::literal::medium_gray;
    else
      result = mln::literal::white;
  }
  // Is it a true result color ?
  else if (14 > hsv.hue())
    result = mln::literal::red;
  else if (29 > hsv.hue())
  {
    // Is is brown or orange ?
    unsigned dist_orange = mln::math::abs(hsv.sat() - 184)
      + mln::math::abs(hsv.val() - 65);

    unsigned dist_brown  = mln::math::abs(hsv.sat() - 255)
      + mln::math::abs(hsv.val() - 125);

    if (dist_orange < dist_brown)
      result = mln::literal::orange;
    else
      result = mln::literal::brown;
  }
  else if (45 > hsv.hue())
  {
    // Is it green or yellow ?
    if (80 > hsv.val())
      result = mln::literal::green;
    else
      result = mln::literal::yellow;
  }
  else if (113 > hsv.hue())
    result = mln::literal::green;
  else if (149 > hsv.hue())
    result = mln::literal::cyan;
  else if (205 > hsv.hue())
    result = mln::literal::blue;
  else if (235 > hsv.hue())
    result = mln::literal::violet;
  else if (242 > hsv.hue())
    result = mln::literal::pink;
  else
    result = mln::literal::red;

  return result;
}

//unsigned count_histo(const mln::image1d<unsigned>& img)
template <typename I>
unsigned count_histo(const mln::Image<I>& img_)
{
  const I& img = exact(img_);

  mln_precondition(img.is_valid());

  unsigned     result = 0;
  mln_piter(I) p(img.domain());

  for_all(p)
    result += img(p);

  return result;
}

// calcul de contribution
float r(short p, unsigned histo_p, short x, unsigned histo_x)
{
  float result = mln::math::sqr(((float)histo_x / histo_p) * (x-p));

  return result;
}

template <typename I>
unsigned peak_histo(const mln::Image<I>& histo_)
{
  const I& histo = exact(histo_);

  mln_precondition(histo.is_valid());

  // Find the peak of the histogram
  unsigned v_max = mln::opt::at(histo, 0);
  short    p_max = 0;

  mln_piter(I) p(histo.domain());

  for_all(p)
  {
    if (v_max < histo(p))
    {
      v_max = histo(p);
      p_max = p.ind();
    }
  }

  return p_max;
}


// unsigned stddev_color(mln::image2d<mln::value::int_u8> input_int_u8,
// 		      const char *name_histo,
// 		      const char *name_image)
// {
//   typedef mln::point1d                                t_point1d;
//   typedef mln::value::rgb8                            t_rgb8;
//   typedef mln::value::int_u8                          t_int_u8;
//   typedef mln::image2d<t_rgb8>                        t_image2d_rgb8;
//   typedef mln::image2d<t_int_u8>                      t_image2d_int_u8;
//   typedef mln::image1d<unsigned>                      t_histo1d;
//   typedef mln::fun::v2v::rgb8_to_int_u8               t_rgb8_to_int_u8;
//   typedef mln::accu::meta::stat::histo1d              t_histo1d_fun;
//   typedef mln::accu::max_site<t_histo1d>              t_max_site_fun;

//   t_histo1d                                           histo;

//   std::cout << "histo : " << name_histo << std::endl;
//   std::cout << "image : " << name_image << std::endl;

//   histo        = mln::data::compute(t_histo1d_fun(), input_int_u8);

//   mln::io::pgm::save(input_int_u8, name_image);
//   mln::io::plot::save_image_sh(histo, name_histo);
//   mln::debug::println(histo);

//   // Find the peak of the histogram
//   unsigned v_max = mln::opt::at(histo, 0);
//   short    p_max = 0;

//   mln_piter_(t_histo1d) p(histo.domain());

//   for_all(p)
//   {
//     if (v_max < histo(p))
//     {
//       v_max = histo(p);
//       p_max = p.ind();
//     }
//   }

//   // Compute the specific stddev

//   float stddev_low = 0.0;
//   float stddev_up  = 0.0;
//   float stddev     = 0.0;

//   if (250 > p_max)
//     for (short i = p_max+1; i < p_max+6; ++i)
//       stddev_up += r(p_max, mln::opt::at(histo,p_max),
// 		     i, mln::opt::at(histo,i));

//   if (5 < p_max)
//     for (short i = p_max-1; i > p_max-6; --i)
//       stddev_low += r(p_max, mln::opt::at(histo,p_max),
// 		      i, mln::opt::at(histo,i));

//   stddev = (250 < p_max)? stddev_low : (5 > p_max)? stddev_up :
//     (stddev_low + stddev_up)/2;

//   std::cout << "max_site    : " << p_max << std::endl;
//   std::cout << "h(max_site) : " << v_max << std::endl;
//   std::cout << "stddev_up   : " << stddev_up << std::endl;
//   std::cout << "stddev_low  : " << stddev_low << std::endl;
//   std::cout << "stddev      : " << stddev << std::endl;

//   return 0;
// }


// -------------------------------------
// input image     <name>.ppm
// map             <name>-<map>.pgm
// thresholded map <name>-<map>.pbm
// histogram       <name>-<map>.sh
// decision        <name>-<map>.txt
// -------------------------------------

// Achromatism     <name>-achromatism.pgm

// call achromatism(input_rgb8, 7, 99.0)
void achromatism(mln::image2d<mln::value::rgb8> input_rgb8,
		 mln::value::int_u8             threshold,
		 float                          percentage)
{
  typedef mln::fun::v2v::rgb_to_achromatism_map<8> t_rgb_to_achromatism_map;

  mln::image2d<mln::value::int_u8> map;
  mln::image2d<mln::value::int_u8> view;
  mln::image2d<bool>               mask;
  mln::image1d<unsigned>           histo;
  unsigned                         cnt1;
  unsigned                         cnt2;
  float                            prop;
  bool                             result;


  map    = mln::data::transform(input_rgb8, t_rgb_to_achromatism_map());
  view   = mln::data::stretch(mln::value::int_u8(), map);
  mask   = mln::binarization::threshold(map, threshold);
  histo  = mln::data::compute(mln::accu::meta::stat::histo1d(),
			      map | (mln::pw::value(mask) == true));
  cnt1   = count_histo(histo);
  cnt2   = mln::geom::nsites(input_rgb8);
  prop   = (100.0 * (cnt2 - cnt1) / cnt2);
  result = (prop > percentage);


  std::ofstream txt_stream("achromatism.txt");
  txt_stream << "Achromatism" << std::endl;

  txt_stream << "Nbre pixels               : " << cnt2       << std::endl;
  txt_stream << "Nbre pixels achromatiques : " << (cnt2-cnt1)<< std::endl;
  txt_stream << "Percentage                : " << prop       << std::endl;
  txt_stream << "Image achromatique        : " << result     << std::endl;
  txt_stream << std::endl;

  txt_stream.flush();
  txt_stream.close();

  mln::io::pgm::save(view, "achromatism.pgm");
  mln::io::plot::save_image_sh(histo, "achromatism.sh");
  mln::io::pbm::save(mask, "achromatism.pbm");
}

// call low_saturation(input_rgb8, achromatism_mask, 100, 95.0)
void low_saturation(mln::image2d<mln::value::hsv_f>  input_hsvf,
		    mln::image2d<bool>               achromatism_mask,
		    mln::value::int_u8               threshold,
		    float                            percentage)
{
  typedef mln::value::hsv_f                   t_hsvf;
  typedef mln::value::hsv_f::s_type           t_sat;
  typedef mln::fun::v2v::component<t_hsvf,1>  t_component_s;

  mln::image2d<t_sat>              map;
  mln::image2d<mln::value::int_u8> view;
  mln::image2d<bool>               mask;
  mln::image1d<unsigned>           histo;
  unsigned                         cnt1;
  unsigned                         cnt2;
  float                            prop;
  bool                             result;


  map    = mln::data::transform(input_hsvf, t_component_s());
  view   = mln::data::stretch(mln::value::int_u8(), map);
// where is histo ??
  prop   = (100.0 * (cnt2 - cnt1) / cnt2);
  result = (prop > percentage);

  std::cout << "Saturation" << std::endl;

  cnt1 = count_histo(histo_s | mln::box1d(mln::point1d(0),mln::point1d(100)));

  cnt2= mln::geom::nsites(achromatic | (mln::pw::value(achromatic)==false));



  std::ofstream txt_stream("achromatism.txt");
  txt_stream << "Saturation" << std::endl;

  txt_stream << "Nbre pixels               : " << cnt2       << std::endl;
  txt_stream << "Nbre p faiblement saturés : " << cnt1       << std::endl;
  txt_stream << "Pourcentage               : " << prop       << std::endl;
  txt_stream << "Image faiblement saturé   : " << result     << std::endl;
  txt_stream << std::endl;

  txt_stream.flush();
  txt_stream.close();

  mln::io::pgm::save(view, "achromatism.pgm");
  mln::io::plot::save_image_sh(histo, "achromatism.sh");
  mln::io::pbm::save(mask, "achromatism.pbm");
}

/*
// COLOR
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00032c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00042c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00076c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00082c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00142c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00215c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00228c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00234c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00248c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00252c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00253c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00255c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00259c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00271c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00290c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00293c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00304c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00307c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00376c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00411c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00419c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00447c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00498c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00510c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00550c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00573c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00589c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00592c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00597c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00599c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00600c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00031c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00034c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00043c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00063c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00065c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00072c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00081c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00083c_20p.ppm");

// BLACK AND WHITE
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00329c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00036c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00037c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00039c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00040c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00049c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00055c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00057c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00068c_20p.ppm");


// A LITTLE BIT OF COLOR
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00262c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00263c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00311c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00319c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00440c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00608c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00630c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00631c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00028c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00046c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00073c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00089c_20p.ppm");
  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/ta00090c_20p.ppm");
*/

// To DO mettre le seuil d'achromaticité en paramètre
// TO DO inverser les couleurs sur les masques
int main()
{
  typedef mln::value::rgb8                                  t_rgb8;
  typedef mln::value::int_u8                                t_int_u8;
  typedef mln::value::hsv_f                                 t_hsvf;
  typedef mln::value::hsv_f::h_type                         t_hue;
  typedef mln::value::hsv_f::s_type                         t_sat;
  typedef mln::value::hsv_f::v_type                         t_val;
  typedef mln::image2d<t_hue>                               t_image2d_hue;
  typedef mln::image2d<t_sat>                               t_image2d_sat;
  typedef mln::image2d<t_val>                               t_image2d_val;
  typedef mln::image2d<t_hsvf>                              t_image2d_hsvf;
  typedef mln::image2d<t_rgb8>                              t_image2d_rgb8;
  typedef mln::image2d<float>                               t_image2d_float;
  typedef mln::image1d<unsigned>                            t_histo1d;
  typedef mln::image2d<t_int_u8>                            t_image2d_int_u8;
  typedef mln::image2d<bool>                                t_mask;
  typedef mln::fun::v2v::f_rgb_to_hsv_f_t                   t_rgb8_to_hsv;
  typedef mln::accu::math::count<t_hsvf>                    t_count;
  typedef mln::fun::v2v::component<t_hsvf,0>                t_component_h;
  typedef mln::fun::v2v::component<t_hsvf,1>                t_component_s;
  typedef mln::fun::v2v::component<t_hsvf,2>                t_component_v;
  typedef mln::fun::p2b::component_equals<t_image2d_hsvf,0> t_component_eq0;
  typedef mln::fun::p2b::component_equals<t_image2d_hsvf,1> t_component_eq1;
  typedef mln::fun::p2b::component_equals<t_image2d_hsvf,2> t_component_eq2;
  typedef mln::fun::p2b::achromatic<t_rgb8>                 t_achromatic;

  t_image2d_rgb8                                            input_rgb8;
  t_image2d_hsvf                                            input_hsvf;

  t_mask                                                    achromatic;
  t_mask                                                    low_saturation;

  t_image2d_float                                           achromatism1;
  t_image2d_int_u8                                          achromatism2;
  t_image2d_float                                           hue_concentration1;
  t_image2d_int_u8                                          hue_concentration2;

  t_image2d_hue                                             input_h;
  t_image2d_hue                                             input_h2;
  t_image2d_sat                                             input_s;
  t_image2d_val                                             input_v;

  t_image2d_int_u8                                          input_h8;
  t_image2d_int_u8                                          input_s8;
  t_image2d_int_u8                                          input_v8;

  t_histo1d                                                 histo_h;
  t_histo1d                                                 histo_s;
  t_histo1d                                                 histo_v;

  unsigned                                                  cnt1;
  unsigned                                                  cnt2;
  float                                                     percentage;
  bool                                                      result;



  // IMAGE LOADING PHASE
  std::cout << "Image loading phase ..." << std::endl;
//  mln::io::ppm::load(input_rgb8, ANNOTATING_1_BILL_IMG_PATH"/bill03.ppm");
//  mln::io::ppm::load(input_rgb8, ICDAR_20P_PPM_IMG_PATH"/mp00082c_20p.ppm");


  achromatism(input_rgb8,7,99.0);
  exit(-1);
  // REPERAGE DES PIXELS ACHROMATICS
  std::cout << "Init achromatic mask ..." << std::endl;
  initialize(achromatic, input_rgb8);
  mln::data::fill(achromatic, false);
  mln::data::fill((achromatic | t_achromatic(input_rgb8, 0.03)).rw(), true);

  mln::io::pbm::save(achromatic, "achromatic.pbm");

  std::cout << "Achieve canal forking ..." << std::endl;
  input_hsvf = mln::data::transform(input_rgb8, t_rgb8_to_hsv());

  input_h    = mln::data::transform(input_hsvf, t_component_h());
  input_s    = mln::data::transform(input_hsvf, t_component_s());
  input_v    = mln::data::transform(input_hsvf, t_component_v());

  // quid des achromatiques ???
  input_h8   = mln::data::stretch(t_int_u8(), input_h);
  input_s8   = mln::data::stretch(t_int_u8(), input_s);
  input_v8   = mln::data::stretch(t_int_u8(), input_v);

  // REPERAGE DES PIXELS ACHROMATICS
  std::cout << "Init low saturation mask ..." << std::endl;
  initialize(low_saturation, input_s8);
  mln::data::fill(low_saturation, false);
  mln::data::fill((low_saturation|(mln::pw::value(input_s8) <
				   mln::pw::cst(100u))).rw(), true);

  mln::io::pbm::save(low_saturation, "low_saturation.pbm");

  std::cout << "Compute histograms ..." << std::endl;
  histo_h    = mln::data::compute(mln::accu::meta::stat::histo1d(),
				  input_h8|(mln::pw::value(achromatic)==false));

  histo_s    = mln::data::compute(mln::accu::meta::stat::histo1d(),
				  input_s8|(mln::pw::value(achromatic)==false));

  histo_v    = mln::data::compute(mln::accu::meta::stat::histo1d(),
				  input_v8|(mln::pw::value(achromatic)==false));


  // etude des cartes

  hue_concentration1=mln::data::transform(input_h,
					  mln::fun::v2v::hue_concentration(histo_h));
  achromatism1=mln::data::transform(input_rgb8,mln::fun::v2v::achromatism());

  hue_concentration2= mln::data::stretch(t_int_u8(), hue_concentration1);
  achromatism2= mln::data::stretch(t_int_u8(), achromatism1);

  mln::io::pgm::save(achromatism2,       "achromatism_map.pgm");
  mln::io::pgm::save(hue_concentration2, "hue_concentration_map.pgm");
  mln::io::pgm::save(input_s8,           "saturation_map.pgm");

//  cnt1 = mln::data::compute(t_count(),
//			   (input_hsvf|t_component_eq0(input_hsvf,-1)).rw());


  // (I)   ACHROMATISME
  std::cout << "Achromatism" << std::endl;
  cnt1 = count_histo(histo_h);
  cnt2 = mln::geom::nsites(input_h);

  percentage = (100.0 * (cnt2 - cnt1) / cnt2);
  result     = percentage > 99.0;

  std::cout << "Nbre pixels               : " << cnt2       << std::endl;
  std::cout << "Nbre pixels achromatiques : " << (cnt2-cnt1)<< std::endl;
  std::cout << "Percentage                : " << percentage << std::endl;
  std::cout << "Image achromatique        : " << result     << std::endl;
  std::cout << std::endl;

  // (II)  FAIBLE SATURATION
  std::cout << "Saturation" << std::endl;

  cnt1 = count_histo(histo_s | mln::box1d(mln::point1d(0),mln::point1d(100)));

  cnt2= mln::geom::nsites(achromatic | (mln::pw::value(achromatic)==false));

  percentage = (100.0 * cnt1 / cnt2);
  result     = percentage > 95.0;

  std::cout << "Nbre pixels               : " << cnt2       << std::endl;
  std::cout << "Nbre p faiblement saturés : " << cnt1       << std::endl;
  std::cout << "Percentage                : " << percentage << std::endl;
  std::cout << "Image faiblement saturé   : " << result     << std::endl;
  std::cout << std::endl;

  // (III) DOMINANCE DE LA TEINTE
  // et peut être 50% des pixels faiblement saturées

  mln::debug::println(histo_h);
  unsigned peak = peak_histo(histo_h);

  cnt1 = count_histo(histo_h | mln::box1d(mln::point1d(peak-20),
					  mln::point1d(peak+20)));

  cnt2= count_histo(histo_h);

  percentage = (100.0 * cnt1 / cnt2);
  result     = percentage > 95.0;

  std::cout << "Position du pic           : " << peak       << std::endl;
  std::cout << "Nbre pixels               : " << cnt2       << std::endl;
  std::cout << "Nbre pixels proches pic   : " << cnt1       << std::endl;
  std::cout << "Percentage                : " << percentage << std::endl;
  std::cout << "Image fortement teintée   : " << result     << std::endl;
  std::cout << std::endl;



  // Autre possibilité
  // calculer le maximum de la teinte et regarder si le pourcentage pixels dont
  // la distance est inférieure à 20 > 95%
  // alors
}

// 3 cartes
// 1) carte d'achromaticité d = max(|r-g|,|r-b|,|g-b|)
// 2) carte de saturation
// 3) carte d'éloignement par rapport au pic de teinte


// QUELS SONT LES CHARACTERISTIQUES HSV DE LA BASE ICDAR ?
// FAIBLE SATURATION DES IMAGES ?
// DOMINANCE DES TEINTES ?
// ACHROMATISME ?
