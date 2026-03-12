#pragma once

namespace WebEngine {
  class Actor {
   public:
    virtual void OnStart() = 0;
    virtual void PreUpdate() = 0;
    virtual void Update() = 0;
  };
}  // namespace WebEngine
