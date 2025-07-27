### 【3151】report part family template data 报告部件族模板的数据

#### 代码

```cpp
    /*HEAD REPORT_PART_FAMILY_TEMPLATE_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_fam.h>  
    #define WRITE(X)      (UF_UI_write_listing_window(X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。该函数用于打印系统日志。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        logical  
            is_template;  
        int  
            family_count,  
            ii,jj,kk;  
        tag_t  
            *families,  
            part = UF_PART_ask_display_part();  
        UF_FAM_family_data_t  
            family_data;  
        UF_FAM_attribute_data_t  
            attribute_data;  
        UF_FAM_member_data_t  
            member_data;  
        UF_UI_open_listing_window();  
        UF_CALL(UF_PART_is_family_template(part, &is_template));  
        if (is_template)  
        {  
            UF_CALL(UF_PART_ask_families(part, &family_count, &families));  
            WRITE_D(family_count);  
            for (ii = 0; ii < family_count; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_D(families[ii]);  
                UF_CALL(UF_FAM_ask_family_data(families[ii], &family_data));  
                WRITE_D(family_data.subtype);  
                WRITE_D(family_data.attribute_count);  
                for (jj = 0; jj < family_data.attribute_count; jj++)  
                {  
                    WRITE_D(jj);  
                    WRITE_D(family_data.attributes[jj]);  
                    UF_CALL(UF_FAM_ask_attribute_data(family_data.attributes[jj],  
                        &attribute_data));  
                    WRITE_D(attribute_data.subtype);  
                    WRITE_D(attribute_data.base_object);  
                    WRITE_S(attribute_data.rules);  
                    WRITE_S(attribute_data.value);  
                    WRITE_S(attribute_data.name);  
                    UF_CALL(UF_FAM_free_attribute_data(&attribute_data));  
                    WRITE("column data\n");  
                    UF_CALL(UF_FAM_ask_member_column_data(families[ii], jj,  
                        &member_data));  
                    WRITE_D(member_data.value_count);  
                    for (kk = 0; kk < member_data.value_count; kk++)  
                    {  
                        WRITE_D(kk);  
                        WRITE_S(member_data.values[kk]);  
                    }  
                    UF_CALL(UF_FAM_free_member_data(&member_data));  
                }  
                WRITE_D(family_data.member_count);  
                for (jj = 0; jj < family_data.member_count; jj++)  
                {  
                    WRITE_D(jj);  
                    UF_CALL(UF_FAM_ask_member_row_data(families[ii], jj,  
                        &member_data));  
                    WRITE("row data\n");  
                    WRITE_D(member_data.value_count);  
                    for (kk = 0; kk < member_data.value_count; kk++)  
                    {  
                        WRITE_D(kk);  
                        WRITE_S(member_data.values[kk]);  
                    }  
                    UF_CALL(UF_FAM_free_member_data(&member_data));  
                }  
                WRITE_S(family_data.name);  
                UF_CALL(UF_FAM_free_family_data(&family_data));  
            }  
        }  
        else  
            UF_UI_write_listing_window("Part is not a family template.\n");  
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

> 这段NX二次开发代码的主要功能是查询和报告NX中当前显示零件的家族模板信息。具体介绍如下：
>
> 1. 包含头文件：代码包含了必要的NX API头文件。
> 2. 错误报告函数：定义了report_error函数，用于打印函数调用失败的信息。
> 3. 数据写入函数：定义了write_integer_to_listing_window和write_string_to_listing_window函数，用于将整数和字符串写入Listing窗口。
> 4. 查询家族模板信息：do_it函数实现了以下功能：查询当前显示零件是否为家族模板。若是家族模板，则查询所有家族信息。对于每个家族，查询其名称、属性数量、成员数量等。对于每个属性，查询其名称、值、规则等。对于每个成员，查询其列数据和行数据。
> 5. 查询当前显示零件是否为家族模板。
> 6. 若是家族模板，则查询所有家族信息。
> 7. 对于每个家族，查询其名称、属性数量、成员数量等。
> 8. 对于每个属性，查询其名称、值、规则等。
> 9. 对于每个成员，查询其列数据和行数据。
> 10. 用户自定义函数：ufusr函数实现了NX二次开发标准入口，其中调用do_it函数来执行上述功能。
> 11. 卸载函数：定义了ufusr_ask_unload函数，用于设置立即卸载。
>
> 综上所述，该代码主要用于获取和报告NX中当前显示零件的家族模板信息，实现了对NX零件模板的二次开发查询功能。
>
