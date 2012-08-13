#ifndef FLOAT_BUFFER_H
#define FLOAT_BUFFER_H

/**
    A simple way to construct vertex arrays.
*/
struct float_buffer
{
    std::vector<float> data;
    
    void clear()
    {
        data.clear();
    }
    
    
    /** 
        The () operators allow easy construction of vertices.
    */
    float_buffer& operator()( float a )
    { 
        data.push_back( a );
        return *this;
    }
    
    float_buffer& operator()( float a, float b )
    { 
        data.push_back( a );
        data.push_back( b );
        return *this;
    }
    
    float_buffer& operator()( float a, float b, float c )
    { 
        data.push_back( a );
        data.push_back( b );
        data.push_back( c );
        return *this;
    }
    
    float_buffer& operator()( float a, float b, float c, float d )
    { 
        data.push_back( a );
        data.push_back( b );
        data.push_back( c );
        data.push_back( d );
        return *this;
    }
};

#endif