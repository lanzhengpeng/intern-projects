### 【1494】list all assembly component part files attributes 列出所有装配组件零件文件的属性

#### 代码

```cpp
    /*HEAD LIST_ALL_ASSEMBLY_COMPONENT_PART_FILES_ATTRIBUTES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_attr.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中新增的函数。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static logical report_part_attributes(tag_t component, void *user_data)  
    {  
        logical  
            *nxman = (logical *)user_data;  
        int  
            ii,  
            n_attrs;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            display_name[MAX_FSPEC_SIZE + 1],  
            partname[MAX_FSPEC_SIZE + 1],  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_ATTR_part_attr_t  
            *attrs;  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
        if (!UF_CALL(UF_ATTR_ask_part_attrs_in_file(partname, &n_attrs, &attrs)))  
        {  
            if (n_attrs > 0)   
            {  
                if (*nxman)  
                    UF_CALL(UF_PART_name_for_display(partname, display_name));  
                else  
                    UF_CALL(uc4574(partname, 2, display_name));  
                WRITE(display_name);  
                WRITE(":\n");  
                for (ii = 0; ii < n_attrs; ii++)  
                {  
                    WRITE("  ");  
                    WRITE(attrs[ii].title);  
                    WRITE(" = ");  
                    WRITE(attrs[ii].string_value);  
                    WRITE("\n");  
                }  
                UF_free(attrs);  
            }  
            return TRUE;  
        }  
        else  
            return FALSE;  
    }  
    static void do_it(void)  
    {  
        logical  
            nxman;  
        tag_t  
            all_cset,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        UF_CALL(UF_ASSEM_ask_all_comp_cset(part, &all_cset));  
        UF_CALL(UF_ASSEM_apply_to_cset(all_cset, report_part_attributes, &nxman));  
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
        UF_ASSEM_options_t  
            assy_options;  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
        /*  No need to load components 里海译:不需要加载组件 */  
            UF_CALL(UF_ASSEM_ask_assem_options(&assy_options));  
            assy_options.load_components = UF_ASSEM_dont_load_components;  
            UF_CALL(UF_ASSEM_set_assem_options(&assy_options));  
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

> 这段代码是用于在NX中进行二次开发的，主要功能是遍历装配中的所有组件，并打印出每个组件的属性信息。以下是代码的主要内容和逻辑：
>
> 1. 定义了宏 UF_CALL，用于调用NX API函数，并报告错误信息。
> 2. 定义了 report_error 函数，用于打印错误信息。
> 3. 定义了 report_part_attributes 函数，作为回调函数，遍历装配中的每个组件，获取组件的属性信息并打印。
> 4. 定义了 do_it 函数，用于获取装配中的所有组件集合，然后调用 report_part_attributes 函数。
> 5. 定义了 ufusr 函数，作为NX的入口函数，调用 do_it 函数。
> 6. 定义了 main 函数，用于接收命令行参数，打开零件文件，然后调用 do_it 函数。main 函数可以处理外部模式或内部模式的调用。
> 7. 整体流程是：先初始化NX，然后打开零件文件，调用 do_it 函数遍历组件并打印属性，最后结束NX。
>
> 该代码实现了遍历组件并打印属性的功能，可以用于组件信息查询，且具有错误处理机制。
>
