### 【2731】set assembly layers like selected component 设置装配图层的显示状态，使其与所选组件相同

#### 代码

```cpp
    /*HEAD SET_ASSEMBLY_LAYERS_LIKE_SELECTED_COMPONENT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_layer.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog 是一个在 V18 版本中新增的函数，其功能是打印系统日志。该函数可用于将系统日志信息打印到日志文件中，以便进行调试和故障排查。 */  
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
    static void set_assembly_layers_like_component( tag_t assy_part, tag_t comp_part )  
    {  
        int  
            inx = 1,  
            layer_status[257];  
        for( inx = 1; inx < 257; inx++ )  
        {  
            UF_CALL( UF_PART_set_display_part( comp_part ));  
            UF_CALL( UF_LAYER_ask_status ( inx, &layer_status[inx] ));  
        }  
            UF_CALL( UF_PART_set_display_part( assy_part ));  
        for( inx = 1; inx < 257; inx++ )  
        {  
            UF_CALL( UF_LAYER_set_status( inx , layer_status[inx] ));  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            tmp = NULL_TAG,  
            comp_part = NULL_TAG,  
            assy_part = UF_PART_ask_display_part();  
        if( (tmp = select_a_component( "Select a Component:" )) != NULL_TAG )  
        {  
            comp_part = UF_ASSEM_ask_prototype_of_occ( tmp );  
            set_assembly_layers_like_component( assy_part, comp_part );  
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

> 这段代码是NX的二次开发代码，主要实现以下功能：
>
> 1. 设置一个错误报告函数report_error，用于在函数执行出错时输出错误信息。
> 2. 定义一个组件选择掩码mask_for_components，用于在选择组件时只选择组件类型的对象。
> 3. 定义一个选择组件的函数select_a_component，用于让用户选择一个组件，并返回组件的tag。
> 4. 定义一个设置装配体层状态与组件相同的函数set_assembly_layers_like_component。
> 5. 在do_it函数中，先选择一个组件，然后获取该组件的原型，最后调用set_assembly_layers_like_component函数设置装配体的层状态与该组件相同。
> 6. ufusr函数是NX二次开发的入口函数，调用do_it函数实现上述功能。
> 7. ufusr_ask_unload函数用于卸载NX插件，返回立即卸载。
>
> 通过以上函数，该代码实现了选择一个组件，然后将装配体的层状态设置为与该组件相同的NX二次开发功能。
>
