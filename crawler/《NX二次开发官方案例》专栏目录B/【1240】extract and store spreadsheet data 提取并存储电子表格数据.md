### 【1240】extract and store spreadsheet data 提取并存储电子表格数据

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_xs.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    static int choose_one_item(char *prompt, char **choices, int n_choices)  
    {  
        int  
            a = 0,  
            ii,  
            resp,  
            z;  
        char  
            options[14][38];  
        do  
        {  
            z = (n_choices - a < 14) ? n_choices - a : 14;  
            for (ii = 0; ii < z; ii++)  
            {  
                strncpy(&options[ii][0], choices[a+ii], 37);  
                options[ii][37] = '\0';  
            }  
            if ((z == 14) && ((a + z) < n_choices))  
                strcpy(&options[13][0], "More...");  
            switch (resp = uc1603(prompt, 0, options, z))  
            {  
                case 1:                     /* Back 里海译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:Cancel的翻译为：取消 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:More... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:已选择一个。 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static void build_unique_temp_name(char *fspec, int ftype)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4577(unique));  
        UF_CALL(uc4575(tmp_dir, ftype, unique, fspec));  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        if (resp == 1) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            which;  
        char *choices[3] = {  
            "MODELING_SHEET",  
            "DEFAULT_SHEET",  
            "PART_FAMILY_SHEET" },  
            os_file[MAX_FSPEC_SIZE+1];  
        if ((which = choose_one_item("Spreadsheet to export?", choices, 3)) > -1)  
        {  
            build_unique_temp_name(os_file, 254);  
            if (!UF_CALL(UF_XS_extract_spreadsheet( choices[which], os_file)))  
            {  
                ECHO("Exported %s to %s\n", choices[which], os_file);  
                if (ask_yes_or_no(choices[which], "Import same file?"))  
                    UF_CALL(UF_XS_store_spreadsheet(choices[which], os_file));  
            }  
        }  
    }  
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

> 这段代码是一个NX二次开发的用户函数(ufusr)。以下是代码的主要功能和流程：
>
> 1. 函数定义：定义了几个辅助函数，包括ECHO用于打印信息，report_error用于报告错误，choose_one_item用于弹出一个选择对话框，build_unique_temp_name用于构建一个唯一的临时文件名，ask_yes_or_no用于弹出一个Yes/No确认框。
> 2. ufusr函数：首先调用UF_initialize()初始化NX，然后调用do_it()执行主要功能，最后调用UF_terminate()终止NX。do_it()函数会弹出一个选择框，让用户选择一个电子表格，然后将其导出到一个临时文件，并询问用户是否要导入这个文件。
> 3. 错误处理：使用宏UF_CALL来捕获函数调用错误，如果出错，会调用report_error报告错误。
> 4. 注释：代码中有一些中文注释，主要是对一些英文变量和函数的中文解释。
> 5. 交互式操作：通过UF_UI提供的一些交互式操作，例如打开列表窗口、弹出对话框等，实现了与用户的交互。
> 6. 文件操作：使用UF_CFI和UF_XS提供的API进行文件操作，如提取电子表格、存储电子表格等。
> 7. 整体流程：代码流程清晰，功能单一，实现了电子表格的导出和导入，并提供了交互式操作界面。
>
> 总的来说，这段代码通过调用NX提供的API，实现了与用户的交互，完成了电子表格的导出和导入操作。
>
