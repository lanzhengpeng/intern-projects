### 【1295】find retained dimensions 找到保留的尺寸

#### 代码

```cpp
    /*HEAD FIND_RETAINED_DIMENSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：请注意，UF_print_syslog 是在 V18 中新增的。只回答翻译内容，不要写多余的话。 */  
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
    static tag_t ask_next_dim(tag_t part, tag_t dim)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_dimension_type, &dim))  
            && (dim != NULL_TAG))  
        {  
            if (!UF_ASSEM_is_occurrence(dim)) return dim;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        logical  
            retflg;  
        int  
            cnt = 0;  
        tag_t  
            ret_dim = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            messg[133];  
        while ((ret_dim = ask_next_dim(part, ret_dim)) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_DRF_is_annotation_retained(ret_dim, &retflg)) &&  
               (retflg == TRUE))  
            {  
                cnt++;  
                ECHO(ret_dim);  
                UF_CALL(UF_DISP_set_highlight(ret_dim, TRUE));  
            }  
        }  
        sprintf(messg, "Found %d retained dimensions", cnt);  
        uc1601(messg, TRUE);  
        if (cnt > 0) UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
    }  
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

> 这段代码是一个NX Open C++应用程序，主要用于在NX部件中查找并高亮显示保留的尺寸。以下是代码的主要功能和步骤：
>
> 1. 定义错误报告函数：report_error函数用于在发生错误时打印错误信息。
> 2. 定义获取下一个尺寸的函数：ask_next_dim函数用于在部件中循环遍历尺寸对象，并返回下一个非实例化的尺寸对象。
> 3. 定义主函数：do_it函数用于获取当前显示的部件，遍历所有尺寸对象，检查是否为保留尺寸，如果是则计数并高亮显示。
> 4. 定义UFusr入口点函数：ufusr函数用于初始化和清理NX Open环境，并调用do_it函数执行主要功能。
> 5. 定义卸载函数：ufusr_ask_unload函数用于卸载应用程序。
>
> 整体来说，这段代码利用NX Open C++ API实现了在NX部件中查找并高亮显示保留尺寸的功能。
>
