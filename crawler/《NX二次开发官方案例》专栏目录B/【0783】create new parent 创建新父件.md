### 【0783】create new parent 创建新父件

#### 代码

```cpp
    /*HEAD CREATE_NEW_PARENT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中的新功能。 */  
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
    static logical file_exists(char *file_spec)  
    {  
        int  
            in_session,  
            missing;  
        UF_CALL(UF_CFI_ask_file_exist(file_spec, &missing));  
        in_session = UF_PART_is_loaded(file_spec);  
        if (missing && (in_session != 1) && (in_session != 2))  
            return FALSE;  
        else  
            return TRUE;  
    }  
    static void do_it(void)  
    {  
        int  
            units = NONE,  
            resp;  
        tag_t  
    cur_part = UF_PART_ask_display_part(),  
    instance,  
            part;  
        char  
            cur_name[MAX_FSPEC_SIZE+1] = { "" },  
            filename[MAX_FSPEC_SIZE+1] = { "" },  
            seeds[2][MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
    double  
            origin[3] = { 0, 0, 0 },  
            matrix[6] = { 1,0,0, 0,1,0 };  
    UF_CALL(UF_PART_ask_part_name(cur_part, cur_name));  
    /*  
        if (UF_CALL(UF_translate_variable("CUSTOM_METRIC_SEED", &seeds[0])) ||  
            UF_CALL(UF_translate_variable("CUSTOM_ENGLISH_SEED", &seeds[1])) ||  
            !file_exists(seeds[0]) || !file_exists(seeds[1])) return;  
    */  
    strcpy(seeds[0], "z:\\parts\\mmpart.prt");  
    strcpy(seeds[1], "z:\\parts\\inpart.prt");  
        while (!UF_CALL(UF_UI_ask_create_part_filename(filename, &units, &resp)) &&  
              (resp == UF_UI_OK) && (file_exists(filename)))  
                uc1601("file exists, try again", TRUE);  
        if (resp == UF_UI_OK)  
        {  
            /* units returns as 1 for metric, 2 for english 译:units returns as 1 for metric, 2 for english */  
            UF_CALL(UF_PART_open(seeds[units-1], &part, &status));  
            if (status.n_parts > 0) report_load_status(&status);  
            if (!status.failed) UF_CALL(UF_PART_rename(part, filename));  
        }  
        UF_CALL(UF_ASSEM_add_part_to_assembly(part, cur_name, NULL,  
            NULL, origin, matrix, 0, &instance, &status));  
        if (status.n_parts > 0) report_load_status(&status);  
    }  
    /* 里海 */  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 错误报告函数：report_error，用于输出错误信息到系统日志和控制台。
> 2. 加载状态报告函数：report_load_status，用于输出部件加载状态。
> 3. 文件存在检测函数：file_exists，用于检测种子文件是否存在。
> 4. 主要函数：do_it，用于创建新部件。获取当前显示的部件名。获取种子文件路径。交互式获取新部件文件名。以种子文件打开新部件。重命名新部件。将新部件添加到当前装配体。
> 5. 获取当前显示的部件名。
> 6. 获取种子文件路径。
> 7. 交互式获取新部件文件名。
> 8. 以种子文件打开新部件。
> 9. 重命名新部件。
> 10. 将新部件添加到当前装配体。
> 11. ufusr函数：作为NX的二次开发入口，初始化NX系统后调用do_it函数，并在结束后关闭NX系统。
> 12. 卸载函数：ufusr_ask_unload，返回立即卸载标志，表示二次开发应用退出时立即卸载。
>
> 这段代码主要演示了NX部件的打开、重命名和添加到装配体的二次开发流程。
>
