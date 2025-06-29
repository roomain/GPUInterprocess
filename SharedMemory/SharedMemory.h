#pragma once
#include <string>
#include <memory>
#include <boost/interprocess/managed_shared_memory.hpp>
#include "notcopiable.h"

template<typename Type>
using SharedAllocator = boost::interprocess::allocator<Type, boost::interprocess::managed_shared_memory::segment_manager>;

enum class OpenMode
{
	Read_Only = 1,
	Write_Only = 1 << 1,
	ReadWrite = Write_Only | Read_Only
};

template<typename Type>
class SharedMemory
{

public:
	
	NOT_COPIABLE(SharedMemory)
	NOT_MOVABLE(SharedMemory)
	
	SharedMemory() = delete;

	explicit SharedMemory(const std::wstring& a_name, const OpenMode a_openMode, const size_t& a_memorySize = 0) :
		m_memoryName{ a_name }, m_openMode{ a_openMode }, m_memSize{ a_memorySize }
	{
		switch (m_openMode)
		{
		case OpenMode::Read_Only:
			m_memory = std::make_unique<boost::interprocess::managed_shared_memory>(
				boost::interprocess::open_only, m_memoryName.c_str());
			break;
		case OpenMode::Write_Only:
		case OpenMode::ReadWrite:
			boost::interprocess::shared_memory_object::remove(m_memoryName.c_str());
			m_memory = std::make_unique<boost::interprocess::managed_shared_memory>(
				boost::interprocess::create_only, m_memoryName.c_str(), m_memSize);
			break;
		}
	}

	virtual ~SharedMemory()
	{
		if (SharedMemory::containsFlag(m_openMode, OpenMode::Write_Only))
		{
			// release memory
			boost::interprocess::shared_memory_object::remove(m_memoryName.c_str());
		}
		//
	}

	constexpr size_t memorySize()const { return m_memSize; }
	inline std::wstring memoryName()const { return m_memoryName; }

	[[nodiscard]] Type* const createObject(const std::string& a_objectName)
	{
		/*if (m_allocator)
		{
			m_allocator = std::make_unique<SharedAllocator<Type>>(m_memory->get_segment_manager());
		}*/
		return m_memory->construct<Type>(a_objectName.c_str())();// (*m_allocator.get());
	}

	[[nodiscard]] const Type* getObject(const std::string& a_objectName)
	{
		return m_memory->find<Type>(a_objectName.c_str()).first;
	}

private:
	std::wstring m_memoryName;
	OpenMode m_openMode;
	size_t m_memSize;
	//std::unique_ptr<SharedAllocator<Type>> m_allocator;
	std::unique_ptr<boost::interprocess::managed_shared_memory> m_memory;

	static constexpr bool containsFlag (const OpenMode a_container, const OpenMode a_flag)
	{
		return a_flag == static_cast<OpenMode>(static_cast<unsigned int>(a_container) & static_cast<unsigned int>(a_flag));
	}
};

