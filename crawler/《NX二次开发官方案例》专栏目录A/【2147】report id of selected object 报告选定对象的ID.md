### 【2147】report id of selected object 报告选定对象的ID

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <stdarg.h>  
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
    /* qq3123197280 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
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
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        unsigned int  
            ID,  
            version;  
        tag_t  
            object;  
        char  
            *file_data,  
            *handle;  
        while ((object = select_anything("Report Object ID")) != NULL_TAG)  
        {  
            handle = UF_TAG_ask_handle_of_tag(object);  
            UF_CALL(UF_TAG_decompose_handle(handle, &file_data, &ID, &version));  
            WRITE_D(ID);  
            UF_free(file_data);  
            UF_free(handle);  
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

> 根据代码注释和NX二次开发知识，这是段NX Open UI的二次开发代码，主要实现功能是选择NX装配中的任意一个对象，并打印出该对象的ID号。
>
> 代码的关键部分包括：
>
> 1. ECHO宏：用于打印错误信息到NX的日志窗口和系统日志。
> 2. UF_CALL宏：封装UF函数调用，用于报告错误。
> 3. mask_add_faces_and_edges函数：设置选择掩码，允许选择实体面和边。
> 4. select_anything函数：使用单选对话框让用户选择一个对象，并返回该对象的tag。
> 5. write_integer_to_listing_window函数：用于打印整数值到NX的列表窗口。
> 6. do_it函数：循环调用select_anything获取对象tag，然后分解tag获取ID并打印。
> 7. ufusr函数：NX Open UI的入口函数，初始化后调用do_it，最后终止。
> 8. ufusr_ask_unload函数：返回立即卸载标志。
>
> 总体来说，这段代码通过封装UF函数，实现了简单的选择打印对象ID的功能，展示了NX Open UI二次开发的基本流程。
>
