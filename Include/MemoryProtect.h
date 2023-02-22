#include <Windows.h>
#include <unordered_map>
#include "xxhash.h"
template<typename T>
class ProtectedMemory {
public:
	ProtectedMemory() {
		void* mem = VirtualAlloc(nullptr, sizeof(T), MEM_COMMIT, PAGE_READWRITE);
		ptr = new(mem) T();
		{
			void* _mem = VirtualAlloc(nullptr, sizeof(uint32_t), MEM_COMMIT, PAGE_READWRITE);
			hash = new(_mem) uint32_t();
		}
		if (ptr == nullptr) {
			throw ("Failed to allocate memory");
		}
	}
	~ProtectedMemory() {
		DWORD oldProtect;
		if (!VirtualProtect(ptr, sizeof(T), PAGE_NOACCESS, &oldProtect)) {
			throw("Failed to set memory protection to PAGE_NOACCESS");
		}
		VirtualFree(ptr, 0, MEM_RELEASE);
	}
	T* get() {
		DWORD oldProtect;
		if (!VirtualProtect(ptr, sizeof(T), PAGE_READONLY, &oldProtect)) {
			throw ("Failed to set memory protection to PAGE_READONLY");
		}
		return ptr;
	}
	bool write(T value) {
	
		DetectionType = value;
		DWORD oldProtect;
		if (!VirtualProtect(ptr, sizeof(T), PAGE_READWRITE, &oldProtect)) {
			return false;
		}
		*ptr = value;
		if (!VirtualProtect(ptr, sizeof(T), oldProtect, &oldProtect)) {
			return false;
		}
		{
			DWORD oldProtect;
			if (!VirtualProtect(hash, sizeof(uint32_t), PAGE_READWRITE, &oldProtect)) {
				return false;
			}
			*hash = hs::xxHash<T>(*ptr);
			if (!VirtualProtect(hash, sizeof(uint32_t), oldProtect, &oldProtect)) {
				return false;
			}
		}
		return true;
	}
	uint32_t  getHash()
	{
		DWORD oldProtect;
		if (!VirtualProtect(hash, sizeof(uint32_t), PAGE_READONLY, &oldProtect)) {
			throw("Failed to set memory protection to PAGE_READONLY");
		}
		return *hash;
	}
	T DetectionType;
private:
	T* ptr;
	uint32_t* hash;
};
template<typename T>
class Protect
{
public:
	T getValue(const char* key)
	{
		if(Protected.count(hs::hash64(key)) > 0)
		{
			auto map = Protected[hs::hash64(key)];
			if (map->getHash() == hs::xxHash<T>(*map->get()) && map->DetectionType == *map->get())
			{
				return *map->get();
			}
			else
			{
				printf("the value has been modifed in memory\n");
				system("pause");
				exitProcess();
			}
		}
		return T();
	}
	void setValue(const char* key, const T& value)
	{
		if (Protected.count(hs::hash64(key)) == 0)
		{
			auto _val = new ProtectedMemory<T>;
			_val->write(value);
			Protected[hs::hash64(key)] = _val;
		}
		else
		{
			auto& map = Protected[hs::hash64(key)];
			map->write((value));
		}
	}
private:
	void exitProcess()
	{
		exit(0);
	}
	std::unordered_map<uint32_t, ProtectedMemory<T>*> Protected;
};
