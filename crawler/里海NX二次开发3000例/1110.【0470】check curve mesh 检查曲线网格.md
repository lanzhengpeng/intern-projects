### 【0470】check curve mesh 检查曲线网格

#### 代码

```cpp
    /*HEAD CHECK_CURVE_MESH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：
注意：UF_print_syslog是V18版本新增的，请只回答翻译，不要添加其他内容。 */  
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
    static void display_temporary_line(double start[3], double end[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
    }  
    static void do_it(void)  
    {  
        int  
            big_ii,  
            big_jj,  
            ii,  
            jj,  
            n_cross,  
            n_primary;  
        tag_t  
            *cross,  
            *primary;  
        double  
            big_gap,  
            gap,  
            big_pos1[3],  
            big_pos2[3],  
            not_used,  
            pos1[3],  
            pos2[3];  
        char  
            msg[MAX_LINE_SIZE+1];  
        while (  
            ((n_primary = select_curves("Select primary curves", &primary)) > 0)  
            && ((n_cross = select_curves("Select cross curves", &cross)) > 0))  
        {  
            big_gap = 0;  
            for (ii = 0; ii < n_primary; ii++)  
                for (jj = 0; jj < n_cross; jj++)  
                {  
                    UF_CALL(UF_MODL_ask_minimum_dist_3(2, primary[ii], cross[jj],  
                        FALSE, NULL, FALSE, NULL, &gap, pos1, pos2, &not_used));  
                    if (gap > big_gap)  
                    {  
                        big_gap = gap;  
                        big_ii = ii;  
                        big_jj = jj;  
                        UF_VEC3_copy(pos1, big_pos1);  
                        UF_VEC3_copy(pos2, big_pos2);  
                    }  
                }  
            if (big_gap > 0)  
            {  
                display_temporary_line(big_pos1, big_pos2);  
                UF_DISP_set_highlight(primary[big_ii], TRUE);  
                UF_DISP_set_highlight(cross[big_jj], TRUE);  
                sprintf(msg, "largest gap = %f", big_gap);  
                uc1601(msg, TRUE);  
                UF_DISP_set_highlight(primary[big_ii], FALSE);  
                UF_DISP_set_highlight(cross[big_jj], FALSE);  
            }  
            else uc1601("No gaps found", TRUE);  
            UF_free(primary);  
            UF_free(cross);  
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

> 这段代码是一个NX的二次开发示例，主要功能是计算两组曲线之间的最小距离，并高亮显示距离最大的两条曲线。以下是代码的主要功能：
>
> 1. 头文件包含：包含了必要的NX头文件，如uf.h、uf_ui.h等。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 3. 曲线选择掩码：定义了一个曲线选择掩码mask_for_curves，用于在选择曲线时过滤出线、圆、圆锥曲线、样条曲线和点。
> 4. 曲线选择函数：定义了一个曲线选择函数select_curves，用于弹出选择对话框，并返回选择的曲线数组。
> 5. 临时线显示函数：定义了一个临时线显示函数display_temporary_line，用于显示两条曲线之间的最小距离线。
> 6. 主函数：定义了主函数do_it，用于计算两组曲线之间的最小距离，并高亮显示距离最大的两条曲线。
> 7. ufusr函数：定义了NX的入口函数ufusr，用于初始化NX环境，调用主函数，然后关闭NX环境。
> 8. 卸载函数：定义了卸载函数ufusr_ask_unload，用于在卸载NX时立即释放内存。
>
> 总的来说，这段代码实现了NX中两组曲线之间的最小距离计算和高亮显示功能，为NX的二次开发提供了曲线处理的基本示例。
>
