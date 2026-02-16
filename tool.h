#include <iostream>



int char_int ( const char** x) {
    if ( ( **x >> 7 & 1 ) == 0 ) {
        (*x)++;
        return (int)*(*x - 1);
    }
    else
        if ( ( **x >> 6 & 1 ) == 1 ) {
            if ( ( **x >> 5 & 1 ) == 1 ) {
                if ( ( **x >> 4 & 1 ) == 1 ) {
                    *x = *x + 4;
                    return (int) ( *( *x - 4 ) & 0x7 ) << 18 | ( *( *x - 3 ) & 0x3f ) << 12 | ( *( *x - 2 ) & 0x3f ) << 6 | ( *( *x - 1 ) & 0x3f );
                }
                else {
                    *x = *x + 3;
                    return (int) ( *( *x - 3 ) & 0xf ) << 12 | ( *( *x - 2 ) & 0x3f ) << 6 | ( *( *x - 1 ) & 0x3f );
                }
            }
            else {
                *x = *x + 2;
                return (int) ( *( *x - 2 ) & 0x1f ) << 6 | ( *( *x - 1 ) & 0x3f );
            }
        }
        else {
            
            std::cout << "  ERROR  :  In char_int (char** ) the char caracter is not UNICODE first simbol";
            
            for (int i = 7 ; i >= 0 ; i--) {
                std::cout << ( **x >> i & 1 );
            }
            
        }
        
    return -1;
}



int read_number ( const char** n ) {
    
    int a = 0;
    
    while ( **n >= 48 && **n < 58 ) {
        a += **n - 48;
        a *= 10;
        (*n)++;
    }
    
    a /= 10;
    
    return a;
}



void swap ( int& a, int& b ) {
    
    int t = a;
    a = b;
    b = t;
    
}



void swap ( char& a, char& b ) {
    
    char t = a;
    a = b;
    b = t;
    
}











