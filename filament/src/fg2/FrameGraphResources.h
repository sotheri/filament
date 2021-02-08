/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TNT_FILAMENT_FG2_FRAMEGRAPHRESOURCES_H
#define TNT_FILAMENT_FG2_FRAMEGRAPHRESOURCES_H

#include "fg2/details/Resource.h"
#include "fg2/FrameGraphId.h"

#include "backend/DriverEnums.h"
#include "backend/Handle.h"

namespace filament::fg2 {

class FrameGraph;
class PassNode;
class VirtualResource;
struct RenderTarget;

/**
 * Used to retrieve the concrete resources in the execute phase.
 */
class FrameGraphResources {
public:
    FrameGraphResources(FrameGraph& fg, PassNode& passNode) noexcept;
    FrameGraphResources(FrameGraphResources const&) = delete;
    FrameGraphResources& operator=(FrameGraphResources const&) = delete;

    struct RenderPassInfo {
        backend::Handle<backend::HwRenderTarget> target;
        backend::RenderPassParams params;
    };

    /**
     * Return the name of the pass being executed
     * @return a pointer to a null terminated string. The caller doesn't get ownership.
     */
    const char* getPassName() const noexcept;

    /**
     * Retrieves the concrete resource for a given handle to a virtual resource.
     * @tparam RESOURCE Type of the resource
     * @param handle    Handle to a virtual resource
     * @return          Reference to the concrete resource
     */
    template<typename RESOURCE>
    RESOURCE const& get(FrameGraphId<RESOURCE> handle) const noexcept;

    /**
     * Retrieves the descriptor associated to a resource
     * @tparam RESOURCE Type of the resource
     * @param handle    Handle to a virtual resource
     * @return          Reference to the descriptor
     */
    template<typename RESOURCE>
    typename RESOURCE::Descriptor const& getDescriptor(FrameGraphId<RESOURCE> handle) const noexcept;

    /**
     * Retrieves the descriptor associated to a subresource
     * @tparam RESOURCE Type of the resource
     * @param handle    Handle to a virtual resource
     * @return          Reference to the subresource descriptor
     */
    template<typename RESOURCE>
    typename RESOURCE::SubResourceDescriptor const& getSubResourceDescriptor(FrameGraphId<RESOURCE> handle) const noexcept;

    /**
     * Retrieves the usage associated to a resource
     * @tparam RESOURCE Type of the resource
     * @param handle    Handle to a virtual resource
     * @return          Reference to the descriptor
     */
    template<typename RESOURCE>
    typename RESOURCE::Usage const& getUsage(FrameGraphId<RESOURCE> handle) const noexcept;

    /**
     * Retrieves the render pass information associated with Builder::userRenderTarget() with the
     * give id.
     * @param id identifier returned by Builder::userRenderTarget()
     * @return RenderPassInfo structure suitable for creating a render pass
     */
    RenderPassInfo getRenderPassInfo(uint32_t id = 0u) const noexcept;

private:
    VirtualResource const* getResource(FrameGraphHandle handle) const noexcept;
    FrameGraph& mFrameGraph;
    PassNode& mPassNode;
};

// ------------------------------------------------------------------------------------------------

template<typename RESOURCE>
RESOURCE const& FrameGraphResources::get(FrameGraphId<RESOURCE> handle) const noexcept {
    // TODO: assert the resource exists and is devirtualized
    return static_cast<Resource<RESOURCE> const*>(getResource(handle))->resource;
}

template<typename RESOURCE>
typename RESOURCE::Descriptor const& FrameGraphResources::getDescriptor(
        FrameGraphId<RESOURCE> handle) const noexcept {
    // TODO: assert the resource exists and is devirtualized
    return static_cast<Resource<RESOURCE> const*>(getResource(handle))->descriptor;
}

template<typename RESOURCE>
typename RESOURCE::SubResourceDescriptor const& FrameGraphResources::getSubResourceDescriptor(
        FrameGraphId<RESOURCE> handle) const noexcept {
    return static_cast<Resource<RESOURCE> const*>(getResource(handle))->subResourceDescriptor;
}

template<typename RESOURCE>
typename RESOURCE::Usage const& FrameGraphResources::getUsage(
        FrameGraphId<RESOURCE> handle) const noexcept {
    // TODO: assert the resource exists and is devirtualized
    return static_cast<Resource<RESOURCE> const*>(getResource(handle))->usage;
}

} // namespace filament::fg2

#endif //TNT_FILAMENT_FG2_FRAMEGRAPHRESOURCES_H