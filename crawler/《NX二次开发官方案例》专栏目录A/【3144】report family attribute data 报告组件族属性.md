### 【3144】report family attribute data 报告组件族属性

#### 代码

```cpp
    /*HEAD REPORT_FAMILY_ATTRIBUTE_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_fam.h>  
    #include <uf_attr.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您提供的Note，UF_print_syslog是V18版本中的新功能。因此，我的回答是：

UF_print_syslog是V18版本中的新功能。 */  
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
        tag_t   part = UF_ASSEM_ask_work_part();  
        logical is_template;  
        int     fam_cnt, ii, jj;  
        char    part_fspec[MAX_FSPEC_SIZE+1];  
        UF_FAM_family_data_t    family_data;  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL( UF_PART_is_family_template( part, &is_template));  
        if(is_template)  
        {  
            int     type, subtype;  
            char    print_str[132];  
            tag_t   *fam_tags, fam_attr = NULL_TAG;  
            tag_t   disp_part = UF_PART_ask_display_part();  
            UF_FAM_member_data_t    mem_data;  
            UF_FAM_attribute_data_t attr_data;  
            UF_UI_open_listing_window();  
            UF_CALL(UF_PART_ask_families( part, &fam_cnt, &fam_tags));  
            sprintf( print_str, "\n\nTag of family of %s: %d\n", part_fspec,  
                fam_tags[0]);  
            UF_UI_write_listing_window( print_str);  
            UF_CALL( UF_FAM_ask_family_data( fam_tags[0], &family_data));  
            sprintf( print_str, "    Number of family members: %d\n\n",  
                family_data.member_count);  
            UF_UI_write_listing_window( print_str);  
            while (!UF_CALL(UF_OBJ_cycle_objs_in_part( disp_part, UF_fam_attr_type,  
                &fam_attr)) && (fam_attr != NULL_TAG))  
            {  
                UF_CALL( UF_OBJ_ask_type_and_subtype( fam_attr, &type, &subtype));  
                attr_data.subtype = UF_fam_attr_name_subtype;  
                attr_data.base_object = NULL_TAG;  
                UF_CALL( UF_FAM_ask_attribute_data( fam_attr, &attr_data));  
                sprintf( print_str, "%-15s", attr_data.name);  
                UF_UI_write_listing_window( print_str);  
            }  
            UF_UI_write_listing_window( "\n");  
            UF_CALL( UF_FAM_free_attribute_data( &attr_data));  
            for( ii = 0; ii < family_data.member_count; ii++ )  
            {  
                UF_CALL( UF_FAM_ask_member_row_data( fam_tags[0], ii, &mem_data));  
                for( jj = 0; jj < mem_data.value_count; jj++)  
                {  
                    sprintf( print_str, "%-15s", mem_data.values[jj]);  
                    UF_UI_write_listing_window( print_str);  
                }  
                UF_UI_write_listing_window( "\n");  
            }  
            UF_free( fam_tags);  
        }  
        else  
            uc1601("Work part is not a template part", 1);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要功能是查询当前工作部件是否为族表模板，如果是，则输出该族表的相关信息。
>
> 代码主要包括以下关键部分：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数出错时打印错误信息。
> 2. 定义了do_it函数，用于执行查询族表信息的主体逻辑。首先获取当前工作部件的tag，并判断是否为族表模板。如果是，则输出该族表的相关信息，包括族表成员数量、族表属性名称、族表成员的值等。
> 3. ufusr函数是NX二次开发的标准入口函数，用于初始化和执行do_it函数。
> 4. ufusr_ask_unload函数用于指定卸载二次开发模块的方式。
>
> 总的来说，这段代码通过NX提供的二次开发接口，实现了查询和输出当前工作部件的族表信息的功能。
>
