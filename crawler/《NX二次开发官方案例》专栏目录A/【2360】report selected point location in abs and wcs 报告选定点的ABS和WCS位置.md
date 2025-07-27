### 【2360】report selected point location in abs and wcs 报告选定点的ABS和WCS位置

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_POINT_LOCATION_IN_ABS_AND_WCS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我总结翻译如下：

UF_print_syslog 是 V18 版本新增的函数，用于打印系统日志信息。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    /* qq3123197280 */  
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a point", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void map_abs2wcs(double *cc)  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            wcs[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));  
        FTN(uf5940)(abs_mx, wcs, mx, &irc);  
        FTN(uf5941)(cc, mx);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            point = NULL_TAG;  
        double  
            pnt_loc[3] = { 0., 0., 0. };  
        while (((point = select_a_point( "Select a Point:" )) != NULL_TAG ))  
        {  
            UF_CALL( UF_CURVE_ask_point_data( point, pnt_loc ));  
            WRITE( "Relative to ABS CSYS:\n" );  
            WRITE3F( pnt_loc );  
            map_abs2wcs( pnt_loc );  
            WRITE( "Relative to Current WCS:\n" );  
            WRITE3F( pnt_loc );  
            WRITE( "\n\n" );  
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

> 这段代码是用于NX的二次开发，其主要功能是让用户选择一个点，并显示该点相对于绝对坐标系和当前工作坐标系的坐标值。
>
> 代码主要包含以下几个部分：
>
> 1. 错误报告函数：用于在调用NX API函数出错时打印错误信息。
> 2. 写数组到列表窗口的函数：用于将数组信息格式化后输出到NX的列表窗口。
> 3. 点选择掩码函数：用于设置点选择对话框的筛选条件，只允许选择点对象。
> 4. 选择点函数：用于弹出一个点选择对话框，让用户选择一个点，并返回该点的tag。
> 5. 坐标系转换函数：用于将点从绝对坐标系转换到当前的工作坐标系。
> 6. 主函数：用于循环让用户选择点，然后显示该点在两个坐标系下的坐标。
> 7. UFusr函数：NX二次开发的入口函数，用于初始化和调用主函数。
> 8. 卸载函数：用于设置二次开发的卸载模式。
>
> 总体来说，这段代码实现了在NX中选择一个点，并查看其在两个坐标系下的坐标的功能。
>
