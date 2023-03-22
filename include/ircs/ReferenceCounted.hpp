#pragma once

#include <atomic>
#include <cstdint>

namespace IRCS {

	class ReferenceCounted {
	public:
		void add_to_reference_count() { count++; }
		void remove_from_reference_count() { count--; }

		[[nodiscard]] std::uint32_t reference_count() const { return count.load(); }
	private:
		mutable std::atomic_uint32_t count { 0 };
	};

} // namespace IRCS