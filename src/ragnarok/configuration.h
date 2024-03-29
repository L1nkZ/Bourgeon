#pragma once

#include <string>

const std::string kYamlConfiguration = R"(
# 2015-11-04aRagexe
20151102:
  CSession:
    layout: 20151102
    CSession: 0x0093DA20
    GetTalkType: 0x0094AFB0
  UIWindowMgr:
    UIWindowMgr: 0x0060C820
    ProcessPushButton: 0x00613100
    SendMsg: 0x00623E20
  CRagConnection:
    CConnection: 0x00814BB0
    SendPacket: 0x00815630
  CModeMgr:
    Switch: 0x0065AE30
  CLoginMode:
    OnUpdate: 0x0090CE90
  CGameMode:
    OnUpdate: 0x0088D730
    ProcessInput: 0x0088e6d0

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
  CLoginMode:
    OnUpdate: 0x00A1F2A0
  CGameMode:
    OnUpdate: 0x00996910
    ProcessInput: 0x00997b50

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
  CLoginMode:
    OnUpdate: 0x009DED20
  CGameMode:
    OnUpdate: 0x0094EE20
    ProcessInput: 0x009500f0
)";