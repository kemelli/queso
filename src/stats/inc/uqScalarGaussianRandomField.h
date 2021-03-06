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

#ifndef __UQ_SCALAR_GAUSSIAN_RANDOM_FIELD_H__
#define __UQ_SCALAR_GAUSSIAN_RANDOM_FIELD_H__

#include <uqScalarCovarianceFunction.h>
#include <uqScalarFunction.h>
#include <uqVectorRV.h>

/*! 
 * \file uqScalarGaussianRandomField.h
 * \brief A class for handling Gaussian random fields (GRF).
 * 
 * \class uqScalarGaussianRandomFieldClass
 * \brief A class for handling scalar Gaussian random fields (GRF).
 *
 * This class implements a scalar Gaussian random field (GRF); i.e. a random field involving 
 * Gaussian probability density functions (PDFs) of the variables. A one-dimensional GRF is 
 * also called a Gaussian process.*/

template <class V, class M>
class uqScalarGaussianRandomFieldClass
{
 public:
  //! @name Constructor/Destructor methods
  //@{
  //! Constructor.
  /*! Constructs a new object, given a prefix, an index set, and both a mean and a 
   * covariance function. This method deletes the previous saved positions. */ 
  uqScalarGaussianRandomFieldClass(const char*                                     prefix,
                                   const uqVectorSetClass<V,M>&                    indexSet,
                                   const uqBaseScalarFunctionClass<V,M>&           meanFunction, 
                                   const uqBaseScalarCovarianceFunctionClass<V,M>& covarianceFunction);
  
  //! TODO: Copy constructor.
  /*! \todo: implement me!*/
  uqScalarGaussianRandomFieldClass(const uqScalarGaussianRandomFieldClass&         obj);
  
  //! Destructor. 
  ~uqScalarGaussianRandomFieldClass();
  //@}
  
  //! @name Set methods
  //@{
  //! TODO: Assignment operator; it copies \c rhs to \c this.
  /*! \todo: implement me!*/
  uqScalarGaussianRandomFieldClass& operator=(const uqScalarGaussianRandomFieldClass& rhs);
  //@}

  //! @name Math methods
  //@{
  //! Index set; access to protected attribute m_indexSet.
  const uqVectorSetClass<V,M>&           indexSet          () const;
  
  //! Mean function; access to protected attribute m_meanFunction.
  const uqBaseScalarFunctionClass<V,M>&  meanFunction      () const;
  
  //! Covariance function; access to protected attribute m_covarianceFunction.
  const uqBaseScalarCovarianceFunctionClass<V,M>& covarianceFunction() const;
  
  //! Function that samples from a Gaussian PDF.
  /*! Given the field positions, this method performs a number of tests, calculates the mean vector,
   * the covariance matrix and then it samples from a Gaussian random vector as 
   * many positions as required.*/
  void                                  sampleFunction(const std::vector<V*>& fieldPositions, V& sampleValues);
  //@}
  
protected:
  //! Copy method.
  void                                  copy          (const uqScalarGaussianRandomFieldClass& src);

  //! Environment.
  const uqBaseEnvironmentClass&         m_env;
  
  //! Prefix.
  std::string                           m_prefix;
	
  //! Index set.	
  const uqVectorSetClass<V,M>&          m_indexSet;
  
  //! Mean function.
  const uqBaseScalarFunctionClass<V,M>& m_meanFunction;
  
  //! Covariance function.
  const uqBaseScalarCovarianceFunctionClass<V,M>& m_covarianceFunction;

  //! Saved positions.
  std::vector<V*>                       m_savedPositions;
  
  //! Image set of the RV.
  uqVectorSpaceClass<V,M>*              m_savedRvImageSpace;
  
  //! Vector of the mean value of the RV.
  V*                                    m_savedRvLawExpVector;
  
  //! Covariance matrix of the RV.
  M*                                    m_savedRvLawCovMatrix;
  
  //! My RV.
  uqGaussianVectorRVClass<V,M>*         m_savedRv;
};

// Default constructor -----------------------------
template <class V, class M>
uqScalarGaussianRandomFieldClass<V,M>::uqScalarGaussianRandomFieldClass(
  const char*                               prefix,
  const uqVectorSetClass<V,M>&              indexSet,
  const uqBaseScalarFunctionClass<V,M>&     meanFunction, 
  const uqBaseScalarCovarianceFunctionClass<V,M>& covarianceFunction)
  :
  m_env                (indexSet.env()),
  m_prefix             ((std::string)(prefix)+"grf_"),
  m_indexSet           (indexSet),
  m_meanFunction       (meanFunction),
  m_covarianceFunction (covarianceFunction),
  m_savedRvImageSpace  (NULL),
  m_savedRvLawExpVector(NULL),
  m_savedRvLawCovMatrix(NULL),
  m_savedRv            (NULL)
{
  m_savedPositions.clear();
}
// Destructor ---------------------------------------
template <class V, class M>
uqScalarGaussianRandomFieldClass<V,M>::~uqScalarGaussianRandomFieldClass()
{
}
// Math methods -------------------------------------
template <class V, class M>
const uqVectorSetClass<V,M>&
uqScalarGaussianRandomFieldClass<V,M>::indexSet() const
{
  return m_indexSet;
}
// --------------------------------------------------
template <class V, class M>
const uqBaseScalarFunctionClass<V,M>&
uqScalarGaussianRandomFieldClass<V,M>::meanFunction() const
{
  return m_meanFunction;
}
// --------------------------------------------------
template <class V, class M>
const uqBaseScalarCovarianceFunctionClass<V,M>&
uqScalarGaussianRandomFieldClass<V,M>::covarianceFunction() const
{
  return m_covarianceFunction;
}
// --------------------------------------------------
template <class V, class M>
void
uqScalarGaussianRandomFieldClass<V,M>::sampleFunction(const std::vector<V*>& fieldPositions, V& sampleValues)
{
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 99)) {
    *m_env.subDisplayFile() << "Entering uqScalarGaussianRandomFieldClass<V,M>::sampleFunction()"
                            << std::endl;
  }

  UQ_FATAL_TEST_MACRO(fieldPositions.size() != sampleValues.sizeLocal(),
                      m_env.fullRank(),
                      "uqScalarGaussianRandomFieldClass<V,M>::sampleFunction()",
                      "invalid input data");

  if ((m_savedPositions.size() == 0   ) &&
      (m_savedRvImageSpace     == NULL) &&
      (m_savedRvLawExpVector   == NULL) &&
      (m_savedRvLawCovMatrix   == NULL) &&
      (m_savedRv               == NULL)) {
    // Ok
  }
  else if ((m_savedPositions.size() != 0   ) &&
           (m_savedRvImageSpace     != NULL) &&
           (m_savedRvLawExpVector   != NULL) &&
           (m_savedRvLawCovMatrix   != NULL) &&
           (m_savedRv               != NULL)) {
    // Ok
  }
  else {
    UQ_FATAL_TEST_MACRO(true,
                        m_env.fullRank(),
                        "uqScalarGaussianRandomFieldClass<V,M>::sampleFunction()",
                        "invalid combination of pointer values");
  }

  unsigned int numberOfPositions = fieldPositions.size();
  bool instantiate = true;
  if (m_savedPositions.size() == numberOfPositions) {
    bool allPositionsAreEqual = true;
    for (unsigned int i = 0; i < numberOfPositions; ++i) {
      UQ_FATAL_TEST_MACRO(m_savedPositions[i] == NULL,
                          m_env.fullRank(),
                          "uqScalarGaussianRandomFieldClass<V,M>::sampleFunction()",
                          "m_savedPositions[i] should not be NULL");
      if ((m_savedPositions[i]->sizeLocal() == fieldPositions[i]->sizeLocal()) &&
          (*(m_savedPositions[i])           == *(fieldPositions[i])          )) {
        // Ok
      }
      else {
        allPositionsAreEqual = false;
        break;
      }
    } // for i
    instantiate = !allPositionsAreEqual;
  }

  if (instantiate) {
    delete m_savedRv;
    delete m_savedRvLawCovMatrix;
    delete m_savedRvLawExpVector;
    delete m_savedRvImageSpace;
    for (unsigned int i = 0; i < m_savedPositions.size(); ++i) {
      delete m_savedPositions[i];
    }
    m_savedPositions.clear();

    // Set m_savedPositions
    m_savedPositions.resize(numberOfPositions,NULL);
    for (unsigned int i = 0; i < m_savedPositions.size(); ++i) {
      m_savedPositions[i] = new V(*(fieldPositions[i]));
    }

    // Set m_savedRvImageSpace
    m_savedRvImageSpace = new uqVectorSpaceClass<V,M>(m_env, "grf_", numberOfPositions, NULL);

    // Set m_savedRvLawExpVector
    m_savedRvLawExpVector = new V(m_savedRvImageSpace->zeroVector());
    for (unsigned int i = 0; i < numberOfPositions; ++i) {
      (*m_savedRvLawExpVector)[i] = m_meanFunction.actualValue(*(fieldPositions[i]),NULL,NULL,NULL,NULL);
    }

    // Set m_savedRvLawCovMatrix
    m_savedRvLawCovMatrix = new M(m_savedRvImageSpace->zeroVector());
    for (unsigned int i = 0; i < numberOfPositions; ++i) {
      for (unsigned int j = 0; j < numberOfPositions; ++j) {
        (*m_savedRvLawCovMatrix)(i,j) = m_covarianceFunction.value(*(fieldPositions[i]),*(fieldPositions[j]));
        if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 3)) {
          *m_env.subDisplayFile() << "In uqScalarGaussianRandomFieldClass<V,M>::sampleFunction()"
                                  << ": i = " << i
                                  << ", j = " << j
                                  << ", *(fieldPositions[i]) = " << *(fieldPositions[i])
                                  << ", *(fieldPositions[j]) = " << *(fieldPositions[j])
                                  << ", (*m_savedRvLawCovMatrix)(i,j) = " << (*m_savedRvLawCovMatrix)(i,j)
                                  << std::endl;
        }
      }
    }

    // Set m_savedRv
    m_savedRv = new uqGaussianVectorRVClass<V,M>("grf_",
                                                 *m_savedRvImageSpace,
                                                 *m_savedRvLawExpVector,
                                                 *m_savedRvLawCovMatrix);

    if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 3)) {
      *m_env.subDisplayFile() << "In uqScalarGaussianRandomFieldClass<V,M>::sampleFunction()"
                              << ": just instantiated Gaussian RV"
                              << "\n *m_savedRvLawExpVector = " << *m_savedRvLawExpVector
                              << "\n *m_savedRvLawCovMatrix = " << *m_savedRvLawCovMatrix
                              << std::endl;
      for (unsigned int i = 0; i < numberOfPositions; ++i) {
        *m_env.subDisplayFile() << " *(m_savedPositions[" << i
                                << "]) = "                << *(m_savedPositions[i])
                                << std::endl;
      }
    }
  } // if (instantiate)

  // Generate sample function
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 99)) {
    *m_env.subDisplayFile() << "In uqScalarGaussianRandomFieldClass<V,M>::sampleFunction()"
                            << ": about to realize sample values"
                            << std::endl;
  }
  m_savedRv->realizer().realization(sampleValues);
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 99)) {
    *m_env.subDisplayFile() << "In uqScalarGaussianRandomFieldClass<V,M>::sampleFunction()"
                            << ": just realized sample values"
                            << std::endl;
  }

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 99)) {
    *m_env.subDisplayFile() << "Leaving uqScalarGaussianRandomFieldClass<V,M>::sampleFunction()"
                            << std::endl;
  }

  return;
}

#endif // __UQ_SCALAR_GAUSSIAN_RANDOM_FIELD_H__ 
