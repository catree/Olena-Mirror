#include <mln/value/ops.hh>

#include <mln/value/concept/vectorial.hh>
#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

#include <mln/value/float01_8.hh>

#ifndef MLN_VALUE_YUV_HH
# define MLN_VALUE_YUV_HH

namespace mln
{
  namespace value
  {

    template <typename E>
    struct YUV
      :
      public internal::value_like_< algebra::vec< 3, float01_8 >, // Equivalent.
				    algebra::vec< 3, float01_8 >, // Encoding.
				    algebra::vec< 3, float01_8 >,      // Interoperation.
				    YUV<E> >
    {
    };

    template <typename Y, typename U, typename V>
    class yuv_ : public YUV< yuv_<Y,U,V> >
    {
    public:

      typedef Y y_type;
      typedef U u_type;
      typedef V v_type;

      /// Constructor without argument.
      yuv_()
      {
      }

      /// Constructor from component values.
      yuv_(const Y& y, const U& u, const V& v)
	: y_(y),
	  u_(u),
	  v_(v)
      {
      }

      /// Read-only access to the y component.
      const Y& y() const
      {
	return this->y_;
      }
      const U& u() const
      {
	return this->u_;
      }
      const V& v() const
      {
	return this->v_;
      }

      /// Read-write access to the y component.
      Y& y()
      {
	return this->y_;
      }
      U& u()
      {
	return this->u_;
      }
      V& v()
      {
	return this->v_;
      }

    private:
      Y y_;
      U u_;
      V v_;
    };

    typedef yuv_<float, float, float> yuv_f;

    typedef yuv_<double, double, double> yuv_d;

  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_YUV_HH
