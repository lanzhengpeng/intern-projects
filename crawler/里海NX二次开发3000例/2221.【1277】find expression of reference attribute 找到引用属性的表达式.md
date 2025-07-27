### 【1277】find expression of reference attribute 找到引用属性的表达式

#### 代码

```cpp
    /*HEAD FIND_EXPRESSION_OF_REFERENCE_ATTRIBUTE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_cfi.h>  
    #include <uf_attr.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本新增的函数。

翻译如下：

注意：UF_print_syslog是V18版本新增的函数。 */  
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
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL,NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /*  This only works when the attribute references a single expression 里海译:仅当属性引用单个表达式时，此方法才有效。 */  
    static void find_expression_of_reference_attribute(tag_t object,  
        char *att_title, char *att_value, char *exp_str)  
    {  
        int  
            ii,  
            n_exps;  
        tag_t  
            *exps,  
            part = UF_ASSEM_ask_work_part();  
        double  
            att_val = atof(att_value),  
            exp_val,  
            t_val,  
            tweak;  
        UF_UNDO_mark_id_t  
            mark;  
        char  
            *lhs,  
            new_str[UF_MAX_EXP_LENGTH+1],  
            *rhs,  
            *t_str;  
        UF_ATTR_value_t  
            new_value;  
        strcpy(exp_str, "");  
    /*  Reading a UF_ATTR_reference so assign the character array to the pointer 里海译:根据上下文，这句话的中文翻译是：“读取一个 UF_ATTR_reference，因此将字符数组分配给指针”。 */  
        new_value.value.reference = new_str;  
    /*  Do not know the precision of the attribute value, so figure it out 里海译:不知道属性值的精确性，因此进行计算。 */  
        strcpy(new_str, att_value);  
        for (ii = 0; ii < strlen(new_str) - 2; ii++)  
            if (new_str[ii] != '.') new_str[ii] = '0';  
        new_str[strlen(new_str) - 1] = '1';  
        tweak = atof(new_str);  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark));  
        UF_CALL(UF_DISP_set_display(FALSE));  
        UF_CALL(UF_MODL_ask_exps_of_part(part, &n_exps, &exps));  
        for (ii = 0; ii < n_exps; ii++)  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_value(exps[ii], &exp_val));  
            if (abs(exp_val - att_val) < tweak)  
            {  
                UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &t_str));  
                UF_CALL(UF_MODL_dissect_exp_string(t_str, &lhs, &rhs, &exps[ii]));  
                t_val = exp_val + tweak;  
                sprintf(new_str, "%s=%f", lhs, t_val);  
                UF_free(lhs);  
                UF_free(rhs);  
                UF_CALL(UF_MODL_edit_exp(new_str));  
                UF_CALL(UF_MODL_update());  
                UF_CALL(UF_ATTR_read_value(object, att_title, UF_ATTR_reference,  
                    &new_value));  
                if (abs(atof(new_value.value.reference) - t_val) < tweak)  
                    strcpy(exp_str, t_str);  
                UF_free(t_str);  
                UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
            }  
        }  
        UF_free(exps);  
        UF_CALL(UF_DISP_set_display(TRUE));  
    }  
    static void report_attributes(tag_t object)  
    {  
        int  
            indx = 0,  
            n;  
        char  
            exp_str[UF_MAX_EXP_LENGTH+1],  
            dstr[21],  
            mstr[UF_UI_MAX_STRING_LEN+1],  
            title[UF_ATTR_MAX_TITLE_LEN+1],  
            tstr[21];  
        UF_ATTR_value_t  
            att;  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_ATTR_count_attributes(object, UF_ATTR_any, &n));  
        sprintf(mstr, "tag %d has %d attributes\n", object, n);  
        UF_CALL(UF_UI_write_listing_window(mstr));  
        while (!UF_CALL(UF_ATTR_cycle(object, &indx, UF_ATTR_any, title, &att))  
            && (indx != 0))  
        {  
            switch ( att.type )  
            {  
                case UF_ATTR_integer:  
                    sprintf(mstr, "%s = %d (INTEGER)\n", title, att.value.integer);  
                break;  
                case UF_ATTR_real:  
                    sprintf(mstr, "%s = %f (FLOATING POINT)\n", title,  
                        att.value.real);  
                break;  
                case UF_ATTR_time:  
                    UF_CALL(uc4582(att.value.time, 2, dstr, tstr));  
                    sprintf(mstr, "%s = %s %s (DATE & TIME)\n", title, dstr, tstr);  
                break;  
                case UF_ATTR_null:  
                    sprintf(mstr, "%s = (NULL)\n", title);  
                break;  
                case UF_ATTR_string:  
                    sprintf(mstr, "%s = %s (STRING)\n", title, att.value.string);  
                    UF_free(att.value.string);  
                break;  
                case UF_ATTR_reference:  
                    find_expression_of_reference_attribute(object, title,  
                        att.value.reference, exp_str);  
                    sprintf(mstr, "%s = %s = %s (REFERENCE)\n", title,  
                        exp_str, att.value.reference);  
                    UF_free(att.value.reference);  
                break;  
                default:  
                break;  
            }  
            UF_CALL(UF_UI_write_listing_window(mstr));  
        }  
        UF_CALL(UF_UI_write_listing_window("\n"));  
    }  
    static void report_load_status(UF_PART_load_status_t *status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        if (status->n_parts > 0)  
        {  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void ensure_fully_loaded(tag_t item)  
    {  
        tag_t  
            part;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        UF_ASSEM_ask_part_name_of_child(  
            UF_ASSEM_ask_inst_of_part_occ(  
            UF_ASSEM_ask_part_occurrence(item)), partname);  
        if ((UF_PART_is_loaded(partname)) != 1)  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            report_load_status(&error_status);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object,  
            owner,  
            orig,  
            proto;  
        while ((object = select_anything("Select something")) != NULL_TAG)  
        {  
            report_attributes(object);  
        /*  This method can be used to access reference attributes not found on  
            object occurences in assembly parts */  
            if (UF_ASSEM_is_occurrence(object))  
            {  
                ensure_fully_loaded(object);  
                proto = UF_ASSEM_ask_prototype_of_occ(object);  
                UF_CALL(UF_OBJ_ask_owning_part(proto, &owner));  
                UF_CALL(UF_ASSEM_set_work_part_quietly(owner, &orig));  
                report_attributes(proto);  
                UF_CALL(UF_ASSEM_set_work_part_quietly(orig, &orig));  
            }  
        }  
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

> 这段代码是用于NX的二次开发代码，其主要功能是选择任意对象，并报告该对象的属性信息，尤其是引用属性对应的表达式。主要步骤如下：
>
> 1. 包含了NX二次开发所需的头文件。
> 2. 定义了错误报告函数report_error，用于在调用NX API出错时输出错误信息。
> 3. 定义了选择任意对象的函数select_anything，用于弹出对话框让用户选择一个对象。
> 4. 定义了查找引用属性对应表达式的函数find_expression_of_reference_attribute，通过对表达式值的微小调整，找到与之对应的表达式。
> 5. 定义了报告对象属性的函数report_attributes，用于输出对象的各种属性信息，包括基本属性和引用属性。
> 6. 定义了确保组件完全加载的函数ensure_fully_loaded。
> 7. 主函数do_it不断让用户选择对象，并报告其属性信息，如果选择的是组件实例，还会报告原型组件的属性信息。
> 8. ufusr函数是NX二次开发的入口函数，在这里调用主函数do_it。
> 9. ufusr_ask_unload函数用于卸载NX的二次开发环境。
>
> 总的来说，这段代码实现了选择对象并查看其属性信息的功能，尤其是对引用属性进行了特殊处理，找出了对应的表达式，以增强属性信息的可读性。
>
