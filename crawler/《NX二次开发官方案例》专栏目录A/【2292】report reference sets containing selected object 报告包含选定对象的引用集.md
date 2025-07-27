### 【2292】report reference sets containing selected object 报告包含选定对象的引用集

#### 代码

```cpp
    /*HEAD REPORT_REFERENCE_SETS_CONTAINING_SELECTED_OBJECT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的功能。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define SHOW_NAME(X) report_object_name(#X, X)  
    static void report_object_name(char *name, tag_t object)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN],  
            obj_name[MAX_ENTITY_NAME_SIZE + 1];  
        if (!UF_OBJ_ask_name(object, obj_name))  
            sprintf(msg, "%s (tag %d) is named %s\n", name, object, obj_name);  
        else  
            sprintf(msg, "%s (tag %d) is not named\n", name, object);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_refsets;  
        tag_t  
            object;  
        while ((object = select_an_object("Report Reference Sets")) != NULL_TAG)  
        {  
            SHOW_NAME(object);  
            WRITE_D(n_refsets = UF_ASSEM_count_ref_sets_in(object));  
            for (ii = 1; ii <= n_refsets; ii++)  
            {  
                WRITE_D(ii);  
                SHOW_NAME(UF_ASSEM_get_ref_set_inst(object, ii));  
            }  
            WRITE("\n");  
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

> 这段代码是用于NX的二次开发，其功能是选择一个对象，并报告该对象包含的引用集。
>
> 主要步骤包括：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了选择对象函数select_an_object，使用UF_UI_select_with_single_dialog选择对象并返回其tag。
> 3. 定义了写入信息到列表窗口的宏WRITE和WRITE_D。
> 4. 定义了报告对象名称的函数report_object_name。
> 5. 定义了主体函数do_it，循环选择对象，调用UF_ASSEM_count_ref_sets_in和UF_ASSEM_get_ref_set_inst查询对象包含的引用集数量和名称，并输出到列表窗口。
> 6. 定义了ufusr函数，初始化NX环境后调用do_it函数，然后终止NX环境。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码通过二次开发实现了选择对象并查询和输出其包含的引用集信息的功能。
>
