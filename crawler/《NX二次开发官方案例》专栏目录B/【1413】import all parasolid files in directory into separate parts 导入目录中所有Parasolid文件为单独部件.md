### 【1413】import all parasolid files in directory into separate parts 导入目录中所有Parasolid文件为单独部件

#### 代码

```cpp
    /*HEAD IMPORT_ALL_PARASOLID_FILES_IN_DIRECTORY_INTO_SEPARATE_PARTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ps.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
    static void ask_import_directory(char *fspec)  
    {  
        char  
            *p_dir;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_IMPORT_DIR, &p_dir));  
        strcpy(fspec, p_dir);  
        UF_free(p_dir);  
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
    static void convert_to_new_partname(char *old_fspec, int old_type, char *addon, char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(uc4576(old_fspec, old_type, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, addon);  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
    }  
    static void do_it(void)  
    {  
        int  
            cnt = 0;  
        uf_list_p_t  
            body_list;  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "" },  
            fname[MAX_FSPEC_SIZE+1],  
            lbb[MAX_FSPEC_SIZE+1],  
            fspec[MAX_FSPEC_SIZE+1],  
            messg[150];  
        tag_t  
            part = NULL_TAG;  
        ask_import_directory(dirspec);  
        if (prompt_for_text("Import parasolid files*Enter directory path", dirspec))  
        {  
            uc4508(dirspec, (1<<11)|(1<<13), 0, "*.x*_*");  
            while (!uc4518())  
            {  
                uc4600(fname);  
                uc4575(dirspec, 0, fname, fspec);  
                sprintf(messg, "(%d) Importing from %s...\n", ++cnt, fspec);  
                WRITE(messg);  
                convert_to_new_partname(fspec, 0, "", lbb);  
                UF_CALL(UF_PART_new(lbb,UF_PART_ENGLISH,&part));  
                if (!UF_CALL(UF_PS_import_data(fspec, &body_list)))  
                {  
                    UF_CALL(UF_MODL_delete_list(&body_list));  
                    sprintf(messg, "(%d) Saving  %s...\n", cnt, lbb);  
                    WRITE(messg);  
                    UF_CALL(UF_PART_save());  
                    UF_CALL(UF_PART_close(part, 0, 0));  
                }  
                else  
                    break;  
            }  
            UF_CALL(uc4548());  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误报告函数：定义了report_error函数，用于打印出函数调用失败的错误代码和消息。
> 2. 目录选择：ask_import_directory函数用于打开文件对话框，让用户选择一个目录。
> 3. 提示输入文本：prompt_for_text函数用于提示用户输入文本，并返回用户是否输入了文本。
> 4. 转换文件名：convert_to_new_partname函数用于将旧文件名转换为新文件名，并拼接后缀。
> 5. 主体逻辑：do_it函数实现了主体逻辑，包括遍历指定目录下的所有Parasolid文件，将每个文件导入NX作为单独的部件，并保存为新文件。
> 6. UFusr函数：ufusr函数是NX的入口函数，用于初始化和终止NX环境，并调用do_it函数。
> 7. 卸载请求：ufusr_ask_unload函数用于请求立即卸载。
>
> 通过以上功能，该代码实现了将一个目录下所有Parasolid文件导入NX，并将每个文件保存为单独部件的功能。
>
