### 【0854】create rect slot 创建矩形槽

#### 代码

```cpp
    /*HEAD CREATE_RECT_SLOT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_trns.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 新增的函数。 */  
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
    static void ask_face_dirs(double *loc, tag_t face, double *x_dir, double *z_dir)  
    {  
        double  
            parm[2],  
            pnt[3],  
            dirU2[3],  
            dirV1[3],  
            dirV2[3],  
            radii[2];  
        UF_CALL(UF_MODL_ask_face_parm(face, loc, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, x_dir, dirV1, dirU2,  
            dirV2, z_dir, radii));  
    }  
    static tag_t select_point_on_face(char *prompt, double cp[3])  
    {  
        int  
            irc;  
        tag_t  
            face,  
            line,  
            view;  
        double  
            dist;  
        UF_CURVE_line_t  
            lp;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_ANY_FACE;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &face, cp, &view));  
        if (irc == 4 || irc == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(face, FALSE));  
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
            UF_CALL(UF_MODL_ask_minimum_dist(face,line,0,cp,0,cp,&dist,cp,cp));  
            UF_CALL(UF_OBJ_delete_object(line));  
            return face;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            irc;  
        tag_t  
            face,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_ANY_FACE;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &face, cp, &view));  
        if (irc == 4 || irc == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(face, FALSE));  
            return face;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat,  
            place,  
            thru1,  
            thru2;  
        double  
            loc[3],  
            x_dir[3],  
            z_dir[3];  
        if ((place = select_point_on_face("Select face for slot", loc))  
        &&  (thru1 = select_a_face("Select first thru face"))  
        &&  (thru2 = select_a_face("Select second thru face")))  
        {  
            ask_face_dirs(loc, place, x_dir, z_dir);  
            UF_CALL(UF_MODL_create_rect_slot(loc, z_dir, x_dir, "0.5", "0.25", "",  
                place, thru1, thru2, &feat));  
            UF_CALL(UF_MODL_create_rect_slot(loc, z_dir, x_dir, "0.5", "0.25", "1",  
                place, thru1, thru2, &feat));  
        }  
    }  
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

> 这是段用于在NX中创建矩形槽的二次开发代码，主要包含以下几个部分：
>
> 1. 包含了NX二次开发所需的相关头文件。
> 2. 定义了错误报告函数report_error，用于打印出函数调用失败时的错误信息。
> 3. 定义了两个坐标转换函数map_abs2view和map_view2abs，用于将绝对坐标系下的点转换到当前视图坐标系，以及反之。
> 4. 定义了ask_face_dirs函数，用于获取面在指定点的法向量和切向量。
> 5. 定义了select_point_on_face函数，用于让用户选择一个面上的点，并返回该点所在的面。
> 6. 定义了select_a_face函数，用于让用户选择一个面。
> 7. 定义了do_it函数，用于创建矩形槽。该函数首先选择一个面上的点作为矩形槽的中心，然后选择两个贯通面作为矩形槽的开口面，最后调用UF_MODL_create_rect_slot函数创建矩形槽。
> 8. 定义了ufusr函数，这是NX二次开发程序的入口函数，初始化后调用do_it函数，最后终止。
> 9. 定义了ufusr_ask_unload函数，用于处理程序卸载时的清理工作。
>
> 整体来说，这段代码实现了让用户交互式选择一个点和一个面，然后在该点处创建一个矩形槽的功能。
>
