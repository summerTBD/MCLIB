#ifndef MCLIB_RESULT_H
#define MCLIB_RESULT_H

// 一个 enum 统一所有返回码（负数 = 错误，和 Linux 一致）
typedef enum {
    MCLIB_OK = 0, // 成功

    MCLIB_ERR_NULL = -1,   // 传入了 NULL 指针
    MCLIB_ERR_OOM = -2,    // malloc / realloc 失败，内存不足
    MCLIB_ERR_BOUNDS = -3, // 数组越界 / 索引超出范围
    MCLIB_ERR_EMPTY = -4,  // 容器为空，比如对空 Vec 调 pop
    MCLIB_ERR_STATE = -5,  // 状态错误，比如 use-after-drop / 重复 init
} MclibResult;

#endif