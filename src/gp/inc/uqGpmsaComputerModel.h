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

#ifndef __UQ_GCM_H__
#define __UQ_GCM_H__

#include <uqGpmsaComputerModelOptions.h>
#include <uqExperimentStorage.h>
#include <uqExperimentModel.h>
#include <uqSimulationStorage.h>
#include <uqSimulationModel.h>
#include <uqGcmSimulationInfo.h>      // 1
#include <uqGcmExperimentInfo.h>      // 2
#include <uqGcmJointInfo.h>           // 3
#include <uqGcmZInfo.h>               // 4
#include <uqGcmTotalInfo.h>           // 5
#include <uqGcmSimulationTildeInfo.h> // 6
#include <uqGcmJointTildeInfo.h>      // 7
#include <uqGcmZTildeInfo.h>          // 8
#include <uqVectorRV.h>
#include <uqInstantiateIntersection.h>
#include <uqMiscellaneous.h>
#include <sys/time.h>

template <class S_V,class S_M,class D_V,class D_M,class P_V,class P_M,class Q_V,class Q_M>
class uqGpmsaComputerModelClass
{
public:
  //*******************************************************************************
  // The following routines are in uqGpmsaComputerModel2.h
  //*******************************************************************************

  uqGpmsaComputerModelClass(const char*                                              prefix,
                            const uqGcmOptionsValuesClass*                           alternativeOptionsValues, // dakota
                            const uqSimulationStorageClass<S_V,S_M,P_V,P_M,Q_V,Q_M>& simulationStorage,
                            const uqSimulationModelClass  <S_V,S_M,P_V,P_M,Q_V,Q_M>& simulationModel,
                            const uqExperimentStorageClass<S_V,S_M,D_V,D_M>*         experimentStorage,
                            const uqExperimentModelClass  <S_V,S_M,D_V,D_M>*         experimentModel,
                            const uqBaseVectorRVClass     <P_V,P_M>*                 thetaPriorRv);
 ~uqGpmsaComputerModelClass();

        void                             calibrateWithBayesMetropolisHastings     (const uqMhOptionsValuesClass* alternativeOptionsValues, // dakota
                                                                                   const P_V&                    totalInitialValues,
                                                                                   const P_M*                    totalInitialProposalCovMatrix);
        void                             calibrateWithLanlMcmc                    (const uqMhOptionsValuesClass* alternativeOptionsValues, // dakota
                                                                                   const P_V&                    totalInitialValues,
                                                                                   const P_M*                    totalInitialProposalCovMatrix);
        void                             calibrateWithBayesMLSampling             ();

        // This routine calls formSigma_z_hat()
        // This routine might call formSigma_z_tilde_hat() in the future
        // This routine calls fillR_formula2_for_Sigma_v_hat_v_asterisk()
        // This routine calls fillR_formula1_for_Sigma_u_hat_u_asterisk()
        // This routine calls fillR_formula1_for_Sigma_w_hat_u_asterisk()
	void                             predictVUsAtGridPoint                    (const S_V& newScenarioVec,
                                                                                   const P_V& newParameterVec,
                                                                                         P_V& vuMeanVec,
                                                                                         P_M& vuCovMatrix,
                                                                                         P_V& vMeanVec,
                                                                                         P_M& vCovMatrix,
                                                                                         P_V& uMeanVec,
                                                                                         P_M& uCovMatrix);

        // This routine calls formSigma_w_hat()
        // This routine calls fillR_formula1_for_Sigma_w_hat_w_asterisk()
	void                             predictWsAtGridPoint                     (const S_V& newScenarioVec,
                                                                                   const P_V& newParameterVec,
                                                                                   const P_V* forcingSampleVecForDebug, // Usually NULL
                                                                                         P_V& wMeanVec,
                                                                                         P_M& wCovMatrix);
        void                             predictExperimentResults                 (const S_V& newScenarioVec,
                                                                                   const D_M& newKmat_interp,
                                                                                   const D_M& newDmat,
                                                                                         D_V& simulationOutputMeanVec,
                                                                                         D_V& discrepancyMeanVec);
        void                             predictSimulationOutputs                 (const S_V& newScenarioVec,
                                                                                   const P_V& newParameterVec,
                                                                                         Q_V& simulationOutputMeanVec);

  //*******************************************************************************
  // The following routines are in uqGpmsaComputerModel3.h
  //*******************************************************************************

  const uqVectorSpaceClass    <P_V,P_M>& totalSpace                               () const;
  const uqVectorSpaceClass    <P_V,P_M>& unique_vu_space                          () const;
  const uqBaseVectorRVClass   <P_V,P_M>& totalPriorRv                             () const;
  const uqGenericVectorRVClass<P_V,P_M>& totalPostRv                              () const;

        void                             print                                    (std::ostream& os) const;

private:
        void                             memoryCheck                              (unsigned int codePositionId);
        void                             generatePriorSeq                         ();
 static double                           staticLikelihoodRoutine                  (const P_V&  totalValues,
                                                                                   const P_V*  totalDirection,
                                                                                   const void* functionDataPtr,
                                                                                         P_V*  gradVector,
                                                                                         P_M*  hessianMatrix,
                                                                                         P_V*  hessianEffect);

        // This routine calls      formSigma_z_hat()
        // This routine might call formSigma_z_tilde_hat() in the future
        double                           likelihoodRoutine                        (const P_V&  totalValues,
                                                                                   const P_V*  totalDirection,
                                                                                   const void* functionDataPtr,
                                                                                         P_V*  gradVector,
                                                                                         P_M*  hessianMatrix,
                                                                                         P_V*  hessianEffect);

  //*******************************************************************************
  // The following routines are in uqGpmsaComputerModel4.h
  //*******************************************************************************

        // This routine is called by likelihoodRoutine()
        // This routine is called by predictVUsAtGridPoint()
        // This routine calls formSigma_z()
        void                             formSigma_z_hat                          (const P_V&                      input_1lambdaEtaVec, // ppp: does multiple Gs affect this routine?
                                                                                   const P_V&                      input_2lambdaWVec,
                                                                                   const P_V&                      input_3rhoWVec,
                                                                                   const P_V&                      input_4lambdaSVec,
                                                                                   const P_V&                      input_5lambdaYVec,   // ppp: what if there is no experimental data?
                                                                                   const P_V&                      input_6lambdaVVec,
                                                                                   const P_V&                      input_7rhoVVec,
                                                                                   const P_V&                      input_8thetaVec,
                                                                                         unsigned int              outerCounter);
        void                             formSigma_z_hat                          (const P_V&                      input_1lambdaEtaVec, // ppp: does multiple Gs affect this routine?
                                                                                   const P_V&                      input_2lambdaWVec,
                                                                                   const P_V&                      input_3rhoWVec,
                                                                                   const P_V&                      input_4lambdaSVec,
                                                                                         unsigned int              outerCounter);

        // This routine is called by likelihoodRoutine()
        // This routine might be called by predictVUsAtGridPoint() in the future
        // This routine calls formSigma_z()
        void                             formSigma_z_tilde_hat                    (const P_V&                      input_1lambdaEtaVec, // ppp: does multiple Gs affect this routine?
                                                                                   const P_V&                      input_2lambdaWVec,
                                                                                   const P_V&                      input_3rhoWVec,
                                                                                   const P_V&                      input_4lambdaSVec,
                                                                                   const P_V&                      input_5lambdaYVec,   // ppp: what if there is no experimental data?
                                                                                   const P_V&                      input_6lambdaVVec,
                                                                                   const P_V&                      input_7rhoVVec,
                                                                                   const P_V&                      input_8thetaVec,
                                                                                         unsigned int              outerCounter);

        // This routine is called by formSigma_z_hat()
        // This routine is called by formSigma_z_tilde_hat()
        // This routine calls fillR_formula2_for_Sigma_v ()
        // This routine calls fillR_formula1_for_Sigma_u ()
        // This routine calls fillR_formula1_for_Sigma_w ()
        // This routine calls fillR_formula1_for_Sigma_uw()
        void                             formSigma_z                              (const P_V&                      input_2lambdaWVec, // ppp: does multiple Gs affect this routine?
                                                                                   const P_V&                      input_3rhoWVec,
                                                                                   const P_V&                      input_4lambdaSVec,
                                                                                   const P_V&                      input_6lambdaVVec, // ppp: what if there is no experimental data?
                                                                                   const P_V&                      input_7rhoVVec,
                                                                                   const P_V&                      input_8thetaVec,
                                                                                         unsigned int              outerCounter);
        void                             formSigma_z                              (const P_V&                      input_2lambdaWVec, // ppp: does multiple Gs affect this routine?
                                                                                   const P_V&                      input_3rhoWVec,
                                                                                   const P_V&                      input_4lambdaSVec,
                                                                                         unsigned int              outerCounter);

        // This routine calls fillR_formula1_for_Sigma_w()
        void                             formSigma_w_hat                          (const P_V&                      input_1lambdaEtaVec,
                                                                                   const P_V&                      input_2lambdaWVec,
                                                                                   const P_V&                      input_3rhoWVec,
                                                                                   const P_V&                      input_4lambdaSVec,
                                                                                   const P_V&                      input_8thetaVec,
                                                                                         unsigned int              outerCounter);

  //*******************************************************************************
  // The following routines are in uqGpmsaComputerModel5.h
  //*******************************************************************************
        void                             fillR_formula2_for_Sigma_v               (const std::vector<const S_V* >& xVecs, // ppp: does multiple Gs affect this routine?
                                                                                   const P_V&                      rho_v_vec,
                                                                                         D_M&                      Rmat,
                                                                                         unsigned int              outerCounter);
        void                             fillR_formula1_for_Sigma_u               (const std::vector<const S_V* >& xVecs, // ppp: does multiple Gs affect this routine?
                                                                                   const P_V&                      tVec,
                                                                                   const P_V&                      rho_w_vec,
                                                                                         D_M&                      Rmat,
                                                                                         unsigned int              outerCounter);
 
        // This routine is called by formSigma_z()
        // This routine is called by formSigma_w_hat()
        void                             fillR_formula1_for_Sigma_w               (const std::vector<const S_V* >& xVecs, // ppp: does multiple Gs affect this routine?
                                                                                   const std::vector<const P_V* >& tVecs,
                                                                                   const P_V&                      rho_w_vec,
                                                                                         D_M&                      Rmat,
                                                                                         unsigned int              outerCounter);
        void                             fillR_formula1_for_Sigma_uw              (const std::vector<const S_V* >& xVecs1,
                                                                                   const P_V&                      tVec1,
                                                                                   const std::vector<const S_V* >& xVecs2,
                                                                                   const std::vector<const P_V* >& tVecs2,
                                                                                   const P_V&                      rho_w_vec,
                                                                                         D_M&                      Rmat,
                                                                                         unsigned int              outerCounter);

        void                             fillR_formula2_for_Sigma_v_hat_v_asterisk(const std::vector<const S_V* >& xVecs1,
                                                                                   const std::vector<const P_V* >& tVecs1,
                                                                                   const S_V&                      xVec2,
                                                                                   const P_V&                      tVec2,
                                                                                   const P_V&                      rho_v_vec,
                                                                                         D_M&                      Rmat,
                                                                                         unsigned int              outerCounter);
        void                             fillR_formula1_for_Sigma_u_hat_u_asterisk(const std::vector<const S_V* >& xVecs1,
                                                                                   const std::vector<const P_V* >& tVecs1,
                                                                                   const S_V&                      xVec2,
                                                                                   const P_V&                      tVec2,
                                                                                   const P_V&                      rho_w_vec,
                                                                                         D_M&                      Rmat,
                                                                                         unsigned int              outerCounter);
        void                             fillR_formula1_for_Sigma_w_hat_u_asterisk(const std::vector<const S_V* >& xVecs1,
                                                                                   const std::vector<const P_V* >& tVecs1,
                                                                                   const S_V&                      xVec2,
                                                                                   const P_V&                      tVec2,
                                                                                   const P_V&                      rho_w_vec,
                                                                                         D_M&                      Rmat,
                                                                                         unsigned int              outerCounter);
        void                             fillR_formula1_for_Sigma_w_hat_w_asterisk(const std::vector<const S_V* >& xVecs1,
                                                                                   const std::vector<const P_V* >& tVecs1,
                                                                                   const S_V&                      xVec2,
                                                                                   const P_V&                      tVec2,
                                                                                   const P_V&                      rho_w_vec,
                                                                                         D_M&                      Rmat,
                                                                                         unsigned int              outerCounter);

  const uqBaseEnvironmentClass&                                         m_env;
        uqGcmOptionsValuesClass                                         m_alternativeOptionsValues;
        uqGpmsaComputerModelOptionsClass*                               m_optionsObj;
        uqFilePtrSetStruct                                              m_dataOutputFilePtrSet;

        uqGcmSimulationInfoClass     <S_V,S_M,P_V,P_M,Q_V,Q_M        >* m_s;
        uqGcmExperimentInfoClass     <S_V,S_M,D_V,D_M,P_V,P_M        >* m_e;
        uqGcmJointInfoClass          <S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>* m_j;
        uqGcmZInfoClass              <S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>* m_z;
        uqGcmTotalInfoClass          <S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>* m_t;
        uqGcmSimulationTildeInfoClass<S_V,S_M,P_V,P_M,Q_V,Q_M        >* m_st;
        uqGcmJointTildeInfoClass     <S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>* m_jt;
        uqGcmZTildeInfoClass         <S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>* m_zt;
        bool                                                            m_thereIsExperimentalData;
        bool                                                            m_allOutputsAreScalar;
        bool                                                            m_formCMatrix;
        bool                                                            m_cMatIsRankDefficient;
        uqBaseScalarFunctionClass    <P_V,P_M>*                         m_likelihoodFunction;
        unsigned int                                                    m_like_counter;

};

template<class S_V,class S_M,class D_V,class D_M,class P_V,class P_M,class Q_V,class Q_M>
std::ostream& operator<<(std::ostream& os, const uqGpmsaComputerModelClass<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>& obj);

template<class S_V,class S_M,class D_V,class D_M,class P_V,class P_M,class Q_V,class Q_M>
std::ostream& operator<<(std::ostream& os, const uqGpmsaComputerModelClass<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>& obj)
{
  obj.print(os);

  return os;
}

#include <uqGpmsaComputerModel2.h>
#include <uqGpmsaComputerModel3.h>
#include <uqGpmsaComputerModel4.h>
#include <uqGpmsaComputerModel5.h>

#endif // __UQ_GCM_H__
