#define LOOP simd::Loop<0, Size>::step
#define LOOP_(_START_, _SIZE_) simd::Loop<_START_, _SIZE_>::step

#define VA_NARGS_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,N,...) N
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)

#define ARRAY_DECL1(_A_) Array &_A_
#define ARRAY_DECL2(_A_, _B_) Array &_A_, const Array &_B_
#define ARRAY_DECL3(_A_, _B_, _C_) Array &_A_, const Array &_B_, const Array &_C_

#define ARRAY_FORW1(_A_) _A_.data
#define ARRAY_FORW2(_A_, _B_) _A_.data, _B_.data
#define ARRAY_FORW3(_A_, _B_, _C_) _A_.data, _B_.data, _C_.data

#define PTR_DECL1(_A_) float *_A_
#define PTR_DECL2(_A_, _B_) float *_A_, const float *_B_
#define PTR_DECL3(_A_, _B_, _C_) float *_A_, const float *_B_, const float *_C_

#define SIMD_ITER_GENERATE(_NAME_, _COUNT_, ...)                                    \
    template<typename Array>                                                        \
    static inline void _NAME_(ARRAY_DECL ## _COUNT_(__VA_ARGS__))                   \
    {                                                                               \
        _NAME_<std::tuple_size<Array>::value>(ARRAY_FORW ## _COUNT_(__VA_ARGS__));  \
    }                                                                               \
                                                                                    \
    template<size_t Size>                                                           \
    static inline void _NAME_(PTR_DECL ## _COUNT_(__VA_ARGS__))

#define SIMD_ITERP(_NAME_, _COUNT_, ...) SIMD_ITER_GENERATE(_NAME_, _COUNT_, __VA_ARGS__)
#define SIMD_ITER(_NAME_, ...) SIMD_ITERP(_NAME_, VA_NARGS(__VA_ARGS__), __VA_ARGS__)
#define SIMD_RECIPE template<size_t Size = DefaultSize> static inline void

namespace simd
{
    namespace detail
    {
        template <size_t Size>
        struct ShouldLoopEven
        {
            static const bool value = Size > 8 && (Size / 4) % 2 == 0 && Size % 4 == 0;
        };

        template <size_t Size>
        struct ShouldLoopOdd
        {
            static const bool value = Size > 8 && (Size / 4) % 2 == 1 && Size % 4 == 0;
        };

        template <size_t Size>
        struct ShouldUnroll
        {
            static const bool value = Size == 4 || Size == 8;
        };

    }
    
    template<size_t Begin, size_t End, class Enable = void>
    struct Loop { };
    
    template<size_t Begin, size_t End>
    struct Loop<Begin, End, std::enable_if_t< detail::ShouldLoopEven<End - Begin>::value > >
    {
        template<typename Action>
        static inline void step(Action&& action)
        {
            for (size_t i = Begin; i < End; i += 4)
            {
                action(i);
                action(i += 4);
            }
        }
    };
    
    template<size_t Begin, size_t End>
    struct Loop<Begin, End, std::enable_if_t< detail::ShouldLoopOdd<End - Begin>::value > >
    {
        template<typename Action>
        static inline void step(Action&& action)
        {
            size_t i = Begin;
            action(i);
            while (i < End)
            {
                action(i += 4);
                action(i += 4);
            }
        }
    };
    
    template<size_t Begin, size_t End>
    struct Loop<Begin, End, std::enable_if_t< detail::ShouldUnroll<End - Begin>::value > >
    {
        template<typename Action>
        static inline void step(Action&& action) {
            action(Begin);
            Loop<Begin + 4, End>::step(action);
        }
    };
    
    template <size_t End>
    struct Loop<End, End>
    {
        template<typename Action>
        static void step(Action&& action) {}
    };
}
