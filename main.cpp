#include <iostream>
#include "RegEx.h"
#include <vector>
#include <fstream>
#include <bits/stdc++.h>

void lit_init ( RegEx::RegEx& ); // ^lit
void op_rval ( RegEx::RegEx& ); // ^op_rval
void op_lval ( RegEx::RegEx& ); // ^op_lval
void exp_def ( RegEx::RegEx& ); // exp

int str_int ( const char* );

int main() {
    
    RegEx::RegEx re;
    
    re.read ( "^sp {_\n|_ &0}" );
    re.read ( "^sp1 {_\n|_ &1}" );
    
    re.read ( "^kw {__|[a;z]|[A;Z]+__|[a;z]|[A;Z]|[0;9]&0}");
    re.read ( "^num {_-&2+[0;9]&1}" );

    // value
    // re.read ( "^value_int {_-&2+[0;9]&1+_L|_l|_U|_u|<UL>|<ul>&2}" );
    re.read ( "^value_int {_-&2+[0;9]&1}" );
    re.read ( "^value_hex {<0x>+[0;9]|[A;f]&1+_L|_l|_U|_u|<UL>|<ul>&2}" );
    re.read ( "^value_char_help {_\\+[ ;~]}" );
    re.read ( "^value_char_help1 {[ ;\")|(\";\')|(\';~]}" );
    re.read ( "^value_char {_\'+^value_char_help|^value_char_help1+_\'}" );
    re.read ( "^value_str_lit {_\"+^value_char_help|^value_char_help1&0+_\"}" );
    re.read ( "^value_func_res_help {^sp+_,+^sp+^value}" );
    re.read ( "^value_func_res_help1 {^value+^value_func_res_help&0}" );
    re.read ( "^value_func_res {^kw+^sp+_(+^sp+^value_func_res_help1&2+^sp+_)}" );
    re.read ( "^value_kw {_*&0+^sp+^kw+^var_arr&0}" );
    re.read ( "^value_invers {_!+^value}" );
    re.read ( "^value {^value_invers|<true>|<false>|<nullptr>|^value_int|^value_hex|^value_str_lit|^value_char|^value_func_res|^value_kw}" );

    // var_dec
    re.read ( "^var_dec0 {^sp+_=+^sp+^value}" );
    re.read ( "^var_dec1 {^sp+_,+^sp+^kw+^var_arr&2+^var_dec0&2}" );
    re.read ( "^var_name {<char>|<short>|<int>|<long>|<bool>|<float>}" );
    re.read ( "^var_arr {^sp+_[+^sp+^exp+^sp+_]}" );
    re.read ( "^var_dec {^sp+^kw+^sp1+^kw+^var_arr&2+^var_dec0&2+^var_dec1&0+^sp+_;}" );

    // cpp_unit
    re.read ( "^cpp_unit {^comment_long|^for|^if|^var_dec|^comment|^exp1}" );
    
    // namespace
    re.read ( "^namespace {^sp+<namespace>+^sp1+^kw+^sp+_{+^class|^var_dec|^func_dec&0+^sp+_}+_;}" );
    
    // function
    re.read ( "^type {<int>|<char>|<void>|<float>}" );
    re.read ( "^func_args1 {^sp+_,+^sp+^type+^sp1+^kw}" );
    re.read ( "^func_args {^sp+^type+^sp1+^kw+^func_args1&0}" );
    re.read ( "^return {^sp+<return>+^sp+^value+^sp+_;}" );
    re.read ( "^func_dec {^sp+^type+^sp1+^kw+^sp+_(+^func_args&2+^sp+_)+^sp+_{+^cpp_unit&0+^return&2+^sp+_}}" );
    
    // class
    re.read ( "^class_help1 {^sp+<public:>|<private:>|<protected:>}" );
    re.read ( "^class {^sp+<class>+^sp1+^kw+^sp+_{+^cpp_unit|^class_help1|^func_dec&0+^sp+_}+_;}" );
    
    // class _ operator overloading
    re.read ( "^operator_help1 {_>*2}" );
    re.read ( "^operator_help2 {_>+_=}" );
    re.read ( "^operator {^operator_help1|^operator_help2|<<<>|_<|_>|<==>|_=|_+|_%|_-}" );
    // re.read ( "^op_ov {^sp+^kw+^sp+^operator+^sp+^value+^sp+_;}" );
    
    // expression
    re.read ( "^exp_help {^sp+^operator+^sp+^value}" );
    re.read ( "^exp {^sp+^value+^exp_help&0}" );
    re.read ( "^exp1 {^exp+^sp+_;}" );
    
    // scope
    re.read ( "^scope {^sp+_{+^cpp_unit&0+^sp+_}}");
    
    // if
    // re.read ( "^con {^sp+^num+^sp+_;}" );
    re.read ( "^else {^sp+<else>+^scope|^cpp_unit}" );
    re.read ( "^if {^sp+<if>+^sp+_(+^sp+^exp+^sp+_)+^cpp_unit|^scope+^else&2}" );
    
    // cpp code
    re.read ( "^cpp_code {^comment_long|^func_dec|^class|^namespace|^using|^comment|^include&0+^sp+<EOF>&2}" );
    
    // variable
    // re.read ( "^var_lvalue {^value_kw}" );
    re.read ( "^var_help {^sp+^operator+^sp+^value}" );
    re.read ( "^var_equ {^sp+^value_kw+^sp+_=+^sp+^value+^var_help&0+^sp+_;}" );
    
    // for
    re.read ( "^for_help {^sp+_(+^sp+^exp1|^var_dec+^exp1+^sp+^inc+^sp+_)}" );
    re.read ( "^for {^sp+<for>+^for_help+^scope|^cpp_unit}" );
    
    // condition
    re.read ( "^con {^sp+^num+^sp+_;}" );
    
    // inc
    re.read ( "^inc {^kw+<++>}" );
    
    // include
    re.read ( "^include_help1 {_\"+^kw+_\"}" );
    re.read ( "^include_help2 {_<+^kw+_>}" );
    // re.read ( "^include {^sp+<#include>+^sp+^include_help2}" );
    re.read ( "^include {^sp+<#include>+^sp+^include_help1|^include_help2}" );
    
    // using
    re.read ( "^using {^sp+<using>+^sp1+<namespace>+^sp1+^kw+^sp+_;}" );
    
    // comment
    re.read ( "^comment {^sp+<//>+[ ;~]&0+_\n}" );
    re.read ( "^comment_long_help {[\n;*)|(*;~]&0+_*+[\n;/)|(/;~]}" );
    re.read ( "^comment_long_help1 {[\n;*)|(*;~]&0+<*//*>}" );
    re.read ( "^comment_long {^sp+</*>+^comment_long_help&0+^comment_long_help1}" );
    
    // expression
    re.read ( "^exp_h1 {<::>}" );
    re.read ( "^exp_h2 {^exp_h1+^kw}" );
    
    // re.read ( "^expression {}" );


    /*lit_init ( re ); // ^lit
    op_rval ( re ); // ^op_rval
    op_lval ( re ); // ^op_lval
    
    re.read ( "^rval {^lit|^kw}" );
    re.read ( "^lval {^kw}" );
    
    exp_def ( re ); // exp*/
    
    
    std::fstream f( "file.txt" );
    std::string file;
    while ( !f.eof () ) {
        std::string temp;
        getline ( f, temp );
        file += temp + '\n';
    }

    const char* test = file.c_str ();

    std::cerr << re["cpp_code"] ( &test, 1 ); std::cout << test << std::endl;
    
    f.close ();
    
    
    return 0;
}



void lit_init ( RegEx::RegEx& re ) {
    
    re.read ( "^int_lit {_-&2+[0;9]&1+_L|_l|_U|_u|<UL>|<ul>&2}" );
    re.read ( "^hex_lit {<0x>+[0;9]|[A;f]&1+_L|_l|_U|_u|<UL>|<ul>&2}" );
    re.read ( "^char_lit_help {_\\+[ ;~]}" );
    re.read ( "^char_lit_help1 {[ ;\")|(\";\')|(\';~]}" );
    re.read ( "^char_lit {_\'+^char_lit_help|^char_lit_help1+_\'}" );
    re.read ( "^str_lit {_\"+^char_lit_help|^char_lit_help1&0+_\"}" );
    
    re.read ( "^lit {^sp+^int_lit|^hex_lit|^char_lit|^str_lit}" );
    
}



void op_rval ( RegEx::RegEx& re ) {
    
    re.read ( "^op_goe {_>+_=}" ); // greate or equal
    re.read ( "^op_sr {_>+_>}" ); // shift right
    re.read ( "^op_sre {_>+_>+_=}" ); // shift right equal
    
    re.read ( "^op_rval {^sp+_*|_/|_%|_+|_-|<<<>|<<=>|_<|^op_sr|^op_goe" \
              "|_>|<==>|<!=>}" \
              "|_&|_^|_||<&&>|<||>" );
    
}




void op_lval ( RegEx::RegEx& re ) {
    
    re.read ( "^op_lval {^sp+_=|<*=>|</=>|<%=>|<+=>|<-=>" \
              "|<<<=>|^op_sre|<&=>|<|=>|<^=>}" );

}




void exp_def ( RegEx::RegEx& re ) {
    
    re.read ( "^exp_op_rval {^op_rval+^rval}" );
    re.read ( "^exp {^rval+^exp_op_rval&0}" );
    
}



int str_int ( const char* x ) {
    
    int res = 0;
    while ( *x >= '0' && *x <= '9' ) {
        res *= 10;
        res += (int)(*x - '0');
        x++;
    }
    
    
    return res;
}

    

