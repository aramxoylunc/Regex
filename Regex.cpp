#include "RegEx.h"
#include "tool.h"
#include <string>

namespace RegEx {
    
    
    
    void check::read ( const char** x ) {
        
    }
    
    
    
    bool check::operator() ( const char** x, bool compile ) {
        return false;
    }
    
    
    
    bool check::operator() ( const char* x ) {
        return (*this) ( &x );
    }
    
    
    
    void check::print ( ) {
        std::cout << "check";
    }
    
    
    
    void check::print_name ( ) {
        std::cout << "check";
    }
    
    
    
    void unit::read ( const char** x ) {
        (*x)++;
        s = char_int (x);
    }

    
    
    bool unit::operator() ( const char** x, bool compile ) {
        
        const char* start = *x;
        if ( s == char_int (x) ) {
            
            if ( compile ) {
                RegEx::T->name = std::string ("unit");
                RegEx::T->start = start;
                RegEx::T->end = *x;
            }
            
            return true;
        }
        else {
            // if ( compile )
            //     RegEx::T->name = std::string ("unit { ") + (char)s + " }";
            
            return false;
        }
        
    }
    
    
    
    void unit::print ( ) {
        std::cout << "_" << s;
    }
    
    
    
    void unit::print_name ( ) {
        std::cout << "unit";
    }
    
    
    
    void pair::read ( const char** x ) {
        
        char s = **x;
        (*x)++;
        
        a = char_int ( x );
        (*x)++;
        b = char_int ( x );
        
        char f = **x;
        (*x)++;
            
            
        if ( s == '[' )
            a--;
            
        if ( f == ']' )
            b++;
        
    }
    
    
    
    bool pair::operator() ( const char** x, bool compile ) {
        
        const char* start = *x;
        int c = char_int (x);
        
        if ( a < c && c < b ) {
            
            if ( compile ) {
                RegEx::T->name = std::string ( "pair" );
                RegEx::T->start = start;
                RegEx::T->end = *x;
            }
            
            return true;
        }
        else {
            return false;
        }
        
    }
    
    
    
    void pair::print ( ) {
        std::cout << "[" << a << ";" << b << "]";
    }
    
    
    
    void pair::print_name ( ) {
        std::cout << "pair";
    }
    
    
    
    void keyword::read ( const char** x ) {
        
        (*x)++;
        
        while ( **x != '>' ) {
            kw += **x;
            (*x)++;
        }
        
        kw.shrink_to_fit ();
        (*x)++;

    }
    
    
    
    bool keyword::operator() ( const char** x, bool compile ) {
        
        int i = 0;
        
        while ( i < kw.size () && (*x)[i] == kw[i] )
            i++;
        
        if ( i == kw.size () ) {
            
            RegEx::T->name = std::string ( "keyword" );
            RegEx::T->start = *x;
            RegEx::T->end = (*x) + i;
            
            (*x) += i;
            return true;
        }
        else
            return false;
        
    }
    
    
    
    void keyword::print ( ) {
        std::cout << "<" << kw << ">";
    }
    
    
    
    void keyword::print_name ( ) {
        std::cout << "keyword";
    }
    
    
    
    void check_list::read ( const char** x ) {
        
        start:
        
        check *c1;
        
        if ( **x == '^' ) {
            // std::cerr << "!!!!!!!";
            (*x)++;
            const char* xr = *x;
            while ( *xr >= '0' && *xr <= '9' || *xr >= 'a' && *xr <= 'z' || *xr >= 'A' && *xr <= 'Z' || *xr == '_' ) xr++;
            // RegEx::RE->keyword (&xr);
            std::string name;
            for (const char* i = *x ; i < xr ; i++)
                name += *i;
            // std::cerr << "???????";
            // std::cerr << name;
            if ( !RegEx::RE->msc.count (name) )
                RegEx::RE->msc[name] = new check_list_add;
            c.push_back ( RegEx::RE->msc[name] );
            *x = xr;
        }
        else {
            switch ( **x ) {
                case '_': c1 = new unit; break;
                case '(':
                case '[': c1 = new pair; break;
                case '<': c1 = new keyword; break;
                default: std::cerr << "  ERROR  :  In check_list::read 1" << *x; exit (0);
            }
            
            c1->read (x);
            c.push_back ( c1 );
        }

        if ( **x == '|' ) {
            (*x)++;
            goto start;
        }
        
        n = 1;
        if ( **x == '*' ) {
            (*x)++;
            n = read_number (x);
        }
        else if ( **x == '&' ) {
            (*x)++;
            n = **x - 48;
            n = -n - 1;
            (*x)++;
        }
    }
    
    
    
    bool check_list::operator() ( const char** x, bool compile ) {
        
        // std::cerr << "\n" << *x << "\n" << n << " ";
        // print ();
        // int size = ( RegEx::T->last != nullptr ) ? RegEx::T->last->tv.size () : 0;
        
        if ( n >= 0 ) {
            
            int _n = n;
            while ( _n > 0 && is_right ( x, compile ) ) _n--;
            
            // std::cout << (n == 0);
            if ( _n == 0 ) {
                return true;
            }
            else {
                
                if ( compile ) { // RegEx::T->last != nullptr ) {
                    // std::cerr << "}";
                    RegEx::T = RegEx::T->last;
                    RegEx::T->tv.resize ( RegEx::T->tv.size () - n + _n );
                    // std::cerr << "{";
                    RegEx::T = &RegEx::T->tv [ RegEx::T->tv.size () - 1 ];
                }
                
                return false;
            }
        }
        else {
            
            // std::cerr << "n _ " << n;
            
            // while ( is_right ( x, compile ) ) i++;
            // std::cout << "n" << n << i;
            
            if ( n == -1 ) {
                while ( is_right ( x, compile ) );
                // std::cout << 1;
                return true;
            }
            else if ( n == -2 ) {
                int i = 0;
                while ( is_right ( x, compile ) ) i++;
                // std::cout << *x << "\n";
                // std::cout << (i > 0);
                
                return i > 0;
            }
            else if ( n == -3 ) {
                is_right ( x, compile );
                return true;
            }
            else
                return true;
        }
    }
    
    
    
    void check_list::print ( ) {
        
        for (int i = 0 ; i < c.size () ; i++) {
            c[i]->print ();
            std::cout << "|";
        }
        std::cout << "\b";
        
    }
    
    
    
    void check_list::print_name ( ) {
        
        std::cout << "check_list";
        
    }
    
    
    
    bool check_list::is_right ( const char** x, bool compile ) {
        
        /*if ( RegEx::T->last != nullptr ) {
            RegEx::T = RegEx::T->last;
            RegEx::T->tv.push_back ( Token () );
            RegEx::T->tv [ RegEx::T->tv.size () - 1 ].last = RegEx::T;
            RegEx::T = &RegEx::T->tv [ RegEx::T->tv.size () - 1 ];
        }*/
        
        int i = 0;
        const char* xt = *x;
        while ( i < c.size () && !(*c[i]) ( &xt, compile ) ) {
            i++;
            xt = *x;
        }
            
        if ( i < c.size () ) {
            *x = xt;
            if ( compile ) { // RegEx::T != nullptr && RegEx::T->last != nullptr ) {
                // std::cerr << "}";
                RegEx::T = RegEx::T->last;
                RegEx::T->tv.push_back ( Token () );
                RegEx::T->tv [ RegEx::T->tv.size () - 1 ].last = RegEx::T;
                // std::cerr << "{";
                RegEx::T = &RegEx::T->tv [ RegEx::T->tv.size () - 1 ];
            }
            
            // (*c[i]) ( x, compile );
            return true;
        }
        else {
            
            /*if ( RegEx::T->last->tv.size () < -1 ) {
                RegEx::T = RegEx::T->last;
                RegEx::T->tv.resize ( RegEx::T->tv.size () - 1 );
                RegEx::T = &RegEx::T->tv [ RegEx::T->tv.size () - 1 ];
            }*/
            
            return false;
        }
    }
    
    
    
    void check_list_add::read ( const char** x ) {
        
        cl.push_back ( new check_list );
        cl[ cl.size() - 1 ]->read (x);
        // cl[0]->print ();
        
        while ( **x == '+' ) {
            (*x)++;
            cl.push_back ( new check_list );
            cl[ cl.size() - 1 ]->read (x);
            // cl[ cl.size() - 1 ]->print ();
        }

    }
    
    
    
    RegEx* RegEx::RE = nullptr;
    RegEx* RegEx::RE1 = nullptr;
    Token* RegEx::T = nullptr;
    
    
    
    bool check_list_add::operator() ( const char** x, bool compile ) {
        
        const char* start = *x;
        
        if ( compile ) { // RegEx::T != nullptr ) {
            RegEx::T->tv.resize ( 1 );
            RegEx::T->tv[0].last = RegEx::T;
            RegEx::T = &RegEx::T->tv[0];
            // std::cerr << "{";
        }
        int i = 0;
        while ( i < cl.size () && (*cl[i]) ( x, compile ) ) {
            i++;
            if ( compile && !RegEx::T->name.empty () ) { // RegEx::T != nullptr ) {
                // if ( RegEx::T->last != nullptr ) {
                    // std::cerr << "}";
                    RegEx::T = RegEx::T->last;
                    RegEx::T->tv.push_back ( Token () );
                    int tvs = RegEx::T->tv.size () - 1;
                    RegEx::T->tv[tvs].last = RegEx::T;
                    // std::cerr << "{";
                    RegEx::T = &RegEx::T->tv[tvs];
                // }
            }
        }
        if ( compile ) { // RegEx::T != nullptr )
            // std::cerr << "}";
            RegEx::T = RegEx::T->last;
        }
        
        // std::cout << *x << std::endl;
       
        if ( i == cl.size () ) {
            
            if ( !ss_name.empty () && compile )
                RegEx::RE->ss[ss_name] += ss_data;
            
            // if ( !ss_name.empty () )
            //      std::cout << ss_name << std::endl;
            
            if ( compile ) { // RegEx::T ) {
                RegEx::T->name = ss_name;
                RegEx::T->start = start;
                RegEx::T->end = *x;
                int i = RegEx::T->tv.size () - 1;
                // if ( RegEx::T->tv[i].name.empty () )
                    RegEx::T->tv.resize ( i );
            }
            
            return true;
        }
        else {
            
            if ( compile ) { // RegEx::T != nullptr ) {
                // std::cerr << (int64_t)RegEx::T;
                RegEx::T->tv.clear (); // resize ( 0 );
                RegEx::T->name = std::string ();
                RegEx::T->start = nullptr;
                RegEx::T->end = nullptr;
            }
            
            return false;
        }
    }
    
    
    
    void check_list_add::print ( ) {
        
        for (int i = 0 ; i < cl.size () ; i++) {
            cl[i]->print ();
            std::cout << "+";
        }
        
        std::cout << "\b";
        
    }
    
    
    
    void check_list_add::print_name ( ) {
        
        std::cout << "check_list_add";
        
    }
    
    
    
    Token::Token () {
        
        start = end= nullptr;
        last = nullptr;
        
    }
    
    
    
    std::string Token::get_content () const {
        return std::string ( start, end );
    }
    
    
    
    int Token::space = -4;
    
    
    
    void Token::print () {
        
        if ( name == "unit" || name == "keyword" ) {
            
            std::cout << ", ";
            std::cout << name << " \"";
            for (const char* x = start ; x < end ; x++)
                if ( *x == '\n' ) {
                    std::cout << "\n";
                }
                else {
                    std::cout << *x;
                }
            std::cout << "\"";
        
        }
        else if ( name == "pair" ) {
            
            std::cout << ", ";
            std::cout << name << " \"";
            for (const char* x = start ; x < end ; x++)
                if ( *x == '\n' ) {
                    std::cout << "\n";
                }
                else {
                    std::cout << *x;
                }
            std::cout << "\"";
            
        }
        else {
            
            std::cout << "\n"; space += 4;
            sp ();
            std::cout << name << " \"";
            for (const char* x = start ; x < end ; x++)
                std::cout << *x;
            std::cout << "\" {";
            // space += 4;
        
            
        
            
            for (int i = 0 ; i < tv.size () ; i++)
                tv[i].print ();
            std::cout << "\n";
            
        
            sp ();
            std::cout << "}";
            space -= 4;
        }
    }
    
    
    
    void Token::sp ( ) {
        for (int i = 0 ; i < space ; i++)
            std::cout << " ";
    }
    
    
    
    RegEx::RegEx () {
        
        // const char* kw = "__|[0;9]|[A;Z]|[a;z]&1";
        // const char* sp = "_\n|_ &0";
        // keyword.read (&kw);
        // space.read (&sp);
        RE = this;
        /* std::cerr << std::hex << "&RegEx _ " << (int64_t)this << "\n";
        std::cerr << "&RegEx.Token _ " << (int64_t)&t << "\n";
        std::cerr << "&RegEx.Token.tv _ " << (int64_t)&t.tv << "\n" << std::dec;*/
        
    }
    
    
    
    void RegEx::read ( const char* x ) {
        
        if ( RE1 == nullptr ) {
            RE1 = new RegEx;
            RE1->read_side ("^all {[\n;~]}");
            RE1->read_side ("^unit {__+^all}");
            RE1->read_side ("^pair {_[|_(+^all+_;+^all+_]|_)}");
            RE1->read_side ("^keyword_help {[ ;>)|(>;~]}");
            RE1->read_side ("^keyword {_<+^keyword_help&0+_>}");
            RE1->read_side ("^check_list3 {_^+__|[0;9]|[A;Z]|[a;z]&1}");
            RE1->read_side ("^check_list4 {_*+[0;9]&1}");
            RE1->read_side ("^check_list2 {_&+[0;3]}");
            RE1->read_side ("^check_list1 {_|+^unit|^pair|^keyword|^check_list3}");
            RE1->read_side ("^check_list {^unit|^pair|^keyword|^check_list3+^check_list1&0+^check_list2|^check_list4&2}");
            RE1->read_side ("^check_list_add1 {_++^check_list}");
            RE1->read_side ("^check_list_add {^check_list+^check_list_add1&0}");
            // RE1->read_side ("^RegEx {_^+__|[0;9]|[A;Z]|[a;z]&1+_\n|_ &1+_{+^check_list+_++^check_list}");
            RE1->read_side ("^RegEx {_^+__|[0;9]|[A;Z]|[a;z]&1+_\n|_ &1+_{+^check_list_add+_}}");
        }
        
        if ( !(*RE1) ["RegEx"] (x) ) {
            std::cerr << "there is a sintax error in " << x;
            exit (0);
        }
        
        read_side ( x );
        
    }
    
    
    
    void RegEx::read_side ( const char* x ) {
        
        x++;
        const char* xr = x;
        while ( *xr >= '0' && *xr <= '9' || *xr >= 'a' && *xr <= 'z' || *xr >= 'A' && *xr <= 'Z' || *xr == '_' ) xr++;
        // keyword (&xr);
        std::string name;
        for (const char* i = x ; i < xr ; i++)
            name += *i;
        x = xr;
        while ( *x == ' ' || *x == '\n' ) x++;
        // space (&x);
        x++;
        if ( !msc.count (name) ) {
            msc[name] = new check_list_add;
        }
        ((check_list_add*)msc[name])->ss_name = name;
        RE = this;
        msc[name]->read (&x);
        
        /*while ( *x == '}' || *x == ' ' ) x++;
        if ( *x != '\0' ) {
            x++;
            xr = x;
            while ( *xr >= '0' && *xr <= '9' || *xr >= 'a' && *xr <= 'z' || *xr >= 'A' && *xr <= 'Z' || *xr == '_' ) xr++;
            // keyword (&xr);
            std::string ss_name ( x, xr );
            x = xr;
            while ( *x == ' ' || *x == '\n' ) x++;
            // space (&x);
            msc[name]->ss_name = ss_name;
            while ( *x == ' ' ) x++;
            xr = x;
            while ( *x != '\0' ) x++;
            msc[name]->ss_data = std::string ( xr, x );
        }*/
    }
    
    
    
    check_list_add& RegEx::operator[] ( std::string name ) {
        RE = this;
        T = &t;
        return *msc[name];
    }

    
    

};








