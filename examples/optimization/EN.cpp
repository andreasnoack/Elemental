#include "El.hpp"
using namespace El;

DistSparseMatrix<double> ConcatFD2D( Int n0, Int n1 )
{
    DistSparseMatrix<double> A;
    const Int height = n0*n1;
    const Int width = 2*n0*n1;
    A.Resize( height, width );
    const Int localHeight = A.LocalHeight();
    A.Reserve( 7*localHeight );

    for( Int iLoc=0; iLoc<localHeight; ++iLoc )
    {
        const Int i = A.GlobalRow(iLoc);
        const Int x0 = i % n0;
        const Int x1 = i / n0;

        A.QueueLocalUpdate( iLoc, i, 15 );
        if( x0 > 0 )    A.QueueLocalUpdate( iLoc, i-1,  -1 );
        if( x0+1 < n0 ) A.QueueLocalUpdate( iLoc, i+1,   2 );
        if( x1 > 0 )    A.QueueLocalUpdate( iLoc, i-n0, -3 );
        if( x1+1 < n1 ) A.QueueLocalUpdate( iLoc, i+n0,  4 );

        const Int iRel = i + n0*n1;
        A.QueueLocalUpdate( iLoc, iRel, 1 );

        // For now, this is meant to use integer division (for reproducing)
        A.QueueLocalUpdate( iLoc, width-1, -10/height );
    }
    A.ProcessQueues();
    return A;
}

int main( int argc, char* argv[] )
{
    Initialize( argc, argv );
    const Int n0 = 50;
    const Int n1 = 50;
    const double lambda1 = 3;
    const double lambda2 = 4;
    const bool print = false;
    
    const int commRank = mpi::WorldRank();

    try
    {
        auto A = ConcatFD2D( n0, n1 );
        DistMultiVec<double> b;
        Gaussian( b, n0*n1, 1 );
        if( print )
        {
            Print( A, "A" );
            Print( b, "b" );
        }
        
        qp::affine::Ctrl<double> ctrl; 
        //ctrl.mehrotraCtrl.print = true;
        //ctrl.mehrotraCtrl.time = true;
        //ctrl.mehrotraCtrl.solveCtrl.progress = true;
        //ctrl.mehrotraCtrl.solveCtrl.time = true;
        
        DistMultiVec<double> x;
        Timer timer;
        mpi::Barrier( mpi::COMM_WORLD );
        if( commRank == 0 )
            timer.Start();
        EN( A, b, lambda1, lambda2, x, ctrl ); 
        if( commRank == 0 )
            Output("EN time: ",timer.Stop()," secs");
        if( print ) 
            Print( x, "x" );
    }
    catch( const exception& e ) { ReportException(e); }

    Finalize();
    return 0;
}
