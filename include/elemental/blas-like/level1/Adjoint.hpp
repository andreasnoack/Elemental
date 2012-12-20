/*
   Copyright (c) 2009-2012, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/

namespace elem {

template<typename T>
inline void
Adjoint( const Matrix<T>& A, Matrix<T>& B )
{
#ifndef RELEASE
    PushCallStack("Adjoint");
#endif
    const int m = A.Height();
    const int n = A.Width();
    if( B.Viewing() )
    {
        if( B.Height() != n || B.Width() != m )
        {
            std::ostringstream msg;
            msg << "If Adjoint'ing into a view, it must be the right size:\n"
                << "  A ~ " << A.Height() << " x " << A.Width() << "\n"
                << "  B ~ " << B.Height() << " x " << B.Width();
            throw std::logic_error( msg.str().c_str() );
        }
    }
    else
        B.ResizeTo( n, m );

    for( int j=0; j<n; ++j )
        for( int i=0; i<m; ++i )
            B.Set(j,i,Conj(A.Get(i,j)));
#ifndef RELEASE
    PopCallStack();
#endif
}

template<typename T,Distribution U,Distribution V,
                    Distribution W,Distribution Z>
inline void
Adjoint( const DistMatrix<T,U,V>& A, DistMatrix<T,W,Z>& B )
{
#ifndef RELEASE
    PushCallStack("Adjoint");
#endif
    if( B.Viewing() )
    {
        if( A.Height() != B.Width() || A.Width() != B.Height() )
        {
            std::ostringstream msg;
            msg << "If Adjoint'ing into a view, it must be the right size:\n"
                << "  A ~ " << A.Height() << " x " << A.Width() << "\n"
                << "  B ~ " << B.Height() << " x " << B.Width();
            throw std::logic_error( msg.str().c_str() );
        }
    }

    if( U == Z && V == W &&
        A.ColAlignment() == B.RowAlignment() &&
        A.RowAlignment() == B.ColAlignment() )
    {
        Adjoint( A.LockedLocalMatrix(), B.LocalMatrix() );
    }
    else
    {
        DistMatrix<T,Z,W> C( B.Grid() );
        if( B.Viewing() || B.ConstrainedColAlignment() )
            C.AlignRowsWith( B );
        if( B.Viewing() || B.ConstrainedRowAlignment() )
            C.AlignColsWith( B );
        C = A;

        if( !B.Viewing() )
        {
            if( !B.ConstrainedColAlignment() )
                B.AlignColsWith( C );
            if( !B.ConstrainedRowAlignment() )
                B.AlignRowsWith( C );
            B.ResizeTo( A.Width(), A.Height() );
        }
        Adjoint( C.LockedLocalMatrix(), B.LocalMatrix() );
    }
#ifndef RELEASE
    PopCallStack();
#endif
}

} // namespace elem