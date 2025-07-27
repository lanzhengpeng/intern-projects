### apply structures load to selected face 在选定的面上应用结构载荷

#### 代码

```cpp
    /*HEAD APPLY_STRUCTURES_LOAD_TO_SELECTED_FACE CCC UFUN */  
    /*Remarks:  
    1. Execute this program in NX structures application  
        while a scenario part is the work part.  
    2. This program was tested by GTAC in NX 1.0.3.2 and NX 2.0.3.1.*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
    #include <uf_sf_ugs_mesh.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog在V18中是新增的，请只回答翻译，不要添加无关内容。 */  
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
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        char  
            lbc_name[20]={"Load Group 1"};  
        double  
            load_value[3] = {333.0, 333.0, 333.0};  
        tag_t  
            obj_id,  
            lbc_case,  
            load_tag;  
        UF_SF_load_mode_t  
            load_type=UF_SF_LOAD_FORCE;  
        UF_SF_LBC_component_type_t  
            component_type=UF_SF_LBC_XYZ;  
        if (UF_CALL(UF_SF__init_module())) return;  
        UF_CALL(UF_SF_create_load_case (lbc_name, &lbc_case));  
        while ((obj_id = select_a_face("Select a face")) != NULL_TAG)  
        {  
            UF_CALL(UF_SF_create_load (load_type, obj_id, obj_id, NULL_TAG,  
                lbc_case, component_type, load_value, &load_tag));  
            UF_CALL(UF_PART_save());  
            uc1601("In Scenario Navigator: double-click on master model and then scenario part.", TRUE);  
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

> 这是一段用于NX结构分析应用的二次开发代码，其主要功能是向选定的面加载结构载荷。具体步骤如下：
>
> 1. 初始化NX结构和有限元模块。
> 2. 创建一个载荷案例，并指定名称为"Load Group 1"。
> 3. 循环提示用户选择一个面，直到用户选择“确定”。
> 4. 对选定的每个面，创建一个结构载荷，并指定载荷类型为力，载荷大小为(333,333,333)。
> 5. 将载荷添加到之前创建的载荷案例中。
> 6. 保存当前部件。
> 7. 提示用户在场景导航器中双击主模型和场景部件。
> 8. 退出结构模块。
> 9. 安全卸载二次开发代码。
>
> 该代码使用UF开发接口，通过用户交互选择面，并加载结构载荷，实现了对NX结构分析进行自定义的功能。
>
