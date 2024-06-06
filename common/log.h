#ifndef LOG_H__
#endif // LOG_H__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    enum log_level
    {
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_FATAL,
    };

    void set_log_level(enum log_level level);
    void log_(enum log_level level, const char *file, int line, const char *func, const char *fmt, ...);

#define log_debug(fmt, ...)                                                      \
    do                                                                           \
    {                                                                            \
        log_(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                            \
    while (0);

#define log_info(fmt, ...)                                                      \
    do                                                                          \
    {                                                                           \
        log_(LOG_LEVEL_INFO, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                           \
    while (0);

#define log_warn(fmt, ...)                                                      \
    do                                                                          \
    {                                                                           \
        log_(LOG_LEVEL_WARN, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                           \
    while (0);

#define log_error(fmt, ...)                                                      \
    do                                                                           \
    {                                                                            \
        log_(LOG_LEVEL_ERROR, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                            \
    while (0);

#define log_fatal(fmt, ...)                                                      \
    do                                                                           \
    {                                                                            \
        log_(LOG_LEVEL_FATAL, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    }                                                                            \
    while (0);

#ifdef __cplusplus
}
#endif // __cplusplus

#ifndef LOG_H__
#endif // LOG_H__