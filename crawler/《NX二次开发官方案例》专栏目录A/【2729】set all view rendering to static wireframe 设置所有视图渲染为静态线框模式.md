### 【2729】set all view rendering to static wireframe 设置所有视图渲染为静态线框模式

#### 代码

```cpp
    /*HEAD SET_ALL_VIEW_RENDERING_TO_STATIC_WIREFRAME CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本新增的功能。 */  
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
    static tag_t ask_next_view(tag_t part, tag_t view)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_view_type, &view));  
        return (view);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            view = NULL_TAG;  
        UF_VIEW_subtype_t  
            subtype;  
        UF_VIEW_type_t  
            type;  
        UF_VIEW_edge_display_options_t  
            options;  
        UF_VIEW_rendering_style_t  
            style;  
        while ((view = ask_next_view(part, view)) != NULL_TAG)  
        {  
            UF_CALL(UF_VIEW_ask_type(view, &type, &subtype));  
            if (type == UF_VIEW_MODEL_TYPE)  
            {  
                UF_CALL(UF_VIEW_ask_surface_display_options(view, &style, &options));  
                UF_CALL(UF_VIEW_set_surface_display_options(view,  
                    UF_VIEW_STATIC_WIREFRAME_STYLE, &options));  
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
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                report_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_save());  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，用于将所有视图设置为静态线框显示模式。具体来说，代码的主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印函数调用失败时的错误代码和消息。
> 2. 定义了一个函数ask_next_view，用于遍历部件中的所有视图，返回下一个视图的tag。
> 3. 主函数do_it遍历部件中的所有视图，将模型视图的表面显示模式设置为静态线框。
> 4. 定义了UFusr函数，这是NX Open C++应用程序的入口点。在这个函数中，初始化NX环境，调用do_it函数，然后终止NX环境。
> 5. main函数是主程序入口，用于打开指定的部件，调用do_it函数，然后保存并关闭部件。另外，main函数还处理了命令行参数，可以接受部件路径或数据库部件名。
> 6. report_load_status函数用于报告部件加载状态，打印加载失败的消息。
>
> 综上所述，这段代码的主要功能是遍历部件中的所有视图，并将模型视图的显示模式设置为静态线框。它是一个典型的NX Open C++应用程序，使用NX提供的API来操作部件和视图。
>
