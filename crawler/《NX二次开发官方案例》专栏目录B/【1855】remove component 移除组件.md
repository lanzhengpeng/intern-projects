### 【1855】remove component 移除组件

#### 代码

```cpp
    /*HEAD REMOVE_COMPONENT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本中新增的函数。在翻译时，只需回答UF_print_syslog是V18版本新增的即可，不需要添加其他无关内容。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_components, NULL, &resp,  
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
            comp;  
        while ((comp = select_a_component("Remove Component")) != NULL_TAG)  
            UF_CALL(UF_ASSEM_remove_instance(UF_ASSEM_ask_inst_of_part_occ(comp)));  
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

> 这段代码是一个NX的二次开发程序，主要用于移除组件。下面是代码的主要功能：
>
> 1. 错误处理：使用宏UF_CALL来包装NX API调用，并在发生错误时输出错误信息。
> 2. 选择组件：mask_for_components函数用于设置选择组件的类型掩码，select_a_component函数用于通过对话框选择一个组件。
> 3. 移除组件：do_it函数通过循环调用select_a_component选择组件，然后使用UF_ASSEM_remove_instance函数移除组件实例。
> 4. 初始化和退出：ufusr函数用于程序的初始化和执行，ufusr_ask_unload用于设置程序卸载时的行为。
> 5. 日志输出：使用了UF_print_syslog函数来输出日志，该函数在NX 18版本中新增。
> 6. 选择组件对话框：使用了NX的选择对话框来让用户选择要移除的组件。
> 7. 组件移除：通过UF_ASSEM_remove_instance函数来移除组件实例。
> 8. 错误提示：使用了自定义的错误报告函数report_error，并在错误发生时输出详细的错误信息。
> 9. 循环移除：程序会循环执行，直到用户没有选择组件为止。
>
> 这段代码实现了在NX环境下通过对话框选择并移除组件的功能。
>
