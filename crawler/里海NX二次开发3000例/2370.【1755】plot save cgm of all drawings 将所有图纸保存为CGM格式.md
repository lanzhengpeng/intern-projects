### 【1755】plot save cgm of all drawings 将所有图纸保存为CGM格式

#### 代码

```cpp
    /*HEAD PLOT_SAVE_CGM_OF_ALL_DRAWINGS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plot.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中内容，UF_print_syslog是V18版本新增的函数。 */  
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
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void build_temp_filespec(char *short_name, int ftype, char *fspec)  
    {  
        char  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4575(tmp_dir, ftype, short_name, fspec));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_dwgs;  
        tag_t  
            *dwgs;  
        char  
            cgm_spec[MAX_FSPEC_SIZE+1],  
            drawing_name[MAX_ENTITY_NAME_SIZE+1],  
            *job_name;  
        UF_CALL(UF_DRAW_ask_drawings(&n_dwgs, &dwgs));  
        for (ii = 0; ii < n_dwgs; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(dwgs[ii], drawing_name));  
            WRITE_S(drawing_name);  
            UF_CALL(UF_PLOT_ask_default_job_name(dwgs[ii], &job_name));  
            WRITE_S(job_name);  
            build_temp_filespec(job_name, 94, cgm_spec);  
            WRITE_S(cgm_spec);  
            UF_CALL(UF_PLOT_save_cgm(dwgs[ii], NULL, job_name, NULL, cgm_spec));  
            UF_free(job_name);  
        }  
        if (n_dwgs > 0) UF_free(dwgs);  
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
                WRITE_S(part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，主要功能是遍历当前NX会话中的所有绘图，并将它们保存为CGM文件。以下是代码的主要功能：
>
> 1. 定义了错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 2. 定义了写入信息到列表窗口的函数write_string_to_listing_window，用于输出绘图名称等信息。
> 3. 定义了构建临时文件名的函数build_temp_filespec，用于根据绘图名称和类型生成临时CGM文件名。
> 4. 主函数do_it遍历所有绘图，获取绘图名称和默认作业名，并调用UF_PLOT_save_cgm函数将每个绘图保存为CGM文件。
> 5. ufusr函数是NX调用的入口函数，初始化环境后执行do_it函数，然后清理环境。
> 6. ufusr_ask_unload函数声明了卸载方式。
> 7. check_load_status函数用于检查加载零件的状态，打印加载失败的错误信息。
> 8. main函数是程序入口，处理命令行参数，打开零件，执行do_it函数，然后关闭零件。
>
> 总体来说，这段代码实现了NX环境中所有绘图的CGM格式导出，并提供了错误处理和命令行参数支持。
>
