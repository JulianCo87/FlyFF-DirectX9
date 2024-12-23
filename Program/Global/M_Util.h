#pragma once
#include <type_traits>
#include <iostream>
#include <memory>
#include <map>
#include <unordered_map>
#ifdef __M_UTIL
#include <functional>
namespace mu
{
#ifdef __AR_UPDATE20191002
	template<typename T> struct remove_all_type {
		typedef T type;
	};
	template<typename T> struct remove_all_type<T*> {
		typedef typename remove_all_type<T>::type type;
	};

	// just SFINAE shit in here
	template <typename _TheClass>struct has_key_type {
		template <typename _ConType> constexpr static char _Test(typename _ConType::key_type* key);
		template <typename _ConType> constexpr static long _Test(_ConType* key);
		constexpr static  bool value = sizeof(_Test<_TheClass>(NULL)) == sizeof(char);
	};
	template <typename _TheClass>struct has_iterator {
		template <typename _ConType> constexpr static char _Test(typename _ConType::iterator* key);
		template <typename _ConType> constexpr static long _Test(_ConType* key);
		constexpr static  bool value = sizeof(_Test<_TheClass>(NULL)) == sizeof(char);
	};
	template <typename _TheClass>struct has_size_type {
		template <typename _ConType> constexpr static char _Test(typename _ConType::size_type* key);
		template <typename _ConType> constexpr static long _Test(_ConType* key);
		constexpr static  bool value = sizeof(_Test<_TheClass>(NULL)) == sizeof(char);
	};

#define HAS_CLASS_MEMBER(member) template<typename, typename T> \
	struct has_##member { \
	static_assert(std::integral_constant<T, false>::value, "Second template parameter needs to be of function type.");}; \
template<typename C, typename Ret, typename... Args> \
struct has_##member<C, Ret(Args...)> { \
private: \
	template<typename T> \
	static constexpr auto check(T*) -> typename std::is_same<decltype(std::declval<T>().##member(std::declval<Args>()...)),Ret>::type;  \
	template<typename> static constexpr std::false_type check(...); \
	typedef decltype(check<C>(0)) type; \
public: \
	static constexpr bool value = type::value; \
};
	HAS_CLASS_MEMBER(Serialize /*vector, list, map ...*/);

	HAS_CLASS_MEMBER(begin);
	HAS_CLASS_MEMBER(size);
#endif
#ifdef __M_USER_BOTS
	namespace tuplehelper {
		template <std::size_t... Is>
		struct index {};

		template <std::size_t N, std::size_t... Is>
		struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

		template <std::size_t... Is>
		struct gen_seq<0, Is...> : index<Is...> {};
	}
	namespace args {
		template <typename _RetType = void, typename ... _ArgsFunc> struct FuncWithArgs {

			using std_tuple = std::tuple<_ArgsFunc...>;
			using std_func = std::function<_RetType(_ArgsFunc...)>;

			std_func _func;
			std_tuple _tuple;

			FuncWithArgs(std::function<_RetType(_ArgsFunc...)> func, _ArgsFunc&&... args)
				: _func(func), _tuple(std::make_tuple(std::forward<_ArgsFunc>(args)...))
			{}

			FuncWithArgs() {

			}

			template <typename... Args> void set(std_func func, Args... args) {
				_func = func;
				_tuple = std::make_tuple(std::forward<Args...>(args)...);
			}

			template <typename... Args, std::size_t... Is>
			void __call_(std::tuple<Args...>& tup, tuplehelper::index<Is...>) {
				_func(std::get<Is>(tup)...);
			}

			template <typename... Args>
			void __call_(std::tuple<Args...>& tup) {
				__call_(tup, tuplehelper::gen_seq<sizeof...(Args)>{});
			}

			template <typename... Args> void call(Args...arg) {
				__call_(_tuple);
			}

			template <std::size_t ... Is> void set(_ArgsFunc... args) {
				std::get<Is, std_tuple>(_tuple) = (std::make_tuple(std::forward<_ArgsFunc>(args)...));
			}
			/*	template <typename F,
			typename... Args>
			static FuncWithArgs<Args...>
			make_func(F f, Args... args)
			{
			return FuncWithArgs<Args...>(std::forward<F>(f),
			std::forward<Args>(args)...);
			}*/
		};
		template <typename _RetTypeFunc,
			typename ... _ArgsFunc>
			struct StringToFunction {
			using _StrType = std::string;
			using my_class_ref = StringToFunction<_StrType, _RetTypeFunc, _ArgsFunc...>&;
			using my_class = StringToFunction<_StrType, _RetTypeFunc, _ArgsFunc...>;
			using std_function = std::function<_RetTypeFunc(_StrType, _ArgsFunc...)>;
			using std_umap= typename std::unordered_map<std::string, std_function>;
			using std_iterator = typename std_umap::iterator;
			private:
				std_umap	mapFunctions;
			public:
				void set(const char* szCmd, std_function func) {
					mapFunctions.insert({ szCmd, func });
				}

				bool exec(const std::string& strCmd, _ArgsFunc... args) {
					auto it = mapFunctions.find(strCmd);
					if (it != mapFunctions.end()) {
						it->second(strCmd, args...);
						return true;
					}
					return false;
				}
		};
	}
#endif
};
#endif


#ifdef __M_USER_BOTS_DB_BASE
#include <sstream>
#include <iomanip>
#include <chrono>
namespace mu {
	template <typename T = __time64_t> 
	T str2time(const char* szInput, const char* szFormat) {
		const auto tCurrentTime = ::time(nullptr);
		errno_t err = 0;
		tm tm;
		// converting ::time(nullptr) value to "tm"
		if ((err = localtime_s(&tm, &tCurrentTime)) != 0) {
			FLERROR_LOG(PROGRAM_NAME, "Error while converting time %s. Format should be the same as follwing: %s",
				szInput, szFormat);
			return 0LL;
		}
		// set sec, min and hour to 0.
		tm.tm_sec = tm.tm_min = tm.tm_hour = 0;
		// stringstream
		std::basic_istringstream<char, ::std::char_traits<char>, ::std::allocator<char>> ss(szInput);
		ss >> std::get_time(&tm, szFormat);

		_set_errno(0);
		// mktime
		return static_cast<T>(std::mktime(&tm));
	}
	template<typename ... Args>
	std::string strfmt(const char* szFormat, Args ... args) {
		const auto nSize = snprintf(nullptr, 0, szFormat, args...) + 1;
		if (nSize <= 1) {
			return std::string();
		}
		std::unique_ptr<char[]> pBuf(new char[nSize]);
		snprintf(pBuf.get(), nSize, szFormat, args...);
		return std::string(pBuf.get(), pBuf.get() + nSize - 1);
	}
};
#endif