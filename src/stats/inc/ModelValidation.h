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

#ifndef UQ_MODEL_VALIDATION_H
#define UQ_MODEL_VALIDATION_H

#include <queso/ValidationCycle.h>

namespace QUESO {

/*! \file uqModelValidation.h
 * \brief A templated class for model validation of the example validationPyramid.
 *
 * \class ModelValidation
 * \brief A templated class for model validation of the example validationPyramid. 
 * 
 * Its derived class exPhysics1Validation enables comparison between the calibration
 * and validate stages. */

template <class P_V,class P_M,class Q_V,class Q_M>
class ModelValidation
{
public:
  //! @name Constructor/Destructor methods
  //@{
  //! Constructor.
  ModelValidation(const BaseEnvironment& env,
                         const char*                   prefix);
  
  //! Virtual destructor.
  virtual ~ModelValidation();
  //@}
  
  //! @name Misc methods
  //@{
  //! Runs calibration, validation and comparison stages. See template specialization.
  virtual void run() = 0;

  //! Access to the environment variable (m_env).
  const BaseEnvironment&                  env  () const;
  
  //! Access to the cycle (m_cycle).
  const ValidationCycle<P_V,P_M,Q_V,Q_M>& cycle() const;
  //@}
  
protected:
  const BaseEnvironment& m_env;
        std::string             m_prefix;

  ValidationCycle<P_V,P_M,Q_V,Q_M>* m_cycle;
};
// Constructor -------------------------------------
template <class P_V,class P_M,class Q_V,class Q_M>
ModelValidation<P_V,P_M,Q_V,Q_M>::ModelValidation(
  const BaseEnvironment& env,
  const char*                   prefix)
  :
  m_env   (env),
  m_prefix((std::string)(prefix) + ""),
  m_cycle (NULL)
{
  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "Entering ModelValidation<P_V,P_M,Q_V,Q_M>::constructor()"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }

  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "Leaving ModelValidation<P_V,P_M,Q_V,Q_M>::constructor()"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }

  return;
}
// Destructor---------------------------------------
template <class P_V,class P_M,class Q_V,class Q_M>
ModelValidation<P_V,P_M,Q_V,Q_M>::~ModelValidation()
{
  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "Entering ModeValidation::destructor()"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }

  if (m_cycle) delete m_cycle;

  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "Leaving ModeValidation::destructor()"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }
}
// Misc methods-------------------------------------
template <class P_V,class P_M,class Q_V,class Q_M>
const BaseEnvironment&
ModelValidation<P_V,P_M,Q_V,Q_M>::env() const
{
  return m_env;
}
//--------------------------------------------------
template <class P_V,class P_M,class Q_V,class Q_M>
const ValidationCycle<P_V,P_M,Q_V,Q_M>&
ModelValidation<P_V,P_M,Q_V,Q_M>::cycle() const
{
  return *m_cycle;
}

}  // End namespace QUESO

#endif // UQ_MODEL_VALIDATION_H
