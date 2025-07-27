### 【2764】set hidden edge color 设置隐藏边缘颜色

#### 代码

```cpp
    /*HEAD SET_HIDDEN_EDGE_COLOR CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define ECHOL(X)   (X ? printf("%s = TRUE\n", #X) : printf("%s = FALSE\n", #X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数。

翻译：
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
    static int prompt_for_an_integer(char *prompt, char *item, int number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            return da[0];  
        }  
        else return -3;  
    }  
    static void do_it(void)  
    {  
        UF_DISP_system_params_t  
            system_parameters;  
        int  
            n = 0;  
        UF_CALL(UF_DISP_ask_system_parameters(&system_parameters));  
        n = system_parameters.hidden_shaded_face_edges;  
        system_parameters.show_shaded_face_edges = TRUE;  
        while ((n = prompt_for_an_integer("Enter Integer for Hidden Edge color ( -1 to 15 )", "Color Number", n)) != -3)   
        {  
            system_parameters.hidden_shaded_face_edges = n;  
            UF_CALL(UF_DISP_set_system_parameters(&system_parameters));  
            UF_CALL(UF_DISP_regenerate_display());  
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

> 这段代码是一个NX Open C++应用程序，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数失败时打印错误信息。
> 2. 定义了一个提示输入整数的函数prompt_for_an_integer，用于提示用户输入整数参数。
> 3. 定义了一个主函数do_it，用于实现以下功能：获取NX显示系统的参数，如隐藏边线颜色等。循环提示用户输入隐藏边线的颜色编号(-1到15)，并将新的颜色参数设置给NX。重新生成NX的显示，以应用新的颜色设置。如果用户输入-1，则退出循环。
> 4. 获取NX显示系统的参数，如隐藏边线颜色等。
> 5. 循环提示用户输入隐藏边线的颜色编号(-1到15)，并将新的颜色参数设置给NX。
> 6. 重新生成NX的显示，以应用新的颜色设置。
> 7. 如果用户输入-1，则退出循环。
> 8. 定义了ufusr函数，作为程序的入口点。在ufusr中，先初始化UF模块，然后调用do_it函数，最后终止UF模块。
> 9. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示在应用程序退出时不需要等待。
>
> 通过以上函数，该程序实现了在NX中设置隐藏边线的颜色，并提供了交互式提示输入功能。
>
