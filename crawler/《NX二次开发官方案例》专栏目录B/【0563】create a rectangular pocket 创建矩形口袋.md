### 【0563】create a rectangular pocket 创建矩形口袋

#### 代码

```cpp
    /*HEAD CREATE_A_RECTANGULAR_POCKET CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_trns.h>  
    #include <uf_mtx.h>  
    #include <uf_vec.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译如下：

注意：UF_print_syslog是V18版本新增的功能，请只回答译文，不要添加其他无关内容。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            pocket,  
            target;  
        double  
            csys[9],  
            loc[3];  
        char  
            *len[3] = { "2.0", "1.0", "0.5" };  
        while ((target = select_placement_face("Select face for rect pocket",  
            loc, csys)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_create_rect_pocket(loc, &csys[6], &csys[0],  
                len, "0", "0", "0", target, &pocket));  
        }  
    }  
    /* 里海 */  
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

> 根据代码的内容，这是一段用于创建矩形口袋的NX二次开发代码。其主要功能包括：
>
> 1. 错误报告：代码定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 坐标转换：代码定义了两个坐标转换函数map_abs2view和map_view2abs，用于将绝对坐标系下的点转换到当前视图坐标系下，以及将视图坐标系下的点转换回绝对坐标系下。
> 3. 选择面：代码定义了一个函数mask_for_planar_face_datum，用于设置选择过滤条件，仅允许选择平面特征面或基准面。另外，代码还定义了一个函数select_placement_face，用于提示用户选择放置矩形口袋的面，并获取面的位置和坐标系信息。
> 4. 创建矩形口袋：代码定义了一个函数do_it，用于循环提示用户选择面，并调用NX的API函数UF_MODL_create_rect_pocket来创建矩形口袋。
> 5. 入口函数：代码定义了ufusr函数作为程序的入口，用于初始化NX环境，调用do_it函数执行创建矩形口袋操作，并在结束时关闭NX环境。
> 6. 卸载函数：代码定义了ufusr_ask_unload函数，用于在程序卸载时立即清理资源。
>
> 综上所述，这段代码实现了选择平面面创建矩形口袋的功能，通过二次开发扩展了NX软件的功能。
>
