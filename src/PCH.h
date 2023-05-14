
#pragma once

#pragma warning(push)
#include <RE/Skyrim.h>
#include <REL/Relocation.h>
#include <SKSE/SKSE.h>

#pragma warning(disable: 4702)
#include <SimpleIni.h>

#ifdef NDEBUG
#	include <spdlog/sinks/basic_file_sink.h>
#else
#	include <spdlog/sinks/msvc_sink.h>
#endif
#pragma warning(pop)

using namespace std::literals;

namespace logger = SKSE::log;

namespace util
{
	using SKSE::stl::report_and_fail;
}

namespace std
{
	template <class T>
	struct hash<RE::BSPointerHandle<T>>
	{
		uint32_t operator()(const RE::BSPointerHandle<T>& a_handle) const
		{
			uint32_t nativeHandle = const_cast<RE::BSPointerHandle<T>*>(&a_handle)->native_handle();  // ugh
			return nativeHandle;
		}
	};
}

#define DLLEXPORT __declspec(dllexport)

#define RELOCATION_OFFSET(SE, AE) REL::VariantOffset(SE, AE, 0).offset()

#include "Plugin.h"
