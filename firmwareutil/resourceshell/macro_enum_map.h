#pragma once

#define PARENS ()
#define EXPAND1(...) __VA_ARGS__
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))

#define FOR_EACH(macro, helper, ...) __VA_OPT__(EXPAND(helper(macro, __VA_ARGS__)))

#define MAKE_ENUM(type_name, ...)   \
type_name{                          \
    __VA_ARGS__                     \
}

#define MAKE_MAP(type_name, ...)    \
type_name{                          \
    __VA_ARGS__                     \
}

#define KEY_HELPER(key, ...) key,
#define NAME_ONLY_HELPER(key, ...) {#key},
#define BRACED_INIT_LIST(...) {__VA_ARGS__}
#define NAMED_BRACED_VALUE_HELPER(key, A) {#key, BRACED_INIT_LIST A},
#define NAMED_VALUE_HELPER(key, ...) {#key, {__VA_ARGS__}},
#define UNNAMED_VALUE_HELPER(key, ...) {__VA_ARGS__},

#define FOR_EACH_HELPER_NONE(macro, a1, ...) macro(a1) __VA_OPT__(FOR_EACH_AGAIN_NONE PARENS (macro, __VA_ARGS__))
#define FOR_EACH_HELPER_1(macro, a1, a2, ...) macro(a1, a2) __VA_OPT__(FOR_EACH_AGAIN_1 PARENS (macro, __VA_ARGS__))
#define FOR_EACH_HELPER_2(macro, a1, a2, a3, ...) macro(a1, a2, a3) __VA_OPT__(FOR_EACH_AGAIN_2 PARENS (macro, __VA_ARGS__))
#define FOR_EACH_HELPER_3(macro, a1, a2, a3, a4, ...) macro(a1, a2, a3, a4) __VA_OPT__(FOR_EACH_AGAIN_3 PARENS (macro, __VA_ARGS__))
#define FOR_EACH_HELPER_4(macro, a1, a2, a3, a4, a5, ...) macro(a1, a2, a3, a4, a5) __VA_OPT__(FOR_EACH_AGAIN_4 PARENS (macro, __VA_ARGS__))
#define FOR_EACH_HELPER_5(macro, a1, a2, a3, a4, a5, a6, ...) macro(a1, a2, a3, a4, a5, a6) __VA_OPT__(FOR_EACH_AGAIN_5 PARENS (macro, __VA_ARGS__))
#define FOR_EACH_HELPER_6(macro, a1, a2, a3, a4, a5, a6, a7, ...) macro(a1, a2, a3, a4, a5, a6, a7) __VA_OPT__(FOR_EACH_AGAIN_6 PARENS (macro, __VA_ARGS__))
#define FOR_EACH_HELPER_7(macro, a1, a2, a3, a4, a5, a6, a7, a8, ...) macro(a1, a2, a3, a4, a5, a6, a7, a8) __VA_OPT__(FOR_EACH_AGAIN_7 PARENS (macro, __VA_ARGS__))
#define FOR_EACH_HELPER_8(macro, a1, a2, a3, a4, a5, a6, a7, a8, a9, ...) macro(a1, a2, a3, a4, a5, a6, a7, a8, a9) __VA_OPT__(FOR_EACH_AGAIN_8 PARENS (macro, __VA_ARGS__))
#define FOR_EACH_HELPER_9(macro, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, ...) macro(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) __VA_OPT__(FOR_EACH_AGAIN_9 PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN_NONE() FOR_EACH_HELPER_NONE
#define FOR_EACH_AGAIN_1() FOR_EACH_HELPER_1
#define FOR_EACH_AGAIN_2() FOR_EACH_HELPER_2 
#define FOR_EACH_AGAIN_3() FOR_EACH_HELPER_3
#define FOR_EACH_AGAIN_4() FOR_EACH_HELPER_4 
#define FOR_EACH_AGAIN_5() FOR_EACH_HELPER_5
#define FOR_EACH_AGAIN_6() FOR_EACH_HELPER_6
#define FOR_EACH_AGAIN_7() FOR_EACH_HELPER_7
#define FOR_EACH_AGAIN_8() FOR_EACH_HELPER_8
#define FOR_EACH_AGAIN_9() FOR_EACH_HELPER_9

#define ENUM_MAP(enum_type_name, map_type_name, num_args, value_helper, ...) \
    MAKE_ENUM(enum_type_name, FOR_EACH(KEY_HELPER, FOR_EACH_HELPER_##num_args, __VA_ARGS__)); \
    MAKE_MAP(map_type_name, FOR_EACH(value_helper, FOR_EACH_HELPER_##num_args, __VA_ARGS__))

#define ENUM_MAP0(enum_type_name, map_type_name, value_helper, ...) \
    MAKE_ENUM(enum_type_name, FOR_EACH(KEY_HELPER, FOR_EACH_HELPER_NONE, __VA_ARGS__)); \
    MAKE_MAP(map_type_name, FOR_EACH(value_helper, FOR_EACH_HELPER_NONE, __VA_ARGS__))
    