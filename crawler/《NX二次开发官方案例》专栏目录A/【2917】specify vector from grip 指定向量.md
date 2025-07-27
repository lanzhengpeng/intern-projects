### 【2917】specify vector from grip 指定向量

#### 代码

```cpp
    /*HEAD SPECIFY_VECTOR_FROM_GRIP CCC UFUN */  
    /*  
        This is a generic UG/Open API (Ufunc) program which can be called whenever  
        it is desired for a GRIP program to use the vector subfunction to query  
        the user to specify a vector.  
         Here is sample GRIP code demonstating how to use the program:  
            NUMBER/ dir(3), loc(3), resp  
            STRING/ prompt(80)  
            GRARGS/ prompt, loc, dir, resp  
            &WCS = &ABS  
    A10:    prompt = 'Specify vector ' + ISTR(ii = ii+1)  
            XSPAWN/ ufun, 'ufunc/specify_vector_from_grip.sl'  
            IFTHEN/ resp > 2  
                LINE/ loc(1..3), loc(1)+dir(1), loc(2)+dir(2), loc(3)+dir(3)  
                JUMP/ A10:  
            ENDIF  
            HALT  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意：UF_print_syslog 是在 V18 版本中新增的。 */  
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
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        double  
            grip_resp,  
            normal[3],  
            origin[3];  
        char  
            prompt[81];  
        UF_args_t  
            args[4];  
        args[0].type    = UF_TYPE_CHAR;  
        args[0].length  = 0;  
        args[0].address = prompt;  
        args[1].type    = UF_TYPE_DOUBLE_ARRAY;  
        args[1].length  = 3;  
        args[1].address = origin;  
        args[2].type    = UF_TYPE_DOUBLE_ARRAY;  
        args[2].length  = 3;  
        args[2].address = normal;  
        args[3].type    = UF_TYPE_DOUBLE;  
        args[3].length  = 0;  
        args[3].address = &grip_resp;  
        UF_CALL(UF_ask_grip_args(4, args));  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            normal, origin, &resp));  
    /*  This aligns the response with the usual GRIP response values 里海译:这与通常的GRIP响应值一致。 */  
        grip_resp = 5.0 - resp;  
        UF_CALL(UF_set_grip_args(4, args));  
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

> 这段代码是一个NX Open API的程序，主要用于实现GRIP程序中的指定向量的功能。它允许用户通过交互方式指定一个向量的起点和方向。
>
> 代码的主要功能包括：
>
> 1. 通过UF_ask_grip_args函数获取GRIP传入的参数，包括提示信息、起点坐标、方向坐标和响应值。
> 2. 调用UF_UI_specify_vector函数弹出对话框，让用户指定一个向量的起点和方向。
> 3. 将用户的响应值转换成GRIP需要的格式，并通过UF_set_grip_args函数返回给GRIP程序。
> 4. 使用UF_CALL宏进行错误处理，如果函数调用失败，会打印错误信息。
> 5. 在ufusr函数中初始化和终止NX Open API。
> 6. 提供ufusr_ask_unload函数，用于立即卸载该ufusr函数。
>
> 这段代码通过封装NX Open API的图形交互函数，实现了GRIP程序与用户交互的功能，增强了GRIP程序的用户友好性。
>
