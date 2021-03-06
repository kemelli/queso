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

#ifndef __UQ_TRANSITION_KERNEL_GROUP_H__
#define __UQ_TRANSITION_KERNEL_GROUP_H__

#include <uqVectorRV.h>
#include <uqScalarFunctionSynchronizer.h>


//*****************************************************
// Base class
//*****************************************************
/*! \file uqTKGroup.h
 *  \brief Class to set up a Transition Kernel.
 * 
 *  \class uqBaseTKGroupClass
 *  \brief This base class allows the representation of a transition kernel.
 * 
 * The transition kernel is a conditional distribution function that represents the probability of 
 * moving from a point \f$ x \in R^n \f$ to a point in the set \f$ A \in B \f$, where \f$ B \f$ is 
 * a Borel set over \f$ R^n \f$. Since it is a distribution function, \f$ P(x,R^n)=1 \f$, where it 
 * is permitted that the chain can make a transition from the point \f$ x \f$ to \f$ x \f$, that is 
 * \f$ P(x, {x}) \f$ is not necessarily zero. */
template<class V, class M>
class uqBaseTKGroupClass {
public:
  //! @name Constructor/Destructor methods
  //@{ 
  //! Default constructor.
  uqBaseTKGroupClass();
  
  //! Constructor.
  uqBaseTKGroupClass(const char*                    prefix,
                     const uqVectorSpaceClass<V,M>& vectorSpace,
                     const std::vector<double>&     scales);
  //! Destructor.
  virtual ~uqBaseTKGroupClass();
  //@}

  //! @name Statistical/Mathematical methods
  //@{
  //! Whether or not the matrix is symmetric. See template specialization.
  virtual       bool                          symmetric                 () const = 0;
  
  //! Gaussian increment property to construct a transition kernel. See template specialization.
  virtual const uqGaussianVectorRVClass<V,M>& rv                        (unsigned int                     stageId ) = 0;
  
  //! Gaussian increment property to construct a transition kernel. See template specialization.
  virtual const uqGaussianVectorRVClass<V,M>& rv                        (const std::vector<unsigned int>& stageIds) = 0;
  //@}

  //! @name Misc methods
  //@{
  //! QUESO's environment.
  const uqBaseEnvironmentClass& env() const;

  //! Pre-computing position; access to protected attribute *m_preComputingPositions[stageId]. 
  const V&                                    preComputingPosition      (unsigned int stageId) const;
  
  //! Sets the pre-computing positions \c m_preComputingPositions[stageId] with a new vector of size \c position.
  virtual       bool                          setPreComputingPosition   (const V& position, unsigned int stageId);
  
  //! Clears the pre-computing positions \c m_preComputingPositions[stageId]
  virtual       void                          clearPreComputingPositions();
  //@}
  
  //! @name I/O methods
  //@{
  //! TODO: Prints the transition kernel.
  /*! \todo: implement me!*/
  virtual       void                          print                     (std::ostream& os) const;
  //@}
protected:
  const   uqEmptyEnvironmentClass*                    m_emptyEnv;
  const   uqBaseEnvironmentClass&                     m_env;
          std::string                                 m_prefix;
  const   uqVectorSpaceClass<V,M>*                    m_vectorSpace;
          std::vector<double>                         m_scales;
          std::vector<const V*>                       m_preComputingPositions;
          std::vector<uqGaussianVectorRVClass<V,M>* > m_rvs; // Gaussian, not Base... And nothing const...
};
// Default constructor ------------------------------
template<class V, class M>
uqBaseTKGroupClass<V,M>::uqBaseTKGroupClass()
  :
  m_emptyEnv             (new uqEmptyEnvironmentClass()),
  m_env                  (*m_emptyEnv),
  m_prefix               (""),
  m_vectorSpace          (NULL),
  m_scales               (0),
  m_preComputingPositions(NULL),
  m_rvs                  (0)
{
}
// Constructor with values---------------------------
template<class V, class M>
uqBaseTKGroupClass<V,M>::uqBaseTKGroupClass(
  const char*                    prefix,
  const uqVectorSpaceClass<V,M>& vectorSpace,
  const std::vector<double>&     scales)
  :
  m_emptyEnv             (NULL),
  m_env                  (vectorSpace.env()),
  m_prefix               (prefix),
  m_vectorSpace          (&vectorSpace),
  m_scales               (scales.size(),1.),
  m_preComputingPositions(scales.size()+1,NULL), // Yes, +1
  m_rvs                  (scales.size(),NULL) // IMPORTANT: it stays like this for scaledTK, but it will be overwritten to '+1' by hessianTK constructor
{
  for (unsigned int i = 0; i < m_scales.size(); ++i) {
    m_scales[i] = scales[i];
  }
}
// Destructor ---------------------------------------
template<class V, class M>
uqBaseTKGroupClass<V,M>::~uqBaseTKGroupClass()
{
  for (unsigned int i = 0; i < m_rvs.size(); ++i) {
    if (m_rvs[i]) delete m_rvs[i];
  }
  for (unsigned int i = 0; i < m_preComputingPositions.size(); ++i) {
    if (m_preComputingPositions[i]) delete m_preComputingPositions[i];
  }
  if (m_emptyEnv) delete m_emptyEnv;
}
// Misc methods--------------------------------------
template<class V, class M>
const uqBaseEnvironmentClass&
uqBaseTKGroupClass<V,M>::env() const
{
  return m_env;
}
//---------------------------------------------------
template<class V, class M>
const V&
uqBaseTKGroupClass<V,M>::preComputingPosition(unsigned int stageId) const
{
  UQ_FATAL_TEST_MACRO(m_preComputingPositions.size() <= stageId,
                      m_env.worldRank(),
                      "uqBaseTKGroupClass<V,M>::preComputingPosition()",
                      "m_preComputingPositions.size() <= stageId");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions[stageId] == NULL,
                      m_env.worldRank(),
                      "uqBaseTKGroupClass<V,M>::preComputingPosition()",
                      "m_preComputingPositions[stageId] == NULL");

  return *m_preComputingPositions[stageId];
}
//---------------------------------------------------
template<class V, class M>
bool
uqBaseTKGroupClass<V,M>::setPreComputingPosition(const V& position, unsigned int stageId)
{
  UQ_FATAL_TEST_MACRO(m_preComputingPositions.size() <= stageId,
                      m_env.worldRank(),
                      "uqBaseTKGroupClass<V,M>::setPreComputingPosition()",
                      "m_preComputingPositions.size() <= stageId");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions[stageId] != NULL,
                      m_env.worldRank(),
                      "uqBaseTKGroupClass<V,M>::setPreComputingPosition()",
                      "m_preComputingPositions[stageId] != NULL");

  m_preComputingPositions[stageId] = new V(position);

  return true;
}
//---------------------------------------------------
template<class V, class M>
void
uqBaseTKGroupClass<V,M>::clearPreComputingPositions()
{
  for (unsigned int i = 0; i < m_preComputingPositions.size(); ++i) {
    if (m_preComputingPositions[i]) {
      delete m_preComputingPositions[i];
      m_preComputingPositions[i] = NULL;
    }
  }

  return;
}
// I/O methods---------------------------------------
template<class V, class M>
void
uqBaseTKGroupClass<V,M>::print(std::ostream& os) const
{
  os << "In uqBaseTKGroupClass<V,M>::print()"
     << ": nothing to be printed" << std::endl;
  return;
}

//*****************************************************
// TK with scaled cov matrix
//*****************************************************
/*! \class uqScaledCovMatrixTKGroupClass
 *  \brief This class allows the representation of a transition kernel with a scaled covariance matrix. */
 
template<class V, class M>
class uqScaledCovMatrixTKGroupClass : public uqBaseTKGroupClass<V,M> {
public:
  //! @name Constructor/Destructor methods
  //@{ 
  //! Default constructor.
  uqScaledCovMatrixTKGroupClass(const char*                    prefix,
                                const uqVectorSpaceClass<V,M>& vectorSpace,
                                const std::vector<double>&     scales,
                                const M&                       covMatrix);
  //! Destructor.
  ~uqScaledCovMatrixTKGroupClass();
  //@}
  
  //! @name Statistical/Mathematical methods
  //@{
  //! Whether or not the matrix is symmetric. Always 'true'.
  /*! \todo: It only returns 'true', thus a test for its symmetricity must be included.*/
  bool                          symmetric                 () const;
  
  //! Gaussian increment property to construct a transition kernel.
  const uqGaussianVectorRVClass<V,M>& rv                        (unsigned int                     stageId );
  
  //! Gaussian increment property to construct a transition kernel.
  const uqGaussianVectorRVClass<V,M>& rv                        (const std::vector<unsigned int>& stageIds);
  
  //! Scales the covariance matrix.
  /*! The covariance matrix is scaled by a factor of \f$ 1/scales^2 \f$.*/
  void                          updateLawCovMatrix        (const M& covMatrix);
  //@}
  
  //! @name Misc methods
  //@{
  //! Sets the pre-computing positions \c m_preComputingPositions[stageId] with a new vector of size \c position.  
  bool                          setPreComputingPosition   (const V& position, unsigned int stageId);
  
  //! Clears the pre-computing positions \c m_preComputingPositions[stageId]
  void                          clearPreComputingPositions();
  //@}
  
  //! @name I/O methods
  //@{
  //! TODO: Prints the transition kernel.
  /*! \todo: implement me!*/
  void                          print                     (std::ostream& os) const;
  //@}
private:
  //! Sets the mean of the RVs to zero.
  void                          setRVsWithZeroMean        ();
  using uqBaseTKGroupClass<V,M>::m_env;
  using uqBaseTKGroupClass<V,M>::m_prefix;
  using uqBaseTKGroupClass<V,M>::m_vectorSpace;
  using uqBaseTKGroupClass<V,M>::m_scales;
  using uqBaseTKGroupClass<V,M>::m_preComputingPositions;
  using uqBaseTKGroupClass<V,M>::m_rvs;

  M m_originalCovMatrix;
};
// Default constructor ------------------------------
template<class V, class M>
uqScaledCovMatrixTKGroupClass<V,M>::uqScaledCovMatrixTKGroupClass(
  const char*                    prefix,
  const uqVectorSpaceClass<V,M>& vectorSpace, // FIX ME: vectorSubset ???
  const std::vector<double>&     scales,
  const M&                       covMatrix)
  :
  uqBaseTKGroupClass<V,M>(prefix,vectorSpace,scales),
  m_originalCovMatrix    (covMatrix)
{
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Entering uqScaledCovMatrixTKGroupClass<V,M>::constructor()"
                           << std::endl;
  }

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "In uqScaledCovMatrixTKGroupClass<V,M>::constructor()"
                           << ": m_scales.size() = "                << m_scales.size()
                           << ", m_preComputingPositions.size() = " << m_preComputingPositions.size()
                           << ", m_rvs.size() = "                   << m_rvs.size()
                           << ", m_originalCovMatrix = "            << m_originalCovMatrix
                           << std::endl;
  }

  setRVsWithZeroMean();

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Leaving uqScaledCovMatrixTKGroupClass<V,M>::constructor()"
                           << std::endl;
  }
}
// Destructor ---------------------------------------
template<class V, class M>
uqScaledCovMatrixTKGroupClass<V,M>::~uqScaledCovMatrixTKGroupClass()
{
}
// Math/Stats methods--------------------------------
template<class V, class M>
bool
uqScaledCovMatrixTKGroupClass<V,M>::symmetric() const
{
  return true;
}
//---------------------------------------------------
template<class V, class M>
const uqGaussianVectorRVClass<V,M>&
uqScaledCovMatrixTKGroupClass<V,M>::rv(unsigned int stageId)
{
  UQ_FATAL_TEST_MACRO(m_rvs.size() == 0,
                      m_env.worldRank(),
                      "uqScaledCovMatrixTKGroupClass<V,M>::rv1()",
                      "m_rvs.size() = 0");

  UQ_FATAL_TEST_MACRO(m_rvs[0] == NULL, // Yes, '0', because that is the id used below
                      m_env.worldRank(),
                      "uqScaledCovMatrixTKGroupClass<V,M>::rv1()",
                      "m_rvs[0] == NULL");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions.size() <= stageId,
                      m_env.worldRank(),
                      "uqScaledCovMatrixTKGroupClass<V,M>::rv1()",
                      "m_preComputingPositions.size() <= stageId");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions[stageId] == NULL,
                      m_env.worldRank(),
                      "uqScaledCovMatrixTKGroupClass<V,M>::rv1()",
                      "m_preComputingPositions[stageId] == NULL");

  if ((m_env.subDisplayFile()        ) &&
      (m_env.displayVerbosity() >= 10)) {
    *m_env.subDisplayFile() << "In uqScaledCovMatrixTKGroupClass<V,M>::rv1()"
                            << ", stageId = " << stageId
                            << ": about to call m_rvs[0]->updateLawExpVector()"
                            << ", vector = " << *m_preComputingPositions[stageId] // FIX ME: might demand parallelism
                            << std::endl;
  }
  m_rvs[0]->updateLawExpVector(*m_preComputingPositions[stageId]);

  return (*m_rvs[0]);
}
//---------------------------------------------------
template<class V, class M>
const uqGaussianVectorRVClass<V,M>&
uqScaledCovMatrixTKGroupClass<V,M>::rv(const std::vector<unsigned int>& stageIds)
{
  UQ_FATAL_TEST_MACRO(m_rvs.size() < stageIds.size(),
                      m_env.worldRank(),
                      "uqScaledCovMatrixTKGroupClass<V,M>::rv2()",
                      "m_rvs.size() < stageIds.size()");

  UQ_FATAL_TEST_MACRO(m_rvs[stageIds.size()-1] == NULL,
                      m_env.worldRank(),
                      "uqScaledCovMatrixTKGroupClass<V,M>::rv2()",
                      "m_rvs[stageIds.size()-1] == NULL");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions.size() <= stageIds[0],
                      m_env.worldRank(),
                      "uqScaledCovMatrixTKGroupClass<V,M>::rv2()",
                      "m_preComputingPositions.size() <= stageIds[0]");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions[stageIds[0]] == NULL,
                      m_env.worldRank(),
                      "uqScaledCovMatrixTKGroupClass<V,M>::rv2()",
                      "m_preComputingPositions[stageIds[0]] == NULL");

  if ((m_env.subDisplayFile()        ) &&
      (m_env.displayVerbosity() >= 10)) {
    *m_env.subDisplayFile() << "In uqScaledCovMatrixTKGroupClass<V,M>::rv2()"
                            << ", stageIds.size() = " << stageIds.size()
                            << ", stageIds[0] = "     << stageIds[0]
                            << ": about to call m_rvs[stageIds.size()-1]->updateLawExpVector()"
                            << ", vector = " << *m_preComputingPositions[stageIds[0]] // FIX ME: might demand parallelism
                            << std::endl;
  }
  m_rvs[stageIds.size()-1]->updateLawExpVector(*m_preComputingPositions[stageIds[0]]);

  return (*m_rvs[stageIds.size()-1]);
}
//---------------------------------------------------
template<class V, class M>
void
uqScaledCovMatrixTKGroupClass<V,M>::updateLawCovMatrix(const M& covMatrix)
{
  for (unsigned int i = 0; i < m_scales.size(); ++i) {
    double factor = 1./m_scales[i]/m_scales[i];
    if ((m_env.subDisplayFile()        ) &&
        (m_env.displayVerbosity() >= 10)) {
      *m_env.subDisplayFile() << "In uqScaledCovMatrixTKGroupClass<V,M>::updateLawCovMatrix()"
                              << ", m_scales.size() = " << m_scales.size()
                              << ", i = "               << i
                              << ", m_scales[i] = "     << m_scales[i]
                              << ", factor = "          << factor
                              << ": about to call m_rvs[i]->updateLawCovMatrix()"
                              << ", covMatrix = \n" << factor*covMatrix // FIX ME: might demand parallelism
                              << std::endl;
    }
    m_rvs[i]->updateLawCovMatrix(factor*covMatrix);
  }

  return;
}

// Misc methods -------------------------------------
template<class V, class M>
bool
uqScaledCovMatrixTKGroupClass<V,M>::setPreComputingPosition(const V& position, unsigned int stageId)
{
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Entering uqScaledCovMatrixTKGroupClass<V,M>::setPreComputingPosition()"
                           << ": position = " << position
                           << ", stageId = "  << stageId
                           << std::endl;
  }

  uqBaseTKGroupClass<V,M>::setPreComputingPosition(position,stageId);
  //setRVsWithZeroMean();

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "In uqScaledCovMatrixTKGroupClass<V,M>::setPreComputingPosition()"
                           << ", position = "        << position
                           << ", stageId = "         << stageId
                           << ": preComputingPos = " << *m_preComputingPositions[stageId];
    if (stageId < m_scales.size()) {
      *m_env.subDisplayFile() << ", factor = " << 1./m_scales[stageId]/m_scales[stageId];
    }
    if (stageId < m_rvs.size()) {
      const uqGaussianJointPdfClass<V,M>* pdfPtr = dynamic_cast< const uqGaussianJointPdfClass<V,M>* >(&(m_rvs[stageId]->pdf()));
      *m_env.subDisplayFile() << ", rvCov = " << pdfPtr->lawCovMatrix(); // FIX ME: might demand parallelism
    }
    *m_env.subDisplayFile() << std::endl;
  }

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Leaving uqScaledCovMatrixTKGroupClass<V,M>::setPreComputingPosition()"
                           << ": position = " << position
                           << ", stageId = "  << stageId
                           << std::endl;
  }

  return true;
}
//---------------------------------------------------
template<class V, class M>
void
uqScaledCovMatrixTKGroupClass<V,M>::clearPreComputingPositions()
{
  uqBaseTKGroupClass<V,M>::clearPreComputingPositions();
  return;
}


// Private methods------------------------------------
template<class V, class M>
void
uqScaledCovMatrixTKGroupClass<V,M>::setRVsWithZeroMean()
{
  UQ_FATAL_TEST_MACRO(m_rvs.size() == 0,
                      m_env.worldRank(),
                      "uqScaledCovMatrixTKGroupClass<V,M>::setRVsWithZeroMean()",
                      "m_rvs.size() = 0");

  UQ_FATAL_TEST_MACRO(m_rvs.size() != m_scales.size(),
                      m_env.worldRank(),
                      "uqScaledCovMatrixTKGroupClass<V,M>::setRVsWithZeroMean()",
                      "m_rvs.size() != m_scales.size()");

  for (unsigned int i = 0; i < m_scales.size(); ++i) {
    double factor = 1./m_scales[i]/m_scales[i];
    UQ_FATAL_TEST_MACRO(m_rvs[i] != NULL,
                        m_env.worldRank(),
                        "uqScaledCovMatrixTKGroupClass<V,M>::setRVsWithZeroMean()",
                        "m_rvs[i] != NULL");
    m_rvs[i] = new uqGaussianVectorRVClass<V,M>(m_prefix.c_str(),
                                                *m_vectorSpace,
                                                m_vectorSpace->zeroVector(),
                                                factor*m_originalCovMatrix);
  }

  return;
}
// I/O methods---------------------------------------
template<class V, class M>
void
uqScaledCovMatrixTKGroupClass<V,M>::print(std::ostream& os) const
{
  uqBaseTKGroupClass<V,M>::print(os);
  return;
}

//*****************************************************
// TK with Hessians
//*****************************************************
/*! \class uqHessianCovMatricesTKGroupClass
 *  \brief This class allows the representation of a transition kernel with Hessians. */

template<class V, class M>
class uqHessianCovMatricesTKGroupClass : public uqBaseTKGroupClass<V,M> {
public:
  //! @name Constructor/Destructor methods
  //@{ 
  //! Default constructor.
  uqHessianCovMatricesTKGroupClass(const char*                                   prefix,
                                   const uqVectorSpaceClass<V,M>&                vectorSpace,
                                   const std::vector<double>&                    scales,
                                   const uqScalarFunctionSynchronizerClass<V,M>& targetPdfSynchronizer);
 
   //! Destructor.
  ~uqHessianCovMatricesTKGroupClass();
  //@}
  
  //! @name Statistical/Mathematical methods
  //@{
  //! Whether or not the matrix is symmetric. Always 'false'.
  /*! \todo: It only returns 'false', thus a test for its symmetricity must be included.*/
  bool                          symmetric                 () const;
  
  //! Gaussian increment property to construct a transition kernel.
  const uqGaussianVectorRVClass<V,M>& rv                        (unsigned int                     stageId );
  
  //! Gaussian increment property to construct a transition kernel.
  const uqGaussianVectorRVClass<V,M>& rv                        (const std::vector<unsigned int>& stageIds);
   //@}
  
  //! @name Misc methods
  //@{
  //! Sets the pre-computing positions \c m_preComputingPositions[stageId] with a new vector of size \c position.
  bool                          setPreComputingPosition   (const V& position, unsigned int  stageId );
  
  //! Clears the pre-computing positions \c m_preComputingPositions[stageId]
  void                          clearPreComputingPositions();
  //@}
  
  //! @name I/O methods
  //@{
  //! TODO: Prints the transition kernel.
  /*! \todo: implement me!*/
  void                          print                     (std::ostream& os) const;
  //@}
private:
  using uqBaseTKGroupClass<V,M>::m_env;
  using uqBaseTKGroupClass<V,M>::m_prefix;
  using uqBaseTKGroupClass<V,M>::m_vectorSpace;
  using uqBaseTKGroupClass<V,M>::m_scales;
  using uqBaseTKGroupClass<V,M>::m_preComputingPositions;
  using uqBaseTKGroupClass<V,M>::m_rvs;

  const uqScalarFunctionSynchronizerClass<V,M>& m_targetPdfSynchronizer;
  std::vector<V*>                               m_originalNewtonSteps;
  std::vector<M*>                               m_originalCovMatrices;
};
// Default constructor ------------------------------
template<class V, class M>
uqHessianCovMatricesTKGroupClass<V,M>::uqHessianCovMatricesTKGroupClass(
  const char*                                   prefix,
  const uqVectorSpaceClass<V,M>&                vectorSpace,
  const std::vector<double>&                    scales,
  const uqScalarFunctionSynchronizerClass<V,M>& targetPdfSynchronizer)
  :
  uqBaseTKGroupClass<V,M>(prefix,vectorSpace,scales),
  m_targetPdfSynchronizer(targetPdfSynchronizer),
  m_originalNewtonSteps  (scales.size()+1,NULL), // Yes, +1
  m_originalCovMatrices  (scales.size()+1,NULL)  // Yes, +1
{
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Entering uqHessianCovMatricesTKGroupClass<V,M>::constructor()"
                           << std::endl;
  }

  m_rvs.resize(scales.size()+1,NULL); // Yes, +1 (IMPORTANT: overwrite initialization done by uqBaseTKGroupClass<V,M>)

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "In uqHessianCovMatricesTKGroupClass<V,M>::constructor()"
                           << ": m_scales.size() = "                   << m_scales.size()
                           << ", m_preComputingPositions.size() = "    << m_preComputingPositions.size()
                           << ", m_rvs.size() = "                      << m_rvs.size()
                           << ", m_originalNewtonSteps.size() = "      << m_originalNewtonSteps.size()
                           << ", m_originalCovMatrices.size() = "      << m_originalCovMatrices.size()
                           << std::endl;
  }

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Leaving uqHessianCovMatricesTKGroupClass<V,M>::constructor()"
                           << std::endl;
  }
}
// Destructor ---------------------------------------
template<class V, class M>
uqHessianCovMatricesTKGroupClass<V,M>::~uqHessianCovMatricesTKGroupClass()
{
}
// Math/Stats methods--------------------------------
template<class V, class M>
bool
uqHessianCovMatricesTKGroupClass<V,M>::symmetric() const
{
  return false;
}
//---------------------------------------------------
template<class V, class M>
const uqGaussianVectorRVClass<V,M>&
uqHessianCovMatricesTKGroupClass<V,M>::rv(unsigned int stageId)
{
  UQ_FATAL_TEST_MACRO(m_rvs.size() <= stageId,
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::rv1()",
                      "m_rvs.size() <= stageId");

  UQ_FATAL_TEST_MACRO(m_rvs[stageId] == NULL,
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::rv1()",
                      "m_rvs[stageId] == NULL");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions.size() <= stageId,
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::rv1()",
                      "m_preComputingPositions.size() <= stageId");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions[stageId] == NULL,
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::rv1()",
                      "m_preComputingPositions[stageId] == NULL");

  m_rvs[stageId]->updateLawExpVector(*m_preComputingPositions[stageId] + *m_originalNewtonSteps[stageId]);
  if ((m_env.subDisplayFile()        ) &&
      (m_env.displayVerbosity() >= 10)) {
    *m_env.subDisplayFile() << "In uqHessianCovMatrixTKGroupClass<V,M>::rv1()"
                            << ", stageId = " << stageId
                            << ": about to call m_rvs[stageId]->updateLawCovMatrix()"
                            << ", covMatrix = \n" << *m_originalCovMatrices[stageId] // FIX ME: might demand parallelism
                            << std::endl;
  }
  m_rvs[stageId]->updateLawCovMatrix(*m_originalCovMatrices[stageId]);

  return *m_rvs[stageId];
}
//---------------------------------------------------
template<class V, class M>
const uqGaussianVectorRVClass<V,M>&
uqHessianCovMatricesTKGroupClass<V,M>::rv(const std::vector<unsigned int>& stageIds)
{
  UQ_FATAL_TEST_MACRO(m_rvs.size() <= stageIds[0],
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::rv2()",
                      "m_rvs.size() <= stageIds[0]");

  UQ_FATAL_TEST_MACRO(m_rvs[stageIds[0]] == NULL,
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::rv2()",
                      "m_rvs[stageIds[0]] == NULL");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions.size() <= stageIds[0],
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::rv2()",
                      "m_preComputingPositions.size() <= stageIds[0]");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions[stageIds[0]] == NULL,
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::rv2()",
                      "m_preComputingPositions[stageIds[0]] == NULL");

  double factor = 1./m_scales[stageIds.size()-1]/m_scales[stageIds.size()-1];

  m_rvs[stageIds[0]]->updateLawExpVector(*m_preComputingPositions[stageIds[0]] + factor*(*m_originalNewtonSteps[stageIds[0]]));
  if ((m_env.subDisplayFile()        ) &&
      (m_env.displayVerbosity() >= 10)) {
    *m_env.subDisplayFile() << "In uqHessianCovMatrixTKGroupClass<V,M>::rv2()"
                            << ", stageIds.size() = " << stageIds.size()
                            << ", stageIds[0] = "     << stageIds[0]
                            << ", factor = "          << factor
                            << ": about to call m_rvs[stageIds[0]]->updateLawCovVector()"
                            << ", covMatrix = \n" << factor*(*m_originalCovMatrices[stageIds[0]]) // FIX ME: might demand parallelism
                            << std::endl;
  }
  m_rvs[stageIds[0]]->updateLawCovMatrix(factor*(*m_originalCovMatrices[stageIds[0]]));

  return *m_rvs[stageIds[0]];
}
// Misc methods--------------------------------------
template<class V, class M>
bool
uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition(const V& position, unsigned int stageId)
{
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Entering uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()"
                           << ": position = " << position
                           << ", stageId = "  << stageId
                           << std::endl;
  }

  bool validPreComputingPosition = true;

  // Verify consistency of sizes
  UQ_FATAL_TEST_MACRO(m_preComputingPositions.size() <= stageId,
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()",
                      "m_preComputingPositions.size() <= stageId");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions.size() != m_rvs.size(),
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()",
                      "m_preComputingPositions.size() != m_rvs.size()");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions.size() != m_originalNewtonSteps.size(),
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()",
                      "m_preComputingPositions.size() != m_originalNewtonSteps.size()");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions.size() != m_originalCovMatrices.size(),
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()",
                      "m_preComputingPositions.size() != m_originalCovMatrices.size()");

  // Verify data is not null
  UQ_FATAL_TEST_MACRO(m_preComputingPositions[stageId] != NULL,
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()",
                      "m_preComputingPositions[stageId] != NULL");

  UQ_FATAL_TEST_MACRO(m_rvs[stageId] != NULL,
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()",
                      "m_rvs[stageId] != NULL");

  UQ_FATAL_TEST_MACRO(m_originalNewtonSteps[stageId] != NULL,
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()",
                      "m_originalNewtonSteps[stageId] != NULL");

  UQ_FATAL_TEST_MACRO(m_originalCovMatrices[stageId] != NULL,
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()",
                      "m_originalCovMatrices[stageId] != NULL");

  uqBaseTKGroupClass<V,M>::setPreComputingPosition(position,stageId);

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "In uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()"
                           << ", position = "                          << position
                           << ", stageId = "                           << stageId
                           << ": m_originalNewtonSteps.size() = "      << m_originalNewtonSteps.size()
                           << ", m_originalCovMatrices.size() = "      << m_originalCovMatrices.size()
                           << ", m_preComputingPositions.size() = "    << m_preComputingPositions.size()
                           << ", m_rvs.size() = "                      << m_rvs.size()
                           << std::endl;
  }

  if (m_targetPdfSynchronizer.domainSet().contains(position)) {
    M* tmpHessian = m_vectorSpace->newMatrix();
    M* tmpCovMat  = m_vectorSpace->newMatrix();
    V* tmpGrad    = m_vectorSpace->newVector();

    double logPrior = 0.;
    double logLikelihood = 0.;
    double logTarget = 0.;
    logTarget = m_targetPdfSynchronizer.callFunction(&position, // Might demand parallel environment
                                                     NULL,
                                                     tmpGrad,
                                                     tmpHessian,
                                                     NULL,
                                                     &logPrior,
                                                     &logLikelihood);
    if (logTarget) {}; // just to remove compiler warning

    // IMPORTANT: covariance matrix = (Hessian)^{-1} !!!
    V unitVector(m_vectorSpace->zeroVector());
    V multVector(m_vectorSpace->zeroVector());
    for (unsigned int j = 0; j < tmpHessian->numCols(); ++j) {
      if (j > 0) unitVector[j-1] = 0.;
      unitVector[j] = 1.;
      tmpHessian->invertMultiply(unitVector, multVector);
      for (unsigned int i = 0; i < tmpHessian->numRowsLocal(); ++i) {
        (*tmpCovMat)(i,j) = multVector[i];
      }
    }
    if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
      *m_env.subDisplayFile() << "In uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()"
                             << ", position = "  << position
                             << ", stageId = "   << stageId
                             << ":\n H = "       << *tmpHessian
                             << "\n H^{-1} = "   << *tmpCovMat
                             << "\n H*H^{-1} = " << (*tmpHessian)*(*tmpCovMat)
                             << "\n H^{-1}*H = " << (*tmpCovMat)*(*tmpHessian)
                             << std::endl;
    }

    // Force covariance matrix to be symmetric, as the Hessian (supposedly) is
    *tmpCovMat = .5*((*tmpCovMat) + tmpCovMat->transpose());

    // Test if covariance matrix is positive definite
    M lowerChol(*tmpCovMat);
    if ((m_env.subDisplayFile()        ) &&
        (m_env.displayVerbosity() >= 10)) {
      *m_env.subDisplayFile() << "In uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()"
                              << ", position = "  << position
                              << ", stageId = "   << stageId
                              << ": calling lowerChol.chol()"
                              << ", lowerChol = " << lowerChol
                              << std::endl;
    }
    int iRC = lowerChol.chol();
    if (iRC) {
      std::cerr << "In uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition(): chol failed\n";
    }
    if ((m_env.subDisplayFile()        ) &&
        (m_env.displayVerbosity() >= 10)) {
      *m_env.subDisplayFile() << "In uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()"
                              << ", position = "  << position
                              << ", stageId = "   << stageId
                              << ": got lowerChol.chol() with iRC = " << iRC
                              << std::endl;
    }

    bool covIsPositiveDefinite = !iRC;

    if (covIsPositiveDefinite) {
      //UQ_FATAL_TEST_MACRO(stageId >= m_scales.size(),
      //                    m_env.worldRank(),
      //                    "uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()",
      //                    "stageId is too large for m_scales");
      //double factor = 1./m_scales[stageId]/m_scales[stageId];
      //*tmpCovMat *= factor;

      m_originalNewtonSteps[stageId] = new V(-1.*(*tmpCovMat)*(*tmpGrad));
      m_originalCovMatrices[stageId] = new M(*tmpCovMat);

      if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
        *m_env.subDisplayFile() << "In uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()"
                               << ", position = "        << position
                               << ", stageId = "         << stageId
                               << ", about to instantiate a Gaussian RV"
                               << ": tmpHessian = "      << *tmpHessian
                               << ", preComputingPos = " << *m_preComputingPositions[stageId]
                               << ", tmpCovMat = "       << *tmpCovMat
                               << ", tmpGrad = "         << *tmpGrad
                               << ", preComputedPos = "  << *m_preComputingPositions[stageId] + *m_originalNewtonSteps[stageId]
                             //<< ", factor = "          << factor
                             //<< ", rvCov = "           << factor*(*tmpCovMat)
                               << std::endl;
      }
      m_rvs[stageId] = new uqGaussianVectorRVClass<V,M>(m_prefix.c_str(),
                                                        *m_vectorSpace,
                                                        *m_preComputingPositions[stageId] + *m_originalNewtonSteps[stageId],
                                                        *m_originalCovMatrices[stageId]);
    }
    else {
      validPreComputingPosition = false;
    }

    delete tmpGrad;
    delete tmpCovMat;
    delete tmpHessian;
  }
  else {
    validPreComputingPosition = false;
  }

  if (validPreComputingPosition == false) {
    // Put "default" values on variables
    V tmpGrad  (m_vectorSpace->zeroVector());
    M tmpCovMat(tmpGrad,1.); // = identity matrix
    m_originalNewtonSteps[stageId] = new V(-1.*tmpCovMat*tmpGrad);
    m_originalCovMatrices[stageId] = new M(tmpCovMat);
    m_rvs[stageId] = new uqGaussianVectorRVClass<V,M>(m_prefix.c_str(),
                                                      *m_vectorSpace,
                                                      *m_preComputingPositions[stageId],
                                                      tmpCovMat);
  }

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 5)) {
    *m_env.subDisplayFile() << "Leaving uqHessianCovMatricesTKGroupClass<V,M>::setPreComputingPosition()"
                           << ": position = " << position
                           << ", stageId = "  << stageId
                           << std::endl;
  }

  return validPreComputingPosition;
}
//---------------------------------------------------
template<class V, class M>
void
uqHessianCovMatricesTKGroupClass<V,M>::clearPreComputingPositions()
{
  UQ_FATAL_TEST_MACRO(m_preComputingPositions.size() != m_originalNewtonSteps.size(),
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::clearPreComputingPositions()",
                      "m_preComputingPositions.size() != m_originalNewtonSteps.size()");

  UQ_FATAL_TEST_MACRO(m_preComputingPositions.size() != m_originalCovMatrices.size(),
                      m_env.worldRank(),
                      "uqHessianCovMatricesTKGroupClass<V,M>::clearPreComputingPositions()",
                      "m_preComputingPositions.size() != m_originalCovMatrices.size()");

  uqBaseTKGroupClass<V,M>::clearPreComputingPositions();

  // RVs are not deleted in base class because the cov matrices are constant in the case of scaledTK class
  for (unsigned int i = 0; i < m_rvs.size(); ++i) {
    if (m_rvs[i]) {
      delete m_rvs[i];
      m_rvs[i] = NULL;
    }
  }

  for (unsigned int i = 0; i < m_originalNewtonSteps.size(); ++i) {
    if (m_originalNewtonSteps[i]) {
      delete m_originalNewtonSteps[i];
      m_originalNewtonSteps[i] = NULL;
    }
  }

  for (unsigned int i = 0; i < m_originalCovMatrices.size(); ++i) {
    if (m_originalCovMatrices[i]) {
      delete m_originalCovMatrices[i];
      m_originalCovMatrices[i] = NULL;
    }
  }

  return;
}
// I/O methods---------------------------------------
template<class V, class M>
void
uqHessianCovMatricesTKGroupClass<V,M>::print(std::ostream& os) const
{
  uqBaseTKGroupClass<V,M>::print(os);
  return;
}
#endif // __UQ_TRANSITION_KERNEL_GROUP_H__
