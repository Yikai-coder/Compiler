# Compiler
哈尔滨工业大学（深圳）2021年秋编译原理实验课，设计实现C子集编译器。

还是暴露了在大型项目上的设计与组织的不足，概念设计上完整性不好，这个主要是由于先前设计的时候后面的内容还没有学习，此外编程规范也没有统一，看起来很奇怪。





词法分析器语法描述

G=(V,T,P,S)

V={id, hex_constant, octal_constant, constant, string_constant, comma, semicolon, colon, percent_sign, corners, question_mark, left_parentheses, right_parentheses, left_bracket, right_bracket, left_braces, right_braces, plus, self_increase, self_add, minus, self_decrease, self_minus, times, self_times, divide, self_divide, and,  logic_and, or, logic_or, deny,  unequal, lt, leq, gt, geq, assign, eq, quote}

T=任意符号

P：

(1) id->letter(letter|digit|_)*

(2)hex_constant->0(x|X)(0-9|a-f|A-F)*

(3)octal_constant->0(0-7)*

(4)constant->0|(1-9)(0-9)*

(5)字符串常量string_constant->'(任意符号)' | "(任意符号)"

(6) comma->,

(7)semicolon->;

(8)colon->:

(9)percent_sign->%

(10)corners->^

(11)question_mark->?

(12)left_parentheses->(

(13))right_parentheses->)

(14)left_bracket->[

(15)right_bracket->]

(16)left_braces->{

(17)right_braces->}

(18)plus->+

(19)self_increase->++

(20)self_add->+=

(21)minus->-

(22)self_decrease->--

(23)self_minus->-=

(24)times->*

(25)self_times->*=

(26)divide->/

(27)self_divide->/=

(28)and->&

(29)logic_and->&&

(30)or->|

(31)logic_or->||

(32)deny->!

(33)unequal->!=

(34)lt-><

(35)leq-><=

(36)gt->>

(37)geq->>=

(38)assign->=

(39)eq->==

(40)quote->'|"



