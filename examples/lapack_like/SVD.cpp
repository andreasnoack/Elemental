/*
   Copyright (c) 2009-2015, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#include "El.hpp"
using namespace std;
using namespace El;

// Typedef our real and complex types to 'Real' and 'C' for convenience
typedef double Real;
typedef Complex<Real> C;

int
main( int argc, char* argv[] )
{
    Initialize( argc, argv );

    try 
    {
        const Int m = Input("--height","height of matrix",100);
        const Int n = Input("--width","width of matrix",100);
        const Int blocksize = Input("--blocksize","algorithmic blocksize",32);
#ifdef EL_HAVE_SCALAPACK
        const Int mb = Input("--mb","block height",32);
        const Int nb = Input("--nb","block width",32);
#endif
        const bool testSeq = Input("--testSeq","test sequential SVD?",false);
        const bool testDecomp = Input("--testDecomp","test full SVD?",true);
        const bool print = Input("--print","print matrices?",false);
        ProcessInput();
        PrintInputReport();

        SetBlocksize( blocksize );
#ifdef EL_HAVE_SCALAPACK
        SetDefaultBlockHeight( mb );
        SetDefaultBlockWidth( nb );
#endif

        const int commRank = mpi::WorldRank();
        Timer timer;

        if( testSeq && commRank == 0 )
        {
            timer.Start();
            Matrix<Real> sSeq;
            Matrix<C> ASeq; 
            Uniform( ASeq, m, n );
            SVD( ASeq, sSeq );
            Output("Sequential SingularValues: ",timer.Stop());
        }

        Grid g( mpi::COMM_WORLD );
        if( commRank == 0 )
            cout << "Grid is " << g.Height() << " x " << g.Width() << endl;
        DistMatrix<C> A(g);
        Uniform( A, m, n );
        if( print )
            Print( A, "A" );

        // Compute just the singular values 
        DistMatrix<Real,VR,STAR> sOnly(g);
        auto U( A );
        if( commRank == 0 )
            timer.Start();
        SVD( U, sOnly );
        if( commRank == 0 )
            Output("  SingularValues time: ",timer.Stop());

#ifdef EL_HAVE_SCALAPACK
        DistMatrix<C,MC,MR,BLOCK> ABlock( A );
        Matrix<Real> sBlock;
        if( commRank == 0 )
            timer.Start();
        SVD( ABlock, sBlock );
        if( commRank == 0 )
            Output("  ScaLAPACK SingularValues time: ",timer.Stop());
        if( commRank == 0 && print )
            Print( sBlock, "s from ScaLAPACK" ); 
#endif

        if( testDecomp )
        {
            // Compute the SVD of A 
            DistMatrix<C> V(g);
            DistMatrix<Real,VR,STAR> s(g);
            U = A;
            if( commRank == 0 )
                timer.Start();
            SVD( U, s, V );
            if( commRank == 0 )
                Output("  SVD time: ",timer.Stop());
            if( print )
            {
                Print( U, "U" );
                Print( V, "V" );
                Print( s, "s" );
            }

#ifdef EL_HAVE_SCALAPACK
            ABlock = A;
            DistMatrix<C,MC,MR,BLOCK> UBlock(g), VHBlock(g);
            if( commRank == 0 )
                timer.Start();
            SVD( ABlock, sBlock, UBlock, VHBlock );
            if( commRank == 0 )
                Output("  ScaLAPACK SVD time: ",timer.Stop());
            if( commRank == 0 && print )
                Print( sBlock, "s from ScaLAPACK" ); 
#endif

            // Compare the singular values from both methods
            sOnly -= s;
            const Real singValDiff = FrobeniusNorm( sOnly );
            const Real twoNormOfA = MaxNorm( s );
            const Real maxNormOfA = MaxNorm( A );
            const Real frobNormOfA = FrobeniusNorm( A );

            DiagonalScale( RIGHT, NORMAL, s, U );
            Gemm( NORMAL, ADJOINT, C(-1), U, V, C(1), A );
            const Real maxNormOfE = MaxNorm( A );
            const Real frobNormOfE = FrobeniusNorm( A );
            const Real epsilon = lapack::MachineEpsilon<Real>();
            const Real scaledResidual =
              frobNormOfE / (max(m,n)*epsilon*twoNormOfA);

            if( commRank == 0 )
            {
                Output("|| A ||_max   = ",maxNormOfA);
                Output("|| A ||_2     = ",twoNormOfA);
                Output("||A - U Sigma V^H||_max = ",maxNormOfE);
                Output("||A - U Sigma V^H||_F   = ",frobNormOfE);
                Output
                ("||A - U Sigma V_H||_F / (max(m,n) eps ||A||_2) = ",
                 scaledResidual);
                Output("|| sError ||_2 = ",singValDiff);
            }
        }
    }
    catch( exception& e ) { ReportException(e); }

    Finalize();
    return 0;
}
