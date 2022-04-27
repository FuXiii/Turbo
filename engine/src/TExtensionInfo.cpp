#include "TExtensionInfo.h"
#include <sstream>
#include "TLayerInfo.h"
#include "TPhysicalDevice.h"

std::vector<std::string> TAllExtensionNames = {
	"UNDEFINED",
	"VK_KHR_16bit_storage",
	"VK_KHR_8bit_storage",
	"VK_KHR_android_surface",
	"VK_KHR_bind_memory2",
	"VK_KHR_buffer_device_address",
	"VK_KHR_copy_commands2",
	"VK_KHR_create_renderpass2",
	"VK_KHR_dedicated_allocation",
	"VK_KHR_deferred_host_operations",
	"VK_KHR_depth_stencil_resolve",
	"VK_KHR_descriptor_update_template",
	"VK_KHR_device_group",
	"VK_KHR_device_group_creation",
	"VK_KHR_display",
	"VK_KHR_display_swapchain",
	"VK_KHR_draw_indirect_count",
	"VK_KHR_driver_properties",
	"VK_KHR_external_fence",
	"VK_KHR_external_fence_capabilities",
	"VK_KHR_external_fence_fd",
	"VK_KHR_external_fence_win32",
	"VK_KHR_external_memory",
	"VK_KHR_external_memory_capabilities",
	"VK_KHR_external_memory_fd",
	"VK_KHR_external_memory_win32",
	"VK_KHR_external_semaphore",
	"VK_KHR_external_semaphore_capabilities",
	"VK_KHR_external_semaphore_fd",
	"VK_KHR_external_semaphore_win32",
	"VK_KHR_get_display_properties2",
	"VK_KHR_get_memory_requirements2",
	"VK_KHR_get_physical_device_properties2",
	"VK_KHR_get_surface_capabilities2",
	"VK_KHR_image_format_list",
	"VK_KHR_imageless_framebuffer",
	"VK_KHR_incremental_present",
	"VK_KHR_maintenance1",
	"VK_KHR_maintenance2",
	"VK_KHR_maintenance3",
	"VK_KHR_multiview",
	"VK_KHR_performance_query",
	"VK_KHR_pipeline_executable_properties",
	"VK_KHR_pipeline_library",
	"VK_KHR_portability_subset",
	"VK_KHR_push_descriptor",
	"VK_KHR_ray_tracing",
	"VK_KHR_relaxed_block_layout",
	"VK_KHR_sampler_mirror_clamp_to_edge",
	"VK_KHR_sampler_ycbcr_conversion",
	"VK_KHR_separate_depth_stencil_layouts",
	"VK_KHR_shader_atomic_int64",
	"VK_KHR_shader_clock",
	"VK_KHR_shader_draw_parameters",
	"VK_KHR_shader_float16_int8",
	"VK_KHR_shader_float_controls",
	"VK_KHR_shader_non_semantic_info",
	"VK_KHR_shader_subgroup_extended_types",
	"VK_KHR_shared_presentable_image",
	"VK_KHR_spirv_1_4",
	"VK_KHR_storage_buffer_storage_class",
	"VK_KHR_surface",
	"VK_KHR_surface_protected_capabilities",
	"VK_KHR_swapchain",
	"VK_KHR_swapchain_mutable_format",
	"VK_KHR_timeline_semaphore",
	"VK_KHR_uniform_buffer_standard_layout",
	"VK_KHR_variable_pointers",
	"VK_KHR_vulkan_memory_model",
	"VK_KHR_wayland_surface",
	"VK_KHR_win32_keyed_mutex",
	"VK_KHR_win32_surface",
	"VK_KHR_xcb_surface",
	"VK_KHR_xlib_surface",
	"VK_EXT_4444_formats",
	"VK_EXT_acquire_xlib_display",
	"VK_EXT_astc_decode_mode",
	"VK_EXT_blend_operation_advanced",
	"VK_EXT_buffer_device_address",
	"VK_EXT_calibrated_timestamps",
	"VK_EXT_conditional_rendering",
	"VK_EXT_conservative_rasterization",
	"VK_EXT_custom_border_color",
	"VK_EXT_debug_marker",
	"VK_EXT_debug_report",
	"VK_EXT_debug_utils",
	"VK_EXT_depth_clip_enable",
	"VK_EXT_depth_range_unrestricted",
	"VK_EXT_descriptor_indexing",
	"VK_EXT_device_memory_report",
	"VK_EXT_direct_mode_display",
	"VK_EXT_directfb_surface",
	"VK_EXT_discard_rectangles",
	"VK_EXT_display_control",
	"VK_EXT_display_surface_counter",
	"VK_EXT_extended_dynamic_state",
	"VK_EXT_external_memory_dma_buf",
	"VK_EXT_external_memory_host",
	"VK_EXT_filter_cubic",
	"VK_EXT_fragment_density_map",
	"VK_EXT_fragment_density_map2",
	"VK_EXT_fragment_shader_interlock",
	"VK_EXT_full_screen_exclusive",
	"VK_EXT_global_priority",
	"VK_EXT_hdr_metadata",
	"VK_EXT_headless_surface",
	"VK_EXT_host_query_reset",
	"VK_EXT_image_drm_format_modifier",
	"VK_EXT_image_robustness",
	"VK_EXT_index_type_uint8",
	"VK_EXT_inline_uniform_block",
	"VK_EXT_line_rasterization",
	"VK_EXT_memory_budget",
	"VK_EXT_memory_priority",
	"VK_EXT_metal_surface",
	"VK_EXT_pci_bus_info",
	"VK_EXT_pipeline_creation_cache_control",
	"VK_EXT_pipeline_creation_feedback",
	"VK_EXT_post_depth_coverage",
	"VK_EXT_private_data",
	"VK_EXT_queue_family_foreign",
	"VK_EXT_robustness2",
	"VK_EXT_sample_locations",
	"VK_EXT_sampler_filter_minmax",
	"VK_EXT_scalar_block_layout",
	"VK_EXT_separate_stencil_usage",
	"VK_EXT_shader_atomic_float",
	"VK_EXT_shader_demote_to_helper_invocation",
	"VK_EXT_shader_image_atomic_int64",
	"VK_EXT_shader_stencil_export",
	"VK_EXT_shader_subgroup_ballot",
	"VK_EXT_shader_subgroup_vote",
	"VK_EXT_shader_viewport_index_layer",
	"VK_EXT_subgroup_size_control",
	"VK_EXT_swapchain_colorspace",
	"VK_EXT_texel_buffer_alignment",
	"VK_EXT_texture_compression_astc_hdr",
	"VK_EXT_tooling_info",
	"VK_EXT_transform_feedback",
	"VK_EXT_validation_cache",
	"VK_EXT_validation_features",
	"VK_EXT_validation_flags",
	"VK_EXT_vertex_attribute_divisor",
	"VK_EXT_ycbcr_image_arrays",
	"VK_AMD_buffer_marker",
	"VK_AMD_device_coherent_memory",
	"VK_AMD_display_native_hdr",
	"VK_AMD_draw_indirect_count",
	"VK_AMD_gcn_shader",
	"VK_AMD_gpu_shader_half_float",
	"VK_AMD_gpu_shader_int16",
	"VK_AMD_memory_overallocation_behavior",
	"VK_AMD_mixed_attachment_samples",
	"VK_AMD_negative_viewport_height",
	"VK_AMD_pipeline_compiler_control",
	"VK_AMD_rasterization_order",
	"VK_AMD_shader_ballot",
	"VK_AMD_shader_core_properties",
	"VK_AMD_shader_core_properties2",
	"VK_AMD_shader_explicit_vertex_parameter",
	"VK_AMD_shader_fragment_mask",
	"VK_AMD_shader_image_load_store_lod",
	"VK_AMD_shader_info",
	"VK_AMD_shader_trinary_minmax",
	"VK_AMD_texture_gather_bias_lod",
	"VK_ANDROID_external_memory_android_hardware_buffer",
	"VK_FUCHSIA_imagepipe_surface",
	"VK_GGP_frame_token",
	"VK_GGP_stream_descriptor_surface",
	"VK_GOOGLE_decorate_string",
	"VK_GOOGLE_display_timing",
	"VK_GOOGLE_hlsl_functionality1",
	"VK_GOOGLE_user_type",
	"VK_IMG_filter_cubic",
	"VK_IMG_format_pvrtc",
	"VK_INTEL_performance_query",
	"VK_INTEL_shader_integer_functions2",
	"VK_MVK_ios_surface",
	"VK_MVK_macos_surface",
	"VK_NN_vi_surface",
	"VK_NV_clip_space_w_scaling",
	"VK_NV_compute_shader_derivatives",
	"VK_NV_cooperative_matrix",
	"VK_NV_corner_sampled_image",
	"VK_NV_coverage_reduction_mode",
	"VK_NV_dedicated_allocation",
	"VK_NV_dedicated_allocation_image_aliasing",
	"VK_NV_device_diagnostic_checkpoints",
	"VK_NV_device_diagnostics_config",
	"VK_NV_device_generated_commands",
	"VK_NV_external_memory",
	"VK_NV_external_memory_capabilities",
	"VK_NV_external_memory_win32",
	"VK_NV_fill_rectangle",
	"VK_NV_fragment_coverage_to_color",
	"VK_NV_fragment_shader_barycentric",
	"VK_NV_framebuffer_mixed_samples",
	"VK_NV_geometry_shader_passthrough",
	"VK_NV_glsl_shader",
	"VK_NV_mesh_shader",
	"VK_NV_ray_tracing",
	"VK_NV_representative_fragment_test",
	"VK_NV_sample_mask_override_coverage",
	"VK_NV_scissor_exclusive",
	"VK_NV_shader_image_footprint",
	"VK_NV_shader_sm_builtins",
	"VK_NV_shader_subgroup_partitioned",
	"VK_NV_shading_rate_image",
	"VK_NV_viewport_array2",
	"VK_NV_viewport_swizzle",
	"VK_NV_win32_keyed_mutex",
	"VK_NVX_image_view_handle",
	"VK_NVX_multiview_per_view_attributes",
	"VK_QCOM_render_pass_shader_resolve",
	"VK_QCOM_render_pass_store_ops",
	"VK_QCOM_render_pass_transform"};

size_t Turbo::Core::TExtensionInfo::GetInstanceExtensionCount(TLayerInfo *layer)
{
	VkResult result = VkResult::VK_ERROR_UNKNOWN;
	uint32_t layer_extension_count = 0;

	do
	{
		if (layer == nullptr || (layer != nullptr && layer->GetName().empty()))
		{
			result = vkEnumerateInstanceExtensionProperties(nullptr, &layer_extension_count, nullptr);
		}
		else
		{
			result = vkEnumerateInstanceExtensionProperties(layer->GetName().c_str(), &layer_extension_count, nullptr);
		}

		if (result == VkResult::VK_SUCCESS)
		{
			break;
		}
	} while (result == VkResult::VK_INCOMPLETE);

	return layer_extension_count;
}

std::vector<Turbo::Core::TExtensionInfo> Turbo::Core::TExtensionInfo::GetInstanceExtensions(TLayerInfo *layer)
{
	std::vector<Turbo::Core::TExtensionInfo> extensions;
	VkResult result = VkResult::VK_ERROR_UNKNOWN;

	do
	{
		uint32_t layer_extension_count = static_cast<uint32_t>(TExtensionInfo::GetInstanceExtensionCount(layer));

		if (layer_extension_count > 0)
		{
			std::vector<VkExtensionProperties> extension_propertiess;
			extension_propertiess.resize(layer_extension_count);

			if (layer == nullptr || (layer != nullptr && layer->GetName().empty()))
			{
				result = vkEnumerateInstanceExtensionProperties(nullptr, &layer_extension_count, extension_propertiess.data());
			}
			else
			{
				result = vkEnumerateInstanceExtensionProperties(layer->GetName().c_str(), &layer_extension_count, extension_propertiess.data());
			}

			if (result == VkResult::VK_SUCCESS)
			{
				extensions.resize(layer_extension_count);

				for (uint32_t extension_index = 0; extension_index < layer_extension_count; extension_index++)
				{
					extensions[extension_index].name = extension_propertiess[extension_index].extensionName;
					extensions[extension_index].extensionType = TExtensionInfo::GetExtensionTypeByExtensionName(extensions[extension_index].name);
					extensions[extension_index].version = extension_propertiess[extension_index].specVersion;
				}

				break;
			}
		}
		else
		{
			break;
		}
	} while (result == VkResult::VK_INCOMPLETE);

	return extensions;
}

size_t Turbo::Core::TExtensionInfo::GetPhysicalDeviceExtensionCount(TPhysicalDevice *physicalDevice, TLayerInfo *layer)
{
	uint32_t physical_device_extension_count = 0;

	if (physicalDevice != nullptr && physicalDevice->GetVkPhysicalDevice() != VK_NULL_HANDLE)
	{
		VkResult result = VkResult::VK_ERROR_UNKNOWN;

		do
		{
			if (layer == nullptr || (layer != nullptr && layer->GetName().empty()))
			{
				result = vkEnumerateDeviceExtensionProperties(physicalDevice->GetVkPhysicalDevice(), nullptr, &physical_device_extension_count, nullptr);
			}
			else
			{
				result = vkEnumerateDeviceExtensionProperties(physicalDevice->GetVkPhysicalDevice(), layer->GetName().c_str(), &physical_device_extension_count, nullptr);
			}

			if (result == VkResult::VK_SUCCESS)
			{
				break;
			}
		} while (result == VkResult::VK_INCOMPLETE);
	}

	return physical_device_extension_count;
}

std::vector<Turbo::Core::TExtensionInfo> Turbo::Core::TExtensionInfo::GetPhysicalDeviceExtensions(TPhysicalDevice *physicalDevice, TLayerInfo *layer)
{
	std::vector<TExtensionInfo> extensions;
	VkResult result = VkResult::VK_ERROR_UNKNOWN;

	do
	{
		uint32_t physical_device_extension_count = static_cast<uint32_t>(TExtensionInfo::GetPhysicalDeviceExtensionCount(physicalDevice, layer));

		if (physical_device_extension_count > 0)
		{
			std::vector<VkExtensionProperties> extension_propertiess;
			extension_propertiess.resize(physical_device_extension_count);

			if (layer == nullptr || (layer != nullptr && layer->GetName().empty()))
			{
				result = vkEnumerateDeviceExtensionProperties(physicalDevice->GetVkPhysicalDevice(), nullptr, &physical_device_extension_count, extension_propertiess.data());
			}
			else
			{
				result = vkEnumerateDeviceExtensionProperties(physicalDevice->GetVkPhysicalDevice(), layer->GetName().c_str(), &physical_device_extension_count, extension_propertiess.data());
			}

			if (result == VkResult::VK_SUCCESS)
			{
				extensions.resize(physical_device_extension_count);

				for (uint32_t extension_index = 0; extension_index < physical_device_extension_count; extension_index++)
				{
					extensions[extension_index].name = extension_propertiess[extension_index].extensionName;
					extensions[extension_index].extensionType = TExtensionInfo::GetExtensionTypeByExtensionName(extensions[extension_index].name);
					extensions[extension_index].version = extension_propertiess[extension_index].specVersion;
				}

				break;
			}
		}
		else
		{
			break;
		}
	} while (result == VkResult::VK_INCOMPLETE);

	return extensions;
}

Turbo::Core::TExtensionType Turbo::Core::TExtensionInfo::GetExtensionTypeByExtensionName(std::string extensionName)
{
	size_t all_extension_name_count = TAllExtensionNames.size();
	for (size_t extension_name_index = 0; extension_name_index < all_extension_name_count;)
	{
		if (TAllExtensionNames[extension_name_index] == extensionName)
		{
			return static_cast<TExtensionType>(extension_name_index);
		}
		++extension_name_index;
	}

	return TExtensionType::UNDEFINED;
}

std::string Turbo::Core::TExtensionInfo::GetExtensionNameByExtensionType(TExtensionType extensionType)
{
	size_t index = static_cast<size_t>(extensionType);

	if (index > TAllExtensionNames.size() - 1)
	{
		return std::string();
	}

	return TAllExtensionNames[index];
}

bool Turbo::Core::TExtensionInfo::IsSupportPhysicalDeviceExtension(TPhysicalDevice *physicalDevice, TExtensionType extensionType)
{
	if (physicalDevice != nullptr && physicalDevice->GetVkPhysicalDevice() != VK_NULL_HANDLE && extensionType != TExtensionType::UNDEFINED && TExtensionInfo::GetPhysicalDeviceExtensionCount(physicalDevice) > 0)
	{
		std::vector<TExtensionInfo> extensions = TExtensionInfo::GetPhysicalDeviceExtensions(physicalDevice);
		size_t extensions_count = extensions.size();
		for (size_t extension_index = 0; extension_index < extensions_count; extension_index++)
		{
			if (extensions[extension_index].GetExtensionType() == extensionType)
			{
				return true;
			}
		}
	}

	return false;
}

Turbo::Core::TExtensionInfo::TExtensionInfo() : Turbo::Core::TInfo()
{
	this->extensionType = TExtensionType::UNDEFINED;
	this->name = "";
	this->version = 0;
}

Turbo::Core::TExtensionInfo::~TExtensionInfo()
{
}

Turbo::Core::TExtensionType Turbo::Core::TExtensionInfo::GetExtensionType()
{
	return this->extensionType;
}

const std::string& Turbo::Core::TExtensionInfo::GetName()
{
	return this->name;
}

uint32_t Turbo::Core::TExtensionInfo::GetVersion()
{
	return this->version;
}

bool Turbo::Core::TExtensionInfo::operator==(const TExtensionInfo &extension) const
{
	if (this->extensionType == extension.extensionType)
	{
		return true;
	}

	return false;
}

bool Turbo::Core::TExtensionInfo::operator!=(const TExtensionInfo &extension) const
{
	if (this->extensionType != extension.extensionType)
	{
		return true;
	}

	return false;
}

std::string Turbo::Core::TExtensionInfo::ToString()
{
	std::stringstream ss;
	ss << this->name << " : extension revision " << this->version;
	return ss.str();
}