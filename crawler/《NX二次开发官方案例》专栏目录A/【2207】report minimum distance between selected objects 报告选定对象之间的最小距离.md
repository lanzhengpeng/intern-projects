### 【2207】report minimum distance between selected objects 报告选定对象之间的最小距离

#### 代码

```cpp
    /*HEAD REPORT_MINIMUM_DISTANCE_BETWEEN_SELECTED_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新功能，用于打印系统日志信息。 */  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_any_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_add_faces_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
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
            subtype,  
            type;  
        tag_t  
            object1,  
            object2;  
        double  
            minimum_distance,  
            loc[3],  
            pt1[3],  
            pt2[3];  
        while (((object1 = select_any_object("First object")) != NULL_TAG) &&  
               ((object2 = select_any_object("Second object")) != NULL_TAG))  
        {  
            WRITE("\nUsing two objects:\n");  
            if (!UF_CALL(UF_MODL_ask_minimum_dist(object1, object2, FALSE, NULL,  
                FALSE, NULL, &minimum_distance, pt1, pt2)))  
            {  
                WRITE3F(pt1);  
                display_temporary_point(pt1);  
                WRITE3F(pt2);  
                display_temporary_point(pt2);  
                display_temporary_line(pt1, pt2);  
                WRITE_F(minimum_distance);  
            }  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object1, &type, &subtype));  
            if (type == UF_point_type)  
            {  
                WRITE("\nUsing coordinates of point 1:\n");  
                UF_CALL(UF_CURVE_ask_point_data(object1, loc));  
                if (!UF_CALL(UF_MODL_ask_minimum_dist(NULL_TAG, object2, TRUE, loc,  
                    FALSE, NULL, &minimum_distance, pt1, pt2)))  
                {  
                    WRITE3F(pt1);  
                    display_temporary_point(pt1);  
                    WRITE3F(pt2);  
                    display_temporary_point(pt2);  
                    display_temporary_line(pt1, pt2);  
                    WRITE_F(minimum_distance);  
                }  
            }  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object2, &type, &subtype));  
            if (type == UF_point_type)  
            {  
                WRITE("\nUsing coordinates of point 2:\n");  
                UF_CALL(UF_CURVE_ask_point_data(object2, loc));  
                if (!UF_CALL(UF_MODL_ask_minimum_dist(object1, NULL_TAG, FALSE,  
                    NULL, TRUE, loc, &minimum_distance, pt1, pt2)))  
                {  
                    WRITE3F(pt1);  
                    display_temporary_point(pt1);  
                    WRITE3F(pt2);  
                    display_temporary_point(pt2);  
                    display_temporary_line(pt1, pt2);  
                    WRITE_F(minimum_distance);  
                }  
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

> 根据代码的内容，这是一个用于计算NX中两个对象之间最小距离的二次开发程序。
>
> 主要功能包括：
>
> 1. 使用UF_MODL_ask_minimum_dist函数计算两个对象之间的最小距离，并获取距离最近的两个点。
> 2. 根据两个对象是否为点对象，分别使用UF_CURVE_ask_point_data获取点的坐标，然后计算点到另一个对象的最小距离。
> 3. 在图形区高亮显示计算得到的最近两点，并显示最小距离值。
> 4. 通过UF_UI_set_sel_mask设置选择过滤条件，只允许选择面和边。
> 5. 使用UF_UI_select_with_single_dialog实现交互式选择对象。
> 6. 使用UF_DISP_display_temporary_point和UF_DISP_display_temporary_line在图形区显示临时点和高亮线。
> 7. 输出计算结果到日志窗口。
> 8. 提供了ufusr和ufusr_ask_unload两个标准函数接口。
>
> 该程序实现了NX中对象之间距离测量的功能，使用户可以方便地选择对象，计算并查看最小距离。
>
