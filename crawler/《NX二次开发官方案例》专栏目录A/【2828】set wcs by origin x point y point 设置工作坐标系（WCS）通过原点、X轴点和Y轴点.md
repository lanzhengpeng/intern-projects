### 【2828】set wcs by origin x point y point 设置工作坐标系（WCS）通过原点、X轴点和Y轴点

#### 代码

```cpp
    /*HEAD SET_WCS_BY_ORIGIN_X_POINT_Y_POINT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。在V18之前的版本中，UF_print_syslog函数并不存在。

因此，UF_print_syslog是V18版本的一个新增功能，主要用于打印系统日志信息。 */  
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
    static void set_wcs_by_origin_x_point_y_point(double *origin, double *x_point,  
        double *y_point)  
    {  
        double  
            csys[9],  
            x_axis[3],  
            y_axis[3];  
        tag_t  
            mx,  
            wcs;  
        UF_VEC3_sub(x_point, origin, x_axis);  
        UF_VEC3_sub(y_point, origin, y_axis);  
        UF_CALL(UF_MTX3_initialize(x_axis, y_axis, csys));  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
    }  
    static void do_it(void)  
    {  
        double  
            origin[3],  
            xpoint[3],  
            ypoint[3];  
        if (specify_point("Orient WCS - Specify the CSYS origin", origin) &&  
            specify_point("Orient WCS - Specify the X-axis point", xpoint) &&  
            specify_point("Orient WCS - Specify the Y-axis point", ypoint))  
            set_wcs_by_origin_x_point_y_point(origin, xpoint, ypoint);  
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

> 这段代码是用于NX Open C++ API的二次开发，其主要功能是设置工作坐标系(WCS)。
>
> 关键函数和流程包括：
>
> 1. set_wcs_by_origin_x_point_y_point函数：通过输入原点(origin)、X轴点(x_point)和Y轴点(y_point)，计算出CSYS的9个参数，并创建临时坐标系，最后设置WCS。
> 2. specify_point函数：用于提示用户指定一个点，并将点坐标存储在传入的数组中。
> 3. ufusr函数：是NX Open C++ API二次开发的入口函数，其中调用UF_initialize进行初始化，do_it进行主体功能实现，最后调用UF_terminate终止。
> 4. ufusr_ask_unload函数：用于卸载二次开发代码。
>
> 通过指定原点、X轴点和Y轴点，代码实现了设置WCS的功能。
>
