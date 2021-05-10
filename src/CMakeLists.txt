set(BOURGEON_FILES
	"main.cc"
	"bourgeon.cc"
	"bourgeon.h"

        # DDRAW Wrapper
        "ddraw/ddraw.def"
        "ddraw/ddraw.cc"
        "ddraw/ddraw.h"

	# Python
	"python/bourgeon.cc"
	"python/ragnarok_client.cc"

	# Ragnarok
	"ragnarok/configuration.h"
	"ragnarok/ragnarok_client.cc"
	"ragnarok/ragnarok_client.h"
	"ragnarok/object_factory.cc"
	"ragnarok/object_factory.h"
	"ragnarok/packets.h"
	"ragnarok/talktype.h"
	"ragnarok/session.cc"
	"ragnarok/session.h"
	"ragnarok/item_info.h"
	"ragnarok/rag_connection.cc"
	"ragnarok/rag_connection.h"
	"ragnarok/ui_window_mgr.cc"
	"ragnarok/ui_window_mgr.h"
	"ragnarok/mode_mgr.cc"
	"ragnarok/mode_mgr.h"
	"ragnarok/login_mode.cc"
	"ragnarok/login_mode.h"
	"ragnarok/game_mode.cc"
	"ragnarok/game_mode.h"

        # Layouts
	"ragnarok/object_layouts/session/layouts.h"
	"ragnarok/object_layouts/session/macro_utils.h"
	"ragnarok/object_layouts/session/20151102.h"
	"ragnarok/object_layouts/session/20170613.h"
	"ragnarok/object_layouts/session/20190116.h"

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
foreach(source IN LISTS BOURGEON_FILES)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

add_library(bourgeon MODULE ${BOURGEON_FILES})
set_target_properties(bourgeon PROPERTIES OUTPUT_NAME "ddraw")
target_include_directories(bourgeon PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_options(bourgeon PRIVATE "/MT")
target_compile_definitions(bourgeon PUBLIC -DWITH_THREAD -DNOMINMAX)
target_compile_definitions(bourgeon PRIVATE -DBOURGEON_VERSION="${PROJECT_VERSION}")
target_compile_definitions(bourgeon PRIVATE $<$<CONFIG:RelWithDebInfo>:BOURGEON_DEBUG>)

target_link_libraries(bourgeon PRIVATE pybind11::embed spdlog yaml-cpp)
