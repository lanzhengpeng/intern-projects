### 【3133】open parts from text file 从文本文件中打开零件代码首先提示用户选择包含零件名称的文本文件，然后...

#### 代码

```cpp
    /*HEAD OPEN_PARTS_FROM_TEXT_FILE CCC UFUN */  
    /* This will open parts that are listed one-per-row in a text file   
               that has a ".txt" extension. Make sure there is a carriage   
               return on the last line. */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的函数，用于打印系统日志。 */  
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
    static logical prompt_for_text_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1];  
        strcpy(filter, "*.txt");  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Text File Name", filter, "",   
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0,  
            jj = 0,  
            n_lines = 0;  
        tag_t  
            part,  
            orig_part = UF_PART_ask_display_part();  
        char  
            messg[300],  
            text_file[66] = { "" },  
            a_line[MAX_LINE_SIZE+1],  
            fname[MAX_FSPEC_SIZE + 1] = { "" };  
        UF_PART_load_status_t  
            status;  
        FILE *the_file;  
        if(prompt_for_text_name("Select Text File that lists Parts to Open", text_file))  
        {  
            the_file = fopen( text_file, "r+" );  
            if( the_file == NULL )  
                printf( "Could not open the file, %s\n", text_file);  
            else  
            {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL) n_lines++;  
            rewind(the_file);  
            for (ii = 0; ii < n_lines; ii++)  
            {  
                fgets(a_line, MAX_LINE_SIZE, the_file);  
                a_line[strlen(a_line) - 1] = '\0'; /* take off carriage return 里海译:去掉回车符 */  
                if (UF_PART_is_loaded(fname) != 0)  
                {  
                    sprintf(messg, "The file, \"%s\", is already open \n", fname);  
                    printf(messg);  
                    uc1601(messg, TRUE);  
                }  
                else  
                {  
                    if (UF_PART_open(fname, &part, &status))  
                    {  
                        sprintf(messg, "Could NOT open the file: \"%s\"\n", fname);  
                        printf(messg);  
                        uc1601(messg, TRUE);  
                    }  
                    else  
                    {  
                        printf( "%s opened\n", fname );   
                        jj++;  
                    }  
                }  
            }  
            fclose( the_file );  
            sprintf(messg, "%d items were listed in text file, %d were opened\n", ii, jj);  
            uc1601(messg, TRUE);  
        }  
            if (orig_part != NULL_TAG)  
                UF_CALL(UF_PART_set_display_part(orig_part));  
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

> 这段代码是用于NX的二次开发代码，其主要功能是批量打开文本文件中列出的部件。以下是代码的主要功能介绍：
>
> 1. 定义了一个错误报告函数report_error，用于在发生UF函数调用错误时输出错误信息。
> 2. 定义了一个对话框函数prompt_for_text_name，用于提示用户选择包含部件名称列表的文本文件。
> 3. 主函数do_it()用于实现打开部件的主要逻辑：调用prompt_for_text_name获取用户选择的文本文件路径。逐行读取文本文件中的部件名称，并调用UF_PART_is_loaded判断部件是否已打开。如果部件未打开，则调用UF_PART_open打开该部件。记录打开的部件数量，并在最后输出打开的部件总数。
> 4. 调用prompt_for_text_name获取用户选择的文本文件路径。
> 5. 逐行读取文本文件中的部件名称，并调用UF_PART_is_loaded判断部件是否已打开。
> 6. 如果部件未打开，则调用UF_PART_open打开该部件。
> 7. 记录打开的部件数量，并在最后输出打开的部件总数。
> 8. ufusr()函数是NX二次开发程序的入口函数，在这里初始化NX环境，调用do_it()执行打开部件操作，最后清理环境。
> 9. ufusr_ask_unload()函数用于卸载二次开发程序。
>
> 总的来说，这段代码实现了通过文本文件批量打开NX部件的功能，为NX的二次开发提供了一种便捷的部件管理方式。
>
