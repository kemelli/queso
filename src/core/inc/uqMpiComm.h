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

#ifndef __UQ_MPI_COMM_H__
#define __UQ_MPI_COMM_H__

#include <uqDefines.h>

#ifdef QUESO_HAS_TRILINOS
#include <Epetra_MpiComm.h>
#endif

#ifdef QUESO_HAS_MPI

#include <mpi.h>
typedef MPI_Comm     uqRawType_MPI_Comm ;
typedef MPI_Group    uqRawType_MPI_Group ;
typedef MPI_Datatype uqRawType_MPI_Datatype ;
typedef MPI_Op       uqRawType_MPI_Op ;
typedef MPI_Status   uqRawType_MPI_Status ;
#define uqRawValue_MPI_COMM_SELF  MPI_COMM_SELF 
#define uqRawValue_MPI_IN_PLACE   MPI_IN_PLACE
#define uqRawValue_MPI_ANY_SOURCE MPI_ANY_SOURCE
#define uqRawValue_MPI_CHAR       MPI_CHAR
#define uqRawValue_MPI_INT        MPI_INT
#define uqRawValue_MPI_DOUBLE     MPI_DOUBLE
#define uqRawValue_MPI_UNSIGNED   MPI_UNSIGNED
#define uqRawValue_MPI_MIN        MPI_MIN
#define uqRawValue_MPI_MAX        MPI_MAX
#define uqRawValue_MPI_SUM        MPI_SUM

#else // QUESO_HAS_MPI

typedef int uqRawType_MPI_Comm ;
typedef int uqRawType_MPI_Group ;
typedef int uqRawType_MPI_Datatype ;
typedef int uqRawType_MPI_Op ;
typedef int uqRawType_MPI_Status ;
#define uqRawValue_MPI_COMM_SELF  0
#define uqRawValue_MPI_IN_PLACE   0
#define uqRawValue_MPI_ANY_SOURCE -1
#define uqRawValue_MPI_CHAR       0
#define uqRawValue_MPI_INT        1
#define uqRawValue_MPI_DOUBLE     2
#define uqRawValue_MPI_UNSIGNED   3
#define uqRawValue_MPI_MIN        0
#define uqRawValue_MPI_MAX        1
#define uqRawValue_MPI_SUM        2

#endif // QUESO_HAS_MPI



/*! \file uqMpiComm.h
    \brief MPI Communicator Class.
*/

/*! \class uqMpiCommClass
    \brief The QUESO MPI Communicator Class.
    
    This class uses MPI (the Message Passing Interface) for distributed-memory 
    communication between one or more parallel processes. It is meant to insulate 
    the user from the specifics of communication that are not required for normal 
    manipulation of linear algebra objects. 
*/


class uqBaseEnvironmentClass;

class uqMpiCommClass
{
public:
   //! @name Constructor/Destructor methods
  //@{ 

  //! Default Constructor
  /*! It should not be used by user.*/
  uqMpiCommClass();
  
  //! QUESO uqMpiComm MPI Constructor.
  /*! This constructs an MpiComm that uses the given "raw" MPI communicator underneath. 
   * The MPI_Comm must be valid for the lifetime of this MpiComm.*/
  uqMpiCommClass(const uqBaseEnvironmentClass& env, uqRawType_MPI_Comm inputRawComm);
  
  //! Copy Constructor.
  /** Makes an exact copy of an existing uqMpiComm instance.*/
  uqMpiCommClass(const uqMpiCommClass& src);
  
  //! Destructor
 ~uqMpiCommClass();
  //@}
 
  //! @name Set methods
  //@{
  //! Assignment operator. 
  uqMpiCommClass& operator= (const uqMpiCommClass& rhs);
  //@}
  
  
  //! @name Attribute Accessor Methods
  //@{
  //! Extract MPI Communicator from a uqMpiComm object.  
  uqRawType_MPI_Comm Comm     () const;
  
  //! Return my process ID. 
  int                MyPID    () const;
  
  //! Returns total number of processes. 
  int                NumProc  () const;
  //@}
  
  //! @name Methods Overridden from Comm
  //@{ 
  //! Combines values from all processes and distributes the result back to all processes  
  /*! \param sendbuf starting address of send buffer
   * \param count number of elements in send buffer
   * \param datatype data type of elements of send buffer
   * \param op operation
   * \param recvbuf (output) starting address of receive buffer*/
  void               Allreduce(void* sendbuf, void* recvbuf, int count, uqRawType_MPI_Datatype datatype, 
			       uqRawType_MPI_Op op, const char* whereMsg, const char* whatMsg) const;
			       
  //! Pause every process in *this communicator until all the processes reach this point. 
  /*! Blocks the caller until all processes in the communicator have called it; that is, 
   * the call returns at any process only after all members of the communicator have entered the call.*/			       
  void               Barrier  () const; // const char* whereMsg, const char* whatMsg) const;
  
  //! Broadcast values from the root process to the slave processes. 
  /*! Broadcasts a message from the process with rank "root" to all other processes of the communicator.
   * \param buffer (input/output) starting address of buffer
   * \param count number of entries in buffer
   * \param datatype data type of buffer
   * \param root rank of broadcast root */
  void               Bcast    (void* buffer, int count, uqRawType_MPI_Datatype datatype, int root,
                               const char* whereMsg, const char* whatMsg) const;
			       
  //! Gather values from each process to collect on all processes. 
  /*!\param sendbuf starting address of send buffer 
   * \param sendcnt number of elements in send buffer 
   * \param sendtype data type of send buffer elements
   * \param recvcount number of elements for any single receive 
   * \param recvtype data type of recv buffer elements 
   * \param root rank of receiving process 
   * \param recvbuf (output) address of receive buffer */			       
  void               Gather   (void *sendbuf, int sendcnt, uqRawType_MPI_Datatype sendtype, 
                               void *recvbuf, int recvcount, uqRawType_MPI_Datatype recvtype, 
                               int root,
                               const char* whereMsg, const char* whatMsg) const;
			       
 //! Gathers into specified locations from all processes in a group
 /*! \param sendbuf starting address of send buffer
  * \param sendcount number of elements in send buffer 
  * \param sendtype data type of send buffer elements 
  * \param recvcounts integer array (of length group size) containing the number of elements 
  * that are received from each process
  * \param displs integer array (of length group size). Entry i specifies the displacement 
  * relative to recvbuf at which to place the incoming data from process i 
  * \param recvtype data type of recv buffer elements 
  * \param root rank of receiving process*/			       
  void               Gatherv  (void *sendbuf, int sendcnt, uqRawType_MPI_Datatype sendtype, 
                               void *recvbuf, int *recvcnts, int *displs, uqRawType_MPI_Datatype recvtype, 
                               int root,
                               const char* whereMsg, const char* whatMsg) const;
			       
  //! Blocking receive of data from this process to another process. 
  /*!\param buf (output) initial address of receive buffer
   * \param status (output) status object
   * \param count maximum number of elements in receive buffer
   * \param datatype datatype of each receive buffer element
   * \param source rank of source
   * \param tag message tag */			       
  void               Recv     (void *buf, int count, uqRawType_MPI_Datatype datatype, int source, int tag, uqRawType_MPI_Status *status,
                               const char* whereMsg, const char* whatMsg) const;
			       
  //! Possibly blocking send of data from this process to another process. 	
  /*!\param buf initial address of send buffer
   * \param count number of elements in send buffer
   * \param datatype datatype of each send buffer element
   * \param dest rank of destination
   * \param tag message tag*/		       
  void               Send     (void *buf, int count, uqRawType_MPI_Datatype datatype, int dest, int tag,
                               const char* whereMsg, const char* whatMsg) const;
 //@}
			       
//! @name Miscellaneous Methods
  //@{			       
  //! Synchronizes all the processes and print debug message.
  void               syncPrintDebugMsg(const char* msg, unsigned int msgVerbosity, unsigned int numUSecs) const;

#ifdef QUESO_HAS_TRILINOS
  //! Extract MPI Communicator from a Epetra_MpiComm object. 
  const Epetra_MpiComm& epetraMpiComm() const;
#endif
  
 //@}
private:
  //! Copies from an existing uqMpiComm instance.
  void               copy          (const uqMpiCommClass& src);
#ifdef QUESO_HAS_MPI
#else
  
  //! Calculates the size of each data type.
  size_t             sizeOfDataType(uqRawType_MPI_Datatype datatype, const char* whereMsg, const char* whatMsg) const;
#endif

  // QUESO environment 
  const uqBaseEnvironmentClass& m_env;
#ifdef QUESO_HAS_TRILINOS
  
  // Epetra MPI communicator
  Epetra_MpiComm*               m_epetraMpiComm;
#endif
  //! Embedded wrapped opaque MPI_Comm object.
  uqRawType_MPI_Comm            m_rawComm;
  
  //! World rank
  int                           m_worldRank;
  
  //! Process ID of this process
  int                           m_myPid;
  
  // Total number of processes
  int                           m_numProc;
};

#endif // __UQ_MPI_COMM_H__
