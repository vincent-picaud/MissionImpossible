#include <vector>

// BAD
template <typename T>
void
scale_v1(const T scalar, std::vector<T>& v)
{
  // version 1
}

// GOOD
template <typename T>
void
scale_v2(const typename std::vector<T>::value_type scalar, std::vector<T>& v)
{
  // version 2
}

int
main()
{
  std::vector<double> v(10);

  scale_v1(2, v);  // <- does not compile
                   // "...deduced conflicting types for parameter ‘T’ (‘int’ and ‘double’)..."
  
  scale_v2(2, v);  // <- OK
}
