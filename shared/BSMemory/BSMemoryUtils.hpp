#pragma once

#include <xmemory>

#include "BSMemory.hpp"
#include "BSScrapMemory.hpp"

template<typename T>
class BSMemoryAllocator : public std::allocator<T> {
public:
	using value_type = T;
	BSMemoryAllocator() = default;

	template<typename U>
	BSMemoryAllocator(const BSMemoryAllocator<U>&) {}

	[[nodiscard]] __declspec(allocator) inline T* allocate(std::size_t n) {
		return BSMemory::malloc<T>(n);
	}

	[[nodiscard]] constexpr std::allocation_result<T*> allocate_at_least(const std::size_t n) {
		return { allocate(n), n };
	}

	inline void deallocate(T* p, std::size_t) noexcept {
		BSMemory::free(p);
	}
};

template<typename T>
class BSScrapAllocator : public std::allocator<T> {
public:
	using value_type = T;
	BSScrapAllocator() = default;

	template<typename U>
	BSScrapAllocator(const BSScrapAllocator<U>&) {}
	
	[[nodiscard]] __declspec(allocator) inline T* allocate(std::size_t n) {
		return BSScrapMemory::malloc<T>(n);
	}

	[[nodiscard]] constexpr std::allocation_result<T*> allocate_at_least(const std::size_t n) {
		return { allocate(n), n };
	}

	inline void deallocate(T* p, std::size_t) noexcept {
		BSScrapMemory::free(p);
	}
};

template<typename T>
class BSScrapBuffer {
public:
	inline BSScrapBuffer(size_t count, void* heap = nullptr) {
		const size_t allocSize = sizeof(T) * count;
		localHeap = heap ? heap : BSScrapMemory::heap_get();
		if (BSScrapMemory::heap_hasSpace(localHeap, allocSize)) [[likely]] {
			dataCount = count;
			data = static_cast<T*>(BSScrapMemory::heap_allocate(localHeap, allocSize, alignof(T)));
		}
		else [[unlikely]] {
			dataCount = 0;
			data = nullptr;
		}
		constructItems();
	}
	inline ~BSScrapBuffer() {
		if (data) [[likely]] {
			destructItems();
			BSScrapMemory::heap_deallocate(localHeap, data);
		}
	}

	BSScrapBuffer(const BSScrapBuffer&) = delete;
	BSScrapBuffer& operator=(const BSScrapBuffer&) = delete;

	inline T* get() {
		return data;
	}

	inline T& operator[](size_t index) {
		return data[index];
	}

protected:
	T*		data;
	size_t	dataCount;
	void*	localHeap;

	inline void constructItems() {
		assert(data);
		if constexpr (!std::is_trivially_constructible_v<T>) {
			for (size_t i = 0; i < dataCount; i++) {
				new (&data[i]) T();
			}
		}
	}

	inline void destructItems() {
		if constexpr (!std::is_trivially_destructible_v<T>) {
			for (size_t i = 0; i < dataCount; i++) {
				data[i].~T();
			}
		}
	}
};