#include <iostream>
#include <ply.h>

#include <TDevice.h>
#include <TDeviceQueue.h>
#include <TEngine.h>
#include <TPhysicalDevice.h>
#include <TVulkanAllocator.h>

#include <TBuffer.h>
#include <TCommandBuffer.h>
#include <TCommandBufferPool.h>
#include <TImage.h>
#include <TImageView.h>

#include <TShader.h>

#include <TAttachment.h>
#include <TComputePipeline.h>
#include <TGraphicsPipeline.h>
#include <TRenderPass.h>
#include <TSubpass.h>

#include <TDescriptorPool.h>
#include <TDescriptorSet.h>
#include <TDescriptorSetLayout.h>
#include <TFramebuffer.h>

#include <TFence.h>
#include <TSemaphore.h>

#include <fstream>

#include <GLFW/glfw3.h>

#include <TSurface.h>
#include <TSwapchain.h>

#include <math.h>

#include <TPipelineDescriptorSet.h>
#include <TSampler.h>

#include <glm/ext.hpp>

#include <TVulkanLoader.h>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

#include <imgui.h>

std::string asset_root(TURBO_ASSET_ROOT);

static bool g_MouseJustPressed[ImGuiMouseButton_COUNT] = {};
static GLFWcursor *g_MouseCursors[ImGuiMouseCursor_COUNT] = {};

std::string ReadTextFile(const std::string &filename)
{
    std::vector<std::string> data;

    std::ifstream file;

    file.open(filename, std::ios::in);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    return std::string{(std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>())};
}

#define TEX_SIZE 512

typedef struct POSITION
{
    float x;
    float y;
    float z;
    float w;
} POSITION;

typedef struct COLOR
{
    float r;
    float g;
    float b;
    float a;
} COLOR;

struct MATRIXS_BUFFER_DATA
{
    glm::mat4 m;
    glm::mat4 v;
    glm::mat4 p;
};

typedef struct Point
{
    POSITION position;
    COLOR color;
} Point;

typedef struct PlyData
{
    std::vector<Point> points;
    POSITION min;
    POSITION max;
} PlyData;

const std::string IMGUI_VERT_SHADER_STR = ReadTextFile(asset_root + "/shaders/imgui.vert");

const std::string IMGUI_FRAG_SHADER_STR = ReadTextFile(asset_root + "/shaders/imgui.frag");

const std::string MY_VERT_SHADER_STR = ReadTextFile(asset_root + "/shaders/PointCloud.vert");

const std::string MY_FRAG_SHADER_STR = ReadTextFile(asset_root + "/shaders/PointCloud.frag");

PlyData LoadPly(const std::string &url)
{
    PlyData result;

    POSITION min = {0, 0, 0, 0};
    POSITION max = {0, 0, 0, 0};

    int elements_count = 0;
    char **elements;
    int file_type;
    float version;

    PlyFile *ply = ply_open_for_reading(const_cast<char *>(url.c_str()), &elements_count, &elements, &file_type, &version);
    if (ply != nullptr)
    {
        // std::cout << "ply != nullptr" << std::endl;

        for (size_t elem_index = 0; elem_index < elements_count; elem_index++)
        {
            auto elem_name = elements[elem_index];
            if (equal_strings("vertex", elem_name))
            {
                int num_elems = 0;
                int num_props = 0;

                auto plist = ply_get_element_description(ply, elem_name, &num_elems, &num_props);
                // std::cout << "elem_name:" << elem_name << std::endl;

                struct PlyVectex
                {
                    float x = 0;
                    float y = 0;
                    float z = 0;
                    unsigned char r = 0;
                    unsigned char g = 0;
                    unsigned char b = 0;
                    unsigned char a = 0;
                };

                PlyProperty vert_props[] = {
                    {"x", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVectex, x), 0, 0, 0, 0},
                    {"y", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVectex, y), 0, 0, 0, 0},
                    {"z", PLY_FLOAT, PLY_FLOAT, offsetof(PlyVectex, z), 0, 0, 0, 0},
                    {"red", PLY_UCHAR, PLY_UCHAR, offsetof(PlyVectex, r), 0, 0, 0, 0},
                    {"green", PLY_UCHAR, PLY_UCHAR, offsetof(PlyVectex, g), 0, 0, 0, 0},
                    {"blue", PLY_UCHAR, PLY_UCHAR, offsetof(PlyVectex, b), 0, 0, 0, 0},
                    {"alpha", PLY_UCHAR, PLY_UCHAR, offsetof(PlyVectex, a), 0, 0, 0, 0},
                };

                ply_get_property(ply, elem_name, &vert_props[0]);
                ply_get_property(ply, elem_name, &vert_props[1]);
                ply_get_property(ply, elem_name, &vert_props[2]);
                ply_get_property(ply, elem_name, &vert_props[3]);
                ply_get_property(ply, elem_name, &vert_props[4]);
                ply_get_property(ply, elem_name, &vert_props[5]);
                ply_get_property(ply, elem_name, &vert_props[6]);

                for (size_t i = 0; i < num_elems; i++)
                {
                    PlyVectex vertex;
                    ply_get_element(ply, &vertex);

                    {
                        Point ply_point_data;
                        ply_point_data.position.x = vertex.x;
                        ply_point_data.position.y = vertex.y;
                        ply_point_data.position.z = vertex.z;
                        ply_point_data.position.w = 0;
                        ply_point_data.color.r = vertex.r / 255.0f;
                        ply_point_data.color.g = vertex.g / 255.0f;
                        ply_point_data.color.b = vertex.b / 255.0f;
                        ply_point_data.color.a = vertex.a / 255.0f;

                        result.points.push_back(ply_point_data);

                        {
                            {
                                if (min.x > ply_point_data.position.x)
                                {
                                    min.x = ply_point_data.position.x;
                                }

                                if (min.y > ply_point_data.position.y)
                                {
                                    min.y = ply_point_data.position.y;
                                }

                                if (min.z > ply_point_data.position.z)
                                {
                                    min.z = ply_point_data.position.z;
                                }
                            }

                            {
                                if (max.x < ply_point_data.position.x)
                                {
                                    min.x = ply_point_data.position.x;
                                }

                                if (max.y < ply_point_data.position.y)
                                {
                                    min.y = ply_point_data.position.y;
                                }

                                if (max.z < ply_point_data.position.z)
                                {
                                    min.z = ply_point_data.position.z;
                                }
                            }
                        }
                    }

                    {
                        // std::cout << "i:" << i << std::endl;
                        // std::cout << "\tx: " << vertex.x << std::endl;
                        // std::cout << "\ty: " << vertex.y << std::endl;
                        // std::cout << "\tz: " << vertex.z << std::endl;
                        // std::cout << "\tr: " << (int)vertex.r << std::endl;
                        // std::cout << "\tg: " << (int)vertex.g << std::endl;
                        // std::cout << "\tb: " << (int)vertex.b << std::endl;
                        // std::cout << "\ta: " << (int)vertex.a << std::endl;
                        //
                        // std::cout << std::endl;
                    }
                }
            }
        }

        ply_close(ply);
    }
    else
    {
        std::cout << "ply == nullptr" << std::endl;
    }

    result.min = min;
    result.max = max;

    //#define AXIS_SCALE 0.001
    //    {
    //        for (int i = 0; i < 1000; i++)
    //        {
    //            Point ply_point_data;
    //            ply_point_data.position.x = i * AXIS_SCALE;
    //            ply_point_data.position.y = 0;
    //            ply_point_data.position.z = 0;
    //            ply_point_data.position.w = 0;
    //            ply_point_data.color.r = 1;
    //            ply_point_data.color.g = 0;
    //            ply_point_data.color.b = 0;
    //            ply_point_data.color.a = 1;
    //
    //            result.points.push_back(ply_point_data);
    //        }
    //    }
    //
    //    {
    //        for (int i = 0; i < 1000; i++)
    //        {
    //            Point ply_point_data;
    //            ply_point_data.position.x = 0;
    //            ply_point_data.position.y = i * AXIS_SCALE;
    //            ply_point_data.position.z = 0;
    //            ply_point_data.position.w = 0;
    //            ply_point_data.color.r = 0;
    //            ply_point_data.color.g = 1;
    //            ply_point_data.color.b = 0;
    //            ply_point_data.color.a = 1;
    //
    //            result.points.push_back(ply_point_data);
    //        }
    //    }
    //
    //    {
    //        for (int i = 0; i < 1000; i++)
    //        {
    //            Point ply_point_data;
    //            ply_point_data.position.x = 0;
    //            ply_point_data.position.y = 0;
    //            ply_point_data.position.z = i * AXIS_SCALE;
    //            ply_point_data.position.w = 0;
    //            ply_point_data.color.r = 0;
    //            ply_point_data.color.g = 0;
    //            ply_point_data.color.b = 1;
    //            ply_point_data.color.a = 1;
    //
    //            result.points.push_back(ply_point_data);
    //        }
    //    }
    //
    //    {
    //        for (int i = 0; i < 1000; i++)
    //        {
    //            Point ply_point_data;
    //            ply_point_data.position.x = i * AXIS_SCALE;
    //            ply_point_data.position.y = i * AXIS_SCALE;
    //            ply_point_data.position.z = i * AXIS_SCALE;
    //            ply_point_data.position.w = 0;
    //            ply_point_data.color.r = 1;
    //            ply_point_data.color.g = 1;
    //            ply_point_data.color.b = 0;
    //            ply_point_data.color.a = 1;
    //
    //            result.points.push_back(ply_point_data);
    //        }
    //    }

    return result;
}

typedef struct PointsPositionImage
{
    Turbo::Core::TRefPtr<Turbo::Core::TImage> image;
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> imageView;
} PointsPositionImage;

typedef struct PointsColorImage
{
    Turbo::Core::TRefPtr<Turbo::Core::TImage> image;
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> imageView;
} PointsColorImage;

typedef struct PointsImageData
{
    PointsPositionImage pointsPositionImage;
    PointsColorImage pointsColorImage;
    uint32_t count = 0;
} PointsImageData;

std::vector<Point> PlyDatasToPoints(const std::vector<PlyData> &plyDatas)
{
    std::vector<Point> result;

    {
        for (auto &ply_data_item : plyDatas)
        {
            for (auto &point_item : ply_data_item.points)
            {
                result.push_back(point_item);
            }
        }
    }

    return result;
}

std::vector<PointsImageData> CreateAllPointsImageData(const std::vector<Point> &points, Turbo::Core::TRefPtr<Turbo::Core::TDevice> device, Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> queue, Turbo::Core::TRefPtr<Turbo::Core::TCommandBufferPool> commandPool)
{
    std::vector<PointsImageData> result;

    auto tex_size = TEX_SIZE;
    auto tex_content_size = tex_size * tex_size;

    auto loop_count = points.size() / tex_content_size;
    auto residue_count = points.size() % tex_content_size;

    auto create_points_position_image = [&](const std::vector<POSITION> &pointsPosition) -> PointsPositionImage {
        PointsPositionImage points_position_image_result;

        {
            Turbo::Core::TRefPtr<Turbo::Core::TImage> points_position_image = nullptr;
            Turbo::Core::TRefPtr<Turbo::Core::TImageView> points_position_image_view = nullptr;
            {
                Turbo::Core::TRefPtr<Turbo::Core::TBuffer> points_position_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, pointsPosition.size() * sizeof(POSITION));
                void *volumn_ptr = points_position_buffer->Map();
                memcpy(volumn_ptr, pointsPosition.data(), pointsPosition.size() * sizeof(POSITION));
                points_position_buffer->Unmap();

                points_position_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R32G32B32A32_SFLOAT, tex_size, tex_size, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST | Turbo::Core::TImageUsageBits::IMAGE_SAMPLED | Turbo::Core::TImageUsageBits::IMAGE_STORAGE, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);

                Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> temp_command_buffer = commandPool->Allocate();
                temp_command_buffer->Begin();
                temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::HOST_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::HOST_WRITE_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, points_position_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                {
                    uint32_t row = pointsPosition.size() / tex_size;
                    uint32_t residue = pointsPosition.size() - row * tex_size;

                    {
                        uint32_t copy_buffer_offset = 0;
                        uint32_t copy_image_offset_x = 0;
                        uint32_t copy_image_offset_y = 0;
                        uint32_t copy_image_offset_z = 0;
                        uint32_t copy_width = tex_size;
                        uint32_t copy_height = row;
                        uint32_t copy_depth = 1;
                        uint32_t copy_mip_level = 0;
                        temp_command_buffer->CmdCopyBufferToImage(points_position_buffer, points_position_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, copy_buffer_offset, 0, 0, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, copy_mip_level, 0, 1, copy_image_offset_x, copy_image_offset_y, copy_image_offset_z, copy_width, copy_height, copy_depth);
                    }

                    if (residue > 0)
                    {
                        uint32_t copy_buffer_offset = row * tex_size * sizeof(POSITION);
                        uint32_t copy_image_offset_x = 0;
                        uint32_t copy_image_offset_y = row;
                        uint32_t copy_image_offset_z = 0;
                        uint32_t copy_width = residue;
                        uint32_t copy_height = 1;
                        uint32_t copy_depth = 1;
                        uint32_t copy_mip_level = 0;
                        temp_command_buffer->CmdCopyBufferToImage(points_position_buffer, points_position_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, copy_buffer_offset, /*copy_width*/ 0, /*copy_height*/ 0, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, copy_mip_level, 0, 1, copy_image_offset_x, copy_image_offset_y, copy_image_offset_z, copy_width, copy_height, copy_depth);
                    }
                }
                temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::FRAGMENT_SHADER_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TAccessBits::SHADER_READ_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::GENERAL, points_position_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                temp_command_buffer->End();

                Turbo::Core::TRefPtr<Turbo::Core::TFence> fence = new Turbo::Core::TFence(device);

                queue->Submit(temp_command_buffer, fence);

                fence->WaitUntil();

                commandPool->Free(temp_command_buffer);

                points_position_image_view = new Turbo::Core::TImageView(points_position_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, points_position_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
            }

            points_position_image_result.image = points_position_image;
            points_position_image_result.imageView = points_position_image_view;
        }

        return points_position_image_result;
    };

    auto create_points_color_image = [&](const std::vector<COLOR> &pointsColor) -> PointsColorImage {
        PointsColorImage points_color_image_result;

        {
            Turbo::Core::TRefPtr<Turbo::Core::TImage> points_color_image = nullptr;
            Turbo::Core::TRefPtr<Turbo::Core::TImageView> points_color_image_view = nullptr;
            {
                Turbo::Core::TRefPtr<Turbo::Core::TBuffer> points_color_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, pointsColor.size() * sizeof(COLOR));
                void *volumn_ptr = points_color_buffer->Map();
                memcpy(volumn_ptr, pointsColor.data(), pointsColor.size() * sizeof(COLOR));
                points_color_buffer->Unmap();

                points_color_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R32G32B32A32_SFLOAT, tex_size, tex_size, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST | Turbo::Core::TImageUsageBits::IMAGE_SAMPLED | Turbo::Core::TImageUsageBits::IMAGE_STORAGE, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);

                Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> temp_command_buffer = commandPool->Allocate();
                temp_command_buffer->Begin();
                temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::HOST_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::HOST_WRITE_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, points_color_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                {
                    uint32_t row = pointsColor.size() / tex_size;
                    uint32_t residue = pointsColor.size() - row * tex_size;

                    {
                        uint32_t copy_buffer_offset = 0;
                        uint32_t copy_image_offset_x = 0;
                        uint32_t copy_image_offset_y = 0;
                        uint32_t copy_image_offset_z = 0;
                        uint32_t copy_width = tex_size;
                        uint32_t copy_height = row;
                        uint32_t copy_depth = 1;
                        uint32_t copy_mip_level = 0;
                        temp_command_buffer->CmdCopyBufferToImage(points_color_buffer, points_color_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, copy_buffer_offset, 0, 0, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, copy_mip_level, 0, 1, copy_image_offset_x, copy_image_offset_y, copy_image_offset_z, copy_width, copy_height, copy_depth);
                    }

                    if (residue > 0)
                    {
                        uint32_t copy_buffer_offset = row * tex_size * sizeof(COLOR);
                        uint32_t copy_image_offset_x = 0;
                        uint32_t copy_image_offset_y = row;
                        uint32_t copy_image_offset_z = 0;
                        uint32_t copy_width = residue;
                        uint32_t copy_height = 1;
                        uint32_t copy_depth = 1;
                        uint32_t copy_mip_level = 0;
                        temp_command_buffer->CmdCopyBufferToImage(points_color_buffer, points_color_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, copy_buffer_offset, /*copy_width*/ 0, /*copy_height*/ 0, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, copy_mip_level, 0, 1, copy_image_offset_x, copy_image_offset_y, copy_image_offset_z, copy_width, copy_height, copy_depth);
                    }
                }
                temp_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::FRAGMENT_SHADER_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TAccessBits::SHADER_READ_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::GENERAL, points_color_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                temp_command_buffer->End();

                Turbo::Core::TRefPtr<Turbo::Core::TFence> fence = new Turbo::Core::TFence(device);

                queue->Submit(temp_command_buffer, fence);

                fence->WaitUntil();

                commandPool->Free(temp_command_buffer);

                points_color_image_view = new Turbo::Core::TImageView(points_color_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, points_color_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
            }

            points_color_image_result.image = points_color_image;
            points_color_image_result.imageView = points_color_image_view;
        }

        return points_color_image_result;
    };

    for (size_t loop_index = 0; loop_index < loop_count; loop_index++)
    {
        std::vector<POSITION> points_position;
        std::vector<COLOR> points_color;
        {
            auto offset = loop_index * tex_content_size;

            for (size_t point_index = 0; point_index < tex_content_size; point_index++)
            {
                points_position.push_back(points[point_index + offset].position);
                points_color.push_back(points[point_index + offset].color);
            }
        }

        auto points_position_image = create_points_position_image(points_position);
        auto points_color_image = create_points_color_image(points_color);

        PointsImageData temp_points_image_data;
        temp_points_image_data.pointsPositionImage = points_position_image;
        temp_points_image_data.pointsColorImage = points_color_image;
        temp_points_image_data.count = points_position.size();

        result.push_back(temp_points_image_data);
    }

    if (residue_count > 0)
    {
        std::vector<POSITION> points_position;
        std::vector<COLOR> points_color;
        {
            auto offset = loop_count * tex_content_size;

            for (size_t point_index = 0; point_index < residue_count; point_index++)
            {
                points_position.push_back(points[point_index + offset].position);
                points_color.push_back(points[point_index + offset].color);
            }

            auto points_position_image = create_points_position_image(points_position);
            auto points_color_image = create_points_color_image(points_color);

            PointsImageData temp_points_image_data;
            temp_points_image_data.pointsPositionImage = points_position_image;
            temp_points_image_data.pointsColorImage = points_color_image;
            temp_points_image_data.count = points_position.size();

            result.push_back(temp_points_image_data);
        }
    }

    return result;
}

int main()
{
    std::vector<PlyData> ply_datas;
    {
        ply_datas.push_back(LoadPly(asset_root + "/models/points.ply"));
        //... 在此增加其他 ply 点云数据;
    }

    auto points = PlyDatasToPoints(ply_datas);
    ply_datas.clear();

    size_t all_point_count = points.size();

    std::cout << "points::size::" << points.size() << std::endl;
    std::cout << "Vulkan Version:" << Turbo::Core::TVulkanLoader::Instance()->GetVulkanVersion().ToString() << std::endl;

    std::vector<Turbo::Core::TLayerInfo> support_layers;
    std::vector<Turbo::Core::TExtensionInfo> instance_support_extensions;
    {
        Turbo::Core::TRefPtr<Turbo::Core::TInstance> temp_instance = new Turbo::Core::TInstance();
        support_layers = temp_instance->GetSupportLayers();
        instance_support_extensions = temp_instance->GetSupportExtensions();
    }

    Turbo::Core::TLayerInfo khronos_validation;
    for (Turbo::Core::TLayerInfo &layer : support_layers)
    {
        if (layer.GetLayerType() == Turbo::Core::TLayerType::VK_LAYER_KHRONOS_VALIDATION)
        {
            khronos_validation = layer;
            break;
        }
    }

    std::vector<Turbo::Core::TLayerInfo> enable_layer;
    if (khronos_validation.GetLayerType() != Turbo::Core::TLayerType::UNDEFINED)
    {
        enable_layer.push_back(khronos_validation);
    }

    std::vector<Turbo::Core::TExtensionInfo> enable_instance_extensions;
    for (Turbo::Core::TExtensionInfo &extension : instance_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_WIN32_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_WAYLAND_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XCB_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
        else if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_XLIB_SURFACE)
        {
            enable_instance_extensions.push_back(extension);
        }
    }

    Turbo::Core::TVersion instance_version(1, 2, 0, 0);
    Turbo::Core::TRefPtr<Turbo::Core::TInstance> instance = new Turbo::Core::TInstance(&enable_layer, &enable_instance_extensions, &instance_version);
    Turbo::Core::TRefPtr<Turbo::Core::TPhysicalDevice> physical_device = instance->GetBestPhysicalDevice();

    if (!glfwInit())
        return -1;
    GLFWwindow *window;
    int window_width = 1920 / 2.0;
    int window_height = 1080 / 2.0;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(window_width, window_height, "Turbo", NULL, NULL);
    VkSurfaceKHR vk_surface_khr = VK_NULL_HANDLE;
    VkInstance vk_instance = instance->GetVkInstance();
    glfwCreateWindowSurface(vk_instance, window, NULL, &vk_surface_khr);

    Turbo::Core::TPhysicalDeviceFeatures physical_device_features = {};
    physical_device_features.sampleRateShading = true;
    physical_device_features.fillModeNonSolid = true;

    std::vector<Turbo::Core::TExtensionInfo> enable_device_extensions;
    std::vector<Turbo::Core::TExtensionInfo> physical_device_support_extensions = physical_device->GetSupportExtensions();
    for (Turbo::Core::TExtensionInfo &extension : physical_device_support_extensions)
    {
        if (extension.GetExtensionType() == Turbo::Core::TExtensionType::VK_KHR_SWAPCHAIN)
        {
            enable_device_extensions.push_back(extension);
        }
    }

    Turbo::Core::TRefPtr<Turbo::Core::TDevice> device = new Turbo::Core::TDevice(physical_device, nullptr, &enable_device_extensions, &physical_device_features);
    Turbo::Core::TRefPtr<Turbo::Core::TDeviceQueue> queue = device->GetBestGraphicsQueue();

    Turbo::Core::TRefPtr<Turbo::Extension::TSurface> surface = new Turbo::Extension::TSurface(device, nullptr, vk_surface_khr);
    uint32_t max_image_count = surface->GetMaxImageCount();
    uint32_t min_image_count = surface->GetMinImageCount();

    uint32_t swapchain_image_count = max_image_count <= min_image_count ? min_image_count : max_image_count - 1;

    Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> swapchain = new Turbo::Extension::TSwapchain(surface, swapchain_image_count, Turbo::Core::TFormatType::B8G8R8A8_SRGB, 1, Turbo::Core::TImageUsageBits::IMAGE_COLOR_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_SRC | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, true);

    std::vector<Turbo::Core::TImage *> swapchain_images = swapchain->GetImages();

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
    for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
    {
        Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        swapchain_image_views.push_back(swapchain_view);
    }

    Turbo::Core::TRefPtr<Turbo::Core::TCommandBufferPool> command_pool = new Turbo::Core::TCommandBufferPool(queue);
    Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> command_buffer = command_pool->Allocate();

    auto all_points_image_data = CreateAllPointsImageData(points, device, queue, command_pool);
    points.clear();

    MATRIXS_BUFFER_DATA matrixs_buffer_data = {};

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(-10.0f, 0.0f, 0));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)swapchain->GetWidth() / (float)swapchain->GetHeight(), 0.1f, 100.0f);

    matrixs_buffer_data.m = model;
    matrixs_buffer_data.v = view;
    matrixs_buffer_data.p = projection;

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> matrixs_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_UNIFORM_BUFFER | Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, sizeof(matrixs_buffer_data));
    void *mvp_ptr = matrixs_buffer->Map();
    memcpy(mvp_ptr, &matrixs_buffer_data, sizeof(matrixs_buffer_data));
    matrixs_buffer->Unmap();

    Turbo::Core::TRefPtr<Turbo::Core::TImage> depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth(), swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

    Turbo::Core::TRefPtr<Turbo::Core::TVertexShader> my_vertex_shader = new Turbo::Core::TVertexShader(device, Turbo::Core::TShaderLanguage::GLSL, MY_VERT_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TFragmentShader> my_fragment_shader = new Turbo::Core::TFragmentShader(device, Turbo::Core::TShaderLanguage::GLSL, MY_FRAG_SHADER_STR);

    std::vector<Turbo::Core::TDescriptorSize> descriptor_sizes;
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::UNIFORM_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::COMBINED_IMAGE_SAMPLER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::SAMPLER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::SAMPLED_IMAGE, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_IMAGE, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::UNIFORM_TEXEL_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_TEXEL_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_BUFFER, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::UNIFORM_BUFFER_DYNAMIC, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::STORAGE_BUFFER_DYNAMIC, 1000));
    descriptor_sizes.push_back(Turbo::Core::TDescriptorSize(Turbo::Core::TDescriptorType::INPUT_ATTACHMENT, 1000));

    Turbo::Core::TRefPtr<Turbo::Core::TDescriptorPool> descriptor_pool = new Turbo::Core::TDescriptorPool(device, descriptor_sizes.size() * 1000, descriptor_sizes);

    Turbo::Core::TSubpass subpass(Turbo::Core::TPipelineType::Graphics);
    subpass.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL);                // swapchain color image
    subpass.SetDepthStencilAttachmentReference(1, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL); // depth image

    Turbo::Core::TSubpass subpass1(Turbo::Core::TPipelineType::Graphics);
    subpass1.AddColorAttachmentReference(0, Turbo::Core::TImageLayout::COLOR_ATTACHMENT_OPTIMAL); // swapchain color image

    std::vector<Turbo::Core::TSubpass> subpasses;
    subpasses.push_back(subpass);  // subpass 0
    subpasses.push_back(subpass1); // subpass 1

    Turbo::Core::TAttachment swapchain_color_attachment(swapchain_images[0]->GetFormat(), swapchain_images[0]->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::PRESENT_SRC_KHR);
    Turbo::Core::TAttachment depth_attachment(depth_image->GetFormat(), depth_image->GetSampleCountBits(), Turbo::Core::TLoadOp::CLEAR, Turbo::Core::TStoreOp::STORE, Turbo::Core::TLoadOp::DONT_CARE, Turbo::Core::TStoreOp::DONT_CARE, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

    std::vector<Turbo::Core::TAttachment> attachemts;
    attachemts.push_back(swapchain_color_attachment);
    attachemts.push_back(depth_attachment);

    Turbo::Core::TRefPtr<Turbo::Core::TRenderPass> render_pass = new Turbo::Core::TRenderPass(device, attachemts, subpasses);

    // Turbo::Core::TVertexBinding position_binding(0, sizeof(POSITION), Turbo::Core::TVertexRate::VERTEX);
    // position_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32B32_SFLOAT, 0); // position

    Turbo::Core::TViewport viewport(0, 0, surface->GetCurrentWidth(), surface->GetCurrentHeight(), 0, 1);
    Turbo::Core::TScissor scissor(0, 0, surface->GetCurrentWidth(), surface->GetCurrentHeight());

    std::vector<Turbo::Core::TVertexBinding> vertex_bindings;

    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> graphics_pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 0, vertex_bindings, my_vertex_shader, my_fragment_shader, Turbo::Core::TTopologyType::POINT_LIST, false, false, false, Turbo::Core::TPolygonMode::POINT, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, true, true, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, 0, 0, false, Turbo::Core::TLogicOp::NO_OP, true, Turbo::Core::TBlendFactor::SRC_ALPHA, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendOp::ADD, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendFactor::ZERO, Turbo::Core::TBlendOp::ADD);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet>> graphics_pipeline_descriptor_sets;
    for (auto &points_image_data_item : all_points_image_data)
    {
        Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> pipeline_descriptor_set = descriptor_pool->Allocate(graphics_pipeline->GetPipelineLayout());
        std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> points_pos_image_views;
        points_pos_image_views.push_back(points_image_data_item.pointsPositionImage.imageView);

        std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> points_color_image_views;
        points_color_image_views.push_back(points_image_data_item.pointsColorImage.imageView);

        std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> matrixs_buffers;
        matrixs_buffers.push_back(matrixs_buffer);

        pipeline_descriptor_set->BindData(0, 0, 0, matrixs_buffers);
        pipeline_descriptor_set->BindData(0, 1, 0, points_pos_image_views);
        pipeline_descriptor_set->BindData(0, 2, 0, points_color_image_views);

        graphics_pipeline_descriptor_sets.push_back(pipeline_descriptor_set);
    }

    // Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> graphics_pipeline_descriptor_set = descriptor_pool->Allocate(graphics_pipeline->GetPipelineLayout());
    //
    // std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> points_pos_image_views;
    // points_pos_image_views.push_back(points_position_image_view);
    //
    // std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> points_color_image_views;
    // points_color_image_views.push_back(points_color_image_view);
    //
    // std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> matrixs_buffers;
    // matrixs_buffers.push_back(matrixs_buffer);
    //
    // graphics_pipeline_descriptor_set->BindData(0, 0, 0, matrixs_buffers);
    // graphics_pipeline_descriptor_set->BindData(0, 1, 0, points_pos_image_views);
    // graphics_pipeline_descriptor_set->BindData(0, 2, 0, points_color_image_views);

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer>> swpachain_framebuffers;
    for (Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_image_view_item : swapchain_image_views)
    {
        std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> image_views;
        image_views.push_back(swapchain_image_view_item);
        image_views.push_back(depth_image_view);

        Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, image_views);
        swpachain_framebuffers.push_back(swapchain_framebuffer);
    }

    //<IMGUI>
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    Turbo::Core::TRefPtr<Turbo::Core::TSampler> imgui_sampler = new Turbo::Core::TSampler(device);

    Turbo::Core::TRefPtr<Turbo::Core::TShader> imgui_vertex_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::VERTEX, Turbo::Core::TShaderLanguage::GLSL, IMGUI_VERT_SHADER_STR);
    Turbo::Core::TRefPtr<Turbo::Core::TShader> imgui_fragment_shader = new Turbo::Core::TShader(device, Turbo::Core::TShaderType::FRAGMENT, Turbo::Core::TShaderLanguage::GLSL, IMGUI_FRAG_SHADER_STR);

    Turbo::Core::TVertexBinding imgui_vertex_binding(0, sizeof(ImDrawVert), Turbo::Core::TVertexRate::VERTEX);
    imgui_vertex_binding.AddAttribute(0, Turbo::Core::TFormatType::R32G32_SFLOAT, IM_OFFSETOF(ImDrawVert, pos));  // position
    imgui_vertex_binding.AddAttribute(1, Turbo::Core::TFormatType::R32G32_SFLOAT, IM_OFFSETOF(ImDrawVert, uv));   // uv
    imgui_vertex_binding.AddAttribute(2, Turbo::Core::TFormatType::R8G8B8A8_UNORM, IM_OFFSETOF(ImDrawVert, col)); // color

    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TShader>> imgui_shaders;
    imgui_shaders.push_back(imgui_vertex_shader);
    imgui_shaders.push_back(imgui_fragment_shader);

    std::vector<Turbo::Core::TVertexBinding> imgui_vertex_bindings;
    imgui_vertex_bindings.push_back(imgui_vertex_binding);

    Turbo::Core::TRefPtr<Turbo::Core::TGraphicsPipeline> imgui_pipeline = new Turbo::Core::TGraphicsPipeline(render_pass, 1, imgui_vertex_bindings, imgui_shaders, Turbo::Core::TTopologyType::TRIANGLE_LIST, false, false, false, Turbo::Core::TPolygonMode::FILL, Turbo::Core::TCullModeBits::MODE_BACK_BIT, Turbo::Core::TFrontFace::CLOCKWISE, false, 0, 0, 0, 1, false, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, false, false, Turbo::Core::TCompareOp::LESS_OR_EQUAL, false, false, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TStencilOp::KEEP, Turbo::Core::TCompareOp::ALWAYS, 0, 0, 0, 0, 0, false, Turbo::Core::TLogicOp::NO_OP, true, Turbo::Core::TBlendFactor::SRC_ALPHA, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendOp::ADD, Turbo::Core::TBlendFactor::ONE_MINUS_SRC_ALPHA, Turbo::Core::TBlendFactor::ZERO, Turbo::Core::TBlendOp::ADD);

    unsigned char *imgui_font_pixels;
    int imgui_font_width, imgui_font_height;
    io.Fonts->GetTexDataAsRGBA32(&imgui_font_pixels, &imgui_font_width, &imgui_font_height);
    size_t imgui_upload_size = imgui_font_width * imgui_font_height * 4 * sizeof(char);

    Turbo::Core::TRefPtr<Turbo::Core::TImage> imgui_font_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::R8G8B8A8_UNORM, imgui_font_width, imgui_font_height, 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_SAMPLED | Turbo::Core::TImageUsageBits::IMAGE_TRANSFER_DST, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY);
    Turbo::Core::TRefPtr<Turbo::Core::TImageView> imgui_font_image_view = new Turbo::Core::TImageView(imgui_font_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, imgui_font_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
    {
        Turbo::Core::TRefPtr<Turbo::Core::TBuffer> imgui_font_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_TRANSFER_SRC, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, imgui_upload_size);
        void *imgui_font_ptr = imgui_font_buffer->Map();
        memcpy(imgui_font_ptr, imgui_font_pixels, imgui_upload_size);
        imgui_font_buffer->Unmap();

        Turbo::Core::TRefPtr<Turbo::Core::TCommandBuffer> imgui_copy_command_buffer = command_pool->Allocate();
        imgui_copy_command_buffer->Begin();
        imgui_copy_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::HOST_BIT, Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TAccessBits::HOST_WRITE_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TImageLayout::UNDEFINED, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, imgui_font_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        imgui_copy_command_buffer->CmdCopyBufferToImage(imgui_font_buffer, imgui_font_image, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, 0, imgui_font_width, imgui_font_height, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 0, 1, 0, 0, 0, imgui_font_width, imgui_font_height, 1);
        imgui_copy_command_buffer->CmdTransformImageLayout(Turbo::Core::TPipelineStageBits::TRANSFER_BIT, Turbo::Core::TPipelineStageBits::FRAGMENT_SHADER_BIT, Turbo::Core::TAccessBits::TRANSFER_WRITE_BIT, Turbo::Core::TAccessBits::SHADER_READ_BIT, Turbo::Core::TImageLayout::TRANSFER_DST_OPTIMAL, Turbo::Core::TImageLayout::SHADER_READ_ONLY_OPTIMAL, imgui_font_image, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
        imgui_copy_command_buffer->End();

        Turbo::Core::TRefPtr<Turbo::Core::TFence> imgui_font_copy_fence = new Turbo::Core::TFence(device);
        queue->Submit(imgui_copy_command_buffer, imgui_font_copy_fence);

        imgui_font_copy_fence->WaitUntil();
    }

    std::vector<std::pair<Turbo::Core::TRefPtr<Turbo::Core::TImageView>, Turbo::Core::TRefPtr<Turbo::Core::TSampler>>> imgui_combined_image_samplers;
    imgui_combined_image_samplers.push_back(std::make_pair(imgui_font_image_view, imgui_sampler));

    Turbo::Core::TRefPtr<Turbo::Core::TPipelineDescriptorSet> imgui_pipeline_descriptor_set = descriptor_pool->Allocate(imgui_pipeline->GetPipelineLayout());
    imgui_pipeline_descriptor_set->BindData(0, 0, 0, imgui_combined_image_samplers);

    io.Fonts->TexID = (ImTextureID)(intptr_t)(imgui_font_image->GetVkImage());

    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> imgui_vertex_buffer = nullptr;
    Turbo::Core::TRefPtr<Turbo::Core::TBuffer> imgui_index_buffer = nullptr;
    //</IMGUI>

    glm::vec3 camera_position = glm::vec3(0, 0, 0);
    glm::vec3 look_forward = glm::vec3(0, 0, 1);

    float horizontal_angle = 0;
    float vertical_angle = 0;

    glm::vec2 previous_mouse_pos = glm::vec2(0, 0);
    glm::vec2 current_mouse_pos = glm::vec2(0, 0);

    float angle = 0;
    float _time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        //<Begin Rendering>
        uint32_t current_image_index = UINT32_MAX;
        Turbo::Core::TRefPtr<Turbo::Core::TSemaphore> wait_image_ready = new Turbo::Core::TSemaphore(device, Turbo::Core::TPipelineStageBits::COLOR_ATTACHMENT_OUTPUT_BIT);
        Turbo::Core::TResult result = swapchain->AcquireNextImageUntil(wait_image_ready, nullptr, &current_image_index);
        switch (result)
        {
        case Turbo::Core::TResult::SUCCESS: {
            // successed get image and go on rendering

            // because we just have one command buffer, so we should reset the command buffer for each frame
            // If we create command buffer for each swapchain image, we don't need to reset it each frame

            //<IMGUI Update>
            int window_w, window_h;
            int display_w, display_h;
            glfwGetWindowSize(window, &window_w, &window_h);
            glfwGetFramebufferSize(window, &display_w, &display_h);
            io.DisplaySize = ImVec2((float)window_w, (float)window_h);
            if (window_w > 0 && window_h > 0)
            {
                io.DisplayFramebufferScale = ImVec2((float)display_w / window_w, (float)display_h / window_h);
            }
            double current_time = glfwGetTime();
            io.DeltaTime = _time > 0.0 ? (float)(current_time - _time) : (float)(1.0f / 60.0f);
            _time = current_time;

            // UpdateMousePosAndButtons
            {
                // Update buttons
                ImGuiIO &io = ImGui::GetIO();
                for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
                {
                    // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
                    io.MouseDown[i] = g_MouseJustPressed[i] || glfwGetMouseButton(window, i) != 0;
                    g_MouseJustPressed[i] = false;
                }

                // Update mouse position
                const ImVec2 mouse_pos_backup = io.MousePos;
                io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

                const bool focused = glfwGetWindowAttrib(window, GLFW_FOCUSED) != 0;
                if (focused)
                {
                    if (io.WantSetMousePos)
                    {
                        glfwSetCursorPos(window, (double)mouse_pos_backup.x, (double)mouse_pos_backup.y);
                    }
                    else
                    {
                        double mouse_x, mouse_y;
                        glfwGetCursorPos(window, &mouse_x, &mouse_y);
                        io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
                    }
                }
            }

            // IUpdateMouseCursor
            {
                ImGuiIO &io = ImGui::GetIO();
                if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) || glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
                    break;

                ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
                if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
                {
                    // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                }
                else
                {
                    // Show OS mouse cursor
                    // FIXME-PLATFORM: Unfocused windows seems to fail changing the mouse cursor with GLFW 3.2, but 3.3 works here.
                    glfwSetCursor(window, g_MouseCursors[imgui_cursor] ? g_MouseCursors[imgui_cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
            }

            // UpdateKeyboard
            {
                ImVec2 mouse_pos = io.MousePos;
                current_mouse_pos = glm::vec2(mouse_pos.x, mouse_pos.y);
                glm::vec2 mouse_pos_delte = current_mouse_pos - previous_mouse_pos;
                previous_mouse_pos = current_mouse_pos;
                mouse_pos_delte.y = -mouse_pos_delte.y;

                int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
                if (state == GLFW_PRESS)
                {
                    horizontal_angle += mouse_pos_delte.x * 0.2;
                    vertical_angle += mouse_pos_delte.y * 0.2;

                    if (vertical_angle > 90)
                    {
                        vertical_angle = 90;
                    }

                    if (vertical_angle < -90)
                    {
                        vertical_angle = -90;
                    }
                }

                float delte_time = io.DeltaTime;
                float speed = 1;

                glm::vec3 forward_axis = glm::vec3(1, 0, 0);
                glm::mat4 forward_rotate_mat = glm::rotate(glm::mat4(1), glm::radians(horizontal_angle), glm::vec3(0, 0, 1));
                forward_rotate_mat = glm::rotate(forward_rotate_mat, glm::radians(-vertical_angle), glm::vec3(0, 1, 0));

                look_forward = forward_rotate_mat * glm::vec4(forward_axis, 1.0);
                look_forward = glm::normalize(look_forward);

                glm::vec3 forward_dir = look_forward;                  // 向前向量
                glm::vec3 up_dir = glm::vec3(0, 0, 1);                 // 向上向量
                glm::vec3 right_dir = glm::cross(up_dir, forward_dir); // 向右向量
                up_dir = glm::cross(right_dir, forward_dir);

                right_dir = glm::normalize(right_dir);
                up_dir = glm::normalize(up_dir);

                int key_W_state = glfwGetKey(window, GLFW_KEY_W);
                if (key_W_state == GLFW_PRESS)
                {
                    // TODO: 向前
                    camera_position += forward_dir * speed * delte_time;
                }

                int key_A_state = glfwGetKey(window, GLFW_KEY_A);
                if (key_A_state == GLFW_PRESS)
                {
                    // TODO: 向左
                    camera_position += -right_dir * speed * delte_time;
                }

                int key_S_state = glfwGetKey(window, GLFW_KEY_S);
                if (key_S_state == GLFW_PRESS)
                {
                    // TODO: 向后
                    camera_position += -forward_dir * speed * delte_time;
                }

                int key_D_state = glfwGetKey(window, GLFW_KEY_D);
                if (key_D_state == GLFW_PRESS)
                {
                    // TODO: 向右
                    camera_position += right_dir * speed * delte_time;
                }

                model = glm::mat4(1.0f);

                glm::vec3 look_point = camera_position + forward_dir;
                view = glm::lookAt(camera_position, look_point, up_dir);
                projection = glm::perspective(glm::radians(45.0f), (float)(swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth()) / (float)(swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight()), 0.1f, 300.0f);

                matrixs_buffer_data.m = model;
                matrixs_buffer_data.v = view;
                matrixs_buffer_data.p = projection;

                void *_ptr = matrixs_buffer->Map();
                memcpy(_ptr, &matrixs_buffer_data, sizeof(matrixs_buffer_data));
                matrixs_buffer->Unmap();
            }

            ImGui::NewFrame();

            {
                ImGui::Begin(TURBO_PROJECT_NAME); // Create a window called "Hello, world!" and append into it.
                ImGui::Text("W,A,S,D to move.");
                ImGui::Text("Push down and drag mouse right button to rotate view.");

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::Text((std::string("All point count : ") + std::to_string(all_point_count)).c_str());
                ImGui::End();
            }
            //</IMGUI Update>

            Turbo::Core::TViewport frame_viewport(0, 0, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight(), 0, 1);
            Turbo::Core::TScissor frame_scissor(0, 0, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight());

            std::vector<Turbo::Core::TViewport> frame_viewports;
            frame_viewports.push_back(frame_viewport);

            std::vector<Turbo::Core::TScissor> frame_scissors;
            frame_scissors.push_back(frame_scissor);

            command_buffer->Begin();

            // Graphics Pipeline Draw
            command_buffer->CmdBeginRenderPass(render_pass, swpachain_framebuffers[current_image_index]);
            command_buffer->CmdBindPipeline(graphics_pipeline);
            // command_buffer->CmdBindPipelineDescriptorSet(graphics_pipeline_descriptor_set);
            command_buffer->CmdSetViewport(frame_viewports);
            command_buffer->CmdSetScissor(frame_scissors);
            for (size_t points_image_index = 0; points_image_index < all_points_image_data.size(); points_image_index++)
            {
                auto draw_instance_count = all_points_image_data[points_image_index].count;
                command_buffer->CmdBindPipelineDescriptorSet(graphics_pipeline_descriptor_sets[points_image_index]);
                command_buffer->CmdDraw(1, draw_instance_count, 0, 0);
            }
            command_buffer->CmdNextSubpass();

            //<IMGUI Rendering>
            ImGui::Render();
            ImDrawData *draw_data = ImGui::GetDrawData();
            const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

            if (!is_minimized)
            {
                // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
                int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
                int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
                if (fb_width <= 0 || fb_height <= 0)
                {
                    break;
                }

                if (draw_data->TotalVtxCount > 0)
                {
                    size_t vertex_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
                    size_t index_size = draw_data->TotalIdxCount * sizeof(ImDrawIdx);

                    if (imgui_vertex_buffer != nullptr)
                    {
                        imgui_vertex_buffer = nullptr;
                    }

                    if (imgui_index_buffer != nullptr)
                    {
                        imgui_index_buffer = nullptr;
                    }

                    imgui_vertex_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_VERTEX_BUFFER, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, vertex_size);
                    imgui_index_buffer = new Turbo::Core::TBuffer(device, 0, Turbo::Core::TBufferUsageBits::BUFFER_INDEX_BUFFER, Turbo::Core::TMemoryFlagsBits::HOST_ACCESS_SEQUENTIAL_WRITE, index_size);

                    ImDrawVert *vtx_dst = (ImDrawVert *)imgui_vertex_buffer->Map();
                    ImDrawIdx *idx_dst = (ImDrawIdx *)imgui_index_buffer->Map();
                    for (int n = 0; n < draw_data->CmdListsCount; n++)
                    {
                        const ImDrawList *cmd_list = draw_data->CmdLists[n];
                        memcpy(vtx_dst, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
                        memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
                        vtx_dst += cmd_list->VtxBuffer.Size;
                        idx_dst += cmd_list->IdxBuffer.Size;
                    }
                    imgui_vertex_buffer->Unmap();
                    imgui_index_buffer->Unmap();

                    command_buffer->CmdBindPipeline(imgui_pipeline);
                    command_buffer->CmdBindPipelineDescriptorSet(imgui_pipeline_descriptor_set);

                    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> imgui_vertex_buffers;
                    imgui_vertex_buffers.push_back(imgui_vertex_buffer);
                    command_buffer->CmdBindVertexBuffers(imgui_vertex_buffers);
                    command_buffer->CmdBindIndexBuffer(imgui_index_buffer, 0, sizeof(ImDrawIdx) == 2 ? Turbo::Core::TIndexType::UINT16 : Turbo::Core::TIndexType::UINT32);

                    float scale[2];
                    scale[0] = 2.0f / draw_data->DisplaySize.x;
                    scale[1] = 2.0f / draw_data->DisplaySize.y;
                    float translate[2];
                    translate[0] = -1.0f - draw_data->DisplayPos.x * scale[0];
                    translate[1] = -1.0f - draw_data->DisplayPos.y * scale[1];

                    command_buffer->CmdPushConstants(sizeof(float) * 0, sizeof(float) * 2, scale);
                    command_buffer->CmdPushConstants(sizeof(float) * 2, sizeof(float) * 2, translate);

                    ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
                    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

                    int global_vtx_offset = 0;
                    int global_idx_offset = 0;

                    for (int n = 0; n < draw_data->CmdListsCount; n++)
                    {
                        const ImDrawList *cmd_list = draw_data->CmdLists[n];
                        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
                        {
                            const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];
                            if (pcmd->UserCallback != NULL)
                            {
                                // User callback, registered via ImDrawList::AddCallback()
                                // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                                if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                                {
                                    command_buffer->CmdBindPipeline(imgui_pipeline);
                                    command_buffer->CmdBindPipelineDescriptorSet(imgui_pipeline_descriptor_set);

                                    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TBuffer>> __imgui_vertex_buffers;
                                    __imgui_vertex_buffers.push_back(imgui_vertex_buffer);
                                    command_buffer->CmdBindVertexBuffers(imgui_vertex_buffers);
                                    command_buffer->CmdBindIndexBuffer(imgui_index_buffer, 0, sizeof(ImDrawIdx) == 2 ? Turbo::Core::TIndexType::UINT16 : Turbo::Core::TIndexType::UINT32);

                                    float __scale[2];
                                    __scale[0] = 2.0f / draw_data->DisplaySize.x;
                                    __scale[1] = 2.0f / draw_data->DisplaySize.y;
                                    float __translate[2];
                                    __translate[0] = -1.0f - draw_data->DisplayPos.x * scale[0];
                                    __translate[1] = -1.0f - draw_data->DisplayPos.y * scale[1];

                                    command_buffer->CmdPushConstants(sizeof(float) * 0, sizeof(float) * 2, __scale);
                                    command_buffer->CmdPushConstants(sizeof(float) * 2, sizeof(float) * 2, __translate);
                                }
                                else
                                    pcmd->UserCallback(cmd_list, pcmd);
                            }
                            else
                            {
                                // Project scissor/clipping rectangles into framebuffer space
                                ImVec4 clip_rect;
                                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
                                {
                                    // Negative offsets are illegal for vkCmdSetScissor
                                    if (clip_rect.x < 0.0f)
                                        clip_rect.x = 0.0f;
                                    if (clip_rect.y < 0.0f)
                                        clip_rect.y = 0.0f;

                                    // Apply scissor/clipping rectangle
                                    VkRect2D scissor;
                                    scissor.offset.x = (int32_t)(clip_rect.x);
                                    scissor.offset.y = (int32_t)(clip_rect.y);
                                    scissor.extent.width = (uint32_t)(clip_rect.z - clip_rect.x);
                                    scissor.extent.height = (uint32_t)(clip_rect.w - clip_rect.y);

                                    Turbo::Core::TScissor imgui_scissor(scissor.offset.x, scissor.offset.y, scissor.extent.width, scissor.extent.height);
                                    std::vector<Turbo::Core::TScissor> imgui_scissors;
                                    imgui_scissors.push_back(imgui_scissor);
                                    command_buffer->CmdSetScissor(imgui_scissors);

                                    // Draw
                                    command_buffer->CmdDrawIndexed(pcmd->ElemCount, 1, pcmd->IdxOffset + global_idx_offset, pcmd->VtxOffset + global_vtx_offset, 0);
                                }
                            }
                        }
                        global_idx_offset += cmd_list->IdxBuffer.Size;
                        global_vtx_offset += cmd_list->VtxBuffer.Size;
                    }
                }
            }
            //</IMGUI Rendering>

            command_buffer->CmdEndRenderPass();
            command_buffer->End();

            Turbo::Core::TRefPtr<Turbo::Core::TFence> fence = new Turbo::Core::TFence(device);
            std::vector<Turbo::Core::TRefPtr<Turbo::Core::TSemaphore>> wait_semaphores;
            wait_semaphores.push_back(wait_image_ready);

            queue->Submit(wait_semaphores, {}, command_buffer, fence);

            fence->WaitUntil(); // or you can use semaphore to wait for get higher performance

            command_buffer->Reset(); // you can create an command buffer each for one swapchain image,for now just one command buffer

            Turbo::Core::TResult present_result = queue->Present(swapchain, current_image_index);
            switch (present_result)
            {
            case Turbo::Core::TResult::MISMATCH: {
                // the size of the window had changed you need to recreate swapchain

                // waite for idle
                device->WaitIdle();

                // destroy related resource
                // clear old swapchain image
                swapchain_images.clear();

                // destroy swapchain image views
                swapchain_image_views.clear();

                // destroy depth image and view
                // destroy framebuffer
                swpachain_framebuffers.clear();

                // recreate swapchain
                Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> old_swapchain = swapchain;
                Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> new_swapchain = new Turbo::Extension::TSwapchain(old_swapchain);

                swapchain = new_swapchain;

                // recreate swapchain image views
                swapchain_images = swapchain->GetImages();
                for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
                {
                    Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                    swapchain_image_views.push_back(swapchain_view);
                }

                // recreate depth image and view
                depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
                depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

                // recreate framebuffer
                for (Turbo::Core::TRefPtr<Turbo::Core::TImageView> image_view_item : swapchain_image_views)
                {
                    std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
                    swapchain_image_views.push_back(image_view_item);
                    swapchain_image_views.push_back(depth_image_view);

                    Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, swapchain_image_views);
                    swpachain_framebuffers.push_back(swapchain_framebuffer);
                }
            }
            break;
            default: {
                //
            }
            break;
            }
        }
        break;
        case Turbo::Core::TResult::TIMEOUT: {
            // you need to wait, or do something else
        }
        break;
        case Turbo::Core::TResult::NOT_READY: {
            // you need to wait, or do something else
        }
        break;
        case Turbo::Core::TResult::MISMATCH: {
            // the size of the window had changed you need to recreate swapchain

            // waite for idle
            device->WaitIdle();

            // destroy related resource
            // clear old swapchain image
            swapchain_images.clear();

            // destroy swapchain image views
            swapchain_image_views.clear();

            // destroy depth image and view
            // destroy framebuffer
            swpachain_framebuffers.clear();

            // recreate swapchain
            Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> old_swapchain = swapchain;
            Turbo::Core::TRefPtr<Turbo::Extension::TSwapchain> new_swapchain = new Turbo::Extension::TSwapchain(old_swapchain);

            swapchain = new_swapchain;

            // recreate swapchain image views
            swapchain_images = swapchain->GetImages();
            for (Turbo::Core::TRefPtr<Turbo::Core::TImage> swapchain_image_item : swapchain_images)
            {
                Turbo::Core::TRefPtr<Turbo::Core::TImageView> swapchain_view = new Turbo::Core::TImageView(swapchain_image_item, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, Turbo::Core::TFormatType::B8G8R8A8_SRGB, Turbo::Core::TImageAspectBits::ASPECT_COLOR_BIT, 0, 1, 0, 1);
                swapchain_image_views.push_back(swapchain_view);
            }

            // recreate depth image and view
            depth_image = new Turbo::Core::TImage(device, 0, Turbo::Core::TImageType::DIMENSION_2D, Turbo::Core::TFormatType::D32_SFLOAT, swapchain->GetWidth() <= 0 ? 1 : swapchain->GetWidth(), swapchain->GetHeight() <= 0 ? 1 : swapchain->GetHeight(), 1, 1, 1, Turbo::Core::TSampleCountBits::SAMPLE_1_BIT, Turbo::Core::TImageTiling::OPTIMAL, Turbo::Core::TImageUsageBits::IMAGE_DEPTH_STENCIL_ATTACHMENT | Turbo::Core::TImageUsageBits::IMAGE_INPUT_ATTACHMENT, Turbo::Core::TMemoryFlagsBits::DEDICATED_MEMORY, Turbo::Core::TImageLayout::UNDEFINED);
            depth_image_view = new Turbo::Core::TImageView(depth_image, Turbo::Core::TImageViewType::IMAGE_VIEW_2D, depth_image->GetFormat(), Turbo::Core::TImageAspectBits::ASPECT_DEPTH_BIT, 0, 1, 0, 1);

            // recreate framebuffer
            for (Turbo::Core::TRefPtr<Turbo::Core::TImageView> image_view_item : swapchain_image_views)
            {
                std::vector<Turbo::Core::TRefPtr<Turbo::Core::TImageView>> swapchain_image_views;
                swapchain_image_views.push_back(image_view_item);
                swapchain_image_views.push_back(depth_image_view);

                Turbo::Core::TRefPtr<Turbo::Core::TFramebuffer> swapchain_framebuffer = new Turbo::Core::TFramebuffer(render_pass, swapchain_image_views);
                swpachain_framebuffers.push_back(swapchain_framebuffer);
            }
        }
        break;
        default: {
            //
        }
        break;
        }
        //</End Rendering>
    }

    for (auto pipeline_descriptor_set_item : graphics_pipeline_descriptor_sets)
    {
        descriptor_pool->Free(pipeline_descriptor_set_item);
    }

    command_pool->Free(command_buffer);

    glfwTerminate();

    return 0;
}