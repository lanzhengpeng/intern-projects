### 【1863】remove reference component attribute 移除组件的REFERENCE_COMPONENT属性

#### 代码

```cpp
    #define _CRT_SECURE_NO_WARNINGS  
    /* remove REFERENCE_COMPONENT attribute from all component instances 里海译:从所有组件实例中移除REFERENCE_COMPONENT属性 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_attr.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_undo.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    /*  This only works when the attribute references a single expression 里海译:仅当属性引用单个表达式时，此操作才有效。 */  
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
    /*  Reading a UF_ATTR_reference so assign the character array to the pointer 里海译:读取一个 UF_ATTR_reference，因此将字符数组分配给指针。 */  
        new_value.value.reference = new_str;  
    /*  Do not know the precision of the attribute value, so figure it out 里海译:不确定属性值的精度，所以先计算一下。 */  
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
            n_attributes;  
        char  
            exp_str[UF_MAX_EXP_LENGTH+1],  
            dstr[21],  
            mstr[UF_UI_MAX_STRING_LEN+1],  
            title[UF_ATTR_MAX_TITLE_LEN+1],  
            tstr[21];  
        UF_ATTR_value_t  
            att;  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_ATTR_count_attributes(object, UF_ATTR_any, &n_attributes));  
        if (n_attributes > 0)  
        {  
             WRITE_D(n_attributes);  
        }  
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
                    find_expression_of_reference_attribute(object, title, att.value.reference, exp_str);  
                    sprintf(mstr, "%s = %s = %s (REFERENCE)\n", title, exp_str, att.value.reference);  
                    UF_free(att.value.reference);  
                    //sprintf(mstr, "%s = %s (REFERENCE)\n", title, att.value.reference);  
                break;  
                default:  
                    sprintf(mstr, "Unknown Attribute Type\n");  
                break;  
            }  
            UF_CALL(UF_UI_write_listing_window(mstr));  
        }  
    }  
    static bool remove_refcomp_attr(tag_t object, char *what, char *name)  
    {  
        int  
            title_type = 0;  
        ECHO("\nObject: %s - %d - %s\n", what, object, name);  
        report_attributes(object);  
        UF_CALL( UF_ATTR_find_attribute(object, UF_ATTR_any, "REFERENCE_COMPONENT", &title_type) );  
        if( title_type ==  UF_ATTR_reference )  
        {  
            ECHO("\nDeleting attribute REFERENCE_COMPONENT with type REFERENCE...\n");  
            UF_CALL( UF_ATTR_delete( object, UF_ATTR_reference, "REFERENCE_COMPONENT") );  
            return true;  
        }  
        if( title_type == UF_ATTR_string )  
        {  
            ECHO("\nnDeleting attribute REFERENCE_COMPONENT with type STRING...\n");  
            UF_CALL( UF_ATTR_delete( object, UF_ATTR_string, "REFERENCE_COMPONENT") );  
            return true;  
        }  
        return false;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            comp = NULL_TAG,  
            part_occ = NULL_TAG,  
            instance = NULL_TAG,  
            proto = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        logical  
            is_occ = false;  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
                instance_name, origin, csys_matrix, transform)))  
            {  
                 if( UF_ASSEM_is_occurrence(comp) )  
                {  
                    proto = UF_ASSEM_ask_prototype_of_occ(comp);  
                    remove_refcomp_attr(proto, "Prototype", part_name);  
                    instance = UF_ASSEM_ask_inst_of_part_occ( comp );  
                    remove_refcomp_attr(instance, "Instance", instance_name);  
                    remove_refcomp_attr(comp, "Occurrence", part_name);  
                }  
            }  
        }  
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

> 这段代码是一个NX的二次开发代码，主要实现了从NX部件的所有组件实例中移除REFERENCE_COMPONENT属性的功能。
>
> 主要功能包括：
>
> 1. 使用UF_OBJ_cycle_objs_in_part函数遍历部件中的所有组件实例。
> 2. 使用UF_ATTR_cycle函数遍历组件实例的所有属性。
> 3. 检查属性名称是否为REFERENCE_COMPONENT，如果是，则使用UF_ATTR_delete函数删除该属性。
> 4. 分别处理组件原型的REFERENCE_COMPONENT属性、组件实例的REFERENCE_COMPONENT属性和组件出现的REFERENCE_COMPONENT属性。
> 5. 使用UF_UNDO_set_mark和UF_UNDO_undo_to_mark函数进行撤销操作。
> 6. 使用UF_UI_write_listing_window函数在列表窗口输出信息。
>
> 通过这段代码，可以自动移除NX部件中所有组件实例的REFERENCE_COMPONENT属性，简化了操作，提高了效率。
>
