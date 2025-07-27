### 【3147】report kf reference of selected objects 获取用户选择的对象的KF参考，并报告出来

#### 代码

```cpp
    /*HEAD REPORT_KF_REFERENCE_OF_SELECTED_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
    #include <uf_part.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static tag_t select_an_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL,NULL,&resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static char *ask_kf_reference(tag_t object)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            *datatype,  
            *name_chain,  
            *string = NULL;  
        UF_KF_instance_p_t  
            instance;  
        UF_KF_list_p_t  
            list = NULL;  
        UF_KF_value_p_t  
            sub_list,  
            value;  
        UF_CALL(UF_KF_init_part(part));  
        UF_CALL(UF_KF_ask_fusion_object(object, &instance));  
        if (instance == NULL)  
        {  
        /*  UF_KF_make_tag fails with occurrences - see PR 6101556 里海译:翻译 UF_KF_make_tag fails with occurrences - see PR 6101556 */  
            if (UF_CALL(UF_KF_make_tag(object, &value))) return NULL;  
            UF_CALL(UF_KF_list_push(list, value, &list));  
            UF_CALL(UF_KF_make_list(list, &value));  
            UF_CALL(UF_KF_ask_value_of_list_item(value, 1, &string, &datatype,  
                &sub_list));  
            UF_free(datatype);  
        }  
        else  
        {  
            UF_CALL(UF_KF_ask_name_chain_of_instance(instance, &name_chain));  
            UF_CALL(allocate_memory(strlen(name_chain)-4, (void **)&string));  
            strcpy(string, strstr(name_chain, ":")+1);  
        }  
        return string;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        char  
            *reference;  
        while ((object = select_an_object("Report KF reference")) != NULL_TAG)  
        {  
            reference = ask_kf_reference(object);  
            WRITE_S(reference);  
            UF_free(reference);  
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

> 这段代码是用于NX(原名Unigraphics)的二次开发，其主要功能是查询并报告选中对象的知识融合(Knowledge Fusion)引用信息。
>
> 代码主要包含以下部分：
>
> 1. 包含头文件，引入了NX提供的各种API函数。
> 2. 定义了错误报告函数report_error，用于在调用NX API函数出错时，输出错误信息。
> 3. 定义了选择对象函数select_an_object，用于通过对话框选择一个对象。
> 4. 定义了字符串写入列表窗口函数write_string_to_listing_window。
> 5. 定义了内存分配函数allocate_memory。
> 6. 定义了查询KF引用函数ask_kf_reference，用于查询选中对象的KF引用信息。
> 7. 定义了主体函数do_it，循环调用select_an_object选择对象，然后调用ask_kf_reference查询KF引用，并输出到列表窗口。
> 8. 定义了ufusr函数，这是NX二次开发程序的入口函数，用于初始化NX环境，调用do_it主体函数，然后结束NX环境。
> 9. 定义了卸载函数ufusr_ask_unload，用于在卸载NX时释放资源。
>
> 总的来说，这段代码通过NX提供的API，实现了交互选择对象并查询其知识融合引用信息的功能。
>
