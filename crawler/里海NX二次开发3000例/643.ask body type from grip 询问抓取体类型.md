### ask body type from grip 询问抓取体类型

#### 代码

```cpp
    /*HEAD ASK_BODY_TYPE_FROM_GRIP CCC UFUN */  
    /*  NOTE:  As of V15, you can use the GRIP command OBTAIN to get this  
                information directly.  
        This is a generic UG/Open API (Ufunc) program which can be called whenever  
        it is desired to know whether a body is a sheet or a solid.  
        Here is sample GRIP code demonstating how to use the program:  
            entity/ body  
            number/ flag  
            grargs/ body, flag  
            mask/ 70  
    a10:    ident/ 'Select a body', body, resp  
            ifthen/ resp > 2  
                xspawn/ ufun, 'ufunc/ask_body_type_from_grip.sl'  
                if/ flag == &yes, messg/'That''s a solid'  
                if/ flag == &no, messg/'That''s a sheet'  
                jump/ a10:  
            endif  
            halt  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中的新增功能。 */  
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
            body;  
        int  
            type = 0;  
        double  
            flag;  
        UF_args_t  
            args[2];  
        args[0].type = UF_TYPE_TAG_T;  
        args[0].length = 0;  
        args[0].address = &body;  
        args[1].type = UF_TYPE_DOUBLE;  
        args[1].length = 0;  
        args[1].address = &flag;  
        UF_CALL(UF_ask_grip_args(2, args));  
        UF_CALL(UF_MODL_ask_body_type(body, &type));  
        switch (type)  
        {  
            case UF_MODL_SOLID_BODY:  
                flag = 1;  
                break;  
            case UF_MODL_SHEET_BODY:  
                flag = 2;  
                break;  
            default:  
                flag = 0; /* Unknown ? 译:对不起，没有提供足够的上下文信息，无法准确翻译"Unknown ?"。如果能提供更多上下文，我可以更好地为您提供帮助。 */  
                break;  
        }  
        UF_CALL(UF_set_grip_args(2, args));  
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

> 这是一段用于判断NX中实体是实体还是薄壳的二次开发代码。其主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于在发生错误时输出错误信息。
> 2. 定义了主体函数do_it，用于判断传入的实体是实体还是薄壳，并通过全局变量flag输出结果。
> 3. 定义了UFusr函数，用于初始化和终止NX环境，并调用do_it函数。
> 4. 定义了ufusr_ask_unload函数，用于设置二次开发模块卸载模式。
> 5. 使用UF_ask_grip_args函数获取传入的实体tag，然后使用UF_MODL_ask_body_type函数判断其类型，根据结果设置全局变量flag的值。
> 6. 使用UF_set_grip_args函数将结果输出，以便后续调用。
> 7. 在注释中提供了使用该函数的GRIP示例代码。
>
> 综上所述，这段代码实现了根据实体tag判断其类型的功能，并通过全局变量输出结果，方便其他函数调用。
>
