// ---------------------------------------------------------------------
//
// Copyright (C) 2004 - 2016 by the deal.II authors
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



// check PETScWrappers::MPI::Vector::linfty_norm()

#include "../tests.h"
#include <deal.II/lac/petsc_parallel_vector.h>
#include <fstream>
#include <iostream>
#include <vector>


void test (PETScWrappers::MPI::Vector &v)
{
  // set some elements of the vector
  double norm = 0;
  for (unsigned int k=0; k<v.size(); k+=1+k)
    {
      v(k) = std::complex<double> (k,2.*k);

      // Ugly, but equal.
      norm = std::max (norm,std::fabs (std::sqrt ((k*k + 2.*k*2.*k))));
    }

  v.compress (VectorOperation::insert);

  // then check the norm
  AssertThrow (v.linfty_norm() == norm, ExcInternalError());

  deallog << "OK" << std::endl;
}

int main (int argc,char **argv)
{
  initlog();
  deallog.depth_console(0);
  deallog.threshold_double(1.e-10);

  try
    {
      Utilities::MPI::MPI_InitFinalize mpi_initialization(argc, argv, 1);
      {
        PETScWrappers::MPI::Vector v (MPI_COMM_WORLD, 100, 100);
        test (v);
      }

    }
  catch (std::exception &exc)
    {
      std::cerr << std::endl << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Exception on processing: " << std::endl
                << exc.what() << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;

      return 1;
    }
  catch (...)
    {
      std::cerr << std::endl << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Unknown exception!" << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;
      return 1;
    };
}
