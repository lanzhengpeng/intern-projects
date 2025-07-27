### 【1211】export current drawing to file 将当前图纸导出到文件

#### 代码

```cpp
    /*HEAD EXPORT_CURRENT_DRAWING_TO_FILE CCC UFUN */  
    /* This PROGRAM takes the current drawing and exports out everything in the  
       drawing. Please note that the file that you are importing it to needs  
       to be loaded in the session in order for this work.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_view.h>  
    #include <uf_disp.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 版本新增的功能。它用于在 syslog 中打印日志消息。 */  
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
        char  
            draw_name[33],  
            view_name[33] = "",  
            import_into[]="/users/reyesf/junk";  
        int  
            num_objects = 0;  
        tag_t  
            view_dep_obj = NULL_TAG,  
            object_array[1000];  
        UF_PART_export_options_t  
            options;  
        uc6448(draw_name);  
        UF_CALL(UF_VIEW_ask_tag_of_view_name(draw_name, &object_array[0]));  
        UF_CALL(uc6492(draw_name));  
        num_objects = 1;  
        while(!uc6499(draw_name, view_name) && strcmp("", view_name))  
        {  
            printf("View name is : %s\n", view_name);  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name,  
                                                 &object_array[num_objects]));  
            num_objects ++;  
        }  
        while(!uc6457("", 2, &view_dep_obj) && (view_dep_obj != NULL_TAG))  
        {  
            object_array[num_objects] = view_dep_obj;  
            num_objects ++;  
        }  
        options.new_part = FALSE;  
        options.params_mode = UF_PART_maintain_params;  
        options.expression_mode = UF_PART_copy_exp_deeply;  
        UF_PART_export_with_options( import_into, num_objects,  
                                      object_array, &options);  
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

> 根据代码的内容，这是一段用于导出NX当前绘图的二次开发代码。
>
> 主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在发生UF函数调用错误时，打印错误信息到syslog和listing window。
> 2. 定义了do_it函数，用于获取当前绘图中的所有视图和视图相关的对象，然后调用UF_PART_export_with_options函数将这些对象导出到指定的文件路径。
> 3. 定义了ufusr函数，作为NX二次开发的入口函数，用于初始化和调用do_it函数。
> 4. 定义了ufusr_ask_unload函数，用于指定在卸载该二次开发程序时立即卸载。
> 5. 使用UF_CALL宏封装UF函数调用，用于在函数调用出错时自动报告错误。
> 6. 使用了一些自定义函数，如uc6448, uc6492, uc6499, uc6457，用于获取当前绘图中的视图信息，这些函数的具体实现未在代码中给出。
> 7. 在导出对象时，设置了UF_PART_export_options结构，包括新零件设置为FALSE，参数模式为保持参数，表达式模式为深度复制。
> 8. 通过UF_PART_export_with_options函数将对象数组导出到指定路径，实现了当前绘图的导出功能。
>
> 总的来说，这段代码实现了在NX中导出当前绘图的功能，包括视图和视图相关的对象，并通过自定义函数获取视图信息。
>
