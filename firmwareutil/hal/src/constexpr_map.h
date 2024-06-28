#pragma once
#include <array>
#include <algorithm>

template <typename Key, typename Value, std::size_t Size>
struct constexpr_map {
public:
	constexpr explicit constexpr_map(std::array<std::pair<Key, Value>, Size> data) : data(data) {}

	[[nodiscard]] constexpr const Value & at(const Key &key) const {
		auto ptr = at_ptr(key);
		if (ptr) {
			return *ptr;
		}

		abort();
	}

	[[nodiscard]] constexpr const Value * at_ptr(const Key &key) const {
		const auto itr = std::find_if(begin(data), end(data), [&key] (const auto &v) { return v.first == key; });

		if (itr != end(data)) {
			return &itr->second;
		}

		return nullptr;
	}

private:
	const std::array<std::pair<Key, Value>, Size> data;
};

template <typename K, typename V, std::size_t Size>
constexpr auto make_constexpr_map(std::pair<K, V> const (& data)[Size]) {
	return constexpr_map<K, V, Size> {std::to_array(data)};
}
