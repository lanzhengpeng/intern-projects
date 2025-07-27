### 【1495】list all assembly component parts attributes 列出所有组件的属性

#### 代码

```cpp
    /*HEAD LIST_ALL_ASSEMBLY_COMPONENT_PARTS_ATTRIBUTES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
    #include <uf_disp.h>  
    #include <uf_attr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 新增的函数，用于打印系统日志。 */  
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
            new_str[UF_MAX_EXP_LENGTH],  
            *rhs,  
            *t_str;  
        UF_ATTR_value_t  
            new_value;  
        new_value.value.reference = new_str;  
    /*  Do not know the precision of the attribute value, so figure it out 里海译:不知道属性值的精确度，所以弄清楚。 */  
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
            exp_str[UF_MAX_EXP_LENGTH],  
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
    /* qq3123197280 */  
    static logical report_component_partname(tag_t component, void *user_data)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        if (UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform))) return FALSE;  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window("\t");  
        UF_UI_write_listing_window(partname);  
        UF_UI_write_listing_window("\n");  
        report_attributes(component);   
        return TRUE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            all_cset,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_ASSEM_ask_all_comp_cset(part, &all_cset));  
        UF_CALL(UF_ASSEM_apply_to_cset(all_cset, report_component_partname, NULL));  
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
    static void check_load_status(UF_PART_load_status_p_t status)  
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
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++程序，主要实现了以下功能：
>
> 1. 初始化和错误处理：使用UF_initialize()函数初始化NX环境，并定义了一个错误处理函数report_error()，用于输出错误信息。
> 2. 表达式解析：定义了一个函数find_expression_of_reference_attribute()，用于解析引用类型属性的数学表达式，并找到对应的表达式字符串。
> 3. 属性报告：定义了一个函数report_attributes()，用于输出指定对象的所有属性信息。
> 4. 组件遍历：定义了一个函数report_component_partname()，用于遍历装配体中的每个组件，并输出其名称和属性信息。
> 5. 组件遍历函数：定义了一个函数do_it()，用于获取装配体的所有组件集合，并调用report_component_partname()函数进行遍历。
> 6. 用户函数：定义了一个用户函数ufusr()，NX会调用该函数。在该函数中，调用do_it()函数实现主要功能。
> 7. 组件加载：在main()函数中，通过命令行参数指定要加载的组件文件，并调用report_component_partname()函数遍历每个组件。
> 8. 状态检查：定义了一个函数check_load_status()，用于检查组件加载状态。
>
> 总的来说，这段代码主要实现了遍历NX装配体的所有组件，并输出每个组件的名称和属性信息的功能。通过命令行参数指定组件文件，可以在外部调用该程序。
>
