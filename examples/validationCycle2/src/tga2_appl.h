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

#ifndef __TGA2_APPL_H__
#define __TGA2_APPL_H__

#include <uqValidationCycle.h>
#include <uqGslMatrix.h>
#include <uqEnvironment.h>

void uqAppl(const uqBaseEnvironmentClass& env);
void uqAppl_LocalComparisonStage(uqValidationCycleClass<uqGslVectorClass,uqGslMatrixClass,uqGslVectorClass,uqGslMatrixClass>& cycle);
void uqAppl_UnifiedComparisonStage(uqValidationCycleClass<uqGslVectorClass,uqGslMatrixClass,uqGslVectorClass,uqGslMatrixClass>& cycle);

#endif // __TGA2_APPL_H__
