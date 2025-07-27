### 【2718】set all dwg view layers 设置所有图形视图图层

#### 代码

```cpp
    /*HEAD SET_ALL_DWG_VIEW_LAYERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
    #define ECHO(X)   (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
    static void do_it(void)  
    {  
        int  
            irc,  
            status,  
            vis_mask[256];  
        tag_t  
            dwg = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            view[MAX_ENTITY_NAME_SIZE+1],  
            drawing[MAX_ENTITY_NAME_SIZE+1];  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &dwg)) &&  
              (dwg != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_name(dwg, drawing));  
            printf("drawing = %s\n", drawing);  
            strcpy(view, "");  
            while (!UF_CALL(uc6499(drawing, view)) && (strcmp(view, "")))  
            {  
                printf("   view = %s\n", view);  
                if ((irc = uc6446(view, &status, vis_mask)) == 0)  
                {  
                    vis_mask[0] = 0;  
                    ECHO(uc6447(view, 2, vis_mask));  
                }  
                else  
                    printf("uc6446 returned %d\n", irc);  
            }  
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

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要用于设置NX图纸中的视图层的可见性。下面是代码的主要功能介绍：
>
> 1. 头文件包含：包含了必要的NX Open C++ API头文件。
> 2. 宏定义：定义了ECHO和UF_CALL两个宏，用于输出调试信息和错误处理。
> 3. 错误处理函数：定义了report_error函数，用于输出函数调用失败时的错误信息。
> 4. 视图层可见性设置：定义了do_it函数，用于遍历图纸中的所有视图，并设置视图层的可见性。
> 5. 主函数：定义了ufusr函数，作为程序的入口。在初始化NX环境后，调用do_it函数，然后终止NX环境。
> 6. 卸载函数：定义了ufusr_ask_unload函数，用于在卸载用户程序插件时返回立即卸载标志。
> 7. 主体逻辑：do_it函数中，首先获取图纸显示部件，然后循环遍历图纸中的所有视图，获取视图名称，并设置视图的图层可见性。
> 8. 调试和错误处理：使用了宏定义进行错误处理和调试信息输出。
>
> 总体来说，这段代码实现了遍历图纸中的所有视图，并根据需要设置视图的图层可见性，是一个典型的NX二次开发示例。
>
