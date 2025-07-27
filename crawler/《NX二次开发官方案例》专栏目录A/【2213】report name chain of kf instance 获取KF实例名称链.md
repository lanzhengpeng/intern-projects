### 【2213】report name chain of kf instance 获取KF实例名称链

#### 代码

```cpp
    /*HEAD REPORT_NAME_CHAIN_OF_KF_INSTANCE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_kf.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 版本新增的功能。 */  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void report_name_chain_of_object(tag_t object)  
    {  
        UF_KF_instance_p_t  
            instance;  
        char  
            *name_chain;  
        UF_CALL(UF_KF_ask_fusion_object(object, &instance));  
        if ((instance != NULL_TAG) &&  
           !UF_CALL(UF_KF_ask_name_chain_of_instance(instance, &name_chain)))  
            uc1601(name_chain, TRUE);  
        else  
            uc1601("That is not a KF Object", TRUE);  
    }  
    static void do_it(void)  
    {  
        logical  
            initialized;  
        tag_t  
            object;  
        if (!UF_CALL(UF_KF_is_initialized(&initialized)) && (!initialized))  
            UF_CALL(UF_KF_init_part(UF_PART_ask_display_part()));  
        while ((object = select_anything("Select UG/KF object")) != NULL_TAG)  
            report_name_chain_of_object(object);  
        while ((object = select_a_feature("Select UG/KF feature")) != NULL_TAG)  
            report_name_chain_of_object(object);  
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

> 这段代码是NX Open C++ API的二次开发示例代码，主要实现了以下功能：
>
> 1. 包含了必要的NX Open C++ API头文件，如uf.h, uf_ui.h等。
> 2. 定义了一个宏UF_CALL，用于检查UF函数调用是否成功，并在出错时报告错误信息。
> 3. 定义了一个静态函数report_error，用于在UF函数调用出错时打印错误信息。
> 4. 定义了一个静态函数select_anything，用于使用单选对话框选择一个NX对象。
> 5. 定义了一个静态函数select_a_feature，用于选择一个NX特征。
> 6. 定义了一个静态函数report_name_chain_of_object，用于获取一个NX对象的名字链，即该对象的所有上级命名对象的名字链。
> 7. 定义了一个静态函数do_it，其中初始化了UG/KF系统，然后循环调用select_anything和select_a_feature选择对象或特征，并调用report_name_chain_of_object打印它们的名字链。
> 8. 定义了ufusr函数，这是NX二次开发的入口函数，其中初始化了NX Open C++环境，调用do_it执行主要功能，然后终止NX Open C++环境。
> 9. 定义了ufusr_ask_unload函数，用于询问是否立即卸载NX Open C++库。
>
> 综上所述，该代码通过NX Open C++ API实现了选择NX对象或特征并打印其名字链的功能，是NX二次开发的一个典型示例。
>
