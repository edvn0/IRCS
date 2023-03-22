//
// Created by ec on 3/22/23.
//

#include "ircs/ReferencePointer.hpp"

#include <unordered_set>
#include <mutex>

namespace IRCS {
	static std::unordered_set<const void*> live_references;
	static std::mutex live_reference_mutex;

	namespace detail {

		void add_to_alive_references(const void* instance)
		{
			std::scoped_lock<std::mutex> lock(live_reference_mutex);
			live_references.insert(instance);
		}

		void remove_from_alive_references(const void* instance)
		{
			std::scoped_lock<std::mutex> lock(live_reference_mutex);
			live_references.erase(instance);
		}

		bool is_alive(const void* instance) { return live_references.contains(instance); }

	} // namespace

}