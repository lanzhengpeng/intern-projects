### 【3040】upgrade instances 将组件升级为实例

#### 代码

```cpp
    /*HEAD UPGRADE_INSTANCES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，V18版本中新增了UF_print_syslog函数，用于打印系统日志。 */  
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
    /* qq3123197280 */  
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
    static int select_components(char *prompt, tag_t **components)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select components", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &cnt, components));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*components)[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *comps;  
        int  
            n_comps = 0;  
        UF_ASSEM_upgrade_status_t  
            status;  
        if ((n_comps = select_components("Select components", &comps)) > 0)  
        {  
            UF_ASSEM_upgrade_to_instances(part, n_comps, comps, TRUE, TRUE, &status);  
        }  
        if(status.failed)  
        {  
            UF_free(status.statuses);  
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

> 这段代码是NX的二次开发代码，主要实现了零件升级为实例的功能。
>
> 1. 包含了必要的NX API头文件。
> 2. 定义了错误报告函数report_error，用于打印错误信息。
> 3. 定义了选择组件的函数select_components，用于打开选择对话框，让用户选择组件。
> 4. 定义了执行升级的函数do_it，首先获取当前显示的零件，然后调用select_components函数选择组件，最后调用UF_ASSEM_upgrade_to_instances函数将选中的组件升级为实例。
> 5. 定义了ufusr函数，这是NX二次开发的入口函数，初始化NX后调用do_it函数执行升级操作，然后结束NX。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发模块不需要延迟卸载。
> 7. 整体来看，这段代码实现了零件升级为实例的功能，用户可以通过选择组件，将这些组件升级为实例。
>
