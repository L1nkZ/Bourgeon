#pragma once

#include <string>

const std::string kYamlConfiguration = R"(
# 2017-06-14bRagexeRE
20170613:
  CSession:
    layout: 20170613
    CSession: 0x00a50b70
    GetTalkType: 0x00a5e960
  UIWindowMgr:
    UIWindowMgr: 0x006a2dd0
    ProcessPushButton: 0x006aa6e0
    SendMsg: 0x006bcfc0
  CRagConnection:
    CConnection: 0x0091d470
    SendPacket: 0x0091e1f0
  CModeMgr:
    Switch: 0x006f4800

# 2019-01-16cRagexe
20190116:
  CSession:
    layout: 20190116
    CSession: 0x00a145b0
    GetTalkType: 0x00a226f0
  UIWindowMgr:
    UIWindowMgr: 0x007166b0
    ProcessPushButton: 0x0071df50
    SendMsg: 0x00731350
  CRagConnection:
    CConnection: 0x008d4580
    SendPacket: 0x008d5410
  CModeMgr:
    Switch: 0x0075f850
)";