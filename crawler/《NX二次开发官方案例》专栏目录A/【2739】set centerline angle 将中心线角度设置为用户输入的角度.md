### 【2739】set centerline angle 将中心线角度设置为用户输入的角度

#### 代码

```cpp
    /*HEAD SET_CENTERLINE_ANGLE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，UF_print_syslog是V18版本中新增的函数。翻译如下：

注意：UF_print_syslog是V18版本新增的函数。 */  
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
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
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
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100];  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        UF_CALL(UF_DRF_ask_preferences(mpi, mpr, rad, dia));  
        if (prompt_for_a_number("Enter centerline angle", "Angle", &mpr[20]))  
        {  
            UF_CALL(UF_DRF_set_preferences(mpi, mpr, rad, dia));  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在发生UF函数调用错误时，输出错误信息到系统日志和对话框。
> 2. 定义了提示输入数值的函数prompt_for_a_number，用于在用户界面中提示用户输入一个数值。
> 3. do_it函数首先获取当前的绘图参数，然后提示用户输入中心线角度，如果用户输入了数值，则更新绘图参数中的中心线角度。
> 4. ufusr是NX二次开发程序的入口函数，首先初始化UF模块，然后调用do_it函数，最后在程序结束前关闭UF模块。
> 5. ufusr_ask_unload函数用于在程序卸载时立即卸载UF模块。
>
> 这段代码的目的是演示如何使用UF函数获取和设置NX的绘图参数，以及如何通过用户界面提示用户输入参数值。
>
