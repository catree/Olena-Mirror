// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_TOPO_TARJAN_FLAT_ZONE_HH
# define OLENA_TOPO_TARJAN_FLAT_ZONE_HH

# include <oln/basics2d.hh>
# include <oln/topo/tarjan/union.hh>

# include <map>

namespace oln {

  namespace topo {

    namespace tarjan {

      /*! Create an image of label of the flat zones.
      **
      ** \todo FIXME: many assertions are missing.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/topo/tarjan/flat-zone.hh>
      ** #include <oln/convert/stretch.hh>
      **
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8> img_type;
      **   img_type in = oln::load(IMG_IN "test-cmap.pgm");
      **   oln::topo::tarjan::flat_zone<img_type> z(in);
      **   save(oln::convert::stretch_balance<ntg::int_u8>(z.label, 0, 255),
      **        IMG_OUT "oln_topo_flat_zone.pgm");
      **  }
      ** \endcode
      ** \image html test-cmap_pgm.png width=6cm
      ** \image latex test-cmap_pgm.png width=6cm
      ** =>
      ** \image html oln_topo_flat_zone.png width=6cm
      ** \image latex oln_topo_flat_zone.png width=6cm
      */
      template <class I>
      struct flat_zone
      {
	typedef oln_point_type(I) point_type;
	typedef oln_value_type(I)  data_type;
	typedef oln_concrete_type(I) image_type;

	typedef tarjan::tarjan_set<image_type, tarjan::empty_class> tarjan_cc;

	const image_type &input;
	tarjan_cc cc;
	/// output image.
	image2d<unsigned> label;

	std::vector<point_type> look_up_table;

	image2d< std::vector<oln::point2d> > ima_region;

	unsigned nlabels_;
	/*! Initialize the flat-zone with an image.
	**
	** \a doit is called.
	*/
	flat_zone(const image_type& input_) : input(input_), cc(input_),
					      label(input_.size()),
					      ima_region(input_.size()),
					      nlabels_(0)
	{
	  doit();
	}
	/// Compute the image of label.
	void
	doit()
	{
	  // Compute cc map
	  {
	    window2d c4;
	    c4.add(0,1).add(1,0);
	    typename image_type::bkd_iter_type p(input);
	    for_all(p)
	      {
		cc.make_set(p);
		window2d::neighb_type p_prime(c4, p);
		for_all (p_prime) if (input.hold(p_prime))
		  if (input[p] == input[p_prime])
		    cc.uni(p_prime.cur(), p);
	      }

	    cc.make_set(point_type(input.nrows(), input.ncols()));

	    // bottom-right corner -> bottom-left corner
	    for (int j = input.ncols() - 1; j >= 0; --j)
	      {
		cc.make_set(point_type(input.nrows(), j));
		cc.uni(point_type(input.nrows(), j + 1), point_type(input.nrows(), j));
	      }

	    // bottom-right corner -> top-right corner
	    for (int i = input.nrows() - 1; i >= 0; --i)
	      {
		cc.make_set(point_type(i, input.ncols()));
		cc.uni(point_type(i + 1, input.ncols()), point_type(i, input.ncols()));
	      }

	    // top-right corner -> top-left corner - 1
	    for (int j = input.ncols() - 1; j > 0; --j)
	      {
		cc.make_set(point_type(-1, j));
		cc.uni(point_type(-1, j + 1), point_type(-1, j));
	      }

	    // bottom-left corner -> top-left corner - 1
	    for (int i = input.nrows() - 1; i > 0; --i)
	      {
		cc.make_set(point_type(i, -1));
		cc.uni(point_type(i + 1, -1), point_type(i, -1));
	      }

	    cc.make_set(point_type(-1, -1));

	    cc.uni(point_type(-1, 0), point_type(-1, -1));
	    cc.uni(point_type(0, -1), point_type(-1, -1));
	  }

	  // Compute label map
	  {
	    typename image_type::fwd_iter_type p(input);

	    // Push_back because no label = 0;
	    look_up_table.push_back(point_type());

	    label.border_adapt_assign(1, ++nlabels_);

	    look_up_table.push_back(cc.find_root(point_type(-1, -1)));

	    for_all(p)
	      {
		if (cc.is_root(p))
		  {
		    label[p] = ++nlabels_;
		    look_up_table.push_back(p);
		  }
		else
		  label[p] = label[cc.parent[p]];
	      }

	    {
	      typename image_type::fwd_iter_type p(input);
	      for_all(p)
		{
		  if (cc.is_root(p))
		    ima_region[p].push_back(p);
		  else
		    ima_region[cc.parent[p]].push_back(p);
		}
	    }

	  }
	}
	/// Get the label of a point \a p.
	const unsigned
	get_label(const point_type & p) const
	{
	  return label[p];
	}
	/// Get the root point of a label \a l.
	const point_type&
	get_root(unsigned l) const
	{
	  return look_up_table[l];
	}
	/// Number of label.
	const unsigned
	nlabels() const
	{
	  return nlabels_;
	}

	/*! Merge two flat zone.
	**
	** \note FIMXE: should be protected, shouldn't it?
	*/
	void
	merge(const int l1, const int l2)
	{
	  point_type root_l1 = look_up_table[l1];
	  point_type root_l2 = look_up_table[l2];
	  assertion(cc.is_root(root_l1));
	  assertion(cc.is_root(root_l2));
	  // merge
	  cc.uni(root_l2, root_l1);
	  // update our tables
	  look_up_table[l2] = root_l1;
	  for (typename std::vector<point_type>::iterator
		 i = ima_region[root_l2].begin();
	       i != ima_region[root_l2].end(); ++i)
            label[*i] = l1;
	  ima_region[root_l1].insert(ima_region[root_l1].end(),
				     ima_region[root_l1].begin(),
				     ima_region[root_l1].end());
	  ima_region[root_l2].clear();

	  --nlabels_;
	}

      };

    } // end of namespace tarjan

  } // end of namespace topo

} // end of namespace oln

#endif // ! OLENA_TOPO_TARJAN_FLAT_ZONE_HH
