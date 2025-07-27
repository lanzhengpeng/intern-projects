### 【0065】add id symbol after dimensions tolerance 在公差后添加ID符号

#### 代码

```cpp
    /*HEAD APPEND_ID_SYMBOL_AFTER_DIMENSIONS_TOLERANCE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文中的内容，UF_print_syslog是V18版本中新增的功能。翻译如下：

请注意，UF_print_syslog是V18版本新增的功能。 */  
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
    static void add_id_symbol_after_dim_tolerance(tag_t dim, char *letter)  
    {  
        int  
            after = 2,  
            ann_data[10],  
            ann_data_form,  
            ann_data_type,  
            center = 2,  
            cycle_flag = 0,  
            line_space = 0,  
            num_segs,  
            one = 1,  
            circle = 19,  
            search_mask[4] = { 0,0,1,0 };  
        double  
            angle,  
            origin[2];  
        char  
            add_text[1][133];  
        strcpy(&add_text[0][0], letter);  
    /*  Find the segment that is the tolerance text 译:找到容差文本的段落。 */  
        while (!UF_CALL(UF_DRF_ask_ann_data(&dim, search_mask, &cycle_flag,  
            ann_data, &ann_data_type, &ann_data_form, &num_segs, origin, &angle))  
            && (cycle_flag != 0) && (ann_data_form != 3));  
        if (cycle_flag != 0)  
        {  
            UF_CALL(UF_DRF_add_to_dimension(&dim, &cycle_flag, ann_data,  
                &circle, &after, &center, &line_space, &one, add_text));  
            UF_CALL(UF_DISP_add_item_to_display(dim));  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            dim;  
        while ((dim = select_a_dimension("Select dimension")) != NULL_TAG)  
            add_id_symbol_after_dim_tolerance(dim, "A");  
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

> 这是一段用于在NX中添加ID符号的二次开发代码。主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在发生UF函数调用错误时打印错误信息。
> 2. 定义了选择过滤器mask_for_dimensions，用于限制选择为尺寸标注。
> 3. 定义了选择尺寸标注的函数select_a_dimension，用于通过对话框让用户选择一个尺寸标注。
> 4. 定义了添加ID符号的函数add_id_symbol_after_dim_tolerance，用于在选定的尺寸标注后添加一个ID符号。
> 5. 定义了主函数do_it，循环调用select_a_dimension和add_id_symbol_after_dim_tolerance，让用户选择尺寸标注并添加ID符号。
> 6. 定义了UF函数ufusr，作为程序的入口点，初始化UF后调用do_it函数，并在结束时终止UF。
> 7. 定义了卸载函数ufusr_ask_unload，用于立即卸载该程序。
>
> 总的来说，该代码实现了在NX中选择尺寸标注并添加ID符号的功能。
>
