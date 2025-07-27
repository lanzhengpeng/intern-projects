### 【2695】select point on face 在面上选择点

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
    static tag_t select_point_on_face(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            face,  
            line,  
            view;  
        double  
            na;  
        UF_CURVE_line_t  
            lp;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &face, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(face, 0));  
            map_abs2view(cp, view);  
            lp.start_point[0] = cp[0];  
            lp.start_point[1] = cp[1];  
            lp.start_point[2] = cp[2] + 1000;  
            lp.end_point[0] = cp[0];  
            lp.end_point[1] = cp[1];  
            lp.end_point[2] = cp[2] - 1000;  
            map_view2abs(lp.start_point, view);  
            map_view2abs(lp.end_point, view);  
            UF_CALL(UF_CURVE_create_line(&lp, &line));  
            UF_CALL(UF_MODL_ask_minimum_dist_3(2, line, face, FALSE, NULL, FALSE,  
                NULL, &na, cp, cp, &na));  
            UF_CALL(UF_OBJ_delete_object(line));  
            return face;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现以下功能：
>
> 1. 坐标系转换函数：包含map_abs2view和map_view2abs，用于将绝对坐标系下的点转换到视图坐标系，以及将视图坐标系下的点转换到绝对坐标系。
> 2. 选择面函数：使用UF_UI库实现选择面的功能，通过设置选择掩码，指定只选择实体类型中的面。
> 3. 选择面上的点函数：函数select_point_on_face允许用户在指定的面上选择一个点，并返回这个面的标识。
> 4. 辅助功能：创建辅助线，计算点到面的最短距离，并将该距离作为选择的点。
> 5. 坐标系转换：在选择的视图下，将点坐标转换为绝对坐标系，并计算该点到面的最短距离。
> 6. 辅助线删除：完成计算后，删除辅助线。
>
> 通过这些功能，该代码实现了在指定视图下选择面上点的功能，并返回该面的标识以及该点到面的最短距离。这为后续的几何计算或分析提供了基础。
>
