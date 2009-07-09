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
 * $Id$
 *
 * Brief description of this file: 
 * 
 *--------------------------------------------------------------------------
 *-------------------------------------------------------------------------- */

#ifndef __UQ_SFP_H__
#define __UQ_SFP_H__

#include <uqStatisticalForwardProblemOptions.h>
#include <uqVectorFunction.h>
#include <uqMonteCarloSG.h>
#include <uqVectorRV.h>
#include <uqSequenceOfVectors.h>

/*! A templated class that represents statistical forward problems.
 */
template <class P_V,class P_M,class Q_V,class Q_M>
class uqStatisticalForwardProblemClass
{
public:
  uqStatisticalForwardProblemClass(const char*                                       prefix,      /*! The prefix.                  */
                                   const uqBaseVectorRVClass      <P_V,P_M>&         paramRv,     /*! The input random variable.   */
                                   const uqBaseVectorFunctionClass<P_V,P_M,Q_V,Q_M>& qoiFunction, /*! The function to compute qoi. */
                                   uqGenericVectorRVClass         <Q_V,Q_M>&         qoiRv);      /*! The output random variable.  */
 ~uqStatisticalForwardProblemClass();

        bool                             computeSolutionFlag() const;
        void                             solveWithMonteCarlo();
  const uqGenericVectorRVClass<Q_V,Q_M>& qoiRv              () const;
  const uqBaseVectorCdfClass  <Q_V,Q_M>& qoiRv_unifiedCdf   () const;

        void                             print              (std::ostream& os) const;

private:
        void                             commonConstructor  ();

  const uqBaseEnvironmentClass&                     m_env;

  const uqBaseVectorRVClass      <P_V,P_M>&         m_paramRv;
  const uqBaseVectorFunctionClass<P_V,P_M,Q_V,Q_M>& m_qoiFunction;
        uqGenericVectorRVClass   <Q_V,Q_M>&         m_qoiRv; // FIX ME: Maybe not always generic ?

        uqBaseVectorSequenceClass<Q_V,Q_M>*         m_paramChain;
        uqBaseVectorSequenceClass<Q_V,Q_M>*         m_qoiChain;
        uqMonteCarloSGClass      <P_V,P_M,Q_V,Q_M>* m_mcSeqGenerator;

        uqBaseVectorRealizerClass<Q_V,Q_M>*         m_solutionRealizer;
 
        uqArrayOfOneDGridsClass  <Q_V,Q_M>*         m_subMdfGrids;
        uqArrayOfOneDTablesClass <Q_V,Q_M>*         m_subMdfValues;
        uqBaseVectorMdfClass     <Q_V,Q_M>*         m_subSolutionMdf;

        uqArrayOfOneDGridsClass  <Q_V,Q_M>*         m_subCdfGrids;
        uqArrayOfOneDTablesClass <Q_V,Q_M>*         m_subCdfValues;
        uqBaseVectorCdfClass     <Q_V,Q_M>*         m_subSolutionCdf;

        uqArrayOfOneDGridsClass  <Q_V,Q_M>*         m_unifiedCdfGrids;
        uqArrayOfOneDTablesClass <Q_V,Q_M>*         m_unifiedCdfValues;
        uqBaseVectorCdfClass     <Q_V,Q_M>*         m_unifiedSolutionCdf;

        uqBaseJointPdfClass      <Q_V,Q_M>*         m_solutionPdf;

        uqStatisticalForwardProblemOptionsClass     m_options;
};

template<class P_V,class P_M,class Q_V,class Q_M>
std::ostream& operator<<(std::ostream& os, const uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>& obj);

template <class P_V,class P_M,class Q_V,class Q_M>
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::uqStatisticalForwardProblemClass(
  const char*                                       prefix,
  const uqBaseVectorRVClass      <P_V,P_M>&         paramRv,
  const uqBaseVectorFunctionClass<P_V,P_M,Q_V,Q_M>& qoiFunction,
        uqGenericVectorRVClass   <Q_V,Q_M>&         qoiRv)
  :
  m_env               (paramRv.env()),
  m_paramRv           (paramRv),
  m_qoiFunction       (qoiFunction),
  m_qoiRv             (qoiRv),
  m_paramChain        (NULL),
  m_qoiChain          (NULL),
  m_mcSeqGenerator    (NULL),
  m_solutionRealizer  (NULL),
  m_subMdfGrids       (NULL),
  m_subMdfValues      (NULL),
  m_subSolutionMdf    (NULL),
  m_subCdfGrids       (NULL),
  m_subCdfValues      (NULL),
  m_subSolutionCdf    (NULL),
  m_unifiedCdfGrids   (NULL),
  m_unifiedCdfValues  (NULL),
  m_unifiedSolutionCdf(NULL),
  m_solutionPdf       (NULL),
  m_options           (m_env,prefix)
{
  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "Entering uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::constructor()"
                           << ": prefix = " << m_options.m_prefix
                           << std::endl;
  }

  m_options.scanOptionsValues();

  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "Leaving uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::constructor()"
                           << ": prefix = " << m_options.m_prefix
                           << std::endl;
  }
}

template <class P_V,class P_M,class Q_V,class Q_M>
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::~uqStatisticalForwardProblemClass()
{
  if (m_solutionPdf       ) delete m_solutionPdf;

  if (m_unifiedSolutionCdf) delete m_unifiedSolutionCdf;
  if (m_unifiedCdfValues  ) delete m_unifiedCdfValues;
  if (m_unifiedCdfGrids   ) delete m_unifiedCdfGrids;

  if (m_subSolutionCdf    ) delete m_subSolutionCdf;
  if (m_subCdfValues      ) delete m_subCdfValues;
  if (m_subCdfGrids       ) delete m_subCdfGrids;

  if (m_subSolutionMdf    ) delete m_subSolutionMdf;
  if (m_subMdfValues      ) delete m_subMdfValues;
  if (m_subMdfGrids       ) delete m_subMdfGrids;

  if (m_solutionRealizer  ) delete m_solutionRealizer;

  if (m_mcSeqGenerator    ) delete m_mcSeqGenerator;

  if (m_qoiChain) {
    m_qoiChain->clear();
    delete m_qoiChain;
  }

  if (m_paramChain) {
    m_paramChain->clear();
    delete m_paramChain;
  }
}

template <class P_V,class P_M, class Q_V, class Q_M>
bool
  uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::computeSolutionFlag() const
{
  return m_options.m_computeSolution;
}

template <class P_V,class P_M,class Q_V,class Q_M>
void
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::solveWithMonteCarlo()
{
  m_env.fullComm().Barrier();
  m_env.syncPrintDebugMsg("Entering uqStatisticalForwardProblemClass<P_V,P_M>::solveWithMonteCarlo()",1,3000000,m_env.fullComm());

  if (m_options.m_computeSolution == false) {
    if ((m_env.subDisplayFile())) {
      *m_env.subDisplayFile() << "In uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::solveWithMonteCarlo()"
                              << ": avoiding solution, as requested by user"
                              << std::endl;
    }
    return;
  }
  if ((m_env.subDisplayFile())) {
    *m_env.subDisplayFile() << "In uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::solveWithMonteCarlo()"
                            << ": computing solution, as requested by user"
                            << std::endl;
  }

  if (m_solutionPdf       ) delete m_solutionPdf;

  if (m_unifiedSolutionCdf) delete m_unifiedSolutionCdf;
  if (m_unifiedCdfValues  ) delete m_unifiedCdfValues;
  if (m_unifiedCdfGrids   ) delete m_unifiedCdfGrids;

  if (m_subSolutionCdf    ) delete m_subSolutionCdf;
  if (m_subCdfValues      ) delete m_subCdfValues;
  if (m_subCdfGrids       ) delete m_subCdfGrids;

  if (m_subSolutionMdf    ) delete m_subSolutionMdf;
  if (m_subMdfValues      ) delete m_subMdfValues;
  if (m_subMdfGrids       ) delete m_subMdfGrids;

  if (m_solutionRealizer  ) delete m_solutionRealizer;

  if (m_mcSeqGenerator    ) delete m_mcSeqGenerator;

  if (m_qoiChain) {
    m_qoiChain->clear();
    delete m_qoiChain;
  }

  if (m_paramChain) {
    m_paramChain->clear();
    delete m_paramChain;
  }

  Q_V numEvaluationPointsVec(m_qoiRv.imageSet().vectorSpace().zeroVector());
  numEvaluationPointsVec.cwSet(250.);

  // Compute output realizer: Monte Carlo approach
  m_paramChain = new uqSequenceOfVectorsClass<P_V,P_M>(m_paramRv.imageSet().vectorSpace(),0,m_options.m_prefix+"paramChain");
  m_qoiChain   = new uqSequenceOfVectorsClass<Q_V,Q_M>(m_qoiRv.imageSet().vectorSpace(),  0,m_options.m_prefix+"qoiChain"  );
  m_mcSeqGenerator = new uqMonteCarloSGClass<P_V,P_M,Q_V,Q_M>(m_options.m_prefix.c_str(),
                                                              m_paramRv,
                                                              m_qoiFunction,
                                                              m_qoiRv);
  m_mcSeqGenerator->generateSequence(*m_paramChain,
                                     *m_qoiChain);
  m_solutionRealizer = new uqSequentialVectorRealizerClass<Q_V,Q_M>((m_options.m_prefix+"Qoi").c_str(),
                                                                    *m_qoiChain);
  m_qoiRv.setRealizer(*m_solutionRealizer);

  // Compute output mdf: uniform sampling approach
  m_subMdfGrids  = new uqArrayOfOneDGridsClass <Q_V,Q_M>((m_options.m_prefix+"QoiMdf_").c_str(),m_qoiRv.imageSet().vectorSpace());
  m_subMdfValues = new uqArrayOfOneDTablesClass<Q_V,Q_M>((m_options.m_prefix+"QoiMdf_").c_str(),m_qoiRv.imageSet().vectorSpace());
  m_qoiChain->subUniformlySampledMdf(numEvaluationPointsVec, // input
                                     *m_subMdfGrids,         // output
                                     *m_subMdfValues);       // output

  m_subSolutionMdf = new uqSampledVectorMdfClass<Q_V,Q_M>((m_options.m_prefix+"Qoi").c_str(),
                                                          *m_subMdfGrids,
                                                          *m_subMdfValues);
  m_qoiRv.setMdf(*m_subSolutionMdf);

  // Compute output cdf: uniform sampling approach
  std::string subCoreName_qoiCdf(m_options.m_prefix+    "QoiCdf_");
  std::string uniCoreName_qoiCdf(m_options.m_prefix+"unifQoiCdf_");
  if (m_env.numSubEnvironments() == 1) subCoreName_qoiCdf = uniCoreName_qoiCdf;

  std::string subCoreName_solutionCdf(m_options.m_prefix+    "Qoi");
  std::string uniCoreName_solutionCdf(m_options.m_prefix+"unifQoi");
  if (m_env.numSubEnvironments() == 1) subCoreName_solutionCdf = uniCoreName_solutionCdf;

  m_subCdfGrids  = new uqArrayOfOneDGridsClass <Q_V,Q_M>(subCoreName_qoiCdf.c_str(),m_qoiRv.imageSet().vectorSpace());
  m_subCdfValues = new uqArrayOfOneDTablesClass<Q_V,Q_M>(subCoreName_qoiCdf.c_str(),m_qoiRv.imageSet().vectorSpace());
  m_qoiChain->subUniformlySampledCdf(numEvaluationPointsVec, // input
                                     *m_subCdfGrids,         // output
                                     *m_subCdfValues);       // output

  m_subSolutionCdf = new uqSampledVectorCdfClass<Q_V,Q_M>(subCoreName_solutionCdf.c_str(),
                                                          *m_subCdfGrids,
                                                          *m_subCdfValues);
  m_qoiRv.setSubCdf(*m_subSolutionCdf);

  // Compute unified cdf if necessary
  if (m_env.numSubEnvironments() == 1) {
    m_qoiRv.setUnifiedCdf(*m_subSolutionCdf);
  }
  else {
    m_unifiedCdfGrids  = new uqArrayOfOneDGridsClass <Q_V,Q_M>(uniCoreName_qoiCdf.c_str(),m_qoiRv.imageSet().vectorSpace());
    m_unifiedCdfValues = new uqArrayOfOneDTablesClass<Q_V,Q_M>(uniCoreName_qoiCdf.c_str(),m_qoiRv.imageSet().vectorSpace());
    m_qoiChain->unifiedUniformlySampledCdf(numEvaluationPointsVec, // input
                                           *m_unifiedCdfGrids,     // output
                                           *m_unifiedCdfValues);   // output

    m_unifiedSolutionCdf = new uqSampledVectorCdfClass<Q_V,Q_M>(uniCoreName_solutionCdf.c_str(),
                                                                *m_unifiedCdfGrids,
                                                                *m_unifiedCdfValues);
    m_qoiRv.setUnifiedCdf(*m_unifiedSolutionCdf);
  }

  // Compute (just unified one) covariance matrix, if requested
  // Compute (just unified one) correlation matrix, if requested
  P_M* pqCovarianceMatrix  = NULL;
  P_M* pqCorrelationMatrix = NULL;
  if (m_options.m_computeCovariances || m_options.m_computeCorrelations) {
    if (m_env.subDisplayFile()) {
      *m_env.subDisplayFile() << "In uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::solveWithMonteCarlo()"
                             << ", prefix = " << m_options.m_prefix
                             << ": instantiating cov and corr matrices"
                             << std::endl;
    }
    pqCovarianceMatrix = new P_M(m_env,
                                 m_paramRv.imageSet().vectorSpace().map(),       // number of rows
                                 m_qoiRv.imageSet().vectorSpace().dimGlobal());  // number of cols
    pqCorrelationMatrix = new P_M(m_env,
                                  m_paramRv.imageSet().vectorSpace().map(),      // number of rows
                                  m_qoiRv.imageSet().vectorSpace().dimGlobal()); // number of cols
#if 0
    uqComputeCovCorrMatricesBetweenVectorRvs<P_V,P_M,Q_V,Q_M>(m_paramRv,
                                                              m_qoiRv,
                                                              std::min(m_paramRv.realizer().subPeriod(),m_qoiRv.realizer().subPeriod()), // FIX ME: might be INFINITY
                                                              *pqCovarianceMatrix,
                                                              *pqCorrelationMatrix);
#else
    uqComputeCovCorrMatricesBetweenVectorSequences(*m_paramChain,
                                                   *m_qoiChain,
                                                   std::min(m_paramRv.realizer().subPeriod(),m_qoiRv.realizer().subPeriod()), // FIX ME: might be INFINITY
                                                   *pqCovarianceMatrix,
                                                   *pqCorrelationMatrix);
#endif
  }

  // Open data output file
  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << "In uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::solveWithMonteCarlo()"
                           << ", prefix = "                                        << m_options.m_prefix
                           << ": checking necessity of opening data output file '" << m_options.m_dataOutputFileName
                           << "'"
                           << std::endl;
  }
  std::ofstream* ofsvar = NULL;
  m_env.openOutputFile(m_options.m_dataOutputFileName,
                       "m",
                       m_options.m_dataOutputAllowedSet,
                       false,
                       ofsvar);

  // Write data out
  if (m_env.subDisplayFile()) {
    if (pqCovarianceMatrix ) {
      *m_env.subDisplayFile() << "In uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::solveWithMonteCarlo()"
                             << ", prefix = "                           << m_options.m_prefix
                             << ": contents of covariance matrix are\n" << *pqCovarianceMatrix
                             << std::endl;
    }
    if (pqCorrelationMatrix) {
      *m_env.subDisplayFile() << "In uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::solveWithMonteCarlo()"
                             << ", prefix = "                            << m_options.m_prefix
                             << ": contents of correlation matrix are\n" << *pqCorrelationMatrix
                             << std::endl;
    }
  }

  if (ofsvar) {
    m_qoiRv.mdf().print(*ofsvar);
    *ofsvar << m_qoiRv.subCdf();

    //if (pqCovarianceMatrix ) *ofsvar << *pqCovarianceMatrix;  // FIX ME: output matrix in matlab format
    //if (pqCorrelationMatrix) *ofsvar << *pqCorrelationMatrix; // FIX ME: output matrix in matlab format

    // Write unified cdf if necessary
    if (m_env.numSubEnvironments() > 1) {
      if (m_qoiRv.imageSet().vectorSpace().zeroVector().numberOfProcessorsRequiredForStorage() == 1) {
        if (m_env.inter0Rank() == 0) {
          *ofsvar << m_qoiRv.unifiedCdf(); //*m_unifiedSolutionCdf;
        }
      }
      else {
        UQ_FATAL_TEST_MACRO(true,
                            m_env.fullRank(),
                            "uqStatisticalForwardProblem<P_V,P_M,Q_V,Q_M>::solveWithMonteCarlo()",
                            "unifed cdf writing, parallel vectors not supported yet");
      }
    }
  }

  // Close data output file
  if (ofsvar) {
    ofsvar->close();
    delete ofsvar;
    if (m_env.subDisplayFile()) {
      *m_env.subDisplayFile() << "In uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::solveWithMonteCarlo()"
                             << ", prefix = "                 << m_options.m_prefix
                             << ": closed data output file '" << m_options.m_dataOutputFileName
                             << "'"
                             << std::endl;
    }
  }
  if (m_env.subDisplayFile()) {
    *m_env.subDisplayFile() << std::endl;
  }

  if (pqCovarianceMatrix ) delete pqCovarianceMatrix;
  if (pqCorrelationMatrix) delete pqCorrelationMatrix;

  m_env.syncPrintDebugMsg("Leaving uqStatisticalForwardProblemClass<P_V,P_M>::solveWithMonteCarlo()",1,3000000,m_env.fullComm());
  m_env.fullComm().Barrier();

  return;
}

template <class P_V,class P_M,class Q_V,class Q_M>
const uqGenericVectorRVClass<Q_V,Q_M>& 
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::qoiRv() const
{
  return m_qoiRv;
}

template <class P_V,class P_M,class Q_V,class Q_M>
const uqBaseVectorCdfClass<Q_V,Q_M>&
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::qoiRv_unifiedCdf() const
{
  if (m_env.numSubEnvironments() == 1) {
    return m_qoiRv.subCdf();
  }

  if (m_env.inter0Rank() < 0) {
    return m_qoiRv.subCdf();
  }

  //UQ_FATAL_TEST_MACRO(m_unifiedSolutionCdf == NULL,
  //                    m_env.fullRank(),
  //                    "uqStatisticalForwardProblem<P_V,P_M,Q_V,Q_M>::qoiRv_unifiedCdf()",
  //                    "variable is NULL");
  return m_qoiRv.unifiedCdf(); //*m_unifiedSolutionCdf;
}


template <class P_V,class P_M,class Q_V,class Q_M>
void
uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>::print(std::ostream& os) const
{
  return;
}

template<class P_V,class P_M,class Q_V,class Q_M>
std::ostream& operator<<(std::ostream& os, const uqStatisticalForwardProblemClass<P_V,P_M,Q_V,Q_M>& obj)
{
  obj.print(os);

  return os;
}
#endif // __UQ_SFP_H__
