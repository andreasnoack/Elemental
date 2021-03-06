/*
   Copyright (c) 2009-2015, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef EL_LU_FULL_HPP
#define EL_LU_FULL_HPP

namespace El {
namespace lu {

template<typename F>
inline void
Full( Matrix<F>& A, Matrix<Int>& p, Matrix<Int>& q )
{
    DEBUG_ONLY(CSE cse("lu::Full"))
    const Int m = A.Height();
    const Int n = A.Width();
    const Int minDim = Min(m,n);

    // Initialize the permutations P and Q
    p.Resize( m, 1 );
    for( Int i=0; i<m; ++i )
        p.Set( i, 0, i );
    q.Resize( n, 1 );
    for( Int j=0; j<n; ++j )
        q.Set( j, 0, j );

    for( Int k=0; k<minDim; ++k )
    {
        const IR ind1( k ), ind2( k+1, END ), indB( k, END ), indR( k, END );

        // Find the index and value of the pivot candidate
        auto ABR = A( indB, indR );
        auto pivot = MaxAbs( ABR );
        const Int iPiv = pivot.i + k;
        const Int jPiv = pivot.j + k;

        RowSwap( A, k, iPiv );
        RowSwap( p, k, iPiv );

        ColSwap( A, k, jPiv );
        RowSwap( q, k, jPiv );

        // Now we can perform the update of the current panel
        const F alpha11 = A.Get(k,k);
        auto a21 = A( ind2, ind1 );
        auto a12 = A( ind1, ind2 );
        auto A22 = A( ind2, ind2 );
        if( alpha11 == F(0) )
            throw SingularMatrixException();
        const F alpha11Inv = F(1) / alpha11;
        a21 *= alpha11Inv;
        Geru( F(-1), a21, a12, A22 );
    }
}

template<typename F>
inline void
Full
( ElementalMatrix<F>& APre, 
  ElementalMatrix<Int>& p, ElementalMatrix<Int>& q )
{
    DEBUG_ONLY(
        CSE cse("lu::Full");
        AssertSameGrids( APre, p, q );
    )
    const Int m = APre.Height();
    const Int n = APre.Width();
    const Int minDim = Min(m,n);

    auto APtr = ReadWriteProxy<F,MC,MR>( &APre );
    auto& A = *APtr;

    // Initialize the permutations P and Q
    p.Resize( m, 1 );
    q.Resize( n, 1 );
    if( p.IsLocalCol(0) )
        for( Int iLoc=0; iLoc<p.LocalHeight(); ++iLoc )
            p.SetLocal( iLoc, 0, p.GlobalRow(iLoc) );
    if( q.IsLocalCol(0) )
        for( Int jLoc=0; jLoc<q.LocalHeight(); ++jLoc )
            q.SetLocal( jLoc, 0, q.GlobalRow(jLoc) );

    for( Int k=0; k<minDim; ++k )
    {
        const IR ind1( k ), ind2( k+1, END ), indB( k, END ), indR( k, END );

        // Find the index and value of the pivot candidate
        auto ABR = A( indB, indR );
        auto pivot = MaxAbs( ABR );
        const Int iPiv = pivot.i + k;
        const Int jPiv = pivot.j + k;

        RowSwap( A, iPiv, k );
        RowSwap( p, iPiv, k );

        ColSwap( A, jPiv, k );
        RowSwap( q, jPiv, k );

        // Now we can perform the update of the current panel
        const F alpha11 = A.Get(k,k);
        auto a21 = A( ind2, ind1 );
        auto a12 = A( ind1, ind2 );
        auto A22 = A( ind2, ind2 );
        if( alpha11 == F(0) )
            throw SingularMatrixException();
        const F alpha11Inv = F(1) / alpha11;
        a21 *= alpha11Inv;
        Geru( F(-1), a21, a12, A22 );
    }
}

} // namespace lu
} // namespace El

#endif // ifndef EL_LU_FULL_HPP
