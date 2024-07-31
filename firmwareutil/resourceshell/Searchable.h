#pragma once
#include <algorithm>
#include <string_view>
#include <cstring>

class Searchable {
public:
	// not marked explicit so that it is implicitly convertable from a const char*
	constexpr Searchable(std::string_view Name) : SearchableName(Name) {}
	[[nodiscard]] inline constexpr std::string_view GetName() const { return SearchableName; }

	/**
	 * Match a partial string `Term` against an array of strings in `Map`. The search succeeds if `Term` matches
	 * exactly one entry in `Map`. The search is fuzzy, meaning the match may occur anywhere in the string.
	 * @tparam T The type of element in `Map`. NOTE: This type must be derived from `Searchable`
	 * @param Map The array of elements to search
	 * @param Length Length of `Map`
	 * @param Term The partial search term to match against
	 * @return Index of the matching element in `Map` if success, otherwise -1
	 */
#define SEARCH_PARTIAL_UNIQUE(Map, Term) Searchable::SearchPartialUnique(Map, sizeof(Map) / sizeof(Map[0]), Term)
	template <typename T>
	static constexpr int32_t SearchPartialUnique(const T *Map, uint32_t Length, std::string_view Term) {
		static_assert(std::is_base_of_v<Searchable, T>);
		return SearchPartialUnique_impl((void *) Map, sizeof(T), Length, Term);
	}
	
	std::string_view getSearchableName() const { return SearchableName; }

private:
	const std::string_view SearchableName;

	// This was broken into a separate function so that the templated part above is as small as possible
	static constexpr uint32_t SearchPartialUnique_impl(const void *Map, uint32_t Size, int32_t Length, std::string_view Term) {
		int32_t Index = -1;

		// Loop through each item in th map
		for (int32_t candidate_idx = 0; candidate_idx < Length; candidate_idx++) {
			// If the term is shorter than the candidate, search for the term multiple times at increasing offset
			// from the first character of the candidate
			const auto &Candidate = ((Searchable*)&((uint8_t*)Map)[candidate_idx * Size])->GetName();
			uint32_t SearchLoops = (uint32_t)std::max(0L, 1L + (int)Candidate.size() - (int)Term.size());
			for (uint32_t candidate_offset = 0; candidate_offset < SearchLoops; candidate_offset++) {
				// Case-insensitive comparison of the term against the candidate at the current character offset
				if (!strncasecmp(&Candidate[candidate_offset], Term.data(), Term.size())) {
					// The term matched the candidate at the current offset
					if (Candidate.size() == Term.size()) {
						// It's an exact match
						return candidate_idx;
					}
					if (Index >= 0 && Index != candidate_idx) {
						// A _different_ candidate also matched, so the term is not unique enough to give an unambiguous result
						return -1;
					}
					// Store the index of the candidate that matched
					Index = candidate_idx;
				}
			}
		}
		return Index;
	}

};

template <typename T>
class SearchableRef : public Searchable {
public:
	constexpr SearchableRef(std::string_view Name, const T& Resource) :
		Searchable(Name), Resource(Resource) {}
	inline constexpr T& get() const { return Resource; }
	inline constexpr const T& getconst() const { return Resource; }

private:
	const T& Resource;
};

template <typename T>
class SearchableVal : public Searchable {
public:
	constexpr SearchableVal(std::string_view Name, T Resource) :
		Searchable(Name), Resource(std::move(Resource)) {}
	inline constexpr T& getref() { return Resource; }
	inline constexpr const T& getconstref() const { return Resource; }

private:
	T Resource;
};
