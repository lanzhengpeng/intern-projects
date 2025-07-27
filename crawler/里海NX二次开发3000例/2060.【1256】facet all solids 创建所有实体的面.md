### 【1256】facet all solids 创建所有实体的面

#### 代码

```cpp
    /*HEAD FACET_ALL_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_facet.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是一个在 V18 中新增的函数。该函数的作用是将日志信息发送到系统日志。因此，对于UF_print_syslog在V18中的新增，建议的翻译是：“UF_print_syslog是V18版本新增的函数，用于将日志信息发送到系统日志。” */  
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
    /* qq3123197280 */  
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body = NULL_TAG,  
            facet,  
            part = UF_PART_ask_display_part();  
        UF_FACET_parameters_t  
            parms;  
        UF_CALL(UF_FACET_ask_default_parameters(&parms));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
            UF_CALL(UF_FACET_facet_solid(body, &parms, &facet));  
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
    /* qq3123197280 */  
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
    static void build_copy_partname(char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, "-new");  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    /* qq3123197280 */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            new_fspec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                if (UF_CALL(UF_PART_open(part_name, &part, &status))  
                    || status.n_parts > 0) check_load_status(&status);  
                else  
                {  
                    do_it();  
                    build_copy_partname(new_fspec);  
                    if (!UF_CALL(UF_PART_save_as(new_fspec)))  
                        printf("\tsaved as %s\n", new_fspec);  
                    UF_PART_close_all();  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这是段NX二次开发代码，主要实现了以下功能：
>
> 1. 错误报告：定义了一个错误报告函数report_error，用于在发生错误时打印错误信息和调用函数。
> 2. 循环遍历零件：定义了一个ask_next_body函数，用于循环遍历零件中的实体，直到找到下一个实体。
> 3. 默认参数设置：定义了一个do_it函数，用于获取默认参数，然后遍历每个实体，并对其实施面处理。
> 4. 初始化和调用do_it：在ufusr函数中，初始化NX环境，调用do_it函数，然后终止NX环境。
> 5. 主函数：在主函数中，首先初始化NX环境，然后循环遍历命令行传入的零件名，打开每个零件，调用do_it函数，然后保存并关闭零件。在结束前打印处理过的零件数量。
> 6. 其他函数：包含了一些辅助函数，如检查加载状态、构建新零件名称等。
>
> 总体来说，这段代码实现了NX零件的面处理功能，通过命令行参数传入零件名称，然后依次处理每个零件的面。
>
