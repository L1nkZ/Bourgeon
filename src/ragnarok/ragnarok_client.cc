#include "ragnarok/ragnarok_client.h"

#include <Windows.h>

#include <array>
#include <iomanip>
#include <sstream>

#include "ragnarok/object_factory.h"
#include "ragnarok/packets.h"
#include "utils/byte_pattern.h"
#include "utils/hooking/hook_manager.h"

RagnarokClient::RagnarokClient()
    : timestamp_(), session_(), rag_connection_(), window_mgr_() {}

bool RagnarokClient::Initialize() {
  timestamp_ = GetClientTimeStamp();
  if (timestamp_ == kUnknownTimeStamp) {
    return false;
  }

  ObjectFactory factory;
  session_ = factory.CreateSession(timestamp_);
  if (!session_) {
    return false;
  }

  rag_connection_ = factory.CreateRagConnection(timestamp_);
  if (!rag_connection_) {
    return false;
  }

  window_mgr_ = factory.CreateUIWindowMgr(timestamp_);
  if (!window_mgr_) {
    return false;
  }

  mode_mgr_ = factory.CreateModeMgr(timestamp_);
  if (!mode_mgr_) {
    return false;
  }

  return true;
}

uint32_t RagnarokClient::timestamp() const { return timestamp_; }

Session& RagnarokClient::session() const { return *session_; }

RagConnection& RagnarokClient::rag_connection() const {
  return *rag_connection_;
}

UIWindowMgr& RagnarokClient::window_mgr() const { return *window_mgr_; }

bool RagnarokClient::UseItemById(int item_id) const {
  PACKET_CZ_USE_ITEM packet;
  ItemInfo iinfo;

  if (!session_->GetItemInfoById(item_id, iinfo)) {
    return false;
  }

  packet.header = static_cast<short>(PacketHeader::CZ_USE_ITEM);
  packet.index = (unsigned short)iinfo.item_index_;
  packet.aid = session_->GetAid();

  return rag_connection_->SendPacket(sizeof(PACKET_CZ_USE_ITEM),
                                     (char*)&packet);
}

uint32_t RagnarokClient::GetClientTimeStamp() const {
  const auto* const p_client_base =
      static_cast<const uint8_t*>(GetClientBase());
  if (p_client_base == nullptr) {
    return kUnknownTimeStamp;
  }

  const auto* p_dos_header =
      reinterpret_cast<const IMAGE_DOS_HEADER*>(p_client_base);
  const auto* p_nt_headers = reinterpret_cast<const IMAGE_NT_HEADERS*>(
      p_client_base + p_dos_header->e_lfanew);

  const std::time_t temp = p_nt_headers->FileHeader.TimeDateStamp;
  std::tm time{};
  gmtime_s(&time, &temp);

  return (time.tm_year + 1900) * 10000 + (time.tm_mon + 1) * 100 + time.tm_mday;
}

void* RagnarokClient::GetClientBase() const {
  return static_cast<void*>(GetModuleHandleW(nullptr));
}

std::string RagnarokClient::GetClientFilename() const {
  std::array<char, MAX_PATH> filename;

  GetModuleFileNameA(nullptr, filename.data(), filename.size());
  return std::string(filename.data());
}
