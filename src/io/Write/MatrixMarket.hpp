/*
   Copyright (c) 2009-2015, Jack Poulson
   All rights reserved.

   This file is part of Elemental and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#pragma once
#ifndef EL_WRITE_MATRIXMARKET_HPP
#define EL_WRITE_MATRIXMARKET_HPP

namespace El {
namespace write {

template<typename T>
inline void
MatrixMarket( const Matrix<T>& A, string basename="matrix" )
{
    DEBUG_ONLY(CSE cse("write::MatrixMarket"))
    
    string filename = basename + "." + FileExtension(MATRIX_MARKET);
    ofstream file( filename.c_str(), std::ios::binary );
    if( !file.is_open() )
        RuntimeError("Could not open ",filename);

    // Write the header
    // ================
    {
        ostringstream os;
        os << "%%MatrixMarket matrix array ";
        if( IsComplex<T>::val )
            os << "complex "; 
        else
            os << "real ";
        os << "general\n";
        file << os.str();
    }
    
    // Write the size line
    // ===================
    const Int m = A.Height();
    const Int n = A.Width();
    {
        ostringstream os; 
        os << m << " " << n << "\n";
        file << os.str();
    }
    
    // Write the entries
    // =================
    for( Int j=0; j<n; ++j )
    {
        for( Int i=0; i<m; ++i )
        {
            ostringstream os;
            os << A.GetRealPart(i,j);
            if( IsComplex<T>::val )
                os << " " << A.GetImagPart(i,j);
            os << "\n";
            file << os.str();
        }
    }
}

} // namespace write
} // namespace El

#endif // ifndef EL_WRITE_MATRIXMARKET_HPP
