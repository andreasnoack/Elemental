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
void Hemm
( LeftOrRight side, UpperOrLower uplo,
  T alpha, const Matrix<T>& A, const Matrix<T>& B, T beta, Matrix<T>& C )
{
    DEBUG_ONLY(CSE cse("Hemm"))
    Symm( side, uplo, alpha, A, B, beta, C, true );
}

template<typename T>
void Hemm
( LeftOrRight side, UpperOrLower uplo,
  T alpha, const ElementalMatrix<T>& A, const ElementalMatrix<T>& B,
  T beta,        ElementalMatrix<T>& C )
{
    DEBUG_ONLY(CSE cse("Hemm"))
    Symm( side, uplo, alpha, A, B, beta, C, true );
}

#define PROTO(T) \
  template void Hemm \
  ( LeftOrRight side, UpperOrLower uplo, \
    T alpha, const Matrix<T>& A, const Matrix<T>& B, \
    T beta,        Matrix<T>& C ); \
  template void Hemm \
  ( LeftOrRight side, UpperOrLower uplo, \
    T alpha, const ElementalMatrix<T>& A, const ElementalMatrix<T>& B, \
    T beta,        ElementalMatrix<T>& C );

#define EL_ENABLE_QUAD
#include "El/macros/Instantiate.h"

} // namespace El
