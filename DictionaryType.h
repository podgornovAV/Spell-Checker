#pragma once
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <string>
#include <algorithm>
struct InsensitiveCompare {
	bool operator() (const std::string& a, const std::string& b) const {
		return std::equal(
			a.begin(),
			a.end(),
			b.begin(),
			b.end(),
			[](char a, char b) {
				return std::tolower(a) == std::tolower(b);
			}
		);
	}
};

struct StringHashByLower {
public:
	size_t operator()(const std::string& str) const {
		std::string lower(str.length(), ' ');
		transform(str.begin(), str.end(), lower.begin(),
			[](unsigned char c) -> unsigned char { return tolower(c); });
		return std::hash<std::string>()(lower);
	}
};

using mapWord_T = std::unordered_map<std::string, uint64_t, StringHashByLower, InsensitiveCompare>;
using dictionary_t = std::map<int, mapWord_T>;
