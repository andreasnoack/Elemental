/*
   Copyright (c) 2009-2015, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#include "El.hpp"

namespace El {

template<typename T,typename S>
void ShiftDiagonal( Matrix<T>& A, S alpha, Int offset )
{
    DEBUG_ONLY(CSE cse("ShiftDiagonal"))
    const Int height = A.Height();
    const Int width = A.Width();
    for( Int j=0; j<width; ++j )
    {
        const Int i = j-offset;
        if( i >= 0 && i < height )
            A.Update(i,j,alpha);
    }
}

template<typename T,typename S>
void ShiftDiagonal( AbstractDistMatrix<T>& A, S alpha, Int offset )
{
    DEBUG_ONLY(CSE cse("ShiftDiagonal"))
    const Int height = A.Height();
    const Int localWidth = A.LocalWidth();
    for( Int jLoc=0; jLoc<localWidth; ++jLoc )
    {
        const Int j = A.GlobalCol(jLoc);
        const Int i = j-offset;
        if( i >= 0 && i < height && A.IsLocalRow(i) )
        {
            const Int iLoc = A.LocalRow(i);
            A.UpdateLocal( iLoc, jLoc, alpha );
        }
    }
}

template<typename T,typename S>
void ShiftDiagonal
( SparseMatrix<T>& A, S alphaPre, Int offset, bool existingDiag )
{
    DEBUG_ONLY(CSE cse("ShiftDiagonal"))
    const Int m = A.Height();
    const Int n = A.Width();
    const T alpha = T(alphaPre);
    if( existingDiag )
    {
        T* valBuf = A.ValueBuffer();
        for( Int i=Max(0,-offset); i<Min(m,n-offset); ++i )
        {
            const Int e = A.Offset( i, i+offset );
            valBuf[e] += alpha;
        } 
    }
    else
    {
        const Int diagLength = Min(m,n-offset) - Max(0,-offset);
        A.Reserve( A.Capacity()+diagLength );
        for( Int i=Max(0,-offset); i<Min(m,n-offset); ++i )
            A.QueueUpdate( i, i+offset, alpha );
        A.ProcessQueues();
    }
}

template<typename T,typename S>
void ShiftDiagonal
( DistSparseMatrix<T>& A, S alphaPre, Int offset, bool existingDiag )
{
    DEBUG_ONLY(CSE cse("ShiftDiagonal"))
    const Int mLocal = A.LocalHeight();
    const Int n = A.Width();
    const T alpha = T(alphaPre);
    if( existingDiag ) 
    {
        T* valBuf = A.ValueBuffer();
        for( Int iLoc=0; iLoc<mLocal; ++iLoc )
        {
            const Int i = A.GlobalRow(iLoc);
            const Int e = A.Offset( iLoc, i+offset );
            valBuf[e] += alpha;
        } 
    }
    else
    {
        A.Reserve( A.Capacity()+mLocal );
        for( Int iLoc=0; iLoc<mLocal; ++iLoc )
        {
            const Int i = A.GlobalRow(iLoc);
            if( i+offset >= 0 && i+offset < n )
                A.QueueLocalUpdate( iLoc, i+offset, alpha );
        }
        A.ProcessLocalQueues();
    }
}

#define PROTO_TYPES(T,S) \
  template void ShiftDiagonal( Matrix<T>& A, S alpha, Int offset ); \
  template void ShiftDiagonal \
  ( AbstractDistMatrix<T>& A, S alpha, Int offset ); \
  template void ShiftDiagonal \
  ( SparseMatrix<T>& A, S alpha, Int offset, bool existingDiag ); \
  template void ShiftDiagonal \
  ( DistSparseMatrix<T>& A, S alpha, Int offset, bool existingDiag ); 

#define PROTO_SAME(T) PROTO_TYPES(T,T) \

#define PROTO_INT(T) PROTO_SAME(T)

#define PROTO_REAL(T) \
  PROTO_TYPES(T,Int) \
  PROTO_SAME(T)

#define PROTO_COMPLEX(T) \
  PROTO_TYPES(T,Int) \
  PROTO_TYPES(T,Base<T>) \
  PROTO_SAME(T)

#define EL_ENABLE_QUAD
#include "El/macros/Instantiate.h"

} // namespace El
