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
void MakeIdentity( Matrix<T>& I )
{
    DEBUG_ONLY(CSE cse("MakeIdentity"))
    Zero( I );
    FillDiagonal( I, T(1) );
}

template<typename T>
void MakeIdentity( AbstractDistMatrix<T>& I )
{
    DEBUG_ONLY(CSE cse("MakeIdentity"))
    Zero( I );
    FillDiagonal( I, T(1) );
}

template<typename T>
void Identity( Matrix<T>& I, Int m, Int n )
{
    DEBUG_ONLY(CSE cse("Identity"))
    I.Resize( m, n );
    MakeIdentity( I );
}

template<typename T>
void Identity( AbstractDistMatrix<T>& I, Int m, Int n )
{
    DEBUG_ONLY(CSE cse("Identity"))
    I.Resize( m, n );
    MakeIdentity( I );
}

template<typename T>
void Identity( SparseMatrix<T>& I, Int m, Int n )
{
    DEBUG_ONLY(CSE cse("Identity"))
    Zeros( I, m, n );
    I.Reserve( Min(m,n) );
    for( Int j=0; j<Min(m,n); ++j )
        I.QueueUpdate( j, j, T(1) );
    I.ProcessQueues();
}

template<typename T>
void Identity( DistSparseMatrix<T>& I, Int m, Int n )
{
    DEBUG_ONLY(CSE cse("Identity"))
    Zeros( I, m, n );
    const Int localHeight = I.LocalHeight();

    // We could reserve less for tall matrices, but this should suffice
    I.Reserve( localHeight );
   
    for( Int iLoc=0; iLoc<I.LocalHeight(); ++iLoc )
    {
        const Int i = I.GlobalRow(iLoc);
        if( i < n )
            I.QueueUpdate( i, i, T(1) );
    }
    I.ProcessQueues();
}

#define PROTO(T) \
  template void MakeIdentity( Matrix<T>& I ); \
  template void MakeIdentity( AbstractDistMatrix<T>& I ); \
  template void Identity( Matrix<T>& I, Int m, Int n ); \
  template void Identity( AbstractDistMatrix<T>& I, Int m, Int n ); \
  template void Identity( SparseMatrix<T>& I, Int m, Int n ); \
  template void Identity( DistSparseMatrix<T>& I, Int m, Int n );

#define EL_ENABLE_QUAD
#include "El/macros/Instantiate.h"

} // namespace El
