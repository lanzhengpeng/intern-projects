### 【0763】create material 创建一个新的材料，并为其分配属性

#### 代码

```cpp
    /*HEAD CREATE_MATERIAL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，可以总结如下：

UF_print_syslog 是 V18 版本中新增的函数。它用于将日志信息输出到系统日志。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* 里海 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_bodies, NULL, &resp,  
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
        int  
            units;  
        tag_t  
            body,  
            material,  
            new_density,  
            new_poisson,  
            new_youngs,  
            part = UF_PART_ask_display_part();  
        UF_SF_material_prop_t  
            property_values;  
        if (UF_CALL(UF_PART_ask_units(part, &units))) return;  
        if ((UF_SF_locate_named_material("MYSTEEL", &material)) ||  
            (material == NULL_TAG))  
        {  
            if( units == UF_PART_METRIC )  
            {  
                UF_CALL(UF_MODL_create_exp_tag("206940000", &new_youngs));  
                UF_CALL(UF_MODL_create_exp_tag( "0.288", &new_poisson));  
                UF_CALL(UF_MODL_create_exp_tag("7.829e-006", &new_density));  
            }  
            else  
            {  
                UF_CALL(UF_MODL_create_exp_tag("30000091.8", &new_youngs));  
                UF_CALL(UF_MODL_create_exp_tag("0.288", &new_poisson));  
                UF_CALL(UF_MODL_create_exp_tag("0.000731", &new_density));  
            }  
            UF_CALL(UF_SF_init_matl_prop( &property_values));  
            property_values.isotropic.youngs_modulus.type =  
                UF_SF_MULTI_PROP_EXPRESSION;  
            property_values.isotropic.youngs_modulus.multi.expression_tag =  
                new_youngs;  
            property_values.isotropic.poissons_ratio.type =  
                UF_SF_MULTI_PROP_EXPRESSION;  
            property_values.isotropic.poissons_ratio.multi.expression_tag =  
                new_poisson;  
            property_values.isotropic.mass_density.type =  
                UF_SF_MULTI_PROP_EXPRESSION;  
            property_values.isotropic.mass_density.multi.expression_tag =  
                new_density;  
            UF_CALL(UF_SF_create_material ("mySteel", "myMETAL",  
                UF_SF_MATERIAL_ISOTROPIC, &property_values, UF_SF_MATL_IS_EDITABLE,  
                &material));  
            UF_SF_free_matl_prop(&property_values);  
        }  
        WRITE_D(material);  
        while ((body = select_a_body("Apply mySteel material")) != NULL_TAG)  
            UF_CALL(UF_SF_link_material(material, body));  
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

> 这段NX二次开发代码的主要功能是创建一个新的材料属性并应用到选中的零件上。以下是代码的主要逻辑：
>
> 1. 初始化NX环境：使用UF_initialize()函数进行NX环境的初始化。
> 2. 创建新材料属性：首先检查材料"MYSTEEL"是否存在，如果不存在则创建。根据当前单位制创建杨氏模量、泊松比和密度三个属性的标签。初始化材料属性结构体，并将这三个属性赋值给结构体。调用UF_SF_create_material()函数创建新材料"mySteel"。
> 3. 首先检查材料"MYSTEEL"是否存在，如果不存在则创建。
> 4. 根据当前单位制创建杨氏模量、泊松比和密度三个属性的标签。
> 5. 初始化材料属性结构体，并将这三个属性赋值给结构体。
> 6. 调用UF_SF_create_material()函数创建新材料"mySteel"。
> 7. 选择零件并应用材料：定义一个选择零件的函数select_a_body()，提示用户选择一个零件，并返回零件的标签。在一个循环中调用select_a_body()，获取用户选择的每个零件，并调用UF_SF_link_material()函数将材料"mySteel"应用到该零件上。
> 8. 定义一个选择零件的函数select_a_body()，提示用户选择一个零件，并返回零件的标签。
> 9. 在一个循环中调用select_a_body()，获取用户选择的每个零件，并调用UF_SF_link_material()函数将材料"mySteel"应用到该零件上。
> 10. 结束NX环境：使用UF_terminate()函数结束NX环境。
> 11. 卸载函数：定义了一个卸载函数ufusr_ask_unload()，返回立即卸载。
> 12. 错误处理：定义了一个错误处理函数report_error()，用于打印错误信息。在关键函数调用后使用宏UF_CALL()进行错误检查，并调用report_error()函数。
> 13. 定义了一个错误处理函数report_error()，用于打印错误信息。
> 14. 在关键函数调用后使用宏UF_CALL()进行错误检查，并调用report_error()函数。
> 15. 日志函数：定义了一个日志函数write_integer_to_listing_window()，用于在日志窗口中输出整数值。
>
> 通过以上主要功能，这段代码实现了创建新材料属性并应用到零件上的功能。
>
