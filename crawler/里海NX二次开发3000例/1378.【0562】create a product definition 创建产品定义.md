### 【0562】create a product definition 创建产品定义

#### 代码

```cpp
    /*HEAD CREATE_A_PRODUCT_DEFINITION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_pd.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据原文，UF_print_syslog是V18版本新增的功能。 */  
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
    static void do_it(void)  
    {  
        int num_attrs = 0;  
        tag_t  
            *attrs,  
            pd;  
        UF_PD_product_definition_t  
            product_definition_data;  
        UF_PD_product_attribute_t  
            attribute_data;  
        UF_PD_attribute_value_t  
            attribute_values[3];  
        product_definition_data.name = "General Drawing Note 2";  
        product_definition_data.num_geometry_references = 0;  
        product_definition_data.retained = TRUE;  
        product_definition_data.num_product_attributes = 1;  
        product_definition_data.product_attributes = &attribute_data;  
        attribute_data.attribute_type = UF_PD_GENERIC_NOTE_TYPE;  
        attribute_data.attribute_group = "Fabrication";  
        attribute_data.attribute_type_name = "General Note";  
        attribute_data.attribute_name = "Drawing Note Text";  
        attribute_data.symbolic_display = "smart_model:generic_note";  
        attribute_data.kf_name = "ug_general_note_attribute";  
        attribute_data.number_busmods = 0;  
        attribute_data.num_attribute_values = 0;  
        attribute_data.values = NULL;  
        attribute_data.num_attribute_value_dats = 0;  
        attribute_data.value_dats = NULL;  
    /*  create product definition with default values from dfa file 译:根据dfa文件创建具有默认值的产品定义 */  
        UF_CALL(UF_PD_create_product_definition(&product_definition_data, &pd));  
    /*  ask the attributes so they can be modified 译:询问属性以便可以修改。 */  
        UF_CALL(UF_PD_ask_product_attribues(pd, &attrs, &num_attrs));  
    /*  set the values to be modified 译:设置要修改的值 */  
        attribute_data.num_attribute_values = 3;  
        attribute_data.values = attribute_values;  
        attribute_values[0].value_type = UF_PD_INTEGER_VALUE;  
        attribute_values[0].value_title = "Category";  
        attribute_values[0].integer_value = 2; /* "Drawing Notes" 译:绘图笔记 */  
        attribute_values[1].value_type = UF_PD_INTEGER_VALUE;  
        attribute_values[1].value_title = "Identifier";  
        attribute_values[1].integer_value = 2; /* "Notes on Sheet 3"; 译:翻译"Notes on Sheet 3"为“第3页注释”。 */  
        attribute_values[2].value_type = UF_PD_STRING_VALUE;  
        attribute_values[2].value_title = "Revision";  
        attribute_values[2].string_value = "1";  
        UF_CALL(UF_PD_modify_product_attribute(attrs[0], &attribute_data));  
        UF_CALL(UF_MODL_update());  
        UF_free(attrs);  
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

> 这段代码是一个NX的二次开发示例，其主要功能是创建一个产品定义并设置其属性。代码的主要步骤如下：
>
> 1. 包含必要的NX开发头文件。
> 2. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. do_it函数实现了创建产品定义的主要逻辑：定义了一个product_definition_data结构，设置产品定义的基本信息，如名称为"General Drawing Note 2"。定义了一个attribute_data结构，设置产品定义的属性信息，如属性类型为"General Note"，属性组为"Fabrication"等。调用UF_PD_create_product_definition函数创建产品定义。调用UF_PD_ask_product_attribues询问产品定义的属性。设置要修改的属性值，包括整数类型和字符串类型。调用UF_PD_modify_product_attribute修改产品定义的属性值。调用UF_MODL_update更新模型。
> 4. 定义了一个product_definition_data结构，设置产品定义的基本信息，如名称为"General Drawing Note 2"。
> 5. 定义了一个attribute_data结构，设置产品定义的属性信息，如属性类型为"General Note"，属性组为"Fabrication"等。
> 6. 调用UF_PD_create_product_definition函数创建产品定义。
> 7. 调用UF_PD_ask_product_attribues询问产品定义的属性。
> 8. 设置要修改的属性值，包括整数类型和字符串类型。
> 9. 调用UF_PD_modify_product_attribute修改产品定义的属性值。
> 10. 调用UF_MODL_update更新模型。
> 11. ufusr函数是NX二次开发的入口函数，在这里调用do_it函数完成产品定义的创建和属性设置。
> 12. ufusr_ask_unload函数用于卸载用户自定义函数。
>
> 总体来说，这段代码演示了如何使用NX提供的API创建一个产品定义，并设置其属性，是NX二次开发的一个基本示例。
>
