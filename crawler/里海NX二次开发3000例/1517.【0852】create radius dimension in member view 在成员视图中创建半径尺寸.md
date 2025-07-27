### 【0852】create radius dimension in member view 在成员视图中创建半径尺寸

#### 代码

```cpp
    /*HEAD CREATE_RADIUS_DIMENSION_IN_MEMBER_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_object_types.h>  
    #include <uf_view.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是在 V18 中新增的函数。

功能说明：
UF_print_syslog 用于打印系统日志信息。

参数说明：
- priority: 日志的优先级
- tag: 日志标签
- format: 格式化字符串，用于输出日志信息
- ...: 可变参数，与 format 参数配合输出具体的日志信息

返回值：
该函数没有返回值。

使用示例：
```c
UF_print_syslog(LOG_DEBUG, "mytag", "This is a debug message");

```

```cpp
# 代码解析
这段代码是NX二次开发代码，主要实现的功能是在NX的成员视图中创建半径尺寸标注。

主要步骤包括：

1. 包含必要的NX二次开发头文件，如uf.h、uf_ui.h等。

2. 定义了宏UF_CALL，用于调用NX函数，并在出错时打印错误信息。

3. 定义了report_error函数，用于打印错误信息。

4. 定义了mask_for_drf_arcs函数，用于设置选择弧时需要的掩码。

5. 定义了select_drf_arc函数，用于让用户选择一个弧。

6. 定义了select_pos函数，用于让用户指定一个点。

7. 定义了do_it函数，用于实现创建半径尺寸标注的逻辑。首先获取用户输入的成员视图名称，然后获取视图tag，展开视图。之后进入循环，让用户选择弧和尺寸原点，在每次循环中调用UF_DRF_create_radius_dim函数创建一个半径尺寸标注。

8. 定义了ufusr函数，作为程序的入口函数。初始化NX后调用do_it函数，然后终止NX。

9. 定义了ufusr_ask_unload函数，返回立即卸载标志。

综上所述，这段代码实现了在NX的成员视图中创建半径尺寸标注的功能。
![是里海啊](https://img-blog.csdnimg.cn/4c206213ad7b4596bd2a216c390652ce.gif#pic_center)


```

#### 代码解析
> 未找到解析内容

