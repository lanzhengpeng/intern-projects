### 【2851】set work part 设置工作部件

#### 代码

```cpp
    /*HEAD SET_WORK_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 新增的功能，用于打印系统日志。 */  
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
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
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
            comp,  
            comp_part;  
        while ((comp = select_a_component("Select new work part")) != NULL_TAG)  
        {  
            comp_part = UF_ASSEM_ask_prototype_of_occ(comp);  
            UF_CALL(UF_ASSEM_set_work_part(comp_part));  
            uc1601("Used UF_ASSEM_set_work_part", TRUE);  
            UF_CALL(UF_ASSEM_set_work_occurrence(comp));  
            uc1601("Used UF_ASSEM_set_work_occurrence", TRUE);  
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

> 根据代码，这是一段NX二次开发的UF函数代码，主要实现以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印UF函数调用失败时的错误信息。
> 2. 定义了一个组件选择掩码函数mask_for_components，用于在组件选择对话框中只显示组件类型的对象。
> 3. 定义了一个组件选择函数select_a_component，用于弹出一个对话框，让用户选择一个组件对象。
> 4. do_it函数实现了循环选择组件，并设置工作组件和工作组件实例的功能。
> 5. ufusr函数是UF程序的入口函数，初始化后调用do_it函数，然后终止。
> 6. ufusr_ask_unload函数返回立即卸载，表示程序不需要持久化状态。
>
> 综合来看，这段代码实现了选择组件并设置工作组件的功能，使用户可以方便地在NX中切换当前工作组件。
>
