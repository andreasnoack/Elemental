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
void Her2
( UpperOrLower uplo,
  T alpha, const Matrix<T>& x, const Matrix<T>& y, Matrix<T>& A )
{
    DEBUG_ONLY(CSE cse("Her2"))
    Syr2( uplo, T(alpha), x, y, A, true );
}

template<typename T>
void Her2
( UpperOrLower uplo,
  T alpha, const ElementalMatrix<T>& x, const ElementalMatrix<T>& y,
                 ElementalMatrix<T>& A )
{
    DEBUG_ONLY(CSE cse("Her2"))
    Syr2( uplo, T(alpha), x, y, A, true );
}

#define PROTO(T) \
  template void Her2 \
  ( UpperOrLower uplo, T alpha, \
    const Matrix<T>& x, const Matrix<T>& y, Matrix<T>& A ); \
  template void Her2 \
  ( UpperOrLower uplo, T alpha, \
    const ElementalMatrix<T>& x, const ElementalMatrix<T>& y, \
          ElementalMatrix<T>& A );

#define EL_ENABLE_QUAD
#include "El/macros/Instantiate.h"

} // namespace El
