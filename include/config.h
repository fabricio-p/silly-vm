#ifndef __SILLY_CONFIG_H__
#define __SILLY_CONFIG_H__

#if !defined(BUILD_TARGET_ARM) && !defined(BUILD_TARGET_AMD64)        \
  && !defined(BUILD_TARGET_AARCH64) && !defined(BUILD_TARGET_X86_32)  \
  && !defined(BUILD_TARGET_X86_64)
#if defined(__arm__) || defined(__arm)
#define BUILD_TARGET_ARM
#elif defined(__amd64__) || defined(__amd64)
#define BUILD_TARGET_AMD64
#elif defined(__aarch64__) || defined(__aarch64)
#define BUILD_TARGET_AARCH64
#elif defined(__i386__) || defined(__i386) || defined(i386)
#define BUILD_TARGET_X86_32
#elif defined(_x86_64__) | defined(__x86_64)
#define BUILD_TARGET_X86_64
#else
#error "Build target is not set"
#endif
#endif


#if defined(BUILD_TARGET_AARCH64) || defined(BUILD_TARGET_X86_32)  \
  || defined(BUILD_TARGET_X86_64)
#define SILLY_CPU_UNALIGNED_POINTERS 1
#else
#define SILLY_CPU_UNALIGNED_POINTERS 0
#endif

#if defined(__LITTLE_ENDIAN__)
#define SILLY_LITTLE_ENDIAN 1
#else
#define SILLY_LITTLE_ENDIAN 0
#endif

#if defined(BUILD_TARGET_ARM) || defined(BUILD_TARGET_X86_32)
#define SILLY_CPU_32BIT 1
#else
#define SILLY_CPU_32BIT 0
#endif

#endif /* __SILLY_CONFIG_H__ */
