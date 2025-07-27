### 【2080】report end points of selected fem edge 获取选中FEM边界的端点信息

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_sf.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_csys.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* qq3123197280 */  
    static int mask_for_polygon_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_caegeom_type, UF_caegeom_edge_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_polygon_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a polygon edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_polygon_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            p_edge;  
        double  
            start_pt[3],  
            end_pt[3],  
            start_tangent[3],  
            end_tangent[3];  
        while ((p_edge = select_a_polygon_edge("Report end locations")) !=  
            NULL_TAG)  
        {  
            UF_CALL(UF_SF_edge_ask_end_points(p_edge, start_pt, end_pt,  
                start_tangent, end_tangent));  
            ECHO("Polygon edge (WRONG - See PR 2159767):\n");  
            WRITE3F(start_pt);  
            WRITE3F(end_pt);  
            UF_CALL(UF_ASSEM_set_work_occurrence(  
                UF_ASSEM_ask_part_occurrence(p_edge)));  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, start_pt,  
                UF_CSYS_ROOT_COORDS, start_pt));  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, end_pt,  
                UF_CSYS_ROOT_COORDS, end_pt));  
            ECHO("\nPolygon edge (Corrected):\n");  
            WRITE3F(start_pt);  
            WRITE3F(end_pt);  
            UF_CALL(UF_ASSEM_set_work_part(part));  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发程序，主要实现了以下功能：
>
> 1. 定义了一个宏UF_CALL，用于调用NX Open API函数，并捕获函数调用错误，打印错误信息。
> 2. 定义了一个宏WRITE3F，用于打印三维坐标点。
> 3. 定义了一个函数mask_for_polygon_edges，用于设置选择过滤器，只选择多边形的边。
> 4. 定义了一个函数select_a_polygon_edge，用于通过对话框选择一个多边形的边，并返回边的tag。
> 5. 定义了一个函数do_it，在该函数中：获取当前显示的部件循环调用select_a_polygon_edge选择多边形的边获取边的起点和终点坐标打印边的原始坐标切换到边的所在部件，将边的坐标转换到根坐标系打印转换后的坐标切换回原始显示的部件
> 6. 获取当前显示的部件
> 7. 循环调用select_a_polygon_edge选择多边形的边
> 8. 获取边的起点和终点坐标
> 9. 打印边的原始坐标
> 10. 切换到边的所在部件，将边的坐标转换到根坐标系
> 11. 打印转换后的坐标
> 12. 切换回原始显示的部件
> 13. 在ufusr函数中：初始化NX Open API调用do_it函数执行主体功能终止NX Open API
> 14. 初始化NX Open API
> 15. 调用do_it函数执行主体功能
> 16. 终止NX Open API
> 17. 定义了一个函数ufusr_ask_unload，在卸载用户程序时被调用。
>
> 总体来说，这个程序实现了在NX中通过对话框选择多边形的边，并打印边的起点和终点坐标的功能。同时，程序处理了坐标系转换的问题。
>
