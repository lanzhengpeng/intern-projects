### 【2200】report maximum diameter of stock of selected route bcurve 报告选定路径BCURVE的库存最大直径

#### 代码

```cpp
    /*HEAD REPORT_MAXIMUM_DIAMETER_OF_STOCK_OF_SELECTED_ROUTE_BCURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_route.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中新增的功能。 */  
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
    /* qq3123197280 */  
    static int is_a_choice(tag_t object, int type[3], void *user_data,  
        UF_UI_selection_p_t select)  
    {  
        uf_list_p_t  
            choices = (uf_list_p_t)user_data;  
        for (; choices != NULL; choices = choices->next)  
        {  
            if (object == choices->eid) return UF_UI_SEL_ACCEPT;  
        }  
        return UF_UI_SEL_REJECT;  
    }  
    /* qq3123197280 */  
    static int mask_for_route_bcurves(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        UF_CALL(UF_UI_set_sel_procs(select, is_a_choice, NULL, user_data));  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_route_bcurve(char *prompt)  
    {  
        int  
            n_part_segs,  
            resp;  
        tag_t  
            object,  
            part = UF_PART_ask_display_part(),  
            *part_segs,  
            view;  
        double  
            cp[3];  
        uf_list_p_t  
            choices;  
        UF_CALL(UF_ROUTE_ask_part_segs(part, &n_part_segs, &part_segs));  
        if (n_part_segs == 0 ) return FALSE;  
        choices = make_a_list(n_part_segs, part_segs);  
        UF_free(part_segs);  
        UF_CALL(UF_UI_select_with_single_dialog("Select a Route BCurve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_route_bcurves,  
                (void *)choices, &resp, &object, cp, &view));  
        UF_CALL(UF_MODL_delete_list(&choices));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            seg,  
            stock;  
        double  
            max_diameter;  
        while ((seg = select_a_route_bcurve("Report Maximum Diameter of Stock"))  
            != NULL_TAG)  
        {  
        /*  Starting in NX 5, UF_ROUTE_ask_object_stock returns an error  
            code of UF_ROUTE_err_no_stock_data when the object does not have  
            stock attached - see PR 5995925  
        */  
            if (!UF_ROUTE_ask_object_stock(seg, &stock))  
            {  
                UF_CALL(UF_ROUTE_ask_stock_diameter(stock, &max_diameter));  
                WRITE_F(max_diameter);  
            }  
            else  
            {  
                WRITE("Segment has no stock attached.\n");  
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

> 这段代码是一个NX的二次开发程序，主要实现了以下功能：
>
> 1. 错误报告函数：定义了report_error函数，用于在出现UF函数调用错误时打印错误信息，包括文件名、行号、调用函数名和错误码。
> 2. 创建选择列表：定义了make_a_list函数，用于根据给定的对象数组创建一个UF列表。
> 3. 选择过滤器：定义了is_a_choice函数，用于检查给定的对象是否在选择列表中，若在则接受该对象，否则拒绝。
> 4. 选择掩码：定义了mask_for_route_bcurves函数，用于设置选择对话框的选择掩码，只允许选择线、圆、圆锥和样条曲线。
> 5. 选择路径BCurve：定义了select_a_route_bcurve函数，用于弹出一个选择对话框，让用户选择一个路径BCurve对象。
> 6. 查询和报告：在do_it函数中，循环调用select_a_route_bcurve函数让用户选择路径BCurve，然后查询每个BCurve的毛坯信息，并报告最大直径。
> 7. 主函数：ufusr是程序的入口函数，初始化UF库，调用do_it函数，然后终止UF库。
> 8. 卸载请求：ufusr_ask_unload函数用于返回立即卸载请求。
>
> 总的来说，这段代码通过交互式选择实现了查询路径BCurve毛坯最大直径的功能，同时包含了错误处理、选择过滤等辅助功能。
>
