### 【3166】list object id from handle 从句柄中获取对象的ID号码

#### 代码

```cpp
    /*HEAD LIST_OBJECT_ID_FROM_HANDLE CCC UFUN */  
    /* This will give the ID number seen when doing Information -> Object 里海译:当执行信息 -> 对象时，这将给出看到的ID号码。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，V18版本新增了UF_print_syslog函数。 */  
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
            msg[133],  
            *handle;  
        printf("===============================\n");  
        while (object = select_anything("Select Object "))  
        {  
            handle = UF_TAG_ask_handle_of_tag(object);  
            sprintf(msg, "The handle is: %s\n ",handle);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
            }  
            UF_CALL(UF_TAG_decompose_handle( handle, &file_data, &sub_file_id,   
    &version));  
            sprintf(msg, "sub file id is %d\n", sub_file_id);  
            UF_UI_write_listing_window(msg);  
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

> 这段代码是NX Open的二次开发示例代码，主要用于实现以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印出调用UF函数时的错误代码和错误信息。
> 2. 定义了一个选择任意对象的函数select_anything，该函数通过对话框让用户选择一个对象，并返回该对象的tag。
> 3. 定义了一个主函数do_it，该函数循环调用select_anything获取用户选择的对象，然后通过UF_TAG_ask_handle_of_tag获取对象的handle。
> 4. 对获取的handle进行分解，得到文件数据、子文件ID和版本号，并将子文件ID打印出来。
> 5. ufusr是主函数，在NX启动时调用，初始化后执行do_it函数，然后终止NX。
> 6. ufusr_ask_unload用于设置NX卸载该程序的方式。
>
> 这段代码的目的是演示如何获取用户选择的对象的子文件ID，主要使用了UF_UI、UF_TAG等NX Open API。
>
