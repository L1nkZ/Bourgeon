set(BOURGEON_FILES
	"main.cc"

	"utils/byte_pattern.cc"
	"utils/byte_pattern.h"
	"utils/log_console.cc"
	"utils/log_console.h"
	"utils/hooking/detours.h"
	"utils/hooking/disasm.cc"
	"utils/hooking/hook_manager.cc"
	"utils/hooking/hook_manager.h"
	"utils/hooking/proxy.h"

	"core/ragnarok_client.cc"
	"core/ragnarok_client.h"
	"core/bourgeon.cc"
	"core/bourgeon.h"

	"ragnarok/object_factory.cc"
	"ragnarok/object_factory.h"
	"ragnarok/packets.h"
	"ragnarok/talktype.h"

	"ragnarok/session/session.cc"
	"ragnarok/session/session.h"
	"ragnarok/session/session_20170613.cc"
	"ragnarok/session/session_20170613.h"
	"ragnarok/session/item_info.h"

	"ragnarok/rag_connection/rag_connection.h"
	"ragnarok/rag_connection/rag_connection_20170613.cc"
	"ragnarok/rag_connection/rag_connection_20170613.h"

	"ragnarok/ui_window_mgr/ui_window_mgr.cc"
	"ragnarok/ui_window_mgr/ui_window_mgr.h"
	"ragnarok/ui_window_mgr/ui_window_mgr_20170613.cc"
	"ragnarok/ui_window_mgr/ui_window_mgr_20170613.h"

	"python/bourgeon.cc"
	"python/ragnarok_client.cc"
)

add_library(bourgeon MODULE ${BOURGEON_FILES})
target_include_directories(bourgeon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bourgeon pybind11::embed)

foreach(source IN LISTS BOURGEON_FILES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()