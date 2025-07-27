### 【2684】select placement face 选择放置面

#### 代码

```cpp
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs_mx, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs_mx, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    /* 里海 */  
    static int mask_for_planar_face_datum(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_placement_face(char *prompt, double *loc, double *csys)  
    {  
        int  
            subtype,  
            type,  
            resp;  
        tag_t  
            face,  
            feat,  
            line,  
            view;  
        UF_CURVE_line_t  
            lp;  
        double  
            dirU2[3],  
            dirV2[3],  
            dist,  
            parm[2],  
            pnt[3],  
            radii[2],  
            z[3];  
        char  
            *angle,  
            *offset;  
        UF_CALL(UF_UI_select_with_single_dialog("Select placement face", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_planar_face_datum, NULL, &resp,  
            &face, loc, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(face, FALSE));  
            map_abs2view(loc);  
            lp.start_point[0] = loc[0];  
            lp.start_point[1] = loc[1];  
            lp.start_point[2] = loc[2] + 1000;  
            lp.end_point[0] = loc[0];  
            lp.end_point[1] = loc[1];  
            lp.end_point[2] = loc[2] - 1000;  
            map_view2abs(lp.start_point);  
            map_view2abs(lp.end_point);  
            UF_CALL(UF_CURVE_create_line(&lp, &line));  
            UF_CALL(UF_MODL_ask_minimum_dist(face,line,0,loc,0,loc,&dist,loc,loc));  
            UF_CALL(UF_OBJ_delete_object(line));  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            if (type == UF_solid_type)  
            {  
                UF_CALL(UF_MODL_ask_face_parm(face, loc, parm, pnt));  
                UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, &csys[0], &csys[3],  
                    dirU2, dirV2, &csys[6], radii));  
            }  
            else  
            {  
                UF_CALL(UF_MODL_ask_object_feat(face, &feat));  
                UF_CALL(UF_MODL_ask_datum_plane_parms(feat,loc,z,&offset,&angle));  
                UF_CALL(UF_MTX3_initialize_z(z, csys));  
                UF_free(angle);  
                UF_free(offset);  
            }  
            UF_VEC3_negate(&csys[6], &csys[6]);  
            return face;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码主要用于NX的二次开发，主要实现了以下功能：
>
> 1. 坐标系变换：通过map_abs2view和map_view2abs函数，实现了绝对坐标系和视图坐标系之间的转换。这为后续操作提供了坐标系上的便利。
> 2. 面选择：select_placement_face函数实现了在选择平面或基准面时，获取面的相关信息，如位置、坐标系等，并返回选择的面的tag。
> 3. 选择器设置：mask_for_planar_face_datum函数用于设置选择器，仅允许选择实体面或基准面。
> 4. 交互选择：通过UF_UI_select_with_single_dialog函数实现了交互式选择平面或基准面，并将选择结果存储在face变量中。
> 5. 信息获取：根据选择的面类型，使用不同的函数获取面的参数、位置、坐标系等信息，并将其存储在相关变量中。
> 6. 坐标系处理：获取坐标系信息后，进行了一些处理，如调整坐标系方向等，最后返回选择的面的tag。
>
> 总的来说，这段代码实现了在NX中交互选择平面或基准面，并获取该面相关信息的二次开发功能，为后续操作提供了基础。
>
