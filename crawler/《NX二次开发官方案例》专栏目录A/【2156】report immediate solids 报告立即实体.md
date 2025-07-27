### 【2156】report immediate solids 报告立即实体

#### 代码

```cpp
    /*HEAD REPORT_IMMEDIATE_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数，用于打印系统日志。 */  
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
    static tag_t ask_next_local_solid(tag_t part, tag_t solid)  
    {  
        int  
            subtype,  
            status,  
            type;  
        while ((solid = UF_OBJ_cycle_all(part, solid)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(solid, &type, &subtype));  
            status = UF_OBJ_ask_status(solid);  
            if ((type == UF_solid_type) &&  
                (subtype == UF_solid_body_subtype) &&  
                ((status == 3) || (UF_OBJ_is_object_a_promotion(solid))) &&  
                (!UF_ASSEM_is_occurrence(solid))) return solid;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            solid = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((solid = ask_next_local_solid(part, solid)) != NULL_TAG)  
            ECHO(solid);  
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

> 这段代码是一个NX Open C++的二次开发示例，其主要功能是遍历当前显示部件中的所有本地实体，并输出它们的tag。具体来说，代码的主要功能包括：
>
> 1. 定义了宏ECHO和UF_CALL，用于打印变量值和封装UF函数调用，并在出错时打印错误信息。
> 2. 实现了report_error函数，用于在UF函数调用出错时打印错误信息。
> 3. 实现了ask_next_local_solid函数，用于遍历部件中的所有实体，并返回下一个本地实体的tag。
> 4. 实现了do_it函数，用于遍历当前显示部件中的所有本地实体，并打印它们的tag。
> 5. 定义了ufusr函数，作为二次开发程序的入口点。在这个函数中，首先初始化UF环境，然后调用do_it函数执行主体功能，最后终止UF环境。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发程序在关闭时不进行清理工作。
>
> 综上所述，这段代码通过封装UF函数，实现了遍历当前显示部件中的所有本地实体，并输出它们的tag的功能，为NX的二次开发提供了示例。
>
