#ifndef RECONOVA_INITIALIZATION_H__
#define RECONOVA_INITIALIZATION_H__


#ifdef __linux
#define RWINIT_API
#else

#ifdef RWINIT_EXPORTS
#define RWINIT_API __declspec(dllexport)
#else
#define RWINIT_API __declspec(dllimport)
#endif

#endif


/** 模块句柄 */
typedef void * RwModuleHandle;

/**
 * 初始化模块，需要设置当前工作目录到exe所在目录
 * @param [in] path 工作目录
 * ..//Relesae
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWINIT_API int InitializeModule(
	const char *path,
	RwModuleHandle *module_handle
);

/**
 * 反初始化人脸模块
 * @return 返回是否成功
 * - 0 表示成功
 * - -1 表示失败
 */
RWINIT_API int UnInitializeModule(void);


#endif