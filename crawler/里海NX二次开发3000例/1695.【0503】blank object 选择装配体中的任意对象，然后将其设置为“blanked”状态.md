### 【0503】blank object 选择装配体中的任意对象，然后将其设置为“blanked”状态

#### 代码

```cpp
    /*HEAD BLANK_OBJECT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数，用于打印系统日志信息。 */  
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
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            object;  
        char  
            prefix[MAX_ENTITY_NAME_SIZE + 1] = { "item-" };  
        while ( (object = select_anything("Select an object")) != NULL_TAG)   
        {  
            UF_CALL(UF_OBJ_set_blank_status(object, UF_OBJ_BLANKED) );  
        }  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发应用程序示例，主要功能如下：
>
> 1. 包含了必要的头文件，如stdio.h、string.h、uf.h、uf_ui.h等。
> 2. 定义了ECHO和UF_CALL两个宏，分别用于打印变量值和调用UF函数并报告错误。
> 3. 实现了一个静态函数report_error，用于打印函数调用失败的错误代码和错误信息。
> 4. 实现了一个静态函数select_anything，使用UF_UI_select_with_single_dialog函数让用户选择一个对象，并返回对象的tag。
> 5. 实现了一个静态函数do_it，不断调用select_anything让用户选择对象，然后调用UF_OBJ_set_blank_status函数将选中的对象设置为空状态。
> 6. 实现了ufusr函数，这是程序的入口函数。在ufusr中初始化UF库，调用do_it执行主要功能，然后终止UF库。
> 7. 实现了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示应用程序可以立即卸载。
> 8. 程序的主要功能是使用户可以不断选择对象，并将选中的对象设置为空状态，以实现隐藏对象的效果。
> 9. 使用UF库提供的函数实现NX的二次开发功能。
> 10. 注释详细，代码结构清晰，易于理解。
>
