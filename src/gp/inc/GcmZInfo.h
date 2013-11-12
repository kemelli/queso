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

#ifndef UQ_GCM_Z_INFO_H
#define UQ_GCM_Z_INFO_H

#include <queso/GcmSimulationInfo.h>
#include <queso/GcmExperimentInfo.h>

namespace QUESO {

template <class S_V,class S_M,class D_V,class D_M,class P_V,class P_M,class Q_V,class Q_M>
class GcmZInfo
{
public:
  // Case with no experiments
  GcmZInfo(bool                                                             formCMatrix,
                  bool                                                             allOutputsAreScalar,
                  const GcmSimulationInfo<S_V,S_M,P_V,P_M,Q_V,Q_M>&         s);
  // Case with experiments, and with vector outputs
  GcmZInfo(bool                                                             formCMatrix,
                  bool                                                             allOutputsAreScalar,
                  const GcmSimulationInfo<S_V,S_M,P_V,P_M,Q_V,Q_M>&         s,
                  const GcmExperimentInfo<S_V,S_M,D_V,D_M,P_V,P_M>&         e,
                  const GcmJointInfo     <S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>& jj);
  // Case with experiments, and with scalar outputs
  GcmZInfo(bool                                                             allOutputsAreScalar,
                  const GcmSimulationInfo<S_V,S_M,P_V,P_M,Q_V,Q_M>&         s,
                  const GcmExperimentInfo<S_V,S_M,D_V,D_M,P_V,P_M>&         e);
 ~GcmZInfo();

  const BaseEnvironment&     m_env;
        unsigned int                m_z_size;
        VectorSpace<D_V,D_M> m_z_space;
        D_V                         m_Zvec_hat;

        D_M*                        m_Cmat; // to be deleted on destructor
        unsigned int                m_Cmat_rank;

        D_M                         m_tmp_Smat_z;
        D_M                         m_tmp_Smat_extra;
        D_M                         m_tmp_Smat_z_hat;
        D_M                         m_tmp_Smat_z_hat_inv;

private:
  void commonConstructor();
};

// Case with no experiments
template <class S_V,class S_M,class D_V,class D_M,class P_V,class P_M,class Q_V,class Q_M>
GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::GcmZInfo(
  bool                                                     formCMatrix,
  bool                                                     allOutputsAreScalar,
  const GcmSimulationInfo<S_V,S_M,P_V,P_M,Q_V,Q_M>& s)
  :
  m_env               (s.m_env),
  m_z_size            (s.m_w_size),
  m_z_space           (m_env, "z_", m_z_size, NULL),
  m_Zvec_hat          (m_z_space.zeroVector()),
  m_Cmat              (NULL), // to be deleted on destructor
  m_Cmat_rank         (0),
  m_tmp_Smat_z        (m_z_space.zeroVector()),
  m_tmp_Smat_extra    (m_z_space.zeroVector()),
  m_tmp_Smat_z_hat    (m_z_space.zeroVector()),
  m_tmp_Smat_z_hat_inv(m_z_space.zeroVector())
{
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 2)) {
    *m_env.subDisplayFile() << "Entering GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(1)"
                            << std::endl;
  }

  m_Zvec_hat = s.m_Zvec_hat_w;

  //********************************************************************************
  // Make checks
  //********************************************************************************
  UQ_FATAL_TEST_MACRO(m_z_space.dimLocal() != (s.m_paper_m * s.m_paper_p_eta),
                      m_env.worldRank(),
                      "GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(1)",
                      "incompatible calculations for 'z' vector size (1)");

  commonConstructor();

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 2)) {
    *m_env.subDisplayFile() << "Leaving GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(1)"
                            << std::endl;
  }
}

// Case with experiments, and with vector outputs
template <class S_V,class S_M,class D_V,class D_M,class P_V,class P_M,class Q_V,class Q_M>
GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::GcmZInfo(
  bool                                                             formCMatrix,
  bool                                                             allOutputsAreScalar,
  const GcmSimulationInfo<S_V,S_M,P_V,P_M,Q_V,Q_M        >& s,
  const GcmExperimentInfo<S_V,S_M,D_V,D_M,P_V,P_M        >& e,
  const GcmJointInfo     <S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>& jj)
  :
  m_env               (s.m_env),
  m_z_size            (s.m_w_size + e.m_v_size + jj.m_u_size),
  m_z_space           (m_env, "z_", m_z_size, NULL),
  m_Zvec_hat          (m_z_space.zeroVector()),
  m_Cmat              (NULL), // to be deleted on destructor
  m_Cmat_rank         (0),
  m_tmp_Smat_z        (m_z_space.zeroVector()),
  m_tmp_Smat_extra    (m_z_space.zeroVector()),
  m_tmp_Smat_z_hat    (m_z_space.zeroVector()),
  m_tmp_Smat_z_hat_inv(m_z_space.zeroVector())
{
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 2)) {
    *m_env.subDisplayFile() << "Entering GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(2)"
                            << std::endl;
  }

  UQ_FATAL_TEST_MACRO(allOutputsAreScalar == true,
                      m_env.worldRank(),
                      "GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(2)",
                      "'allOutputsAreScalar' should be false");

  m_Zvec_hat.cwSetConcatenated(jj.m_Zvec_hat_vu,s.m_Zvec_hat_w);

  if (formCMatrix) {
    //********************************************************************************
    // Form 'C' matrix
    //********************************************************************************
    m_Cmat      = new D_M(m_env,jj.m_omega_space.map(),m_z_size);
    m_Cmat_rank = std::min(m_Cmat->numRowsGlobal(),m_Cmat->numCols()); // Might be smaller

    //********************************************************************************
    // Compute 'C' matrix
    //********************************************************************************
    m_Cmat->cwSet(0.);
    m_Cmat->cwSet(                                    0,                               0,*jj.m_Bmat_with_permut);
    m_Cmat->cwSet(jj.m_Bmat_with_permut->numRowsLocal(),jj.m_Bmat_with_permut->numCols(),  s.m_Kmat            );

    m_Cmat_rank= m_Cmat->rank(0.,1.e-8 ); // todo: should be an option
    unsigned int cRank14    = m_Cmat->rank(0.,1.e-14);
    if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 3)) {
      *m_env.subDisplayFile() << "In GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(2)"
                              << ": m_Cmat.numRowsLocal() = "  << m_Cmat->numRowsLocal()
                              << ", m_Cmat.numCols() = "       << m_Cmat->numCols()
                              << ", m_Cmat.rank(0.,1.e-8) = "  << m_Cmat_rank
                              << ", m_Cmat.rank(0.,1.e-14) = " << cRank14
                              << std::endl;
    }

    UQ_FATAL_TEST_MACRO(m_Cmat_rank != (jj.m_Bmat_rank + s.m_Kmat_rank),
                        m_env.worldRank(),
                        "GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(2)",
                        "'m_Cmat_rank' should be the sum of 'B' and 'K' ranks");

    UQ_FATAL_TEST_MACRO(m_Cmat->numRowsLocal() <= m_Cmat->numCols(),
                        m_env.worldRank(),
                        "GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(2)",
                        "'m_Cmat' should be a 'vertical' rectangular matrix");

    UQ_FATAL_TEST_MACRO(m_Cmat->numCols() != m_z_space.dimLocal(),
                        m_env.worldRank(),
                        "GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(2)",
                        "'m_Cmat' has invalid numCols");

    UQ_FATAL_TEST_MACRO(m_Cmat_rank > m_Cmat->numCols(),
                        m_env.worldRank(),
                        "GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(2)",
                        "'m_Cmat' has invalid rank");
  }

  //********************************************************************************
  // Make checks
  //********************************************************************************
  UQ_FATAL_TEST_MACRO(m_z_space.dimLocal() != (e.m_paper_n * e.m_paper_p_delta + e.m_paper_n * s.m_paper_p_eta + s.m_paper_m * s.m_paper_p_eta),
                      m_env.worldRank(),
                      "GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(2)",
                      "incompatible calculations for 'z' vector size (2)");

  commonConstructor();

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 2)) {
    *m_env.subDisplayFile() << "Leaving GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(2)"
                            << std::endl;
  }
}

// Case with experiments, and with scalar outputs
template <class S_V,class S_M,class D_V,class D_M,class P_V,class P_M,class Q_V,class Q_M>
GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::GcmZInfo(
  bool                                                     allOutputsAreScalar,
  const GcmSimulationInfo<S_V,S_M,P_V,P_M,Q_V,Q_M>& s,
  const GcmExperimentInfo<S_V,S_M,D_V,D_M,P_V,P_M>& e)
  :
  m_env               (s.m_env),
  m_z_size            (s.m_w_size + e.m_v_size),
  m_z_space           (m_env, "z_", m_z_size, NULL),
  m_Zvec_hat          (m_z_space.zeroVector()),
  m_Cmat              (NULL), // to be deleted on destructor
  m_Cmat_rank         (0),
  m_tmp_Smat_z        (m_z_space.zeroVector()),
  m_tmp_Smat_extra    (m_z_space.zeroVector()),
  m_tmp_Smat_z_hat    (m_z_space.zeroVector()),
  m_tmp_Smat_z_hat_inv(m_z_space.zeroVector())
{
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 2)) {
    *m_env.subDisplayFile() << "Entering GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(3)"
                            << std::endl;
  }

  UQ_FATAL_TEST_MACRO(allOutputsAreScalar == false,
                      m_env.worldRank(),
                      "GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(3)",
                      "'allOutputsAreScalar' should be true");

  //m_Zvec_hat.cwSetConcatenated(e.m_Zvec_hat_v,s.m_Zvec_hat_w); // ppp

  //********************************************************************************
  // Make checks
  //********************************************************************************
  UQ_FATAL_TEST_MACRO(m_z_space.dimLocal() != (e.m_paper_n * e.m_paper_p_delta + e.m_paper_n * s.m_paper_p_eta + s.m_paper_m * s.m_paper_p_eta),
                      m_env.worldRank(),
                      "GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(3)",
                      "incompatible calculations for 'z' vector size (2)");

  commonConstructor();

  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 2)) {
    *m_env.subDisplayFile() << "Leaving GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::constructor(3)"
                            << std::endl;
  }
}

template <class S_V,class S_M,class D_V,class D_M,class P_V,class P_M,class Q_V,class Q_M>
GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::~GcmZInfo()
{
  delete m_Cmat; // to be deleted on destructor
}

template <class S_V,class S_M,class D_V,class D_M,class P_V,class P_M,class Q_V,class Q_M>
void
GcmZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::commonConstructor()
{
  //********************************************************************************
  // Display information
  //********************************************************************************
  if ((m_env.subDisplayFile()) && (m_env.displayVerbosity() >= 2)) {
    *m_env.subDisplayFile() << "In GcnZInfo<S_V,S_M,D_V,D_M,P_V,P_M,Q_V,Q_M>::commonConstructor()"
                            << "\n  'z' vector size = " << m_z_space.dimLocal() // = n * p_delta + n * p_eta + m * p_eta
                            << std::endl;
  }

  return;
}

}  // End namespace QUESO

#endif // UQ_GCM_Z_INFO_H
