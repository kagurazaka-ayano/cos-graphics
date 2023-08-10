#ifndef _CG_LOG_H_
#define _CG_LOG_H_

#include <stdlib.h>
#include "cos_graphics/defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CG_MODIFY_OUTPUT_MESSAGE
    #define CG_PRINT(...) CGPrint(CGSTR("CosGraphics"), CGSTR("Output"), __VA_ARGS__)
    #define CG_PRINT_WITH_FUNCTION(...) CGPrintWithFunction(CGSTR("CosGraphics"), CGSTR("Output"), __FUNCTION__, __VA_ARGS__)
    #define CG_ERROR(...) CGPrintWithFunction(CGSTR("CosGraphics"), CGSTR("Error"), __FUNCTION__, __VA_ARGS__)
    #define CG_WARNING(...) CGPrintWithFunction(CGSTR("CosGraphics"), CGSTR("Warning"), __FUNCTION__, __VA_ARGS__)

    #ifdef CG_PRINT_VERBOSE
        #define CG_PRINT_VERBOSE(...) CG_PRINT(__VA_ARGS__)
        #define CG_PRINT_VERBOSE_WITH_FUNCTION(...) CG_PRINT_WITH_FUNCTION(__VA_ARGS__)
        #define CG_ERROR_VERBOSE(...) CG_ERROR(__VA_ARGS__)
        #define CG_WARNING_VERBOSE(...) CG_WARNING(__VA_ARGS__)
    #else
        #define CG_PRINT_VERBOSE(...)
        #define CG_PRINT_VERBOSE_WITH_FUNCTION(...)
        #define CG_ERROR_VERBOSE(...)
        #define CG_WARNING_VERBOSE(...)
    #endif

#endif

/**
 * @brief Print out message. The message will follow format: [sender] <type>: message
 * 
 * @param sender The sender of the message.
 * @param type The type of the message.
 * @param fmt The format of the message.
 * @param ... The arguments of the message.
 */
void CGPrint(const CGChar* sender, const CGChar* type, const CGChar* fmt, ...);

/**
 * @brief Print out message with file and function information. The message will follow format: 
 * [sender] <type>: file: file_name, function function_name: message
 * 
 * @param sender The sender of the message.
 * @param type The type of the message.
 * @param function_name The name of the function.
 * @param fmt The format of the message.
 * @param ... 
 */
void CGPrintWithFunction(const CGChar* sender, const CGChar* type, const char* function_name, const CGChar* fmt, ...);

/**
 * @brief Should print out error. If condition is true, then print out error and exit the function.
 */
#define CG_ERROR_CONDITION(cond, ...) \
        if (cond){                      \
            CG_ERROR(__VA_ARGS__);      \
            return;                     \
        }                               \
        ((void)0)
/**
 * @brief Should print out error. If condition is true, then print out error and return the return_value.
 */
#define CG_ERROR_COND_RETURN(cond, return_value, ...) \
        if (cond){                      \
            CG_ERROR(__VA_ARGS__);      \
            return return_value;        \
        }                               \
        ((void)0)
/**
 * @brief Should print out error. If condition is true, then print out error and crash the program.
 */
#define CG_ERROR_COND_EXIT(cond, exit_value, ...) \
        if (cond){                      \
            CG_ERROR(__VA_ARGS__);      \
            exit(exit_value);           \
        }                               \
        ((void)0)


#ifdef __cplusplus
}
#endif

#endif  // _CG_LOG_H_