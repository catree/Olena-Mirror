#include <mln/core/image/image2d.hh>
#include <mln/core/alias/box2d.hh>
int main()
{
  box2d b(2,3);
  image2d<int> ima(b);
  
  // On image2d, Site <=> point2d
  point2d p(1, 2);

  // Associate '9' as value for the site/point2d (1,2).
  // The value is returned by reference and can be changed.
  ima.at(1,2) = 9;
  std::cout << "ima.at(1,2) = " << ima.at(1,2)
	    << std::endl;
  std::cout << "ima(p) = " << ima(p) << std::endl;

  std::cout << "---" << std::endl;


  // Associate '2' as value for the site/point2d (1,2).
  // The value is returned by reference
  // and can be changed as well.
  ima(p) = 2; 
  std::cout << "ima.at(1,2) = " << ima.at(1,2)
	    << std::endl;
  std::cout << "ima(p) = " << ima(p) << std::endl;
}