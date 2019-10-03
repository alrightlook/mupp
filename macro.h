#pragma once

#define PP_CAT(x, y) PP_PRIMITIVE_CAT(x, y)
#define PP_PRIMITIVE_CAT(x, y) x##y

#define PP_ELSE_BLOCKER \
  switch (0)            \
  case 0:               \
  default:

#define PP_EVAL(...) __VA_ARGS__
#define PP_EAT(...)

#define PP_NUM_ARGS_SEQ(_1, _2, _3, _4, _5, _6, _7, _8, x, ...) x
#define PP_NUM_ARGS(...) PP_NUM_ARGS_SEQ(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)

#define PP_IF(c) PP_CAT(PP_IF_, c)
#define PP_IF_0(t, f) f
#define PP_IF_1(t, f) t

#define PP_FOR_EACH(m, ...) \
  PP_CAT(PP_FOR_EACH_, PP_NUM_ARGS(__VA_ARGS__))(m, __VA_ARGS__)
#define PP_FOR_EACH_1(m, _0) m(_0)
#define PP_FOR_EACH_2(m, _0, _1) m(_0) m(_1)
#define PP_FOR_EACH_3(m, _0, _1, _2) m(_0) m(_1) m(_2)
#define PP_FOR_EACH_4(m, _0, _1, _2, _3) m(_0) m(_1) m(_2) m(_3)
#define PP_FOR_EACH_5(m, _0, _1, _2, _3, _4) m(_0) m(_1) m(_2) m(_3) m(_4)
#define PP_FOR_EACH_6(m, _0, _1, _2, _3, _4, _5) \
  m(_0) m(_1) m(_2) m(_3) m(_4) m(_5)
#define PP_FOR_EACH_7(m, _0, _1, _2, _3, _4, _5, _6) \
  m(_0) m(_1) m(_2) m(_3) m(_4) m(_5) m(_6)
#define PP_FOR_EACH_8(m, _0, _1, _2, _3, _4, _5, _6, _7) \
  m(_0) m(_1) m(_2) m(_3) m(_4) m(_5) m(_6) m(_7)

#define PP_STR_IMPL(x) #x
#define PP_STR(x) PP_STR_IMPL(x)

#define PP_TAIL_IMPL(x, ...) __VA_ARGS__
#define PP_TAIL(...) PP_TAIL_IMPL(__VA_ARGS__)

#define PP_CHECK_IMPL(x, n, ...) n
#define PP_CHECK(...) PP_CHECK_IMPL(__VA_ARGS__, 0, )
#define PP_PROBE(x) x, 1,

#define PP_IS_PAREN(x) PP_CHECK(PP_IS_PAREN_PROBE x)
#define PP_IS_PAREN_PROBE(...) PP_PROBE(~)

#define PP_UNPARENTHESIZE(x) PP_IF(PP_IS_PAREN(x))(PP_EVAL x, x)
