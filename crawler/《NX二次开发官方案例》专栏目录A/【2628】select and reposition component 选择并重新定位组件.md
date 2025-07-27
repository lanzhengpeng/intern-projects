### 【2628】select and reposition component 选择并重新定位组件

#### 代码

```cpp
    /*HEAD SELECT_AND_REPOSITION_COMPONENT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_vec.h>  
    #include <uf_assem.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中新增的函数。文档只要求翻译，不提供任何额外信息。 */  
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
    /* qq3123197280 */  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static void translate_component(tag_t comp, double vec[3])  
    {  
        tag_t  
            instance;  
        double  
            instance_origin[3],  
            instance_matrix[9],  
            instance_transform[4][4];  
        char  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1],  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE];  
        instance = UF_ASSEM_ask_inst_of_part_occ(comp);  
        UF_CALL( UF_ASSEM_ask_component_data( instance, part_name, refset_name,  
            instance_name, instance_origin, instance_matrix, instance_transform));  
        UF_VEC3_add(instance_origin, vec, instance_origin);  
        UF_CALL( UF_ASSEM_reposition_instance(instance, instance_origin,  
            instance_matrix));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            my_comp = NULL_TAG;  
        double  
            delta[3] = { 10., 15., 20. };  
        while((my_comp = select_a_component( "Pick a Component:" )) != NULL_TAG )  
        {  
            translate_component( my_comp, delta );  
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

> 这段代码是用于NX的二次开发，其主要功能是实现以下目的：
>
> 1. 定义错误处理函数report_error，用于输出错误信息。
> 2. 定义组件选择函数mask_for_components，用于设置选择组件的类型。
> 3. 定义组件选择函数select_a_component，用于提示用户选择组件。
> 4. 定义组件平移函数translate_component，用于获取组件的实例数据并平移组件。
> 5. 主函数do_it用于循环选择组件并平移组件。
> 6. ufusr函数用于初始化、调用主函数、终止NX。
> 7. ufusr_ask_unload函数用于立即卸载NX。
>
> 整体来看，该代码实现了在NX中循环选择组件，并将每个选中的组件平移一段距离的功能。
>
