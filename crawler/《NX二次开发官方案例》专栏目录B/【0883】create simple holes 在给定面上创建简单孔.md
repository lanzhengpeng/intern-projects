### 【0883】create simple holes 在给定面上创建简单孔

#### 代码

```cpp
    /*HEAD CREATE_SIMPLE_HOLES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_vec.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_mtx.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数，用于打印系统日志。 */  
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
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
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
    static tag_t select_point_on_planar_face_or_datum(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_planar_face_datum, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static tag_t select_placement_face(char *prompt, double *loc, double *csys)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            face,  
            feat,  
            line;  
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
        if ((face = select_point_on_planar_face_or_datum(prompt, loc)) != NULL_TAG)  
        {  
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
    static int prompt_hole_parms( char *dia, char *dep, char *tip )  
    {  
        char  
            menu[3][16] = { "Diameter", "Depth", "Tip Angle" };  
        double  
            values[3];  
        int  
            irc,  
            junk;  
        sscanf(dia, "%lf", &values[0]);  
        sscanf(dep, "%lf", &values[1]);  
        sscanf(tip, "%lf", &values[2]);  
        irc = uc1609("Enter hole parameters", menu, 3, values, &junk);  
        if (irc == 3 || irc == 4)  
        {  
            sprintf(dia, "%f", values[0]);  
            sprintf(dep, "%f", values[1]);  
            sprintf(tip, "%f", values[2]);  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void create_simple_holes(void)  
    {  
        char  
            dia[20] = { "1.0" },  
            dep[20] = { "1.0" },  
            tip[20] = { "0.0" };  
        double  
            loc[3],  
            csys[9];  
        tag_t  
            face,  
            hole;  
        while ((face = select_placement_face("Select face for hole", loc, csys)) &&  
               (prompt_hole_parms(dia, dep, tip)))  
        {  
            UF_CALL(UF_MODL_create_simple_hole(loc, &csys[6], dia, dep, tip,  
                face, NULL_TAG, &hole));  
        }  
    }  
    static void do_it(void)  
    {  
        create_simple_holes();  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++ UFUN，用于创建简单的孔特征。
>
> 主要功能包括：
>
> 1. 包含了NX Open C++的标准头文件。
> 2. 定义了一个错误报告函数report_error，用于打印错误信息。
> 3. 定义了两个函数map_abs2view和map_view2abs，用于坐标系之间的转换。
> 4. 定义了一个函数mask_for_planar_face_datum，用于创建选择面或基准面的选择掩码。
> 5. 定义了一个函数select_point_on_planar_face_or_datum，用于选择平面或基准面。
> 6. 定义了一个函数select_placement_face，用于选择放置孔的面，并获取面的坐标系。
> 7. 定义了一个函数prompt_hole_parms，用于提示用户输入孔的参数。
> 8. 定义了一个函数create_simple_holes，用于创建孔特征。
> 9. 定义了一个函数do_it，用于执行创建孔特征。
> 10. 定义了UFUN入口函数ufusr，用于初始化NX系统，执行功能，然后终止NX系统。
> 11. 定义了一个函数ufusr_ask_unload，用于卸载UFUN。
>
> 总的来说，这段代码通过UFUN的方式实现了在NX中创建简单孔特征的功能。
>
