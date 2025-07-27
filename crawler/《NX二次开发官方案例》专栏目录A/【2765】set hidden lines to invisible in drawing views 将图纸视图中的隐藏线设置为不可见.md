### 【2765】set hidden lines to invisible in drawing views 将图纸视图中的隐藏线设置为不可见

#### 代码

```cpp
    /*HEAD SET_HIDDEN_LINES_TO_INVISIBLE_IN_DRAWING_VIEWS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，我理解如下：

在V18版本中，新增了一个函数UF_print_syslog，用于打印系统日志信息。

请只回答翻译内容，避免添加其他无关内容。 */  
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
                view_parms.hidden_line = UF_DRAW_hidden_line_removal_on;  
                view_parms.hidden_line_font = UF_OBJ_FONT_INVISIBLE;  
                /* options are:   
                    UF_OBJ_FONT_INVISIBLE       
                    UF_OBJ_FONT_SOLID           
                    UF_OBJ_FONT_DASHED          
                    UF_OBJ_FONT_PHANTOM         
                    UF_OBJ_FONT_CENTERLINE      
                    UF_OBJ_FONT_DOTTED          
                    UF_OBJ_FONT_LONG_DASHED     
                    UF_OBJ_FONT_DOTTED_DASHED  
                */  
                UF_CALL(UF_DRAW_set_view_display(views[ii], &view_parms));  
                UF_CALL(UF_DRAW_update_one_view(drawing, views[ii]));  
            }  
            if (n_views > 0) UF_free(views);  
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

> 这段代码是NX的二次开发代码，其主要功能是设置当前图纸中的所有视图的隐藏线为不可见。
>
> 代码流程如下：
>
> 1. 初始化NX环境。
> 2. 获取当前图纸对象，然后获取图纸中的所有视图。
> 3. 遍历每个视图，获取视图显示参数，然后进行参数修正，确保参数合法。
> 4. 将隐藏线设置为不可见(UF_OBJ_FONT_INVISIBLE)，然后更新视图显示参数。
> 5. 更新视图，使设置生效。
> 6. 结束NX环境。
>
> 该代码实现了隐藏线的不可见设置，并且通过修正参数、更新视图等步骤确保了视图的显示效果。
>
