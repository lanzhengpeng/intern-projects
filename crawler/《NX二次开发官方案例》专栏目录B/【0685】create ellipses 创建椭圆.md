### 【0685】create ellipses 创建椭圆

#### 代码

```cpp
    /*HEAD CREATE_ELLIPSES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中新增的函数。 */  
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
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void map_abs2wcs(double *cc)  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            wcs[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));  
        FTN(uf5940)(abs_mx, wcs, mx, &irc);  
        FTN(uf5941)(cc, mx);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            ellipse,  
            wcs;  
        double  
            end_angle = 360,  
            n_adj,  
            start_angle = 0;  
        UF_CURVE_conic_t  
            conic_data;  
        conic_data.k1 = 2;  
        conic_data.k2 = 1;  
        conic_data.conic_type = UF_conic_ellipse_subtype;  
        conic_data.rotation_angle = 0.0;  
        while (specify_point("Ellipse Center", conic_data.center) &&  
            prompt_for_two_numbers("Ellipse", "Start Angle", "End Angle", &start_angle,  
                &end_angle) &&  
            prompt_for_two_numbers("Ellipse", "Semimajor", "Semiminor", &conic_data.k1,  
                &conic_data.k2))  
        {  
            UF_CALL(UF_CSYS_ask_wcs(&wcs));  
            UF_CALL(UF_CSYS_ask_matrix_of_object(wcs, &conic_data.matrix_tag));  
            map_abs2wcs(conic_data.center);  
            while (start_angle < 0) start_angle = 360 + start_angle;  
            conic_data.start_param = atan( tan( DEGRA * start_angle) *  
                (conic_data.k1 / conic_data.k2));  
            modf((start_angle + 90)/180, &n_adj);  
            conic_data.start_param = (n_adj * PI) + conic_data.start_param;  
            while (end_angle < 0) end_angle = 360 + end_angle;  
            conic_data.end_param = atan( tan( DEGRA * end_angle ) *  
                (conic_data.k1 / conic_data.k2));  
            modf((end_angle + 90)/180, &n_adj);  
            conic_data.end_param = (n_adj * PI) + conic_data.end_param;  
            if (end_angle < start_angle)  
                conic_data.end_param = TWOPI + conic_data.end_param;  
            UF_CALL(UF_CURVE_create_conic(&conic_data, &ellipse));  
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

> 这段代码是一个NX Open C++ UFUN程序，主要用于在NX中绘制椭圆。其主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在UF函数调用出错时输出错误信息。
> 2. 定义了specify_point函数，用于提示用户输入椭圆中心点坐标。
> 3. 定义了prompt_for_two_numbers函数，用于提示用户输入椭圆的起始角度和终止角度、以及长轴和短轴的长度。
> 4. 定义了map_abs2wcs函数，用于将绝对坐标系下的点转换到工作坐标系下。
> 5. 定义了do_it函数，用于创建椭圆。该函数首先获取当前的工作坐标系，然后提示用户输入椭圆的中心点坐标、起始角度、终止角度、长轴和短轴长度。根据用户输入，计算出椭圆的参数方程，并调用UF_CURVE_create_conic函数创建椭圆。
> 6. ufusr函数是程序的入口函数，首先初始化UF系统，然后调用do_it函数绘制椭圆，最后终止UF系统。
> 7. ufusr_ask_unload函数用于卸载UF系统。
>
> 总体来说，这段代码实现了交互式绘制椭圆的功能，用户可以自由输入椭圆的各种参数，程序会根据这些参数在NX中绘制出相应的椭圆。
>
