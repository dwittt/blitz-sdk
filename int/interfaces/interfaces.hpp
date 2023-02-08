#pragma once
#include <windows.h>
#include <cstdint>
#include <string>
#include <stdexcept>

#include "baseclientdll.hpp"

#include "clientclass.hpp"

namespace interfaces {
	enum class interface_type { index, bruteforce };

	template <typename ret, interface_type type>
	ret* get_interface(const std::string& module_name, const std::string& interface_name) {
		using create_interface_fn = void* (*)(const char*, int*);
		const auto fn = reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandle(module_name.c_str()), "CreateInterface"));

		if (fn) {
			void* result = nullptr;

			switch (type) {
			case interface_type::index:
				result = fn(interface_name.c_str(), nullptr);

				break;
			case interface_type::bruteforce:
				char buf[128];

				for (uint32_t i = 0; i <= 100; ++i) {
					memset(static_cast<void*>(buf), 0, sizeof buf);

					result = fn(interface_name.c_str(), nullptr);

					if (result)
						break;
				}

				break;
			}

			if (!result)
				throw std::runtime_error(interface_name + " wasn't found in " + module_name);

			return static_cast<ret*>(result);
		}

		throw std::runtime_error(module_name + " wasn't found");
	}

	inline i_base_client_dll* client;
	inline i_client_mode* clientmode;

	bool initialize();
}
