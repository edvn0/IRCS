#pragma once

#include "ircs/ReferenceCounted.hpp"

#include <bit>
#include <cstddef>
#include <utility>

namespace IRCS {

	template <class T> static constexpr auto* bit_cast(T* t)
	{
#ifndef __APPLE__
		return std::bit_cast<T*>(t);
#else
		return reinterpret_cast<T*>(t);
#endif
	}

	namespace detail {
		void add_to_alive_references(const void* instance);

		void remove_from_alive_references(const void* instance);

		bool is_alive(const void* instance);
	} // namespace detail

	template <typename T> class ReferencePointer {
	public:
		ReferencePointer()
			: instance(nullptr)
		{
		}

		explicit ReferencePointer(std::nullptr_t n)
			: instance(nullptr)
		{
		}

		explicit ReferencePointer(T* instance)
			: instance(instance)
		{
			inc_ref();
		}

		template <typename Other> explicit ReferencePointer(const ReferencePointer<Other>& other)
		{
			instance = bit_cast(other.instance);
			inc_ref();
		}

		template <typename Other> [[maybe_unused]] explicit ReferencePointer(ReferencePointer<Other>&& other)
		{
			instance = bit_cast(other.instance);
			other.instance = nullptr;
		}

		[[maybe_unused]] static ReferencePointer<T> copy_without_increment(const ReferencePointer<T>& other)
		{
			ReferencePointer<T> result = nullptr;
			result->instance = other.instance;
			return result;
		}

		~ReferencePointer() { dec_ref(); }

		ReferencePointer(const ReferencePointer& other)
			: instance(other.instance)
		{
			inc_ref();
		}

		ReferencePointer& operator=(std::nullptr_t)
		{
			dec_ref();
			instance = nullptr;
			return *this;
		}

		ReferencePointer& operator=(const ReferencePointer& other)
		{
			if (&other == this)
				return *this;

			other.inc_ref();
			dec_ref();

			instance = other.instance;
			return *this;
		}

		template <typename T2> ReferencePointer& operator=(const ReferencePointer<T2>& other)
		{
			other.inc_ref();
			dec_ref();

			instance = other.instance;
			return *this;
		}

		template <typename T2> ReferencePointer& operator=(ReferencePointer<T2>&& other)
		{
			dec_ref();

			instance = other.instance;
			other.instance = nullptr;
			return *this;
		}

		bool valid() const { return static_cast<bool>(instance); }

		T* operator->() { return instance; }
		const T* operator->() const { return instance; }

		T& operator*() { return instance; }
		const T& operator*() const { return instance; }

		T* raw() { return instance; }
		const T* raw() const { return instance; }

		void reset(T* in_instance = nullptr)
		{
			dec_ref();
			instance = in_instance;
		}

		template <typename T2> ReferencePointer<T2> as() const { return ReferencePointer<T2>(*this); }

		template <typename... Args> static ReferencePointer<T> create(Args&&... args)
		{
			return ReferencePointer<T>(new T(std::forward<Args>(args)...));
		}

		static ReferencePointer<T> create() { return ReferencePointer<T>(); }

		bool operator==(const ReferencePointer<T>& other) const { return instance == other.instance; }

		bool operator!=(const ReferencePointer<T>& other) const { return !(this->operator==(other)); }

	private:
		void inc_ref() const
		{
			if (instance) {
				instance->add_to_reference_count();
				detail::add_to_alive_references(static_cast<const void*>(instance));
			}
		}

		void dec_ref() const
		{
			if (instance) {
				instance->remove_from_reference_count();
				if (instance->reference_count() == 0) {
					delete instance;
					detail::remove_from_alive_references(static_cast<const void*>(instance));
					instance = nullptr;
				}
			}
		}

		template <typename OtherT> friend class ReferencePointer;
		mutable T* instance;
	};

	template <class T> class [[maybe_unused]] WeakReference {
	public:
		WeakReference() = default;

		[[maybe_unused]] explicit WeakReference(ReferencePointer<T> reference) { instance = reference.raw(); }

		[[maybe_unused]] explicit WeakReference(T* in_instance) { instance = in_instance; }

		T* operator->() { return instance; }
		const T* operator->() const { return instance; }

		T& operator*() { return *instance; }
		const T& operator*() const { return *instance; }

		[[nodiscard]] bool is_valid() const { return instance != nullptr && detail::is_alive(instance); }
		explicit operator bool() const { return is_valid(); }

	private:
		mutable T* instance = nullptr;
	};

} // namespace IRCS
