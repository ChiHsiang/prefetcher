#ifndef TRANSPOSE_IMPL
#define TRANSPOSE_IMPL


#if defined(MAIN) | defined(NAIVE)
void naive_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            *(dst + x * h + y) = *(src + y * w + x);
}
#endif

#if defined(MAIN) | defined(SSE)
void sse_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x += 4) {
        for (int y = 0; y < h; y += 4) {
            __m128i I0 = _mm_loadu_si128((__m128i *)(src + (y + 0) * w + x));
            __m128i I1 = _mm_loadu_si128((__m128i *)(src + (y + 1) * w + x));
            __m128i I2 = _mm_loadu_si128((__m128i *)(src + (y + 2) * w + x));
            __m128i I3 = _mm_loadu_si128((__m128i *)(src + (y + 3) * w + x));
            __m128i T0 = _mm_unpacklo_epi32(I0, I1);
            __m128i T1 = _mm_unpacklo_epi32(I2, I3);
            __m128i T2 = _mm_unpackhi_epi32(I0, I1);
            __m128i T3 = _mm_unpackhi_epi32(I2, I3);
            I0 = _mm_unpacklo_epi64(T0, T1);
            I1 = _mm_unpackhi_epi64(T0, T1);
            I2 = _mm_unpacklo_epi64(T2, T3);
            I3 = _mm_unpackhi_epi64(T2, T3);
            _mm_storeu_si128((__m128i *)(dst + ((x + 0) * h) + y), I0);
            _mm_storeu_si128((__m128i *)(dst + ((x + 1) * h) + y), I1);
            _mm_storeu_si128((__m128i *)(dst + ((x + 2) * h) + y), I2);
            _mm_storeu_si128((__m128i *)(dst + ((x + 3) * h) + y), I3);
        }
    }
}
#endif

#if defined(MAIN) | defined(SSE_PREFETCH)
void sse_prefetch_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x += 4) {
        for (int y = 0; y < h; y += 4) {
#define PFDIST  8
            _mm_prefetch(src+(y + PFDIST + 0) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 1) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 2) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 3) *w + x, _MM_HINT_T1);

            __m128i I0 = _mm_loadu_si128 ((__m128i *)(src + (y + 0) * w + x));
            __m128i I1 = _mm_loadu_si128 ((__m128i *)(src + (y + 1) * w + x));
            __m128i I2 = _mm_loadu_si128 ((__m128i *)(src + (y + 2) * w + x));
            __m128i I3 = _mm_loadu_si128 ((__m128i *)(src + (y + 3) * w + x));
            __m128i T0 = _mm_unpacklo_epi32(I0, I1);
            __m128i T1 = _mm_unpacklo_epi32(I2, I3);
            __m128i T2 = _mm_unpackhi_epi32(I0, I1);
            __m128i T3 = _mm_unpackhi_epi32(I2, I3);
            I0 = _mm_unpacklo_epi64(T0, T1);
            I1 = _mm_unpackhi_epi64(T0, T1);
            I2 = _mm_unpacklo_epi64(T2, T3);
            I3 = _mm_unpackhi_epi64(T2, T3);
            _mm_storeu_si128((__m128i *)(dst + ((x + 0) * h) + y), I0);
            _mm_storeu_si128((__m128i *)(dst + ((x + 1) * h) + y), I1);
            _mm_storeu_si128((__m128i *)(dst + ((x + 2) * h) + y), I2);
            _mm_storeu_si128((__m128i *)(dst + ((x + 3) * h) + y), I3);
        }
    }
}
#endif

#if defined(MAIN) | defined(AVX)
void avx_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x += 8) {
        for (int y = 0; y < h; y += 8) {
            __m256 r0, r1, r2, r3, r4, r5, r6, r7;
            __m256 t0, t1, t2, t3, t4, t5, t6, t7;

            r0 = _mm256_loadu_ps((__m256 *)(src + (y + 0) * w + x));
            r1 = _mm256_loadu_ps((__m256 *)(src + (y + 1) * w + x));
            r2 = _mm256_loadu_ps((__m256 *)(src + (y + 2) * w + x));
            r3 = _mm256_loadu_ps((__m256 *)(src + (y + 3) * w + x));
            r4 = _mm256_loadu_ps((__m256 *)(src + (y + 4) * w + x));
            r5 = _mm256_loadu_ps((__m256 *)(src + (y + 5) * w + x));
            r6 = _mm256_loadu_ps((__m256 *)(src + (y + 6) * w + x));
            r7 = _mm256_loadu_ps((__m256 *)(src + (y + 7) * w + x));

            t0 = _mm256_unpacklo_ps(r0, r1);
            t1 = _mm256_unpackhi_ps(r0, r1);
            t2 = _mm256_unpacklo_ps(r2, r3);
            t3 = _mm256_unpackhi_ps(r2, r3);
            t4 = _mm256_unpacklo_ps(r4, r5);
            t5 = _mm256_unpackhi_ps(r4, r5);
            t6 = _mm256_unpacklo_ps(r6, r7);
            t7 = _mm256_unpackhi_ps(r6, r7);

            r0 = _mm256_shuffle_ps(t0, t2, 0x44);
            r1 = _mm256_shuffle_ps(t0, t2, 0xEE);
            r2 = _mm256_shuffle_ps(t1, t3, 0x44);
            r3 = _mm256_shuffle_ps(t1, t3, 0xEE);
            r4 = _mm256_shuffle_ps(t4, t6, 0x44);
            r5 = _mm256_shuffle_ps(t4, t6, 0xEE);
            r6 = _mm256_shuffle_ps(t5, t7, 0x44);
            r7 = _mm256_shuffle_ps(t5, t7, 0xEE);

            t0 = _mm256_permute2f128_ps(r0, r4, 0x20);
            t1 = _mm256_permute2f128_ps(r1, r5, 0x20);
            t2 = _mm256_permute2f128_ps(r2, r6, 0x20);
            t3 = _mm256_permute2f128_ps(r3, r7, 0x20);
            t4 = _mm256_permute2f128_ps(r0, r4, 0x31);
            t5 = _mm256_permute2f128_ps(r1, r5, 0x31);
            t6 = _mm256_permute2f128_ps(r2, r6, 0x31);
            t7 = _mm256_permute2f128_ps(r3, r7, 0x31);

            _mm256_storeu_ps((__m256 *)(dst + ((x + 0) * h) + y), t0);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 1) * h) + y), t1);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 2) * h) + y), t2);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 3) * h) + y), t3);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 4) * h) + y), t4);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 5) * h) + y), t5);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 6) * h) + y), t6);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 7) * h) + y), t7);

        }
    }
}
#endif

#if defined(MAIN) | defined(AVX_PREFETCH)
void avx_prefetch_transpose(int *src, int *dst, int w, int h)
{
    for (int x = 0; x < w; x += 8) {
        for (int y = 0; y < h; y += 8) {

#define PFDIST 8
            _mm_prefetch(src+(y + PFDIST + 0) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 1) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 2) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 3) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 4) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 5) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 6) *w + x, _MM_HINT_T1);
            _mm_prefetch(src+(y + PFDIST + 7) *w + x, _MM_HINT_T1);


            __m256 r0, r1, r2, r3, r4, r5, r6, r7;
            __m256 t0, t1, t2, t3, t4, t5, t6, t7;

            r0 = _mm256_loadu_ps((__m256 *)(src + (y + 0) * w + x));
            r1 = _mm256_loadu_ps((__m256 *)(src + (y + 1) * w + x));
            r2 = _mm256_loadu_ps((__m256 *)(src + (y + 2) * w + x));
            r3 = _mm256_loadu_ps((__m256 *)(src + (y + 3) * w + x));
            r4 = _mm256_loadu_ps((__m256 *)(src + (y + 4) * w + x));
            r5 = _mm256_loadu_ps((__m256 *)(src + (y + 5) * w + x));
            r6 = _mm256_loadu_ps((__m256 *)(src + (y + 6) * w + x));
            r7 = _mm256_loadu_ps((__m256 *)(src + (y + 7) * w + x));

            t0 = _mm256_unpacklo_ps(r0, r1);
            t1 = _mm256_unpackhi_ps(r0, r1);
            t2 = _mm256_unpacklo_ps(r2, r3);
            t3 = _mm256_unpackhi_ps(r2, r3);
            t4 = _mm256_unpacklo_ps(r4, r5);
            t5 = _mm256_unpackhi_ps(r4, r5);
            t6 = _mm256_unpacklo_ps(r6, r7);
            t7 = _mm256_unpackhi_ps(r6, r7);

            r0 = _mm256_shuffle_ps(t0, t2, 0x44);
            r1 = _mm256_shuffle_ps(t0, t2, 0xEE);
            r2 = _mm256_shuffle_ps(t1, t3, 0x44);
            r3 = _mm256_shuffle_ps(t1, t3, 0xEE);
            r4 = _mm256_shuffle_ps(t4, t6, 0x44);
            r5 = _mm256_shuffle_ps(t4, t6, 0xEE);
            r6 = _mm256_shuffle_ps(t5, t7, 0x44);
            r7 = _mm256_shuffle_ps(t5, t7, 0xEE);

            t0 = _mm256_permute2f128_ps(r0, r4, 0x20);
            t1 = _mm256_permute2f128_ps(r1, r5, 0x20);
            t2 = _mm256_permute2f128_ps(r2, r6, 0x20);
            t3 = _mm256_permute2f128_ps(r3, r7, 0x20);
            t4 = _mm256_permute2f128_ps(r0, r4, 0x31);
            t5 = _mm256_permute2f128_ps(r1, r5, 0x31);
            t6 = _mm256_permute2f128_ps(r2, r6, 0x31);
            t7 = _mm256_permute2f128_ps(r3, r7, 0x31);

            _mm256_storeu_ps((__m256 *)(dst + ((x + 0) * h) + y), t0);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 1) * h) + y), t1);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 2) * h) + y), t2);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 3) * h) + y), t3);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 4) * h) + y), t4);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 5) * h) + y), t5);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 6) * h) + y), t6);
            _mm256_storeu_ps((__m256 *)(dst + ((x + 7) * h) + y), t7);

        }
    }
}
#endif

#endif /* TRANSPOSE_IMPL */
