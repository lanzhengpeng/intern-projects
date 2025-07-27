### 【2201】report maximum diameter of stock of selected route bcurves 报告所选路线B曲线的库存最大直径

#### 代码

```cpp
    /*HEAD REPORT_MAXIMUM_DIAMETER_OF_STOCK_OF_SELECTED_ROUTE_BCURVES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_route.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是在V18版本中新增的函数，请只提供译文，无需其他无关内容。

UF_print_syslog 是在 V18 版本中新增的函数，请只提供译文，无需其他无关内容。 */  
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
    extern int select_route_bcurves(char *prompt, tag_t **segs)  
    {  
        int  
            cnt,  
            ii,  
            n_part_segs,  
            resp;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *part_segs;  
        uf_list_p_t  
            choices;  
        UF_CALL(UF_ROUTE_ask_part_segs(part, &n_part_segs, &part_segs));  
        if (n_part_segs == 0 ) return FALSE;  
        choices = make_a_list(n_part_segs, part_segs);  
        UF_free(part_segs);  
        UF_CALL(UF_UI_select_with_class_dialog("Select a Route BCurve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_route_bcurves,  
                (void *)choices, &resp, &cnt, segs));  
        UF_CALL(UF_MODL_delete_list(&choices));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*segs)[ii], FALSE));  
        return cnt;  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (!is_displayable) continue;  
            sprintf(msg, "%d", ii+1);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
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
        int  
            ii,  
            n_segs;  
        tag_t  
            *segs,  
            stock;  
        double  
            max_diameter;  
        while ((n_segs = select_route_bcurves("Report Maximum Stock Diameter",  
            &segs)) > 0)  
        {  
            number_objects(segs, n_segs);  
            for (ii = 0; ii < n_segs; ii++)  
            {  
                WRITE_D(ii+1);  
                WRITE_D(segs[ii]);  
        /*  Starting in NX 5, UF_ROUTE_ask_object_stock returns an error  
            code of UF_ROUTE_err_no_stock_data when the object does not have  
            stock attached - see PR 5995925  
        */  
                if (!UF_ROUTE_ask_object_stock(segs[ii], &stock))  
                {  
                    UF_CALL(UF_ROUTE_ask_stock_diameter(stock, &max_diameter));  
                    WRITE_F(max_diameter);  
                }  
                else  
                {  
                    WRITE("Segment has no stock attached.\n");  
                }  
            }  
            UF_free(segs);  
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

> 这段NX二次开发代码的主要功能是选择并报告所选路径曲线的最大毛坯直径。
>
> 主要步骤包括：
>
> 1. 选择路径曲线：调用select_route_bcurves函数，通过对话框让用户选择路径曲线，并获取选择结果。
> 2. 编号和显示：对选择的路径曲线进行编号和显示。
> 3. 查询最大毛坯直径：对每条路径曲线，调用UF_ROUTE_ask_object_stock和UF_ROUTE_ask_stock_diameter查询最大毛坯直径。
> 4. 报告结果：将查询到的最大毛坯直径报告到日志窗口。
> 5. 释放内存：释放选择路径曲线的内存。
> 6. 循环选择：重复上述步骤，直到用户没有选择路径曲线。
>
> 代码还包括错误处理函数report_error，用于报告函数调用错误；创建列表的函数make_a_list；以及设置选择过滤器的函数mask_for_route_bcurves和is_a_choice。
>
> 总体而言，这段代码实现了选择路径曲线并报告最大毛坯直径的功能，为NX路径曲线操作提供了一个实用的示例。
>
