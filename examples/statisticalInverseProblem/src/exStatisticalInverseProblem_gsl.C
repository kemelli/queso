/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------
 *
 * Copyright (C) 2008 The PECOS Development Team
 *
 * Please see http://pecos.ices.utexas.edu for more information.
 *
 * This file is part of the QUESO Library (Quantification of Uncertainty
 * for Estimation, Simulation and Optimization).
 *
 * QUESO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QUESO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QUESO. If not, see <http://www.gnu.org/licenses/>.
 *
 *--------------------------------------------------------------------------
 *
 * $Id: exStatisticalInverseProblem_gsl.C 17566 2011-02-12 18:21:56Z prudenci $
 *
 * Brief description of this file: 
 * 
 * This is an example of how to define and solve a statistical inverse problem 
 * using QUESO classes and algorithms. The code itself is in the templated
 * routine 'uqAppl(*env)'. This routine is called right after the initialization
 * of the MPI environment and of the QUESO environment and is available in
 * file 'exStatisticalInverseProblem_appl.h'
 *
 *--------------------------------------------------------------------------
 *-------------------------------------------------------------------------- */

#include <exStatisticalInverseProblem_appl.h>
#include <uqGslMatrix.h>

int main(int argc, char* argv[])
{
  //************************************************
  // Initialize environments
  //************************************************
#ifdef QUESO_HAS_MPI
  MPI_Init(&argc,&argv);
#endif

  UQ_FATAL_TEST_MACRO(argc != 2,
                      UQ_UNAVAILABLE_RANK,
                      "main()",
                      "input file must be specified in command line as argv[1], just after executable argv[0]");
#ifdef QUESO_HAS_MPI
  uqFullEnvironmentClass* env = new uqFullEnvironmentClass(MPI_COMM_WORLD,argv[1],"",NULL);
#else
  uqFullEnvironmentClass* env = new uqFullEnvironmentClass(0,argv[1],"",NULL);
#endif
  //std::cout << "proc " << env->fullRank() << ", HERE main 000" << std::endl;
  //env->fullComm().Barrier();
  //std::cout << "proc " << env->fullRank() << ", HERE main 001" << std::endl;

  //************************************************
  // Call application
  //************************************************
  uqAppl<uqGslVectorClass, // type for parameter vectors
         uqGslMatrixClass  // type for parameter matrices
        >(*env);

  //************************************************
  // Finalize environments
  //************************************************
  delete env;
#ifdef QUESO_HAS_MPI
  MPI_Finalize();
#endif

  return 0;
}
