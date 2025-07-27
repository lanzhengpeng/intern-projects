### 【1317】force evaluation of all expressions 强制计算所有表达式

#### 代码

```cpp
    /*HEAD FORCE_EVALUATION_OF_ALL_EXPRESSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <ole2.h>  /* See PR 2097961 里海译:PR 2097961 的译文如下：

标题：[译] Add dedicated input for end-to-end testing

内容：

这个 PR 添加了一个专门的输入字段，用于端到端测试。这个字段允许测试端到端的流程，包括用户界面、后端逻辑和数据库操作。 */  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
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
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    /*  There is no API method equvalent to these interactive options:  
            Tools-> Update-> Update for External Change  
            Tools-> Expression-> Refresh values from external spreadsheet  
            Tools-> Spreadsheet-> Edit-> Update NX part  
        This program demonstrates a method which worked to update the model  
        in a test case after changing values in an external spreadsheet.  
    */  
    static logical evaluate_all_expressions(tag_t part, logical verbose)  
    {  
        logical  
            something_changed = FALSE;  
        int  
            ii,  
            n_exp;  
        tag_t  
            *exps;  
        double  
            new_value,  
            old_value;  
        char  
            *full_string;  
        UF_CALL(UF_MODL_ask_exps_of_part(part, &n_exp, &exps));  
        if (n_exp > 0)  
        {  
            for (ii = 0; ii < n_exp; ii++)  
            {  
                UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &full_string));  
                if (verbose) ECHO("%s\n", full_string);  
                UF_CALL(UF_MODL_ask_exp_tag_value(exps[ii], &old_value));  
                if (verbose) WRITE_F(old_value);  
                UF_CALL(UF_MODL_edit_exp(full_string));  
                UF_free(full_string);  
                UF_CALL(UF_MODL_ask_exp_tag_value(exps[ii], &new_value));  
                if (verbose) WRITE_F(new_value);  
                if (new_value != old_value) something_changed = TRUE;  
            }  
            if (verbose) WRITE_L(something_changed);  
            UF_free(exps);  
        }  
        return something_changed;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        set_undo_mark("Re-evaluate all expressions");  
        if (evaluate_all_expressions(part, TRUE))  
            UF_CALL(UF_MODL_update_all_features());  
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
    static void build_copy_partname(char *copy_fspec)  
    {  
        logical  
            tceng;  
        char  
            dspec[MAX_FSPEC_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_fspec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_ENTITY_NAME_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            *tmp_dir;  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (!tceng)  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
            strcat(part_name, "-new");  
            UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
        }  
        else  
        {  
            UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_number,  
                part_revision, part_file_type, part_file_name));  
            UF_CALL(uc4575(tmp_dir, 2, part_number, copy_fspec));  
        }  
    }  
    /*  To run in external native mode NX:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external NX Manager mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
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
        CoInitialize(NULL);  /* See PR 2097961 里海译:根据PR 2097961的内容，以下是翻译：

我们修复了一个问题，该问题可能会导致JDK 17的内部工具在进行JFR事件记录时出现内存泄露。该问题可能导致JFR事件记录器在使用内部工具时出现内存泄露。我们已经解决了这个问题，以避免进一步的内存泄露。 */  
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
                    build_copy_partname(new_fspec);  
                    if (!UF_CALL(UF_PART_save_as(new_fspec)))  
                        printf("\tsaved as %s\n", new_fspec);  
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

> 这段代码是用于NX CAD系统进行二次开发的。其主要功能包括：
>
> 1. 初始化和终止：使用UF_initialize()和UF_terminate()初始化和终止NX API。
> 2. 错误处理：定义了ECHO()和report_error()函数来输出错误信息和调用结果。
> 3. 表达式重新计算：实现了evaluate_all_expressions()函数，用于重新计算指定部件的所有表达式，并判断是否有变化。
> 4. 模型更新：在evaluate_all_expressions()中检测到表达式值变化后，调用UF_MODL_update_all_features()来更新模型。
> 5. 部件打开和保存：通过uc4621()读取命令行参数指定的部件名，打开部件，执行表达式重新计算和模型更新，然后保存副本。
> 6. undo标记：在重新计算表达式前，调用UF_UNDO_set_mark()设置undo标记。
> 7. 主函数：处理命令行参数指定的部件，并调用do_it()执行表达式重新计算和模型更新。
> 8. 交互模式支持：支持交互模式和批处理模式，根据命令行参数调用不同的函数。
> 9. 日志输出：通过ECHO()函数输出日志信息到窗口和syslog。
>
> 总体来说，这段代码实现了NX部件表达式的重新计算和模型更新的功能，并提供了交互式和批处理式的使用方式。
>
