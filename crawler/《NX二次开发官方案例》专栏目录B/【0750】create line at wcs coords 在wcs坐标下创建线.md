### 【0750】create line at wcs coords 在wcs坐标下创建线

#### 代码

```cpp
    /*HEAD CREATE_LINE_AT_WCS_COORDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_trns.h>  
    #include <uf_csys.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据您提供的提示，UF_print_syslog是在V18版本中新增的功能。翻译如下：

UF_print_syslog是V18版本新增的功能。 */  
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
    static logical prompt_for_three_numbers(char *prompt, char *item1, char *item2,  
        char * item3, double *number1, double *number2, double *number3)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[3][16];  
        double  
            da[3];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        strncpy(&menu[2][0], item3, 15);  
        da[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
        resp = uc1609(prompt, menu, 3, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void map_wcs2abs(double *cc)  
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
        FTN(uf5940)(wcs, abs_mx, mx, &irc);  
        FTN(uf5941)(cc, mx);  
    }  
    static void pt_populate(double pt2pop[3], double pt[3])  
    {  
        pt2pop[0] = pt[0];  
        pt2pop[1] = pt[1];  
        pt2pop[2] = pt[2];  
    }  
    static void do_it(void)  
    {  
        UF_CURVE_line_t  
            line_data;  
        tag_t  
            line_tag;  
        double  
            st[3] = {0},  
            end[3]= {0};  
        while( (prompt_for_three_numbers("Line Start", "XC", "YC", "ZC",  
            &st[0], &st[1], &st[2])) &&   
            (prompt_for_three_numbers("Line End", "XC", "YC", "ZC",   
            &end[0], &end[1], &end[2])) )  
        {  
            pt_populate( line_data.start_point, st);  
            pt_populate( line_data.end_point , end);  
            map_wcs2abs( line_data.start_point);  
            map_wcs2abs( line_data.end_point);  
            UF_CALL(UF_CURVE_create_line(&line_data, &line_tag));  
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

> 这段代码是一个NX Open C++的二次开发示例，用于在NX中创建线。
>
> 代码主要实现了以下功能：
>
> 1. 错误处理函数：report_error函数用于打印错误信息。
> 2. 交互式获取参数：prompt_for_three_numbers函数用于交互式获取用户输入的三个数字。
> 3. 世界坐标系到绝对坐标系的转换：map_wcs2abs函数用于将用户输入的基于世界坐标系的点转换为绝对坐标系。
> 4. 创建线：do_it函数用于获取用户输入的线的起点和终点，转换为绝对坐标系后，调用UF_CURVE接口创建线。
> 5. 主函数：ufusr函数用于初始化和调用do_it函数。
> 6. 卸载函数：ufusr_ask_unload函数用于设置二次开发的卸载方式。
>
> 整体而言，这段代码通过交互式获取用户输入的线的起点和终点，转换坐标系后调用UF_CURVE接口在NX中创建线。
>
