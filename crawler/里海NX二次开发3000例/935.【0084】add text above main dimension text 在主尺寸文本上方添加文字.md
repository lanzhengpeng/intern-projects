### 【0084】add text above main dimension text 在主尺寸文本上方添加文字

#### 代码

```cpp
    /*HEAD APPEND_TEXT_ABOVE_MAIN_DIMENSION_TEXT CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据上下文，UF_print_syslog 是 V18 中新增的函数。 */  
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
    /* 里海 */  
    static int mask_for_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void add_text_above_dimension(tag_t dim, char add_text[1][133])  
    {  
        int  
            above = 3,  
            ann_data[10],  
            ann_data_form,  
            ann_data_type,  
            center = 2,  
            cycle_flag = 0,  
            line_space = 0,  
            num_segs,  
            one = 1,  
            pure_text = 18,  
            search_mask[4] = { 0,0,1,0 };  
        double  
            angle,  
            origin[2];  
    /*  Find the segment that is the main dimension text 译:找到主要尺寸文本段。 */  
        while (!UF_CALL(UF_DRF_ask_ann_data(&dim, search_mask, &cycle_flag,  
            ann_data, &ann_data_type, &ann_data_form, &num_segs, origin, &angle))  
            && (cycle_flag != 0) && (ann_data_form != 1));  
        if (cycle_flag != 0)  
        {  
            UF_CALL(UF_DRF_add_to_dimension(&dim, &cycle_flag, ann_data,  
                &pure_text, &above, &center, &line_space, &one, add_text));  
            UF_CALL(UF_DISP_add_item_to_display(dim));  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            dim;  
        char  
            text[1][133] = { "" };  
        while (((dim = select_a_dimension("Select dimension")) != NULL_TAG) &&  
            (prompt_for_text("Text to append (above)", &text[0][0])))  
            add_text_above_dimension(dim, text);  
    }  
    /* 里海 */  
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

> 这段代码是一个NX二次开发示例，其主要功能是在选择的尺寸线上方添加文本注释。
>
> 具体来说，代码的主要部分包括：
>
> 1. 定义错误报告函数report_error，用于在发生UF函数调用错误时记录错误信息。
> 2. 定义函数mask_for_dimensions，用于设置选择过滤条件，只允许选择尺寸线。
> 3. 定义函数select_a_dimension，用于让用户选择一个尺寸线，并返回其tag。
> 4. 定义函数prompt_for_text，用于提示用户输入要添加的文本。
> 5. 定义函数add_text_above_dimension，用于在指定的尺寸线上方添加文本注释。
> 6. 定义函数do_it，用于循环让用户选择尺寸线并添加文本，直到用户取消选择。
> 7. 定义ufusr函数，作为NX二次开发的入口函数，初始化后调用do_it函数，完成后终止。
> 8. 定义ufusr_ask_unload函数，用于立即卸载NX二次开发应用程序。
>
> 通过以上各部分的协同工作，该代码实现了在选择的尺寸线上方添加文本注释的功能，为用户提供了便捷的尺寸文本注释添加工具。
>
