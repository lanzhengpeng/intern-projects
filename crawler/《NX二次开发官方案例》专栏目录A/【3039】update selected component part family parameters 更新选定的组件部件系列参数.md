### 【3039】update selected component part family parameters 更新选定的组件部件系列参数

#### 代码

```cpp
    /*HEAD UPDATE_SELECTED_COMPONENT_PART_FAMILY_PARAMETERS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define ECHOL(X)   (X ? printf("%s = TRUE\n", #X) : printf("%s = FALSE\n", #X))  
    #define ECHOS(X)   (printf("%s = %s\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数。

仅回答译文，不要添加任何废话。 */  
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
    static void report_instance_status(UF_ASSEM_instance_status_t *instance_status)  
    {  
        printf("\n");  
        ECHOL(instance_status->child_changed);  
        ECHO(instance_status->current_child);  
        ECHO(instance_status->previous_child);  
        ECHOS(instance_status->info);  
        if (instance_status->info != NULL) UF_free(instance_status->info);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            comp,  
            inst;  
        UF_ASSEM_instance_status_t  
            instance_status;  
        while ((comp=select_a_component("Select component to update")) != NULL_TAG)  
        {  
            inst = UF_ASSEM_ask_inst_of_part_occ(comp);  
        /*  Component->Component Properties->Parameters->Part Family->Information 里海译:组件 -> 组件属性 -> 参数 -> 部件系列 -> 信息 */  
            UF_CALL(UF_ASSEM_eval_instance_intent(&inst, FALSE, &instance_status));  
            report_instance_status(&instance_status);  
        /*  Component->Component Properties->Parameters->Part Family->Update 里海译:Component -> 组件属性 -> 参数 -> 部件家族 -> 更新 */  
            UF_CALL(UF_ASSEM_eval_instance_intent(&inst, TRUE, &instance_status));  
            report_instance_status(&instance_status);  
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

> 这段代码是NX的二次开发代码，其主要功能是更新组件的部件系列参数。以下是代码的主要功能介绍：
>
> 1. 包含必要的NX API头文件，并定义了错误报告函数report_error，用于打印出函数调用失败的信息。
> 2. 定义了mask_for_components函数，用于设置组件选择器的选择掩码，仅允许选择组件类型的对象。
> 3. 定义了select_a_component函数，用于弹出对话框让用户选择一个组件。
> 4. 定义了report_instance_status函数，用于打印组件实例的状态信息，包括是否更改、当前和上一个子组件等。
> 5. 定义了do_it函数，循环调用select_a_component获取用户选择的组件，然后查询组件的实例信息，并打印出来。接着更新实例的意图，再次打印实例信息。
> 6. ufusr是二次开发程序的入口函数，调用UF_initialize初始化NX，调用do_it执行更新操作，最后调用UF_terminate终止NX。
> 7. ufusr_ask_unload函数用于设置程序卸载时的行为。
>
> 总的来说，这段代码通过交互式选择组件，并更新其部件系列参数，实现了对NX组件参数的管理。
>
