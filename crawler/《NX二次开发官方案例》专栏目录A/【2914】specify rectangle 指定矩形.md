### 【2914】specify rectangle 指定矩形

#### 代码

```cpp
    struct rectangle_data_s {  
        double  
            origin[3],  
            x_axis[3],  
            y_axis[3],  
            opposite[3];  
    };  
    typedef struct rectangle_data_s rectangle_data_t, *rectangle_data_p_t;  
    static void rubberband_rectangle_cb(double *screen_pos,  
        UF_UI_motion_cb_data_p_t motion_cb_data, void *client_data)  
    {  
        double  
            dx[3],  
            dy[3],  
            end[3],  
            pos[5][3],  
            start[3];  
        rectangle_data_p_t  
            my_data = (rectangle_data_p_t) client_data;  
        UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, my_data->origin,  
            UF_CSYS_ROOT_WCS_COORDS, start);  
        UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, screen_pos,  
            UF_CSYS_ROOT_WCS_COORDS, end);  
        UF_VEC3_scale( end[0] - start[0], my_data->x_axis, dx );  
        UF_VEC3_scale( end[1] - start[1], my_data->y_axis, dy );  
        UF_VEC3_copy( my_data->origin, pos[0] );  
        UF_VEC3_add( pos[0], dx, pos[1] );  
        UF_VEC3_add( pos[1], dy, pos[2] );  
        UF_VEC3_sub( pos[2], dx, pos[3] );  
        UF_VEC3_sub( pos[3], dy, pos[4] );  
        UF_CALL(UF_DISP_display_ogp_polyline(motion_cb_data->view_tag, pos, 5));  
    }  
    static logical specify_rectangle(char *prompt, rectangle_data_p_t the_data)  
    {  
        int  
            resp;  
        tag_t  
            view;  
        char  
            msg[133];  
        sprintf(msg, "%s corner 1", prompt);  
        UF_CALL(UF_UI_specify_screen_position(msg, NULL, NULL, the_data->origin,  
            &view, &resp));  
        if (resp != UF_UI_PICK_RESPONSE) return FALSE;  
        ask_wcs_x_and_y_axes(the_data->x_axis, the_data->y_axis);  
        sprintf(msg, "%s corner 2", prompt);  
        UF_CALL(UF_UI_specify_screen_position(msg, rubberband_rectangle_cb,   
            (void *)the_data, the_data->opposite, &view, &resp));  
        if (resp != UF_UI_PICK_RESPONSE) return FALSE;  
        return TRUE;  
    }

```

#### 代码解析

> 这段代码是用于在NX中绘制矩形框的二次开发代码，主要实现如下功能：
>
> 1. 定义了一个结构体rectangle_data_s，用于存储矩形框的起始点、x轴方向、y轴方向和对面点。
> 2. rubberband_rectangle_cb函数用于在拖动时绘制临时矩形框。它根据起始点和当前点计算矩形的顶点，并使用UF_DISP_display_ogp_polyline函数绘制出矩形框。
> 3. specify_rectangle函数用于提示用户输入矩形框的两个对角点。首先提示输入第一个点，然后提示输入第二个点，同时调用rubberband_rectangle_cb函数绘制临时矩形框。如果用户成功选择两个点，则返回TRUE。
> 4. 代码使用NX的UI、坐标系转换和向量运算等二次开发接口，实现了交互式绘制矩形框的功能。
> 5. 通过定义结构体和回调函数，代码实现了矩形框数据的封装和绘制逻辑的分离，提高了代码的复用性和可维护性。
>
> 综上所述，这段代码实现了在NX中通过用户交互绘制矩形框的功能，是NX二次开发中的一个典型应用示例。
>
