### 【0654】create csys at absolute of selected component 创建选定组件的绝对坐标系

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_trns.h>  
    #include <uf_csys.h>  
    #include <uf_obj.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
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
    static tag_t create_alive_normal_abs_csys(void)  
    {  
        tag_t  
            matrix,  
            new_csys;  
        double  
            origin[3] = { 0,0,0 },  
            values[9] = { 1,0,0, 0,1,0, 0,0,1 };  
        UF_CALL(UF_CSYS_create_matrix(values, &matrix));  
        UF_CALL(UF_CSYS_create_csys(origin, matrix, &new_csys));  
        return new_csys;  
    }  
    static void ask_component_transform_matrix(tag_t component, double *transform)  
    {  
        double  
            csys_matrix[9],  
            origin[3];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            comp,  
            newCsys;  
        char  
            csysName[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        double  
            mx[16];  
        while ((comp = select_a_component("Create ABS csys")) != NULL_TAG)  
        {  
            newCsys = create_alive_normal_abs_csys();  
            ask_component_transform_matrix(comp, mx);  
            UF_CALL(UF_MODL_transform_entities(1, &newCsys, mx));  
            UF_CALL(UF_DISP_add_item_to_display(newCsys)); // redisplay  
            UF_CALL(UF_DISP_set_highlight(newCsys, TRUE));  
            uc1601("Here", TRUE);  
            UF_CALL(UF_DISP_set_highlight(newCsys, FALSE));  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX二次开发示例，主要功能是在NX中为选择的组件创建一个绝对坐标系（ABS csys）。具体步骤如下：
>
> 1. 包含了UF相关的头文件。
> 2. 定义了ECHO和report_error两个宏，用于打印错误信息。
> 3. 定义了mask_for_components函数，用于设置选择组件的类型过滤条件。
> 4. 定义了select_a_component函数，用于让用户选择一个组件，并返回组件的tag。
> 5. 定义了create_alive_normal_abs_csys函数，用于创建一个绝对坐标系。
> 6. 定义了ask_component_transform_matrix函数，用于获取组件的变换矩阵。
> 7. 定义了do_it函数，其中循环调用select_a_component，为每个组件创建一个绝对坐标系，并设置高亮显示。
> 8. ufusr是NX二次开发程序的入口函数，初始化后调用do_it函数，然后终止。
> 9. ufusr_ask_unload用于处理程序卸载。
>
> 整体上，这段代码通过交互式选择组件，并创建对应的绝对坐标系，以实现组件坐标系的定位和显示。
>
