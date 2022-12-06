#include "TContext.h"
#include <core/include/TInstance.h>
#include <core/include/TVersion.h>
#include <core/include/TVulkanLoader.h>

Turbo::Render::TContext::TContext()
{
    // TODO:
    // check Vulkan support
    // create instance
    // create physical device
    // create device
    // create device queue
    if (!Turbo::Core::TInstance::IsSupportVulkan())
    {
        std::cout << "Not Support Vulkan" << std::endl;
    }
    else
    {
        Turbo::Core::TVersion version = Turbo::Core::TVulkanLoader::Instance()->GetVulkanVersion();
        std::cout << "Vulkan Version:" << version.ToString() << std::endl;
    }
}

Turbo::Render::TContext::~TContext()
{
    // TODO:
    // destroy device queue
    // destroy device
    // destroy physical device
    // destroy instance
}
