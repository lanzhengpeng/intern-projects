### 【2997】trim curve to length 将曲线截断到指定长度

#### 代码

```cpp
    /*HEAD TRIM_CURVE_TO_LENGTH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，翻译内容为：UF_print_syslog是V18版本新增的函数。 */  
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
    static void ask_pos_on_obj(tag_t obj, double loc[3])  
    {  
        tag_t  
            line;  
        double  
            dist;  
        UF_CURVE_line_t  
            lp;  
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
        UF_CALL(UF_MODL_ask_minimum_dist(obj,line,0,loc,0,loc,&dist,loc,loc));  
        UF_CALL(UF_OBJ_delete_object(line));  
    }  
    static tag_t select_point_on_curve(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            curve,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask[4];  
        opts.num_mask_triples = 4;  
        opts.mask_triples = mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask[0].object_type = UF_line_type;  
        mask[0].object_subtype = 0;  
        mask[0].solid_type = 0;  
        mask[1].object_type = UF_circle_type;  
        mask[1].object_subtype = 0;  
        mask[1].solid_type = 0;  
        mask[2].object_type = UF_conic_type;  
        mask[2].object_subtype = UF_all_subtype;  
        mask[2].solid_type = 0;  
        mask[3].object_type = UF_spline_type;  
        mask[3].object_subtype = 0;  
        mask[3].solid_type = 0;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &resp, &curve, cp, &view));  
        if (resp == 4 || resp == 5)  
        {  
            ask_pos_on_obj(curve, cp);  
            UF_CALL(UF_DISP_set_highlight(curve, 0));  
            return curve;  
        }  
        else return NULL_TAG;  
    }  
    static logical ask_real(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            irc,  
            total = 2,  
            use_point = 3;  
        tag_t  
            curve;  
        double  
            length,  
            pos[3];  
        while ((curve = select_point_on_curve("Select curve end to trim", pos))  
            != NULL_TAG)  
        {  
            UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, 1.0, UF_MODL_UNITS_PART,  
                &length));  
            if (ask_real("Enter new curve length", "length", &length))  
            {  
                FTN(uf5903)(&curve, &total, &length, &use_point, pos, &irc);  
                UF_CALL(irc);  
            }  
        }  
    }  
    /* qq3123197280 */  
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

> 这段代码是NX的二次开发代码，其主要功能是对曲线进行裁剪，以调整曲线的长度。以下是代码的主要功能和流程：
>
> 1. 定义了错误报告函数report_error，用于打印出调用UF函数时的错误信息。
> 2. 定义了两个坐标转换函数map_abs2view和map_view2abs，用于将绝对坐标系坐标转换为视图坐标系坐标，以及将视图坐标系坐标转换回绝对坐标系坐标。
> 3. 定义了ask_pos_on_obj函数，用于在指定曲线上选择一个点，并在视图中创建一条垂直线，以获取点到曲线的最短距离。
> 4. 定义了select_point_on_curve函数，用于提示用户选择曲线上的一个点，并返回该点在曲线上的位置坐标。
> 5. 定义了ask_real函数，用于提示用户输入一个实数，并返回用户输入的值。
> 6. 定义了do_it函数，用于循环提示用户选择要裁剪的曲线端点，获取曲线长度，提示用户输入新的长度，然后调用UF_CURVE_trim_curve函数对曲线进行裁剪。
> 7. 定义了ufusr函数，用于初始化UF，调用do_it函数执行裁剪操作，然后终止UF。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载二次开发程序的标志。
>
> 整体来说，这段代码实现了通过用户交互来裁剪曲线长度的功能，具有较好的实用性和交互性。
>
