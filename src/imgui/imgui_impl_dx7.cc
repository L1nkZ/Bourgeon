// dear imgui: Renderer Backend for DirectX7
// This needs to be used along with a Platform Backend (e.g. Win32)

// You can copy and use unmodified imgui_impl_* files in your project. See
// examples/ folder for examples of using this. If you are new to Dear ImGui,
// read documentation from the docs/ folder + read the top of imgui.cpp. Read
// online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui/imgui_impl_dx7.h"

// DirectX
#include <d3d.h>

#include <vector>

struct CUSTOMVERTEX {
  float pos[3];
  D3DCOLOR col;
  float uv[2];
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// DirectX data
static LPDIRECT3DDEVICE7 g_pd3dDevice = nullptr;
static LPDIRECTDRAWSURFACE7 g_FontTexture = nullptr;
static std::vector<CUSTOMVERTEX> g_vertices;
static std::vector<WORD> g_indices;

#ifdef IMGUI_USE_BGRA_PACKED_COLOR
#define IMGUI_COL_TO_DX7_ARGB(_COL) (_COL)
#else
#define IMGUI_COL_TO_DX7_ARGB(_COL) \
  (((_COL)&0xFF00FF00) | (((_COL)&0xFF0000) >> 16) | (((_COL)&0xFF) << 16))
#endif

static void ImGui_ImplDX7_SetupRenderState(ImDrawData* draw_data) {
  // Setup viewport
  D3DVIEWPORT7 vp;
  vp.dwX = vp.dwY = 0;
  vp.dwWidth = (DWORD)draw_data->DisplaySize.x;
  vp.dwHeight = (DWORD)draw_data->DisplaySize.y;
  vp.dvMinZ = 0.0f;
  vp.dvMaxZ = 1.0f;
  g_pd3dDevice->SetViewport(&vp);

  // Setup render state: fixed-pipeline, alpha-blending, no face culling, no
  // depth testing, shade mode (for gradient)
  g_pd3dDevice->SetTexture(0, nullptr);
  g_pd3dDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
  g_pd3dDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, FALSE);
  g_pd3dDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
  g_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, FALSE);
  g_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATER);
  g_pd3dDevice->SetRenderState(D3DRENDERSTATE_ALPHAREF, 0x00);
  g_pd3dDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
  g_pd3dDevice->SetRenderState(D3DRENDERSTATE_SHADEMODE, D3DSHADE_GOURAUD);
  g_pd3dDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
  g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
  g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

  // Setup orthographic projection matrix
  // Our visible imgui space lies from draw_data->DisplayPos (top left) to
  // draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is
  // (0,0) for single viewport apps. Being agnostic of whether <d3d.h> or
  // <DirectXMath.h> can be used, we aren't relying on
  // D3DXMatrixIdentity()/D3DXMatrixOrthoOffCenterLH() or
  // DirectX::XMMatrixIdentity()/DirectX::XMMatrixOrthographicOffCenterLH()
  {
    const float L = draw_data->DisplayPos.x + 0.5f;
    const float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x + 0.5f;
    const float T = draw_data->DisplayPos.y + 0.5f;
    const float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y + 0.5f;

    D3DMATRIX mat_identity = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    D3DMATRIX mat_projection = {2.0f / (R - L),
                                0.0f,
                                0.0f,
                                0.0f,
                                0.0f,
                                2.0f / (T - B),
                                0.0f,
                                0.0f,
                                0.0f,
                                0.0f,
                                0.5f,
                                0.0f,
                                (L + R) / (L - R),
                                (T + B) / (B - T),
                                0.5f,
                                1.0f};
    g_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &mat_identity);
    g_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &mat_identity);
    g_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &mat_projection);
  }
}

// Render function.
void ImGui_ImplDX7_RenderDrawData(ImDrawData* draw_data) {
  // Avoid rendering when minimized
  if (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f) {
    return;
  }

  // Create and grow buffers if needed
  if (g_vertices.size() < static_cast<size_t>(draw_data->TotalVtxCount)) {
    g_vertices.resize(draw_data->TotalVtxCount + 5000);
  }
  if (g_indices.size() < static_cast<size_t>(draw_data->TotalIdxCount)) {
    g_indices.resize(draw_data->TotalIdxCount + 10000);
  }

  // FIXME: Backup DX7 state

  // Copy and convert all vertices into a single contiguous buffer, convert
  // colors to DX7 default format.
  // FIXME-OPT: This is a minor waste of resource, the ideal is to use
  // imconfig.h and
  //  1) to avoid repacking colors:   #define IMGUI_USE_BGRA_PACKED_COLOR
  //  2) to avoid repacking vertices: #define
  //  IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT struct ImDrawVert { ImVec2 pos;
  //  float z; ImU32 col; ImVec2 uv; }
  size_t vertices_offset = 0;
  size_t indices_offset = 0;

  for (int n = 0; n < draw_data->CmdListsCount; n++) {
    const ImDrawList* cmd_list = draw_data->CmdLists[n];
    const ImDrawVert* vtx_src = cmd_list->VtxBuffer.Data;
    for (int i = 0; i < cmd_list->VtxBuffer.Size; i++) {
      auto& vertex = g_vertices[vertices_offset + i];
      vertex.pos[0] = vtx_src->pos.x;
      vertex.pos[1] = vtx_src->pos.y;
      vertex.pos[2] = 0.0f;
      vertex.col = IMGUI_COL_TO_DX7_ARGB(vtx_src->col);
      vertex.uv[0] = vtx_src->uv.x;
      vertex.uv[1] = vtx_src->uv.y;
      vtx_src++;
    }
    memcpy(g_indices.data() + indices_offset, cmd_list->IdxBuffer.Data,
           cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));

    vertices_offset += cmd_list->VtxBuffer.Size;
    indices_offset += cmd_list->IdxBuffer.Size;
  }

  // Setup desired DX state
  ImGui_ImplDX7_SetupRenderState(draw_data);

  // Render command lists
  // (Because we merged all buffers into a single one, we maintain our own
  // offset into them)
  int global_vtx_offset = 0;
  int global_idx_offset = 0;
  ImVec2 clip_off = draw_data->DisplayPos;
  for (int n = 0; n < draw_data->CmdListsCount; n++) {
    const ImDrawList* cmd_list = draw_data->CmdLists[n];
    for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
      const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
      if (pcmd->UserCallback != NULL) {
        // User callback, registered via ImDrawList::AddCallback()
        // (ImDrawCallback_ResetRenderState is a special callback value used by
        // the user to request the renderer to reset render state.)
        if (pcmd->UserCallback == ImDrawCallback_ResetRenderState) {
          ImGui_ImplDX7_SetupRenderState(draw_data);
        } else {
          pcmd->UserCallback(cmd_list, pcmd);
        }
      } else {
        const auto texture =
            reinterpret_cast<LPDIRECTDRAWSURFACE7>(pcmd->TextureId);
        g_pd3dDevice->SetTexture(0, texture);
        // FIXME: Replace scissor test to fix issues when scrolling
        g_pd3dDevice->DrawIndexedPrimitive(
            D3DPT_TRIANGLELIST, D3DFVF_CUSTOMVERTEX,
            g_vertices.data() + pcmd->VtxOffset + global_vtx_offset,
            static_cast<DWORD>(cmd_list->VtxBuffer.Size),
            g_indices.data() + pcmd->IdxOffset + global_idx_offset,
            pcmd->ElemCount, 0);
      }
    }
    global_idx_offset += cmd_list->IdxBuffer.Size;
    global_vtx_offset += cmd_list->VtxBuffer.Size;
  }

  // FIXME: Restore DX7 state
}

bool ImGui_ImplDX7_Init(IDirect3DDevice7* device) {
  // Setup backend capabilities flags
  ImGuiIO& io = ImGui::GetIO();
  io.BackendRendererName = "imgui_impl_dx7";
  io.BackendFlags |=
      ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the
                                               // ImDrawCmd::VtxOffset field,
                                               // allowing for large meshes.

  g_pd3dDevice = device;
  g_pd3dDevice->AddRef();
  return true;
}

void ImGui_ImplDX7_Shutdown() {
  ImGui_ImplDX7_InvalidateDeviceObjects();
  if (g_pd3dDevice) {
    g_pd3dDevice->Release();
    g_pd3dDevice = nullptr;
  }
}

static bool ImGui_ImplDX7_CreateFontsTexture() {
  // Build texture atlas
  ImGuiIO& io = ImGui::GetIO();
  uint8_t* pixels;
  int width, height, bytes_per_pixel;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);

  // Convert RGBA32 to BGRA32 (because RGBA32 is not well supported by DX7
  // devices)
#ifndef IMGUI_USE_BGRA_PACKED_COLOR
  if (io.Fonts->TexPixelsUseColors) {
    ImU32* dst_start =
        (ImU32*)ImGui::MemAlloc(width * height * bytes_per_pixel);
    for (ImU32 *src = (ImU32*)pixels, *dst = dst_start,
               *dst_end = dst_start + width * height;
         dst < dst_end; src++, dst++)
      *dst = IMGUI_COL_TO_DX7_ARGB(*src);
    pixels = (uint8_t*)dst_start;
  }
#endif

  D3DDEVICEDESC7 ddDesc;
  g_pd3dDevice->GetCaps(&ddDesc);

  DDSURFACEDESC2 ddsd;
  ZeroMemory(&ddsd, sizeof(ddsd));
  ddsd.dwSize = sizeof(ddsd);
  ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT |
                 DDSD_TEXTURESTAGE;
  ddsd.ddsCaps.dwCaps = DDSCAPS_TEXTURE;
  ddsd.dwWidth = width;
  ddsd.dwHeight = height;
  // Turn on texture management for hardware devices
  if (ddDesc.deviceGUID == IID_IDirect3DHALDevice) {
    ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
  } else if (ddDesc.deviceGUID == IID_IDirect3DTnLHalDevice) {
    ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;
  } else {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
  }

  ddsd.ddpfPixelFormat.dwSize = sizeof(ddsd.ddpfPixelFormat);
  ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
  ddsd.ddpfPixelFormat.dwRGBBitCount = 32;
  ddsd.ddpfPixelFormat.dwRBitMask = 0xff0000;
  ddsd.ddpfPixelFormat.dwGBitMask = 0xff00;
  ddsd.ddpfPixelFormat.dwBBitMask = 0xff;
  ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0xff000000;

  LPDIRECTDRAWSURFACE7 pddsRender = nullptr;
  LPDIRECTDRAW7 pDD = nullptr;

  g_pd3dDevice->GetRenderTarget(&pddsRender);
  if (pddsRender == nullptr) {
    return false;
  }
  pddsRender->GetDDInterface((void**)&pDD);
  pddsRender->Release();

  if (pDD == nullptr) {
    return false;
  }

  HRESULT result = pDD->CreateSurface(&ddsd, &g_FontTexture, nullptr);
  pDD->Release();

  if (!SUCCEEDED(result)) {
    return false;
  }

  // Upload texture to graphics system
  {
    DDSURFACEDESC2 lock_ddsd;
    lock_ddsd.dwSize = sizeof(lock_ddsd);
    if (g_FontTexture->Lock(nullptr, &lock_ddsd, DDLOCK_WAIT, nullptr) !=
        D3D_OK) {
      return false;
    }
    for (DWORD y = 0; y < lock_ddsd.dwHeight; y++) {
      memcpy((uint8_t*)lock_ddsd.lpSurface + lock_ddsd.lPitch * y,
             pixels + (lock_ddsd.dwWidth * bytes_per_pixel) * y,
             (lock_ddsd.dwWidth * bytes_per_pixel));
    }
    g_FontTexture->Unlock(nullptr);
  }

  // Store our identifier
  io.Fonts->SetTexID((ImTextureID)g_FontTexture);

#ifndef IMGUI_USE_BGRA_PACKED_COLOR
  if (io.Fonts->TexPixelsUseColors) {
    ImGui::MemFree(pixels);
  }
#endif

  return true;
}

bool ImGui_ImplDX7_CreateDeviceObjects() {
  if (!g_pd3dDevice) {
    return false;
  }
  if (!ImGui_ImplDX7_CreateFontsTexture()) {
    return false;
  }
  return true;
}

void ImGui_ImplDX7_InvalidateDeviceObjects() {
  if (!g_pd3dDevice) {
    return;
  }

  g_vertices.clear();
  g_indices.clear();

  if (g_FontTexture) {
    g_FontTexture->Release();
    g_FontTexture = nullptr;
    ImGui::GetIO().Fonts->SetTexID(nullptr);
  }  // We copied g_pFontTextureView to io.Fonts->TexID so let's clear that as
     // well.
}

void ImGui_ImplDX7_NewFrame() {
  if (g_FontTexture == nullptr) {
    ImGui_ImplDX7_CreateDeviceObjects();
  }
}
