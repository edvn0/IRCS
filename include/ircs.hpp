#include "ircs/ReferencePointer.hpp"

template <typename T, typename... Args>
static IRCS::ReferencePointer<T> make_ref(Args&&... args) {
	return IRCS::ReferencePointer<T>(new T(std::forward<Args>(args)...));
}
