### 【1401】highlight sectioned solid 突出显示分段实体

#### 代码

```cpp
    /*HEAD HIGHLIGHT_SECTIONED_SOLID CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 新增的功能。 */  
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
    static int mask_for_xhatch(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = {{ UF_drafting_entity_type, UF_draft_crosshatch_subtype, 0 },  
                       { UF_solid_section_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_xhatch(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select cross hatching", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_xhatch, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            gtype,  
            gsubtype;  
        tag_t  
            group,  
            solid,  
            xhatch;  
        while ((xhatch = select_xhatch("Highlight sectioned solid")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRAW_ask_group_of_curve(xhatch, &group, &gtype,  
                &gsubtype));  
            if (gtype == UF_solid_section_type)  
            {  
                UF_CALL(UF_DRAW_ask_solid_of_section(group, &solid));  
                UF_CALL(UF_DISP_set_highlight(solid, 1));  
                uc1601("Solid highlighted", TRUE);  
                UF_CALL(UF_DISP_set_highlight(solid, 0));  
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

> 这段代码是一个NX Open C++二次开发应用程序，用于高亮显示带横截面的实体。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印出函数调用失败时的错误信息。
> 2. 定义了一个选择掩码函数mask_for_xhatch，用于选择横截面图案。
> 3. 定义了一个选择横截面图案函数select_xhatch，用于选择用户指定的横截面图案。
> 4. 定义了一个主函数do_it，用于循环选择横截面图案，获取图案所属的横截面组，查询横截面对应的实体，并高亮显示该实体。
> 5. 定义了ufusr函数，作为程序的入口点，初始化NX Open环境，调用do_it函数执行主功能，并在结束时清理NX Open环境。
> 6. 定义了卸载函数ufusr_ask_unload，用于立即卸载应用程序。
>
> 整体来说，这段代码通过交互式选择横截面图案，高亮显示对应的实体，实现了NX中横截面图案与实体的关联功能。
>
