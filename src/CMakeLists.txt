set(BOURGEON_FILES
	main.cc

	utils/byte_pattern.cc
	utils/byte_pattern.h
	utils/log_console.cc
	utils/log_console.h
	utils/hooking/detours.h
	utils/hooking/disasm.cc
	utils/hooking/hook_manager.cc
	utils/hooking/hook_manager.h

	core/ragnarok_client.cc
	core/ragnarok_client.h
	core/py_callbacks.cc
	core/py_callbacks.h
	core/bourgeon.cc
	core/bourgeon.h
)

add_library(bourgeon SHARED ${BOURGEON_FILES})
target_include_directories(bourgeon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bourgeon pybind11::embed)