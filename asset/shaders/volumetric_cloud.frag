#version 450
layout(push_constant) uniform my_push_constants_t
{
    float time;
    float resolutionX;
    float resolutionY;

    float cameraPosX;
    float cameraPosY;
    float cameraPosZ;

    float lookForwardDirX;
    float lookForwardDirY;
    float lookForwardDirZ;

    // bounding box
    float boxPosX;
    float boxPosY;
    float boxPosZ;

    float boxForwardDirX;
    float boxForwardDirY;
    float boxForwardDirZ;

    float boxHalfDiagonalVectorX;
    float boxHalfDiagonalVectorY;
    float boxHalfDiagonalVectorZ;

    float coverage;

    float power;
}
my_push_constants;
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform texture3D perlinWorleyNoise;
layout(set = 0, binding = 1) uniform sampler mySampler;

#define PI 3.1415926

float hash(float n)
{
    return fract(sin(n) * 43758.5453);
}

float remap(float x, float a, float b, float c, float d)
{
    return (((x - a) / (b - a)) * (d - c)) + c;
}

// 结构体：点法式平面
struct Surface
{
    vec3 point;
    vec3 normal;
};

// 结构体：包围盒结构
struct BoundingBox
{
    vec3 position;
    vec3 forwardDir;
    vec3 halfDiagonalVector;
};

// 结构体：包围盒的正交基
struct BoudingBoxOrthogonalBasis
{
    vec3 forward;
    vec3 up;
    vec3 right;
};

// 结构体：包围盒的尺寸（长宽高）
struct BoundingBoxSize
{
    float strip; // length（length名称和GLSL的length(...)函数重名了，导致报错，改成strip）
    float width;
    float height;
};

// 结构体：包围盒的六个面
struct BoudingBoxSurfaces
{
    Surface positiveForwardSurface;
    Surface negativeForwardSurface;

    Surface positiveUpSurface;
    Surface negativeUpSurface;

    Surface positiveRightSurface;
    Surface negativeRightSurface;
};

// 结构体：射线与包围盒交点信息
struct BoundingBoxIntersections
{
    vec3 firstInterectionPos;
    vec3 secondInterectionPos;
};

// 计算射线在平面上的交点之间的距离
// 无交点返回false
// 有交点返回true，并返回distance距离值
bool CalRaySurafaceIntersectDistance(vec3 rayOrigin, vec3 rayDir, vec3 surfacePoint, vec3 surfaceNormal, out float distance)
{
    // 如果射线方向和平面平行（与平面法线垂直），则永远没有交点
    if (dot(rayDir, surfaceNormal) == 0)
    {
        return false;
    }
    vec3 surface_normal = normalize(surfaceNormal);
    vec3 ray_dir = normalize(rayDir);
    distance = (dot(surfacePoint, surface_normal) - dot(rayOrigin, surface_normal)) / dot(ray_dir, surface_normal);
    return true;
}

// 计算射线在平面上的交点之间的距离
// 对于Surface数据结构的重载
bool CalRaySurafaceIntersectDistance(vec3 rayOrigin, vec3 rayDir, Surface surface, out float distance)
{
    float distance_result = 0;
    bool is_intersect = CalRaySurafaceIntersectDistance(rayOrigin, rayDir, surface.point, surface.normal, distance_result);
    distance = distance_result;
    return is_intersect;
}

// 获得包围盒的长宽高
BoundingBoxSize GetBoundingBoxSize(BoundingBox boundingBox)
{
    BoundingBoxSize result;

    vec3 half_diagonal_vector = boundingBox.halfDiagonalVector;

    result.strip = abs(half_diagonal_vector.x) * 2;
    result.width = abs(half_diagonal_vector.z) * 2;
    result.height = abs(half_diagonal_vector.y) * 2;

    return result;
}

// 获得包围盒的正交基
BoudingBoxOrthogonalBasis GetBoundingBoxOrthogonalBasis(BoundingBox boundingBox)
{
    BoudingBoxOrthogonalBasis result;

    vec3 forward = boundingBox.forwardDir;
    vec3 up = vec3(0, 1, 0);
    vec3 right = cross(forward, up);
    up = cross(right, forward);

    result.forward = normalize(forward);
    result.up = normalize(up);
    result.right = normalize(right);

    return result;
}

// 获得包围盒的六个面
BoudingBoxSurfaces GetBoundingBoxSurfaces(BoundingBox boundingBox)
{
    BoudingBoxSurfaces result;

    BoudingBoxOrthogonalBasis bouding_box_orthogonal_basis = GetBoundingBoxOrthogonalBasis(boundingBox);
    BoundingBoxSize bounding_box_size = GetBoundingBoxSize(boundingBox);

    vec3 bounding_box_pos = boundingBox.position;

    float strip = bounding_box_size.strip;
    float width = bounding_box_size.width;
    float height = bounding_box_size.height;

    vec3 forward = bouding_box_orthogonal_basis.forward;
    vec3 up = bouding_box_orthogonal_basis.up;
    vec3 right = bouding_box_orthogonal_basis.right;

    vec3 positive_forward_surface_normal = forward;
    vec3 positive_forward_surface_point = bounding_box_pos + positive_forward_surface_normal * (width / 2.0);

    vec3 negative_forward_surface_normal = -positive_forward_surface_normal;
    vec3 negative_forward_surface_point = bounding_box_pos + negative_forward_surface_normal * (width / 2.0);

    vec3 positive_up_surface_normal = up;
    vec3 positive_up_surface_point = bounding_box_pos + positive_up_surface_normal * (height / 2.0);

    vec3 negative_up_surface_normal = -positive_up_surface_normal;
    vec3 negative_up_surface_point = bounding_box_pos + negative_up_surface_normal * (height / 2.0);

    vec3 positive_right_surface_normal = right;
    vec3 positive_right_surface_point = bounding_box_pos + positive_right_surface_normal * (strip / 2.0);

    vec3 negative_right_surface_normal = -positive_right_surface_normal;
    vec3 negative_right_surface_point = bounding_box_pos + negative_right_surface_normal * (strip / 2.0);

    result.positiveForwardSurface.point = positive_forward_surface_point;
    result.positiveForwardSurface.normal = positive_forward_surface_normal;

    result.negativeForwardSurface.point = negative_forward_surface_point;
    result.negativeForwardSurface.normal = negative_forward_surface_normal;

    result.positiveUpSurface.point = positive_up_surface_point;
    result.positiveUpSurface.normal = positive_up_surface_normal;

    result.negativeUpSurface.point = negative_up_surface_point;
    result.negativeUpSurface.normal = negative_up_surface_normal;

    result.positiveRightSurface.point = positive_right_surface_point;
    result.positiveRightSurface.normal = positive_right_surface_normal;

    result.negativeRightSurface.point = negative_right_surface_point;
    result.negativeRightSurface.normal = negative_right_surface_normal;

    return result;
}

// 获取点是否在包围盒内（点在包围盒内，包括点在面上）
bool IsPointInBoundingBox(vec3 point, BoundingBox boundingBox)
{
    vec3 bounding_box_pos = boundingBox.position;
    vec3 bounding_box_to_point_vector = point - bounding_box_pos;

    BoudingBoxOrthogonalBasis bounding_box_orthogonal_basis = GetBoundingBoxOrthogonalBasis(boundingBox);
    BoundingBoxSize bounding_box_size = GetBoundingBoxSize(boundingBox);

    vec3 bounding_box_forward = bounding_box_orthogonal_basis.forward;
    vec3 bounding_box_up = bounding_box_orthogonal_basis.up;
    vec3 bounding_box_right = bounding_box_orthogonal_basis.right;

    float project_bounding_box_forward_length = abs(dot(bounding_box_to_point_vector, bounding_box_forward));
    float project_bounding_box_up_length = abs(dot(bounding_box_to_point_vector, bounding_box_up));
    float project_bounding_box_right_length = abs(dot(bounding_box_to_point_vector, bounding_box_right));

    float bounding_box_half_width = bounding_box_size.width / 2.0;
    float bounding_box_half_height = bounding_box_size.height / 2.0;
    float bounding_box_half_strip = bounding_box_size.strip / 2.0;

    float little_compensate = 0.0005;

    bounding_box_half_width += little_compensate;
    bounding_box_half_height += little_compensate;
    bounding_box_half_strip += little_compensate;

    if (project_bounding_box_forward_length > bounding_box_half_width)
    {
        return false;
    }

    if (project_bounding_box_up_length > bounding_box_half_height)
    {
        return false;
    }

    if (project_bounding_box_right_length > bounding_box_half_strip)
    {
        return false;
    }

    return true;
}

// 光线与包围盒求交，并返回所有交点
bool BoundingBoxIntersect(vec3 origin, vec3 dir, BoundingBox boundingBox, out BoundingBoxIntersections intersections)
{
    BoudingBoxSurfaces bounding_box_surface = GetBoundingBoxSurfaces(boundingBox);

    float ray_origin_to_positive_forward_surface_distance = 0;
    bool is_positive_forward_surface_intersect = CalRaySurafaceIntersectDistance(origin, dir, bounding_box_surface.positiveForwardSurface, ray_origin_to_positive_forward_surface_distance);

    float ray_origin_to_negative_forward_surface_distance = 0;
    bool is_negative_forward_surface_intersect = CalRaySurafaceIntersectDistance(origin, dir, bounding_box_surface.negativeForwardSurface, ray_origin_to_negative_forward_surface_distance);

    float ray_origin_to_positive_up_surface_distance = 0;
    bool is_positive_up_surface_intersect = CalRaySurafaceIntersectDistance(origin, dir, bounding_box_surface.positiveUpSurface, ray_origin_to_positive_up_surface_distance);

    float ray_origin_to_negative_up_surface_distance = 0;
    bool is_negative_up_surface_intersect = CalRaySurafaceIntersectDistance(origin, dir, bounding_box_surface.negativeUpSurface, ray_origin_to_negative_up_surface_distance);

    float ray_origin_to_positive_right_surface_distance = 0;
    bool is_positive_right_surface_intersect = CalRaySurafaceIntersectDistance(origin, dir, bounding_box_surface.positiveRightSurface, ray_origin_to_positive_right_surface_distance);

    float ray_origin_to_negative_right_surface_distance = 0;
    bool is_negative_right_surface_intersect = CalRaySurafaceIntersectDistance(origin, dir, bounding_box_surface.negativeRightSurface, ray_origin_to_negative_right_surface_distance);

    vec3 positive_forward_surface_intersection = vec3(0, 0, 0);
    if (is_positive_forward_surface_intersect)
    {
        positive_forward_surface_intersection = origin + dir * ray_origin_to_positive_forward_surface_distance;
        bool is_point_in_bounding_box = IsPointInBoundingBox(positive_forward_surface_intersection, boundingBox);
        if (!is_point_in_bounding_box)
        {
            is_positive_forward_surface_intersect = false;
        }
    }

    vec3 negative_forward_surface_intersection = vec3(0, 0, 0);
    if (is_negative_forward_surface_intersect)
    {
        negative_forward_surface_intersection = origin + dir * ray_origin_to_negative_forward_surface_distance;
        bool is_point_in_bounding_box = IsPointInBoundingBox(negative_forward_surface_intersection, boundingBox);
        if (!is_point_in_bounding_box)
        {
            is_negative_forward_surface_intersect = false;
        }
    }

    vec3 positive_up_surface_intersection = vec3(0, 0, 0);
    if (is_positive_up_surface_intersect)
    {
        positive_up_surface_intersection = origin + dir * ray_origin_to_positive_up_surface_distance;
        bool is_point_in_bounding_box = IsPointInBoundingBox(positive_up_surface_intersection, boundingBox);
        if (!is_point_in_bounding_box)
        {
            is_positive_up_surface_intersect = false;
        }
    }

    vec3 negative_up_surface_intersection = vec3(0, 0, 0);
    if (is_negative_up_surface_intersect)
    {
        negative_up_surface_intersection = origin + dir * ray_origin_to_negative_up_surface_distance;
        bool is_point_in_bounding_box = IsPointInBoundingBox(negative_up_surface_intersection, boundingBox);
        if (!is_point_in_bounding_box)
        {
            is_negative_up_surface_intersect = false;
        }
    }

    vec3 positive_right_surface_intersection = vec3(0, 0, 0);
    if (is_positive_right_surface_intersect)
    {
        positive_right_surface_intersection = origin + dir * ray_origin_to_positive_right_surface_distance;
        bool is_point_in_bounding_box = IsPointInBoundingBox(positive_right_surface_intersection, boundingBox);
        if (!is_point_in_bounding_box)
        {
            is_positive_right_surface_intersect = false;
        }
    }

    vec3 negative_right_surface_intersection = vec3(0, 0, 0);
    if (is_negative_right_surface_intersect)
    {
        negative_right_surface_intersection = origin + dir * ray_origin_to_negative_right_surface_distance;
        bool is_point_in_bounding_box = IsPointInBoundingBox(negative_right_surface_intersection, boundingBox);
        if (!is_point_in_bounding_box)
        {
            is_negative_right_surface_intersect = false;
        }
    }

    uint intersection_count = 0;
    if (is_positive_forward_surface_intersect)
    {
        intersection_count = intersection_count + 1;
    }

    if (is_negative_forward_surface_intersect)
    {
        intersection_count = intersection_count + 1;
    }

    if (is_positive_up_surface_intersect)
    {
        intersection_count = intersection_count + 1;
    }

    if (is_negative_up_surface_intersect)
    {
        intersection_count = intersection_count + 1;
    }

    if (is_positive_right_surface_intersect)
    {
        intersection_count = intersection_count + 1;
    }

    if (is_negative_right_surface_intersect)
    {
        intersection_count = intersection_count + 1;
    }

    if (intersection_count == 0)
    {
        return false;
    }

    if (intersection_count == 1)
    {
        return false;
    }

    if (intersection_count == 2)
    {
        float origin_to_surface_distance[2];
        uint index = 0;
        if (is_positive_forward_surface_intersect)
        {
            origin_to_surface_distance[index] = ray_origin_to_positive_forward_surface_distance;
            index = index + 1;
        }

        if (is_negative_forward_surface_intersect)
        {
            origin_to_surface_distance[index] = ray_origin_to_negative_forward_surface_distance;
            index = index + 1;
        }

        if (is_positive_up_surface_intersect)
        {
            origin_to_surface_distance[index] = ray_origin_to_positive_up_surface_distance;
            index = index + 1;
        }

        if (is_negative_up_surface_intersect)
        {
            origin_to_surface_distance[index] = ray_origin_to_negative_up_surface_distance;
            index = index + 1;
        }

        if (is_positive_right_surface_intersect)
        {
            origin_to_surface_distance[index] = ray_origin_to_positive_right_surface_distance;
            index = index + 1;
        }

        if (is_negative_right_surface_intersect)
        {
            origin_to_surface_distance[index] = ray_origin_to_negative_right_surface_distance;
            index = index + 1;
        }

        float min_distance = min(origin_to_surface_distance[0], origin_to_surface_distance[1]);
        float max_distance = max(origin_to_surface_distance[0], origin_to_surface_distance[1]);

        if (min_distance > 0 && max_distance > 0)
        {
            intersections.firstInterectionPos = origin + dir * min_distance;
            intersections.secondInterectionPos = origin + dir * max_distance;

            return true;
        }

        if (min_distance < 0 && max_distance > 0)
        {
            intersections.firstInterectionPos = origin;
            intersections.secondInterectionPos = origin + dir * max_distance;

            return true;
        }

        return false;
    }

    return false;
}

// 获取盒子内点相对三维噪音纹理采样点
vec3 GetSamplePointPosition(vec3 point, BoundingBox boundingBox)
{
    vec3 result = vec3(0, 0, 0);

    vec3 bounding_box_pos = boundingBox.position;

    BoundingBoxSize bounding_box_size = GetBoundingBoxSize(boundingBox);

    BoudingBoxOrthogonalBasis bounding_box_orthogonal_basis = GetBoundingBoxOrthogonalBasis(boundingBox);

    vec3 bounding_box_forward = bounding_box_orthogonal_basis.forward;
    vec3 bounding_box_up = bounding_box_orthogonal_basis.up;
    vec3 bounding_box_right = bounding_box_orthogonal_basis.right;

    vec3 lower_right_corner_pos = bounding_box_pos + bounding_box_forward * (bounding_box_size.width / 2) + bounding_box_up * (-bounding_box_size.height / 2) + bounding_box_right * (bounding_box_size.strip / 2);

    vec3 lower_right_corner_to_point = point - lower_right_corner_pos;

    vec3 lower_right_corner_strip_dir = normalize(bounding_box_right * (-1));
    vec3 lower_right_corner_up_dir = normalize(bounding_box_up * (1));
    vec3 lower_right_corner_width_dir = normalize(bounding_box_forward * (-1));

    float project_strip = dot(lower_right_corner_to_point, lower_right_corner_strip_dir);
    float project_height = dot(lower_right_corner_to_point, lower_right_corner_up_dir);
    float project_width = dot(lower_right_corner_to_point, lower_right_corner_width_dir);

    result = vec3(project_strip / bounding_box_size.strip, project_height / bounding_box_size.height, project_width / bounding_box_size.width);
    return result;
}

vec3 move = vec3(0, 0, 0);

float GetPerlinWorleyCloudDensity(vec3 point, float coverage, BoundingBox boundingBox)
{
    vec3 sample_point = GetSamplePointPosition(point, boundingBox);
    vec4 fbm = texture(sampler3D(perlinWorleyNoise, mySampler), sample_point + move, 0);
    float perlin_worley = fbm.x;
    vec3 worleys = fbm.yzw;
    float worly_fbm = worleys.x * 0.625 + worleys.y * 0.125 + worleys.z * 0.25;
    float cloud = remap(perlin_worley, worly_fbm - 1., 1., 0., 1.);
    cloud = pow(cloud, 0.3 + 1.5 * smoothstep(0.2, 0.5, sample_point.y));
    float density = remap(cloud, 1 - coverage, 1., 0., 1.); // coverage
    return density;
}

float HenyeyGreensteinPhaseFunction(float g, vec3 sunDir, vec3 viewDir)
{
    float mu = dot(sunDir, viewDir);
    float gSqr = g * g;
    float oofp = 1.0 / (4.0 * 3.141592);
    return oofp * (1.0 - gSqr) / pow(1.0 - 2.0 * g * mu + gSqr, 1.5);
}

float HenyeyGreensteinPhaseFunction(float g, float mu)
{
    float gSqr = g * g;
    float oofp = 1.0 / (4.0 * 3.141592);
    return oofp * (1.0 - gSqr) / pow(1.0 - 2.0 * g * mu + gSqr, 1.5);
}

vec3 MultipleOctaves(float density, float mu, float stepL, vec3 sigmaExtinction)
{
    vec3 luminance = vec3(0);
    const float octaves = 1.0;

    // Attenuation
    float a = 1.0;
    // Contribution
    float b = 1.0;
    // Phase attenuation
    float c = 1.0;

    float phase;

    for (float i = 0.0; i < octaves; i++)
    {
        // Two-lobed HG
        phase = mix(HenyeyGreensteinPhaseFunction(-0.3 * c, mu), HenyeyGreensteinPhaseFunction(0.3 * c, mu), 0.7);
        luminance += b * phase * exp(-stepL * density * sigmaExtinction * a);
        // Lower is brighter
        a *= 0.2;
        // Higher is brighter
        b *= 0.5;
        c *= 0.5;
    }
    return luminance;
}

vec3 LightRay(vec3 origin, vec3 dir, float mu, vec3 sigmaExtinction, float coverage, BoundingBox boundingBox)
{
    vec3 result = vec3(1, 1, 1);

    BoundingBoxIntersections intersections;
    bool is_intersect = BoundingBoxIntersect(origin, dir, boundingBox, intersections);

    if (is_intersect)
    {
        vec3 start_pos = intersections.firstInterectionPos;
        vec3 end_pos = intersections.secondInterectionPos;

        int max_step = 6;
        float step = abs(length(end_pos - start_pos)) / max_step;
        float light_ray_density = 0.0;

        vec3 point = start_pos;
        for (int j = 0; j < max_step; j++)
        {
            point = start_pos + dir * step * j;
            float density = GetPerlinWorleyCloudDensity(point, coverage, boundingBox);
            if (density <= 0.0)
            {
                break;
            }

            light_ray_density += density;
        }

        vec3 beers_law = MultipleOctaves(light_ray_density, mu, step, sigmaExtinction);

        return mix(beers_law * 2.0 * (1.0 - (exp(-step * light_ray_density * 2.0 * sigmaExtinction))), beers_law, 0.5 + 0.5 * mu);
    }

    return result;
}

const vec3 albedo = vec3(0.85, 0.82, 0.90);
const vec3 sun_dir = normalize(vec3(1000, 1000, 1000));
const vec3 sunLightColour = vec3(1, 1, 1);

vec3 RayMarchingBoundingBox(vec3 origin, vec3 dir, BoundingBox boundingBox, float coverage, float time)
{
    vec3 color = vec3(0, 0, 0);
    BoundingBoxIntersections intersections;
    bool is_intersect = BoundingBoxIntersect(origin, dir, boundingBox, intersections);

    float speed = 0.01;
    move = move + vec3(speed * time, 0, 0);

    if (is_intersect)
    {
        vec3 start_pos = intersections.firstInterectionPos;
        vec3 end_pos = intersections.secondInterectionPos;

        int max_step = 128;
        float step = abs(length(end_pos - start_pos)) / max_step;
        vec3 T = vec3(1, 1, 1);

        float mu = dot(sun_dir, dir);
        float phase_function = HenyeyGreensteinPhaseFunction(0.5, sun_dir, dir); // 相函数计算

        vec3 sunLight = sunLightColour * my_push_constants.power;

        vec3 radiance = vec3(1, 1, 1);

        vec3 point = start_pos;
        for (int i = 0; i < max_step; ++i)
        {
            point = start_pos + dir * step * i * hash(dot(point, vec3(12.256, 2.646, 6.356)));
            float density = GetPerlinWorleyCloudDensity(point, coverage, boundingBox);

            vec3 sigmaS = vec3(1.);               /*散射系数*/
            vec3 sigmaA = vec3(0.);               /*吸收系数*/
            vec3 sigmaE = (sigmaS + sigmaA);      /*消亡系数*/
            vec3 sampleSigmaS = sigmaS * density; // 采样点的散射
            vec3 sampleSigmaE = sigmaE * density; // 采样点的消亡
            //<光照>
            if (density > 0.0)
            {
                vec3 ambient = vec3(0.412, 0.513, 0.607);

                vec3 luminance = /*0.1 **/ ambient + sunLight * phase_function * LightRay(point, sun_dir, mu, sigmaE, coverage, boundingBox);
                luminance *= sampleSigmaS;
                vec3 transmittance = exp(-sampleSigmaE * step);
                color += T * (luminance - luminance * transmittance) / sampleSigmaE;
                T *= transmittance;
                if (length(T) < 0.01)
                {
                    break;
                }
            }
            //</光照>
        }
        return color;
    }

    return color;
}

void main()
{
    float iTime = my_push_constants.time;
    vec2 iResolution = vec2(my_push_constants.resolutionX, my_push_constants.resolutionY);
    vec2 fragCoord = vec2(uv.x * iResolution.x, uv.y * iResolution.y);
    vec3 cameraPos = vec3(my_push_constants.cameraPosX, my_push_constants.cameraPosY, my_push_constants.cameraPosZ);
    vec3 lookForwardDir = vec3(my_push_constants.lookForwardDirX, my_push_constants.lookForwardDirY, my_push_constants.lookForwardDirZ);

    vec3 boudingBoxPos = vec3(my_push_constants.boxPosX, my_push_constants.boxPosY, my_push_constants.boxPosZ);
    vec3 boudingBoxForwardDir = vec3(my_push_constants.boxForwardDirX, my_push_constants.boxForwardDirY, my_push_constants.boxForwardDirZ);
    vec3 boudingBoxHalfDiagonalVector = vec3(my_push_constants.boxHalfDiagonalVectorX, my_push_constants.boxHalfDiagonalVectorY, my_push_constants.boxHalfDiagonalVectorZ);

    float coverage = my_push_constants.coverage;

    BoundingBox bounding_box;
    bounding_box.position = boudingBoxPos;
    bounding_box.forwardDir = boudingBoxForwardDir;
    bounding_box.halfDiagonalVector = boudingBoxHalfDiagonalVector;

    vec3 forwardDir = vec3(my_push_constants.lookForwardDirX, my_push_constants.lookForwardDirY, my_push_constants.lookForwardDirZ);
    vec3 upDir = vec3(0, 1, 0);
    vec3 rightDir = cross(forwardDir, upDir);
    upDir = cross(rightDir, forwardDir);

    forwardDir = normalize(forwardDir);
    rightDir = normalize(rightDir);
    upDir = normalize(upDir);

    vec2 screen_ndc = vec2(uv.x, 1 - uv.y) * 2 - 1; // Normalized Device Corrdinate:[-1,1]

    float near = 0.1;
    float aspect = iResolution.y / iResolution.x; // height/width
    float horizontalFov = PI / 3.0;               // 60

    float screenHalfWidth = near * tan(horizontalFov / 2.0);
    float screenHalfHeight = screenHalfWidth * aspect;

    vec3 pixel_pos = cameraPos + forwardDir * near + rightDir * screen_ndc.x * screenHalfWidth + upDir * screen_ndc.y * screenHalfHeight;

    vec3 rayDir = pixel_pos - cameraPos;
    rayDir = normalize(rayDir);

    vec3 color = RayMarchingBoundingBox(cameraPos, rayDir, bounding_box, coverage, iTime);

    outColor = vec4(color, 1.0);
}