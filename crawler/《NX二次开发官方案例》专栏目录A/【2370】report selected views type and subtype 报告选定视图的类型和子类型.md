### 【2370】report selected views type and subtype 报告选定视图的类型和子类型

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_VIEWS_TYPE_AND_SUBTYPE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的说明，UF_print_syslog是在V18版本中新增的功能。文档要求只提供译文，不添加其他内容。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static tag_t select_a_view(char *prompt)  
    {  
        int  
            view_of_cursor,  
            resp;  
        double  
            pos[3];  
        char  
            vname[MAX_ENTITY_NAME_SIZE+1];  
        tag_t  
            view = NULL_TAG;  
        UF_CALL( UF_UI_ask_cursor_view( &view_of_cursor));  
        UF_CALL( UF_UI_set_cursor_view( 0));  
        resp = uc1615(prompt, pos);  
        UF_CALL( UF_UI_set_cursor_view( view_of_cursor));  
        if (resp == 5)  
        {  
            UF_CALL( uc1653( vname ));  
            UF_CALL( UF_VIEW_ask_tag_of_view_name(vname, &view));  
            return view;  
        }  
        else  
            return NULL_TAG;  
    }  
    static void report_view_info(tag_t view)  
    {  
        int  
            ii,  
            n_regions,  
            subtype,  
            type;  
        tag_t  
            *regions;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_DRAW_break_region_data_t  
            break_region_data;  
        WRITE_D(view);  
        UF_CALL(UF_OBJ_ask_name(view, view_name));  
        WRITE_S(view_name);  
        UF_CALL(UF_VIEW_ask_type(view, &type, &subtype));  
        WRITE_D(type);  
        WRITE_D(subtype);  
        if ((type == UF_VIEW_DRAWING_MEMBER_TYPE) &&  
            (subtype == UF_VIEW_BREAK_SUBTYPE))  
        {  
            UF_CALL(UF_DRAW_ask_break_regions(view, &n_regions, &regions));  
            WRITE_D(n_regions);  
            for (ii = 0; ii < n_regions; ii++)  
            {  
                UF_CALL(UF_DRAW_ask_break_region_data(regions[ii],  
                    &break_region_data));  
                report_view_info(break_region_data.view_tag);  
            }  
            if (n_regions > 0) UF_free(regions);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            view;  
        while ((view = select_a_view("Report type and subtype")) != NULL_TAG)  
            report_view_info(view);  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发示例，主要用于报告选定视图的类型和子类型。其主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义了两个辅助函数write_string_to_listing_window和write_integer_to_listing_window，用于向列表窗口写入字符串和整数。
> 3. 定义了一个选择视图的函数select_a_view，用于提示用户选择一个视图，并返回视图的tag。
> 4. 定义了一个报告视图信息的函数report_view_info，用于报告给定视图的类型、子类型等信息。
> 5. 定义了一个主函数do_it，用于循环提示用户选择视图，并对每个选定的视图调用report_view_info函数。
> 6. 定义了ufusr函数，作为程序的入口点，初始化并调用do_it函数，最后终止。
> 7. 定义了ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 总的来说，这段代码通过提示用户选择视图，并报告每个视图的类型和子类型信息，展示了NX视图相关的二次开发功能。
>
