// This file was taken from SimpleROHook and adapted
#include "ddraw/proxy_idirectdraw.h"

#include "backends/imgui_impl_win32.h"
#include "bourgeon.h"
#include "imgui/imgui_impl_dx7.h"
#include "utils/log_console.h"

CProxyIDirectDraw7 *CProxyIDirectDraw7::lpthis;

CProxyIDirectDraw7::CProxyIDirectDraw7(IDirectDraw7 *ptr)
    : m_Instance(ptr),
      CooperativeLevel(0),
      PrimarySurfaceFlag(0),
      TargetSurface(nullptr) {
  LogDebug("IDirectDraw7::Create");
}

CProxyIDirectDraw7::~CProxyIDirectDraw7() { LogDebug("IDirectDraw7::Release"); }

ULONG CProxyIDirectDraw7::Proxy_Release(void) {
  ULONG Count;

  Count = m_Instance->Release();
  LogDebug("CProxyIDirectDraw7::Release(): RefCount = {}", Count);
  delete this;

  return Count;
}

HRESULT CProxyIDirectDraw7::Proxy_RestoreAllSurfaces(void) {
  return m_Instance->RestoreAllSurfaces();
}

HRESULT CProxyIDirectDraw7::Proxy_QueryInterface(THIS_ REFIID riid,
                                                 LPVOID FAR *ppvObj) {
  LogDebug("CProxyIDirectDraw7::QueryInterface()");

  if (IsEqualGUID(riid, IID_IDirect3D7)) {
    LogDebug("CProxyIDirectDraw7::IDirect3D7 Create.");
    HRESULT temp_ret = m_Instance->QueryInterface(riid, ppvObj);

    if (temp_ret == S_OK) {
      void *ret_cProxy;
      ret_cProxy = new CProxyIDirect3D7((IDirect3D7 *)*ppvObj);
      *ppvObj = ret_cProxy;

      return S_OK;
    } else {
      return temp_ret;
    }
  }

  return m_Instance->QueryInterface(riid, ppvObj);
}

HRESULT CProxyIDirectDraw7::Proxy_CreateSurface(
    LPDDSURFACEDESC2 SurfaceDesc, LPDIRECTDRAWSURFACE7 FAR *CreatedSurface,
    IUnknown FAR *pUnkOuter) {
  DDSURFACEDESC2 OrgSurfaceDesc2 = *SurfaceDesc;
  HRESULT Result =
      m_Instance->CreateSurface(SurfaceDesc, CreatedSurface, pUnkOuter);
  if (FAILED(Result)) {
    return Result;
  }

  if (SurfaceDesc->dwFlags & DDSD_CAPS) {
    DDSCAPS2 *Caps = &SurfaceDesc->ddsCaps;
    if (Caps->dwCaps == DDSCAPS_BACKBUFFER) {
      LogDebug("BackBuffer Surface");
    }

    const auto ptr = reinterpret_cast<size_t>(*CreatedSurface);

    if (Caps->dwCaps & DDSCAPS_PRIMARYSURFACE) {
      *CreatedSurface = new CProxyIDirectDrawSurface7(*CreatedSurface);
      PrimarySurfaceFlag = 1;
    } else if (Caps->dwCaps & DDSCAPS_3DDEVICE) {
      if (CooperativeLevel & DDSCL_FULLSCREEN && !PrimarySurfaceFlag) {
        LogDebug("Hook the 3D Device Rendering Surface.");
        LogDebug("  FullScreen Mode");
        *CreatedSurface = new CProxyIDirectDrawSurface7(*CreatedSurface);
      } else {
        LogDebug("Hook the 3D Device Rendering Surface.");
        LogDebug("  Window Mode");
        TargetSurface = *CreatedSurface;
      }
    } else {
      if (CooperativeLevel & DDSCL_FULLSCREEN) {
        if (Caps->dwCaps & DDSCAPS_BACKBUFFER) {
          LogDebug("Hook Rendering Surface without 3D Device.");
          LogDebug("  FullScreen Mode");
          *CreatedSurface = new CProxyIDirectDrawSurface7(*CreatedSurface);
        }
      } else {
        if (Caps->dwCaps & DDSCAPS_BACKBUFFER) {
          LogDebug("Hook Rendering Surface without 3D Device.");
          LogDebug("  Window Mode");
        }
      }
    }
  }

  return Result;
}

HRESULT CProxyIDirectDraw7::Proxy_GetDisplayMode(LPDDSURFACEDESC2 Desc) {
  LogDebug("IDirectDraw7::GetDisplayMode()");
  HRESULT Result = m_Instance->GetDisplayMode(Desc);

  if (FAILED(Result)) {
    return Result;
  }

  DWORD RefreshRate = Desc->dwRefreshRate;
  LogDebug("    RefreshRate = {}", Desc->dwRefreshRate);

  if (RefreshRate == 0) {
    LogDebug("    RefreshRate = {} >> dummyset 100", Desc->dwRefreshRate);
    RefreshRate = 100;
  }

  return Result;
}

HRESULT CProxyIDirectDraw7::Proxy_SetCooperativeLevel(HWND hWnd,
                                                      DWORD dwFlags) {
  LogDebug("IDirectDraw7::SetCooperativeLevel(): dwFlags = 0x{:x}", dwFlags);
  DDSURFACEDESC2 dummy;
  dummy.dwSize = sizeof(DDSURFACEDESC2);
  Proxy_GetDisplayMode(&dummy);
  CooperativeLevel = dwFlags;

  return m_Instance->SetCooperativeLevel(hWnd, dwFlags);
}

HRESULT CProxyIDirectDraw7::Proxy_SetDisplayMode(DWORD p1, DWORD p2, DWORD p3,
                                                 DWORD p4, DWORD p5) {
  LogDebug("IDirectDraw7::SetDisplayMode()");
  HRESULT Result = m_Instance->SetDisplayMode(p1, p2, p3, p4, p5);
  if (FAILED(Result)) {
    return Result;
  }

  LogDebug("    {} | {} | {} | {} | {}", p1, p2, p3, p4, p5);

  DDSURFACEDESC2 Desc;
  ::ZeroMemory(&Desc, sizeof(Desc));
  Desc.dwSize = sizeof(Desc);
  m_Instance->GetDisplayMode(&Desc);
  LogDebug("    RefreshRate = {}", Desc.dwRefreshRate);
  DWORD RefreshRate = Desc.dwRefreshRate;

  if (RefreshRate == 0) {
    LogDebug("    RefreshRate = {} >> dummyset 100", Desc.dwRefreshRate);
    RefreshRate = 100;
  }

  return Result;
}

HRESULT CProxyIDirectDraw7::Proxy_WaitForVerticalBlank(DWORD dwFlags,
                                                       HANDLE hEvent) {
  return m_Instance->WaitForVerticalBlank(dwFlags, hEvent);
}

HRESULT CProxyIDirect3D7::Proxy_CreateDevice(REFCLSID rclsid,
                                             LPDIRECTDRAWSURFACE7 lpDDS,
                                             LPDIRECT3DDEVICE7 *lplpD3DDevice) {
  LogDebug("CProxyIDirect3D7::CreateDevice()");
  HRESULT temp_ret = m_Instance->CreateDevice(rclsid, lpDDS, lplpD3DDevice);
  if (temp_ret == D3D_OK) {
    void *ret_cProxy;
    ret_cProxy =
        new CProxyIDirect3DDevice7((IDirect3DDevice7 *)*lplpD3DDevice, lpDDS);
    *lplpD3DDevice = (LPDIRECT3DDEVICE7)ret_cProxy;

    // Init imgui
    ImGui_ImplDX7_Init(*lplpD3DDevice);

    return D3D_OK;
  }

  return temp_ret;
}

CProxyIDirect3DDevice7::~CProxyIDirect3DDevice7() {
  LogDebug("CProxyIDirect3D7::Release()");
}

ULONG CProxyIDirect3DDevice7::Proxy_Release() {
  ULONG Count = m_Instance->Release();
  LogDebug("CProxyIDirect3DDevice7::Release(): RefCount = {}", Count);

  if (Count == 0) {
    ImGui_ImplDX7_Shutdown();
    delete this;
  }

  return Count;
}

HRESULT CProxyIDirect3DDevice7::Proxy_SetRenderState(
    THIS_ D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState) {
  return m_Instance->SetRenderState(dwRenderStateType, dwRenderState);
}

HRESULT CProxyIDirect3DDevice7::Proxy_BeginScene() {
  return m_Instance->BeginScene();
}

HRESULT CProxyIDirect3DDevice7::Proxy_EndScene(void) {
  // Draw UI
  ImGui_ImplDX7_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
  Bourgeon::Instance().RenderUI();
  ImGui::EndFrame();
  ImGui::Render();
  // Render UI with DX7
  ImGui_ImplDX7_RenderDrawData(ImGui::GetDrawData());

  return m_Instance->EndScene();
}
