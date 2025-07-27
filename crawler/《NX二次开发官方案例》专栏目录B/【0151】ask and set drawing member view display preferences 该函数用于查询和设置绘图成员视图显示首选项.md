### 【0151】ask and set drawing member view display preferences 该函数用于查询和设置绘图成员视图显示首选项

#### 代码

```cpp
    /*HEAD ASK_AND_SET_DRAWING_MEMBER_VIEW_DISPLAY_PREFERENCES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 新增的功能，用于打印系统日志。 */  
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
    /*  This function does the same validity checks as the function  
        UF_DRAW_set_view_display in V18.  Some of the data returned by  
        UF_DRAW_ask_view_display does not meet these checks.  
        See PR 4363501.  
    */  
    static void fixup_view_display_parms(UF_DRAW_view_prfs_p_t view_parms)  
    {  
        if (view_parms->hidden_line < UF_DRAW_hidden_line_removal_off ||  
            view_parms->hidden_line > UF_DRAW_hidden_line_removal_on)  
            view_parms->hidden_line = UF_DRAW_hidden_line_removal_off;  
        if (view_parms->edge_hiding_edge < UF_DRAW_edge_hiding_edge_off ||  
            view_parms->edge_hiding_edge > UF_DRAW_edge_hiding_edge_on)  
            view_parms->edge_hiding_edge = UF_DRAW_edge_hiding_edge_off;  
        if (view_parms->smooth < UF_DRAW_smooth_off ||  
            view_parms->smooth > UF_DRAW_smooth_on)  
            view_parms->smooth = UF_DRAW_smooth_off;  
        if (view_parms->silhouettes < UF_DRAW_silhouettes_off ||  
            view_parms->silhouettes > UF_DRAW_silhouettes_on)  
            view_parms->silhouettes = UF_DRAW_silhouettes_off;  
        if (view_parms->uvhatch < UF_DRAW_uvhatch_off ||  
            view_parms->uvhatch > UF_DRAW_uvhatch_on)  
            view_parms->uvhatch = UF_DRAW_uvhatch_off;  
        if (view_parms->tolerance < 0.0)  
            view_parms->tolerance = 0.0;  
        if (view_parms->hidden_line_color < 0 || view_parms->hidden_line_color > 15)  
            view_parms->hidden_line_color = 0;  
        if (view_parms->hidden_line_font < 0 || view_parms->hidden_line_font > 7)  
            view_parms->hidden_line_font = 0;  
        if (view_parms->hidden_line_width < -1 || view_parms->hidden_line_width > 2)  
            view_parms->hidden_line_width = -1;  
        if (view_parms->smooth_edge_color < 0 || view_parms->smooth_edge_color > 15)  
            view_parms->smooth_edge_color = 0;  
        if (view_parms->smooth_edge_font < 0 || view_parms->smooth_edge_font > 7)  
            view_parms->smooth_edge_font = 0;  
        if (view_parms->smooth_edge_width < -1 || view_parms->smooth_edge_width > 2)  
            view_parms->smooth_edge_width = -1;  
        if (view_parms->smooth_edge_gap < UF_DRAW_gap_off ||  
            view_parms->smooth_edge_gap > UF_DRAW_gap_on )  
            view_parms->smooth_edge_gap = UF_DRAW_gap_off;  
        if (view_parms->smooth_edge_gap_size < 0.0 )  
            view_parms->smooth_edge_gap_size = 0.0;  
        if (view_parms->virtual_intersect < UF_DRAW_virtual_intersect_off ||  
            view_parms->virtual_intersect > UF_DRAW_virtual_intersect_on )  
            view_parms->virtual_intersect = UF_DRAW_virtual_intersect_off;  
        if (view_parms->virtual_intersect_color < 0 ||  
            view_parms->virtual_intersect_color > 15)  
            view_parms->virtual_intersect_color = 0;  
        if (view_parms->virtual_intersect_font < 0 ||  
            view_parms->virtual_intersect_font > 7)  
            view_parms->virtual_intersect_font = 0;  
        if (view_parms->virtual_intersect_width < -1 ||  
            view_parms->virtual_intersect_width > 2)  
            view_parms->virtual_intersect_width = -1;  
        if (view_parms->virtual_intersect_gap < UF_DRAW_gap_off ||  
            view_parms->virtual_intersect_gap > UF_DRAW_gap_on )  
            view_parms->virtual_intersect_gap = UF_DRAW_gap_off;  
        if (view_parms->virtual_intersect_gap_size < 0.0 )  
            view_parms->virtual_intersect_gap_size = 0.0;  
        if (view_parms->extracted_edges != UF_DRAW_extracted_edges_on &&  
            view_parms->extracted_edges != UF_DRAW_extracted_edges_off )  
            view_parms->extracted_edges = UF_DRAW_extracted_edges_on;  
        if (view_parms->visible_line_color < 0 ||  
            view_parms->visible_line_color > 15)  
            view_parms->visible_line_color = 0;  
        if (view_parms->visible_line_font < 0 || view_parms->visible_line_font > 7)  
            view_parms->visible_line_font = 0;  
        if (view_parms->visible_line_width < -1 ||  
            view_parms->hidden_line_width > 2)  
            view_parms->visible_line_width = -1;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_views;  
        tag_t  
            drawing,  
            *views;  
        UF_DRAW_view_prfs_t  
            view_parms;  
        if (!UF_CALL(UF_DRAW_ask_current_drawing(&drawing)) &&  
            (drawing != NULL_TAG))  
        {  
            UF_CALL(UF_DRAW_ask_views(drawing, &n_views, &views));  
            for (ii = 0; ii < n_views; ii++)  
            {  
                UF_CALL(UF_DRAW_ask_view_display(views[ii], &view_parms));  
                fixup_view_display_parms(&view_parms);  
                view_parms.visible_line_font = UF_OBJ_FONT_SOLID;  
                UF_CALL(UF_DRAW_set_view_display(views[ii], &view_parms));  
            }  
            if (n_views > 0) UF_free(views);  
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

> 根据代码的注释和函数定义，这段代码实现了以下功能：
>
> 1. 获取当前图纸：首先调用UF_DRAW_ask_current_drawing()函数获取当前图纸的tag。
> 2. 获取所有视图：接着调用UF_DRAW_ask_views()函数获取当前图纸中所有视图的数组，包括视图的数量和每个视图的tag。
> 3. 遍历每个视图：对于每个视图，调用UF_DRAW_ask_view_display()函数获取视图的显示参数，然后调用fixup_view_display_parms()函数修正参数，确保其在有效范围内。
> 4. 设置视图参数：将视图的visible_line_font设置为UF_OBJ_FONT_SOLID，并调用UF_DRAW_set_view_display()函数设置视图的显示参数。
> 5. 释放内存：如果视图数组不为空，则调用UF_free()函数释放视图数组的内存。
> 6. 初始化和终止：在函数ufusr()中调用UF_initialize()进行初始化，调用do_it()执行主要功能，最后调用UF_terminate()终止。
> 7. 卸载函数：提供了卸载函数ufusr_ask_unload()，返回UF_UNLOAD_IMMEDIATELY表示可以立即卸载。
>
> 综上所述，这段代码的主要功能是获取当前图纸的所有视图，设置视图的显示参数，包括隐藏线移除、边隐藏、平滑、阴影、UV填充等，并确保参数的有效性。
>
