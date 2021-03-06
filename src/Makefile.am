# Build in these directories:
# DIST_SUBDIRS = basic stats

AM_CPPFLAGS = -I$(top_srcdir)/src/core/inc \
	      -I$(top_srcdir)/src/misc/inc  \
	      -I$(top_srcdir)/src/basic/inc \
	      -I$(top_srcdir)/src/stats/inc \
	      -I$(top_srcdir)/src/gp/inc \
	      -I$(top_builddir)/src/core/inc \
	        $(BOOST_CPPFLAGS) \
	        $(GSL_CFLAGS) \
	        $(ANN_CFLAGS)

###	      -I$(top_srcdir)/src/interface 

if GRVY_ENABLED
  AM_CPPFLAGS += $(GRVY_CFLAGS)
endif

if GLPK_ENABLED	
  AM_CPPFLAGS += $(GLPK_CFLAGS)
endif

if HDF5_ENABLED	
  AM_CPPFLAGS += $(HDF5_CFLAGS) 
endif

if SVN_CHECKOUT
BUILT_SOURCES = .license.stamp
endif

# Dynamic library build: we include external library linkage here to
# make it easier for the user.

lib_LTLIBRARIES      = libqueso.la
libqueso_includedir  = $(prefix)/include
libqueso_la_LDFLAGS  = $(all_libraries) -release $(GENERIC_RELEASE)
libqueso_la_LDFLAGS += $(GSL_LIBS) 
libqueso_la_LDFLAGS += $(BOOST_PROGRAM_OPTIONS_LDFLAGS) $(BOOST_PROGRAM_OPTIONS_LIBS)
libqueso_la_LDFLAGS += $(ANN_LIBS)

if GRVY_ENABLED	
  libqueso_la_LDFLAGS += $(GRVY_LIBS)
endif

if GLPK_ENABLED	
  libqueso_la_LDFLAGS += $(GLPK_LIBS)
endif

if HDF5_ENABLED	
  libqueso_la_LDFLAGS += $(HDF5_LIBS)
endif

# TODO: cleanup way epetra is handled; it looks like it is being put
# in LDFLAGS directly. Would like to have a EPETRA_LIBS or some such
# variable later.

if TRILINOS_ENABLED
  libqueso_la_LDFLAGS += -lteuchoscore -lteuchoscomm -lteuchosnumerics -lteuchosparameterlist -lteuchosremainder -lepetra
endif

#libqueso_la_LDFLAGS += -Wl,-rpath,$(TRILINOS_HOME)/lib -lepetra

# Sources from core/src

libqueso_la_SOURCES = \
	$(top_srcdir)/src/core/src/uqDefines.C \
	$(top_srcdir)/src/core/src/uqEnvironment.C \
	$(top_srcdir)/src/core/src/uqEnvironmentOptions.C \
	$(top_srcdir)/src/core/src/uqRngBase.C \
	$(top_srcdir)/src/core/src/uqRngGsl.C \
	$(top_srcdir)/src/core/src/uqRngBoost.C \
	$(top_srcdir)/src/core/src/uqBasicPdfsBase.C \
	$(top_srcdir)/src/core/src/uqBasicPdfsGsl.C \
	$(top_srcdir)/src/core/src/uqBasicPdfsBoost.C \
	$(top_srcdir)/src/core/src/uqVector.C \
	$(top_srcdir)/src/core/src/uqMatrix.C \
	$(top_srcdir)/src/core/src/uqTeuchosVector.C \
	$(top_srcdir)/src/core/src/uqTeuchosMatrix.C \
	$(top_srcdir)/src/core/src/uqMpiComm.C \
	$(top_srcdir)/src/core/src/uqMap.C

# Sources from core/src with gsl conditional 

if UQBT_GSL
libqueso_la_SOURCES += \
	$(top_srcdir)/src/core/src/uqGslVector.C \
	$(top_srcdir)/src/core/src/uqGslMatrix.C
endif

# Sources from misc/src

libqueso_la_SOURCES += \
	$(top_srcdir)/src/misc/src/uq1D1DFunction.C \
	$(top_srcdir)/src/misc/src/uq1DQuadrature.C \
	$(top_srcdir)/src/misc/src/uqComplexFft.C \
	$(top_srcdir)/src/misc/src/uqMiscellaneous.C \
	$(top_srcdir)/src/misc/src/uqRealFft.C

# Sources from misc/src withn gsl conditional

if UQBT_GSL
  libqueso_la_SOURCES += $(top_srcdir)/src/misc/src/uqGslAsciiTable.C
endif

# Sources from basic/src

libqueso_la_SOURCES += \
	$(top_srcdir)/src/basic/src/uqTeuchosVectorSpace.C \
	$(top_srcdir)/src/basic/src/uqSequenceStatisticalOptions.C


# Sources from basic/src with gsl conditional

if UQBT_GSL
  libqueso_la_SOURCES += $(top_srcdir)/src/basic/src/uqGslVectorSpace.C
endif

# Sources from stats/src

libqueso_la_SOURCES += \
	$(top_srcdir)/src/stats/src/uqFiniteDistribution.C \
	$(top_srcdir)/src/stats/src/uqMetropolisHastingsSG.C \
	$(top_srcdir)/src/stats/src/uqMetropolisHastingsSGOptions.C \
	$(top_srcdir)/src/stats/src/uqMLSampling.C \
	$(top_srcdir)/src/stats/src/uqMLSamplingOptions.C \
	$(top_srcdir)/src/stats/src/uqMLSamplingLevelOptions.C \
	$(top_srcdir)/src/stats/src/uqMonteCarloSGOptions.C \
	$(top_srcdir)/src/stats/src/uqStatisticalInverseProblemOptions.C \
	$(top_srcdir)/src/stats/src/uqStatisticalForwardProblemOptions.C \
	$(top_srcdir)/src/stats/src/uqInfoTheory.C

# Sources from gp/src

libqueso_la_SOURCES += \
	$(top_srcdir)/src/gp/src/uqExperimentModelOptions.C \
	$(top_srcdir)/src/gp/src/uqGpmsaComputerModelOptions.C \
	$(top_srcdir)/src/gp/src/uqSimulationModelOptions.C

# Sources from interface 

###libqueso_la_SOURCES += \
###	$(top_srcdir)/src/interface/basic_classes.cpp \
###	$(top_srcdir)/src/interface/basic_interface.cpp

# Headers to install from core/inc

libqueso_include_HEADERS = \
	$(top_builddir)/src/core/inc/queso.h \
	$(top_srcdir)/src/core/inc/uqDefines.h \
	$(top_srcdir)/src/core/inc/uqEnvironment.h \
	$(top_srcdir)/src/core/inc/uqEnvironmentOptions.h \
	$(top_srcdir)/src/core/inc/uqRngBase.h \
	$(top_srcdir)/src/core/inc/uqRngGsl.h \
	$(top_srcdir)/src/core/inc/uqRngBoost.h \
	$(top_srcdir)/src/core/inc/uqBasicPdfsBase.h \
	$(top_srcdir)/src/core/inc/uqBasicPdfsGsl.h \
	$(top_srcdir)/src/core/inc/uqBasicPdfsBoost.h \
	$(top_srcdir)/src/core/inc/uqGslMatrix.h \
	$(top_srcdir)/src/core/inc/uqGslVector.h \
	$(top_srcdir)/src/core/inc/uqTeuchosMatrix.h \
	$(top_srcdir)/src/core/inc/uqTeuchosVector.h \
	$(top_srcdir)/src/core/inc/uqMatrix.h \
	$(top_srcdir)/src/core/inc/uqMpiComm.h \
	$(top_srcdir)/src/core/inc/uqMap.h \
	$(top_srcdir)/src/core/inc/uqDistArray.h \
	$(top_srcdir)/src/core/inc/uqVector.h

# Headers to install from misc/inc

libqueso_include_HEADERS += \
	$(top_srcdir)/src/misc/inc/uq1D1DFunction.h \
	$(top_srcdir)/src/misc/inc/uq1DQuadrature.h \
	$(top_srcdir)/src/misc/inc/uq2dArrayOfStuff.h \
	$(top_srcdir)/src/misc/inc/uqArrayOfOneDGrids.h \
	$(top_srcdir)/src/misc/inc/uqArrayOfOneDTables.h \
	$(top_srcdir)/src/misc/inc/uqAsciiTable.h \
	$(top_srcdir)/src/misc/inc/uqCovCond.h \
	$(top_srcdir)/src/misc/inc/uqFft.h \
	$(top_srcdir)/src/misc/inc/uqMiscellaneous.h \
	$(top_srcdir)/src/misc/inc/uqOneDGrid.h

# Headers to install from basic/inc

libqueso_include_HEADERS += \
	$(top_srcdir)/src/basic/inc/uqArrayOfSequences.h \
	$(top_srcdir)/src/basic/inc/uqInstantiateIntersection.h \
	$(top_srcdir)/src/basic/inc/uqScalarFunction.h \
	$(top_srcdir)/src/basic/inc/uqScalarFunctionSynchronizer.h \
	$(top_srcdir)/src/basic/inc/uqScalarSequence.h \
	$(top_srcdir)/src/basic/inc/uqSequenceOfVectors.h \
	$(top_srcdir)/src/basic/inc/uqSequenceStatisticalOptions.h \
	$(top_srcdir)/src/basic/inc/uqVectorFunction.h \
	$(top_srcdir)/src/basic/inc/uqVectorFunctionSynchronizer.h \
	$(top_srcdir)/src/basic/inc/uqVectorSequence.h \
	$(top_srcdir)/src/basic/inc/uqVectorSet.h \
	$(top_srcdir)/src/basic/inc/uqVectorSpace.h \
	$(top_srcdir)/src/basic/inc/uqVectorSubset.h

# Headers to install from stats/inc 

libqueso_include_HEADERS += \
	$(top_srcdir)/src/stats/inc/uqFiniteDistribution.h \
	$(top_srcdir)/src/stats/inc/uqJointPdf.h \
	$(top_srcdir)/src/stats/inc/uqMarkovChainPositionData.h \
	$(top_srcdir)/src/stats/inc/uqMetropolisHastingsSG1.h \
	$(top_srcdir)/src/stats/inc/uqMetropolisHastingsSG2.h \
	$(top_srcdir)/src/stats/inc/uqMetropolisHastingsSGOptions.h \
	$(top_srcdir)/src/stats/inc/uqMLSampling1.h \
	$(top_srcdir)/src/stats/inc/uqMLSampling2.h \
	$(top_srcdir)/src/stats/inc/uqMLSampling3.h \
	$(top_srcdir)/src/stats/inc/uqMLSamplingOptions.h \
	$(top_srcdir)/src/stats/inc/uqMLSamplingLevelOptions.h \
	$(top_srcdir)/src/stats/inc/uqModelValidation.h \
	$(top_srcdir)/src/stats/inc/uqMonteCarloSG.h \
	$(top_srcdir)/src/stats/inc/uqMonteCarloSGOptions.h \
	$(top_srcdir)/src/stats/inc/uqScalarCdf.h \
	$(top_srcdir)/src/stats/inc/uqStatisticalForwardProblem.h \
	$(top_srcdir)/src/stats/inc/uqStatisticalForwardProblemOptions.h \
	$(top_srcdir)/src/stats/inc/uqStatisticalInverseProblem.h \
	$(top_srcdir)/src/stats/inc/uqStatisticalInverseProblemOptions.h \
	$(top_srcdir)/src/stats/inc/uqTKGroup.h \
	$(top_srcdir)/src/stats/inc/uqValidationCycle.h \
	$(top_srcdir)/src/stats/inc/uqVectorCdf.h \
	$(top_srcdir)/src/stats/inc/uqVectorMdf.h \
	$(top_srcdir)/src/stats/inc/uqVectorRealizer.h \
	$(top_srcdir)/src/stats/inc/uqVectorRV.h \
	$(top_srcdir)/src/stats/inc/uqInfoTheory.h \
	$(top_srcdir)/src/stats/inc/uqScalarCovarianceFunction.h \
	$(top_srcdir)/src/stats/inc/uqMatrixCovarianceFunction.h \
	$(top_srcdir)/src/stats/inc/uqScalarGaussianRandomField.h \
	$(top_srcdir)/src/stats/inc/uqVectorGaussianRandomField.h

# Headers to install from gp/inc 

libqueso_include_HEADERS += \
	$(top_srcdir)/src/gp/inc/uqExperimentModel.h \
	$(top_srcdir)/src/gp/inc/uqExperimentModelOptions.h \
	$(top_srcdir)/src/gp/inc/uqExperimentStorage.h \
	$(top_srcdir)/src/gp/inc/uqGcmExperimentInfo.h \
	$(top_srcdir)/src/gp/inc/uqGcmJointInfo.h \
	$(top_srcdir)/src/gp/inc/uqGcmJointTildeInfo.h \
	$(top_srcdir)/src/gp/inc/uqGcmSimulationInfo.h \
	$(top_srcdir)/src/gp/inc/uqGcmSimulationTildeInfo.h \
	$(top_srcdir)/src/gp/inc/uqGcmTotalInfo.h \
	$(top_srcdir)/src/gp/inc/uqGcmZInfo.h \
	$(top_srcdir)/src/gp/inc/uqGcmZTildeInfo.h \
	$(top_srcdir)/src/gp/inc/uqGpmsaComputerModel.h \
	$(top_srcdir)/src/gp/inc/uqGpmsaComputerModel2.h \
	$(top_srcdir)/src/gp/inc/uqGpmsaComputerModel3.h \
	$(top_srcdir)/src/gp/inc/uqGpmsaComputerModel4.h \
	$(top_srcdir)/src/gp/inc/uqGpmsaComputerModel5.h \
	$(top_srcdir)/src/gp/inc/uqGpmsaComputerModelOptions.h \
	$(top_srcdir)/src/gp/inc/uqSimulationModel.h \
	$(top_srcdir)/src/gp/inc/uqSimulationModelOptions.h \
	$(top_srcdir)/src/gp/inc/uqSimulationStorage.h

# Headers to install, but not part of dist

nodist_libqueso_include_HEADERS    =       \
	$(top_builddir)/config_queso.h       \
	$(top_builddir)/src/core/inc/queso.h

# Headers to install from interface

###libqueso_include_HEADERS += \
###	$(top_srcdir)/src/interface/queso_basic.h     \
###	$(top_srcdir)/src/interface/basic_classes.h   \
###	$(top_srcdir)/src/interface/basic_interface.h \
###	$(top_srcdir)/src/interface/basic_int.h

# Standalone binaries

bin_PROGRAMS               = queso_version
queso_version_SOURCES      = $(top_srcdir)/src/core/src/version.C
queso_version_LDADD        = -L$(top_builddir)/src -lqueso $(HDF5_LIBS) 
if TRILINOS_ENABLED
  queso_version_LDADD     += -lteuchoscore -lteuchoscomm -lteuchosnumerics -lteuchosparameterlist -lteuchosremainder -lepetra
endif

queso_version_DEPENDENCIES = libqueso.la

if CODE_COVERAGE_ENABLED
  CLEANFILES = *.gcda *.gcno
endif

#---------------------------------
# Embedded license header support
#---------------------------------

if SVN_CHECKOUT
.license.stamp: $(top_srcdir)/LICENSE
	$(top_srcdir)/src/common/lic_utils/update_license.pl $(top_srcdir)/LICENSE \
	$(libqueso_include_HEADERS) $(libqueso_la_SOURCES) $(top_srcdir)/src/core/inc/queso.h
	echo 'updated source license headers' >$@
endif
