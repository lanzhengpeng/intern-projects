### 【0646】create component array 创建组件阵列

#### 代码

```cpp
    /*HEAD CREATE_COMPONENT_ARRAY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中的新功能，用于打印系统日志消息。 */  
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
    /* 里海 */  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_line(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a line", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
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
            array;  
        char  
            name[31] = { "Example_Array" };  
        UF_ASSEM_mc_array_data_t  
            array_data;  
        array_data.array_subtype = UF_ASSEM_linear_array;  
        array_data.master_component = NULL_TAG;  
        array_data.array_name = name;  
        UF_CALL(UF_MODL_create_exp_tag("2", &array_data.dimensions[0]));  
        UF_CALL(UF_MODL_create_exp_tag("3", &array_data.dimensions[1]));  
        UF_CALL(UF_MODL_create_exp_tag("20", &array_data.offsets[0]));  
        UF_CALL(UF_MODL_create_exp_tag("10", &array_data.offsets[1]));  
        while (((array_data.template_component =  
                 select_a_component("Select component for array")) != NULL_TAG)  
            && ((array_data.axis_definitions[0] =  
                 select_a_line("Select x axis")) != NULL_TAG)  
            && ((array_data.axis_definitions[1] =  
                 select_a_line("Select y axis")) != NULL_TAG))  
            UF_CALL(UF_ASSEM_create_mc_array(&array_data, &array));  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 错误报告函数：该函数用于在调用NX API函数出现错误时打印错误信息，包括错误码、错误消息、出错的函数调用等，方便调试。
> 2. 组件选择遮罩：用于设置组件选择的遮罩，只允许选择组件类型的对象。
> 3. 选择组件：通过对话框让用户选择一个组件，并返回该组件的tag。
> 4. 线条选择遮罩：用于设置线条选择的遮罩，只允许选择线类型或实体边类型的对象。
> 5. 选择线条：通过对话框让用户选择一个线条，并返回该线条的tag。
> 6. 创建组件阵列：这是主函数，负责创建一个线性阵列。首先创建阵列数据结构，设置阵列名称、子类型为线性阵列、主组件为空。然后通过对话框让用户选择阵列的模板组件、x轴和y轴。最后调用UF_ASSEM_create_mc_array创建阵列，直到用户取消选择。
> 7. ufusr函数：这是NX二次开发的入口函数，在NX启动时调用。在这里初始化NX环境，调用主函数，然后终止NX环境。
> 8. 卸载函数：返回立即卸载标志，表示该二次开发模块可以立即卸载。
>
> 总的来说，这段代码通过NX提供的API，实现了线性阵列的创建功能。代码结构清晰，功能明确，是一个典型的NX二次开发示例。
>
