set(BOURGEON_FILES
	"main.cc"
	"bourgeon.cc"
	"bourgeon.h"

	# Python
	"python/bourgeon.cc"
	"python/ragnarok_client.cc"

	# Ragnarok
	"ragnarok/ragnarok_client.cc"
	"ragnarok/ragnarok_client.h"
	"ragnarok/object_factory.cc"
	"ragnarok/object_factory.h"
	"ragnarok/packets.h"
	"ragnarok/talktype.h"

	"ragnarok/session/session.cc"
	"ragnarok/session/session.h"
	"ragnarok/session/item_info.h"
	"ragnarok/session/20170613/session.cc"
	"ragnarok/session/20170613/session.h"

	"ragnarok/rag_connection/rag_connection.cc"
	"ragnarok/rag_connection/rag_connection.h"
	"ragnarok/rag_connection/20170613/rag_connection.cc"
	"ragnarok/rag_connection/20170613/rag_connection.h"

	"ragnarok/ui_window_mgr/ui_window_mgr.cc"
	"ragnarok/ui_window_mgr/ui_window_mgr.h"
	"ragnarok/ui_window_mgr/20170613/ui_window_mgr.cc"
	"ragnarok/ui_window_mgr/20170613/ui_window_mgr.h"

	"ragnarok/mode_mgr/mode_mgr.cc"
	"ragnarok/mode_mgr/mode_mgr.h"
	"ragnarok/mode_mgr/20170613/mode_mgr.cc"
	"ragnarok/mode_mgr/20170613/mode_mgr.h"

	# Utils
	"utils/byte_pattern.cc"
	"utils/byte_pattern.h"
	"utils/log_console.cc"
	"utils/log_console.h"
	"utils/hooking/detours.h"
	"utils/hooking/disasm.cc"
	"utils/hooking/hook_manager.cc"
	"utils/hooking/hook_manager.h"
	"utils/hooking/proxy.h"
)

add_library(bourgeon MODULE ${BOURGEON_FILES})
target_include_directories(bourgeon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(bourgeon PRIVATE /MT$<$<CONFIG:Debug>:d>)
target_compile_definitions(bourgeon PRIVATE $<$<CONFIG:RelWithDebInfo>:BOURGEON_DEBUG>)

target_link_libraries(bourgeon pybind11::embed)
target_link_libraries(bourgeon spdlog)

foreach(source IN LISTS BOURGEON_FILES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()