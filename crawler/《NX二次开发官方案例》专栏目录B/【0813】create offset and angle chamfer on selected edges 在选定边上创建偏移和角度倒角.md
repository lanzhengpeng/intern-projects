### 【0813】create offset and angle chamfer on selected edges 在选定边上创建偏移和角度倒角

#### 代码

```cpp
    /*HEAD CREATE_OFFSET_AND_ANGLE_CHAMFER_ON_SELECTED_EDGES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_edges(char *prompt, tag_t **edges)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select edges", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_edges, NULL, &resp, &cnt, edges));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*edges)[ii], FALSE));  
        return cnt;  
    }  
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
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
            n_edges;  
        tag_t  
            chamfer,  
            *edges;  
        double  
            angle = 45,  
            offset = 0.25;  
        char  
            angle_str[20],  
            offset_str[20];  
        uf_list_p_t  
            edge_list;  
        while (((n_edges = select_edges("Chamfer", &edges)) > 0) &&  
            prompt_for_two_numbers("Chamfer", "Offset", "Angle", &offset, &angle))  
        {  
            sprintf(angle_str, "%f", angle);  
            sprintf(offset_str, "%f", offset);  
            strip_trailing_zeros(angle_str);  
            strip_trailing_zeros(offset_str);  
            edge_list = make_a_list(n_edges, edges);  
            UF_free(edges);  
        /*  The "0" is a necessary dummy value - see PR 4789614 译:"The '0' is a necessary dummy value - see PR 4789614" */  
            UF_CALL(UF_MODL_create_chamfer( 3, offset_str, "0", angle_str,  
                edge_list, &chamfer));  
            UF_CALL(UF_MODL_delete_list(&edge_list));  
        }  
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

> 这段NX Open C++代码用于在NX中创建一个“偏移和角度斜面”功能。主要功能和步骤包括：
>
> 1. 错误报告函数：用于报告UF函数的错误信息。
> 2. 选择边界的函数：用于选择要斜面的边界。
> 3. 提示输入偏移和角度的函数：用于提示用户输入偏移量和角度值。
> 4. 创建边界列表的函数：用于将用户选择的边界创建为列表。
> 5. 主函数：用于循环选择边界，提示用户输入偏移量和角度，然后创建斜面特征。
> 6. UF初始化和终止：用于初始化和终止NX Open。
> 7. 卸载函数：用于卸载用户自定义函数。
>
> 总体来说，这段代码通过NX Open API实现了选择边界，提示输入参数，创建斜面特征等功能，展示了NX Open C++编程的基本流程和技巧。
>
