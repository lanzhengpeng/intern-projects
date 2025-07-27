### 【2694】select point on face in work part 在零件上选择面上的点

#### 代码

```cpp
    static void map_abs2view(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    /* 里海 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_point_on_face_in_work_part(char *prompt, double cp[3])  
    {  
        int  
            n,  
            resp;  
        tag_t  
            line,  
            object,  
            view;  
        double  
            tol;  
        UF_CURVE_line_t  
            lp;  
        UF_MODL_intersect_info_p_t  
            *ints;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, cp, UF_CSYS_ROOT_COORDS,  
                cp));  
            map_abs2view(cp, view);  
            lp.start_point[0] = cp[0];  
            lp.start_point[1] = cp[1];  
            lp.start_point[2] = cp[2] + 1000;  
            lp.end_point[0] = cp[0];  
            lp.end_point[1] = cp[1];  
            lp.end_point[2] = cp[2] - 1000;  
            map_view2abs(lp.start_point, view);  
            map_view2abs(lp.end_point, view);  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, lp.start_point,  
                UF_CSYS_WORK_COORDS, lp.start_point));  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, lp.end_point,  
                UF_CSYS_WORK_COORDS, lp.end_point));  
            UF_CALL(UF_CURVE_create_line(&lp, &line));  
            UF_MODL_ask_distance_tolerance(&tol);  
            UF_CALL(UF_MODL_intersect_objects(object, line, tol, &n, &ints));  
            if (n > 0)  
            {  
                UF_VEC3_copy(ints[0]->intersect.point.coords, cp);  
                UF_free(ints);  
            }  
            UF_CALL(UF_OBJ_delete_object(line));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码实现了在NX中通过用户界面选择一个面，然后在选择的面上创建一个点到绝对坐标系转换的函数。具体来说，它实现了以下功能：
>
> 1. map_abs2view和map_view2abs函数：实现了绝对坐标系和工作坐标系之间的转换。
> 2. mask_for_faces函数：用于设置选择集，仅允许选择实体面。
> 3. select_point_on_face_in_work_part函数：通过用户界面提示用户选择一个面，并返回该面的tag。同时，该函数在选择的面上随机创建一个点到绝对坐标系的转换。
> 4. select_point_on_face_in_work_part函数内部流程：首先提示用户选择一个面，然后获取选择的面，在面上随机创建一个点到绝对坐标系的转换，并返回该点的绝对坐标。
> 5. 该函数通过创建一条穿过选择面的直线，并求直线与面的交点来获取面上的一个点，实现了在面上选择点的功能。
> 6. 使用绝对坐标系和工作坐标系之间的转换，确保了选择点的坐标系与NX的坐标系一致。
> 7. 该函数可用于在NX的二次开发中，获取用户选择的面上一个点的绝对坐标。
>
> 总的来说，这段代码通过用户界面交互，实现了在NX中选择面并获取该面上一个点的绝对坐标的功能，为NX的二次开发提供了便利。
>
