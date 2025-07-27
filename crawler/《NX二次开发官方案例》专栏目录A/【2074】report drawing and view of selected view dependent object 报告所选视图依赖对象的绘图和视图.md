### 【2074】report drawing and view of selected view dependent object 报告所选视图依赖对象的绘图和视图

#### 代码

```cpp
    /*HEAD REPORT_DRAWING_AND_VIEW_OF_SELECTED_VIEW_DEPENDENT_OBJECT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，可以翻译为：

注意：UF_print_syslog 是在 V18 中新增的。请只提供译文，不要添加任何无关的评论。 */  
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
    /* qq3123197280 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            status,  
            view_subtype,  
            view_type;  
        tag_t  
            drawing,  
            object,  
            view;  
        char  
            drawing_name[MAX_ENTITY_NAME_SIZE+1],  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        while ((object = select_anything("Select object")) != NULL_TAG)  
        {  
            UF_CALL(uc6409(object, &status, view_name));  
            WRITE_D(status);  
            if (status == 1)  
            {  
                WRITE_S(view_name);  
                UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view));  
                WRITE_D(view);  
                UF_CALL(UF_VIEW_ask_type(view, &view_type, &view_subtype));  
                WRITE_D(view_type);  
                WRITE_D(view_subtype);  
                if ((view_type == UF_VIEW_DRAWING_SHEET_TYPE) ||  
                    (view_type == UF_VIEW_DRAWING_MEMBER_TYPE))  
                {  
                    UF_CALL(UF_DRAW_ask_drawing_of_view(view, &drawing));  
                    WRITE_D(drawing);  
                    UF_CALL(UF_OBJ_ask_name(drawing, drawing_name));  
                    WRITE_S(drawing_name);  
                }  
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

> 这段代码是NX的二次开发代码，主要用于选择对象并获取其相关的视图信息。具体功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了一个选择对象的面和边的选择掩码mask_add_faces_and_edges，用于在对话框中选择对象时应用。
> 3. 定义了一个选择任意对象的函数select_anything，它使用对话框让用户选择一个对象，并返回其tag。
> 4. 定义了两个辅助函数write_integer_to_listing_window和write_string_to_listing_window，用于将信息输出到列表窗口。
> 5. 定义了主函数do_it，用于不断循环让用户选择对象，然后获取该对象的状态、视图名称、视图tag、视图类型、视图子类型等信息，并输出到列表窗口。如果视图是图纸或图纸成员，还会获取其相关的图纸名称并输出。
> 6. 定义了ufusr函数作为二次开发的入口函数，初始化NX环境，调用主函数do_it，然后终止NX环境。
> 7. 定义了ufusr_ask_unload函数，用于在卸载用户函数时立即卸载。
>
> 综上所述，这段代码的主要功能是让用户选择对象，获取其相关的视图信息，并将这些信息输出到列表窗口，以实现NX二次开发中对视图信息的查询功能。
>
