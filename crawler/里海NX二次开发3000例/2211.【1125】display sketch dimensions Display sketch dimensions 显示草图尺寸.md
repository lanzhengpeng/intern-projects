### 【1125】display sketch dimensions Display sketch dimensions 显示草图尺寸

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sket.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static tag_t select_a_sketch(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            sketch;  
        if (!UF_CALL(UF_UI_select_sketch(prompt, NULL, &sketch, &resp))  
        && (resp == UF_UI_OK))  
           return sketch;  
        else  
            return NULL_TAG;  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_dims,  
            status;  
        tag_t  
            *dims,  
            exp,  
            sketch;  
        double  
            value;  
        char  
            exp_string[UF_MAX_EXP_LENGTH+1];  
        while ((sketch = select_a_sketch("Display dimensions")) != NULL_TAG)  
        {  
            UF_CALL(UF_SKET_ask_dimensions_of_sketch(sketch, &n_dims, &dims));  
            WRITE_D(n_dims);  
            for (ii = 0; ii < n_dims; ii++)  
            {  
                UF_CALL(UF_SKET_ask_dim_status(dims[ii], &exp, exp_string,  
                    &value, &status));  
                UF_CALL(UF_DISP_display_sket_dimensions(sketch, 1, &exp,  
                    UF_DISP_WORK_VIEW_ONLY, UF_DISP_USE_SPECIFIED_COLOR,  
                    status));  
                uc1601(exp_string, TRUE);  
                UF_DISP_refresh();  
            }  
            if (n_dims < 0) UF_free(dims);  
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

> 这段代码是一个NX Open的二次开发示例，其主要功能是选择一个草图并显示其所有尺寸。以下是代码的主要功能介绍：
>
> 1. 错误报告宏定义：使用了UF_CALL宏来检查函数调用是否成功，并在失败时报告错误。
> 2. 选择草图函数：select_a_sketch函数用于选择一个草图，并返回其tag。
> 3. 显示尺寸：do_it函数用于显示选定草图的尺寸。它首先获取草图的所有尺寸信息，然后逐个显示每个尺寸。
> 4. 主函数：ufusr函数是程序的入口点。它初始化NX Open，调用do_it函数来显示尺寸，并在结束时终止NX Open。
> 5. 卸载函数：ufusr_ask_unload函数用于在卸载NX Open时立即卸载。
> 6. 其他函数和宏：使用了NX Open的一些函数，如UF_SKET_ask_dimensions_of_sketch来获取草图尺寸，UF_DISP_display_sket_dimensions来显示尺寸等。
> 7. 错误处理：通过report_error函数来报告函数调用错误，并打印错误信息。
> 8. 交互式选择：通过UF_UI_select_sketch函数实现交互式选择草图。
>
> 总体来说，这段代码通过交互方式选择草图，并遍历显示草图的每个尺寸，实现了在NX中显示草图尺寸的功能。
>
