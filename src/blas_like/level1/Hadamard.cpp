/*
   Copyright (c) 2009-2015, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#include "El.hpp"

// C(i,j) := A(i,j) B(i,j)

namespace El {

template<typename T> 
void Hadamard( const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C )
{
    DEBUG_ONLY(CSE cse("Hadamard"))
    if( A.Height() != B.Height() || A.Width() != B.Width() )
        LogicError("Hadamard product requires equal dimensions");
    C.Resize( A.Height(), A.Width() );

    const Int height = A.Height();
    const Int width = A.Width();
    const T* ABuf = A.LockedBuffer();
    const T* BBuf = B.LockedBuffer();
          T* CBuf = C.Buffer();
    const Int ALDim = A.LDim();
    const Int BLDim = B.LDim();
    const Int CLDim = C.LDim();

    for( Int j=0; j<width; ++j )
        for( Int i=0; i<height; ++i )
            CBuf[i+j*CLDim] = ABuf[i+j*ALDim]*BBuf[i+j*BLDim];
}

template<typename T> 
void Hadamard
( const ElementalMatrix<T>& A,
  const ElementalMatrix<T>& B, 
        ElementalMatrix<T>& C )
{
    DEBUG_ONLY(CSE cse("Hadamard"))
    const ElementalData ADistData = A.DistData();
    const ElementalData BDistData = B.DistData();
    ElementalData CDistData = C.DistData();
    if( A.Height() != B.Height() || A.Width() != B.Width() )
        LogicError("Hadamard product requires equal dimensions");
    AssertSameGrids( A, B );
    if( ADistData.colDist != BDistData.colDist ||
        ADistData.rowDist != BDistData.rowDist ||
        BDistData.colDist != CDistData.colDist ||
        BDistData.rowDist != CDistData.rowDist )
        LogicError("A, B, and C must share the same distribution");
    if( A.ColAlign() != B.ColAlign() || A.RowAlign() != B.RowAlign() )
        LogicError("A and B must be aligned");
    C.AlignWith( A.DistData() );
    C.Resize( A.Height(), A.Width() );
    Hadamard( A.LockedMatrix(), B.LockedMatrix(), C.Matrix() );
}

template<typename T> 
void Hadamard
( const DistMultiVec<T>& A, const DistMultiVec<T>& B, DistMultiVec<T>& C )
{
    DEBUG_ONLY(CSE cse("Hadamard"))
    if( A.Height() != B.Height() || A.Width() != B.Width() )
        LogicError("Hadamard product requires equal dimensions");
    C.SetComm( A.Comm() );
    C.Resize( A.Height(), A.Width() );
    Hadamard( A.LockedMatrix(), B.LockedMatrix(), C.Matrix() );
}

#define PROTO(T) \
  template void Hadamard \
  ( const Matrix<T>& A, const Matrix<T>& B, Matrix<T>& C ); \
  template void Hadamard \
  ( const ElementalMatrix<T>& A, \
    const ElementalMatrix<T>& B, \
          ElementalMatrix<T>& C ); \
  template void Hadamard \
  ( const DistMultiVec<T>& A, \
    const DistMultiVec<T>& B, \
          DistMultiVec<T>& C );

#define EL_ENABLE_QUAD
#include "El/macros/Instantiate.h"

} // namespace El
