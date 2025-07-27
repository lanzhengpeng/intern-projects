### 【1714】open specified nxmanager master part 打开指定的NX管理器主零件

#### 代码

```cpp
    /*HEAD OPEN_SPECIFIED_UGMANAGER_MASTER_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
        */  
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
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part;  
        char  
            part_number[26+1] = { "" },  
            part_revision[16+1] = { "" },  
            encoded_i[MAX_FSPEC_SIZE+1],  
            encoded_o[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        while (prompt_for_text("Enter master part number", part_number)  
            && prompt_for_text("Enter master part revision", part_revision))  
        {  
            UF_CALL(UF_UGMGR_encode_part_filename(part_number, part_revision,  
                "", "", encoded_i));  
            WRITE_S(encoded_i);  
            UF_CALL(UF_PART_open(encoded_i, &part, &status));  
            if (status.n_parts > 0) report_load_status(&status);  
            if (!status.failed)  
            {  
                UF_PART_ask_part_name(part, encoded_o);  
                WRITE_S(encoded_o);  
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

```

#### 代码解析

> 这段代码是用于NX二次开发的示例代码，主要实现以下功能：
>
> 1. 定义了错误处理宏UF_CALL，用于在调用NX API函数时检查错误，并在出错时报告错误信息。
> 2. 实现了字符串写入列表窗口的函数write_string_to_listing_window，用于向NX的列表窗口输出信息。
> 3. 定义了提示输入字符串的函数prompt_for_text，用于提示用户输入零件编号和版本号。
> 4. 定义了报告加载状态的函数report_load_status，用于输出加载零件的名称和状态。
> 5. 主函数do_it实现了以下流程：a. 不断提示用户输入零件编号和版本号，直到用户不输入为止。b. 将输入的零件编号和版本号编码成NX零件文件名。c. 尝试加载该零件文件，并输出加载状态。d. 如果加载成功，则输出零件的名称。
> 6. ufusr是NX调用的入口函数，在其中初始化NX，调用主函数do_it，然后终止NX。
> 7. ufusr_ask_unload函数用于设置卸载模式。
>
> 总体来说，这段代码通过交互式输入零件编号和版本号，实现加载对应的NX零件文件，并输出零件名称和加载状态。
>
