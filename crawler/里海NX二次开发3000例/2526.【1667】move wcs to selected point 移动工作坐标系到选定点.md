### 【1667】move wcs to selected point 移动工作坐标系到选定点

#### 代码

```cpp
    /*HEAD MOVE_WCS_TO_SELECTED_POINT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的函数。 */  
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
    static void set_wcs_origin(double new_origin[3])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        double  
            old_origin[3];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, old_origin));  
        UF_CALL(UF_CSYS_create_temp_csys(new_origin, wcs_mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
    }  
    static void do_it(void)  
    {  
        double  
            new_origin[3];  
        if (specify_point("Indicate new WCS origin", new_origin))  
            set_wcs_origin(new_origin);  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误报告函数：通过调用UF_get_fail_message获取错误信息，并输出到系统日志和对话框中。
> 2. 获取用户指定点：使用函数specify_point，让用户在图形窗口中指定一个点，并将该点的坐标存储在数组loc中。
> 3. 设置WCS原点：函数set_wcs_origin会先获取当前的WCS信息，然后创建一个新的临时坐标系，其原点为用户指定的点，并将新的坐标系设置为WCS。
> 4. 主函数：函数do_it调用specify_point让用户指定新原点，然后调用set_wcs_origin设置WCS原点。
> 5. ufusr函数：该函数是NX二次开发的入口函数，初始化NX环境后调用do_it函数，最后终止NX环境。
> 6. 卸载函数：函数ufusr_ask_unload返回立即卸载标志，表示该模块不需要在NX中保留状态，可以立即卸载。
>
> 总体而言，该代码实现了在NX中让用户指定WCS原点的功能，并在用户指定新原点后更新WCS。通过二次开发，扩展了NX的交互式操作功能。
>
