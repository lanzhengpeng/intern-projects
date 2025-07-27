### 【1739】place hole on datum 在基准面上打孔

#### 代码

```cpp
    /*HEAD PLACE_HOLE_ON_DATUM CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：请注意，UF_print_syslog 是在 V18 中新增的。 */  
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
    static tag_t select_point_on_datum(char *prompt, double cp[3])  
    {  
        int  
            irc;  
        tag_t  
            datum_id,  
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
        mask.object_type = UF_datum_plane_type;  
        mask.object_subtype = 0;  
        mask.solid_type = 0;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &datum_id, cp, &view));  
        if (irc == 4 || irc == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(datum_id, FALSE));  
            uc1615("Indicate hole position on datum", cp);  
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
            UF_CALL(UF_MODL_ask_minimum_dist(datum_id,line,0,cp,0,cp,&dist,cp,cp));  
            UF_CALL(UF_OBJ_delete_object(line));  
            return datum_id;  
        }  
        else return NULL_TAG;  
    }  
    static void ask_datum_dir(double *dir_vec, tag_t datum_id)  
    {  
        double  
            orig[3];  
        char  
            *off_dist,  
            *off_ang;  
        UF_CALL(UF_MODL_ask_datum_plane_parms(datum_id, orig, dir_vec, &off_dist,  
                   &off_ang));  
        UF_free(off_dist);  
        UF_free(off_ang);  
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
            dia[20] = { "0.25" },  
            dep[20] = { "1.0" },  
            tip[20] = { "0.0" };  
        double  
            loc[3],  
            z_dir[3];  
        tag_t  
            datum_feat,  
            datum_id,  
            hole;  
        char  
            ynopt[2][38] = { "YES", "NO"  };  
        int  
            choice;  
        while ((datum_id = select_point_on_datum("Select datum placement for hole",                           loc)) && (prompt_hole_parms(dia, dep, tip)))  
        {  
            UF_CALL(UF_MODL_ask_object_feat(datum_id, &datum_feat));  
            ask_datum_dir(z_dir, datum_feat );  
            UF_DISP_conehead(UF_DISP_WORK_VIEW_ONLY, loc, z_dir, 0);  
            choice = uc1603("REVERSE VECTOR DIRECTION OF HOLE ?",1,ynopt,2);  
            if(choice == 5)  
            {  
                UF_DISP_refresh();  
                UF_VEC3_negate(z_dir, z_dir);  
                UF_DISP_conehead(UF_DISP_WORK_VIEW_ONLY, loc, z_dir, 0);  
            }  
            UF_CALL(UF_MODL_create_simple_hole(loc, z_dir, dia, dep, tip,  
                                datum_id, NULL_TAG, &hole));  
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

> 这段代码是一个NX Open C++ API的示例，用于创建简单孔。主要包括以下几个部分：
>
> 1. 定义了错误报告函数report_error，用于捕捉API调用错误并打印错误信息。
> 2. 定义了视图坐标系和绝对坐标系之间的转换函数map_abs2view和map_view2abs。
> 3. 定义了选择基准面的函数select_point_on_datum，用户可以交互选择一个基准面，并在该基准面上指定一个点作为孔的位置。
> 4. 定义了获取基准面方向的函数ask_datum_dir。
> 5. 定义了提示输入孔参数的函数prompt_hole_parms。
> 6. 定义了创建简单孔的函数create_simple_holes，循环提示用户选择基准面、输入孔参数，并调用UF_MODL_create_simple_hole函数创建孔。
> 7. 定义了主函数do_it，用于调用create_simple_holes函数。
> 8. 定义了ufusr入口函数，用于初始化和终止NX，并调用do_it函数。
> 9. 定义了卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总的来说，这段代码实现了交互式创建简单孔的功能，通过提示用户选择基准面、输入孔参数，然后调用NX API创建孔。
>
