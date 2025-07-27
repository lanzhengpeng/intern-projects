### 【1874】rename instance Rename Instance

#### 代码

```cpp
    /*HEAD RENAME_INSTANCE CCC UFUN */  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
        tag_t   proto, comp, inst_tag;  
        char    part_name[MAX_FSPEC_SIZE+1], name[30];  
        comp = select_a_component( "Select component to rename");  
        proto = UF_ASSEM_ask_prototype_of_occ( comp);  
        UF_PART_ask_part_name( proto, part_name);  
        UF_CALL( uc4574( part_name, 2, name));  
        inst_tag = UF_ASSEM_ask_inst_of_part_occ( comp);  
        UF_CALL( UF_ASSEM_rename_instance( inst_tag, name));  
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

> 这段代码是用于NX的二次开发，主要实现以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义了一个组件选择函数mask_for_components，用于设置组件选择对话框的过滤条件，只允许选择组件类型的对象。
> 3. 定义了一个组件选择函数select_a_component，用于显示一个对话框，让用户选择一个组件，并返回其tag。
> 4. 定义了一个函数do_it，用于实现重命名组件实例的功能：调用select_a_component让用户选择一个组件。获取该组件的原始模型tag。获取原始模型的名称。生成一个新的实例名称。获取该组件的实例tag。调用UF_ASSEM_rename_instance函数重命名实例。
> 5. 调用select_a_component让用户选择一个组件。
> 6. 获取该组件的原始模型tag。
> 7. 获取原始模型的名称。
> 8. 生成一个新的实例名称。
> 9. 获取该组件的实例tag。
> 10. 调用UF_ASSEM_rename_instance函数重命名实例。
> 11. 定义了ufusr函数，用于初始化NX系统，调用do_it函数实现重命名，然后终止NX系统。
> 12. 定义了ufusr_ask_unload函数，用于在二次开发应用程序卸载时调用。
>
> 总的来说，这段代码的主要功能是实现NX组件实例的重命名。
>
