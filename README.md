# WebEngine
Next-generation web engine tech demo. Written in C++. Uses the WebGPU rendering API, with native support for Windows, Linux, and macOS, as well as all WebGPU-compatible browsers including Safari (iOS) and Chrome (Android)


[View on web browser &rarr;](https://phantomcloak.me)


## Tech Stack
- [Flecs](https://github.com/SanderMertens/flecs) - High-performance Entity Component System (ECS)
- [Jolt Physics](https://github.com/jrouwe/JoltPhysics) - Physics engine for collision detection and rigid body simulation
- [Ozz Animation](https://github.com/guillaumeblanc/ozz-animation) - Skeletal animation and skinning
- [Assimp](https://github.com/assimp/assimp) - Asset importer supporting 40+ file formats
- [ImGui](https://github.com/ocornut/imgui) - Immediate mode GUI for debug tooling
- [Emscripten](https://github.com/emscripten-core/emscripten) - C++ to WebAssembly compiler toolchain


## Architecture
Built around a **layer stack** and an **ECS-driven scene**, with rendering, physics, animation, and the editor as independent subsystems

- **ECS** via [Flecs](https://github.com/SanderMertens/flecs) — UUID-identified entities with parent-child hierarchy managed through `RelationshipComponent`
- **Layer stack** — application logic split into `Layer` subclasses (`EditorLayer`, `ImGuiLayer`) with attach, update, and event hooks
- **Physics** via [Jolt](https://github.com/jrouwe/JoltPhysics) — custom broad-phase and object layer filters, ray casting for editor picking, tracked vehicle system built on `VehicleConstraint`
- **Skeletal animation** via [Ozz-Animation](https://github.com/guillaumeblanc/ozz-animation) — per-frame sampling job converts local transforms to model-space bone matrices uploaded to a GPU storage buffer
- **Profiling** via [Tracy](https://github.com/wolfpld/tracy) — macro-based zones (`RN_PROFILE_FUNC`, `RN_PROFILE_FUNCN`), zero overhead when disabled
- **Logging** via [spdlog](https://github.com/gabime/spdlog) — core and client loggers writing to console, file, and a live `ImGuiLogSink` for in-editor display
- **Editor** — `EditorLayer` provides a scene hierarchy view, property panel, viewport, ImGuizmo transform gizmos, and a fly camera

## Rendering
Built on **WebGPU**, running natively via [Dawn](https://dawn.googlesource.com/dawn) (Vulkan, Metal, DX12) and in all modern browsers via Emscripten/WebAssembly

- **Physically based forward rendering** with Cook-Torrance BRDF — GGX normal distribution, Smith geometry, and Schlick Fresnel
- **Instanced draw calls** batched by mesh/material; transforms stored as a compact 3×vec4 and reconstructed on the GPU
- **Shader reflection** — parses WGSL source at load time to extract resource declarations and auto-generate `WGPUBindGroupLayout`s with no manual specification otherwise required in WebGPU
- **Name-based dynamic binding** via `BindingManager` — resources are set by name rather than raw group/location indices, with per-resource invalidation and validation
- **Cascaded shadow maps** (4 cascades) with per-frame recomputed splits based on the camera frustum; PCF filtering for smooth penumbrae
- **Image-Based Lighting (IBL)** — irradiance and pre-filtered specular maps computed via compute shaders for environment-accurate diffuse and specular
- **GPU mipmap generation** via compute shaders at load time for 2D textures and cubemaps
- **GPU skeletal animation** — bone matrices in a storage buffer, 4-weight per-vertex blending in the vertex shader; animation runtime via Ozz-Animation
- **Dynamic material uniforms** — scalar and vector properties set by name, offsets resolved via shader reflection and written directly into the GPU uniform buffer
- **Modular render pass architecture** — each pass independently declares its framebuffer, bind groups, and pipeline, with live shader hot-reload

### Build

- Web: `cmake --preset emscripten`
- MacOS: `cmake --preset dawn-macos`
