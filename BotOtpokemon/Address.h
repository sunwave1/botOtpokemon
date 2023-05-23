#pragma once

#include <map>
#include <string>
#include <windows.h>

using Offset = std::map<std::string, uint32_t>;

class Address {
public:
	Address(uint32_t moduleAddress) : moduleAddress(moduleAddress) {}
	~Address() = default;

	uint32_t getSumAddress(std::string& type) {
		if (offsets[type]) { return -1; }
		return offsets[type] + moduleAddress;
	}

private:
	uint32_t moduleAddress{};
	Offset offsets{
		{"PlayerTarget", 0xA60FC4},
		{"PlayerPokemon", 0xA60F48}
	};
};