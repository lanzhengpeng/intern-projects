### 【2740】set chaining tolerance 设置链式公差

#### 代码

```cpp
    /*HEAD SET_CHAINING_TOLERANCE CCC UFUN */  
    /*  Here is the corresponding GRIP which modifies the chaining tolerance.  
            NUMBER/  tolerance  
            UFARGS/  tolerance  
            &CHTOL = tolerance  
            print/'Chaining Tolerance has been set to '+FSTRL(&CHTOL)  
            HALT  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog 是 V18 版本新增的函数。 */  
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
        double  
            n = 0,  
            dn;  
        UF_args_t  
            arg;  
        prompt_for_a_number( "Enter Chaining Tolerance", "Number 0 - 100", &n);  
        dn = (double)n;  
        arg.type = UF_TYPE_DOUBLE;  
        arg.length = 0;  
        arg.address = &dn;  
    #ifdef _WIN32  
        UF_CALL(UF_call_grip("n:\\grip\\set_chaining_tolerance.grx", 1, &arg));  
    #else  
        UF_CALL(UF_call_grip("grip/set_chaining_tolerance.grx", 1, &arg));  
    #endif  
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

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要功能是设置NX中的链式公差（chaining tolerance）。以下是代码的主要逻辑：
>
> 1. 定义了report_error函数，用于在调用NX API时出错时打印错误信息。
> 2. 定义了prompt_for_a_number函数，用于弹出一个对话框，提示用户输入链式公差，范围是0-100。
> 3. 定义了do_it函数，用于调用一个Grip脚本（set_chaining_tolerance.grx），将用户输入的链式公差传递给Grip脚本。
> 4. ufusr函数是NX二次开发程序的入口函数，在这里调用do_it函数来设置链式公差。
> 5. ufusr_ask_unload函数返回UF_UNLOAD_IMMEDIATELY，表示程序不需要在NX退出时卸载。
> 6. 使用UF_CALL宏来调用NX API，并在出错时调用report_error函数。
> 7. 在Windows和Linux平台下，Grip脚本的路径不同。
> 8. 使用NX Open UI的函数来弹出对话框获取用户输入。
> 9. 通过NX Open C++ API调用Grip脚本，实现二次开发功能。
>
> 总体来说，这段代码通过C++调用NX API和Grip脚本，实现了设置链式公差的功能，展示了NX二次开发的基本流程。
>
