image2d<int> ima3 = clone(ima1); // Makes a deep copy.

ima3(p) = 3;

// prints "3 - 2"
std::cout << ima3(p) << " - " << ima1(p) << std::endl;
// prints "false"
std::cout << (ima3.id() == ima1.id()) << std::endl;