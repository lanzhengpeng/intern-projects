### 【2830】set wcs by selected component 根据选择的组件设置WCS

#### 代码

```cpp
    /*HEAD SET_WCS_BY_SELECTED_COMPONENT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您的要求，关于UF_print_syslog，我翻译如下：

UF_print_syslog是V18版本中新增的功能，用于打印系统日志信息。 */  
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
            mx,  
            wcs;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        while ((comp = select_a_component("Set WCS")) != NULL_TAG)  
        {  
            UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
                instance_name, origin, csys_matrix, transform));  
            UF_CALL(UF_CSYS_create_matrix(csys_matrix, &mx));  
            UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
            UF_CALL(UF_CSYS_set_wcs(wcs));  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 设置组件坐标系为工作坐标系(WCS)。用户可以在装配体中选择一个组件，代码会获取该组件的坐标系，并将其设置为当前的工作坐标系。
> 2. 代码使用了UF框架的API，实现了错误处理、组件选择、坐标系获取和设置等功能。
> 3. 错误处理部分通过宏定义UF_CALL封装了UF函数调用，实现了错误码的捕获和打印。
> 4. 组件选择部分实现了组件类型的选择过滤，并通过对话框让用户选择一个组件。
> 5. 获取组件坐标系部分调用了UF_ASSEM_ask_component_data获取组件的坐标系信息。
> 6. 设置坐标系部分调用了UF_CSYS_create_matrix、UF_CSYS_create_temp_csys和UF_CSYS_set_wcs来实现临时坐标系创建和工作坐标系设置。
> 7. 主函数ufusr实现了初始化、调用主要逻辑和终止。
> 8. ask_unload函数实现了立即卸载。
>
> 总体来说，这段代码实现了在NX中通过选择组件来设置工作坐标系的二次开发功能，利用了UF框架的API，并包含了错误处理、组件选择、坐标系获取和设置等完整流程。
>
