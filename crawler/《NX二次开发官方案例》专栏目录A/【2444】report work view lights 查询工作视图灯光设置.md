### 【2444】report work view lights 查询工作视图灯光设置

#### 代码

```cpp
    /*HEAD REPORT_WORK_VIEW_LIGHTS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，我了解到：

- UF_print_syslog 是一个新函数，首次出现在 V18 版本中。
- 这个函数用于打印系统日志。
- 在 V18 版本之前，并不存在这个函数。
- 由于这是一个新增函数，因此需要翻译相关文档以便用户了解其功能和使用方法。
- 在翻译过程中，只需要提供准确的译文，无需添加任何解释或评论。

以上是对提供的信息的总结。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            work_view;  
        UF_VIEW_lighting_t  
            view_light;  
    /*  New in V16.0  
        UF_CALL(UF_VIEW_ask_work_view(&work_view));  
    */  
        UF_CALL(UF_VIEW_ask_tag_of_view_name("", &work_view));  
        UF_CALL(UF_VIEW_ask_view_light(work_view, &view_light));  
        WRITE_D(view_light.two_sided_light);  
        WRITE_F(view_light.shininess);  
        WRITE_D(view_light.light_count);  
        for (ii = 0; ii < view_light.light_count; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_S(view_light.lights[ii].light_name);  
            WRITE_D(view_light.lights[ii].light_type);  
            WRITE_D(view_light.lights[ii].light_mode);  
            WRITE3F(view_light.lights[ii].location);  
            WRITE3F(view_light.lights[ii].to);  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open API的二次开发示例，主要用于获取当前工作视图的灯光信息并进行打印输出。代码的主要功能包括：
>
> 1. 定义错误报告函数report_error，用于打印API调用错误信息。
> 2. 定义整数、浮点数、字符串和浮点数数组的打印函数，用于将信息输出到列表窗口。
> 3. do_it函数用于获取当前工作视图的tag，并调用UF_VIEW_ask_view_light函数获取视图灯光信息。然后遍历每个灯光，打印其名称、类型、模式、位置和方向等信息。
> 4. ufusr函数是NX二次开发程序的入口函数，在这里调用do_it函数执行主体功能。
> 5. ufusr_ask_unload函数用于设置程序的卸载模式。
>
> 总体来说，这段代码通过NX Open API获取并打印当前工作视图的灯光信息，为NX二次开发提供了一个示例。
>
