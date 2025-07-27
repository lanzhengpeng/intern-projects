### 【2455】retrieve pattern 检索图案

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_vec.h>  
    #include <uf_patt.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static logical prompt_for_existing_part_name(char *prompt, char *fspec)  
    {  
        logical  
            unused;  
        int  
            resp;  
        UF_CALL(UF_UI_set_prompt(prompt));  
        if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
            || (resp == UF_UI_CANCEL)) return FALSE;  
        return TRUE;  
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
    static void ask_wcs_axes(double *xaxis, double *yaxis, double *zaxis)  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        double  
            axes[9],  
            origin[3];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
        UF_VEC3_copy(&axes[0], xaxis);  
        UF_VEC3_copy(&axes[3], yaxis);  
        UF_VEC3_copy(&axes[6], zaxis);  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    static void do_it(void)  
    {  
        tag_t  
            pattern;  
        double  
            patt_data[13] = { 1,0,0, 0,1,0, 0,0,1, 0,0,0, 1 };  
        char  
            pattern_spec[MAX_FSPEC_SIZE+1] = { "" };  
        while (prompt_for_existing_part_name("Pattern", pattern_spec) &&  
            specify_point("Indicate destination origin", &patt_data[9]))  
        {  
            ask_wcs_axes(&patt_data[0], &patt_data[3], &patt_data[6]);  
            WRITENZ(uc5823(pattern_spec, "", 1, patt_data, &pattern));  
        }  
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

> 这段代码是一个NX Open的二次开发示例，主要用于创建和放置一个Pattern。以下是代码的主要功能介绍：
>
> 1. 错误报告函数：定义了report_error函数，用于在函数调用失败时报告错误。
> 2. 用户交互函数：定义了prompt_for_existing_part_name和specify_point，分别用于提示用户输入pattern文件名和指定放置位置。
> 3. 获取坐标系函数：定义了ask_wcs_axes，用于获取当前工作坐标系的三个轴向量。
> 4. 创建Pattern：在do_it函数中，首先提示用户输入pattern文件名和指定放置位置，然后获取坐标系信息，并调用uc5823函数创建pattern。
> 5. UF初始化和终止：在ufusr函数中，初始化UF模块，调用do_it执行主逻辑，最后终止UF模块。
> 6. 卸载请求：ufusr_ask_unload函数返回立即卸载。
>
> 总体来说，这是一个简单的NX二次开发示例，实现了创建和放置pattern的功能，通过用户交互获取必要参数。
>
