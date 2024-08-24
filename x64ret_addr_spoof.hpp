#include <type_traits>

namespace return_spoofing {
	namespace stub {
		extern "C" void* _x64spoof_stub();

		template <typename Ret, typename... Args>
		static inline auto shellcode_stub(
			const void* shell,
			Args... args
		) -> Ret {
			auto fn = (Ret(*)(Args...))(shell);
			return fn(args...);
		}

		template <std::size_t Argc, typename>
		struct argument_remapper {
			template<
				typename Ret,
				typename First,
				typename Second,
				typename Third,
				typename Fourth,
				typename... Pack
			>
			static auto do_call(
				const void* shell,
				void* shell_param,
				First first,
				Second second,
				Third third,
				Fourth fourth,
				Pack... pack
			) -> Ret {
				return shellcode_stub<
					Ret,
					First,
					Second,
					Third,
					Fourth,
					void*,
					void*,
					Pack...
				>(
					shell,
					first,
					second,
					third,
					fourth,
					shell_param,
					nullptr,
					pack...
					);
			}
		};

		template <std::size_t Argc>
		struct argument_remapper<Argc, std::enable_if_t<Argc <= 4>> {
			template<
				typename Ret,
				typename First = void*,
				typename Second = void*,
				typename Third = void*,
				typename Fourth = void*
			>
			static auto do_call(
				const void* shell,
				void* shell_param,
				First first = First{},
				Second second = Second{},
				Third third = Third{},
				Fourth fourth = Fourth{}
			) -> Ret {
				return shellcode_stub<
					Ret,
					First,
					Second,
					Third,
					Fourth,
					void*,
					void*
				>(
					shell,
					first,
					second,
					third,
					fourth,
					shell_param,
					nullptr
					);
			}
		};
	}

	template <typename result, typename... arguments>
	static inline auto spoofing_call(
		const void* trampoline,
		result(*fn)(arguments...),
		arguments... args
	) -> result {
		struct shell_params {
			const void* trampoline;
			void* function;
			void* register_;
		};

		shell_params p = { trampoline, reinterpret_cast<void*>(fn) };
		using mapper = stub::argument_remapper<sizeof...(arguments), void>;
		return mapper::template do_call<result, arguments...>((const void*)&stub::_x64spoof_stub, &p, args...);
	}
};