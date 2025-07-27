### 【0884】create simple sxview 创建简单截面视图

#### 代码

```cpp
    /*HEAD CREATE_SIMPLE_SXVIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_drf.h>  
    #include <uf_draw.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以总结如下：

1. UF_print_syslog 是 V18 版本中新增的函数。
2. 该函数用于打印系统日志信息到日志文件。
3. 文档强调只需要提供译文，不需要添加任何额外内容。
4. 这条信息主要是为了通知用户，在 V18 版本中新增了 UF_print_syslog 函数。

综上所述，UF_print_syslog 是 V18 版本中新增的函数，用于打印系统日志信息到日志文件。 */  
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
    static tag_t select_drawing_member_view(char *prompt)  
    {  
        int  
            orig_cv,  
            resp;  
        tag_t  
            view_tag;  
        double  
            pos[3];  
        UF_CALL(UF_UI_ask_cursor_view(&orig_cv));  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_specify_screen_position(prompt, NULL, NULL, pos,  
            &view_tag, &resp));  
        UF_CALL(UF_UI_set_cursor_view(orig_cv));  
        if (resp == UF_UI_PICK_RESPONSE)  
            return view_tag;  
        else  
            return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static int mask_for_circular_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_circular_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select circular edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_circular_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_position(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            draw_tag,  
            pview_tag,  
            sxvw_tag;  
        double  
            arrow_dir[3] = { 0, 1, 0},  
            step_dir[3] = { 1, 0, 0},  
            vw_plc[3];  
        UF_DRF_object_t  
            cut_obj;  
        UF_CALL(UF_DRF_init_object_structure(&cut_obj));  
        if (!UF_CALL(UF_DRAW_ask_current_drawing(&draw_tag)) &&  
            ((pview_tag = select_drawing_member_view("select parent view"))  
                != NULL_TAG) &&  
            ((cut_obj.object_tag = select_a_circular_edge("select edge for cut"))  
                != NULL_TAG) &&  
            specify_position("Indicate position of section view", vw_plc))  
        {  
            cut_obj.object_view_tag = pview_tag;  
            cut_obj.object_assoc_type = UF_DRF_arc_center;  
            UF_CALL(UF_DRAW_create_simple_sxview(draw_tag, 1.0, step_dir,  
                arrow_dir, pview_tag, &cut_obj, vw_plc, &sxvw_tag));  
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

> 这段NX二次开发代码实现了创建简单截面视图的功能，其主要步骤包括：
>
> 1. 初始化NX环境，通过UF_DRAW_ask_current_drawing获取当前图纸。
> 2. 用户选择父视图，通过select_drawing_member_view函数实现。
> 3. 用户选择一个圆形边线作为切割对象，通过select_a_circular_edge函数实现。
> 4. 用户指定截面视图的位置，通过specify_position函数实现。
> 5. 调用UF_DRAW_create_simple_sxview函数，根据用户选择的父视图、切割对象、位置等信息，创建一个简单的截面视图。
> 6. 释放NX环境，结束程序。
>
> 此外，代码中还包含了错误处理函数report_error，用于在函数调用失败时打印错误信息。通过这些步骤，用户可以方便地通过交互方式创建截面视图。
>
