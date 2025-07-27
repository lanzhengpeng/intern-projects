### 【0537】copy named coordinate system from component to assembly 将组件中的命名坐标系复制到装配体中

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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static tag_t ask_next_of_name_and_type(tag_t part, char *name, int type,  
        tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, type, TRUE, &object));  
        return object;  
    }  
    static tag_t create_duplicate_alive_normal_csys(tag_t csys)  
    {  
        tag_t  
            matrix,  
            new_csys;  
        double  
            origin[3],  
            values[9];  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, values));  
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
            csys,  
            newCsys;  
        char  
            csysName[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        double  
            mx[16];  
        while (((comp = select_a_component("Copy csys")) != NULL_TAG) &&  
            prompt_for_text("Csys to Copy", csysName))  
        {  
            csys = ask_next_of_name_and_type(UF_ASSEM_ask_prototype_of_occ(comp),  
                csysName, UF_coordinate_system_type, NULL_TAG);  
            if (csys == NULL_TAG)  
            {  
                ECHO("csys named %s not found\n", csysName);  
                continue;  
            }  
            newCsys = create_duplicate_alive_normal_csys(csys);  
            ask_component_transform_matrix(comp, mx);  
            UF_CALL(UF_MODL_transform_entities(1, &newCsys, mx));  
            UF_CALL(UF_OBJ_set_name(newCsys, csysName));  
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

> 这段代码是一个NX Open C++的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个ECHO函数，用于在列表窗口和系统日志中打印信息。
> 2. 定义了一个report_error函数，用于在函数调用失败时打印错误信息。
> 3. 定义了一个mask_for_components函数，用于在组件选择对话框中仅显示组件类型。
> 4. 定义了一个select_a_component函数，用于打开组件选择对话框，让用户选择一个组件。
> 5. 定义了一个prompt_for_text函数，用于提示用户输入文本。
> 6. 定义了一个ask_next_of_name_and_type函数，用于遍历组件下指定名称和类型的对象。
> 7. 定义了一个create_duplicate_alive_normal_csys函数，用于复制一个活动的坐标系。
> 8. 定义了一个ask_component_transform_matrix函数，用于获取组件的变换矩阵。
> 9. 定义了一个do_it函数，用于遍历选择组件，复制坐标系，并应用组件变换。
> 10. 定义了ufusr函数，作为程序的入口，初始化NX环境，执行do_it函数，然后终止NX环境。
> 11. 定义了一个ufusr_ask_unload函数，用于卸载动态库。
>
> 该代码实现了选择组件、复制坐标系并应用组件变换的功能，为NX的二次开发提供了一个有价值的参考。
>
