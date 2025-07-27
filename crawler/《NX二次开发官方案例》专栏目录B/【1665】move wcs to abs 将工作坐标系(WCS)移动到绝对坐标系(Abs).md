### 【1665】move wcs to abs 将工作坐标系(WCS)移动到绝对坐标系(Abs)

#### 代码

```cpp
    /*HEAD MOVE_WCS_TO_ABS CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_csys.h>  
    #include <uf_disp.h>  
    #include <uf_mtx.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是在V18版本中新增的函数。 */  
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
        tag_t  
            matobj,                     /* Matrix object 里海译:矩阵对象 */  
            csys;                       /* Csys object  里海译:翻译Csys object，即坐标系对象。 */  
        double  
            origin[3] = { 0,0,0 },      /* Absolute origin. 里海译:绝对起源。 */  
            matrix[9];                  /* Identity matrix 里海译:Identity matrix的中文翻译为“单位矩阵”。 */  
        UF_MTX3_identity( matrix );  
        if (!UF_CALL( UF_CSYS_create_matrix(matrix, &matobj) ) &&  
            !UF_CALL( UF_CSYS_create_temp_csys(origin, matobj, &csys) ))  
            UF_CALL( UF_CSYS_set_wcs(csys) );  
        UF_DISP_refresh();  
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

> 这段代码是用于NX CAD系统进行坐标系转换的二次开发代码。
>
> 代码的主要功能是将绝对坐标系(ACS)转换为工作坐标系(WCS)。具体步骤如下：
>
> 1. 定义错误处理函数report_error，用于打印错误信息。
> 2. 定义转换函数do_it：
>
> 1. 定义ufusr函数，作为NX二次开发的入口函数：
>
> 1. 定义ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 综上所述，这段代码实现了将绝对坐标系(ACS)转换为工作坐标系(WCS)的功能，在NX CAD系统中具有实用价值。
>
