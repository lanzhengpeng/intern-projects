### 【2772】set layer 设置图层

#### 代码

```cpp
    /*HEAD SET_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。

只回答翻译内容，不添加额外信息：

UF_print_syslog是V18版本中新增的函数。 */  
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
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
        else return 0;  
    }  
    static void do_it(void)  
    {  
        int  
            n = 256;  
        tag_t  
            item;  
        while (((item = select_anything("Select object to move")) != NULL_TAG)  
            && ((n = prompt_for_an_integer("Enter new layer", "Layer", n)) != 0))  
            UF_CALL(UF_OBJ_set_layer(item, n));  
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

> 根据代码内容，这是一段用于NX二次开发的代码，其主要功能是让用户选择对象并将其移动到指定的层。以下是代码的详细功能：
>
> 1. 包含必要的头文件，如uf.h, uf_ui.h等。
> 2. 定义了宏ECHO和UF_CALL，用于输出变量值和调用UF函数时报告错误。
> 3. 定义了report_error函数，用于报告UF函数调用的错误。
> 4. 定义了select_anything函数，使用对话框让用户选择一个对象，并返回其tag。
> 5. 定义了prompt_for_an_integer函数，使用对话框提示用户输入一个整数。
> 6. 定义了do_it函数，循环调用select_anything和prompt_for_an_integer，让用户选择对象和输入层号，然后调用UF_OBJ_set_layer函数将对象移动到指定层。
> 7. 定义了ufusr函数，用于初始化和终止UF，并调用do_it执行主要功能。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码实现了选择对象并移动到指定层的基本功能，使用了UF函数进行NX的二次开发。
>
