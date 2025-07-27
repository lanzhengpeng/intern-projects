### 【2906】specify circle 指定圆

#### 代码

```cpp
    static void map_arc_center_from_abs(UF_CURVE_arc_t *adata)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12];  
        UF_CALL(UF_CSYS_ask_matrix_values(adata->matrix_tag, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        FTN(uf5941)(adata->arc_center, mx);  
    }  
    static void map_point_to_matrix(tag_t matrix, double *pos)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12];  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }  
    /*  This motion callback emulates the rubberbanding circle seen when  
        using the interactive option Insert->Curve->Basic Curves...->Circle  */  
    static void rubberband_circle_cb(double *screen_pos,  
        UF_UI_motion_cb_data_p_t motion_cb_data, void *client_data)  
    {  
        double  
            axes[9];  
        UF_CURVE_arc_p_t  
            my_data = (UF_CURVE_arc_p_t) client_data;  
        map_point_to_matrix(my_data->matrix_tag, screen_pos);  
        UF_VEC3_distance(my_data->arc_center, screen_pos, &my_data->radius);  
        if (my_data->radius > 0)  
        {  
            UF_CALL(UF_CSYS_ask_matrix_values(my_data->matrix_tag, axes));  
            UF_CALL(UF_DISP_display_ogp_circle(motion_cb_data->view_tag,  
                axes, my_data->arc_center, my_data->radius));  
        }  
    }  
    static logical specify_circle(char *prompt, UF_CURVE_arc_p_t the_data)  
    {  
        int  
            resp;  
        tag_t  
            view;  
        char  
            msg[133];  
        double  
            on_arc[3];  
        sprintf(msg, "%s center", prompt);  
        UF_CALL(UF_UI_specify_screen_position(msg, NULL, NULL,  
            the_data->arc_center, &view, &resp));  
        if (resp != UF_UI_PICK_RESPONSE) return FALSE;  
        the_data->matrix_tag = ask_wcs_matrix_tag();  
        map_arc_center_from_abs(the_data);  
        sprintf(msg, "%s point on arc", prompt);  
        UF_CALL(UF_UI_specify_screen_position(msg, rubberband_circle_cb,  
            (void *)the_data, on_arc, &view, &resp));  
        if (resp != UF_UI_PICK_RESPONSE) return FALSE;  
        map_point_to_matrix(the_data->matrix_tag, on_arc);  
        UF_VEC3_distance(the_data->arc_center, on_arc, &the_data->radius);  
        return TRUE;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了在NX中绘制圆形的功能。以下是代码的主要功能：
>
> 1. map_arc_center_from_abs函数：该函数将圆心从绝对坐标系转换到当前坐标系。
> 2. map_point_to_matrix函数：该函数将一个点从屏幕坐标系转换到当前坐标系。
> 3. rubberband_circle_cb函数：该函数是运动回调，用于在用户移动鼠标时动态显示圆形。它根据圆心和半径实时更新圆形的显示。
> 4. specify_circle函数：该函数让用户指定圆心位置，并动态显示圆形以让用户指定圆弧上的点来定义半径。最后，它根据圆心和半径创建一个圆形。
> 5. 主要流程：specify_circle函数首先让用户指定圆心，然后通过运动回调动态显示圆形，让用户指定圆弧上的点来定义半径，最后根据圆心和半径创建圆形。
>
> 总体来说，这段代码实现了在NX中交互式绘制圆形的功能，通过运动回调实现了类似橡皮筋的动态显示效果，使得用户可以直观地定义圆形。
>
