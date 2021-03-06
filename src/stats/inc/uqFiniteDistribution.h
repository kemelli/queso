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

#ifndef __UQ_FINITE_DISTRIBUTION_H__
#define __UQ_FINITE_DISTRIBUTION_H__

#include <uqEnvironment.h>

/*! \file uqFiniteDistribution.h
 * \brief A templated class for a finite distribution.
 *
 * \class uqFiniteDistributionClass
 * \brief A templated class for a finite distribution.
 * 
 * Unordered, discrete distribution, whose weights must be nonnegative, and are treated as unnormalized 
 * probabilities.\n
 * 
 * TODO: Describe me better!*/

class uqFiniteDistributionClass {
public:
  //! @name Constructor/Destructor methods
  //@{
  //! Constructor.
  uqFiniteDistributionClass(const uqBaseEnvironmentClass& env,
                            const char*                   prefix,
                            const std::vector<double>&    inpWeights);
  //! Virtual destructor
  virtual ~uqFiniteDistributionClass();
  //@}
  
  //! @name Misc methods
  //@{
  //! Environment; access to protected attribute m_env.  
  const uqBaseEnvironmentClass& env    () const;
  //@}
  
  //! @name Statistical methods
  //@{
  //! Weights.
  const std::vector<double>&    weights() const;
  
  //! Samples.
  unsigned int            sample () const;
 //@}
  
protected:
  const uqBaseEnvironmentClass& m_env;
        std::string             m_prefix;
	std::vector<double>     m_weights;

	std::map<double,unsigned int> m_map;
};
#endif // __UQ_FINITE_DISTRIBUTION_H__
