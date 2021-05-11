// This file was taken from SimpleROHook
#include "ddraw/proxy_idirectdraw.h"

// #include "Core/RoCodeBind.h"
#include "utils/log_console.h"

CProxyIDirectDraw7 *CProxyIDirectDraw7::lpthis;

CProxyIDirectDraw7::CProxyIDirectDraw7(IDirectDraw7 *ptr)
    : m_Instance(ptr),
      CooperativeLevel(0),
      PrimarySurfaceFlag(0),
      TargetSurface(NULL) {
  // g_pRoCodeBind = new CRoCodeBind();
  LogDebug("IDirectDraw7::Create");
}

CProxyIDirectDraw7::~CProxyIDirectDraw7() { LogDebug("IDirectDraw7::Release"); }

ULONG CProxyIDirectDraw7::Proxy_Release(void) {
  ULONG Count;

  // if (g_pRoCodeBind) delete g_pRoCodeBind;
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
  LogDebug(("CProxyIDirectDraw7::QueryInterface()"));

  if (IsEqualGUID(riid, IID_IDirect3D7)) {
    LogDebug(("CProxyIDirectDraw7::IDirect3D7 Create."));
    HRESULT temp_ret = m_Instance->QueryInterface(riid, ppvObj);

    if (temp_ret == S_OK) {
      void *ret_cProxy;
      // LPVOID FAR *ppvObj_proxy;
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

  LogDebug(("IDirectDraw7::CreateSurface(): Desc.dwFlags = 0x{:x}",
            SurfaceDesc->dwFlags));
  LogDebug(("DDSD_BACKBUFFERCOUNT = {:x}", DDSD_BACKBUFFERCOUNT));
  HRESULT Result =
      m_Instance->CreateSurface(SurfaceDesc, CreatedSurface, pUnkOuter);

  if (FAILED(Result)) return Result;

  if (SurfaceDesc->dwFlags & DDSD_CAPS) {
    DDSCAPS2 *Caps = &SurfaceDesc->ddsCaps;
    LogDebug(("    Desc.ddsCaps.dwCaps = 0x{:x}", Caps->dwCaps));

    if (Caps->dwCaps == DDSCAPS_BACKBUFFER) {
      LogDebug(("BackBuffer Surface"));
    }

    const auto ptr = reinterpret_cast<size_t>(*CreatedSurface);
    LogDebug("0x{:x} | W {} | H {}", ptr, SurfaceDesc->dwWidth,
             SurfaceDesc->dwHeight);

    if (Caps->dwCaps & DDSCAPS_PRIMARYSURFACE) {
      *CreatedSurface = new CProxyIDirectDrawSurface7(*CreatedSurface);
      LogDebug(
          ("Primary Surface = {:x}", reinterpret_cast<size_t>(CreatedSurface)));
      PrimarySurfaceFlag = 1;
    } else if (Caps->dwCaps & DDSCAPS_3DDEVICE) {
      if (CooperativeLevel & DDSCL_FULLSCREEN && !PrimarySurfaceFlag) {
        LogDebug(("Hook the 3D Device Rendering Surface."));
        LogDebug(("  FullScreen Mode"));
        *CreatedSurface = new CProxyIDirectDrawSurface7(*CreatedSurface);

        // SetScreenSize((int)SurfaceDesc->dwWidth, (int)SurfaceDesc->dwHeight);
      } else {
        LogDebug(("Hook the 3D Device Rendering Surface."));
        LogDebug(("  Window Mode"));
        TargetSurface = *CreatedSurface;

        // SetScreenSize((int)SurfaceDesc->dwWidth, (int)SurfaceDesc->dwHeight);
      }
    } else {
      if (CooperativeLevel & DDSCL_FULLSCREEN) {
        if (Caps->dwCaps & DDSCAPS_BACKBUFFER) {
          LogDebug(("Hook Rendering Surface without 3D Device."));
          LogDebug(("  FullScreen Mode"));
          *CreatedSurface = new CProxyIDirectDrawSurface7(*CreatedSurface);
        }
      } else {
        if (Caps->dwCaps & DDSCAPS_BACKBUFFER) {
          LogDebug(("Hook Rendering Surface without 3D Device."));
          LogDebug(("  Window Mode"));
        }
      }
    }
  }

  return Result;
}

HRESULT CProxyIDirectDraw7::Proxy_GetDisplayMode(LPDDSURFACEDESC2 Desc) {
  LogDebug(("IDirectDraw7::GetDisplayMode()"));
  HRESULT Result = m_Instance->GetDisplayMode(Desc);

  if (FAILED(Result)) return Result;

  DWORD RefreshRate = Desc->dwRefreshRate;
  LogDebug(("    RefreshRate = {}", Desc->dwRefreshRate));

  if (RefreshRate == 0) {
    LogDebug(("    RefreshRate = {} >> dummyset 100", Desc->dwRefreshRate));
    RefreshRate = 100;
  }

  // g_PerformanceCounter.SetMonitorRefreshRate((int)RefreshRate);

  return Result;
}

HRESULT CProxyIDirectDraw7::Proxy_SetCooperativeLevel(HWND hWnd,
                                                      DWORD dwFlags) {
  LogDebug(("IDirectDraw7::SetCooperativeLevel(): dwFlags = 0x{:x}", dwFlags));

  // if (g_pSharedData) g_pSharedData->g_hROWindow = hWnd;

  // if (g_pRoCodeBind) g_pRoCodeBind->InitWindowHandle(hWnd);

  DDSURFACEDESC2 dummy;
  dummy.dwSize = sizeof(DDSURFACEDESC2);
  Proxy_GetDisplayMode(&dummy);
  CooperativeLevel = dwFlags;

  return m_Instance->SetCooperativeLevel(hWnd, dwFlags);
}

HRESULT CProxyIDirectDraw7::Proxy_SetDisplayMode(DWORD p1, DWORD p2, DWORD p3,
                                                 DWORD p4, DWORD p5) {
  LogDebug(("IDirectDraw7::SetDisplayMode()"));
  HRESULT Result = m_Instance->SetDisplayMode(p1, p2, p3, p4, p5);

  if (FAILED(Result)) return Result;

  LogDebug(("    {} | {} | {} | {} | {}", p1, p2, p3, p4, p5));

  DDSURFACEDESC2 Desc;
  ::ZeroMemory(&Desc, sizeof(Desc));
  Desc.dwSize = sizeof(Desc);
  m_Instance->GetDisplayMode(&Desc);
  LogDebug(("    RefreshRate = {}", Desc.dwRefreshRate));
  DWORD RefreshRate = Desc.dwRefreshRate;

  if (RefreshRate == 0) {
    LogDebug(("    RefreshRate = {} >> dummyset 100", Desc.dwRefreshRate));
    RefreshRate = 100;
  }

  // g_PerformanceCounter.SetMonitorRefreshRate((int)RefreshRate);

  return Result;
}

HRESULT CProxyIDirectDraw7::Proxy_WaitForVerticalBlank(DWORD dwFlags,
                                                       HANDLE hEvent) {
  static double VSyncWaitTick = 0.0;

  // LogDebug(("IDirectDraw7::WaitForVerticalBlank(): dwFlags =
  // 0x{:x} | hEvent = 0x{:x}", dwFlags, hEvent));
  HRESULT result;

  // g_PerformanceCounter.ModifiCounter();

  // if ((CooperativeLevel & DDSCL_FULLSCREEN) == 0) {
  //  if (g_pSharedData && g_pSharedData->fix_windowmode_vsyncwait) {
  //    Sleep((DWORD)((VSyncWaitTick * 950) / 1000));
  //    g_PerformanceCounter.InitInstaltPerformance();
  //    result = m_Instance->WaitForVerticalBlank(dwFlags, hEvent);
  //    VSyncWaitTick = g_PerformanceCounter.CalcInstaltPerformance();
  //    // result = m_Instance->WaitForVerticalBlank(dwFlags, hEvent);
  //    result = DD_OK;
  //  } else {
  //    result = m_Instance->WaitForVerticalBlank(dwFlags, hEvent);
  //  }
  //} else {
  //  result = m_Instance->WaitForVerticalBlank(dwFlags, hEvent);
  //}
  result = m_Instance->WaitForVerticalBlank(dwFlags, hEvent);

  return result;
}

HRESULT CProxyIDirect3D7::Proxy_CreateDevice(REFCLSID rclsid,
                                             LPDIRECTDRAWSURFACE7 lpDDS,
                                             LPDIRECT3DDEVICE7 *lplpD3DDevice) {
  LogDebug(("CProxyIDirect3D7::CreateDevice2()"));
  HRESULT temp_ret = m_Instance->CreateDevice(rclsid, lpDDS, lplpD3DDevice);

  if (temp_ret == D3D_OK) {
    void *ret_cProxy;
    ret_cProxy =
        new CProxyIDirect3DDevice7((IDirect3DDevice7 *)*lplpD3DDevice, lpDDS);
    *lplpD3DDevice = (LPDIRECT3DDEVICE7)ret_cProxy;

    return D3D_OK;
  }

  return temp_ret;
}

void CProxyIDirect3DDevice7::Proxy_Release(void) {
  // Empty
}

HRESULT CProxyIDirect3DDevice7::Proxy_SetRenderState(
    THIS_ D3DRENDERSTATETYPE dwRenderStateType, DWORD dwRenderState) {
  // LogDebug(("CProxyIDirect3D7::Proxy_SetRenderState(): type
  // = %08X | val = %08X", dwRenderStateType, dwRenderState));

  if (dwRenderStateType == D3DRENDERSTATE_ZENABLE && dwRenderState == 0) {
    // UI is drawn after the Zbuffer is disabled.
    // if (g_pRoCodeBind) g_pRoCodeBind->DrawOn3DMap(m_Instance);
  }

  return m_Instance->SetRenderState(dwRenderStateType, dwRenderState);
}

HRESULT CProxyIDirect3DDevice7::Proxy_BeginScene(void) {
  HRESULT result;

  if (m_firstonce) {
    m_firstonce = false;

    // if (g_pRoCodeBind) g_pRoCodeBind->Init(m_Instance);
  }

  m_frameonce = TRUE;
  // LogDebug(("CProxyIDirect3D7::Proxy_BeginScene()"));

  result = m_Instance->BeginScene();

  return result;
}

HRESULT CProxyIDirect3DDevice7::Proxy_EndScene(void) {
  // g_PerformanceCounter.ModifiFrameRate();

  // if (g_pRoCodeBind) g_pRoCodeBind->DrawSRHDebug(m_Instance);

  return m_Instance->EndScene();
}
