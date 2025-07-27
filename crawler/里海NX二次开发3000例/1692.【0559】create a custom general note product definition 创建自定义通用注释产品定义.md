### 【0559】create a custom general note product definition 创建自定义通用注释产品定义

#### 代码

```cpp
    /*HEAD CREATE_A_CUSTOM_GENERAL_NOTE_PRODUCT_DEFINITION CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译如下：

注意：UF_print_syslog在V18中是新增的，请只提供译文，不要添加无关内容。 */  
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
            pd,  
            *attrs;  
        char  
            *strings[3] = { "Custom text line 1",  
                            "Custom text line 2",  
                            "Custom text line 3" };  
        UF_PD_product_definition_t  
            product_definition_data;  
        UF_PD_product_attribute_t  
            attribute_data;  
        UF_PD_attribute_value_t  
            attribute_values[3];  
        UF_PD_attribute_value_data_t  
            attribute_value_data;  
        product_definition_data.name = "Custom General Note 2";  
        product_definition_data.num_geometry_references = 0;  
        product_definition_data.retained = TRUE;  
        product_definition_data.num_product_attributes = 1;  
        product_definition_data.product_attributes = &attribute_data;  
        attribute_data.attribute_type = UF_PD_GENERIC_NOTE_TYPE;  
        attribute_data.attribute_group = "Fabrication";  
        attribute_data.attribute_type_name = "General Note";  
        attribute_data.attribute_name = "Customer Note Text";  
        attribute_data.symbolic_display = "smart_model:generic_note";  
        attribute_data.kf_name = "ug_general_note_attribute";  
        attribute_data.number_busmods = 0;  
        attribute_data.num_attribute_values = 0;  
        attribute_data.values = NULL;  
        attribute_data.num_attribute_value_dats = 0;  
        attribute_data.value_dats = NULL;  
        /* create product definition 译:创建产品定义 */  
        UF_CALL(UF_PD_create_product_definition(&product_definition_data, &pd));  
        /* ask the attributes so they can be modified 译:请求属性以进行修改 */  
        if (UF_CALL(UF_PD_ask_product_attribues(pd, &attrs, &num_attrs)) ||  
            (num_attrs == 0)) return;  
        /* set the values to be modified 译:将需要修改的值设置为 */  
        attribute_data.num_attribute_values = 3;  
        attribute_data.values = attribute_values;  
        attribute_values[0].value_type = UF_PD_INTEGER_VALUE;  
        attribute_values[0].value_title = "Category";  
        attribute_values[0].integer_value = 3; /* "User Defined" 译:自定义 */  
        attribute_values[1].value_type = UF_PD_INTEGER_VALUE;  
        attribute_values[1].value_title = "Identifier";  
        attribute_values[1].integer_value = 1; /* "User Defined"; 译:用户定义 */  
        attribute_values[2].value_type = UF_PD_STRING_VALUE;  
        attribute_values[2].value_title = "Revision";  
        attribute_values[2].string_value = "1";  
        attribute_data.num_attribute_value_dats = 1;  
        attribute_data.value_dats = &attribute_value_data;  
        attribute_value_data.title = "Text";  
        attribute_value_data.dat.num_strings = 3;  
        attribute_value_data.dat.strings = strings;  
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

> 这段代码是用于在NX中创建一个自定义通用注释的产品定义的二次开发代码。主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了do_it函数，用于执行创建自定义通用注释产品定义的逻辑。
> 3. 在do_it函数中，首先定义了产品定义数据结构，包括产品名称、保留状态、属性数量等，并指定了属性类型为通用注释。
> 4. 调用UF_PD_create_product_definition函数创建产品定义。
> 5. 调用UF_PD_ask_product_attribues请求产品属性，以便后续修改。
> 6. 设置需要修改的属性值，包括类别、标识符、修订版等，并指定文本属性值。
> 7. 调用UF_PD_modify_product_attribute修改产品属性。
> 8. 调用UF_MODL_update更新模型。
> 9. 在ufusr函数中，初始化UF模块，调用do_it函数，然后终止UF模块。
> 10. 定义ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码实现了在NX中创建一个自定义通用注释产品定义的功能，包括设置注释的文本内容。
>
