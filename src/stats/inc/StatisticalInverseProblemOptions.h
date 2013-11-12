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
// $Id$
//
//--------------------------------------------------------------------------

#ifndef UQ_SIP_OPTIONS_H
#define UQ_SIP_OPTIONS_H

#include <queso/Environment.h>
//#include <queso/MetropolisHastingsSGOptions.h>

#undef UQ_SIP_READS_SOLVER_OPTION

#define UQ_SIP_FILENAME_FOR_NO_FILE "."

// _ODV = option default value
#define UQ_SIP_COMPUTE_SOLUTION_ODV        1
#define UQ_SIP_DATA_OUTPUT_FILE_NAME_ODV   UQ_SIP_FILENAME_FOR_NO_FILE
#define UQ_SIP_DATA_OUTPUT_ALLOWED_SET_ODV ""
#ifdef UQ_SIP_READS_SOLVER_OPTION
#define UQ_SIP_SOLVER_ODV                  "bayes_mc" // Bayesian formula + Metropolis-Hastings
#endif

namespace QUESO {

/*! \file uqStatisticalInverseProblemOptions.h
    \brief Classes to allow options to be passed to a Statistical Inverse Problem.
*/

/*! \class SipOptionsValues
 *  \brief This class provides options for a Statistical Inverse Problem if no input file is available.
 * 
 *  In order to solve a Statistical Inverse Problem (SIP), QUESO expects some options for its methods to be 
 * fully defined. This class provides default values for such options if no input file is available. */

class SipOptionsValues
{
public:
  //! @name Constructor/Destructor methods
  //@{
  //! Default constructor.
  /*! Assigns the default suite of options to the Statistical Inverse Problem.*/
  SipOptionsValues            ();
  
  //! Copy constructor.
  /*! It assigns the same options values from  \c src to \c this.*/
  SipOptionsValues            (const SipOptionsValues& src);
  
  //! Destructor
  ~SipOptionsValues            (); 
  //@}
  
  //! @name Set methods
  //@{ 
  //! Assignment operator; it copies \c rhs to \c this. 
  SipOptionsValues& operator= (const SipOptionsValues& rhs);
  //@}

  bool                   m_computeSolution;
  std::string            m_dataOutputFileName;
  std::set<unsigned int> m_dataOutputAllowedSet;
#ifdef UQ_SIP_READS_SOLVER_OPTION
  std::string            m_solverString;
#endif

  //MhOptionsValues m_mhOptionsValues;

private:
  //! Copies the option values from \c src to \c this.
  void copy(const SipOptionsValues& src);
};

// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------

/*! \class StatisticalInverseProblemOptions
 *  \brief This class reads option values for a Statistical Inverse Problem from an input file.
 * 
 *  This class reads the option values for the Statistical Inverse Problem (SIP) from an input file 
 * provided by the user. The class expects the prefix '\<prefix\>_ip_'. For instance, if 'prefix' 
 * is 'foo_775_', then the constructor will read all options that begin with 'foo_775_ip_'. If the 
 * options request data to be written in the output file (MATLAB .m format only, for now), the user
 * can run 'grep zeros \<OUTPUT FILE NAME\>' after the solution procedure ends in order to check 
 * which MATLAB variables are defined and set. The names of the variables are self explanatory.*/

class StatisticalInverseProblemOptions
{
public:
  //! @name Constructor/Destructor methods
  //@{ 
  //! Constructor: reads options from the input file.
  StatisticalInverseProblemOptions(const BaseEnvironment& env, const char* prefix);
  
  //! Constructor: with alternative option values.
  /*! In this constructor, the input options are given by \c alternativeOptionsValues, rather than the 
   * options input file*/
  StatisticalInverseProblemOptions(const BaseEnvironment& env, const char* prefix, const SipOptionsValues& alternativeOptionsValues);
 
  //! Destructor
  ~StatisticalInverseProblemOptions();
  //@}
  
  //! @name I/O methods
  //@{
  //! It scans the option values from the options input file.
  void scanOptionsValues();  
  
  //!  It prints the option values.
  void print            (std::ostream& os) const;
  //@}
  
  SipOptionsValues       m_ov;
  std::string                   m_prefix;

private:
  //! Define my SIP options as the default options.
  void   defineMyOptions  (po::options_description& optionsDesc) const;
  
  //! Gets the option values of the SIP.
  void   getMyOptionValues(po::options_description& optionsDesc);

  const BaseEnvironment& m_env;

  po::options_description*      m_optionsDesc;
  std::string                   m_option_help;
  std::string                   m_option_computeSolution;
  std::string                   m_option_dataOutputFileName;
  std::string                   m_option_dataOutputAllowedSet;
#ifdef UQ_SIP_READS_SOLVER_OPTION
  std::string                   m_option_solver;
#endif
};

//! Prints the object \c obj, overloading an operator.
std::ostream& operator<<(std::ostream& os, const StatisticalInverseProblemOptions& obj);

}  // End namespace QUESO

#endif // UQ_SIP_OPTIONS_H
