//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// QUESO - a library to support the Quantification of Uncertainty
// for Estimation, Simulation and Optimization
//
// Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor, 
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
// 
// $Id:$
//
//--------------------------------------------------------------------------

#include <uqMLSampling1.h>

#ifdef QUESO_HAS_GLPK
void BIP_routine(glp_tree *tree, void *info)
{
  const uqBaseEnvironmentClass& env = *(((BIP_routine_struct *) info)->env);
  unsigned int currLevel            =   ((BIP_routine_struct *) info)->currLevel;

  int reason = glp_ios_reason(tree);

  if ((env.subDisplayFile()) && (env.displayVerbosity() >= 1)) {
    *env.subDisplayFile() << "In BIP_routine()"
                          << ", level " << currLevel+LEVEL_REF_ID
                          << ": glp_ios_reason() = " << reason
                          << std::endl;
  }
  std::cout << "In BIP_routine: reason = " << reason << std::endl;

  switch (reason) {
    case GLP_IROWGEN: // 0x01  /* request for row generation       */
      sleep(1);  
    break;

    case GLP_IBINGO:  // 0x02  /* better integer solution found    */
      sleep(1);  
    break;

    case GLP_IHEUR:   // 0x03  /* request for heuristic solution   */
      // Do nothing
    break;

    case GLP_ICUTGEN: // 0x04  /* request for cut generation       */
      // Do nothing
    break;

    case GLP_IBRANCH: // 0x05  /* request for branching            */
      // Do nothing
    break;

    case GLP_ISELECT: // 0x06  /* request for subproblem selection */
      // Do nothing
    break;

    case GLP_IPREPRO: // 0x07  /* request for preprocessing        */
      // Do nothing
    break;

    default:
      UQ_FATAL_TEST_MACRO(true,
                          env.worldRank(),
                          "BIP_routine()",
                          "invalid glp_ios_readon");
    break;
  }

  return;
}
#endif // QUESO_HAS_GLPK
