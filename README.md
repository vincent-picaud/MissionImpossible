
# Table of Contents

1.  [Compilation](#orgf6b59f3)
    1.  [Release](#orge2ff3cb)
2.  [Examples](#org29e9e2a)
    1.  [Hessian action \(H.v\)](#org60501dd)

Check:


<a id="orgf6b59f3"></a>

# Compilation


<a id="orge2ff3cb"></a>

## Release

    meson --buildtype=release -Db_ndebug=on build-release


<a id="org29e9e2a"></a>

# Examples


<a id="org60501dd"></a>

## Hessian action \(H.v\)

Observe that
\[
H.v=\nabla_X \langle v,\nabla_X f \rangle
\]

\[
\nabla_X v . \nabla_X f = \nabla_X  \langle v, \sum_j \partial_j f \rangle =\sum_{ij} v_i \partial_{ij}f = H.v
\]

The associated code is

    #include "AutoDiffCpp/ad.hpp"
    #include "AutoDiffCpp/derivatives.hpp"
    
    #include <gtest/gtest.h>
    #include <vector>
    
    using namespace AutoDiffCpp;
    
    // Idea H.v = ∇_w(v.∇_w f)
    // See: http://www.bcl.hamilton.ie/~barak/papers/nc-hessian.pdf
    TEST(Nested, Hv)
    {
      AD<AD<double>> x0(3), x1(4), y;
      y = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);
    
      assert(y == 254);
    
      auto y_gradient = Jacobian_row(y);  // Compute ∇_w f
    
      assert(y_gradient[x0] == 604);
      assert(y_gradient[x1] == -100);
    
      AD<double> v0(5), v1(6), z;
      z = v0 * y_gradient[x0] + v1 * y_gradient[x1];  // Compute v.∇_w f
    
      auto z_gradient = Jacobian_row(z);  // Compute ∇_w(v.∇_w f) = H.v
    
      assert(z_gradient[x0] == 3890);
      assert(z_gradient[x1] == -480);
    }

    #include "AutoDiffCpp/ad.hpp"
    #include "AutoDiffCpp/derivatives.hpp"
    
    #include <gtest/gtest.h>
    #include <vector>
    
    using namespace AutoDiffCpp;
    
    // Idea H.v = ∇_w(v.∇_w f)
    // See: http://www.bcl.hamilton.ie/~barak/papers/nc-hessian.pdf
    TEST(Nested, Hv)
    {
      AD<AD<double>> x0(3), x1(4), y;
      y = (1 - x0) * (1 - x0) + 10 * (x1 - x0 * x0) * (x1 - x0 * x0);
    
      assert(y == 254);
    
      auto y_gradient = Jacobian_row(y);  // Compute ∇_w f
    
      assert(y_gradient[x0] == 604);
      assert(y_gradient[x1] == -100);
    
      AD<double> v0(5), v1(6), z;
      z = v0 * y_gradient[x0] + v1 * y_gradient[x1];  // Compute v.∇_w f
    
      auto z_gradient = Jacobian_row(z);  // Compute ∇_w(v.∇_w f) = H.v
    
      assert(z_gradient[x0] == 3890);
      assert(z_gradient[x1] == -480);
    }

