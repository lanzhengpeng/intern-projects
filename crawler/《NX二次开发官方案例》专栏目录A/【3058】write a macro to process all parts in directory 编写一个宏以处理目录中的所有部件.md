### 【3058】write a macro to process all parts in directory 编写一个宏以处理目录中的所有部件

#### 代码

```cpp
    /*HEAD WRITE_A_MACRO_TO_PROCESS_ALL_PARTS_IN_DIRECTORY CCC UFUN */  
    /*  This program will take a recorded macro file containing steps to  
        be performed in each part in a directory and create a mega macro  
        which will open each part, perform those steps, then save and close  
        the part.  
        Be sure to:  
            Record macro with Reset  
            Put the template macro in the directory to be processed  
        Do NOT:  
            Include the open, save, and close steps in the template macro  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog在V18中是新增的。所以翻译为：

注：UF_print_syslog是V18中的新增功能。 */  
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
    static logical prompt_for_macro_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[UF_CFI_MAX_PATH_NAME_SIZE] = { "*.macro" };  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Macro File", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            cnt = 0,  
            resp;  
        long  
            mark;  
        char  
            a_line[MAX_LINE_SIZE+2],  
            dirspec[MAX_FSPEC_SIZE+1] = { "" },  
            fname[MAX_FSPEC_SIZE+1],  
            msg[UF_UI_MAX_STRING_LEN+1],  
            mspec[MAX_FSPEC_SIZE+1] = { "" },  
            part_name[MAX_FSPEC_SIZE+1],  
            tspec[MAX_FSPEC_SIZE+1] = { "" },  
            unique[MAX_FSPEC_SIZE+1];  
        FILE  
            *macro,  
            *template;  
        if (prompt_for_macro_file_name("Template macro file", tspec))  
        {  
            UF_CALL(uc4576(tspec, 0, dirspec, fname));  
            sprintf(unique, "all_%s", fname);  
            UF_CALL(uc4575(dirspec, 0, unique, mspec));  
            WRITE("Creating macro file - ");  
            WRITE(mspec);  
            WRITE("...\n");  
            if ((template = fopen(tspec, "r")) == NULL) return;  
            if ((macro = fopen(mspec, "w")) == NULL) return;  
        /*  Copy the header lines 里海译:复制标题行，仅提供翻译，不要添加其他内容。 */  
            while ((fgets(a_line, MAX_LINE_SIZE, template)) != NULL)  
            {  
                fprintf(macro, "%s\n", a_line);  
                a_line[5] = '\0';  
                if (!strcmp(a_line, "RESET"))  
                {  
                    mark = ftell(template);  
                    break;  
                }  
            }  
            WRITE(dirspec);  
            WRITE(":\n");  
            uc4508(dirspec, (1<<13)|(1<<11), 0, "*.prt");  
            while ((resp = uc4518()) == 0)  
            {  
                UF_CALL(uc4600(fname));  
                UF_CALL(uc4575(dirspec, 2, fname, part_name));  
                sprintf(msg, "%d.  %s\n", ++cnt, part_name);  
                WRITE(msg);  
            /*  Standard NX 4 macro lines to open a part 里海译:对不起，我无法直接执行代码或提供实时输出。您能否提供一些关于NX宏的详细信息，以便我更好地理解您的问题？ */  
                fprintf(macro,  
                    "MENU, 0, UG_FILE_OPEN UG_GATEWAY_MAIN_MENUBAR ! \n");  
                fprintf(macro, "FILE_DIALOG_BEGIN 0, ! filebox with tools_data\n");  
                fprintf(macro, "FILE_DIALOG_UPDATE 2\n");  
                fprintf(macro, "FOCUS CHANGE IN 1\n");  
                fprintf(macro, "FOCUS CHANGE OUT 1\n");  
                fprintf(macro, "FOCUS CHANGE IN 1\n");  
                fprintf(macro, "FILE_DIALOG_END\n");  
                fprintf(macro, "FILE_BOX -2, ");  
                fprintf(macro, part_name);  
                fprintf(macro, "  ");  
                fprintf(macro, dirspec);  
                fprintf(macro, "\\*.PRT" );  
                fprintf(macro, "  0 ! Open Part File\n");  
                fprintf(macro, "   SET_VALUE: 0 ! FSB item\n");  
            /*  Copy the rest of the template macro 里海译:请提供需要翻译的文本，我会为您翻译。 */  
                fseek(template, mark, SEEK_SET);  
                while ((fgets(a_line, MAX_LINE_SIZE, template)) != NULL)  
                    fprintf(macro, "%s\n", a_line);  
            /*  Standard NX 2 macro lines to save and close all parts 里海译:在 NX 2 宏中保存并关闭所有部件的标准代码行如下：


```

```cpp

上述代码将完成仪表板，遍历会话中的所有部件，保存每个部件，然后关闭它。 */  
                fprintf(macro,  
                    "MENU, 0, UG_FILE_SAVE_PART UG_GATEWAY_MAIN_MENUBAR ! \n");  
                fprintf(macro,  
                    "MENU, 0, UG_FILE_CLOSE_ALL UG_GATEWAY_MAIN_MENUBAR ! \n");  
                fprintf(macro, "FOCUS CHANGE IN 1\n");  
                fprintf(macro,  
                    "MESSAGE_BOX -2, Are you sure you want to close all parts?\n");  
            }  
            UF_CALL(uc4548());  
            sprintf(msg, "\n%s will process %d parts.\n", mspec, cnt);  
            WRITE(msg);  
            fclose(macro);  
            fclose(template);  
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

> 这段代码是一个NX Open C++宏，用于自动化处理目录下所有部件的流程。主要功能包括：
>
> 1. 提示用户输入模板宏文件路径，从中读取处理每一步骤的宏命令。
> 2. 遍历指定目录下的所有部件文件。
> 3. 对于每个部件，先打开部件，执行模板宏中的步骤，然后保存并关闭部件。
> 4. 将所有这些步骤组合成一个大的宏文件，以便一次性执行目录下所有部件的处理流程。
> 5. 用户可以指定目录和模板宏文件，程序会自动生成处理所有部件的宏文件，并提示用户执行。
>
> 代码通过NX提供的C++ API实现，使用UF_initialize()初始化API，UF_terminate()终止API，以及一系列其他函数来遍历目录、打开部件、执行宏命令等。此外，代码中还定义了UF_CALL宏来简化错误处理，以及WRITE宏来输出信息到日志窗口。整体而言，这段代码实现了自动化处理目录下所有部件的功能。
>
