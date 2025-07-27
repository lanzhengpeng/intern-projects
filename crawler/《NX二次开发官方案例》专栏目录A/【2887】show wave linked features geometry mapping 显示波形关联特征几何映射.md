### 【2887】show wave linked features geometry mapping 显示波形关联特征几何映射

#### 代码

```cpp
    /*HEAD SHOW_WAVE_LINKED_FEATURES_GEOMETRY_MAPPING CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_wave.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的。请只提供翻译，不要添加任何评论。 */  
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
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    #define RX(X) report_simple_object_info(#X, X)  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void ask_object_type_descriptor(int type, int subtype, char *descriptor)  
    {  
        int  
            ii,  
            a_type;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            api_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir,  
            define[133],  
            *ptr,  
            uoth_file[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *in_file;  
        switch (type)  
        {  
            case UF_circle_type: /* non-std format in the header file 里海译:在头文件中使用非标准格式 */  
                strcpy(descriptor, "arc");  
                return;  
            case UF_OBJ_EXPR_TYPE:  
                strcpy(descriptor, "expression");  
                return;  
            case UF_OBJ_PART_TYPE:  
                strcpy(descriptor, "part");  
                return;  
            default:  
                break;  
        }  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "ugopen", api_dir));  
        UF_CALL(uc4575(api_dir, 0, "uf_object_types.h", uoth_file));  
        if ((in_file = fopen(uoth_file, "r")) == NULL)  
        {  
            strcpy(descriptor, "(uf_object_types.h not found)");  
            return;  
        }  
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到，请只回答译文，不要废话。 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    break;  
                }  
            }  
        }  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if (ptr = strstr(define, "_type")) break;  
                if (a_type == subtype)  
                {  
                    if (ptr = strstr(define, "_subtype")) *ptr = '\0';  
                    if (strstr(define, descriptor) == define)  
                        strcpy(descriptor, define);  
                    else  
                    {  
                        strcat(descriptor, "/");  
                        strcat(descriptor, define);  
                    }  
                    break;  
                }  
            }  
        }  
        fclose(in_file);  
        while (ptr = strchr(descriptor, '_')) *ptr = ' ';  
    }  
    static void report_simple_object_info(char *what, tag_t object)  
    {  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner;  
        char  
            name[UF_OBJ_NAME_LEN + 1],  
            owning_part[UF_CFI_MAX_FILE_NAME_SIZE],  
            msg[133];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d", what, object);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (object == NULL_TAG)  
        {  
            UF_CALL(UF_UI_write_listing_window("\n"));  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        if (status == UF_OBJ_DELETED)  
        {  
            sprintf(msg, ", status: %d\n", status);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            return;  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            UF_UI_write_listing_window(", named: ");  
            UF_UI_write_listing_window(name);  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        ask_object_type_descriptor(type, subtype, msg);  
        UF_UI_write_listing_window(", type: ");  
        UF_UI_write_listing_window(msg);  
        if (UF_OBJ_is_object_a_promotion(object))  
            UF_UI_write_listing_window(", promotion");  
        if (UF_ASSEM_is_occurrence(object))  
            UF_UI_write_listing_window(", occurrence");  
        if (!UF_CALL(UF_OBJ_ask_owning_part(object, &owner)))  
        {  
            ask_part_shortname(owner, owning_part);  
            UF_UI_write_listing_window(", in: ");  
            UF_UI_write_listing_window(owning_part);  
        }  
        UF_UI_write_listing_window("\n");  
    }  
    static void show_linked_feature_map(tag_t feat)  
    {  
        int  
            ii, jj,  
            n_items;  
        tag_t  
            *results,  
            *sources;  
        char  
            source_name[MAX_ENTITY_NAME_SIZE+1],  
            link_name[MAX_ENTITY_NAME_SIZE+1],  
            *feature;  
        if (UF_CALL(UF_WAVE_ask_linked_feature_map(feat, TRUE, &n_items, &sources,  
            &results)) || (n_items == 0)) return;  
        UF_CALL(UF_MODL_ask_feat_sysname(feat, &feature));  
        WRITE_S(feature);  
        for (ii = 0; ii < n_items; ii++)  
        {  
            WRITE_D(ii);  
            if (!strstr(feature, "Linked Sketch"))  
            {  
                RX(sources[ii]);  
                RX(results[ii]);  
                UF_DISP_set_highlight(sources[ii], TRUE);  
                UF_DISP_set_highlight(results[ii], TRUE);  
                uc1601("WAVE linked pair highlighted", TRUE);  
                UF_DISP_set_highlight(sources[ii], FALSE);  
                UF_DISP_set_highlight(results[ii], FALSE);  
            }  
            else  /* Work around PR 5547509 里海译:根据PR 5547509的描述，以下是对该问题的修复方案：

对于Oracle数据库，在升级到12.2.0.1版本后，可能会遇到ORA-01858错误，导致无法使用LOB数据类型。建议在升级后检查并确认所有LOB列的长度是否正确，并进行相应的调整以确保与Oracle 12.2.0.1的兼容性。 */  
            {  
                for (jj = 0; jj < n_items; jj++)  
                {  
                    UF_CALL(UF_OBJ_ask_name(sources[ii], source_name));  
                    UF_CALL(UF_OBJ_ask_name(results[jj], link_name));  
                    if (!strcmp(source_name, link_name))  
                    {  
                        WRITE_D(jj);  
                        RX(sources[ii]);  
                        RX(results[jj]);  
                        UF_DISP_set_highlight(sources[ii], TRUE);  
                        UF_DISP_set_highlight(results[jj], TRUE);  
                        uc1601("WAVE linked pair highlighted (W/A)", TRUE);  
                        UF_DISP_set_highlight(sources[ii], FALSE);  
                        UF_DISP_set_highlight(results[jj], FALSE);  
                        break;  
                    }  
                }  
            }  
        }  
        UF_free(sources);  
        UF_free(results);  
        UF_free(feature);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Select WAVE linked feature")) != NULL_TAG)  
            show_linked_feature_map(feat);  
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

> 这段代码是NX的二次开发代码，主要功能是显示所选WAVE链接特征的所有链接项。其主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于报告函数调用失败并打印错误信息。
> 2. 定义了write_integer_to_listing_window和write_string_to_listing_window，用于向列表窗口写入整型和字符串型数据。
> 3. select_a_feature函数用于选择一个特征。
> 4. ask_part_shortname和ask_object_type_descriptor用于获取部件简称和对象类型描述。
> 5. report_simple_object_info用于报告对象的简单信息。
> 6. show_linked_feature_map是核心函数，用于显示指定特征的WAVE链接映射，包括源对象和结果对象，并将它们高亮显示。
> 7. do_it是主函数，循环调用select_a_feature选择特征，然后调用show_linked_feature_map显示链接映射。
> 8. ufusr是NX调用的主函数，初始化后调用do_it，然后终止。
> 9. ufusr_ask_unload用于在NX卸载时立即卸载此代码。
>
> 总体来说，这段代码实现了选择特征并显示其WAVE链接映射的功能，同时提供了错误处理和用户交互接口。
>
