### 【0092】apply boundary condition to selected face 将边界条件应用于选定的面

#### 代码

```cpp
    /*HEAD APPLY_BOUNDARY_CONDITION_TO_SELECTED_FACE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog 是 V18 版本中新增的函数。 */  
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
        double  
            bc_value[6] = {0.0,0.0,0.0,0.0,0.0,0.0};  
        tag_t  
            obj_id,  
            bc_tag;  
        UF_SF_bc_mode_t  
            bc_type=UF_SF_BC_CONSTRAINT;  
        UF_SF_LBC_component_type_t  
            compo=UF_SF_LBC_XYZ;  
        if (UF_CALL(UF_SF__init_module())) return;  
        while ((obj_id = select_a_face("Select a face")) != NULL_TAG)  
        {  
            UF_CALL(UF_SF_create_bc (bc_type, obj_id, obj_id, NULL_TAG,  
                NULL_TAG, compo, bc_value, &bc_tag));  
            UF_CALL(UF_PART_save());  
            uc1601("In Scenario Navigator: double click on master model and then scenario part.", TRUE);  
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

> 这段代码是NX的二次开发代码，主要功能是在NX结构分析应用中，给选中的面添加边界条件。代码主要包括以下几个部分：
>
> 1. 包含头文件，声明了UF、UF_UI、UF_SF、UF_SF_UGS_MESH等库函数。
> 2. 定义了一个错误报告函数report_error，用于打印错误信息。
> 3. 定义了一个面选择函数mask_for_faces，用于在选择对话框中设置选择类型为实体和面。
> 4. 定义了一个选择面的函数select_a_face，用于提示用户选择一个面，并返回选中面的标识。
> 5. 定义了主函数do_it，用于初始化边界条件模块，循环调用select_a_face选择面，然后为选中的面创建边界条件，并保存模型。
> 6. 定义了ufusr函数，是NX二次开发的入口函数，调用主函数do_it。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载。
>
> 总的来说，这段代码实现了在NX结构分析应用中，给选中的面添加边界条件的功能，通过选择对话框获取用户选择的面对象标识，然后调用UF_SF库函数为该面对象创建边界条件。
>
