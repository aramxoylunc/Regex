
#ifndef REGEX_H
#define REGEX_H
#include <vector>
#include <map>
#include <string>

namespace RegEx {
    
    
    
    class check {
        
        public:
        
            virtual void read ( const char** );
            virtual bool operator() ( const char** x, bool=false );
            virtual bool operator() ( const char* x );
            
            virtual void print ();
            virtual void print_name ();

    };
    
    
    
    class unit : public check {
        
        public:
        
            void read ( const char** ) override;
            bool operator() ( const char** x, bool=false ) override;
            using check::operator();

            void print () override;
            void print_name () override;
        
        private:
        
            int s;
        
    };



    class pair : public check {
        
        public:
            
            void read ( const char** x )override;
            bool operator() ( const char** x, bool=false ) override;
            using check::operator();
            
            void print () override;
            void print_name () override;

        public:
            
            int a;
            int b;
            
    };
    
    
    
    class keyword : public check {
        
        public:
            
            void read ( const char** x ) override;
            bool operator() ( const char** x, bool=false ) override;
            using check::operator();
            
            void print () override;
            void print_name () override;

        public:
        
            std::string kw;
        
    };
    
    

    class check_list : public check {
        
        public:
            
            void read ( const char** x ) override;
            bool operator() ( const char** x, bool=false ) override;
            using check::operator();
            
            void print () override;
            void print_name () override;

        private:
        
            bool is_right ( const char** x, bool=false );

        public:
        
            std::vector<check*> c;
            int n; // {*} n = n : {x} 0 _ >= 0 : 1 _ > 0 : 2 _ < 2 : 3 _ do not set count :      
            
    };
    
    
    
    class check_list_add : public check {
        
        public:
        
            void read ( const char** x ) override;
            bool operator() ( const char** x, bool=false ) override;
            using check::operator();
  
            void print () override;
            void print_name () override;

        public:
        
            std::vector < check* > cl; // check list
            std::string ss_name;
            std::string ss_data;
            
    };
    
    
    
    class Token {
        
        public:
        
            Token ();
            
            std::string get_content () const;
            
            void print ();
            void sp ( );
            static int space;
            
        // private:
        public:
        
            std::string name;
            const char *start, *end;
            std::vector<Token> tv;
            Token *last;
            
        
    };
    
    
    
    class RegEx {
        
        public:
        
            RegEx ();
        
            void read ( const char* x );
            check_list_add& operator[] ( std::string name );
        
        private:
        
            void read_side ( const char* x );
        
        public:
        //private:
        
            std::map < std::string, check_list_add* > msc;
            std::map < std::string, std::string > ss;
            // check_list keyword;
            // check_list space;
            static RegEx* RE;
            static RegEx* RE1;
            Token t;
            static Token* T;
            
    };



}




#endif











