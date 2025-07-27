### 【2824】set wcs by face 设置坐标系为选定面的功能

#### 代码

```cpp
    /*HEAD SET_WCS_BY_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_disp.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_mtx.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中新增的功能，用于打印系统日志。因此，UF_print_syslog 是 V18 版本中新增的功能。 */  
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
    /* qq3123197280 */  
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
            n,  
            resp;  
        tag_t  
            face,  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            map_abs2view(cp);  
            lp.start_point[0] = cp[0];  
            lp.start_point[1] = cp[1];  
            lp.start_point[2] = cp[2] + 1000;  
            lp.end_point[0] = cp[0];  
            lp.end_point[1] = cp[1];  
            lp.end_point[2] = cp[2] - 1000;  
            map_view2abs(lp.start_point);  
            map_view2abs(lp.end_point);  
            UF_CALL(UF_CURVE_create_line(&lp, &line));  
            face = object;  
            UF_MODL_ask_distance_tolerance(&tol);  
            UF_CALL(UF_MODL_intersect_objects(face, line, tol, &n, &ints));  
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
    static void set_wcs_by_face_miduv(tag_t face)  
    {  
        tag_t  
            wcs,  
            mx;  
        double  
            csys[9],  
            junk[3],  
            origin[3],  
            parm[2],  
            uvs[4],  
            x_dir[3],  
            y_dir[3];  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
        parm[0] = (uvs[0] + uvs[1]) / 2;  
        parm[1] = (uvs[2] + uvs[3]) / 2;  
        UF_CALL(UF_MODL_ask_face_props(face, parm, origin, x_dir, y_dir,  
            junk, junk, junk, junk));  
        UF_CALL(UF_MTX3_initialize(x_dir, y_dir, csys));  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
    }  
    static void set_wcs_by_face_minuv(tag_t face)  
    {  
        tag_t  
            wcs,  
            mx;  
        double  
            csys[9],  
            junk[3],  
            origin[3],  
            parm[2],  
            uvs[4],  
            x_dir[3],  
            y_dir[3];  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
        parm[0] = uvs[0];  
        parm[1] = uvs[2];  
        UF_CALL(UF_MODL_ask_face_props(face, parm, origin, x_dir, y_dir,  
            junk, junk, junk, junk));  
        UF_CALL(UF_MTX3_initialize(x_dir, y_dir, csys));  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
    }  
    static void set_wcs_by_face_point(tag_t face, double origin[3])  
    {  
        tag_t  
            mx,  
            wcs;  
        double  
            csys[9],  
            junk[3],  
            parm[2],  
            pnt[3],  
            x_dir[3],  
            y_dir[3];  
        UF_CALL(UF_MODL_ask_face_parm(face, origin, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, x_dir, y_dir, junk,  
            junk, junk, junk));  
        UF_CALL(UF_MTX3_initialize(x_dir, y_dir, csys));  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        double  
            pos[3];  
        while ((face = select_point_on_face("Select face", pos)) != NULL_TAG)  
        {  
            set_wcs_by_face_point(face, pos);  
            uc1601("At selected point", TRUE);  
            set_wcs_by_face_miduv(face);  
            uc1601("At centeral u/v point", TRUE);  
            set_wcs_by_face_minuv(face);  
            uc1601("At minimum u/v point", TRUE);  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发示例程序，其主要功能是让用户选择一个面，然后以不同的方式设置WCS(工作坐标系)。下面是代码的主要功能：
>
> 1. 函数介绍：定义了错误报告函数report_error()，用于在发生UF函数调用错误时打印错误信息。还定义了视图坐标系和绝对坐标系之间的转换函数map_abs2view()和map_view2abs()。
> 2. 选择面：定义了mask_for_faces()函数，用于设置选择过滤条件，只选择实体面。然后定义了select_point_on_face()函数，用于让用户选择一个面，并返回面的标签和选择点的绝对坐标。
> 3. 设置WCS：定义了3个函数set_wcs_by_face_point()、set_wcs_by_face_miduv()和set_wcs_by_face_minuv()，分别以选择点、面的中点UV坐标和面的最小UV坐标为原点，面的切线方向为X轴和Y轴，创建临时坐标系并设置为WCS。
> 4. 主要逻辑：在do_it()函数中，循环调用select_point_on_face()选择面，然后分别以选择点、面的中点UV坐标和面的最小UV坐标为原点设置WCS。
> 5. 用户函数入口：定义了ufusr()函数，作为用户函数的入口，在UF_initialize()和UF_terminate()之间调用do_it()函数。
> 6. 卸载函数：定义了ufusr_ask_unload()函数，返回立即卸载标志。
>
> 总体来说，这段代码通过选择面并设置WCS的不同方式，展示了NX二次开发中坐标系变换、选择过滤和UF函数调用的基本方法。
>
