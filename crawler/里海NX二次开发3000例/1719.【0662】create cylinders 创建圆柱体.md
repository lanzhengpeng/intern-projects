### 【0662】create cylinders 创建圆柱体

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            cylinder;  
        double  
            height = 4.0,  
            diam = 1.0,  
            dir[3],  
            origin[3];  
        char  
            dia_str[20],  
            hgt_str[20];  
        while (prompt_for_two_numbers("Enter Cylinder Parameters", "Diameter",  
            "Height", &diam, &height) &&  
            specify_vector("Cylinder axis", dir, origin))  
        {  
            sprintf(hgt_str, "%f", height);  
            sprintf(dia_str, "%f", diam);  
            UF_CALL(UF_MODL_create_cylinder(UF_NULLSIGN, 0, origin,  
                hgt_str, dia_str, dir, &cylinder));  
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

> 这段代码是一段NX Open C++的二次开发代码，实现了以下功能：
>
> 1. ECHO函数：用于输出信息到列表窗口和系统日志。
> 2. report_error函数：用于报告函数调用失败的错误代码和错误消息。
> 3. prompt_for_two_numbers函数：通过对话框提示用户输入两个数字，并返回用户是否输入。
> 4. specify_vector函数：通过对话框提示用户指定一个向量，并返回用户是否指定。
> 5. do_it函数：主函数，循环提示用户输入圆柱的直径和高度，以及指定圆柱轴线。然后调用UF_MODL_create_cylinder函数创建圆柱。
> 6. ufusr函数：NX二次开发程序的入口函数，用于初始化NX系统并调用do_it函数。
> 7. ufusr_ask_unload函数：用于卸载二次开发程序。
>
> 总体来说，这段代码实现了通过对话框交互式创建圆柱的功能。用户可以输入圆柱的直径、高度，以及指定圆柱轴线。通过调用UF_MODL_create_cylinder函数，根据用户输入的参数创建圆柱。
>
