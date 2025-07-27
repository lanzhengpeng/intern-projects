### 【0784】create new part using seed part 打开种子部件作为新部件

#### 代码

```cpp
    /*HEAD OPEN_SEED_PART_AS_NEW_PART CCC UFUN */  
    /*  
        This program is designed to open and rename a seed part instead of  
        creating a new part when File->New is selected in native UG.  
        set environment variables CUSTOM_METRIC_SEED and CUSTOM_ENGLISH_SEED  
        to point to your metric and english seed part files respectively.  
        To implement this save the menuscript file lines below to a file using a  
        ".men" extension in a "startup" folder under a folder listed in either  
        $UGII_UG_CUSTOM_DIRECTORY_FILE or $UGII_CUSTOM_DIRECTORY_FILE.  E.g.  
        %UGII_USER_DIR%\startup  
        Then compile this C code, and place the resulting program file in  
        the corresponding "application" folder.  
        For more information on the "startup" and "application" subdirectories  
        or the UGII_CUSTOM_DIRECTORY_FILE environment variable, consult the  
        UG/Open Menuscript documentation.  
    ! Begin menufile  
    VERSION 120  
    EDIT UG_GATEWAY_MAIN_MENUBAR  
    MODIFY  
        BUTTON UG_FILE_NEW  
        ACTIONS create_new_part_using_seed_part  
    END_OF_MODIFY  
    ! End menufile  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，V18版本新增了一个名为UF_print_syslog的函数。

对于中文翻译，可以表达为：

"注意：UF_print_syslog是V18版本新增的函数。"

希望这个翻译符合要求，如果有需要修改的地方，请告诉我。 */  
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
            part;  
        char  
            filename[MAX_FSPEC_SIZE+1] = { "" },  
            *seeds[2];  
        UF_PART_load_status_t  
            status;  
        if (UF_CALL(UF_translate_variable("CUSTOM_METRIC_SEED", &seeds[0])) ||  
            UF_CALL(UF_translate_variable("CUSTOM_ENGLISH_SEED", &seeds[1])) ||  
            !file_exists(seeds[0]) || !file_exists(seeds[1])) return;  
        while (!UF_CALL(UF_UI_ask_create_part_filename(filename, &units, &resp)) &&  
              (resp == UF_UI_OK) && (file_exists(filename)))  
                uc1601("file exists, try again", TRUE);  
        if (resp == UF_UI_OK)  
        {  
            /* units returns as 1 for metric, 2 for english 译:units returns as 1 for metric, 2 for english的翻译为：单位返回值以1表示公制，以2表示英制。 */  
            UF_CALL(UF_PART_open(seeds[units-1], &part, &status));  
            if (status.n_parts > 0) report_load_status(&status);  
            if (!status.failed) UF_CALL(UF_PART_rename(part, filename));  
        }  
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

> 这是段NX二次开发的菜单脚本和C语言代码，主要实现了在NX中选择“文件”->“新建”时，不是创建一个新的空白部件，而是打开并重命名一个种子部件。具体功能包括：
>
> 1. 代码中定义了两个环境变量：CUSTOM_METRIC_SEED 和 CUSTOM_ENGLISH_SEED，用于指定公制和英制种子部件文件路径。
> 2. 菜单脚本修改了NX的“文件”菜单，将“新建”按钮的动作改为调用自定义的create_new_part_using_seed_part函数。
> 3. C语言代码实现了create_new_part_using_seed_part函数，该函数会检查两个种子部件文件是否存在，然后提示用户输入新部件的文件名，打开对应的种子部件，并重命名。
> 4. 代码还包含了错误报告函数和加载状态报告函数。
> 5. ufusr函数是NX调用的入口函数，初始化NX环境，调用create_new_part_using_seed_part函数，然后终止NX环境。
> 6. ufusr_ask_unload函数指定了NX卸载本插件的方式。
>
> 总之，这段代码通过二次开发，实现了在NX中快速打开种子部件并重命名，以提高工作效率。
>
