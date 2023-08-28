// 使用两个无符号整型经过16轮微小加密算法生成一个随机的无符号整型。
// GPU Random Numbers via the Tiny Encryption Algorithm
uint Tea(uint val0, uint val1)
{
    uint v0 = val0;
    uint v1 = val1;
    uint s0 = 0;

    for (uint n = 0; n < 16; n++)
    {
        s0 += 0x9e3779b9;
        v0 += ((v1 << 4) + 0xa341316c) ^ (v1 + s0) ^ ((v1 >> 5) + 0xc8013ea4);
        v1 += ((v0 << 4) + 0xad90777d) ^ (v0 + s0) ^ ((v0 >> 5) + 0x7e95761e);
    }

    return v0;
}

// 生成一个在 [0, 2^24) 范围中的无符号整型随机值，并初始化之前的（prev）随机数生成器
// 使用的是数值分析的线性同余生成器（Numerical Recipes linear congruential generator）
// 知乎上有一位大佬提到了该算法：https://www.zhihu.com/question/34515945/answer/59082990
uint LinearCongruentialGenerator(inout uint prev)
{
    uint LCG_A = 1664525u;
    uint LCG_C = 1013904223u;
    prev = (LCG_A * prev + LCG_C);
    return prev & 0x00FFFFFF;
}

// 生成一个在 [0, 1) 范围中的单精度浮点型随机，并初始化之前的（prev）随机数生成器
float Random(inout uint prev)
{
    return (float(LinearCongruentialGenerator(prev)) / float(0x01000000));
}