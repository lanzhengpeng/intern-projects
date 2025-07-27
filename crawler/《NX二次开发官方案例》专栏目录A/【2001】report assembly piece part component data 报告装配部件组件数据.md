### 【2001】report assembly piece part component data 报告装配部件组件数据

#### 代码

```cpp
    /*HEAD REPORT_ASSEMBLY_PIECE_PART_COMPONENT_DATA CCC UFUN */  
    /*  This will return the lowest level piece part component data in an assembly  
        even if no components are loaded.  */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    #define WRITE3F(X) (write_double_array(#X, X, 1, 3))  
    #define WRITE9F(X) (write_double_array(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array(#X, X, 4, 4))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void write_double_array(char *name, double *array, int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    /* qq3123197280 */  
    static void report_component_data(tag_t comp)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            *comp_part_name,  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        if (!UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
            instance_name, origin, csys_matrix, transform)))  
        {  
            UF_CALL(UF_PART_file_name_for_display(part_name, &comp_part_name));  
            WRITE_S(comp_part_name);  
            UF_free(comp_part_name);  
            WRITE_S(refset_name);  
            WRITE_S(instance_name);  
            WRITE3F(origin);  
            WRITE9F(csys_matrix);  
            WRITE16F((double*)transform);  
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void add_piece_part_comps_to_list(tag_t comp, uf_list_p_t pp_list)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *children;  
        if ((n = UF_ASSEM_ask_all_part_occ_children(comp, &children)) == 0)  
        {  
            UF_CALL(UF_MODL_put_list_item(pp_list, comp));  
        }  
        else  
        {  
            for (ii = 0; ii < n; ii++)  
                add_piece_part_comps_to_list(children[ii], pp_list);  
            UF_free(children);  
        }  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    /*  This will return the lowest level piece part components in an assembly  
        even if no components are loaded.  */  
    static int ask_assembly_piece_part_components(tag_t assy, tag_t **pp_comps)  
    {  
        tag_t  
            root = UF_ASSEM_ask_root_part_occ(assy);  
        uf_list_p_t  
            pp_list;  
        UF_CALL(UF_MODL_create_list(&pp_list));  
        add_piece_part_comps_to_list(root, pp_list);  
        return make_an_array(&pp_list, pp_comps);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *pp_comps;  
        n = ask_assembly_piece_part_components(part, &pp_comps);  
        for (ii = 0; ii < n; ii++) report_component_data(pp_comps[ii]);  
        if (n > 0) UF_free(pp_comps);  
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
    static void turn_off_loading_of_components(void)  
    {  
        UF_ASSEM_options_t  
            assy_options;  
        UF_CALL(UF_ASSEM_ask_assem_options(&assy_options));  
        if (assy_options.load_components != UF_ASSEM_dont_load_components)  
        {  
            assy_options.load_components = UF_ASSEM_dont_load_components;  
            UF_CALL(UF_ASSEM_set_assem_options(&assy_options));  
        }  
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
            turn_off_loading_of_components();  
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

> 这段代码是一个NX的二次开发示例，其主要功能是遍历装配体中的所有组件，并打印出每个组件的详细信息，包括零件名称、引用集名称、实例名称、原点、坐标系和变换矩阵。
>
> 具体步骤包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印出调用UF函数失败时的错误信息。
> 2. 定义了用于打印字符串和数组的辅助函数。
> 3. 定义了report_component_data函数，用于打印组件的详细信息。
> 4. 定义了递归函数add_piece_part_comps_to_list，用于遍历装配体中的所有组件，并将最底层的零件组件添加到一个列表中。
> 5. 定义了ask_assembly_piece_part_components函数，用于获取装配体中最底层的零件组件列表。
> 6. 在do_it函数中，获取当前显示的零件，并调用ask_assembly_piece_part_components获取零件组件列表，然后遍历列表打印每个组件的详细信息。
> 7. 在主函数main中，关闭自动加载组件的选项，然后遍历命令行参数中指定的所有零件文件，打开每个零件，调用do_it函数打印组件信息，然后关闭所有零件。
>
> 总体来说，这段代码通过递归遍历装配体，实现了获取并打印出所有最底层零件组件信息的功能。
>
