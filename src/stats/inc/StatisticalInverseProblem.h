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

#ifndef UQ_SIP_H
#define UQ_SIP_H

#include <queso/StatisticalInverseProblemOptions.h>
#include <queso/MetropolisHastingsSG1.h>
#include <queso/MLSampling1.h>
#include <queso/InstantiateIntersection.h>
#include <queso/VectorRV.h>
#include <queso/ScalarFunction.h>
//#include <grvy.h>

namespace QUESO {

/*! \file uqStatisticalInverseProblem.h
    \brief Class to solve a Statistical Inverse Problem
*/

/*! \class StatisticalInverseProblem
 *  \brief This templated class represents a Statistical Inverse Problem.
 * 
 * This class is templated on the type 'P_V' of vector and type 'P_M' of matrix, where 
 * 'P_' stands for 'parameter'. Some templated classes might also use 'Q_V' and 'Q_M' 
 * when referring to a vector type and a matrix type, where 'Q_' stands for 'quantities 
 * of interest'. Conceptually, a statistical inverse problem has two input entities and 
 * one output entity.\n
 *
 * The input entities of a statistical inverse problem are:
<list type=number>
<item> the prior RV, an instance of class 'BaseVectorRV<P_V,P_M>', and
<item> the likelihood function, an instance of class 'BaseScalarFunction<P_V,P_M>'.
</list>
 * Let \f$ \pi()\f$ denote the mathematical likelihood function and \f$ x \f$ denote a 
 * vector of parameters. The likelihood function object stores the routine that computes 
 * \f$ \pi(x) \f$ and whatever data necessary by such routine. The routine in the 
 * likelihood function object can compute either the actual value \f$ \pi(x) \f$ or the
 * value \f$ \ln[\pi(x)] \f$. See files 
 * 'basic/inc/ScalarFunction.h' and 'stats/inc/JointPdf.h' for more details.\n
 *
 * The output entity of a statistical inverse problem is the posterior RV, another instance 
 * of class 'BaseVectorRV<P_V,P_M>', which stores the solution according to the Bayesian 
 * approach. Upon return from a solution operation, the posterior RV is available through the 
 * operation 'postRv()'. Such posterior RV is able to provide:
<list type=number>
<item> a joint pdf (up to a multiplicative constant) through the operation 'postRv().pdf()',
       which returns an instance of the class 'BaseJointPdf<P_V,P_M>', and
<item> a vector realizer through the operation 'postRv().realizer()', which returns an
       instance of the class 'BaseVectorRealizer<P_V,P_M>'.
</list>*/

/* OLD STUFF: The constructor of the 'scalar function' asks for the user to specify which 
 * value the routine is actually computing, so that the 'scalar function' class can properly 
 * implements both class operations 'actualValue()' and 'minus2LnValue()'*/

template <class P_V,class P_M>
class StatisticalInverseProblem
{
public:
 //! @name Constructor/Destructor methods
 //@{
 //! Constructor. 
 /*! Requirements: 1) the image set of the vector random variable 'priorRv', 2) the domain set of 
 * the likelihood function 'likelihoodFunction' and 3) the image set of the vector random variable 
 * 'postRv' should belong to vector spaces of equal dimensions. If the requirements are satisfied, 
 * the constructor then reads input options that begin with the string '\<prefix\>ip_'. If no 
 * options input file is provided, the construction assigns \c alternativeOptionsValues to the 
 * options of the SIP.*/
  StatisticalInverseProblem(const char*                               prefix,
                                   const SipOptionsValues*            alternativeOptionsValues, // dakota
                                   const BaseVectorRV      <P_V,P_M>& priorRv,            
                                   const BaseScalarFunction<P_V,P_M>& likelihoodFunction, 
                                         GenericVectorRV   <P_V,P_M>& postRv);
  
  //! Destructor
  ~StatisticalInverseProblem();
  //@}

  //! @name Statistical methods
  //@{
  //! Whether or not compute the solution.  
  bool                             computeSolutionFlag             () const;
   
  //! Solves the problem through Bayes formula and a Metropolis-Hastings algorithm. 
  /*! Requirements: 1) 'initialValues' should have the same number of components as member variable
   * 'm_priorRv' and 2) if 'initialProposalCovMatrix' is not NULL, it should be square and its size 
   * should be equal to the size of 'initialValues'. If the requirements are satisfied, this methods
   * checks the member flag 'm_computeSolution' (one of the options read from the input file during 
   * construction). If the flag is 'false', the operation returns immediately, computing nothing; 
   * otherwise, the operation sets the member variable 'm_postRv' accordingly. The operation:
<list type=number>
<item> sets the pdf of 'm_postRv' equal to an instance of 'BayesianJointPdf<P_V,P_M>',
<item> instantiates 'SequenceOfVectors<P_V,P_M>' (the chain),
<item> instantiates 'MetropolisHastingsSG<P_V,P_M>' (the Metropolis-Hastings algorithm),
<item> populates the chain with the Metropolis-Hastings algorithm, and
<item> sets the realizer of 'm_postRv' with the contents of the chain.
</list> */
  void solveWithBayesMetropolisHastings(const MhOptionsValues* alternativeOptionsValues, // dakota
					const P_V&                    initialValues,
					const P_M*                    initialProposalCovMatrix);
  
  //! Solves with Bayes Multi-Level (ML) sampling.
  void                             solveWithBayesMLSampling        ();
  
  //! Returns the Prior RV; access to private attribute m_priorRv.
  const BaseVectorRV   <P_V,P_M>& priorRv                   () const;
  
  //! Returns the Posterior RV; access to private attribute m_postrRv.
  /*! The Posterior RV contains the solution of the Bayes problem.*/
  const GenericVectorRV<P_V,P_M>& postRv                    () const;
  
  //! Returns the logarithm value of the evidence. Related to ML.
  double                           logEvidence                     () const;

   //! Returns the mean of the logarithm value of the likelihood. Related to ML.
  double                           meanLogLikelihood               () const;
  
  //\TODO Related to ML.
  double                           eig                             () const;
  //@}   
	
  //! @name I/O methods
  //@{
  //! TODO: Prints the sequence.
  /*! \todo: implement me!*/
  void print(std::ostream& os) const;
  //@}
private:
  const BaseEnvironment&                 m_env;

  const BaseVectorRV        <P_V,P_M>&   m_priorRv;
  const BaseScalarFunction  <P_V,P_M>&   m_likelihoodFunction;
        GenericVectorRV     <P_V,P_M>&   m_postRv;

        VectorSet           <P_V,P_M>*   m_solutionDomain;
        BaseJointPdf        <P_V,P_M>*   m_solutionPdf;
        BaseVectorMdf       <P_V,P_M>*   m_subSolutionMdf;
        BaseVectorCdf       <P_V,P_M>*   m_subSolutionCdf;
        BaseVectorRealizer  <P_V,P_M>*   m_solutionRealizer;

        MetropolisHastingsSG<P_V,P_M>*   m_mhSeqGenerator;
        MLSampling          <P_V,P_M>*   m_mlSampler;
        BaseVectorSequence  <P_V,P_M>*   m_chain;
        ScalarSequence      <double>*    m_logLikelihoodValues;
        ScalarSequence      <double>*    m_logTargetValues;
        
        SipOptionsValues                  m_alternativeOptionsValues;
        StatisticalInverseProblemOptions* m_optionsObj;

#ifdef UQ_ALSO_COMPUTE_MDFS_WITHOUT_KDE
        ArrayOfOneDGrids    <P_V,P_M>*   m_subMdfGrids;
        ArrayOfOneDTables   <P_V,P_M>*   m_subMdfValues;
#endif
};
//! Prints the object \c obj, overloading an operator.
template<class P_V,class P_M>
std::ostream& operator<<(std::ostream& os, const StatisticalInverseProblem<P_V,P_M>& obj);

// Default constructor -----------------------------
template <class P_V,class P_M>
StatisticalInverseProblem<P_V,P_M>::StatisticalInverseProblem(
  /*! The prefix                 */ const char*                               prefix,
  /*! Options (if no input file) */ const SipOptionsValues*            alternativeOptionsValues, // dakota
  /*! The prior RV               */ const BaseVectorRV      <P_V,P_M>& priorRv,
  /*! The likelihood function    */ const BaseScalarFunction<P_V,P_M>& likelihoodFunction,
  /*! The posterior RV           */       GenericVectorRV   <P_V,P_M>& postRv)
  :
  m_env                     (priorRv.env()),
  m_priorRv                 (priorRv),
  m_likelihoodFunction      (likelihoodFunction),
  m_postRv                  (postRv),
  m_solutionDomain          (NULL),
  m_solutionPdf             (NULL),
  m_subSolutionMdf          (NULL),
  m_subSolutionCdf          (NULL),
  m_solutionRealizer        (NULL),
  m_mhSeqGenerator          (NULL),
  m_mlSampler               (NULL),
  m_chain                   (NULL),
  m_logLikelihoodValues     (NULL),
  m_logTargetValues         (NULL),
  m_alternativeOptionsValues(),
  m_optionsObj              (NULL)
{
#ifdef QUESO_MEMORY_DEBUGGING
  std::cout << "Entering Sip" << std::endl;
#endif
  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "Entering StatisticalInverseProblem<P_V,P_M>::constructor()"
                            << ": prefix = " << prefix
                            << ", alternativeOptionsValues = " << alternativeOptionsValues
                            << ", m_env.optionsInputFileName() = " << m_env.optionsInputFileName()
                            << std::endl;
  }

  if (alternativeOptionsValues) m_alternativeOptionsValues = *alternativeOptionsValues;
  if (m_env.optionsInputFileName() == "") {
    m_optionsObj = new StatisticalInverseProblemOptions(m_env,prefix,m_alternativeOptionsValues);
  }
  else {
    m_optionsObj = new StatisticalInverseProblemOptions(m_env,prefix);
    m_optionsObj->scanOptionsValues();
  }
#ifdef QUESO_MEMORY_DEBUGGING
  std::cout << "In Sip, finished scanning options" << std::endl;
#endif

  UQ_FATAL_TEST_MACRO(priorRv.imageSet().vectorSpace().dimLocal() != likelihoodFunction.domainSet().vectorSpace().dimLocal(),
                      m_env.worldRank(),
                      "StatisticalInverseProblem<P_V,P_M>::constructor()",
                      "'priorRv' and 'likelihoodFunction' are related to vector spaces of different dimensions");

  UQ_FATAL_TEST_MACRO(priorRv.imageSet().vectorSpace().dimLocal() != postRv.imageSet().vectorSpace().dimLocal(),
                      m_env.worldRank(),
                      "StatisticalInverseProblem<P_V,P_M>::constructor()",
                      "'priorRv' and 'postRv' are related to vector spaces of different dimensions");

  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "Leaving StatisticalInverseProblem<P_V,P_M>::constructor()"
                            << ": prefix = " << m_optionsObj->m_prefix
                            << std::endl;
  }

  return;
}

// Destructor --------------------------------------
template <class P_V,class P_M>
StatisticalInverseProblem<P_V,P_M>::~StatisticalInverseProblem()
{
  if (m_chain) {
    m_chain->clear();
    delete m_chain;
  }
  if (m_logLikelihoodValues) {
    m_logLikelihoodValues->clear();
    delete m_logLikelihoodValues;
  }
  if (m_logTargetValues) {
    m_logTargetValues->clear();
    delete m_logTargetValues;
  }
  if (m_mlSampler       ) delete m_mlSampler;
  if (m_mhSeqGenerator  ) delete m_mhSeqGenerator;
  if (m_solutionRealizer) delete m_solutionRealizer;
  if (m_subSolutionCdf  ) delete m_subSolutionCdf;
  if (m_subSolutionMdf  ) delete m_subSolutionMdf;
  if (m_solutionPdf     ) delete m_solutionPdf;
  if (m_solutionDomain  ) delete m_solutionDomain;
  if (m_optionsObj      ) delete m_optionsObj;
}
// Statistical methods -----------------------------
template <class P_V,class P_M>
void
StatisticalInverseProblem<P_V,P_M>::solveWithBayesMetropolisHastings(
  const MhOptionsValues* alternativeOptionsValues, // dakota
  const P_V&                    initialValues,
  const P_M*                    initialProposalCovMatrix)
{
  //grvy_timer_begin("BayesMetropolisHastings"); TODO: revisit timing output
  //std::cout << "proc " << m_env.fullRank() << ", HERE sip 000" << std::endl;
  m_env.fullComm().Barrier();
  //std::cout << "proc " << m_env.fullRank() << ", HERE sip 001" << std::endl;
  m_env.fullComm().syncPrintDebugMsg("Entering StatisticalInverseProblem<P_V,P_M>::solveWithBayesMetropolisHastings()",1,3000000);

  if (m_optionsObj->m_ov.m_computeSolution == false) {
    if ((m_env.subDisplayFile())) {
      *m_env.subDisplayFile() << "In StatisticalInverseProblem<P_V,P_M>::solveWithBayesMetropolisHastings()"
                              << ": avoiding solution, as requested by user"
                              << std::endl;
    }
    return;
  }
  if ((m_env.subDisplayFile())) {
    *m_env.subDisplayFile() << "In StatisticalInverseProblem<P_V,P_M>::solveWithBayesMetropolisHastings()"
                            << ": computing solution, as requested by user"
                            << std::endl;
  }

  UQ_FATAL_TEST_MACRO(m_priorRv.imageSet().vectorSpace().dimLocal() != initialValues.sizeLocal(),
                      m_env.worldRank(),
                      "StatisticalInverseProblem<P_V,P_M>::solveWithBayesMetropolisHastings()",
                      "'m_priorRv' and 'initialValues' should have equal dimensions");

  if (initialProposalCovMatrix) {
    UQ_FATAL_TEST_MACRO(m_priorRv.imageSet().vectorSpace().dimLocal() != initialProposalCovMatrix->numRowsLocal(),
                        m_env.worldRank(),
                        "StatisticalInverseProblem<P_V,P_M>::solveWithBayesMetropolisHastings()",
                        "'m_priorRv' and 'initialProposalCovMatrix' should have equal dimensions");
    UQ_FATAL_TEST_MACRO(initialProposalCovMatrix->numCols() != initialProposalCovMatrix->numRowsGlobal(),
                        m_env.worldRank(),
                        "StatisticalInverseProblem<P_V,P_M>::solveWithBayesMetropolisHastings()",
                        "'initialProposalCovMatrix' should be a square matrix");
  }

  if (m_mlSampler       ) delete m_mlSampler;
  if (m_mhSeqGenerator  ) delete m_mhSeqGenerator;
  if (m_solutionRealizer) delete m_solutionRealizer;
  if (m_subSolutionCdf  ) delete m_subSolutionCdf;
  if (m_subSolutionMdf  ) delete m_subSolutionMdf;
  if (m_solutionPdf     ) delete m_solutionPdf;
  if (m_solutionDomain  ) delete m_solutionDomain;

  P_V numEvaluationPointsVec(m_priorRv.imageSet().vectorSpace().zeroVector());
  numEvaluationPointsVec.cwSet(250.);

  // Compute output pdf up to a multiplicative constant: Bayesian approach
  m_solutionDomain = InstantiateIntersection(m_priorRv.pdf().domainSet(),m_likelihoodFunction.domainSet());

  m_solutionPdf = new BayesianJointPdf<P_V,P_M>(m_optionsObj->m_prefix.c_str(),
                                                       m_priorRv.pdf(),
                                                       m_likelihoodFunction,
                                                       1.,
                                                       *m_solutionDomain);

  m_postRv.setPdf(*m_solutionPdf);

  // Compute output realizer: Metropolis-Hastings approach
  m_chain               = new SequenceOfVectors<P_V,P_M>(m_postRv.imageSet().vectorSpace(),0,m_optionsObj->m_prefix+"chain");
  m_logLikelihoodValues = new ScalarSequence<double>    (m_env,0,m_optionsObj->m_prefix+"logLike"  );
  m_logTargetValues     = new ScalarSequence<double>    (m_env,0,m_optionsObj->m_prefix+"logTarget");
  m_mhSeqGenerator = new MetropolisHastingsSG<P_V,P_M>(m_optionsObj->m_prefix.c_str(), // dakota
                                                              alternativeOptionsValues,
                                                              m_postRv,
                                                              initialValues,
                                                              initialProposalCovMatrix);

  m_mhSeqGenerator->generateSequence(*m_chain,
                                     NULL, //m_logLikelihoodValues,
                                     NULL);//m_logTargetValues);

  m_solutionRealizer = new SequentialVectorRealizer<P_V,P_M>(m_optionsObj->m_prefix.c_str(),
                                                                    *m_chain);

  m_postRv.setRealizer(*m_solutionRealizer);

  m_env.fullComm().syncPrintDebugMsg("In StatisticalInverseProblem<P_V,P_M>::solveWithBayesMetropolisHastings(), code place 1",3,3000000);
  //m_env.fullComm().Barrier();

  // Compute output mdf: uniform sampling approach
#ifdef UQ_ALSO_COMPUTE_MDFS_WITHOUT_KDE
  m_subMdfGrids  = new ArrayOfOneDGrids <P_V,P_M>((m_optionsObj->m_prefix+"Mdf_").c_str(),m_postRv.imageSet().vectorSpace());
  m_subMdfValues = new ArrayOfOneDTables<P_V,P_M>((m_optionsObj->m_prefix+"Mdf_").c_str(),m_postRv.imageSet().vectorSpace());
  m_chain->subUniformlySampledMdf(numEvaluationPointsVec, // input
                                  *m_subMdfGrids,         // output
                                  *m_subMdfValues);       // output
  m_subSolutionMdf = new SampledVectorMdf<P_V,P_M>(m_optionsObj->m_prefix.c_str(),
                                                          *m_subMdfGrids,
                                                          *m_subMdfValues);
  m_postRv.setMdf(*m_subSolutionMdf);

  if ((m_optionsObj->m_ov.m_dataOutputFileName                       != UQ_SIP_FILENAME_FOR_NO_FILE                    ) &&
      (m_optionsObj->m_ov.m_dataOutputAllowedSet.find(m_env.subId()) != m_optionsObj->m_ov.m_dataOutputAllowedSet.end())) {
    if (m_env.subRank() == 0) {
      // Write data output file
      if (m_env.subDisplayFile()) {
        *m_env.subDisplayFile() << "Opening data output file '" << m_optionsObj->m_ov.m_dataOutputFileName
                                << "' for calibration problem with problem with prefix = " << m_optionsObj->m_prefix
                                << std::endl;
      }

      // Open file
      // Always write at the end of an eventual pre-existing file
      std::ofstream* ofsvar = new std::ofstream((m_optionsObj->m_ov.m_dataOutputFileName+"_sub"+m_env.subIdString()+".m").c_str(), std::ofstream::out | std::ofstream::in | std::ofstream::ate);
      if ((ofsvar            == NULL ) ||
          (ofsvar->is_open() == false)) {
        delete ofsvar;
        ofsvar = new std::ofstream((m_optionsObj->m_ov.m_dataOutputFileName+"_sub"+m_env.subIdString()+".m").c_str(), std::ofstream::out | std::ofstream::trunc);
      }
      UQ_FATAL_TEST_MACRO((ofsvar && ofsvar->is_open()) == false,
                          m_env.worldRank(),
                          "StatisticalInverseProblem<P_V,P_M>::solveWithBayesMetropolisHastings()",
                          "failed to open file");

      m_postRv.mdf().print(*ofsvar);

      // Close file
      //ofsvar->close();
      delete ofsvar;
      if (m_env.subDisplayFile()) {
        *m_env.subDisplayFile() << "Closed data output file '" << m_optionsObj->m_ov.m_dataOutputFileName
                                << "' for calibration problem with problem with prefix = " << m_optionsObj->m_prefix
                                << std::endl;
      }
    }
  }
#endif
  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << std::endl;
  }

  m_env.fullComm().syncPrintDebugMsg("Leaving StatisticalInverseProblem<P_V,P_M>::solveWithBayesMetropolisHastings()",1,3000000);
  m_env.fullComm().Barrier();
  // grvy_timer_end("BayesMetropolisHastings"); TODO: revisit timers
  return;
}
//--------------------------------------------------
template <class P_V,class P_M>
void
StatisticalInverseProblem<P_V,P_M>::solveWithBayesMLSampling()
{
  m_env.fullComm().Barrier();
  m_env.fullComm().syncPrintDebugMsg("Entering StatisticalInverseProblem<P_V,P_M>::solveWithBayesMLSampling()",1,3000000);

  if (m_optionsObj->m_ov.m_computeSolution == false) {
    if ((m_env.subDisplayFile())) {
      *m_env.subDisplayFile() << "In StatisticalInverseProblem<P_V,P_M>::solveWithBayesMLSampling()"
                              << ": avoiding solution, as requested by user"
                              << std::endl;
    }
    return;
  }
  if ((m_env.subDisplayFile())) {
    *m_env.subDisplayFile() << "In StatisticalInverseProblem<P_V,P_M>::solveWithBayesMLSampling()"
                            << ": computing solution, as requested by user"
                            << std::endl;
  }

  if (m_mlSampler       ) delete m_mlSampler;
  if (m_mhSeqGenerator  ) delete m_mhSeqGenerator;
  if (m_solutionRealizer) delete m_solutionRealizer;
  if (m_subSolutionCdf  ) delete m_subSolutionCdf;
  if (m_subSolutionMdf  ) delete m_subSolutionMdf;
  if (m_solutionPdf     ) delete m_solutionPdf;
  if (m_solutionDomain  ) delete m_solutionDomain;

  P_V numEvaluationPointsVec(m_priorRv.imageSet().vectorSpace().zeroVector());
  numEvaluationPointsVec.cwSet(250.);

  // Compute output pdf up to a multiplicative constant: Bayesian approach
  m_solutionDomain = InstantiateIntersection(m_priorRv.pdf().domainSet(),m_likelihoodFunction.domainSet());

  m_solutionPdf = new BayesianJointPdf<P_V,P_M>(m_optionsObj->m_prefix.c_str(),
                                                       m_priorRv.pdf(),
                                                       m_likelihoodFunction,
                                                       1.,
                                                       *m_solutionDomain);

  m_postRv.setPdf(*m_solutionPdf);

  // Compute output realizer: ML approach
  m_chain = new SequenceOfVectors<P_V,P_M>(m_postRv.imageSet().vectorSpace(),0,m_optionsObj->m_prefix+"chain");
  m_mlSampler = new MLSampling<P_V,P_M>(m_optionsObj->m_prefix.c_str(),
                                             //m_postRv,
                                               m_priorRv,
                                               m_likelihoodFunction);
  //                                           initialValues,
  //                                           initialProposalCovMatrix);

  m_mlSampler->generateSequence(*m_chain,
                                NULL,
                                NULL);

  m_solutionRealizer = new SequentialVectorRealizer<P_V,P_M>(m_optionsObj->m_prefix.c_str(),
                                                                    *m_chain);

  m_postRv.setRealizer(*m_solutionRealizer);

  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << std::endl;
  }

  m_env.fullComm().syncPrintDebugMsg("Leaving StatisticalInverseProblem<P_V,P_M>::solveWithBayesMLSampling()",1,3000000);
  m_env.fullComm().Barrier();

  return;
}
//--------------------------------------------------
template <class P_V,class P_M>
const BaseVectorRV<P_V,P_M>& 
StatisticalInverseProblem<P_V,P_M>::priorRv() const
{
  return m_priorRv;
}
//--------------------------------------------------
template <class P_V,class P_M>
const GenericVectorRV<P_V,P_M>& 
StatisticalInverseProblem<P_V,P_M>::postRv() const
{
  return m_postRv;
}
//--------------------------------------------------
template <class P_V,class P_M>
double StatisticalInverseProblem<P_V,P_M>::logEvidence() const
{
  UQ_FATAL_TEST_MACRO(m_mlSampler == NULL,
                      m_env.worldRank(),
                      "StatisticalInverseProblem<P_V,P_M>::logEvidence()",
                      "m_mlSampler is NULL");
  return m_mlSampler->logEvidence();
}
//--------------------------------------------------
template <class P_V,class P_M>
double StatisticalInverseProblem<P_V,P_M>::meanLogLikelihood() const
{
  UQ_FATAL_TEST_MACRO(m_mlSampler == NULL,
                      m_env.worldRank(),
                      "StatisticalInverseProblem<P_V,P_M>::meanLogLikelihood()",
                      "m_mlSampler is NULL");
  return m_mlSampler->meanLogLikelihood();
}
//--------------------------------------------------
template <class P_V,class P_M>
double StatisticalInverseProblem<P_V,P_M>::eig() const
{
  UQ_FATAL_TEST_MACRO(m_mlSampler == NULL,
                      m_env.worldRank(),
                      "StatisticalInverseProblem<P_V,P_M>::eig()",
                      "m_mlSampler is NULL");
  return m_mlSampler->eig();
}
// I/O methods--------------------------------------
template <class P_V,class P_M>
void
StatisticalInverseProblem<P_V,P_M>::print(std::ostream& os) const
{
  return;
}
//--------------------------------------------------
// Operator declared outside class definition-------
//--------------------------------------------------
template<class P_V,class P_M>
std::ostream& operator<<(std::ostream& os, const StatisticalInverseProblem<P_V,P_M>& obj)
{
  obj.print(os);

  return os;
}

}  // End namespace QUESO

#endif // UQ_SIP_H
