/*
   Copyright (c) 2009-2015, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#include "El.hpp"

namespace El {

template<typename T>
void Adjoint( const Matrix<T>& A, Matrix<T>& B )
{
    DEBUG_ONLY(CSE cse("Adjoint"))
    Transpose( A, B, true );
}

template<typename T>
void Adjoint( const ElementalMatrix<T>& A, ElementalMatrix<T>& B )
{
    DEBUG_ONLY(CSE cse("Adjoint"))
    Transpose( A, B, true );
}

template<typename T>
void Adjoint
( const BlockMatrix<T>& A, BlockMatrix<T>& B )
{
    DEBUG_ONLY(CSE cse("Adjoint"))
    Transpose( A, B, true );
}

template<typename T>
void Adjoint( const SparseMatrix<T>& A, SparseMatrix<T>& B )
{
    DEBUG_ONLY(CSE cse("Adjoint"))
    Transpose( A, B, true );
}

template<typename T>
void Adjoint( const DistSparseMatrix<T>& A, DistSparseMatrix<T>& B )
{
    DEBUG_ONLY(CSE cse("Adjoint"))
    Transpose( A, B, true );
}

#define PROTO(T) \
  template void Adjoint( const Matrix<T>& A, Matrix<T>& B ); \
  template void Adjoint \
  ( const ElementalMatrix<T>& A, ElementalMatrix<T>& B ); \
  template void Adjoint \
  ( const BlockMatrix<T>& A, BlockMatrix<T>& B ); \
  template void Adjoint( const SparseMatrix<T>& A, SparseMatrix<T>& B ); \
  template void Adjoint( const DistSparseMatrix<T>& A, DistSparseMatrix<T>& B );

#define EL_ENABLE_QUAD
#include "El/macros/Instantiate.h"

} // namespace El
