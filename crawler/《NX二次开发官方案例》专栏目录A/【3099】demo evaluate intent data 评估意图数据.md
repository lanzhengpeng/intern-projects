### 【3099】demo evaluate intent data 评估意图数据

#### 代码

```cpp
    /*HEAD DEMO_EVALUATE_INTENT_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_fam.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
        int  
            num_matches,  
            *match_indices,  
            ii,  
            count = 0;  
        char  
            match[256],  
            int_name[] = "first_intent";  
        tag_t  
            attr_tag = NULL_TAG,  
            *fam_tag;  
        UF_FAM_family_data_t  
            family_data;  
        UF_FAM_attribute_data_t  
            attribute_data;  
        UF_FAM_intent_data_t  
            int_data;  
        printf(" *************************************************\n");  
        UF_CALL(UF_PART_ask_families(UF_ASSEM_ask_work_part(), &count, &fam_tag));  
        if(count > 0)  
        {  
            UF_CALL(UF_FAM_ask_family_data(fam_tag[0], &family_data));  
            for(ii = 0; ii < family_data.attribute_count; ii ++)  
            {  
                printf("********** Attribute %d ********\n", ii + 1);  
                UF_CALL(UF_FAM_ask_attribute_data(family_data.attributes[ii],  
                                                  &attribute_data));  
                printf("Name is : %s\n", attribute_data.name);  
                printf("Vaule is : %s\n", attribute_data.value);  
                if(strcmp(attribute_data.name, "FAMILY_ATTRIBUTE")==0)  
                {  
                    attr_tag = family_data.attributes[ii];  
                }  
            }  
            int_data.family = fam_tag[0];  
            int_data.attribute_count = 1;  
            int_data.attributes = malloc(sizeof( tag_t));  
            int_data.attributes[0] = attr_tag;  
            int_data.match_criteria = malloc(sizeof( char *) );  
            sprintf(match, "this_is_attribute_of_first_test");  
            //sprintf(match, "with_blend==1");  
            printf("match is : %s\n", match);  
            int_data.match_criteria[0] = match;  
            int_data.name = int_name;  
            UF_CALL(UF_FAM_evaluate_intent_data(&int_data, &num_matches,  
                                                &match_indices));  
            printf("number of matches is: %d\n", num_matches);  
        }  
        UF_free(fam_tag);  
    }  
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

> 这段代码是一个NX Open C++二次开发示例，其主要功能是演示如何查询和评估NX部件的家族信息。
>
> 代码的关键点包括：
>
> 1. 包含了必要的NX Open C++头文件，用于操作NX模型。
> 2. 定义了UF_CALL宏，用于调用NX API，并在出错时输出错误信息。
> 3. 定义了do_it函数，该函数执行以下操作：查询当前工作部件的所有家族信息。遍历每个家族的所有属性，并输出属性名称和值。创建一个Intent Data结构，指定要匹配的家族和属性。调用UF_FAM_evaluate_intent_data函数，评估Intent Data，并获取匹配结果的数量。
> 4. 查询当前工作部件的所有家族信息。
> 5. 遍历每个家族的所有属性，并输出属性名称和值。
> 6. 创建一个Intent Data结构，指定要匹配的家族和属性。
> 7. 调用UF_FAM_evaluate_intent_data函数，评估Intent Data，并获取匹配结果的数量。
> 8. 定义了ufusr函数，该函数在NX启动时被调用，执行do_it函数。
> 9. 定义了ufusr_ask_unload函数，该函数返回立即卸载标志，用于在卸载用户函数时调用。
>
> 总的来说，这段代码的目的是演示如何在NX部件中查询家族信息，创建Intent Data进行匹配评估，并输出匹配结果。
>
