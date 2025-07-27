### 【0899】create smart line 创建智能线条

#### 代码

```cpp
    /*HEAD CREATE_SMART_LINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中新增的函数。在 V18 版本中，UF_print_syslog 是一个新增加的函数。 */  
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
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            points[2],  
            so_line;  
        while (((points[0] = select_a_point("Select line start")) != NULL_TAG) &&  
               ((points[1] = select_a_point("Select line end")) != NULL_TAG))  
        {  
            UF_CALL(UF_SO_create_line_two_points(points[0],  
                UF_SO_update_within_modeling, points, &so_line));  
            UF_CALL(UF_SO_set_visibility_option(so_line, UF_SO_visible));  
            UF_CALL(UF_SO_display(so_line, UF_DISP_ALL_ACTIVE_VIEWS,  
                UF_DISP_USE_SPECIFIED_COLOR, UF_OBJ_YELLOW));  
        }  
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

> 这段代码是用于NX的二次开发，实现了在NX中选择两个点并创建一条智能线的功能。具体代码功能如下：
>
> 1. 错误报告函数：使用report_error函数来报告UF函数调用错误，并在控制台和日志窗口中输出错误信息。
> 2. 点选择掩码：使用mask_for_points函数设置选择掩码，只允许选择点对象。
> 3. 选择点对象：使用select_a_point函数提示用户选择一个点，并返回点的tag。
> 4. 创建智能线：在do_it函数中，循环提示用户选择两个点，然后使用UF_SO_create_line_two_points函数创建一条智能线，并设置其可见性和颜色。
> 5. UF初始化和终止：在ufusr函数中初始化和终止UF，调用do_it函数执行功能。
> 6. 卸载请求：ufusr_ask_unload函数用于请求立即卸载NX Open应用程序。
>
> 总体来说，这段代码实现了在NX中选择两点并创建智能线的基本功能，包含错误处理和选择过滤等。
>
