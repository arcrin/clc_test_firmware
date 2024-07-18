#pragma once

// macro magic from https://www.scs.stanford.edu/~dm/blog/va-opt.html
#define PARENS ()
#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__
#define FOR_EACH(macro, helper, ...) __VA_OPT__(EXPAND(helper(macro, __VA_ARGS__)))

#define MAKE_ENUM(type_name, ...) \
type_name {                       \
    __VA_ARGS__                   \
}

#define MAKE_MAP(type_name, ...) \
type_name {                      \
    __VA_ARGS__                  \
}

#define KEY_HELPER(key, ...) key,
#define NAME_ONLY_HELPER(key, ...) {#key},
#define BRACED_INIT_LIST(...) {__VA_ARGS__}
#define NAMED_BRACED_VALUE_HELPER(key, A) {#key, BRACED_INIT_LIST A},
#define NAMED_VALUE_HELPER(key, ...) {#key, {__VA_ARGS__}},
#define UNNAMED_VALUE_HELPER(key, ...) {__VA_ARGS__},

// NOTE there appears to be a bug in the CLion linter (clang-tidy) where this for-each helper is incorrectly parsed
//  when there is only one `arg`. The error causes __VA_OPT__ to be parsed even when it is empty. This leads to
//  recursive parsing up to the limit set be the `EXPAND` macros (324). This causes CLion to lag heavily when ever
//  it tries to parse a macro that uses this helper. The workaround is to add a dummy argument (like 0)
//  after the real argument
#define FOR_EACH_HELPER_NONE(macro, a1, dummy, ...) macro(a1) __VA_OPT__(FOR_EACH_AGAIN_NONE PARENS (macro, __VA_ARGS__))
//#define FOR_EACH_HELPER_0(macro, a1, ...) macro(a1) __VA_OPT__(FOR_EACH_AGAIN_0 PARENS (macro, __VA_ARGS__))
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
//#define FOR_EACH_AGAIN_0() FOR_EACH_HELPER_0
#define FOR_EACH_AGAIN_1() FOR_EACH_HELPER_1
#define FOR_EACH_AGAIN_2() FOR_EACH_HELPER_2
#define FOR_EACH_AGAIN_3() FOR_EACH_HELPER_3
#define FOR_EACH_AGAIN_4() FOR_EACH_HELPER_4
#define FOR_EACH_AGAIN_5() FOR_EACH_HELPER_5
#define FOR_EACH_AGAIN_6() FOR_EACH_HELPER_6
#define FOR_EACH_AGAIN_7() FOR_EACH_HELPER_7
#define FOR_EACH_AGAIN_8() FOR_EACH_HELPER_8
#define FOR_EACH_AGAIN_9() FOR_EACH_HELPER_9

/**
 * TODO Update this to support parentheses around each tuple of arguments, instead of expecting a flattened list
 *  In this case, the key_helper and for_each_helper can be inferred
 * ENUM_MAP is a macro for creating an enum and an array of the same length.
 * The enum values can then be used to index the array, as if it were a map.
 * The array elements are of the type presented in `map_type_name`. This may be a structure with multiple elements.
 * For example, say the type is `struct {int A; int B; };` The variadic arguments `...` shall be list in the form:
 *  `enum_key1, A1, B1, enum_key2, A2, B2, ...`
 * Two helper variadic macros must be provided to parse this list of arguments: One to extract the key,
 * another to create the array element. Each helper macro in this case would take 3 named arguments and `...`
 * @param enum_type_name Type and name of the enum
 * @param map_type_name Type and name of the array
 * @param num_args Number of arguments per value, excluding the mandatory enum key argument
 * @param value_helper Helper macro to create the array element from the argument list
 * @param ... flattened list of key/array element arguments
 */
#define ENUM_MAP(enum_type_name, map_type_name, num_args, value_helper, ...) \
MAKE_ENUM(enum_type_name, FOR_EACH(KEY_HELPER, FOR_EACH_HELPER_##num_args, __VA_ARGS__));                  \
MAKE_MAP(map_type_name, FOR_EACH(value_helper, FOR_EACH_HELPER_##num_args, __VA_ARGS__))

#define ENUM_MAP0(enum_type_name, map_type_name, value_helper, ...) \
MAKE_ENUM(enum_type_name, FOR_EACH(KEY_HELPER, FOR_EACH_HELPER_NONE, __VA_ARGS__));                  \
MAKE_MAP(map_type_name, FOR_EACH(value_helper, FOR_EACH_HELPER_NONE, __VA_ARGS__))
