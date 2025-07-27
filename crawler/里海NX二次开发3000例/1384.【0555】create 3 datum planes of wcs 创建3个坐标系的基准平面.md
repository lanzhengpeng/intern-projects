### 【0555】create 3 datum planes of wcs 创建3个坐标系的基准平面

#### 代码

```cpp
    /*HEAD CREATE_3_DATUM_PLANES_OF_WCS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog 是 V18 中的新增功能，只回答译文，不要废话。

翻译：

注意：UF_print_syslog 是 V18 中的新增功能，只回答译文，不要废话。 */  
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
    static void ask_wcs_info(double origin[3], double axes[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            dplane[3];  
        double  
            directions[9],  
            point[3];  
        ask_wcs_info(point, directions);  
        for (ii = 0; ii < 3; ii++)  
            UF_CALL(UF_MODL_create_fixed_dplane(point, &directions[3*ii],  
                &dplane[ii]));  
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

> 这是一段NX的二次开发代码，其主要功能是在NX中创建工作坐标系(WCS)的3个固定平面。
>
> 代码主要包含以下几个部分：
>
> 1. 包含头文件：包含了UF、UF_UI、UF_CSYS、UF_MODL等NX API的头文件。
> 2. 错误报告函数report_error：用于在API调用出错时报告错误信息。
> 3. 获取WCS信息函数ask_wcs_info：用于获取当前WCS的原点坐标和3个坐标轴的方向。
> 4. 创建固定平面函数do_it：根据WCS信息，创建3个与WCS坐标轴平行的固定平面。
> 5. ufusr函数：NX二次开发程序的入口函数，在这里调用do_it函数来创建固定平面。
> 6. ufusr_ask_unload函数：用于卸载NX插件。
>
> 整体来说，这段代码通过获取当前WCS的坐标系信息，并基于这些信息创建3个固定的坐标系平面。这些固定平面与WCS的坐标轴平行，可以帮助用户更好地理解和使用WCS坐标系。
>
