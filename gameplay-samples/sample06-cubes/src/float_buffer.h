#ifndef FLOAT_BUFFER_H
#define FLOAT_BUFFER_H

/**
    A simple way to construct vertex arrays.
*/
template<typename T>
struct data_buffer
{
    std::vector<T> data;
    
    void clear()
    {
        data.clear();
    }
    
    size_t byte_size()
    {
        return data.size() * sizeof(T);
    }
    
  
    /** 
        The () operators allow easy construction of vertices.
    */
    data_buffer& operator()( T a )
    { 
        data.push_back( a );
        return *this;
    }
    
    data_buffer& operator()( T a, T b )
    { 
        data.push_back( a );
        data.push_back( b );
        return *this;
    }
    
    data_buffer& operator()( T a, T b, T c )
    { 
        data.push_back( a );
        data.push_back( b );
        data.push_back( c );
        return *this;
    }
    
    data_buffer& operator()( T a, T b, T c, T d )
    { 
        data.push_back( a );
        data.push_back( b );
        data.push_back( c );
        data.push_back( d );
        return *this;
    }
};

#endif