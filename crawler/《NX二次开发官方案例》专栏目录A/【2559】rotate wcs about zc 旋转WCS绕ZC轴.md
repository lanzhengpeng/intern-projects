### 【2559】rotate wcs about zc 旋转WCS绕ZC轴

#### 代码

```cpp
    /*HEAD ROTATE_WCS_ABOUT_ZC CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_mtx.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 版本新增的功能，用于在系统日志中打印信息。 */  
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
    static void do_it(void)  
    {  
        int  
            irc,  
            one = 1,  
            two = 2,  
            zero = 0;  
        tag_t  
            csys,  
            grp,  
            wcs_ent,  
            wcs_mx;  
        double  
            ninety = 90.0,  
            origin[3],  
            rot_mx[16],  
            wcs[9];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, wcs));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, wcs_mx, &csys));  
    /*  New in V16.0  
        UF_CALL(UF_MTX4_rotation(origin, &wcs[6], 90.0, rot_mx));  
        obsoletes call to uf5945 below */  
        FTN(uf5945)(origin, &wcs[6], &ninety, rot_mx, &irc);  
        FTN(uf5947)(rot_mx, &csys, &one, &two, &zero, &two, &csys, &grp, &irc);  
        UF_CALL(UF_CSYS_set_wcs(csys));  
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

> 这段代码是一个NX Open的二次开发示例，用于实现将当前工作坐标系绕ZC轴旋转90度的功能。
>
> 代码的主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印出调用UF函数失败的错误信息。
> 2. 定义了一个do_it函数，用于实现旋转功能：获取当前工作坐标系的tag获取工作坐标系的矩阵和原点基于原点和ZC轴方向，创建一个旋转90度的旋转矩阵将旋转矩阵应用于当前工作坐标系，得到一个新的坐标系将新坐标系设置为当前工作坐标系
> 3. 获取当前工作坐标系的tag
> 4. 获取工作坐标系的矩阵和原点
> 5. 基于原点和ZC轴方向，创建一个旋转90度的旋转矩阵
> 6. 将旋转矩阵应用于当前工作坐标系，得到一个新的坐标系
> 7. 将新坐标系设置为当前工作坐标系
> 8. ufusr函数是NX Open的入口函数，在初始化后调用do_it函数，实现旋转功能。
> 9. ufusr_ask_unload函数返回立即卸载，表示该程序不需要在NX中常驻。
>
> 总体来说，这段代码利用NX Open提供的API，实现了对当前工作坐标系的旋转控制，是一个NX二次开发的典型示例。
>
