#pragma once

#include "webgpu/webgpu.h"

namespace WebEngine
{
  class CommandBuffer
  {
   public:
    void Begin();
    void End();
    void Submit();

    WGPUCommandEncoder GetNativeEncoder() { return m_Encoder; }

   private:
    WGPUCommandEncoder m_Encoder;
    WGPUCommandBuffer m_Buffer;
  };
}  // namespace WebEngine
