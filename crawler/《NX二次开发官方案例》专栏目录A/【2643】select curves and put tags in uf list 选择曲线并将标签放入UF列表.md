### 【2643】select curves and put tags in uf list 选择曲线并将标签放入UF列表

#### 代码

```cpp
    /*HEAD SELECT_CURVES_AND_PUT_TAGS_IN_UF_LIST CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数，用于打印系统日志。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    /* qq3123197280 */  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
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
    static void do_it(void)  
    {  
        int  
            num_crvs = 0,  
            inx = 0;  
        tag_t  
            *curves,  
            list_item = NULL_TAG;  
        uf_list_p_t  
            my_list;  
        num_crvs = select_curves( "Select curves", &curves );  
        if( 0 == num_crvs ) return;  
        UF_CALL( UF_MODL_create_list( &my_list ));  
        for( inx = 0; inx < num_crvs; inx++ ) /* put curves in list 里海译:将曲线添加到列表中。 */  
        {  
            UF_CALL( UF_MODL_put_list_item( my_list, curves[inx] ));  
        }  
               /* at this point the curves have been added to the list 里海译:在这个时候，曲线已经被添加到列表中了。 */  
        for( inx = 0; inx < num_crvs; inx++ )    /* print the list 里海译:print the list的翻译是打印列表。 */  
        {  
            WRITE_D( inx );  
            UF_CALL( UF_MODL_ask_list_item( my_list, inx, &list_item ));  
            WRITE_D( list_item );  
        }  
        UF_CALL( UF_MODL_delete_list( &my_list ));  
        UF_free( curves );  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数执行出错时打印错误信息。
> 2. 定义了一个用于选择曲线的函数mask_for_curves，设置了选择过滤条件，只选择线、圆、锥曲线、样条曲线和点。
> 3. 定义了一个写整数到列表窗口的函数write_integer_to_listing_window。
> 4. 定义了一个选择曲线的函数select_curves，弹出一个选择对话框，让用户选择曲线，并返回曲线的tag数组。
> 5. 在主函数do_it中，首先调用select_curves获取用户选择的曲线，然后创建一个列表，将曲线添加到列表中，并遍历列表打印曲线的tag。
> 6. 定义了ufusr函数，在NX启动时自动调用，执行do_it函数。
> 7. 定义了ufusr_ask_unload函数，在NX卸载时自动调用，返回立即卸载。
>
> 通过这段代码，实现了在NX中创建一个列表，并将用户选择的曲线添加到列表中，然后遍历并打印列表中的曲线tag。
>
