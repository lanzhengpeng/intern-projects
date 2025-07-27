### 【1901】report all components load status 报告所有组件的加载状态

#### 代码

```cpp
    /*HEAD REPORT_ALL_COMPONENTS_LOAD_STATUS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，可以总结如下：

UF_print_syslog 是 V18 版本新增的函数。

该函数用于在系统日志中打印信息。

使用方法是在需要打印日志的地方调用 UF_print_syslog，并传入需要打印的信息。

例如：
UF_print_syslog("这是一条日志信息");

这样会在系统日志中打印出 "这是一条日志信息"。

UF_print_syslog 适用于 V18 及以上版本。 */  
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
    static void do_it(void)  
    {  
        int  
            status;  
        tag_t  
            comp = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
                instance_name, origin, csys_matrix, transform));  
            status = UF_PART_is_loaded(part_name);  
            switch (status)  
            {  
                case 0:  
                    printf("%s - is not loaded\n", part_name);  
                    break;  
                case 1:  
                    printf("%s - is fully loaded\n", part_name);  
                    break;  
                case 2:  
                    printf("%s - is partially loaded\n", part_name);  
                    break;  
                default:  
                    printf("%s - invalid loaded status (%d)\n", part_name, status);  
                    UF_CALL(status);  
                    break;  
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

> 这是段用于查询NX部件加载状态的二次开发代码。主要功能包括：
>
> 1. 初始化和终止NX环境，使用UF_initialize和UF_terminate。
> 2. 定义错误处理函数report_error，用于在出错时打印错误信息到系统日志和对话框。
> 3. 定义函数ask_next_component，用于遍历部件中的组件，并返回下一个组件。
> 4. 定义函数do_it，用于查询每个组件的加载状态，并打印到控制台。
> 5. 定义ufusr函数，是二次开发的入口函数，调用do_it函数。
> 6. 定义ufusr_ask_unload函数，用于卸载当前打开的部件。
> 7. 定义check_load_status函数，用于打印部件加载状态信息。
> 8. 主函数main负责处理命令行参数，打开指定部件，并调用do_it函数查询加载状态。如果参数中指定了-pim，则从IMAN数据库加载部件。
>
> 通过遍历组件树，查询每个组件的加载状态，并将结果打印到控制台，实现了查询NX部件加载状态的功能。
>
