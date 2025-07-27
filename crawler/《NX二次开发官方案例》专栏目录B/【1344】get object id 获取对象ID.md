### 【1344】get object id 获取对象ID

#### 代码

```cpp
    /*HEAD GET_OBJECT_ID CCC UFUN */  
    /* the sub_file_id is the same as the ID number returned by info --> object 里海译:子文件ID与info --> object返回的ID编号相同。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #define ECHOS(X)   (printf("%s = %s\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的功能。因此，翻译为“UF_print_syslog是V18版本新增的功能。” */  
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
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        unsigned int  
            version,  
            sub_file_id;  
        char  
            *file_data,  
            *handle;  
        while (object = select_anything("Select Object "))  
        {  
            handle = UF_TAG_ask_handle_of_tag(object);  
            UF_CALL(UF_TAG_decompose_handle( handle, &file_data, &sub_file_id,  
                &version));  
            ECHOS(handle);  
            ECHOS(file_data);  
            ECHO(sub_file_id);  
            ECHO(version);  
            UF_free(handle);  
            UF_free(file_data);  
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

> 这段代码是NX Open C++ API的一个示例，用于实现选择任意NX对象，并获取其ID编号。代码的主要功能包括：
>
> 1. 错误处理：定义了错误处理函数report_error，用于在函数调用失败时输出错误信息。
> 2. 对象选择：定义了select_anything函数，使用UI选择对话框让用户选择一个对象，并返回其tag。
> 3. ID获取：do_it函数循环调用select_anything，对每个选择的对象，使用UF_TAG_ask_handle_of_tag和UF_TAG_decompose_handle函数获取对象的ID编号，并打印出来。
> 4. 主函数：ufusr是主函数，初始化NX系统，调用do_it，然后终止NX系统。
> 5. 卸载处理：ufusr_ask_unload函数用于设置立即卸载二次开发程序。
>
> 总体来说，这段代码实现了选择任意NX对象，并获取其ID编号的功能，具有一定的参考价值。
>
