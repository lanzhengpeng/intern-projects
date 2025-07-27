### 【1992】report arc info 报告弧信息

#### 代码

```cpp
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
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
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void report_arc_info(tag_t arc)  
    {  
        tag_t  
            display_part = UF_PART_ask_display_part(),  
            work_part = UF_ASSEM_ask_work_part();  
        double  
            abs_offset,  
            abs_x[3] = { 1, 0, 0 },  
            abs_z[3] = { 0, 0, 1 },  
            arc_length,  
            abs_end_angle,  
            end_disp_abs[3],  
            end_disp_wcs[3],  
            end_work_abs[3],  
            center_disp_wcs[3],  
            center_work_abs[3],  
            delta_angle,  
            diameter,  
            abs_start_angle,  
            start_disp_abs[3],  
            start_disp_wcs[3],  
            start_work_abs[3];  
        UF_EVAL_p_t  
            evaluator;  
        UF_EVAL_arc_t  
            e_arc;  
        UF_CALL(UF_EVAL_initialize(arc, &evaluator));  
        UF_CALL(UF_EVAL_ask_arc(evaluator, &e_arc));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, e_arc.limits[0], start_disp_abs,  
            NULL));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, e_arc.limits[1], end_disp_abs,  
            NULL));  
        UF_CALL(UF_EVAL_free(evaluator));  
        WRITE_F(e_arc.limits[0] * RADEG);  
        WRITE_F(e_arc.limits[1] * RADEG);  
        UF_VEC3_angle_between(abs_x, e_arc.x_axis, abs_z, &abs_offset);  
        abs_start_angle = e_arc.limits[0] - abs_offset;  
        if (abs_start_angle < 0) abs_start_angle = abs_start_angle + (2 * PI);  
        WRITE_F(abs_start_angle * RADEG);  
        abs_end_angle = e_arc.limits[1] - abs_offset;  
        if (abs_end_angle < 0) abs_end_angle = abs_end_angle + (2 * PI);  
        WRITE_F(abs_end_angle * RADEG);  
        UF_DISP_labeled_conehead( UF_DISP_ALL_ACTIVE_VIEWS, e_arc.center,  
            e_arc.x_axis, 0, "X");  
        UF_DISP_labeled_conehead( UF_DISP_ALL_ACTIVE_VIEWS, e_arc.center,  
            e_arc.y_axis, 0, "Y");  
        display_temporary_point(e_arc.center);  
        WRITE_F(e_arc.radius);  
        WRITE_F(diameter = e_arc.radius * 2);  
        WRITE_F(arc_length = (e_arc.limits[1]-e_arc.limits[0])*e_arc.radius);  
        WRITE_F(delta_angle = (e_arc.limits[1]-e_arc.limits[0])*RADEG);  
        ECHO("\n");  
        WRITE3F(e_arc.center);  
        WRITE3F(start_disp_abs);  
        WRITE3F(end_disp_abs);  
        ECHO("\n");  
        UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, e_arc.center,  
            UF_CSYS_ROOT_WCS_COORDS, center_disp_wcs));  
        UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, start_disp_abs,  
            UF_CSYS_ROOT_WCS_COORDS, start_disp_wcs));  
        UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, end_disp_abs,  
            UF_CSYS_ROOT_WCS_COORDS, end_disp_wcs));  
        WRITE3F(center_disp_wcs);  
        WRITE3F(start_disp_wcs);  
        WRITE3F(end_disp_wcs);  
        ECHO("\n");  
        if (work_part != display_part)  
        {  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, e_arc.center,  
                UF_CSYS_WORK_COORDS, center_work_abs));  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, start_disp_abs,  
                UF_CSYS_WORK_COORDS, start_work_abs));  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, end_disp_abs,  
                UF_CSYS_WORK_COORDS, end_work_abs));  
            WRITE3F(center_work_abs);  
            WRITE3F(start_work_abs);  
            WRITE3F(end_work_abs);  
            ECHO("\n");  
        }  
    }

```

#### 代码解析

> 这段NX Open API代码实现了以下功能：
>
> 1. 定义了两个宏WRITE_F和WRITE3F，用于打印浮点数和浮点数数组。
> 2. 定义了函数write_double_array_to_listing_window，用于打印浮点数数组到列表窗口。
> 3. 定义了函数display_temporary_point，用于显示临时点。
> 4. 定义了函数report_arc_info，用于报告弧的相关信息，包括：弧的起点和终点角度弧心坐标弧的半径和直径弧长弧的起点、终点和弧心在世界坐标系和工作坐标系中的坐标
> 5. 弧的起点和终点角度
> 6. 弧心坐标
> 7. 弧的半径和直径
> 8. 弧长
> 9. 弧的起点、终点和弧心在世界坐标系和工作坐标系中的坐标
> 10. 使用UF_EVAL API获取弧的参数信息。
> 11. 使用UF_DISP API显示临时点、标注圆锥头等。
> 12. 使用UF_CSYS API进行坐标转换。
> 13. 判断工作部件和显示部件是否相同，如果是不同部件，则还需要打印弧的参数在工作坐标系中的坐标。
>
> 总的来说，这段代码通过调用NX Open API获取弧的信息，并进行坐标转换，然后打印到列表窗口，实现了弧信息的查询功能。
>
