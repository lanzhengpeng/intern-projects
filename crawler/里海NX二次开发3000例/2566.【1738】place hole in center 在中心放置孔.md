### 【1738】place hole in center 在中心放置孔

#### 代码

```cpp
    /*HEAD PLACE_HOLE_IN_CENTER CCC UFUN */  
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
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意：UF_print_syslog是在V18版本中新增的，只回答译文，不要废话。 */  
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
    static void get_face_dirs(double loc[3], tag_t face, double xyz[9])  
    {  
        double  
            parm[2],  
            pnt[3],  
            dirU2[3],  
            dirV2[3],  
            radii[2];  
        UF_CALL(UF_MODL_ask_face_parm(face, loc, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, &xyz[0], &xyz[3], dirU2,  
            dirV2, &xyz[6], radii));  
    }  
    static tag_t select_a_linear_edge(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            irc;  
        tag_t  
            edge,  
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
        mask.solid_type = UF_UI_SEL_FEATURE_LINEAR_EDGE;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &edge, cp, &view));  
        if (irc == 4 || irc == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(edge, FALSE));  
            return edge;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t select_a_circular_edge(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            irc;  
        tag_t  
            edge,  
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
        mask.solid_type = UF_UI_SEL_FEATURE_CIRCULAR_EDGE;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &edge, cp, &view));  
        if (irc == 4 || irc == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(edge, FALSE));  
            return edge;  
        }  
        else return NULL_TAG;  
    }  
    static int rpo_hole(tag_t feat)  
    {  
        tag_t  
            target[2],  
            tool[2];  
        int  
            target_qual[2] = { UF_MODL_ARC_CENTER, UF_MODL_ARC_CENTER },  
            tool_qual[2] = { UF_MODL_ARC_CENTER, UF_MODL_ARC_CENTER };  
        char  
            *constraints_array[2] = { "PARA_DIST_PARMS",  
                                       "PARA_DIST_PARMS" },  
            *constraints_value[2] = { "0.0", "0.0" };  
        if ((tool[0] = select_a_circular_edge("Select tool edge")) &&  
            (target[0] = select_a_circular_edge("Select target edge ")))  
        {  
            tool[1] = tool[0];  
            target[1] = target[0];  
            return(UF_MODL_create_rpo_constraints(feat, NULL_TAG, NULL_TAG,  
                target, target_qual, tool, tool_qual, constraints_value,  
                constraints_array, 2));  
        }  
        else return (0);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face,  
            hole;  
        double  
            dir[3],  
            loc[3],  
            dest_csys[9];  
        UF_CALL(UF_MODL_register_rpo_routine(rpo_hole));  
        while ((face=select_point_on_face("Select face for hole", loc)) != NULL_TAG)  
        {  
            get_face_dirs(loc, face, dest_csys);  
            UF_VEC3_negate(&dest_csys[6], dir);  
            UF_CALL(UF_MODL_create_c_sunk_hole(loc, dir, "0.25", "1.5", "0.5",  
                "82", "118", face, NULL_TAG, &hole));  
        }  
        UF_CALL(UF_MODL_unregister_rpo_routine());  
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
    /*  Note:  can NOT be unloaded - calls UF_MODL_register_rpo_routine.  
        return (UF_UNLOAD_IMMEDIATELY);  
    */  
        return (UF_UNLOAD_UG_TERMINATE);  
    }

```

#### 代码解析

> 根据代码，这是一段NX的二次开发代码，实现了以下功能：
>
> 1. 用户可以通过选择一个面来创建一个沉头孔。在选择面上，程序会自动确定孔的放置位置和方向。
> 2. 用户需要选择工具圆弧边和目标圆弧边，程序会根据这两个边生成约束，以确保孔的参数化驱动。
> 3. 使用了UF_MODL_register_rpo_routine函数来注册自定义约束求解函数rpo_hole。在创建沉头孔特征时，会调用这个函数来应用约束。
> 4. 使用了UF_MODL_create_c_sunk_hole函数来创建沉头孔特征。这个函数的参数包括孔的位置、方向、直径、深度、锥角、顶锥角等信息。
> 5. 为了方便用户选择，代码提供了选择面、线性边、圆弧边等选择函数。
> 6. 为了处理坐标转换，代码提供了视图坐标系到绝对坐标系的转换函数。
> 7. 通过错误处理函数report_error来报告函数调用错误。
> 8. 使用了ufusr函数作为程序的入口点，在NX启动时自动执行。
> 9. ufusr_ask_unload函数返回UF_UNLOAD_UG_TERMINATE，表示该库在NX退出时卸载。
>
> 总体来说，这段代码实现了一个基于NX的沉头孔自动创建功能，用户可以通过简单的选择操作来生成沉头孔特征，并且实现了参数化驱动。
>
