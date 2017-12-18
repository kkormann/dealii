// ---------------------------------------------------------------------
//
// Copyright (C) 2017 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------


// test LAPACKFullMatrix::add() for rank1 downdate of a Cholesky factorization

/* MWE in Octave:
A = pascal(4)
A =

    1    1    1    1
    1    2    3    4
    1    3    6   10
    1    4   10   20

x = [3 2 1 5]';

R = chol(A)
R =

   1   1   1   1
   0   1   2   3
   0   0   1   3
   0   0   0   1

R1 = cholupdate(R,x)

R1 =

   3.16228   2.21359   1.26491   5.05964
   0.00000   1.04881   2.09762   2.66970
   0.00000   0.00000   1.00000   3.00000
   0.00000   0.00000   0.00000   1.80907

x = x / 2

R2 = cholupdate(R1,x,'-')

R2 =

   2.78388   1.97566   1.16743   4.40033
   0.00000   1.04727   2.09454   2.67978
   0.00000   0.00000   1.00000   3.00000
   0.00000   0.00000   0.00000   1.79050

*/

#include "../tests.h"
#include "create_matrix.h"
#include <deal.II/lac/lapack_full_matrix.h>
#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/vector.h>

#include <iostream>


template <typename NumberType>
void
test()
{
  const unsigned int size = 4;
  LAPACKFullMatrix<NumberType> A(size);
  A.set_property(LAPACKSupport::symmetric);
  Vector<NumberType> v(size);

  A(0,0) = 1;
  A(0,1) = 1;
  A(0,2) = 1;
  A(0,3) = 1;
  A(1,0) = 1;
  A(1,1) = 2;
  A(1,2) = 3;
  A(1,3) = 4;
  A(2,0) = 1;
  A(2,1) = 3;
  A(2,2) = 6;
  A(2,3) = 10;
  A(3,0) = 1;
  A(3,1) = 4;
  A(3,2) =10;
  A(3,3) = 20;

  v(0) = 3;
  v(1) = 2;
  v(2) = 1;
  v(3) = 5;

  const NumberType a = 1.;

  A.compute_cholesky_factorization();
  deallog << "Cholesky:" << std::endl;
  A.print_formatted(deallog.get_file_stream(),5);

  A.add(a,v);

  deallog << "Cholesky updated:" << std::endl;
  A.print_formatted(deallog.get_file_stream(),5);

  v /= 2.;

  A.add(-1.,v);

  deallog << "Cholesky downdated:" << std::endl;
  A.print_formatted(deallog.get_file_stream(),5);

}


int main()
{
  const std::string logname = "output";
  std::ofstream logfile(logname.c_str());
  logfile.precision(5);
  deallog.attach(logfile);

  test<double>();
}
