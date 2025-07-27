### 【0957】create tube along selected curves 创建沿选定曲线的管子

#### 代码

```cpp
    /*HEAD CREATE_TUBE_ALONG_SELECTED_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是 V18 中的新功能，请只提供译文，无需其他无关内容。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_point_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select curves", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }  
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    static void do_it(void)  
    {  
        int  
            n_curves;  
        tag_t  
            *curves;  
        char  
            *limits[2] = { "9.85", "5.35" };  
        uf_list_p_t  
            path_list,  
            tube_list;  
        if ((n_curves = select_curves("Tube path", &curves)) > 0)  
        {  
            path_list = make_a_list(n_curves, curves);  
            if (!UF_CALL(UF_MODL_create_tube(path_list, limits, UF_NULLSIGN,  
                &tube_list))) UF_CALL(UF_MODL_delete_list(&tube_list));  
            UF_CALL(UF_MODL_delete_list(&path_list));  
            UF_free(curves);  
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

> 这段代码是一个NX的二次开发示例，主要功能是根据用户选择的曲线创建管状实体。
>
> 代码的主要逻辑如下：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时输出错误信息。
> 2. 定义了一个选择曲线的函数select_curves，用于弹出选择对话框，让用户选择曲线实体。这里使用了自定义的选择掩码，只允许选择线、圆、锥曲线、样条线和点。
> 3. 定义了一个创建列表的函数make_a_list，用于根据用户选择的曲线创建UF_MODL列表。
> 4. 定义了主体函数do_it，首先调用select_curves获取用户选择的曲线，然后创建路径列表path_list。接着调用UF_MODL_create_tube根据路径列表创建管状实体，并将结果保存在tube_list中。
> 5. 最后在ufusr函数中初始化NX环境，调用do_it函数，然后终止NX环境。
> 6. 定义了ufusr_ask_unload函数，用于在卸载时立即卸载该模块。
>
> 总的来说，这段代码实现了根据用户选择的曲线创建管状实体的功能，使用自定义选择掩码，并通过UF_MODL列表来创建实体，体现了NX二次开发的基本流程。
>
