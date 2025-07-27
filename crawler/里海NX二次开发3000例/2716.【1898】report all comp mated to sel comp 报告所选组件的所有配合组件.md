### 【1898】report all comp mated to sel comp 报告所选组件的所有配合组件

#### 代码

```cpp
    /*HEAD REPORT_ALL_COMP_MATED_TO_SEL_COMP CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog 是 V18 中的新功能，只回答译文，不要添加其他内容。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_component_name(tag_t component_tag)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        WRITE_D(component_tag);  
        if (!UF_CALL(UF_ASSEM_ask_component_data(component_tag, part_name,  
            refset_name, instance_name, origin, csys_matrix, transform)))  
            WRITE_S(instance_name);  
    }  
    static void get_component_name(tag_t component_tag, char *component_name)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        if (!UF_CALL(UF_ASSEM_ask_component_data(component_tag, part_name,  
            refset_name, instance_name, origin, csys_matrix, transform)))  
            strcpy( component_name, instance_name);  
    }  
    static void do_it(void)  
    {  
        logical  
            mated;  
        tag_t  
            parent_comp = NULL_TAG,  
            comp = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            selected_component[MAX_ENTITY_NAME_SIZE+1],  
            search_string[MAX_ENTITY_NAME_SIZE+5];  
        UF_ASSEM_mating_condition_t  
            mc_data;  
        while ((parent_comp = select_a_component("Select a component")) != NULL_TAG)  
        {  
            get_component_name(parent_comp, selected_component);  
            strcpy( search_string, "->");  
            strcat( search_string, selected_component);  
            WRITE("Components mated to ");  
            WRITE(selected_component);  
            WRITE("\n------------------------------------------------------");  
            WRITE("\n");  
            //WRITE_S( selected_component);  
            while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
            {  
                UF_CALL(UF_ASSEM_ask_mc_data_of_compnt(comp, &mated, &mc_data));  
                if( mated && strstr(mc_data.name, search_string))  
                {  
                    report_component_name( comp );  
                    //WRITE_S((char *)mc_data.name);  
                    //WRITE("\n");  
                }  
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

> 这段NX二次开发代码的主要功能是报告所选组件的所有配合组件。
>
> 关键点总结如下：
>
> 1. 初始化和终止：使用UF_initialize和UF_terminate进行NX系统的初始化和终止。
> 2. 错误报告函数：定义了report_error函数用于报告错误。
> 3. 选择组件：使用ask_next_component函数遍历所有组件，并使用select_a_component函数让用户选择一个组件。
> 4. 组件名称获取：使用get_component_name和report_component_name函数获取和报告组件的名称。
> 5. 配合关系查询：使用UF_ASSEM_ask_mc_data_of_compnt函数查询组件的配合关系，并使用strstr函数过滤出与所选组件相关的配合关系。
> 6. 配合组件报告：通过循环遍历所有组件，并报告与所选组件相关的配合组件。
> 7. 主函数：在主函数中，先打开所有传入的部件，然后依次处理每个部件，最后关闭所有部件。
> 8. 加载状态检查：使用check_load_status函数检查每个部件的加载状态。
> 9. 外部调用：使用uc4624和uc4621函数处理外部传入的部件路径。
> 10. 执行流程：通过do_it函数执行报告配合组件的功能。
>
> 以上是该代码的主要功能点，您作为作者，可以根据需要对该代码进行修改和扩展。
>
