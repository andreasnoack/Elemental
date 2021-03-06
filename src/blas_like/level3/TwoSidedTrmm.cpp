/*
   Copyright (c) 2009-2015, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#include "El.hpp"

#include "./TwoSidedTrmm/Unblocked.hpp"
#include "./TwoSidedTrmm/LVar4.hpp"
#include "./TwoSidedTrmm/UVar4.hpp"

namespace El {

template<typename T> 
void TwoSidedTrmm
( UpperOrLower uplo, UnitOrNonUnit diag, Matrix<T>& A, const Matrix<T>& B )
{
    DEBUG_ONLY(CSE cse("TwoSidedTrmm"))
    if( uplo == LOWER )
        twotrmm::LVar4( diag, A, B );
    else
        twotrmm::UVar4( diag, A, B );
}

template<typename T> 
void TwoSidedTrmm
( UpperOrLower uplo, UnitOrNonUnit diag, 
  ElementalMatrix<T>& A, const ElementalMatrix<T>& B )
{
    DEBUG_ONLY(CSE cse("TwoSidedTrmm"))
    if( uplo == LOWER )
        twotrmm::LVar4( diag, A, B );
    else
        twotrmm::UVar4( diag, A, B );
}

template<typename T>
void TwoSidedTrmm
( UpperOrLower uplo, UnitOrNonUnit diag,
  DistMatrix<T,STAR,STAR>& A, const DistMatrix<T,STAR,STAR>& B )
{ TwoSidedTrmm( uplo, diag, A.Matrix(), B.LockedMatrix() ); }

#define PROTO(T) \
  template void TwoSidedTrmm \
  ( UpperOrLower uplo, UnitOrNonUnit diag, \
    Matrix<T>& A, const Matrix<T>& B ); \
  template void TwoSidedTrmm \
  ( UpperOrLower uplo, UnitOrNonUnit diag, \
    ElementalMatrix<T>& A, const ElementalMatrix<T>& B ); \
  template void TwoSidedTrmm \
  ( UpperOrLower uplo, UnitOrNonUnit diag, \
    DistMatrix<T,STAR,STAR>& A, const DistMatrix<T,STAR,STAR>& B );

#define EL_NO_INT_PROTO
#include "El/macros/Instantiate.h"

} // namespace El
