### 【2910】specify plane from grip Specify plane from grip：从GRIP指定平面

#### 代码

```cpp
    /*HEAD SPECIFY_PLANE_FROM_GRIP CCC UFUN */  
    /*  
        This is a generic UG/Open API (Ufunc) program which can be called whenever  
        it is desired for a GRIP program to use the plane subfunction to query  
        the user to specify a plane.  
         Here is sample GRIP code demonstating how to use the program:  
            NUMBER/ mx(9), loc(3), resp  
            STRING/ prompt(80)  
            ENTITY/ p(3)  
            GRARGS/ prompt, loc, mx, resp  
            &WCS = &ABS  
    A10:    prompt = 'Specify plane ' + ISTR(ii = ii+1)  
            XSPAWN/ ufun, 'ufunc/specify_plane_from_grip.sl'  
            IFTHEN/ resp > 2  
                p(1) = POINT/ loc  
                p(2) = POINT/ loc(1) + mx(1), loc(2) + mx(2), loc(3) + mx(3)  
                p(3) = POINT/ loc(1) + mx(4), loc(2) + mx(5), loc(3) + mx(6)  
                PLANE/ p(1..3)  
                DELETE/ p(1..3)  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的，仅提供翻译，不要添加无关内容。 */  
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
            mode = 0,  
            resp;  
        tag_t  
            plane;  
        double  
            grip_resp,  
            origin[3],  
            xyz[9];  
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
        args[2].length  = 9;  
        args[2].address = xyz;  
        args[3].type    = UF_TYPE_DOUBLE;  
        args[3].length  = 0;  
        args[3].address = &grip_resp;  
        UF_CALL(UF_ask_grip_args(4, args));  
        UF_CALL(UF_UI_specify_plane(prompt, &mode, 0, &resp, xyz, origin, &plane));  
        grip_resp = resp;  
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

> 这是段用于在NX GRIP程序中调用用户界面以指定平面的UFUNC函数代码。以下是主要内容和步骤：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于输出UF函数调用失败时的错误信息。
> 2. 主体函数：do_it函数用于获取GRIP传入的参数，并调用UF_UI_specify_plane函数打开用户界面让用户指定平面。指定的平面参数会通过GRIP传出。
> 3. UFUNC入口函数：ufusr是UFUNC的入口函数，首先调用UF_initialize进行初始化，然后执行do_it函数，最后调用UF_terminate进行清理。
> 4. 卸载函数：定义了卸载函数ufusr_ask_unload，返回UF_UNLOAD_IMMEDIATELY表示UFUNC可以被立即卸载。
> 5. 代码注释：提供了详细的代码注释，以帮助读者理解代码功能。
> 6. GRIP调用示例：在代码头部给出了如何在GRIP中调用该UFUNC函数的示例代码。
> 7. 注意事项：代码使用了NX OpenUF函数，需要运行在NX环境中。
>
> 综上所述，这段代码实现了在GRIP程序中调用用户界面以指定平面的功能，并以UFUNC的形式封装，便于在GRIP中调用。
>
