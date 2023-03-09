//--------------------------------------
// CONFIG
//--------------------------------------

#define ANIMATE_WATER 1

// Bloom
#define BLOOM_SIZE (0.5)
#define BLOOM_THRESHOLD (1.01)
#define BLOOM_RANGE (0.3)
#define BLOOM_FRAME_BLEND (0.2)

//--------------------------------------
// DEFINES & CONSTANTS
//--------------------------------------

// utility defines
#define ZERO (min(1, 0))
#define X_AXIS vec3(1, 0, 0)
#define Y_AXIS vec3(0, 1, 0)
#define Z_AXIS vec3(0, 0, 1)

#define PI 3.14159265
#define TAU (2 * PI)
#define PHI (sqrt(5) * 0.5 + 0.5)

// Materials
#define MAT_WOOD 101
#define MAT_LEAFS 102
#define MAT_GROUND 201
#define MAT_GRASS 202
#define MAT_UNDERWATER 301
#define MAT_BONE 401
#define MAT_FRUIT 501
#define MAT_PLANT 701
#define MAT_STONE 801

//--------------------------------------
// TYPES
//--------------------------------------

//--------------------------------------
// UTILITY FUNCTIONS
//--------------------------------------
mat3 rotation(vec3 axis, float angle)
{
    // axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return inverse(mat3(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c));
}

//--------------------------------------
// BASIC SDFs SHAPES
// https://iquilezles.org/articles/distfunctions
//--------------------------------------
float sdSphere(vec3 p, float s)
{
    return length(p) - s;
}

float sdBox(vec3 p, vec3 b)
{
    vec3 d = abs(p) - b;
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

float sdCone(vec3 p, vec3 a, vec3 b, float ra, float rb)
{
    float rba = rb - ra;
    float baba = dot(b - a, b - a);
    float papa = dot(p - a, p - a);
    float paba = dot(p - a, b - a) / baba;
    float x = sqrt(papa - paba * paba * baba);
    float cax = max(0.0, x - ((paba < 0.5) ? ra : rb));
    float cay = abs(paba - 0.5) - 0.5;
    float k = rba * rba + baba;
    float f = clamp((rba * (x - ra) + paba * baba) / k, 0.0, 1.0);
    float cbx = x - ra - f * rba;
    float cby = paba - f;
    float s = (cbx < 0.0 && cay < 0.0) ? -1.0 : 1.0;
    return s * sqrt(min(cax * cax + cay * cay * baba, cbx * cbx + cby * cby * baba));
}

float sdEllipsoid(in vec3 p, in vec3 r) // approximated
{
    float k0 = length(p / r);
    float k1 = length(p / (r * r));
    return k0 * (k0 - 1.0) / k1;
}

float dot2(in vec2 v)
{
    return dot(v, v);
}
float dot2(in vec3 v)
{
    return dot(v, v);
}
float ndot(in vec2 a, in vec2 b)
{
    return a.x * b.x - a.y * b.y;
}
float udTriangle(vec3 p, vec3 a, vec3 b, vec3 c)
{
    vec3 ba = b - a;
    vec3 pa = p - a;
    vec3 cb = c - b;
    vec3 pb = p - b;
    vec3 ac = a - c;
    vec3 pc = p - c;
    vec3 nor = cross(ba, ac);

    return sqrt((sign(dot(cross(ba, nor), pa)) + sign(dot(cross(cb, nor), pb)) + sign(dot(cross(ac, nor), pc)) < 2.0) ? min(min(dot2(ba * clamp(dot(ba, pa) / dot2(ba), 0.0, 1.0) - pa), dot2(cb * clamp(dot(cb, pb) / dot2(cb), 0.0, 1.0) - pb)), dot2(ac * clamp(dot(ac, pc) / dot2(ac), 0.0, 1.0) - pc)) : dot(nor, pa) * dot(nor, pa) / dot2(nor));
}

//--------------------------------------
// SDF BLEND & DOMAIN REPETITION
// https://mercury.sexy/hg_sdf/
//--------------------------------------

// HG
float fOpUnionRound(float a, float b, float r)
{
    vec2 u = max(vec2(r - a, r - b), vec2(0));
    return max(r, min(a, b)) - length(u);
}

float pModPolar(inout vec2 p, float repetitions)
{
    float angle = 2. * PI / repetitions;
    float a = atan(p.y, p.x) + angle / 2.;
    float r = length(p);
    float c = floor(a / angle);
    a = mod(a, angle) - angle / 2.;
    p = vec2(cos(a), sin(a)) * r;
    // For an odd number of repetitions, fix cell index of the cell in -x direction
    // (cell index would be e.g. -5 and 5 in the two halves of the cell):
    if (abs(c) >= (repetitions / 2.))
        c = abs(c);
    return c;
}

// Repeat in two dimensions
vec2 pMod2(inout vec2 p, vec2 size)
{
    vec2 c = floor((p + size * 0.5) / size);
    p = mod(p + size * 0.5, size) - size * 0.5;
    return c;
}

// Same, but mirror every second cell so all boundaries match
vec2 pModMirror2(inout vec2 p, vec2 size)
{
    vec2 halfsize = size * 0.5;
    vec2 c = floor((p + halfsize) / size);
    p = mod(p + halfsize, size) - halfsize;
    p *= mod(c, vec2(2)) * 2.0 - vec2(1.0);
    return c;
}

float fOpIntersectionRound(float a, float b, float r)
{
    vec2 u = max(vec2(r + a, r + b), vec2(0));
    return min(-r, max(a, b)) + length(u);
}

float fOpDifferenceRound(float a, float b, float r)
{
    return fOpIntersectionRound(a, -b, r);
}

// IQ
float opSmoothUnion(float d1, float d2, float k)
{
    float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
    return mix(d2, d1, h) - k * h * (1.0 - h);
}

//--------------------------------------
// SDF FIGURES
//--------------------------------------
float treeTrunk(vec3 pos)
{
    float r = 1e10;

    // trunk stem
    r = sdCone(pos, vec3(0.0), vec3(0.0, 0.35, 0.0), 0.12, 0.1);

    // repeat ellpisoids for roots
    vec3 rootsDomain = pos;
    pModPolar(rootsDomain.xz, 5.);
    r = fOpUnionRound(r, sdEllipsoid(rootsDomain - vec3(0.1, 0.01, 0.0), vec3(0.09, 0.04, 0.04)), 0.04);

    // displacement
    r += sin(30. * pos.x) * sin(30. * pos.y) * sin(30. * pos.z) * 0.01;

    return r;
}

float treeLeafs(vec3 pos)
{
    float r = 1e10;

    // pos += (noise(pos * 200.0) - 0.5) * 2.0 * 0.001;

    // sphere piramid
    vec3 leafsDomain = pos - vec3(0., 0.42, 0.);

    float blend = 0.12;
    pModPolar(leafsDomain.xz, 3.);
    r = min(r, sdSphere(leafsDomain - vec3(0.16, 0.0, 0.0), 0.22));
    // r = opSmoothUnion(r, sdSphere(leafsDomain - vec3(-0.08, 0.00, 0.14), 0.22), blend);
    // r = opSmoothUnion(r, sdSphere(leafsDomain - vec3(-0.09, 0.00, -0.14), 0.22), blend);

    r = min(r, sdSphere(pos - vec3(0.0, 0.62, 0.0), 0.22));
    // r = opSmoothUnion(r, sdSphere(pos - vec3(0.0, 0.50, 0.0), 0.36), blend);

    // leaf displacement
    r += sin(20. * pos.x) * sin(20. * pos.y) * sin(20. * pos.z) * 0.01;

    return r + 0.02;
}

float grassStems(vec3 pos, float grassMask)
{
    float r = 1e10;
    // noise pattern approach, (DISCARDED)
    // float noisePattern = noise(vec3(pos.xz, 0.0) * 300.0);
    // r = sdBox( pos - vec3( 0.0, 0.02, 0.0), vec3(0.6,0.05,0.6) );
    // r += step(0.5, noisePattern) * 0.001;

    float distortion = (sin(20. * pos.x) * sin(30. * pos.y) * sin(20. * pos.z) + cos(30. * pos.x) * cos(35. * pos.z)) * 0.012;
    grassMask += distortion * 0.8;
    float area = fOpDifferenceRound(sdBox(pos - vec3(0.0, -0.03, 0.0), vec3(0.58, 0.02, 0.58)), grassMask - 0.03, 0.03) - 0.04;

    // pos.y += 0.01;

    // grass stem CONE (DISCARDED)
    // vec2 id = pMod2(pos.xz, vec2(0.02, 0.02));
    // r =  sdCone(pos, vec3(sin(id.y * 12.34)*cos(id.x*23.0)*0.007, 0., sin(id.x * 12.34)*0.008),
    //                  vec3(sin(id.y * 30.54)*0.008, 0.04 + 0.02*((sin(id.x* 1.23)*cos(id.y*20.0) + 0.5) * 2.0),0.0), 0.003, 0.001 );

    // triangle stem alternative (DISCARDED)
    // r = udTriangle( pos - vec3(sin(id.y * 12.34)*0.015, 0., sin(id.x * 12.34)*0.012),
    //                       vec3(-0.005, 0., sin(id.x * 2.34)*0.005),
    //                       vec3(0.005, 0.,  sin(id.y * 1.34)*0.005),
    //                       vec3(sin(id.y * 52.34)*0.002, 0.05, sin(id.x * 32.34)*0.002) );
    // r = opSmoothUnion(r, sdBox( pos - vec3( 0.0, -0.05, 0.0), vec3(0.6,0.035,0.6)) - 0.02, 0.01);

    r = min(r, sdBox(pos - vec3(0.0, -0.07, 0.0), vec3(0.6, 0.04, 0.6) - 0.04) - 0.02);
    r = max(r, (area - distortion)) - 0.04;

    float remover = sdBox(pos - vec3(0.0, -0.12, 0.0), vec3(0.7, 0.1, 0.7) - 0.02);
    // remover = max(remover, grassMask - distortion);
    remover -= distortion;

    r = fOpDifferenceRound(area, remover, 0.02);

    return r;
    // return grassMask + 0.03;
}

float treeFruits(vec3 pos)
{
    float r = 1e10;
    // vec3 fruitPos = pos - vec3(-0.32, 0.45, -0.31);
    // fruitPos = rotation(Y_AXIS, PI*0.25) * fruitPos;
    // pModPolar(fruitPos.xy, 3.);
    // fruitPos -= vec3(0.15, 0.0, 0.01);
    // pModPolar(fruitPos.zy, 2.);
    // fruitPos -= vec3(0.1, 0.0, 0.15);

    // pModPolar(fruitPos.xy, 2.);
    // fruitPos -= vec3(0.02, 0.0, -0.1);
    // r = sdSphere(fruitPos , 0.04);

    // fruit that... works...
    // r =        sdSphere(pos - vec3(-0.25, 0.45, -0.0),  0.04);
    // r = min(r, sdSphere(pos - vec3(-0.46, 0.4,   0.02), 0.04));
    // r = min(r, sdSphere(pos - vec3(-0.46, 0.8,  -0.28), 0.04));
    // r = min(r, sdSphere(pos - vec3(-0.3,  0.4,  -0.62), 0.04));
    // r = min(r, sdSphere(pos - vec3(-0.55, 0.4,  -0.55), 0.04));
    // r = min(r, sdSphere(pos - vec3(0.04,  0.34, -0.28), 0.04));

    // new fruit ?
    // r =        sdSphere(pos - vec3(-0.26, 0.45, -0.03),  0.08);

    return r;
}

//--------------------------------------
// MAIN SDF
//--------------------------------------
vec2 map(in vec3 pos)
{
    vec2 res = vec2(1e10, 0.0);
#define opMin(_v, _m) res = (_v < res.x) ? vec2(_v, _m) : res

    float substrate = sdBox(pos - vec3(0.0, -0.15, 0.0), vec3(0.6, 0.15, 0.6));
    float waterCavity = sdEllipsoid(pos - vec3(0.65, -0.02, 0.35), vec3(0.5, 0.2, 0.6));
    waterCavity = opSmoothUnion(waterCavity, sdEllipsoid(pos - vec3(0.28, -0.02, 0.65), vec3(0.4, 0.2, 0.4)), 0.08);
    waterCavity -= sin(20. * pos.x) * sin(30. * pos.y) * sin(20. * pos.z) * 0.012;
    substrate = fOpDifferenceRound(substrate, waterCavity, 0.07);
    // substrate = max(substrate, -waterCavity);

    opMin(substrate, MAT_GROUND);

    float underWater = max(res.x, waterCavity - 0.03);
    // res.x += 0.001;
    res = (underWater < (res.x + 0.0001)) ? vec2(underWater, MAT_UNDERWATER) : res;
    // opMin(underWater, MAT_UNDERWATER);

    opMin(grassStems(pos, waterCavity), MAT_GRASS);

    vec3 treePos = pos - vec3(-0.3, 0.028, -0.3);
    float treeTrunkDist = treeTrunk(treePos);
    opMin(treeTrunkDist, MAT_WOOD);
    float fruitsDist = treeFruits(pos);
    float leafsDist = treeLeafs(treePos);
    opMin(leafsDist, MAT_LEAFS);
    opMin(fruitsDist, MAT_FRUIT);

    // Rocks
    float rock = sdEllipsoid(pos - vec3(0.3, 0.06, -0.3), vec3(0.09, 0.09, 0.07));
    rock = min(rock, sdEllipsoid(pos - vec3(0.26, 0.06, -0.38), vec3(0.04, 0.055, 0.04)));
    rock = min(rock, sdEllipsoid(pos - vec3(-0.28, 0.04, 0.38), vec3(0.06, 0.055, 0.055)));
    rock += sin(30. * pos.x) * sin(30. * pos.y + 5.5) * sin(28. * pos.z) * 0.02;
    opMin(rock, MAT_STONE);

    // Bone
    // float bone = 0;

    // water test
    // opMin(sdBox( pos - vec3( 0.25, -0.16, 0.15), vec3(0.35,0.13,0.45) ), 0);

    // res.x -= 0.005;
    return res;
}

float gTime = 0.0;
float mapWaterVolume(vec3 pos)
{
    float baseBox = sdBox(pos - vec3(0.18, -0.17, 0.15), vec3(0.4, 0.11, 0.43));
    float wv = baseBox;
    // animate water
#if defined(ANIMATE_WATER) && ANIMATE_WATER
    vec3 offset = vec3(-gTime * 0.04 - 0.02, 0.0, gTime * 0.06 + 0.1);
#else
    vec3 offset = vec3(0.1);
#endif
    wv += (1.0 - clamp((pos.y / -0.15), 0.0, 1.0))                                                                                                                                                                          // affect mostly on top of the water surface
          * (1.0 * sin(-22. * (pos.x + offset.x)) * sin(23. * (pos.z + offset.z)) + sin(20. * (pos.z + offset.z + 12.5)) * 0.3 + cos(15. * (pos.z + 2.1 + offset.x)) * 0.2 + cos(-21. * (pos.x + offset.z)) * 0.5) * 0.015; //*sin(30.*pos.y)
    wv -= 0.004;
    wv = max(wv, sdBox(pos - vec3(0.18, -0.17, 0.15), vec3(0.4, 0.14, 0.43)));
    wv -= 0.004;
    return wv;
}

//--------------------------------------
// RAYMARCHING
// https://iquilezles.org/articles/rmshadows
//--------------------------------------
float calcSoftshadow(in vec3 ro, in vec3 rd, in float tmin, in float tmax)
{
    // bounding volume
    // float tp = (maxHei-ro.y)/rd.y; if( tp>0.0 ) tmax = min( tmax, tp );

    float res = 1.0;
    float t = tmin;
    for (int i = ZERO; i < 22; i++)
    {
        float h = map(ro + rd * t).x;
        float s = clamp(8.0 * h / t, 0.0, 1.0);
        res = min(res, s * s * (3.0 - 2.0 * s));
        t += clamp(h, 0.02, 0.10);
        if (res < 0.005 || t > tmax)
            break;
    }
    return clamp(res, 0.0, 1.0);
}

// https://iquilezles.org/articles/normalsSDF
vec3 calcNormal(in vec3 pos)
{
#if 0
    vec2 e = vec2(1.0,-1.0)*0.5773*0.0005;
    return normalize( e.xyy*map( pos + e.xyy ).x + 
					  e.yyx*map( pos + e.yyx ).x + 
					  e.yxy*map( pos + e.yxy ).x + 
					  e.xxx*map( pos + e.xxx ).x );
#else
    // inspired by tdhooper and klems - a way to prevent the compiler from inlining map() 4 times
    vec3 n = vec3(0.0);
    for (int i = ZERO; i < 4; i++)
    {
        vec3 e = 0.5773 * (2.0 * vec3((((i + 3) >> 1) & 1), ((i >> 1) & 1), (i & 1)) - 1.0);
        n += e * map(pos + 0.0005 * e).x;
    }
    return normalize(n);
#endif
}

vec3 calcNormalWater(in vec3 pos)
{
    vec3 n = vec3(0.0);
    for (int i = ZERO; i < 4; i++)
    {
        vec3 e = 0.5773 * (2.0 * vec3((((i + 3) >> 1) & 1), ((i >> 1) & 1), (i & 1)) - 1.0);
        n += e * mapWaterVolume(pos + 0.0005 * e);
    }
    return normalize(n);
}

float calcAO(in vec3 pos, in vec3 nor)
{
    float occ = 0.0;
    float sca = 1.00;
    for (int i = ZERO; i < 5; i++)
    {
        float hr = 0.01 + 0.12 * float(i) / 4.0;
        vec3 aopos = nor * hr + pos;
        float dd = map(aopos).x;
        occ += -(dd - hr) * sca;
        sca *= 0.95;
    }
    return clamp(1.0 - 3.0 * occ, 0.0, 1.0) * (0.5 + 0.5 * nor.y);
}

vec3 calcSkyColor(vec3 aDirection)
{
    float t = smoothstep(0.1, 0.6, 0.5f * (aDirection.y + 1.0f));
    return mix(vec3(0.4f, 0.4f, 0.2f), vec3(0.4f, 0.6f, 1.0f), t);
}

vec4 calcColor(int matId, vec3 pos, vec3 normal, float diffuse, float fresnel)
{
    vec4 FinalColor = vec4(0.1, 0.1, 0.1, 1) * diffuse;
    if (matId == MAT_WOOD)
    {
        vec3 WoodBrown = vec3(0.287, 0.11882, 0.04) * 1.5;
        vec3 WoodBrownShadow = vec3(0.1847, 0.0482, 0.016) * 1.2;
        FinalColor.rgb = mix(WoodBrownShadow, WoodBrown, diffuse);
        FinalColor.rgb += WoodBrown * fresnel * 2.0;
    }
    else if (matId == MAT_LEAFS)
    {
        vec3 Leafs = vec3(0.0882, 0.447, 0.04);
        vec3 LeafsShadow = vec3(0.00582, 0.247, 0.02);
        FinalColor.rgb = mix(LeafsShadow, Leafs, diffuse) * 0.7;
        FinalColor.rgb += Leafs * fresnel * 2.5;
    }
    else if (matId == MAT_GRASS)
    {
        vec3 Grass = vec3(0.0882, 0.247, 0.04);
        vec3 GrassShadow = vec3(0.00582, 0.147, 0.02);
        FinalColor.rgb = mix(GrassShadow, Grass, diffuse);
        FinalColor.rgb += Grass * fresnel * 1.5;
    }
    else if (matId == MAT_STONE)
    {
        vec3 Stone = vec3(0.4, 0.4, 0.4);
        vec3 StoneShadow = vec3(0.2, 0.2, 0.3);
        FinalColor.rgb = mix(StoneShadow, Stone, diffuse);
        FinalColor.rgb += Stone * fresnel * 0.5;
    }
    else if (matId == MAT_FRUIT)
    {
        vec3 Fruit = vec3(0.8, 0.1, 0.01);
        vec3 FruitShadow = vec3(0.75, 0.1, 0.01);
        FinalColor.rgb = mix(FruitShadow, Fruit, diffuse);
        FinalColor.rgb += vec3(0.8, 0.5, 0.5) * fresnel * 2.5;
    }
    else if (matId == MAT_GROUND)
    {
        if (abs(dot(normal, Y_AXIS)) > 0.5)
        {
            vec3 Sand = vec3(0.447, 0.447, 0.04);
            vec3 SandShadow = vec3(0.347, 0.247, 0.02);
            FinalColor.rgb = mix(SandShadow, Sand, diffuse);
        }
        else
        {
            float axis = dot(normal, X_AXIS);
            vec2 basePos = (abs(axis) > 0.5) ? pos.zy : pos.xy;

            // 0-1 UVs based on ground block size
            vec2 uv = basePos.xy / vec2(0.6, 0.15);

            // Adjust position
            uv += vec2(0.0, 0.6);
            uv += (axis > 0.5) ? vec2(-0.12, 0.0) : vec2(0.32, 0.0);

            // Sine
            vec2 p = uv * 2.0 - 1.0;
            p *= 15.0;
            float sfunc = p.y + 5.0 * sin(uv.x * 5.0) + 4.0 * cos(uv.x * 3.0);
            sfunc *= 0.01;
            sfunc = abs(sfunc);
            sfunc = smoothstep(0.25, 0.251, sfunc);

            // Brown mix
            vec3 GroundBrown = mix(vec3(0.4, 0.25, 0.08), vec3(0.28, 0.15, 0.05), sfunc); // vec3(0.35, 0.18, 0.1)
            vec3 GroundBrownShadow = GroundBrown * 0.5;                                   // mix(vec3(0.20, 0.08, 0.04), vec3(0.12, 0.06, 0.025), sfunc); // vec3(0.35, 0.18, 0.1)
            FinalColor.rgb = mix(GroundBrownShadow, GroundBrown, mix(0.3, 1.0, diffuse));
            FinalColor.rgb += GroundBrown * fresnel * 0.8;
        }
    }
    else if (matId == MAT_UNDERWATER)
    {
        vec3 Sand = vec3(0.447, 0.447, 0.04);
        vec3 SandShadow = vec3(0.347, 0.247, 0.02);
        FinalColor.rgb = mix(SandShadow, Sand, diffuse);
    }

    return FinalColor;
}

vec3 castRay(vec3 ro, vec3 rd)
{
    vec3 res = vec3(0.0, 1e10, 0.0);
    float tmin = 1.0;
    float tmax = 20.0;
    float t = tmin;
    for (int i = 0; i < 70 && t < tmax; i++)
    {
        vec2 h = map(ro + rd * t);
        if (abs(h.x) < (0.0001 * t))
        {
            res = vec3(t, h.x, h.y);
            break;
        }
        t += h.x;
    }

    return res;
}

vec4 applyWaterVolume(vec3 ro, vec3 rd, float depth, vec4 color)
{
    float tmin = 1.0;
    float tmax = 20.0;
    float t = tmin;
    float hit = 0.0;
    float h = 0.0;
    float distInsideWater = 0.0;
    for (int i = 0; i < 70 && t < tmax; i++)
    {
        h = mapWaterVolume(ro + rd * t);
        if (abs(h) < (0.0001 * t))
        {
            distInsideWater += h;
            hit = 1.0;
            break;
        }
        else if (hit > 0.0)
        {
            break;
        }
        t += h;
        if (depth > 0.0 && ((t + 0.0011) > depth))
        {
            break;
        }
    }

    depth = (depth > 0.0) ? depth : t * 2.5;

    vec4 WaterBlue = vec4(0.1, 0.4, 1.0, color.a);

    vec3 pos = ro + rd * t;
    vec3 lightDir = normalize(vec3(-0.5, 1.1, -0.6));
    float shadow = calcSoftshadow(pos, lightDir, 0.02, 2.5);
    vec3 normal = calcNormalWater(pos);
    float NdL = clamp(dot(normal, lightDir), 0.0, 1.0);
    vec3 hal = normalize(lightDir - rd);
    float spe = pow(clamp(dot(normal, hal), 0.0, 1.0), 40.0)
        //*mix(0.5, 1.0, NdL* shadow)  //shadow
        //*(0.04 + 2.5*pow( clamp(1.0+dot(hal, rd),0.0,1.0), 1.0 ));
        ;
    spe = smoothstep(0.5, 0.9, spe);
    // light affecting water
    WaterBlue = mix(WaterBlue * 0.5, WaterBlue, NdL * shadow);

    // all inside water is bluiedish
    color = mix(color, WaterBlue * 0.8 + color * WaterBlue * 0.5, hit * 0.3);

    // distance to closest point
    float nearest = clamp(map(pos).x, 0.0, 1.0);
    color = mix(color, WaterBlue, clamp(pow(nearest * hit, 1.3) * 5.0, 0.0, 1.0));

    // distance to center of the diorama hack
    color = mix(color, WaterBlue, clamp(pow(length(pos) * hit, 2.0) * 1.2, 0.0, 1.0));
    // color = mix(color, WaterBlue * 0.5, ((t / depth)) * hit * 0.7);
    // return vec4(mix(color.rgb, normal * 0.5 + 0.5, hit), 1.0);

#define WATER_OPACITY_INIT 0.3
#define WATER_OPACITY_COEFF 2.5

    float fresnel = pow(clamp(1.0 + dot(normal, rd), 0.0, 1.0), 2.4);
    color.rgb += hit * 2.00 * spe * vec3(1.);
    // color = mix(color, color * exp(-(1.-WaterBlue)*(WATER_OPACITY_INIT + WATER_OPACITY_COEFF*distInsideWater)), hit);
    color += mix(vec4(0.0), fresnel * color * 2.0, hit * mix(0.2, 1.0, shadow));
    return color;
}

vec4 render(vec2 uv, in vec3 ro, in vec3 rd, in vec3 rdx, in vec3 rdy)
{
    // vec4 finalColor = vec4(0.042); //vec4(calcSkyColor(rd), 0.0);
    vec4 finalColor = vec4(calcSkyColor(rd), 0.0);
    vec3 res = castRay(ro, rd);

    if (res.y < 0.002)
    {
        vec3 lightDir = normalize(vec3(-0.5, 1.1, -0.6));
        vec3 pos = ro + rd * res.x;
        vec3 normal = calcNormal(pos);

        float ao = calcAO(pos, normal);
        float shadow = calcSoftshadow(pos, lightDir, 0.02, 2.5);
        float NdL = clamp(dot(normal, lightDir), 0.0, 1.0);
        float fresnel = pow(clamp(1.0 + dot(normal, rd), 0.0, 1.0), 2.4);

        float diffuse = shadow * NdL * 12.0;

        vec4 color = calcColor(int(res.z), pos, normal, diffuse, fresnel) * mix(0.22, 1.0, ao);

        finalColor = vec4(color.rgb, res.x);

        finalColor.rgb = finalColor.rgb * 0.4 + 0.6 * finalColor.rgb * calcSkyColor(normal);

        // finalColor = vec4(normal * 0.5 + 0.5, res.x);
    }

    finalColor = applyWaterVolume(ro, rd, res.x, finalColor);

    return finalColor;
}