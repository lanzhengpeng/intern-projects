### 【2295】report refset info 报告参考集信息

#### 代码

```cpp
    /*HEAD REPORT_REFSET_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_group.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 版本中新增的函数。 */  
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
    static void report_tag_info(char *name, tag_t object)  
    {  
        int  
            ii,  
            n,  
            status,  
            subtype,  
            type,  
            units;  
        char  
            *exp_str,  
            *feat_type,  
            idstring[20],  
            obj_name[UF_OBJ_NAME_LEN + 1],  
            part_name[MAX_FSPEC_SIZE + 1];  
        tag_t  
            *members;  
        printf("%s - tag %d is ", name, object);  
        if (object)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if (type == UF_feature_type)  
            {  
                UF_CALL(UF_MODL_ask_feat_type( object, &feat_type));  
                printf("a %s feature\n", feat_type);  
                UF_free(feat_type);  
            }  
            else  
            {  
                if (UF_ASSEM_is_occurrence(object))  
                    printf("an occurrence, ");  
                if (!UF_OBJ_ask_name(object, obj_name))  
                    printf("named %s, ", obj_name);  
                status = UF_OBJ_ask_status(object);  
                if (status == UF_OBJ_ALIVE)  
                {  
                    printf("type %d, subtype %d", type, subtype);  
                    switch (type)  
                    {  
                        case UF_solid_type:  
                            switch (subtype)  
                            {  
                                case UF_solid_edge_subtype:  
                                    UF_CALL(UF_MODL_ask_edge_type(object, &type));  
                                    printf(", edge type %d\n", type);  
                                    break;  
                                case UF_solid_face_subtype:  
                                    UF_CALL(UF_MODL_ask_face_type(object, &type));  
                                    printf(", face type %d\n", type);  
                                    break;  
                                case UF_solid_body_subtype:  
                                    UF_CALL(UF_MODL_ask_body_type(object, &type));  
                                    if (type == UF_MODL_SHEET_BODY)  
                                         printf(" (sheet body)\n");  
                                    else printf(" (solid body)\n");  
                                    break;  
                                default:  
                                    printf("\n");  
                                    break;  
                            }  
                            break;  
                        case UF_group_type:  
                            UF_CALL(UF_GROUP_ask_group_data(object, &members, &n));  
                            printf(" has %d group members\n", n);  
                            for (ii=0; ii<n; ii++)  
                            {  
                                sprintf(idstring, "  member %d", ii);  
                                report_tag_info(idstring, members[ii]);  
                                UF_CALL(UF_DISP_set_highlight(members[ii], FALSE));  
                            };  
                            if (n > 0) UF_free(members);  
                            break;  
                        case UF_reference_set_type:  
                            UF_CALL(UF_ASSEM_ask_ref_set_members(object, &n,  
                                &members));  
                            printf(" has %d ref set members\n", n);  
                            for (ii=0; ii<n; ii++)  
                            {  
                                sprintf(idstring, "  member %d", ii);  
                                report_tag_info(idstring, members[ii]);  
                            }  
                            UF_free(members);  
                            break;  
                        default:  
                            printf("\n");  
                            break;  
                    }  
                }  
                else  
                {  
                    if (type == 0)  
                    {  
                        if (!UF_MODL_ask_exp_tag_string(object, &exp_str))  
                        {  
                            printf("an expression - %s\n", exp_str);  
                            UF_free(exp_str);  
                        }  
                    }  
                    else  
                    {  
                        if (!UF_PART_ask_status(object, &status))  
                        {  
                            UF_CALL(UF_PART_ask_units(object, &units ));  
                            UF_PART_ask_part_name(object, part_name);  
                            switch (units)  
                            {  
                                case UF_PART_METRIC:  
                                    printf("a metric part - %s\n", part_name);  
                                    break;  
                                case UF_PART_ENGLISH:  
                                    printf("an inch part - %s\n", part_name);  
                                    break;  
                            }  
                        }  
                        else  
                            printf("type %d, subtype %d, status %d\n", type,  
                                subtype, status);  
                    }  
                }  
            }  
        }  
        else printf("a NULL_TAG\n");  
    }  
    static tag_t ask_next_ref_set(tag_t part, tag_t refset)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_reference_set_type,  
            &refset)) && (refset != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(refset, &type, &subtype));  
    #ifdef UF_reference_design_model_subtype  
            if ((subtype == UF_reference_design_subtype) ||  
                (subtype == UF_reference_design_model_subtype) ||  
                (subtype == UF_reference_design_lightweight_subtype))  
                return refset;  
    #else  
            if (subtype == UF_reference_design_subtype) return refset;  
    #endif  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            ref_set = NULL_TAG;  
        while ((ref_set = ask_next_ref_set(part, ref_set)) != NULL_TAG)  
            report_tag_info("ref_set", ref_set);  
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

> 这段NX二次开发代码的主要功能是遍历当前显示部件中的所有参考集，并输出每个参考集的信息。
>
> 代码的主要步骤如下：
>
> 1. 定义了一个report_error函数，用于在出错时输出错误信息。
> 2. 定义了一个report_tag_info函数，用于输出一个对象的信息，包括类型、名称等。
> 3. 定义了一个ask_next_ref_set函数，用于遍历部件中的下一个参考集。
> 4. 定义了一个do_it函数，用于遍历当前显示部件中的所有参考集，并调用report_tag_info输出每个参考集的信息。
> 5. 定义了ufusr函数，这是NX二次开发的入口函数。在这个函数中，首先初始化NX系统，然后调用do_it函数，最后关闭NX系统。
> 6. 定义了ufusr_ask_unload函数，用于卸载当前加载的NX二次开发模块。
>
> 总体来说，这段代码通过遍历部件中的所有参考集，并输出每个参考集的信息，实现了对参考集的检查和报告功能。
>
