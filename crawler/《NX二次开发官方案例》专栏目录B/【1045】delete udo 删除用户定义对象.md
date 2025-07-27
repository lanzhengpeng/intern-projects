### 【1045】delete udo 删除用户定义对象

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
    static int filter_udo_class(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        UF_UDOBJ_class_t  
            *desired_class_id = (UF_UDOBJ_class_t *) data,  
            this_class_id;  
        UF_UDOBJ_all_data_t  
            udo_data;  
        if (type[0] != UF_user_defined_object_type) return (UF_UI_SEL_REJECT);  
        UF_CALL(UF_UDOBJ_ask_udo_data (object, &udo_data));  
        this_class_id = udo_data.class_id;  
        UF_CALL(UF_UDOBJ_free_udo_data(&udo_data));  
        if ((this_class_id == *desired_class_id) || (*desired_class_id == 0))  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_udo_class(UF_UI_selection_p_t select, void *class)  
    {  
        UF_UI_mask_t  
            mask = { UF_user_defined_object_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_udo_class, NULL,  
                class)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_udo(char *prompt, UF_UDOBJ_class_t desired_class)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_udo_class, &desired_class,  
            &resp, &object, cp, &view));  
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
        while ((object = select_a_udo("Select UDO", 0)) != NULL_TAG)  
            UF_CALL(UF_OBJ_delete_object(object));  
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

> 根据代码的内容，这是一段用于在NX中批量删除用户自定义对象的二次开发代码。具体来说：
>
> 1. 错误处理和日志输出：代码首先定义了错误处理函数report_error，用于在函数调用出错时输出错误信息，并包含了一个用于输出信息的宏ECHO。
> 2. 对象过滤函数：filter_udo_class函数用于过滤用户自定义对象，可以指定需要保留的类ID。如果指定了类ID，则只保留该类ID的对象；如果没有指定类ID，则保留所有用户自定义对象。
> 3. 对象选择函数：mask_udo_class函数用于设置选择过滤器，只允许选择用户自定义对象。select_a_udo函数用于弹出选择对话框，让用户选择一个用户自定义对象。
> 4. 删除对象：do_it函数循环调用select_a_udo让用户选择用户自定义对象，然后调用UF_OBJ_delete_object将其删除。直到用户取消选择为止。
> 5. 程序入口：ufusr是程序的入口函数，其中初始化NX环境，调用do_it执行删除操作，最后终止NX环境。
> 6. 卸载请求：ufusr_ask_unload函数返回立即卸载标志，表示在卸载时无需等待。
>
> 总体来说，这段代码实现了在NX中批量删除用户自定义对象的功能，具有错误处理和日志输出功能，通过选择对话框交互式选择要删除的对象。
>
