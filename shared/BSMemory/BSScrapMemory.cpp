#include "BSScrapMemory.hpp"
#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOSERVICE
#define NOMCX
#define NOIME
#include "Windows.h"
#endif

#pragma optimize("y", on)

namespace BSScrapMemory {

	struct alignas(4) MemoryManager {
		const bool bInitialized;
	};

	struct alignas(4) ScrapHeap {
		const char* const pBufferStart;
		const char* const pStackPos;
		const char* const pBufferEnd;
		const void* const pLastBlock;
	};

	static INIT_ONCE		kInitOnce = INIT_ONCE_STATIC_INIT;
	static MemoryManager*	pMemoryManager = nullptr;
	static void* __fastcall	InitAllocator(void* apThis);

	static constexpr bool	ASSERT_EMPTY_FREES = true;

	namespace CurrentMemoryManager {

		static ScrapHeap*		(__thiscall* GetThreadScrapHeap)(MemoryManager* apThis) = reinterpret_cast<decltype(GetThreadScrapHeap)>(InitAllocator);

	}

	namespace CurrentScrapHeap {

		// While default value is 8MiB, it is completely arbitrary, and replaceable by mods
		static uint32_t* pMaxMemory = nullptr;

		static void*		(__thiscall* Allocate)(ScrapHeap* apThis, size_t size, size_t alignment) = nullptr;
		static void			(__thiscall* Deallocate)(ScrapHeap* apThis, void* ptr) = nullptr;
		static size_t		(__thiscall* Size)(const ScrapHeap* apThis, void* ptr) = nullptr;

		static inline uint32_t __fastcall GetAllocatedMemory(const ScrapHeap* apScrapHeap) {
			return apScrapHeap->pStackPos - apScrapHeap->pBufferStart;
		}

		static inline bool __fastcall HasSpace(const ScrapHeap* apScrapHeap, size_t size) {
			if (pMaxMemory) [[likely]]
				return (size + GetAllocatedMemory(apScrapHeap)) < *pMaxMemory;

			// Some kind of heap replacer is being used - have to go blind
			return true;
		}
	}

}

namespace BSScrapMemory {

	static BOOL WINAPI BSScrapAllocatorInitializer(PINIT_ONCE InitOnce, PVOID Parameter, PVOID* Context) {
		constexpr uint32_t uiMatchPattern = 0x56413F2E;
		if (*reinterpret_cast<uint32_t*>(0xEB6610) == uiMatchPattern) {
			// GECK
			pMemoryManager = reinterpret_cast<MemoryManager*>(0xF21B5C);
			CurrentScrapHeap::Allocate = reinterpret_cast<decltype(CurrentScrapHeap::Allocate)>(0x8559C0);
			CurrentScrapHeap::Deallocate = reinterpret_cast<decltype(CurrentScrapHeap::Deallocate)>(0x855B30);
			CurrentScrapHeap::Size = reinterpret_cast<decltype(CurrentScrapHeap::Size)>(0x855C30);
			CurrentScrapHeap::pMaxMemory = reinterpret_cast<uint32_t*>(0x8563F1);
			CurrentMemoryManager::GetThreadScrapHeap = reinterpret_cast<decltype(CurrentMemoryManager::GetThreadScrapHeap)>(0x854540);
		}
		else [[likely]] {
			// Game
			pMemoryManager = reinterpret_cast<MemoryManager*>(0x11F6238);
			CurrentScrapHeap::Allocate = reinterpret_cast<decltype(CurrentScrapHeap::Allocate)>(0xAA54A0);
			CurrentScrapHeap::Deallocate = reinterpret_cast<decltype(CurrentScrapHeap::Deallocate)>(0xAA5610);
			CurrentScrapHeap::Size = reinterpret_cast<decltype(CurrentScrapHeap::Size)>(0xAA5710);
			CurrentScrapHeap::pMaxMemory = reinterpret_cast<uint32_t*>(0xAA5ED1);
			CurrentMemoryManager::GetThreadScrapHeap = reinterpret_cast<decltype(CurrentMemoryManager::GetThreadScrapHeap)>(0xAA42E0);

			// Check if ScrapHeapManager::Init is not modified
			// Heap replacers (usually) hook it or nop it, meaning it's not possible for us to assume ScrapHeap's layout nor limits
			if (*reinterpret_cast<uint8_t*>(0xAA58D0) != 0x55)
				CurrentScrapHeap::pMaxMemory = nullptr;
		}

		if (!pMemoryManager->bInitialized) [[unlikely]] {
#ifdef NDEBUG
			DebugBreak();
#else
			assert(false && "Tried to use thread-local ScrapHeap before MemoryManager's initialization!");
#endif
			return FALSE;
		}

		return TRUE;
	}

	static void* __fastcall InitAllocator(void* apThis) {
		BSScrapMemory::initialize();
		return heap_get();
	}

}

namespace BSScrapMemory {

	__declspec(noinline) bool initialize() {
		return InitOnceExecuteOnce(&kInitOnce, BSScrapAllocatorInitializer, nullptr, nullptr);
	}

	__declspec(noalias) bool __fastcall hasSpace(size_t size) {
		return heap_hasSpace(heap_get(), size);
	}

	__declspec(allocator) __declspec(restrict) void* __cdecl malloc(size_t size) {
		return heap_allocate(heap_get(), size, 4);
	}

	__declspec(allocator) __declspec(restrict) void* __cdecl calloc(size_t num, size_t size) {
		const size_t stSize = num * size;

		void* pMemory = malloc(stSize);
		assert(pMemory);

		if (pMemory) [[likely]]
			memset(pMemory, 0, stSize);

		return pMemory;
	}

	__declspec(allocator) __declspec(restrict) void* __cdecl aligned_alloc(size_t alignment, size_t size) {
		return heap_allocate(heap_get(), size, alignment);
	}

	__declspec(noalias) void __cdecl free(void* ptr) {
		heap_deallocate(heap_get(), ptr);
	}

	__declspec(noalias) void __cdecl free_sized(void* ptr, size_t size) {
		free(ptr);
	}

	__declspec(noalias) void __cdecl aligned_free(void* ptr) {
		free(ptr);
	}

	__declspec(noalias) void __cdecl free_aligned_sized(void* ptr, size_t alignment, size_t size) {
		return aligned_free(ptr);
	}

	__declspec(noalias) size_t __cdecl msize(void* ptr) {
		return heap_size(heap_get(), ptr);
	}

}

namespace BSScrapMemory {

	void* __fastcall heap_get() {
		return CurrentMemoryManager::GetThreadScrapHeap(pMemoryManager);
	}

	__declspec(allocator) __declspec(restrict) void* __fastcall heap_allocate(void* heap, size_t size, size_t alignment) {
		assert(heap);
		assert(heap_hasSpace(heap, size));
		return CurrentScrapHeap::Allocate(static_cast<ScrapHeap*>(heap), size, alignment);
	}

	__declspec(noalias) void __fastcall heap_deallocate(void* heap, void* ptr) {
		assert(heap);
		if (ptr) [[likely]] {
			CurrentScrapHeap::Deallocate(static_cast<ScrapHeap*>(heap), ptr);
		}
		else {
			assert(!ASSERT_EMPTY_FREES);
		}
	}

	__declspec(noalias) size_t __fastcall heap_size(const void* heap, void* ptr) {
		assert(heap);
		return CurrentScrapHeap::Size(static_cast<const ScrapHeap*>(heap), ptr);
	}

	__declspec(noalias) bool __fastcall heap_hasSpace(const void* heap, size_t size) {
		assert(heap);
		return CurrentScrapHeap::HasSpace(static_cast<const ScrapHeap*>(heap), size);
	}

}

#pragma optimize("", on)