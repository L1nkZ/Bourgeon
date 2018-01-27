#include "ragnarok_client.h"
#include <Windows.h>
#include <iomanip>
#include <sstream>
#include "core/native_hooks.h"
#include "ragnarok/object_factory.h"
#include "ragnarok/packets.h"
#include "utils/byte_pattern.h"
#include "utils/hooking/hook_manager.h"

RagnarokClient::RagnarokClient()
    : timestamp_(), session_(), rag_connection_() {}

bool RagnarokClient::Initialize() {
  timestamp_ = GetClientTimeStamp();
  if (timestamp_ == kUnknownTimeStamp) return false;

  ObjectFactory factory;
  session_ = factory.CreateSession(timestamp_);
  if (!session_) return false;

  rag_connection_ = factory.CreateRagConnection(timestamp_);
  if (!rag_connection_) return false;

  RegisterHooks();

  return true;
}

unsigned long RagnarokClient::timestamp() const { return timestamp_; }

Session& RagnarokClient::session() const { return *session_; }

RagConnection& RagnarokClient::rag_connection() const {
  return *rag_connection_;
}

bool RagnarokClient::UseItemById(int item_id) const {
  ITEM_INFO iinfo = session_->GetItemInfoById(item_id);
  if (iinfo.item_index_ == 0) return false;

  PACKET_CZ_USE_ITEM packet;
  packet.header = static_cast<short>(PacketHeader::CZ_USE_ITEM);
  packet.index = (unsigned short)iinfo.item_index_;
  packet.aid = session_->GetAid();
  return rag_connection_->SendPacket(sizeof(PACKET_CZ_USE_ITEM),
                                     (char*)&packet);
}

unsigned long RagnarokClient::GetClientTimeStamp() const {
  char* client_base = static_cast<char*>(GetClientBase());
  BytePattern pattern({'m', 'y', 'l', 'o', 'g', '('}, "xxxxxx");
  MEMORY_BASIC_INFORMATION mbi;

  while (VirtualQuery(client_base, &mbi, sizeof(mbi))) {
    if (mbi.Protect != PAGE_NOACCESS) {
      char* result =
          static_cast<char*>(pattern.Search(client_base, mbi.RegionSize));
      if (result) {
        return TranslateTimeStamp(std::string(result + 6, result + 6 + 11));
      }
    } else {
      break;
    }
    client_base += mbi.RegionSize;
  }

  return kUnknownTimeStamp;
}

void* RagnarokClient::GetClientBase() const {
  return reinterpret_cast<void*>(GetModuleHandle(nullptr));
}

std::string RagnarokClient::GetClientFilename() const {
  char filename[MAX_PATH];

  GetModuleFileNameA(nullptr, filename, MAX_PATH);
  return std::string(filename);
}

// Translate a string of the form "Jan 02 1990" to an unsigned long of the form
// 20170102
unsigned long RagnarokClient::TranslateTimeStamp(const std::string& str) const {
  std::istringstream sstream(str);
  std::tm time;

  sstream >> std::get_time(&time, "%b %d %Y");
  return (time.tm_year + 1900) * 10000 + (time.tm_mon + 1) * 100 + time.tm_mday;
}

bool RagnarokClient::RegisterHooks() {
  using namespace hooking;

  native_hooks::HookOnKeyDown();
  native_hooks::HookOnChatMessage();

  return true;
}
